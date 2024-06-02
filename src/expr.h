#ifndef INTERPRETER_EXPR_H
#define INTERPRETER_EXPR_H

#include "token_types.h"

typedef struct {
    void *left;
    Token operator;
    void *right;
} Expr_Binary;

typedef struct {
    Token operator;
    void *right;
} Expr_Unary;

typedef struct {
    void *value;
} Expr_Literal;

typedef struct {
    void *expression;
} Expr_Grouping;

union Expr {
    Expr_Binary binary;
    Expr_Unary unary;
    Expr_Literal literal;
    Expr_Grouping grouping;
};

#endif // INTERPRETER_EXPR_H
