#include <stdio.h>
#include <stdlib.h>

typedef struct _process{
    int number;
    double createTime;
    double arrive;
    double processingTime;
    double startProcessingTime;
    double endProcessingTime;
    double turnAroundTime;
    struct _process *next;
    struct _process *prev;
} Process;

void Print_Queue(Process *head) {
    Process *i;
    for (i = head; i != NULL; i = i->next) {
        printf("P%d (%p) next:(%p) prev:P(%p)\n", i->number, i, i->next, i->prev);
    }
}

int Set_FIFOqueue(Process **head, Process **tail, int N, char *input_file) {
    int i = 0;
    double arrive, processingTime;
    Process *new_process, *now;
    FILE *fp;
    fp = fopen(input_file, "r");
    if (fp == NULL) {
        printf("READ_FILE_EREOR\n");
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

void dequeue(Process **head, int *totalTurnAroundTime, double *elapsedTime) {
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

double First_Come_First_Serve(Process **head, int N) {
    int i = 1;
    double totalTurnAroundTime, aveTrunAroundTime, elapsedTime;
    elapsedTime = (*head)->arrive;

    while (1) {
        printf("\n%d番目に早い処理終了はP%d\n", i, (*head)->number);
        if ((*head)->arrive > elapsedTime) {
            (*head)->startProcessingTime = (*head)->arrive;
        } else {
            (*head)->startProcessingTime = elapsedTime;
        }
        (*head)->endProcessingTime = (*head)->startProcessingTime + (*head)->processingTime;
        (*head)->turnAroundTime = (*head)->endProcessingTime - (*head)->arrive;
        totalTurnAroundTime += (*head)->turnAroundTime;
        elapsedTime = (*head)->endProcessingTime;

        printf("\n%d番目に処理終了はP%d\n", i, (*head)->number);
        printf("P%dの到着時間 = %f\n", (*head)->number, (*head)->arrive);
        printf("P%dの処理開始時間 = %f\n", (*head)->number, (*head)->startProcessingTime);
        printf("P%dの処理終了時間 = %f\n", (*head)->number, (*head)->endProcessingTime);
        printf("P%dの応答時間 = %f\n", (*head)->number, (*head)->turnAroundTime);

        if ((*head)->next != NULL) {
            *head = (*head)->next;
            free((*head)->prev);
            (*head)->prev = NULL;
        } else {
            break;
        }
        i++;
    }
    aveTrunAroundTime = totalTurnAroundTime / (double)N;
    return aveTrunAroundTime;
}


double Shortest_Job_Next(Process **head, int N) {
    int i = 1;
    double totalTurnAroundTime, aveTurnAroundTime, elapsedTime;
    elapsedTime = (*head)->arrive;
    while (1) {
        Search_Min_Processing(head, elapsedTime);
        if ((*head)->arrive > elapsedTime) {
            (*head)->startProcessingTime = (*head)->arrive;
        } else {
            (*head)->startProcessingTime = elapsedTime;
        }
        (*head)->endProcessingTime = (*head)->startProcessingTime + (*head)->processingTime;
        (*head)->turnAroundTime = (*head)->endProcessingTime - (*head)->arrive;
        totalTurnAroundTime += (*head)->turnAroundTime;
        elapsedTime = (*head)->endProcessingTime;

        printf("\n%d番目に処理終了はP%d\n", i, (*head)->number);
        printf("P%dの到着時間 = %f\n", (*head)->number, (*head)->arrive);
        printf("P%dの処理開始時間 = %f\n", (*head)->number, (*head)->startProcessingTime);
        printf("P%dの処理終了時間 = %f\n", (*head)->number, (*head)->endProcessingTime);
        printf("P%dの応答時間 = %f\n", (*head)->number, (*head)->turnAroundTime);

        if ((*head)->next != NULL) {
            *head = (*head)->next;
            free((*head)->prev);
            (*head)->prev = NULL;
        } else {
            break;
        }
        i++;
    }
    aveTurnAroundTime = totalTurnAroundTime / (double)N;
    return aveTurnAroundTime;
}

double Round_Robin(Process **head, Process **tail, double timeSlice, int N) {
    int i = 1;
    double totalTurnAroundTime, aveTurnAroundTime, elapsedTime = (*head)->arrive;
    Process *tmp;
    while (1) {
        if ((*head)->startProcessingTime < 0) {
            if ((*head)->arrive > elapsedTime) {
                (*head)->startProcessingTime = (*head)->arrive;
            } else {
                (*head)->startProcessingTime = elapsedTime;
            }
        }
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
            if ((*head)->arrive > elapsedTime) {
                (*head)->endProcessingTime = (*head)->arrive + (*head)->processingTime;
            } else {
                (*head)->endProcessingTime = elapsedTime + (*head)->processingTime;
            }
            (*head)->turnAroundTime = (*head)->endProcessingTime - (*head)->createTime;
            totalTurnAroundTime += (*head)->turnAroundTime;
            elapsedTime = (*head)->endProcessingTime;

            printf("\n%d番目に処理終了はP%d\n", i, (*head)->number);
            printf("P%dの到着時間 = %f\n", (*head)->number, (*head)->createTime);
            printf("P%dの処理開始c時間 = %f\n", (*head)->number, (*head)->startProcessingTime);
            printf("P%dの処理終了時間 = %f\n", (*head)->number, (*head)->endProcessingTime);
            printf("P%dの応答時間 = %f\n", (*head)->number, (*head)->turnAroundTime);

            if ((*head)->next != NULL) {
            *head = (*head)->next;
            free((*head)->prev);
            (*head)->prev = NULL;
            } else {
                break;
            }
            i++;
        }
    }
    aveTurnAroundTime = totalTurnAroundTime / (double)N;
    return aveTurnAroundTime;
}

int main() {
    int N, sel, real_N;
    double timeslice;
    Process *head, *tail;
    printf("How many processes ? > ");
    scanf("%d", &N);
    real_N = Set_FIFOqueue(&head, &tail, N, "input.csv");
    printf("How do you want to calculate?\n1:First Come First Serve\n2:Shortest Job Next\n3:Round Robin\n> ");
    scanf("%d", &sel);
    if (sel == 1) {
        printf("\n\n*********\n平均応答時間は%f\n", First_Come_First_Serve(&head, real_N));
    }
    else if (sel == 2) {
        printf("\n\n*********\n平均応答時間は%f\n", Shortest_Job_Next(&head, real_N));
    }
    else if(sel == 3) {
        printf("How long is Timeslice? > ");
        scanf("%lf", &timeslice);
        printf("\n\n*********\n平均応答時間は%f\n", Round_Robin(&head, &tail, timeslice, real_N));
    }
    return 0;
}
