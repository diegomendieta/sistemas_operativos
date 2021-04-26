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
  int n_finished_processes = 0;
  fscanf(input_file, "%d", &n_processes);

  int n_queues = atoi(argv[3]);

  int program_input = atoi(argv[4]);

  int total_program_time = 0;
  int program_time_to_s = 0;
  int program_time_s = atoi(argv[5]);

  Queue** system_queues = calloc(n_queues, sizeof(Queue*));
  Process** system_processes = calloc(n_processes, sizeof(Process*));

  for (int i = 0; i < n_queues; i++)
  {
    system_queues[i] = queue_init(n_queues, program_input, i);
  }
  for (int i = 0; i < n_processes; i++)
  {
    char* nombre_proceso;
    nombre_proceso = (char *)malloc(sizeof(char)*32);
    int pid;
    int tiempo_inicio;
    int cycles;
    int wait;
    int waiting_delay;

    fscanf(input_file, "%s", nombre_proceso);
    fscanf(input_file, "%d", &pid);
    fscanf(input_file, "%d", &tiempo_inicio);
    fscanf(input_file, "%d", &cycles);
    fscanf(input_file, "%d", &wait);
    fscanf(input_file, "%d", &waiting_delay);
    

    printf("\nnombre_proceso: %s", nombre_proceso);
    Process* process = process_init(pid, nombre_proceso, tiempo_inicio, cycles, wait, waiting_delay);
    printf("\nprocess -> name: %s", process -> name);
    system_processes[i] = process;
    printf("\nprocess -> name: %s", system_processes[i] -> name);
    printf("\nprocess -> PID: %i | process: %p", process -> PID, process);
    printf("\nnombre_proceso: %s", nombre_proceso);
  }

  for (int id_p = 0; id_p < n_processes; id_p++)
  {
    printf("\n5process -> name: %s | process -> PID: %i", system_processes[id_p] -> name, system_processes[id_p] -> PID);
  }

  printf("\nn_finished_processes: %i", n_finished_processes);
  while (n_finished_processes < n_processes)
  {
    printf("\nSE RENUEVA TIEMPO S");
    while (program_time_to_s < program_time_s)
    {
      Queue* queue;
      for (int id_q = 0; id_q < n_queues; id_q++)
      {
        if (system_queues[id_q] -> priority == 0)
        {
          queue = system_queues[id_q];
        }
      }
      
      for (int id_p = 0; id_p < n_processes; id_p++)
      {
        if (system_processes[id_p] -> tiempo_inicio == total_program_time)
        {
          printf("\n5process -> name: %s | process -> PID: %i", system_processes[id_p] -> name, system_processes[id_p] -> PID);
          Process* process = system_processes[id_p];
          printf("\n6process -> name: %s", process -> name);
          add_process(queue, process);
          printf("\n7process -> name: %s", process -> name);
        }
      }
      Queue* actual_queue = NULL;
      for (int id_q = 0; id_q < n_queues; id_q++)
      {
        printf("\nsystem_queues[id_q] -> priority: %i | system_queues[id_q] -> head: %p", system_queues[id_q] -> priority, system_queues[id_q] -> head);
        printf("\nsystem_queues[id_q] -> priority: %i | system_queues[id_q] -> tail: %p", system_queues[id_q] -> priority, system_queues[id_q] -> tail);
        if (system_queues[id_q] -> head != NULL)
        {
          actual_queue = system_queues[id_q];
          break;
        }
      }
      
      if (actual_queue != NULL)
      {
        Process* process_tobe_executed = actual_queue -> head;
        int result_execution = execute_next_process(actual_queue, total_program_time);

        if (result_execution == 3)
        {
          printf("\nPROCESS FINISHED");
          n_finished_processes = n_finished_processes + 1;
        }
        else if (result_execution == 1)
        {
          int new_priority = actual_queue -> priority + 1;
          for (int id_q = 0; id_q < n_queues; id_q++)
          {
            if (system_queues[id_q] -> priority == new_priority)
            {
              add_process(system_queues[id_q], process_tobe_executed);
              printf("\nprocess -> name: %s | process -> PID: %i | process -> priority: %i", process_tobe_executed -> name, process_tobe_executed -> PID, process_tobe_executed -> priority);
            }
          }
        }
      }
      for (int id_p = 0; id_p < n_processes; id_p++)
      {
        if (system_processes[id_p] -> state == 2 || system_processes[id_p] -> state == 1)
        {
          int finishes_wait = process_wait(system_processes[id_p]);
          if (finishes_wait == 1)
          {
            for (int id_q = 0; id_q < n_queues; id_q++)
            {
              if (system_queues[id_q] -> priority == 0)
              {
                add_process(system_queues[id_q], system_processes[id_p]);
              }
            }
          }
        }
      }
      program_time_to_s = program_time_to_s + 1;
      total_program_time = total_program_time + 1;
    }
    Queue* queue;
    for (int id_q = 1; id_q < n_queues; id_q++)
    {
      queue = system_queues[id_q];
      while (queue -> head != NULL)
      {
        Process* new_head = queue -> head -> next;
        add_process(system_queues[0], queue -> head);
        queue -> head = new_head;
      }
    }
    program_time_to_s = 0;
  }

  for (int id_p = 0; id_p < n_processes; id_p++){
    printf("\nprocess -> name: %s | process -> PID: %i", system_processes[id_p] -> name, system_processes[id_p] -> PID);
    output_process(system_processes[id_p], output_file);
  }
  for (int id_p = 0; id_p < n_processes; id_p++){
    process_destroy(system_processes[id_p]);
  }
  for (int id_q = 0; id_q < n_queues; id_q++){
    queue_destroy(system_queues[id_q]);
  }
  fclose(input_file);
  fclose(output_file);
  free(system_queues);
  free(system_processes);

}
