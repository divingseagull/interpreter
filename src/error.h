#ifndef INTERPRETER_ERROR_H
#define INTERPRETER_ERROR_H


#include <stdio.h>
#include <stdbool.h>

#include "str.h"


static struct ErrorHandlerStruct {
    bool had_error;
} ErrorHandler;

void error_handler_init(void) {
    ErrorHandler.had_error = false;
}

typedef enum {
    RES_OK,
    RES_SYNTAX_EXPECT_NAME,
    RES_SYNTAX_EXPECT_EQUAL,
    RES_SYNTAX_EXPECT_PAREN,
    RES_SYNTAX_EXPECT_SEMICOLON,
    RES_SYNTAX_EXPECT_EXPRESSION,
    RES_SYNTAX_EXPECT_STATEMENT,
    RES_RUNTIME_DIVISION_BY_ZERO,
    RES_RUNTIME_INVALID_TYPE,
    RES_RUNTIME_UNDECLARED_VARIABLE,
    RES_INTERNAL_UNEXPECTED_ARGUMENT,
} Results;

typedef struct {
    Results type_enum;
    String message;
} Res;

// 값 사용시 무조건 res_type이 RES_OK인지 체크한 후 사용
#define Result(type)               \
typedef struct {                   \
    type val;                      \
    Res res_type;                  \
}                                  \


#define Result_ok(type, result_type) \
result_type result_type##_unwrap(type t) {      \
    result_type r;                   \
    r.val = t;                       \
    return r;                        \
}

void raise_error(Results err, bool terminate_program, String message) {
    ErrorHandler.had_error = true;
    printf("Error: %s", (char*)message.address);
    if (terminate_program) {
        exit(-1);
    }
}

#endif // INTERPRETER_ERROR_H
