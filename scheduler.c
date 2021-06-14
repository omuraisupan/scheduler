#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int arrive;
    int processingTime;
    int startProcessing;
    int endProcessing;
    int turnAroundTime;
    int next;
    int prev;
} Process;

int Set_Process(Process *process, int N, char *input_file) {
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
        process[i].turnAroundTime = 0;
        process[i].next = -1;
        process[i].prev = -1;
    }
    fclose(fp);
    return i;
}

int Insert_Arrive(Process *process, int first, int *last, int insert) {
    int tmp;
    if (process[*last].arrive > process[insert].arrive) {
        tmp = process[insert].next;
        process[insert].next = *last;
        process[insert].prev = process[*last].prev;
        process[*last].next = tmp;
        process[*last].prev = insert;
        if (*last == first) {
            return insert;
        } else {
            Insert_Arrive(process, first, &process[insert].prev, insert);
        }
    } else {
        process[insert].prev = *last;
        process[*last].next = insert;
        *last = insert;
        return first;
    }
}

int Insert_Processing(Process *process, int first, int *last, int insert) {
    int tmp;
    if (process[*last].processingTime > process[insert].processingTime) {
        tmp = process[insert].next;
        process[insert].next = *last;
        process[insert].prev = process[*last].prev;
        process[*last].next = tmp;
        process[*last].prev = insert;
        if (*last == first) {
            return insert;
        } else {
            Insert_Processing(process, first, &process[insert].prev, insert);
        }
    } else {
        process[insert].prev = *last;
        process[*last].next = insert;
        *last = insert;
        return first;
    }
}

void Sorting_Order_Processing(Process *process, int done) {
    int first = process[done].next;
    int last = first;
    int insert = process[last].next;
    while (process[done].endProcessing >= process[insert].arrive && last != -1 && insert != -1) {
        first = Insert_Processing(process, first, &last, insert);
        process[done].next = first;
        last = insert;
        insert = process[last].next;
    }
}

int Set_Order(Process *process, int N) {
    int i, j;
    int last = 0, next = 1, first = 0;
    process[0].next = 1;
    process[0].prev = -1;
    for (i = 1; i < N; i++) {
        first = Insert_Arrive(process, first, &last, i);
    }
    return first;
}

double First_Come_First_Serve(Process *process, int N, int first) {
    int i;
    int now = first, prev = 0;
    int total = 0;
    double ave;
    for (i = 0; i < N; i++) {
        printf("%d番目に早い到着はP%d\n", i, now);
        if (process[now].arrive > process[prev].endProcessing) {
            process[now].startProcessing = process[now].arrive;
        } else {
            process[now].startProcessing = process[prev].endProcessing;
        }
        process[now].endProcessing = process[now].startProcessing + process[now].processingTime;
        process[now].turnAroundTime = process[now].endProcessing - process[now].arrive;
        total += process[now].turnAroundTime;
        printf("P%dの到着時間 = %d\n", now, process[now].arrive);
        printf("P%dの処理開始時間 = %d\n", now, process[now].startProcessing);
        printf("P%dの処理終了時間 = %d\n", now, process[now].endProcessing);
        printf("P%dの応答時間 = %d\n", now, process[now].turnAroundTime);
        prev = now;
        now = process[now].next;
    }
    ave = (double)total / (double)N;
    return ave;
}

double Shortest_Job_Next(Process *process, int N, int first) {
    int i;
    int now = first, prev = 0;
    int sortedPoint;
    int total = 0;
    double ave;
    for (i = 0; i < N; i++) {
        if (process[now].arrive > process[prev].endProcessing) {
            process[now].startProcessing = process[now].arrive;
        } else {
            process[now].startProcessing = process[prev].endProcessing;
        }
        process[now].endProcessing = process[now].startProcessing + process[now].processingTime;
        process[now].turnAroundTime = process[now].endProcessing - process[now].arrive;
        total += process[now].turnAroundTime;
        printf("P%dの到着時間 = %d\n", now, process[now].arrive);
        printf("P%dの処理開始時間 = %d\n", now, process[now].startProcessing);
        printf("P%dの処理終了時間 = %d\n", now, process[now].endProcessing);
        printf("P%dの応答時間 = %d\n", now, process[now].turnAroundTime);
        if (process[now].next != -1) Sorting_Order_Processing(process, now);
        prev = now;
        now = process[now].next;
    }
    ave = (double)total / (double)N;
    return ave;
}

void Round_Robin(Process *process, int N, int timeslice) {
    int i;
    int total = 0;
    double ave;
    process[0].startProcessing = process[0].arrive;
    if (timeslice < process[0].processingTime) {
        process[0].turnAroundTime += timeslice;
    }
    for (i = 0; i < N; i++) {

    }
}

int main() {
    int N, sel, timeslice, first;
    int real_N;
    printf("How many processes ? > ");
    scanf("%d", &N);
    Process *process = (Process *)malloc(sizeof(Process) * N);
    if (process == NULL) {
        printf("MEMORY ALLOCATION ERROR\n");
        return -1;
    }
    real_N = Set_Process(process, N, "input.csv");
    first = Set_Order(process, real_N);

    printf("How do you want to calculate?\n1:First Come First Serve\n2:Shortest Job Next\n3:Round Robin\n> ");
    scanf("%d", &sel);
    if (sel == 1) {
        printf("%f\n", First_Come_First_Serve(process, real_N, first));
    }
    else if (sel == 2) {
        printf("%f\n", Shortest_Job_Next(process, real_N, first));
    }
    else if(sel == 3) {
        printf("How long is Timeslice? > ");
        scanf("%d", &timeslice);
        Set_Process(process, N, "input.csv");
    }
    return 0;
}
