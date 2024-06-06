#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

#include <stdbool.h>

#include "cvector.h"
#include "expr.h"
#include "statement.h"
#include "token_types.h"
#include "error.h"
#include "str.h"


Result(Token) token_result;
Result_ok(Token, token_result);

Expr expression(void);
Stmt statement(void);
Expr equality(void);
Expr comparison(void);
Expr term(void);
Expr factor(void);
Expr unary(void);
Expr primary(void);

Stmt statement_print(void);
Stmt statement_expression(void);

bool parser_match(TokenTypeEnum *type, size_t len);
bool parser_match_single(TokenTypeEnum type);
bool parser_check(TokenTypeEnum type);
Token parser_advance(void);
bool parser_is_at_end(void);
Token parser_peek(void);
Token parser_previous(void);
token_result parser_consume(TokenTypeEnum type, String message);

size_t parser_current = 0;
cvector_vector_type(Token) parser_tokens;


Expr expression(void) {
    return equality();
}

Stmt statement(void) {
    if (parser_match_single(PRINT)) return statement_print();

    return statement_expression();
}

Stmt statement_print(void) {
    Expr value = expression();
    parser_consume(SEMICOLON, str_from("Expect ';' after value."));
    Stmt stmt;
    stmt.type_enum = STMT_PRINT;
    stmt.union_type->stmt_print = { value };
    return stmt;
}

Stmt statement_expression(void) {
    Expr expr = expression();
    parser_consume(SEMICOLON, str_from("Expect ';' after expression."));
    Stmt stmt;
    stmt.type_enum = STMT_EXPRESSION;
    stmt.union_type->stmt_expression = { expr };
}

Expr equality(void) {
    Expr expr = comparison();

    TokenTypeEnum typeList[] = {BANG_EQUAL, EQUAL_EQUAL};
    while (parser_match(typeList, 2)) {
        Expr right = comparison();
        Token _operator = parser_previous();
        Expr_Binary binary = expr.union_type->binary;
        binary.left = expr;
        binary.op = _operator;
        binary.operator_enum.binary_enum = (OperatorBinaryEnum) parser_peek().TokenTypes;
        binary.right = right;
    }
    return expr;
}

Expr comparison() {
    Expr expr = term();

    TokenTypeEnum l[] = {GREATER, GREATER_EQUAL, LESS, LESS_EQUAL};
    while (parser_match(l, 4)) {
        Token operator = parser_previous();
        Expr right = term();
        Expr_Binary binary = expr.union_type->binary;
        binary.left = expr;
        binary.op = operator;
        binary.operator_enum.binary_enum = (OperatorBinaryEnum) parser_peek().TokenTypes;
        binary.right = right;
    }

    return expr;
}

Expr term(void) {
    Expr expr = factor();

    TokenTypeEnum l[] = {GREATER, GREATER_EQUAL, LESS, LESS_EQUAL};
    while (parser_match(l, 4)) {
        Token operator = parser_previous();
        Expr right = factor();
        Expr_Binary binary = expr.union_type->binary;
        binary.left = expr;
        binary.op = operator;
        binary.operator_enum.binary_enum = (OperatorBinaryEnum) parser_peek().TokenTypes;
        binary.right = right;
    }

    return expr;
}

Expr factor(void) {
    Expr expr = unary();

    TokenTypeEnum l[] = {SLASH, STAR};
    while (parser_match(l, 2)) {
        Token operator = parser_previous();
        Expr right = unary();
        Expr_Binary binary = expr.union_type->binary;
        binary.left = expr;
        binary.op = operator;
        binary.operator_enum.binary_enum = (OperatorBinaryEnum) parser_peek().TokenTypes;
        binary.right = right;
    }

    return expr;
}

Expr unary(void) {
    Expr expr = primary();

    TokenTypeEnum l[] = {BANG, MINUS};
    while (parser_match(l, 2)) {
        Token operator = parser_previous();
        Expr right = primary();
        Expr_Binary binary = expr.union_type->binary;
        binary.left = expr;
        binary.op = operator;
        binary.operator_enum.binary_enum = (OperatorBinaryEnum) parser_peek().TokenTypes;
        binary.right = right;
    }

    return expr;
}

Expr primary(void) {
    Expr_Literal literal;

    if (parser_match_single(FALSE)) {
        ObjectType obj_new_arg = { .obj_bool = false };
        Object obj = object_new(obj_new_arg, OBJ_BOOL);
        literal.value = obj;

    }
    if (parser_match_single(TRUE)) {
        ObjectType obj_new_arg = { .obj_bool = true };
        Object obj = object_new(obj_new_arg, OBJ_BOOL);
        literal.value = obj;
    }
    if (parser_match_single(_NULL)) {
        ObjectType obj_new_arg = { .obj_null = NULL };
        Object obj = object_new(obj_new_arg, OBJ_NULL);
        literal.value = obj;
    }
    if (parser_match_single(NUMBER)) {
        ObjectType obj_new_arg = { .obj_float = parser_previous().literal_or_operator.literal._float };
        Object obj = object_new(obj_new_arg, OBJ_FLOAT);
        literal.value = obj;
    }
    if (parser_match_single(STRING)) {
        ObjectType obj_new_arg = { .obj_string = parser_previous().literal_or_operator.literal.string };
        Object obj = object_new(obj_new_arg, OBJ_STRING);
        literal.value = obj;
    }
    if (parser_match_single(LEFT_PAREN)) {
        Expr expr = expression();
        String err_msg = str_from("Expected ')'.");
        token_result result = parser_consume(RIGHT_PAREN, err_msg);
        if (result.is_err) {
            switch (result.result.err.err) {
                case ERR_PAREN_NOT_CLOSED:
                    NULL;
                    break;
            }
        }

        Expr_Grouping grouping;
        switch (expr.type_enum) {
            case EXPR_BINARY:
                grouping.expression.binary = expr.union_type->binary;
                break;
            case EXPR_UNARY:
                grouping.expression.unary = expr.union_type->unary;
                break;
            case EXPR_LITERAL:
                grouping.expression.literal = expr.union_type->literal;
                break;
        }

        expr.union_type->grouping = grouping;
        expr.type_enum = EXPR_GROUPING;

        return expr;
    }

    Expr expr;
    expr.union_type->literal = literal;
    expr.type_enum = EXPR_LITERAL;
    return expr;
}

token_result parser_consume(TokenTypeEnum type, String message) {
    if (parser_check(type)) {
        Token_ok(parser_advance());
        token_result result;
        result.result.ok = parser_advance();
        result.is_err = false;
        return result;
    }

    token_result result;
    result.result.err.err = ERR_PAREN_NOT_CLOSED;
    result.result.err.message = message;
    result.is_err = true;
    return result;
}

bool parser_match(TokenTypeEnum types[], size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (parser_check(types[i])) {
            parser_advance();
            return true;
        }
    }
    return false;
}

bool parser_match_single(TokenTypeEnum type) {
    if (parser_check(type)) {
        parser_advance();
        return true;
    }
    return false;
}

bool parser_check(TokenTypeEnum type) {
    if (parser_is_at_end()) return false;
    return parser_peek().TokenTypes == type;
}

Token parser_advance(void) {
    if (!parser_is_at_end()) parser_current++;
    return parser_previous();
}

bool parser_is_at_end(void) {
    if (parser_peek().TokenTypes == END_OF_FILE) return true;
    return false;
}

Token parser_previous(void) {
    return parser_tokens[parser_current - 1];
}

Token parser_peek(void) {
    return parser_tokens[parser_current];
}

#endif // INTERPRETER_PARSER_H
