#ifndef INTERPRETER_STR_H
#define INTERPRETER_STR_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "cvector.h"


typedef struct String {
    size_t len;
    size_t capacity;
    void *address;
} String;

cvector_vector_type(String) string_list = NULL;

String *string_realloc(String *self, size_t size);

// 문자열을 생성하고 초기화하는 함수
extern String string_empty(void) {
    String str = {
            .len = 0,
            .capacity = 1, // 최소한의 용량으로 시작합니다
            .address = malloc(sizeof(char)) // 최소한의 메모리를 할당합니다
    };
    if (str.address == NULL) {
        // 메모리 할당 실패 시
        str.capacity = 0;
    }

    cvector_push_back(string_list, str);

    return str;
}

// 주어진 C 스트링으로부터 새로운 문자열을 생성하는 함수
extern String string_from_array(const char *string) {
    String new_str;

    size_t l = strlen(string)+1;
    new_str.len = l;
    new_str.capacity = l; // 널 문자를 포함하지 않음
    void *ptr = malloc(l);  // value.union_type.binary.right 영역 침범
    if (ptr == NULL) {
        // 메모리 할당 실패 시
        new_str.capacity = 0;
        return new_str;
    }
    strncpy(ptr, string, l);
    new_str.address = ptr;

    cvector_push_back(string_list, new_str);

    return new_str;
}

extern String string_from_float(double f) {
    String str = string_empty();
    string_realloc(&str, 64);
    sprintf(str.address, "%lf", f);
    string_realloc(&str, strlen(str.address));
    cvector_push_back(string_list, str);
    return str;
}

extern String string_from_bool(const bool b) {
    return b ? string_from_array("true") : string_from_array("false");
}

// 두 문자열을 이어붙이는 함수
extern String string_concat(String self, String str) {
    size_t new_capacity = self.len + str.len + 1; // null 종료 문자를 고려하지 않음
    String new_str = string_empty();
    void *ptr = realloc(new_str.address, new_capacity * sizeof(char));
    if (ptr != NULL) {
        new_str.address = ptr;
        int i = sprintf(new_str.address, "%s", self.address);
        i--;  // sprintf는 끝에 널 문자를 포함시키기 때문에 널 문자를 제거하기 위해 i--를 함
        i += sprintf(new_str.address, "%s", str.address);

        strcat(self.address, str.address); // 두 문자열을 이어붙입니다
        self.len = new_capacity;
    }
    return self;
}

void string_free(String *self) {
    if (self->address != NULL) {
        free(self->address); // 동적으로 할당된 메모리를 해제합니다
        self->address = NULL; // 포인터를 NULL로 설정하여 잘못된 참조를 방지합니다
    }
}

String *string_realloc(String *self, size_t size) {
    // realloc 함수를 사용하여 메모리 크기를 조정합니다
    void *ptr = realloc(self->address, size * sizeof(char)); // size에 sizeof(char)를 곱하여 전체 바이트 크기를 계산합니다
    if (ptr != NULL) {
        self->address = ptr;
        self->capacity = size;
    }
    return self;
}

#endif //INTERPRETER_STR_H
