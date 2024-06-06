#ifndef INTERPRETER_ERROR_H
#define INTERPRETER_ERROR_H

#include "str.h"

typedef enum {
    ERR_PAREN_NOT_CLOSED,
    ERR_DIVISION_BY_ZERO,
} Errors;

typedef struct {
    Errors err;
    String message;
} Err;

#define Result(type)               \
typedef struct {                   \
    union {                        \
        type ok;                   \
        Err err;                   \
    } result;                      \
    bool is_err;                   \
}

#define Result_ok(type, result_type) \
result_type type##_ok(type t) {      \
    result_type r;                   \
    r.is_err = false;                \
    r.result.ok = t;                 \
    return r;                        \
}



#endif // INTERPRETER_ERROR_H
