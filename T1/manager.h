#pragma once

/* Representa un proceso Manager. */
typedef struct manager {
    int root;
    float timeout;
} Manager;

/*
Inicializa un proceso Manager. Los procesos Manager inician y monitorean
procesos hijos. Pueden ser Root o No Root.
*/
Manager* initManager(int root, float timeout);
