#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


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

  while (n_processes)
  {
    
  }

}
