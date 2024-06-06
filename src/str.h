#ifndef INTERPRETER_STR_H
#define INTERPRETER_STR_H

#include <stdlib.h>

typedef struct {
    size_t len;
    size_t capacity;
    void *address;
} String;

String str_empty(void);
String str_from(const char *string);
String str_concat(String self, String str);

extern String str_empty(void) {
    String str = {
            .len = 0,
            .capacity = 0,
    };
    void *ptr = malloc(sizeof(str));
    str.address = ptr;
    return str;
}

extern String str_from(const char *string) {
    String new_str;
    size_t l = strlen(string);
    new_str.len = l;
    new_str.capacity = l;
    void *ptr = calloc(l, sizeof(char));
    strncpy(ptr, string, l);
    new_str.address = ptr;
    return new_str;
}

extern String str_from_float(const double f) {
    String new_str;
    void *ptr = malloc(sizeof(f));
    sprintf(ptr, "%lf", f);
    new_str.address = ptr;
    new_str.capacity = strlen(ptr);
    new_str.len = strlen(ptr);
    return new_str;
}

extern String str_concat(String self, String str) {
    size_t new_capacity = self.capacity + str.capacity;
    realloc(self.address, new_capacity);
    self.len = self.len + str.len;
    return self;
}

#endif //INTERPRETER_STR_H
