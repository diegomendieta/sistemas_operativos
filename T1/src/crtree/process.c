#include "process.h"

// Variable global que indica qué proceso debe matar un Worker (su hijo).
pid_t pid_to_kill;

// Variable global que indica qué procesos debe matar un Manager (sus hijos).
pid_t* pid_array_to_kill;

// Variable global que indica el número de hijos que tiene un Manager.
int n_children;

// Construye el archivo de output de un Worker.
void buildWorkerOutput(int idx, char* name, char** args, int n_args,
                       int exec_time, int return_code, int interrupted){
    printf("Entrando a función buildWorkerOutput...\n");

    char path[10];
    sprintf(path, "%d", idx);
    char* extension = ".txt";
    strcat(path, extension);

    FILE* file = fopen(path, "w");
    fprintf(file, "%s,", name);
    for (int j = 1; j <= n_args; j++){
        fprintf(file, "%s,", args[j]);
    }
    fprintf(file, "%d,", exec_time);
    fprintf(file, "%d,", return_code);
    fprintf(file, "%d", interrupted);
    fclose(file);

    printf("Saliendo de función buildWorkerOutput...\n\n");
}

// Construye el archivo de output de un Manager.
void buildManagerOutput(int process_id, int* child_idxs, int n){
    printf("Entrando a función buildManagerOutput...\n");

    FILE* to_file;
    FILE* from_file;
    char* extension = ".txt";

    char from_path[11];
    char buffer[BUFFER_SIZE];

    char to_path[11];
    sprintf(to_path, "%d", process_id);
    strcat(to_path, extension);
    
    to_file = fopen(to_path, "w");
    for (int i = 0; i < n; i++){
        sprintf(from_path, "%d", child_idxs[i]);
        strcat(from_path, extension);

        // printf("from_path: %s\n", from_path);

        from_file = fopen(from_path, "r");
        while (fgets(buffer, BUFFER_SIZE, from_file)){           
            fprintf(to_file, "%s", buffer);
        }
        fclose(from_file);

        if (i != n-1) fprintf(to_file, "\n");
    }
    fclose(to_file);

    printf("Saliendo de función buildManagerOutput...\n\n");
}

// Ejecuta un proceso de tipo Worker. Recibe un archivo y el índice del proceso
// a ejecutar.
void execWorker(InputFile* file, int process){

    // Seteamos el manejo de señales.
    setWorkerSignalHandling();

    printf("\n[%d] Ejecutando proceso Worker...\n", process);

    // linea: id,exec,n,arg_1,...,arg_n
    char** line = file -> lines[process];
    
    char* to_exec = line[1];
    const int n = atoi(line[2]);

    int status, return_code, interrupted;

    char* args[n+2];
    args[0] = to_exec;
    for (int counter = 1; counter <= n; counter++){
        line[3 + (counter - 1)] = rightStrip(line[3 + (counter - 1)]);
        args[counter] = line[3 + (counter - 1)];
        // printf("%s\n", line[3 + (counter - 1)]);
    }
    args[n+1] = NULL;

    pid_t pid, own_pid;
    pid = fork();
    own_pid = getpid();

    time_t start = time(NULL);

    // Proceso padre.
    if (pid > 0){
        printf("Ejecutando el proceso padre[%d]...\n", own_pid);

        pid_to_kill = pid;
        printf("Seteando pid_to_kill en %d\n", pid_to_kill);

        wait(&status);

        /*
        ESTADÍSTICAS
        */
        // name: to_exec
        // args: args[1:n+1]
        time_t exec_time = time(NULL) - start;
        return_code = WEXITSTATUS(status);
        interrupted = !WIFEXITED(status);

        // Generamos archivo de output.
        buildWorkerOutput(
            process, to_exec, args, n, exec_time, return_code, interrupted
        );
        
        printf("Cerrando el proceso padre[%d].\n\n", own_pid);
        exit(0);
    }

    // Proceso hijo.
    else if (pid == 0){
        pid_t parent_pid = getppid();
        printf("Ejecutando el proceso hijo[%d] con padre[%d]...\n",
                own_pid, parent_pid);
        printf("Archivo: %s\n", args[0]);
        execvp(args[0], args);

        // Agregamos print para chequear si hay errores.
        printf("ESTA LÍNEA NO DEBERÍA MOSTRARSE.\n");
        exit(1);
    }
}

// Ejecuta un proceso de tipo Manager. Recibe un archivo y el índice del proceso
// a ejecutar.
void execManager(InputFile* file, int process){

    // linea: id,timeout,n,linea_1,...,linea_n
    char** line = file -> lines[process];
    char* id = line[0];

    int cmp, root;

    cmp = strcmp(id, "R");
    root = !cmp;

    setManagerSignalHandling(root);

    int timeout = atoi(line[1]);

    // Seteamos el tiempo de timeout. Cuando este se cumpla, se envían una señal
    // SIGALRM al proceso.
    alarm(timeout);

    const int n = atoi(line[2]);

    // Seteamos la variable global del número de hijos.
    n_children = n;

    pid_array_to_kill = malloc(sizeof(pid_t) * n);

    printf("\n[%d] Ejecutando proceso Manager ", process);
    if (!root) printf("No ");
    printf("Root...\n");

    int i;

    // Almacenamos los índices de las líneas de los procesos hijo.
    int child_idxs[n];
    for (i = 0; i < n; i++){
        child_idxs[i] = atoi(line[3 + i]);
    }

    pid_t parent_pid, p, own_pid;

    own_pid = getpid();
    printf("Ejecutando el proceso padre[%d]...\n", own_pid);

    // Array que almacena estatus de salida de los hijos.
    int status[n];

    parent_pid = own_pid;
    for (i = 0; i < n; i++){
        char** child_line = file -> lines[child_idxs[i]];

        p = fork();

        // Si el proceso es padre.
        if (p > 0) {
            printf("Agregando %d al array de hijos.\n", p);
            pid_array_to_kill[i] = p;
            continue;
        }

        own_pid = getpid();
        printf("Ejecutando el proceso hijo[%d] con padre[%d]...\n",
                own_pid, parent_pid);

        id = child_line[0];
        cmp = strcmp(id, "W");

        // Si es un proceso Worker.
        if (!cmp){
            execWorker(file, child_idxs[i]);
        }

        // Si es un proceso Manager.
        else {
            execManager(file, child_idxs[i]);
        }
    }

    // Seteamos espera del padre a los hijos.
    for (i = 0; i < n; i++){
        printf(
            "Seteando espera del padre[%d] al hijo[%d]\n",
            own_pid, pid_array_to_kill[i]
        );
        waitpid(pid_array_to_kill[i], &status[i], 0);
    }
    
    if (p > 0) {
        // Creamos el archivo de output.
        buildManagerOutput(process, child_idxs, n);
        printf("Cerrando el proceso padre[%d].\n\n", own_pid);

        free(pid_array_to_kill);
        exit(0);
    }
}
