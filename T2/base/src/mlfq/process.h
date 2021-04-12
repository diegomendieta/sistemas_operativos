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
    int time_running;
    int turnos_cpu;
} Process;

Process* process_init(int PID, char* name, int tiempo_inicio, int cycles, int wait, int waiting_delay);
void execute_process(Process* process);
