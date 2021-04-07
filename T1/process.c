#include <sys/types.h>
// #include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#include "process.h"


// Ejecuta un proceso de tipo Worker a partir de la línea de un archivo.
void execWorker(char** start_line){

    int status;

    char* exec = start_line[1];
    const int n = atoi(start_line[2]);

    char* args[n];
    for (int counter = 0; counter < n; counter++){
        args[counter] = start_line[3 + counter];
        printf("arg[%d]: %s\n", counter, args[counter]);
    }

    printf("Exec: %s\n", exec);
    printf("n: %d\n", n);

    pid_t child_pid = fork();

    // Proceso hijo.
    if (child_pid == 0){
        printf("Ejecutando el proceso hijo...");
        execve(exec);
    }

    // Proceso padre.
    else if (child_pid > 0){
        printf("Ejecutando el proceso padre...");
        wait(&status);
    }
}

// Ejecuta un proceso de tipo Manager Root.
void execManagerRoot(){
}

// Ejecuta un proceso de tipo Manager No Root.
void execManagerNotRoot(){
}