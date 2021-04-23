#pragma once

#include <stdio.h>
#include <signal.h>

#include "process.h"

// Setea el manejo de señales para un proceso Worker.
void setWorkerSignalHandling();

// Setea el manejo de señales para un proceso Manager.
void setManagerSignalHandling(int root);
