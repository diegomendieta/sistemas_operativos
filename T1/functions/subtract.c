#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    sleep(3);

    int subtraction = atoi(argv[1]);
    for (int i = 2; argv[i] != NULL; i++){
        subtraction -= atoi(argv[i]);
    }
    printf("La resta es %d\n", subtraction);
    return subtraction;
}