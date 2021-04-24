#include "signal_handler.h"

// Maneja la señal SIGABRT en un Worker.
void workerAbortHandler(){
    printf("\nManejando señal SIGABRT en un proceso Worker.\n");
    printf("Matando proceso %d...\n", pid_to_kill);
    kill(pid_to_kill, SIGABRT);
    printf("Proceso %d matado.\n\n", pid_to_kill);
}

// Setea el manejo de señales para un proceso Worker.
void setWorkerSignalHandling(){
    // Ignoramos la señal SIGINT.
    signal(SIGINT, SIG_IGN);
    signal(SIGABRT, workerAbortHandler);
}

// Maneja la señal SIGABRT en un Manager.
void managerAbortHandler(){
    printf("\nManejando señal SIGABRT en un proceso Manager.\n");
    for (int j = 0; j < n_children; j++){
        printf("Matando proceso %d...\n", pid_to_kill);
        kill(pid_array_to_kill[j], SIGABRT);
        printf("Proceso %d matado.\n\n", pid_to_kill);
    }
}

// Maneja la señal SIGINT en un Manager.
void managerRootIntHandler(){
    printf("\nManejando señal SIGINT en un proceso Manager Root.\n");
    for (int j = 0; j < n_children; j++){
        printf("Matando proceso %d...\n", pid_to_kill);
        kill(pid_array_to_kill[j], SIGABRT);
        printf("Proceso %d matado.\n\n", pid_to_kill);
    }
}

// Setea el manejo de señales para un proceso Manager.
void setManagerSignalHandling(int root){
    signal(SIGABRT, managerAbortHandler);
    signal(SIGALRM, managerAbortHandler);

    if (root) signal(SIGINT, managerRootIntHandler);
    else signal(SIGINT, SIG_IGN);
}
