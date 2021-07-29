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

void Write_Result(Process *process, char *resul_file);
int Set_FIFOqueue(Process **head, Process **tail, int N, char *input_file);
void Move_Nextqueue(Process **head, double *elapsedTime);
int Dequeue(Process **head, double *totalTurnAroundTime, double *elapsedTime, int *i, char *result_file);
void Search_Min_Processing(Process **head, double elapsedTime);
void Search_Arrivequeue(Process **head, Process **tail, double elapsedTime);
double First_Come_First_Serve(Process **head, int N, char *result_file);
double Shortest_Job_Next(Process **head, int N, char *result_file);
double Round_Robin(Process **head, Process **tail, double timeSlice, int N, char *result_file);