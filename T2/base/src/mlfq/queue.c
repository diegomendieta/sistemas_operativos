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

    /* Calculo de quantum según fórmula */
    queue -> quantum = (queue -> system_queues - queue -> priority) * queue -> program_input;
    printf(
        "\nQUEUE priority: %i | quantum: %i",
        queue -> priority,
        queue -> quantum
    );
    queue -> head = NULL;
    queue -> tail = NULL;
    return queue;
}

void add_process(Queue* queue, Process* process)
{
    process -> state = 1;
    if (queue -> head == NULL && queue -> tail == NULL)
    {
        queue -> head = process;
    }
    else
    {
        queue -> tail -> next = process;
    }
    process -> cycles_for_wait = 0;
    process -> cycles_waiting = 0;
    process -> next = NULL;
    process -> quantum = queue -> quantum;
    process -> priority = queue -> priority;
    queue -> tail = process;
}

void add_process_afterwait(Queue* queue, Process* process)
{
    /* Se comporta igual que el add_process, pero no renueva el quantum */
    process -> state = 1;
    if (queue -> head == NULL && queue -> tail == NULL)
    {
        queue -> head = process;
    }
    else
    {
        queue -> tail -> next = process;
    }
    process -> next = NULL;
    process -> priority = queue -> priority;
    queue -> tail = process;
}

void add_process_afterquantum(Queue* queue, Process* process)
{
    /* Se comporta igual que el add process, pero no renueva el tiempo hasta
    tener que pasar a estado WAIT */
    process -> state = 1;
    if (queue -> head == NULL && queue -> tail == NULL)
    {
        queue -> head = process;
    }
    else
    {
        queue -> tail -> next = process;
    }
    process -> quantum = queue -> quantum;
    process -> next = NULL;
    process -> priority = queue -> priority;
    queue -> tail = process;
}

void add_waiting_process(Queue* queue, Process* process)
{
    /* Agregamos proceso que se encuentra en estado de WAIT */
    if (queue -> head == NULL && queue -> tail == NULL)
    {
        queue -> head = process;
    }
    else
    {
        queue -> tail -> next = process;
    }
    if (process -> quantum == 0) process -> quantum = queue -> quantum;
    process -> next = NULL;
    process -> priority = queue -> priority;
    queue -> tail = process;
}

Process* choose_queue(Queue* queue)
{
    Process* current_process = queue -> head;
    while (current_process != NULL)
    {
        if (current_process -> state == 1)
        {
            return current_process;
        }
        current_process = current_process -> next;
    }
    return NULL;
}

int execute_next_process(Queue* queue, int total_program_time)
{
    Process* executing_process = queue -> head;
    Process* previous_process = queue -> head;
    while (executing_process -> state == 2)
    {
        previous_process = executing_process;
        executing_process = executing_process -> next;
    }
    execute_process(executing_process, total_program_time);
    Process* head_process = queue -> head;
    if (head_process == executing_process)
    {
        if (head_process -> state == 3)
        {
            queue -> head = head_process -> next;
            if (queue -> head == NULL)
            {
                queue -> tail = NULL;
            }
            return 3;
        }
        else if (head_process -> state == 2)
        {
            queue -> head = head_process -> next;
            if (queue -> head == NULL)
            {
                queue -> tail = NULL;
            }
            return 2;
        }
        else if (head_process -> state == 1)
        {
            queue -> head = head_process -> next;
            if (queue -> head == NULL)
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
    else
    {
        if (executing_process -> state == 3)
        {
            previous_process -> next = executing_process -> next;
            executing_process -> next = NULL;
            return 3;
        }
        else if (executing_process -> state == 2)
        {
            previous_process -> next = executing_process -> next;
            executing_process -> next = NULL;
            return 2;
        }
        else if (executing_process -> state == 1)
        {
            previous_process -> next = executing_process -> next;
            executing_process -> next = NULL;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    
    /*execute_process(queue -> head, total_program_time);
    Process* head_process = queue -> head;
    if (head_process -> state == 3)
    {
        queue -> head = head_process -> next;
        if (queue -> head == NULL)
        {
            queue -> tail = NULL;
        }
        return 3;
    }
    else if (head_process -> state == 2)
    {
        queue -> head = head_process -> next;
        if (queue -> head == NULL)
        {
            queue -> tail = NULL;
        }
        return 2;
    }
    else if (head_process -> state == 1)
    {
        queue -> head = head_process -> next;
        if (queue -> head == NULL)
        {
            queue -> tail = NULL;
        }
        return 1;
    }
    else
    {
        return 0;
    }*/
}

void queue_destroy(Queue* queue)
{
    free(queue);
}