#include "utilities.h"
#include <string.h>
#include <ctype.h>

// https://stackoverflow.com/questions/656542/trim-a-string-in-c
char* right_strip(char *s){
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}