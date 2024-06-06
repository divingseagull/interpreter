#ifndef STATEMENT_H
#define STATEMENT_H

#include "expr.h"

union Stmt_Union;

struct Stmt_Expression_Struct;
struct Stmt_Print_Struct;

typedef enum {
    STMT_EXPRESSION,
    STMT_PRINT,
} StmtTypeEnum;

typedef struct {
    union Stmt_Union *union_type;
    StmtTypeEnum type_enum;
} Stmt;


typedef struct Stmt_Expression_Struct {
    Expr expression;
} Stmt_Expression;

typedef struct Stmt_Print_Struct {
    Expr expression;
} Stmt_Print;

typedef union Stmt_Union {
    Stmt_Expression stmt_expression;
    Stmt_Print stmt_print;
} Stmt_Union_Type;

#endif //STATEMENT_H
