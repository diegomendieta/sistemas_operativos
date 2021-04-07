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

    int cmp = strcmp(id, "W");

    // Si es Worker.
    // linea: id,exec,n,arg_1,...,arg_n
    if (cmp == 0){
        printf("Executing Worker process...\n");
        execWorker(start_line);
    }

    /*
    // linea: id,timeout,n,linea_1,...,linea_n
    // Si es Manager Root.
    else if (id == "R"){
        execManagerRoot();
    }

    // Si es Manager No Root.
    else {
        // id == "M"
        execManagerNotRoot();
    }
    */

    input_file_destroy(file);
    return 0;
}
