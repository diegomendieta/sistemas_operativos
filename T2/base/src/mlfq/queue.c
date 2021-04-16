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
    return queue;
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
    process -> cycles_for_wait = 0;
    process -> cycles_waiting = 0;
    process -> next = NULL;
    process -> quantum = queue -> quantum;
    process -> priority = queue -> priority;
    queue -> tail = process;
}

int execute_next_process(Queue* queue)
{
    printf("\nqueue -> priority: %i", queue -> priority);
    execute_process(queue -> head);
    Process* head_process = queue -> head;
    if (head_process -> state == 3)
    {
        queue -> head = head_process -> next;
        if (queue -> head != NULL)
        {
            queue -> head -> prev = NULL;
        }
        else
        {
            queue -> tail = NULL;
        }
        return 3;
    }
    else if (head_process -> state == 2)
    {
        queue -> head = head_process -> next;
        if (queue -> head != NULL)
        {
            queue -> head -> prev = NULL;
        }
        else
        {
            queue -> tail = NULL;
        }
        return 2;
    }
    else if (head_process -> state == 1)
    {
        queue -> head = head_process -> next;
        if (queue -> head != NULL)
        {
            queue -> head -> prev = NULL;
        }
        else
        {
            queue -> tail = NULL;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

void queue_destroy(Queue* queue)
{
    free(queue);
}