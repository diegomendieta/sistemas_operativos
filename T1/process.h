#pragma once

#include "file_manager.h"
#include "signal_handler.h"

// Ejecuta un proceso de tipo Worker. Recibe un archivo y el índice del proceso
// a ejecutar.
void execWorker(InputFile* file, int process);

// Ejecuta un proceso de tipo Manager. Recibe un archivo y el índice del proceso
// a ejecutar.
void execManager(InputFile* file, int process);

// Variable global
pid_t pid_to_kill;
