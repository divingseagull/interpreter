#ifndef INTERPRETER_READLINE_H
#define INTERPRETER_READLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4000

char* readline(char *prompt) {
    char buffer[BUFFER_SIZE] = {0, };
    unsigned int idx = 0;

    printf("%s", prompt); // 사용자에게 프롬프트를 출력합니다

    // 문자열을 저장할 동적 메모리를 할당합니다
    char *ptr = malloc(sizeof(buffer));
    if (ptr == NULL) {
        // 메모리 할당에 실패한 경우 NULL을 반환합니다
        return NULL;
    }

    while (1) { 
        char c = (char)getchar();

        if (c == '\n' || c == EOF) {
            break;
        } else {
            buffer[idx++] = c;
        }
    }

    // 읽은 문자열을 동적으로 할당한 메모리에 복사합니다
    strcpy(ptr, buffer);

    return ptr;
}

char *readfile(char *filename) {
    FILE *f_ptr = fopen(filename, "r");

    if (f_ptr == NULL) {
        printf("파일을 열 수 없습니다");
        exit(-1);
    }

    char buffer[BUFFER_SIZE] = {0, };
    while (fgets(buffer, BUFFER_SIZE, f_ptr) != NULL);
}

#endif
