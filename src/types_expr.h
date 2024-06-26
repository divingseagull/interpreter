#ifndef INTERPRETER_EXPR_H
#define INTERPRETER_EXPR_H

#include "types_token.h"
#include "object.h"

struct ExprStruct;

typedef enum ExprTypeEnum{
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_LITERAL,
    EXPR_GROUPING,
    EXPR_VAR,
    EXPR_ASSIGN,
} ExprTypeEnum;

struct ExprStruct;


typedef struct ExprBinary {
    struct ExprStruct *left;
    Token op;
    BinaryOperatorEnum operator_enum;
    struct ExprStruct *right;
} ExprBinary;

typedef struct ExprUnary {
    Token op;
    UnaryOperatorEnum operator_enum;
    struct ExprStruct *right;
} ExprUnary;

typedef struct ExprLiteral {
    Object value;
} ExprLiteral;

typedef union ExprExpression {
    ExprBinary binary;
    ExprUnary unary;
    ExprLiteral literal;
} ExprExpression;

typedef struct ExprGrouping {
    ExprExpression expression;
} ExprGrouping;

typedef struct ExprVariable {
    Token name;
} ExprVariable;

typedef struct ExprAssign {
    Token name;
    struct ExprStruct *value;
} ExprAssign;

typedef union ExprUnionType {
    ExprBinary binary;
    ExprUnary unary;
    ExprLiteral literal;
    ExprGrouping grouping;
    ExprVariable variable;
    ExprAssign assign;
} ExprUnionType;

typedef struct ExprStruct {
    ExprUnionType union_type;
    ExprTypeEnum type_enum;
} Expr;

Expr expr_null(void) {
    Expr expr = {
            .type_enum = EXPR_LITERAL,
            .union_type = NULL,
    };
    return expr;
}

#endif // INTERPRETER_EXPR_H
