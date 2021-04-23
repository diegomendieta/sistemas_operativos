#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    sleep(2);

    int sum = 0;
    for (int i = 1; argv[i] != NULL; i++){
        sum += atoi(argv[i]);
    }
    printf("La suma es %d\n", sum);
    return sum;
}