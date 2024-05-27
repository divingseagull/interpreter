#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cvector.h"  // repo: https://github.com/eteran/c-vector

cvector_vector_type(char) tokenList;

char *get_from_prompt(void) {
    char buffer[1000];
    printf("%s", " >");
    fgets(buffer, sizeof(buffer), stdin);
    char *ptr = (char*)malloc(sizeof(buffer));
    return ptr;
}

char *get_from_file(char *filename) {
    FILE *file = fopen(filename, "r");

    fclose(file);
}
