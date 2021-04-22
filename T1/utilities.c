#include "utilities.h"

// https://stackoverflow.com/questions/656542/trim-a-string-in-c
char* rightStrip(char *s){
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}
