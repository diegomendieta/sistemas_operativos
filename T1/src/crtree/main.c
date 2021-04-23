#include "main.h"

/* Main. Echa a andar un crtree. */
int main(int argc, char** argv){

    char* input = argv[1];
    int process = atoi(argv[2]);

    InputFile* file = read_file(input);

    char** start_line = file -> lines[process];
    char* id = start_line[0];

    // 0 si id == "W", 1 si no.
    int cmp = strcmp(id, "W");

    pid_t pid = fork();

    if (pid < 0){
        printf("Error ocurred when forking.");
        return 1;
    }

    // Si es child.
    if (pid == 0){
        // Si es Worker.
        if (cmp == 0){
            execWorker(file, process);
        }
        
        // Si es Manager.
        else {
            execManager(file, process);
        }
    }

    // Si es parent.
    else {
        waitpid(pid, NULL, 0);
        printf("Destroying file...\n");
        input_file_destroy(file);
        return 0;
    }
}
