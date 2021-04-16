#include <stdio.h>
#include <signal.h>
#include "signal_handler.h"

// Manejador de señales
void signal_handler(int sign){
    printf("Manejando señal [%d]...\n", sign);
    printf("Haciendo kill de %d\n", pid_to_kill);
    kill(pid_to_kill, SIGABRT);
}