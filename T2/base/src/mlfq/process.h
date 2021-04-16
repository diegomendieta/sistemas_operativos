#pragma once

typedef struct process {
    int PID;
    char* name;
    int quantum;
    int priority;
    int state;
    int tiempo_inicio;
    int cycles;
    int wait;
    int waiting_delay;
    struct process* prev;
    struct process* next;
    int total_cycles;
    int cycles_for_wait;
    int cycles_waiting;
    int turnos_cpu;
} Process;

Process* process_init(int PID, char* name, int tiempo_inicio, int cycles, int wait, int waiting_delay);
void execute_process(Process* process);
void output_process(Process* process, FILE* output_file);
int process_wait(Process* process);
void process_destroy(Process* process);
