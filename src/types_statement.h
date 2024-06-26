#ifndef STATEMENT_H
#define STATEMENT_H

#include "types_expr.h"

union StmtUnion;

typedef enum StmtTypeEnum {
    STMT_NULL,
    STMT_EXPRESSION,
    STMT_WHILE,
    STMT_PRINT,
    STMT_VAR,
} StmtTypeEnum;

typedef struct StmtExpression {
    Expr expression;
} StmtExpression;

typedef struct StmtIf {
    Expr condition;
    union StmtUnion *body;
} StmtIf;

typedef struct StmtFor {
    Expr declaration;
    Expr condition;
    Expr increment;
    union StmtUnion *body;
} StmtFor;

typedef struct StmtWhile {
    Expr condition;
    StmtTypeEnum type_enum;
    union StmtUnion *body;
} StmtWhile;

typedef struct StmtPrint {
    Expr expression;
} StmtPrint;

typedef struct StmtVar {
    Token name;
    Expr initializer;
} StmtVar;

typedef union StmtUnion {
    StmtExpression stmt_expression;
    StmtPrint stmt_print;
    StmtVar stmt_var;
    StmtWhile stmt_while;
    StmtFor stmt_for;
    StmtIf stmt_if;
} StmtUnionType;

typedef struct Stmt {
    StmtUnionType union_type;
    StmtTypeEnum type_enum;
} Stmt;

Stmt stmt_null(void) {
    Stmt stmt = {
            .type_enum = STMT_NULL,
            .union_type = NULL,
    };
    return stmt;
}

#endif //STATEMENT_H
