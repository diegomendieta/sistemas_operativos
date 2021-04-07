#pragma once

/* Representa un proceso Worker. */
typedef struct worker {
    char* name;
    float exec_time;
    int return_code;
    int interrupted;
} Worker;

/* Inicializa un Worker con un nombre determinado. */
Worker* initWorker(char* name);
