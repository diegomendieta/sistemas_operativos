#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){
    int secs = atoi(argv[1]);

    printf("Sleeping for %d seconds...\n", secs);
    sleep(secs);
    printf("Waking up...\n");

    return 0;
}