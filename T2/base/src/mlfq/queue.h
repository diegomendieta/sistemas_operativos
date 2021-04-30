#pragma once

#include "process.h"

typedef struct queue {
    int system_queues;
    int program_input;
    int priority;
    int quantum;
    Process* head;
    Process* tail;
} Queue;

Queue* queue_init(int system_queues, int program_input, int priority);

void add_process(Queue* queue, Process* process);

void add_process_afterwait(Queue* queue, Process* process);

void add_process_afterquantum(Queue* queue, Process* process);

void add_waiting_process(Queue* queue, Process* process);

Process* choose_queue(Queue* queue);

int execute_next_process(Queue* queue, int total_program_time);

void queue_destroy(Queue* queue);
