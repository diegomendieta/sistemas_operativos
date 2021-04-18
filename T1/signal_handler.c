#include "signal_handler.h"

// Maneja la señal SIGABRT en un Worker.
void workerAbortHandler(){
    printf("Manejando señal SIGABRT en un proceso Worker.\n");
    printf("Matando proceso %d...\n", pid_to_kill);
    kill(pid_to_kill, SIGABRT);
}

// Setea el manejo de señales para un proceso Worker.
void setWorkerSignalHandling(){
    // Ignoramos la señal SIGINT.
    signal(SIGINT, SIG_IGN);
    signal(SIGABRT, workerAbortHandler);
}

// Maneja la señal SIGABRT en un Manager.
void managerAbortHandler(){
    for (int j = 0; j < n_childs; j++){
        kill(pid_array_to_kill[j], SIGABRT);
    }
}

// Setea el manejo de señales para un proceso Manager.
void setManagerSignalHandling(int root){
    signal(SIGABRT, managerAbortHandler);
    signal(SIGALRM, managerAbortHandler);

    if (root) signal(SIGINT, managerAbortHandler);
    else signal(SIGINT, SIG_IGN);
}
