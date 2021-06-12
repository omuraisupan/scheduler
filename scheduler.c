#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int arrive;
    int processingTime;
    int startProcessing;
    int endProcessing;
    int timeslice;
    int turnAroundTime;
} Process;

int Set_Process(Process *process, int N, char *input_file, int timeslice) {
    int i;
    int arrive, processingTime;
    FILE *fp;
    fp = fopen(input_file, "r");
    if (fp == NULL) {
        printf("READ_FILE_EREOR\n");
        return -1;
    }
    for (i = 0; fscanf(fp, "%d, %d", &arrive, &processingTime) != EOF && N > i; i++) {
        process[i].arrive = arrive;
        process[i].processingTime = processingTime;
        process[i].startProcessing = 0;
        process[i].endProcessing = 0;
        process[i].timeslice = timeslice;
        process[i].turnAroundTime = 0;
    }
    fclose(fp);
    return i;
}

void Sort_Arrive(Process *process, int N) {

}

void Sort_ProcessingTime (Process *process, int N) {

}

double First_Come_First_Serve(Process *process, int N) {
    int i;
    int total = 0;
    double ave;
    process[0].startProcessing = process[0].arrive;
    process[0].endProcessing = process[0].startProcessing + process[0].processingTime;
    process[0].turnAroundTime = process[0].endProcessing - process[0].arrive;
    total += process[0].turnAroundTime;
    for (i = 1; i < N; i++) {
        if (process[i].arrive > process[i - 1].endProcessing) {
            process[i].startProcessing = process[i].arrive;
        } else {
            process[i].startProcessing = process[i - 1].endProcessing;
        }
        process[i].endProcessing = process[i].startProcessing + process[i].processingTime;
        process[i].turnAroundTime = process[i].endProcessing - process[i].arrive;
        total += process[i].turnAroundTime;
    }
    ave = (double)total / (double)N;
    return ave;
}

void Shortest_Remaining_Tiem(Process *process, int N) {
    int i;
    int total;
    double ave;
    Sort_ProcessingTime(process, N);
}

void Round_Robin() {

}

int main() {
    int N, sel, timeslice;
    int real_N;
    printf("How many processes ? > ");
    scanf("%d", &N);
    Process *process = (Process *)malloc(sizeof(Process) * N);
    printf("How do you want to calculate?\n1:First Come First Serve\n2:Shortest Remaining Time\n3:Round-Robin\n> ");
    scanf("%d", &sel);
    if (sel == 1) {
        real_N = Set_Process(process, N, "input.csv", 0);
        Sort_Arrive(process, real_N);
        printf("%f\n", First_Come_First_Serve(process, real_N));
    }
    else if (sel == 2) {
        real_N = Set_Process(process, N, "input.csv", 0);
    }
    else if(sel == 3) {
        printf("How long is Timeslice? > ");
        scanf("%d", &timeslice);
        Set_Process(process, N, "input.csv", timeslice);
    }
    return 0;
}