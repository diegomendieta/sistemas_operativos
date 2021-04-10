#pragma once

typedef struct process {
    int PID;
    char* name;
    int priority;
    int state;
} Process;

Process* process_init(int PID, char* name);
