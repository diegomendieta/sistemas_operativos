#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "queue.h"


/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char **argv)
{
  if (argc != 6)
  {
    printf("Modo de uso: %s INPUT OUTPUT Q q S\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    printf("\tQ es la cantidad de colas que tendrá el programa\n");
    printf("\tq es la variable usada para el cálculo de los quantums de las cola\n");
    printf("\tS es el período en el cual los procesos pasan a la cola de mayor prioridad\n");
    return false;
  }

  return true;
}


int main(int argc, char **argv)
{
  if (!check_arguments(argc, argv))
  {
    return 1;
  }

  /* Abrimos el archivo de input */
  FILE *input_file = fopen(argv[1], "r");

  /* Abrimos el archivo de output */
  FILE *output_file = fopen(argv[2], "w");

  int n_processes;
  fscanf(input_file, "%d", &n_processes);

  int n_queues = atoi(argv[3]);

  int program_input = atoi(argv[4]);

  int program_time = 0;
  int program_time_s = atoi(argv[5]);

  Queue** system_queues = calloc(n_queues, sizeof(Queue*));
  Process** system_processes = calloc(n_processes, sizeof(Process*));

  for (int i = 0; i < n_queues; i++)
  {
    Queue* queue = queue_init(n_queues, program_input, i);
    system_queues[i] = queue;
  }

  for (int i = 0; i < n_processes; i++)
  {
    char nombre_proceso[32];
    int pid;
    int tiempo_inicio;
    int cycles;
    int wait;
    int waiting_delay;

    fscanf(input_file, "%d", &nombre_proceso);
    fscanf(input_file, "%d", &pid);
    fscanf(input_file, "%d", &tiempo_inicio);
    fscanf(input_file, "%d", &cycles);
    fscanf(input_file, "%d", &wait);
    fscanf(input_file, "%d", &waiting_delay);

    Process* process = process_init(pid, nombre_proceso, tiempo_inicio, cycles, wait, waiting_delay);
    system_processes[i] = process;
  }

  while (program_time < program_time_s)
  {
    Queue* queue;
    for (int id_q = 0; id_q < n_queues; id_q++)
    {
      if (system_queues[id_q] -> priority == 0)
      {
        queue = system_queues[id_q];
      }
    }
    Process* process;
    for (int id_p = 0; id_p < n_processes; id_p++)
    {
      if (system_processes[id_p] -> tiempo_inicio == program_time)
      {
        process = system_processes[id_p];
        add_process(queue, process);
      }
    }
    for (int id_q = 0; id_q < n_queues; id_q++)
    {
      if (system_queues[id_q] -> head != NULL && system_queues[id_q] -> tail != NULL)
      {
        queue = system_queues[id_q];
      }
    }
    int return_value = execute_next_process(queue);
  }

}
