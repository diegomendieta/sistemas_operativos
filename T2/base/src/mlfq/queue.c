#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

Queue* queue_init(int system_queues, int program_input, int priority)
{
    Queue* queue = calloc(1, sizeof(Queue));
    queue -> system_queues = system_queues;
    queue -> program_input = program_input;
    queue -> priority = 0;
    queue -> quantum = (queue -> system_queues - queue -> priority) * queue -> program_input;
}