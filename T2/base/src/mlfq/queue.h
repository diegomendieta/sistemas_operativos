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
int execute_next_process(Queue* queue);