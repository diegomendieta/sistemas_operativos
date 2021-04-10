#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "process.h"

Process* process_init(int PID, char* name)
{
    Process* process = calloc(1, sizeof(Process));
    process -> PID = PID;
    process -> name = name;
    process -> priority = 0; /*ARREGLAR*/
    process -> state = 1;
}