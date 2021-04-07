#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "worker.h"


/*
Inicializa un proceso Worker con un nombre determinado. Los procesos Worker
ejecutan programas y llevan estadísticas.
*/
Worker* initWorker(char* name){
    Worker* worker = calloc(sizeof(Worker), 1);
    worker -> name = name;
    return worker;
}

/* Genera una línea de output de un proceso Worker. */
char* generateOutputLine(Worker* worker){
    char* output_line;
    return output_line;
}
