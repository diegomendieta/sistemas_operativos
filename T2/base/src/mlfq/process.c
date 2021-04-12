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
    process -> time_running = 0;
    process -> turnos_cpu = 0;
}

void execute_process(Process* process)
{
    if (process -> state == 1)
    {
        process -> state = 0;
        process -> turnos_cpu = process -> turnos_cpu + 1;
    }
    process -> time_running = process -> time_running + 1;
    process -> quantum = process -> quantum - 1;
    process -> cycles = process -> cycles - 1;

}