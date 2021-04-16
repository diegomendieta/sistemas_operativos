#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "process.h"

Process* process_init(int PID, char* name, int tiempo_inicio, int cycles, int wait, int waiting_delay)
{
    Process* process = calloc(1, sizeof(Process));
    process -> PID = PID;
    process -> name = name;
    process -> quantum = 0; /* VER CÓMO CONECTAR CON ATRIBUTOS DEL QUEUE */
    process -> priority = 0; /* ARREGLAR */
    process -> state = 1;
    process -> tiempo_inicio = tiempo_inicio;
    process -> cycles = cycles;
    process -> wait = wait;
    process -> waiting_delay = waiting_delay;
    process -> prev = NULL;
    process -> next = NULL;
    process -> total_cycles = 0;
    process -> cycles_for_wait = 0;
    process -> cycles_waiting = 0;
    process -> turnos_cpu = 0;
    return process;
}

void execute_process(Process* process)
{
    printf("\nprocess_%i -> quantum: %i", process -> PID, process -> quantum);
    if (process -> state == 1)
    {
        process -> state = 0;
        process -> turnos_cpu = process -> turnos_cpu + 1;
    }
    process -> cycles_for_wait = process -> cycles_for_wait + 1;
    printf("\nprocess_%i -> total_cycles: %i", process -> PID, process -> total_cycles);
    process -> total_cycles = process -> total_cycles + 1;
    process -> quantum = process -> quantum - 1;
    if (process -> total_cycles == process -> cycles)
    {
        process -> state = 3;
    }
    else if (process -> cycles_for_wait == process -> wait && process -> wait != 0)
    {
        process -> cycles_for_wait = 0;
        process -> state = 2;
    }
    else if (process -> quantum == 0)
    {
        process -> state = 1;
    }
}

int process_wait(Process* process)
{
    process -> cycles_waiting = process -> cycles_waiting + 1;
    if (process -> cycles_waiting == process -> waiting_delay)
    {
        process -> state = 1;
        process -> cycles_waiting = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

void output_process(Process* process, FILE* output_file)
{
    fprintf(output_file, "%s,%i,%i,%i,%i,%i\n", process -> name, process -> turnos_cpu, process -> turnos_cpu, process ->tiempo_inicio, process -> turnos_cpu, process ->tiempo_inicio);
}

void process_destroy(Process* process)
{
    free(process);
}