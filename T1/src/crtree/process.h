#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#include "file_manager.h"
#include "signal_handler.h"
#include "utilities.h"

// Ejecuta un proceso de tipo Worker. Recibe un archivo y el índice del proceso
// a ejecutar.
void execWorker(InputFile* file, int process);

// Ejecuta un proceso de tipo Manager. Recibe un archivo y el índice del proceso
// a ejecutar.
void execManager(InputFile* file, int process);



// Variables globales.
pid_t pid_to_kill;
pid_t* pid_array_to_kill;
int n_children;
