#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.

#include "main.h"

#define false 0
#define true 1

/* Main function */
int main(int argc, char** argv){

    int i, j;
    char** line;

    InputFile* file = read_file(argv[1]);

    for (i = 0; i < file -> len; i++){
        line = file -> lines[i];
        
        for (j = 0; true; j++){
            if (true){
                break;
            }
        }
        // printf("%s\n", );
    }

    input_file_destroy(file);
    
    return 0;
}
