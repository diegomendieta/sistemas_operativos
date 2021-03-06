#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "process.h"

Process* process_init(int PID, char* name, int tiempo_inicio, int cycles,
                      int wait, int waiting_delay)
{
    /* initial memory allocation */
    Process* process = calloc(1, sizeof(Process));

    /* initial attributes given by input file */
    process -> PID = PID;
    process -> name = name;
    process -> quantum = 0;
    process -> tiempo_inicio = tiempo_inicio;
    process -> cycles = cycles;
    process -> wait = wait;
    process -> waiting_delay = waiting_delay;

    /* atributos variables según estado y ubicación de proceso */
    process -> priority = -1;
    process -> state = -1;

    /* punteros hacia procesos previo y siguiente en cola */
    process -> next = NULL;

    /* contadores de ciclos de CPU y wait */
    process -> total_cycles = 0;
    process -> cycles_for_wait = 0;
    process -> cycles_waiting = 0;
    process -> turnos_cpu = 0;

    /* atributos calculados para el output */
    process -> n_interruptions = 0;
    process -> turnaround_time = 0;
    process -> response_time = 0;
    process -> waiting_time = 0;

    return process;
}

void execute_process(Process* process, int total_program_time)
{
    printf("\nejecutando proceso: %s", process -> name);
    printf(
        "\nprocess -> PID: %i | cycles_for_wait: %i | wait: %i | quantum: %i | total_cycles: %i | cycles needed: %i",
        process -> PID,
        process -> cycles_for_wait,
        process -> wait, process -> quantum,
        process -> total_cycles,
        process -> cycles
    );

    /* Si proceso pasa de estado READY a RUNNING */
    if (process -> state == 1)
    {
        if (process -> turnos_cpu == 0) 
            process -> response_time = total_program_time - process -> tiempo_inicio;
        
        process -> state = 0;
        process -> turnos_cpu = process -> turnos_cpu + 1;
    }
    process -> cycles_for_wait = process -> cycles_for_wait + 1;
    process -> total_cycles = process -> total_cycles + 1;
    process -> quantum = process -> quantum - 1;

    /* Si proceso culmina sus cycles totales y pasa a FINISHED */
    if (process -> total_cycles == process -> cycles)
    {
        if (process -> quantum == 0) (process -> n_interruptions)++;

        process -> state = 3;
        process -> turnaround_time = total_program_time - process -> tiempo_inicio + 1;
    }

    /* Si el proceso tiene que pasar a estado WAIT */
    else if (process -> cycles_for_wait == process -> wait && process -> wait != 0)
    {
        process -> cycles_for_wait = 0;
        process -> state = 2;
    }

    /* Si el proceso agota su quantum */
    else if (process -> quantum == 0)
    {
        (process -> n_interruptions)++;
        process -> state = 1;
    }
}

int process_wait(Process* process)
{
    /* Si el proceso esta en estado WAIT */
    if (process -> state == 2)
    {
        process -> cycles_waiting = process -> cycles_waiting + 1;

        /* Si el proceso terminó su tiempo de WAIT */
        if (process -> cycles_waiting == process -> waiting_delay)
        {
            process -> state = 1;
            process -> waiting_time = process -> waiting_time + process -> cycles_waiting;
            process -> cycles_waiting = 0;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    /* Si el proceso está en estado READY */
    else if (process -> state == 1)
    {
        process -> waiting_time++;
        return 0;
    }
    else return 0;
}

void output_process(Process* process, FILE* output_file)
{
    fprintf(
        output_file,
        "%s,%i,%i,%i,%i,%i\n",
        process -> name,
        process -> turnos_cpu,
        process -> n_interruptions,
        process -> turnaround_time,
        process -> response_time,
        process -> waiting_time
    );
}

void queue_print(Process* process)
{
    printf("<- %s", process -> name);
    if (process -> next != NULL) queue_print(process -> next);
}

void process_destroy(Process* process)
{
    if (process -> PID != -1) free(process -> name);
    free(process);
}