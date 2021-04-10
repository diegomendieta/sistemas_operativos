#pragma once

typedef struct queue {
    int system_queues;
    int program_input;
    int priority;
    int quantum;
} Queue;

Queue* queue_init(int system_queues, int program_input, int priority);