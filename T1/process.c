#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process.h"


// Ejecuta un proceso de tipo Worker a partir de la línea de un archivo.
void execWorker(char** start_line){

    int status;

    char* to_exec = start_line[1];
    const int n = atoi(start_line[2]);

    char* args[n+2];
    args[0] = to_exec;
    for (int counter = 1; counter <= n; counter++){
        args[counter] = start_line[3 + (counter - 1)];
        printf("arg[%d]: %s\n", counter, args[counter]);
    }
    args[n+1] = NULL;

    printf("Exec: %s\n", to_exec);
    printf("n: %d\n", n);

    pid_t child_pid = fork();

    // Proceso hijo.
    if (child_pid == 0){
        printf("Ejecutando el proceso hijo...");
        execvp(args[0], args);
        printf("Esta línea no debería mostrarse.");
        exit(0);
    }

    // Proceso padre.
    else if (child_pid > 0){
        printf("Ejecutando el proceso padre...\n");
        wait(&status);
        printf("Código de salida del proceso hijo: %d\n", WEXITSTATUS(status));
        exit(0);
    }
}

// Ejecuta un proceso de tipo Manager Root.
void execManagerRoot(){
}

// Ejecuta un proceso de tipo Manager No Root.
void execManagerNotRoot(){
}