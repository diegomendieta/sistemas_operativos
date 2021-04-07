#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "manager.h"


/*
Inicializa un proceso Manager. Los procesos Manager inician y monitorean
procesos hijos. Pueden ser Root o No Root.
*/
Manager* initManager(int root, float timeout){
    Manager* manager = calloc(sizeof(Manager), 1);
    manager -> root = root;
    manager -> timeout = timeout;
    return manager;
}
