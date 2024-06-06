#ifndef INTERPRETER_EXPR_H
#define INTERPRETER_EXPR_H

#include "token_types.h"
#include "object.h"

union Expr_Union;

struct Expr_Struct;
struct Expr_Binary_Struct;
struct Expr_Unary_Struct;
struct Expr_Literal_Struct;
struct Expr_Grouping_Struct;

typedef enum {
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_LITERAL,
    EXPR_GROUPING,
} ExprTypeEnum;

typedef struct Expr_Struct {
    union Expr_Union *union_type;
    ExprTypeEnum type_enum;
} Expr;


typedef struct Expr_Binary_Struct {
    Expr left;
    Token op;
    OperatorUnion operator_enum;
    Expr right;
} Expr_Binary;

typedef struct Expr_Unary_Struct {
    Token op;
    OperatorUnion operator_enum;
    Expr right;
} Expr_Unary;

typedef struct Expr_Literal_Struct {
    Object value;
} Expr_Literal;

typedef union {
    Expr_Binary binary;
    Expr_Unary unary;
    Expr_Literal literal;
} Expr_Expression;

typedef struct Expr_Grouping_Struct {
    Expr_Expression expression;
} Expr_Grouping;

typedef union Expr_Union {
    Expr_Binary binary;
    Expr_Unary unary;
    Expr_Literal literal;
    Expr_Grouping grouping;
} Expr_Union_Type;

#endif // INTERPRETER_EXPR_H
