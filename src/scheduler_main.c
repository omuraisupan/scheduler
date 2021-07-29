#include <stdio.h>
#include <stdio.h>
#include "scheduler.h"

int main() {
    int N, sel, real_N;
    double timeslice;
    Process *head, *tail;
    FILE *fp;
    printf("How many processes ? > ");
    scanf("%d", &N);
    real_N = Set_FIFOqueue(&head, &tail, N, "input.csv");
    printf("How do you want to calculate?\n1:First Come First Serve\n2:Shortest Job Next\n3:Round Robin\n> ");
    scanf("%d", &sel);
    fp = fopen("result_scheduler_simulator.csv", "w");
    fprintf(fp, "number, arrive, start, end, turnaround\n");
    fclose(fp);
    if (fp = NULL) {
        printf("WRITE_FILE_ERROR\n");
        return 0;
    }
    if (sel == 1) {
        printf("\n*********\naverage turnaround time is %f\n", First_Come_First_Serve(&head, real_N, "result_scheduler_simulator.csv"));
    }
    else if (sel == 2) {
        printf("\n*********\naverage turnaround time is %f\n", Shortest_Job_Next(&head, real_N, "result_scheduler_simulator.csv"));
    }
    else if(sel == 3) {
        printf("How long is Timeslice? > ");
        scanf("%lf", &timeslice);
        printf("\n*********\naverage turnaround time is %f\n", Round_Robin(&head, &tail, timeslice, real_N, "result_scheduler_simulator.csv"));
    }
    return 0;
}