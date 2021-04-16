#pragma once

#include "file_manager.h"
#include "signal_handler.h"

// Ejecuta un proceso de tipo Worker. Recibe un archivo y el índice del proceso
// a ejecutar.
void execWorker(InputFile* file, int process);

// Ejecuta un proceso de tipo Manager. Recibe un archivo y el índice del proceso
// a ejecutar.
void execManager(InputFile* file, int process);

// Construye un archivo de output un Worker.
void buildWorkerOutput(int process, char* name, char** args, int n_args,
                       int exec_time, int return_code, int interrupted);

void buildManagerOutput();

// Variable global
pid_t pid_to_kill;
