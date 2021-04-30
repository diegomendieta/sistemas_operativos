#pragma once

typedef struct process {
    int PID;
    char* name;
    int quantum;
    int tiempo_inicio;
    int cycles;
    int wait;
    int waiting_delay;
    int priority;
    int state;
    struct process* next;
    int total_cycles;
    int cycles_for_wait;
    int cycles_waiting;
    int turnos_cpu;
    int n_interruptions;
    int turnaround_time;
    int response_time;
    int waiting_time;
} Process;

Process* process_init(int PID, char* name, int tiempo_inicio, int cycles,
                      int wait, int waiting_delay);

void execute_process(Process* process, int total_program_time);

void output_process(Process* process, FILE* output_file);

int process_wait(Process* process);

void queue_print(Process* process);

void process_destroy(Process* process);
