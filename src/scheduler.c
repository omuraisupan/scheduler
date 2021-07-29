#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

void Write_Result(Process *process, char *result_file) {
    FILE *fp;
    fp = fopen(result_file, "a");
    if (fp == NULL) {
        printf("READ_FILE_ERROR\n");
        exit(1);
    }
    fprintf(fp, "P%d, %f, %f, %f, %f\n", process->number, process->createTime, 
            process->startProcessingTime, process->endProcessingTime, process->turnAroundTime);
    fclose(fp);
}

int Set_FIFOqueue(Process **head, Process **tail, int N, char *input_file) {
    int i = 0;
    double arrive, processingTime;
    Process *new_process, *now;
    FILE *fp;
    fp = fopen(input_file, "r");
    if (fp == NULL) {
        printf("READ_FILE_ERROR\n");
        exit(1);
    }
    for (i = 0; fscanf(fp, "%lf, %lf", &arrive, &processingTime) == 2 && N > i; i++) {
        new_process = (Process *)malloc(sizeof(Process));
        if (new_process == NULL) {
            printf("MEMORY ALLOCATION ERROR\n");
            exit(1);
        }
        new_process->number = i + 1;
        new_process->createTime = arrive;
        new_process->arrive = arrive;
        new_process->processingTime = processingTime;
        new_process->startProcessingTime = -1.0;
        new_process->endProcessingTime = -1.0;
        new_process->turnAroundTime = -1.0;
        if (i == 0) {
            new_process->next = NULL;
            new_process->prev = NULL;
            *head = new_process;
            *tail = new_process;
        } else {
            now = *head;
            while (now != NULL && now->arrive <= new_process->arrive) {
                now = now->next;
            }
            if (now == NULL) {
                (*tail)->next = new_process;
                new_process->prev = *tail;
                *tail = new_process;
            }
            else if (now->arrive > new_process->arrive) {
                new_process->next = now;
                new_process->prev = now->prev;
                if (now->prev == NULL) {
                    *head = new_process;
                } else {
                    (now->prev)->next = new_process;
                }
                now->prev = new_process;
            }
        }
    }
    fclose(fp);
    return i;
}

void Move_Nextqueue(Process **head, double *elapsedTime) {
    if ((*head)->startProcessingTime < 0) {
        if ((*head)->arrive > *elapsedTime) {
            (*head)->startProcessingTime = (*head)->arrive;
        } else {
            (*head)->startProcessingTime = *elapsedTime;
        }
    }
}

int Dequeue(Process **head, double *totalTurnAroundTime, double *elapsedTime, int *i, char *result_file) {
    if ((*head)->arrive > *elapsedTime) {
        (*head)->endProcessingTime = (*head)->arrive + (*head)->processingTime;
    } else {
        (*head)->endProcessingTime = *elapsedTime + (*head)->processingTime;
    }
    (*head)->turnAroundTime = (*head)->endProcessingTime - (*head)->createTime;
    *totalTurnAroundTime += (*head)->turnAroundTime;
    *elapsedTime = (*head)->endProcessingTime;

    Write_Result(*head, result_file);

    if ((*head)->next != NULL) {
        *head = (*head)->next;
        free((*head)->prev);
        (*head)->prev = NULL;
        (*i)++;
        return 1;
    } else {
        return 0;
    }
}

void Search_Min_Processing(Process **head, double elapsedTime) {
    Process *i, *min;
    min = *head;
    for (i = *head; i != NULL; i = i->next) {
        if (i->arrive <= elapsedTime && i->processingTime < min->processingTime) {
            min = i;
        }
    }
    if (min != *head) {
        min->prev->next = min->next;
        if (min->next != NULL) {
            min->next->prev = min->prev;
        }
        (*head)->prev = min;
        min->prev = NULL;
        min->next = *head;
        *head = min;
    }
}

void Search_Arrivequeue(Process **head, Process **tail, double elapsedTime) {
    Process *i;
    for (i = *head; i != NULL; i = i->next) {
        if (i->arrive > elapsedTime) {
            if (i != *head) {
                *tail = i->prev;
                return;
            } else break;
        }
        *tail = i;
    }
}

double First_Come_First_Serve(Process **head, int N, char *result_file) {
    int i = 1;
    double totalTurnAroundTime = 0.0, aveTrunAroundTime, elapsedTime = 0.0;
    elapsedTime = (*head)->arrive;

    while (1) {
        Move_Nextqueue(head, &elapsedTime);
        if (Dequeue(head, &totalTurnAroundTime, &elapsedTime, &i, result_file) == 0) break;
    }
    aveTrunAroundTime = totalTurnAroundTime / (double)N;
    return aveTrunAroundTime;
}


double Shortest_Job_Next(Process **head, int N, char *result_file) {
    int i = 1;
    double totalTurnAroundTime = 0.0, aveTurnAroundTime, elapsedTime = 0.0;
    elapsedTime = (*head)->arrive;
    while (1) {
        Search_Min_Processing(head, elapsedTime);
        Move_Nextqueue(head, &elapsedTime);
        if (Dequeue(head, &totalTurnAroundTime, &elapsedTime, &i, result_file) == 0) break;
    }
    aveTurnAroundTime = totalTurnAroundTime / (double)N;
    return aveTurnAroundTime;
}

double Round_Robin(Process **head, Process **tail, double timeSlice, int N, char *result_file) {
    int i = 1;
    double totalTurnAroundTime = 0.0, aveTurnAroundTime, elapsedTime = (*head)->arrive;
    Process *tmp;
    while (1) {
        Move_Nextqueue(head, &elapsedTime);
        if ((*head)->processingTime > timeSlice) {
            (*head)->processingTime -= timeSlice;
            (*head)->arrive = elapsedTime;
            Search_Arrivequeue(head, tail, elapsedTime + timeSlice);
            if (*head != *tail) {
                if ((*tail)->next != NULL) {
                    (*tail)->next->prev = *head;
                }
                tmp = (*head)->next;
                (*head)->next->prev = NULL;
                (*head)->next = (*tail)->next;
                (*head)->prev = *tail;
                (*tail)->next = *head;
                *head = tmp;
            }
            elapsedTime += timeSlice;
        } else {
            if (Dequeue(head, &totalTurnAroundTime, &elapsedTime, &i, result_file) == 0) break;
        }
    }
    aveTurnAroundTime = totalTurnAroundTime / (double)N;
    return aveTurnAroundTime;
}
