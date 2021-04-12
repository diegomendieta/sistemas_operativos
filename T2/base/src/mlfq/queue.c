#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

Queue* queue_init(int system_queues, int program_input, int priority)
{
    Queue* queue = calloc(1, sizeof(Queue));
    queue -> system_queues = system_queues;
    queue -> program_input = program_input;
    queue -> priority = priority;
    queue -> quantum = (queue -> system_queues - queue -> priority) * queue -> program_input;
    queue -> head = NULL;
    queue -> tail = NULL;
}

void add_process(Queue* queue, Process* process)
{
    if (queue -> head == NULL && queue -> tail == NULL)
    {
        process -> prev = NULL;
        queue -> head = process;
    }
    else
    {
        process -> prev = queue -> tail;
        queue -> tail -> next = process;
    }
    process -> next = NULL;
    process -> quantum = queue -> quantum;
    process -> priority = queue -> priority;
    queue -> tail = process;
}

int execute_next_process(Queue* queue)
{
    execute_process(queue -> head);
    if (queue -> head -> time_running == queue -> head -> wait)
    {
        queue -> head = queue -> head -> next;
        queue -> head -> prev = NULL;
        return 2;
    }
    else if (queue -> head -> quantum == 0)
    {
        queue -> head = queue -> head -> next;
        queue -> head -> prev = NULL;
        return 1;
    }
    else if (queue -> head -> cycles == 0)
    {
        queue -> head = queue -> head -> next;
        queue -> head -> prev = NULL;
        return 3;
    }
}