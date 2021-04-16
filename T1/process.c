#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>      // getitimer
#include <unistd.h>
#include <time.h>

#include "process.h"

// Variable global
pid_t pid_to_kill;

// Construye un archivo de output un Worker.
void buildWorkerOutput(int idx, char* name, char** args, int n_args,
                       int exec_time, int return_code, int interrupted){
    printf("Entrando a función buildWorkerOutput...\n");

    char path[10];
    sprintf(path, "%d", idx);
    char* extension = ".txt";
    strcat(path, extension);

    FILE* file = fopen(path, "w");
    fprintf(file, "%s,", name);
    for (int j = 1; j <= n_args; j++){
        fprintf(file, "%s,", args[j]);
    }
    fprintf(file, "%d,", exec_time);
    fprintf(file, "%d,", return_code);
    fprintf(file, "%d", interrupted);
    fclose(file);

    printf("Saliendo de función buildWorkerOutput...\n\n");
}

// Construye un archivo de output de un Manager.
void buildManagerOutput(){
}

// Ejecuta un proceso de tipo Worker. Recibe un archivo y el índice del proceso
// a ejecutar.
void execWorker(InputFile* file, int process){
    
    // Indicamos cómo manejar las señales SIGINT y SIGABRT.
    // En el caso de SIGINT, la ignoramos.
    signal(SIGINT, SIG_IGN);
    // En el caso de SIGABRT, aplicamos la función signal_handler.
    signal(SIGABRT, signal_handler);

    printf("\n[%d] Ejecutando proceso Worker...\n", process);

    // linea: id,exec,n,arg_1,...,arg_n
    char** line = file -> lines[process];
    
    char* to_exec = line[1];
    const int n = atoi(line[2]);

    int status, return_code, interrupted;

    char* args[n+2];
    args[0] = to_exec;
    for (int counter = 1; counter <= n; counter++){
        args[counter] = line[3 + (counter - 1)];
    }
    args[n+1] = NULL;

    pid_t pid, own_pid;
    pid = fork();
    own_pid = getpid();

    time_t start, exec_time;
    start = time(NULL);

    // Proceso padre.
    if (pid > 0){
        pid_to_kill = pid;

        printf("Ejecutando el proceso padre[%d]...\n", own_pid);
        wait(&status);

        /*
        ESTADÍSTICAS
        */
        // name: to_exec
        // args: args[1:n+1]
        exec_time = time(NULL) - start;
        return_code = WEXITSTATUS(status);
        interrupted = !WIFEXITED(status);

        printf("---------\n");
        printf("[%d] Stats: \n", pid);
        printf("Name: %s\n", to_exec);

        printf("Args: ");
        for (int i = 1; i < n+1; i++){
            printf("%s", args[i]);
            if (i < n) printf(",");
        }
        printf("\n");

        printf("Execution time: %ld\n", exec_time);
        printf("Return code: %d\n", return_code);
        printf("Interrupted: %d\n", interrupted);
        printf("---------\n");

        // Generamos archivo de output.
        buildWorkerOutput(
            process, to_exec, args, n, exec_time, return_code, interrupted
        );
        
        printf("Cerrando el proceso padre[%d].\n\n", own_pid);
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
    
    // linea: id,timeout,n,linea_1,...,linea_n
    char** line = file -> lines[process];
    
    char* id = line[0];

    int timeout, cmp, root;

    timeout = atoi(line[1]);
    const int n = atoi(line[2]);

    cmp = strcmp(id, "R");
    root = !cmp;

    printf("\n[%d] Ejecutando proceso Manager ", process);
    if (!root) printf("No ");
    printf("Root...\n");

    int i;

    // Almacenamos los índices de las líneas de los procesos hijo.
    int child_idxs[n];
    for (i = 0; i < n; i++){
        child_idxs[i] = atoi(line[3 + i]);
    }

    // Array que almacena las ids de los procesos hijo.
    pid_t pid_arr[n];

    pid_t parent_pid, p, own_pid;

    own_pid = getpid();
    printf("Ejecutando el proceso padre[%d]...\n", own_pid);

    // Array que almacena estatus de salida de los hijos.
    int status[n];

    parent_pid = own_pid;
    for (i = 0; i < n; i++){
        char** child_line = file -> lines[child_idxs[i]];

        p = fork();

        // Si el proceso es padre, setear que espere al hijo y continuar.
        if (p > 0) {
            printf("Seteando espera del padre[%d] al hijo[%d]\n", own_pid, p);
            waitpid(p, &status[i], 0);
            continue;
        }

        own_pid = getpid();
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

    if (p > 0) {
        printf("Cerrando el proceso padre[%d].\n\n", own_pid);
        exit(0);
    }
}
