#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
    sleep(2);

    int sum = 0;
    int count = 0;
    for (int i = 1; argv[i] != NULL; i++){
        sum += atoi(argv[i]);
        count += 1;
    }

    float avg = sum / count;
    printf("El promedio es %.2f\n", avg);
    return avg;
}