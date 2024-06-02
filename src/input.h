#ifndef INTERPRETER_INPUT_H
#define INTERPRETER_INPUT_H

#include <stdlib.h>
#include <stdio.h>
#include "cvector.h"


cvector_vector_type(char) tokenList;

char* get_from_prompt(void) {
    char buffer[4000];
    printf("%s", " >");
    fgets(buffer, sizeof(buffer), stdin);
    fflush(stdin);
    char *ptr = malloc(sizeof(buffer));
    return ptr;
}

char* readline(char *prompt) {
    char buffer[4000] = {0};
    unsigned int idx = 0;
    char *ptr = malloc(sizeof(buffer));

    while (true) {
        char c = (char)getchar();

        if (c == '\n' || c == EOF) {
            break;
        }
        buffer[idx++] = c;
    }

    return ptr;
}

#endif //INTERPRETER_INPUT_H
