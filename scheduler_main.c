#include <stdio.h>
#include "scheduler.h"

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