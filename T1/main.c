#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
// #include <string.h>

#include "main.h"

#define false 0
#define true 1

/* Main */
int main(int argc, char** argv){

    char* input = argv[1];
    int process = atoi(argv[2]);

    InputFile* file = read_file(input);

    char** start_line = file -> lines[process];
    char* id = start_line[0];

    int cmp;

    // 0 si id == "W", 1 si no.
    cmp = strcmp(id, "W");

    // Si es Worker.
    if (cmp == 0){
        execWorker(file, process);
    }
    
    // Si es Manager.
    else {
        execManager(file, process);
    }

    input_file_destroy(file);
    // printf("\n");
    return 0;
}
