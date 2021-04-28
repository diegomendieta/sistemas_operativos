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

  /* Numero de procesos totales que recibirá el sistema */
  int n_processes;

  /* Contador de procesos en estado FINISHED */
  int n_finished_processes = 0;
  fscanf(input_file, "%d", &n_processes);

  /* Numero de colas */
  int n_queues = atoi(argv[3]);

  /* q minúscula para cálculo de quantum de cada cola */
  int program_input = atoi(argv[4]);

  /* Contador de "tiempo real" del sistema */
  int total_program_time = 0;

  /* Contador de "tiempo real" desde última vez que se llegó al tiempo S */
  int program_time_to_s = 0;

  /* Tiempo S */
  int program_time_s = atoi(argv[5]);

  /* Todas las colas del sistema */
  Queue** system_queues = calloc(n_queues, sizeof(Queue*));

  /* Todos los procesos del sistema */
  Process** system_processes = calloc(n_processes, sizeof(Process*));

  /* Creamos todas las colas y las agregamos a nuestro array de colas del sistema */
  for (int i = 0; i < n_queues; i++)
  {
    system_queues[i] = queue_init(n_queues, program_input, (n_queues - i - 1));
  }

  /* Creamos todos los procesos (sin iniciarlos todavía) para guardarlos en nuestro array de procesos del sistema */
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

  /* Condición que permite que siga corriendo el código mientras no se terminen TODOS los procesos */
  while (n_finished_processes < n_processes)
  {
    /* Guarda todos aquellos procesos que agotaron su quantum en la última cola, por lo que deben esperar el tiempo S para volver a tener tiempo de CPU */
    Process** bottom_processes = malloc(n_processes * sizeof(Process*));

    /* Número de procesos que agotan su quantum en la última cola */
    int n_bottom_processes = 0;
    printf("\nSE RENUEVA TIEMPO S");

    /* Condición que permite pasar los procesos a la primera cola cada vez que no se llega al tiempo S */
    while (program_time_to_s < program_time_s)
    {
      /* Tomamos la cola de mayor prioridad para luego ingresar ahí los procesos que inicien en el t (total_program_time) actual */
      Queue* queue;
      for (int id_q = 0; id_q < n_queues; id_q++)
      {
        if (system_queues[id_q] -> priority == (n_queues) - 1)
        {
          queue = system_queues[id_q];
        }
      }
      
      /* Todos aquellos procesos cuyo t de inicio (proceso -> tiempo_inicio) sea igual al t actual (total_program_time) se añaden a la cola de mayor prioridad */
      for (int id_p = 0; id_p < n_processes; id_p++)
      {
        if (system_processes[id_p] -> tiempo_inicio == total_program_time && system_processes[id_p] -> state == -1)
        {
          Process* process = system_processes[id_p];
          add_process(queue, process);
        }
      }

      /* Encontramos la cola de mayor prioridad que tenga al menos un proceso en su interior en estado READY */
      /* Así, sabemos que en el t actual, este proceso será el dueño de la CPU */
      Queue* actual_queue = NULL;
      for (int id_q = 0; id_q < n_queues; id_q++)
      {
        printf("\nqueue PRIORITY: %i | HEAD: %p | TAIL: %p", system_queues[id_q] -> priority, system_queues[id_q] -> head, system_queues[id_q] -> tail);
        if (id_q == 0) printf("\nqueue PRIORITY: %i | HEAD: %p | TAIL: %p", system_queues[id_q + 1] -> priority, system_queues[id_q + 1] -> head, system_queues[id_q + 1] -> tail);
        if (system_queues[id_q] -> head != NULL && choose_queue(system_queues[id_q]) != NULL)
        {
          actual_queue = system_queues[id_q];
          break;
        }
      }

      /* En el caso de tener un proceso que ya se encontraba en estado RUNNING anteriormente, encontramos la cola a la cual estaba
      asociada este proceso, dejándola a esta como el indicador de que proceso ejecutar (o continuar ejecutando) en la CPU
      dejando sin efecto lo hecho en el paso anterior */
      for (int id_q = 0; id_q < n_queues; id_q++)
      {
        if (system_queues[id_q] -> head != NULL)
        {
          if (system_queues[id_q] -> head -> state == 0)
          {
            actual_queue = system_queues[id_q];
            break;
          }
        }
      }
      
      /* Ejecutamos el proceso que se encontraba en primer lugar de la cola previamente seleccionada
      (proceso encontrado o que ya estaba corriendo) */
      Process* process_tobe_executed;
      Process* fake_process = process_init(-1, "fake_process", -1, 0, 0, 0);
      if (actual_queue != NULL)
      {
        process_tobe_executed = actual_queue -> head;
        while (process_tobe_executed -> state == 2)
        {
          process_tobe_executed = process_tobe_executed -> next;
        }
        int result_execution = execute_next_process(actual_queue, total_program_time);

        /* Obtenemos el resultado de la ejecución del proceso, donde pasa solamente una unidad de tiempo */

        /* 
          result_execution == 3 : FINISHED
          result_execution == 2 : WAITING
          result_execution == 1 : READY (porque se acabó su quantum y baja de prioridad)
        */

        if (result_execution == 3)
        {
          printf("\nPROCESS FINISHED");
          n_finished_processes = n_finished_processes + 1;
        }
        else if (result_execution == 1)
        {
          int new_priority = actual_queue -> priority - 1;

          /* Si prioridad pasó de 0 a -1, significa que acabó su quantum en la cola de menor prioridad
          , por lo que debe esperar a que pase la condición de tiempo S */
          if (new_priority == -1)
          {
            bottom_processes[n_bottom_processes] = process_tobe_executed;
            n_bottom_processes++;
          }

          /* Encontramos la cola que corresponda a su nueva prioridad y agregamos el proceso al final de dicha cola */
          else
          {
            for (int id_q = 0; id_q < n_queues; id_q++)
            {
              if (system_queues[id_q] -> priority == new_priority)
              {
                add_process_afterquantum(system_queues[id_q], process_tobe_executed);
                printf("\nprocess -> name: %s | process -> PID: %i | process -> priority: %i", process_tobe_executed -> name, process_tobe_executed -> PID, process_tobe_executed -> priority);
              }
            }
          }
        }
        else if (result_execution == 2)
        {
          for (int id_q = 0; id_q < n_queues; id_q++)
          {
            /* Si su prioridad ya es máxima, entonces no sube de prioridad */
            if (process_tobe_executed -> priority == n_queues - 1) 
            {
              /* Si al pasar a estado de WAIT también agotó su quantum, renovamos su quantum */
              add_waiting_process(system_queues[0], process_tobe_executed);
            }
            else if (system_queues[id_q] -> priority == process_tobe_executed -> priority + 1)
            {
              add_waiting_process(system_queues[id_q], process_tobe_executed);
            }
          }
        }
      }
      else process_tobe_executed = fake_process;
      printf("\n");
      for (int id_q = 0; id_q < n_queues; id_q++)
      {
        printf("\nQUEUE -> PRIORITY: %i", system_queues[id_q] -> priority);
        if (system_queues[id_q] -> head != NULL) queue_print(system_queues[id_q] -> head);
      }
      printf("\nWAITING: ");
      for (int id_p = 0; id_p < n_processes; id_p++)
      {
        if (system_processes[id_p] -> state == 2)
          printf("%s, tpo_restante: %i | ", system_processes[id_p] -> name, system_processes[id_p] -> waiting_delay - system_processes[id_p] -> cycles_waiting);
      }
      printf("\n");

      /* Recorremos nuestro array de procesos totales encontrando aquellos que se encuentran en estado WAIT (= 2) o READY (= 1)
      para simular que pasa una unidad de tiempo en ese estado */
      for (int id_p = 0; id_p < n_processes; id_p++)
      {
        printf("\nprocess ID: %i | state: %i | priority: %i | waiting time: %i", system_processes[id_p] -> PID, system_processes[id_p] -> state, system_processes[id_p] -> priority, system_processes[id_p] -> waiting_time);
        if (system_processes[id_p] -> state == 2 || system_processes[id_p] -> state == 1)
        {
          int finishes_wait = -1;
          if (system_processes[id_p] != process_tobe_executed) finishes_wait = process_wait(system_processes[id_p]);
          /* Si el valor de retorno de process_wait es = 1, sabemos que el proceso culminó su etapa de WAIT y pasa a estar READY */
          if (finishes_wait == 1)
          {
            printf("\n%s terminó WAIT, tiene prioridad: %i", system_processes[id_p] -> name, system_processes[id_p] -> priority);
          }
        }
      }
      process_destroy(fake_process);
      printf("\nprogram_time_to_s: %i", program_time_to_s);
      printf("\ntotal_program_time: %i", total_program_time);

      /* Aumentamos en uno nuestras variables de "tiempo real" */
      program_time_to_s = program_time_to_s + 1;
      total_program_time = total_program_time + 1;
    }

    /* Revisamos todas las queues pasando todos sus procesos a la queue de prioridad más alta */
    
    Queue* queue;
    for (int id_q = 1; id_q < n_queues; id_q++)
    {
      int corriendo = 0;
      int continuar = 0;
      Process* proceso_corriendo;
      queue = system_queues[id_q];
      while (queue -> head != NULL && continuar == 0)
      {
        printf("\nproceso enviado a primera cola: %s", queue -> head -> name);
        printf("\nnew head: %p", queue -> head -> next);
        if (queue -> head -> state == 0)
        {
          corriendo = 1;
          proceso_corriendo = queue -> head;
          queue -> head = queue -> head -> next;
        }
        else
        {
          Process* current_process = queue -> head;
          queue -> head = queue -> head -> next;
          if (current_process -> state == 2) add_waiting_process(system_queues[0], current_process);
          else add_process_afterquantum(system_queues[0], current_process);
        }
        printf("\nqueue -> head: %p", queue -> head);
        printf("\ncontinuar: %i", continuar);
      }
      queue -> tail = NULL;
      if (corriendo == 1)
      {
        queue -> head = proceso_corriendo;
        queue -> head -> next = NULL;
        queue -> tail = proceso_corriendo;
        continuar = 1;
      }
    }

    /* Añadimos a la cola de prioridad más alta todos aquellos procesos que se quedaron sin quantum
    en la cola de menor prioridad */
    for (int i = 0; i < n_bottom_processes; i++)
    {
      add_process(system_queues[0], bottom_processes[i]);
    }

    /* Renovamos el contador de tiempo hasta S */
    program_time_to_s = 0;
    free(bottom_processes);
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
