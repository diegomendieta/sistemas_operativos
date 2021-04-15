#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process.h"


// Ejecuta un proceso de tipo Worker. Recibe un archivo y el índice del proceso
// a ejecutar.
void execWorker(InputFile* file, int process){
    printf("Ejecutando proceso Worker...\n");
    char** line = file -> lines[process];
    // linea: id,exec,n,arg_1,...,arg_n

    char* to_exec = line[1];
    const int n = atoi(line[2]);

    int status;

    char* args[n+2];
    args[0] = to_exec;
    for (int counter = 1; counter <= n; counter++){
        args[counter] = line[3 + (counter - 1)];
        // printf("arg[%d]: %s\n", counter, args[counter]);
    }
    args[n+1] = NULL;

    // printf("Exec: %s\n", to_exec);
    // printf("n: %d\n", n);

    pid_t pid, own_pid;
    pid = fork();
    own_pid = getpid();

    // Proceso padre.
    if (pid > 0){
        printf("Ejecutando el proceso padre[%d]...\n", own_pid);
        wait(&status);
        printf("Código de salida del proceso hijo: %d\n", WEXITSTATUS(status));

        /*
        Falta generar output.
        */
        
        printf("Cerrando el proceso padre[%d].\n", own_pid);
        exit(0);
    }

    // Proceso hijo.
    else if (pid == 0){
        pid_t parent_pid = getppid();
        printf("Ejecutando el proceso hijo[%d] con padre[%d]...\n",
                own_pid, parent_pid);
        printf("Archivo: %s\n", args[0]);
        execvp(args[0], args);
        
        // Agregamos print para chequear si hay errores.
        printf("ESTA LÍNEA NO DEBERÍA MOSTRARSE.\n");
        exit(1);
    }
}

// Ejecuta un proceso de tipo Manager. Recibe un archivo y el índice del proceso
// a ejecutar.
void execManager(InputFile* file, int process){
    
    char** line = file -> lines[process];
    // linea: id,timeout,n,linea_1,...,linea_n

    char* id = line[0];

    int timeout, cmp, root;

    timeout = atoi(line[1]);
    const int n = atoi(line[2]);

    cmp = strcmp(id, "R");
    root = !cmp;

    printf("Ejecutando proceso Manager ");
    if (!root) printf("No ");
    printf("Root...\n");

    int i;

    // Almacenamos los índices de las líneas de los procesos hijo.
    int child_idxs[n];
    for (i = 0; i < n; i++){
        child_idxs[i] = atoi(line[3 + i]);
    }

    int status;
    pid_t pid, own_pid;
    for (i = 0; i < n; i++){
        char** child_line = file -> lines[child_idxs[i]];

        pid = fork();
        own_pid = getpid();

        // Si es padre.
        if (pid > 0){
            printf("Ejecutando el proceso padre[%d]...\n", own_pid);
            wait(&status);
            printf("Cerrando el proceso padre[%d].\n", own_pid);
            exit(0);
        }

        // Si es hijo.
        else if (pid == 0){
            pid_t parent_pid = getppid();
            printf("Ejecutando el proceso hijo[%d] con padre[%d]...\n",
                    own_pid, parent_pid);

            id = child_line[0];
            cmp = strcmp(id, "W");

            // Si es un proceso Worker.
            if (!cmp){
                execWorker(file, child_idxs[i]);
            }

            // Si es un proceso Manager.
            else {
                execManager(file, child_idxs[i]);
            }
        }
    }
}
