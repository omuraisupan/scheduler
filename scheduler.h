#pragma once

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

int Set_FIFOqueue(Process **head, Process **tail, int N, char *input_file);
void Move_Nextqueue(Process **head, double *elapsedTime);
int Dequeue(Process **head, double *totalTurnAroundTime, double *elapsedTime, int *i);
void Search_Min_Processing(Process **head, double elapsedTime);
void Search_Arrivequeue(Process **head, Process **tail, double elapsedTime);
double First_Come_First_Serve(Process **head, int N);
double Shortest_Job_Next(Process **head, int N);
double Round_Robin(Process **head, Process **tail, double timeSlice, int N);