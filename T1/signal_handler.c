#include <stdio.h>
#include <signal.h>
#include "signal_handler.h"

// Manejador de señales
void signal_handler(int sign){
    printf("Manejando señal [%d]...\n", sign);
    kill(pid_to_kill, SIGABRT);
}