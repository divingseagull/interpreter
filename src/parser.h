#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

// includes
#include <stdbool.h>

#include "cvector.h"

#include "types_expr.h"
#include "types_statement.h"
#include "types_token.h"
#include "error.h"
#include "str.h"
#include "box.h"


Result(Token) TokenResult;
Result_ok(Token, TokenResult);

// functions
Expr expression(void);
Stmt statement(void);
Expr equality(void);
Expr comparison(void);
Expr term(void);
Expr factor(void);
Expr unary(void);
Expr primary(void);
Expr assignment(void);

Stmt statement_print(void);
Stmt statement_expression(void);
Stmt var_declaration(void);

bool parser_match(TokenTypeEnum type);
bool parser_check(TokenTypeEnum type);
bool parser_check_next(TokenTypeEnum type);
bool parser_is_at_end(void);

Token parser_retreat(void);
Token parser_advance(void);
Token parser_peek_next(void);
Token parser_peek(void);
Token parser_previous(void);
TokenResult parser_consume(TokenTypeEnum type, Results expected, String message);

cvector_vector_type(Stmt) parser_parse(void);


// global variables
size_t parser_current = 0;
cvector_vector_type(Token) parser_tokens = NULL;


Expr expression(void) {
    return assignment();
}

Stmt declaration(void) {
    if (parser_match(TOKEN_VAR)) return var_declaration();
    return statement();
}

Stmt statement(void) {
    switch (parser_peek().TokenTypes) {
//        case TOKEN_AND:
//            return statement_and();
//            break;
//        case TOKEN_OR:
//            return statement_or();
//            break;
//        case TOKEN_NOT:
//            return statement_not();
//            break;
//        case TOKEN_IF:
//            return statement_if();
//            break;
//        case TOKEN_FOR:
//            return statement_for();
//            break;
//        case TOKEN_WHILE:
//            return statement_while();
//            break;
        case TOKEN_VAR:
            return var_declaration();
            break;
        case TOKEN_PRINT:
            parser_advance();
            // 시작 시점은 PRINT의 인자
            return statement_print();
            break;
    }

    return statement_expression();
}


Stmt statement_print(void) {
    Expr value = expression();
    TokenResult result = parser_consume(TOKEN_SEMICOLON,
                                        RES_SYNTAX_EXPECT_SEMICOLON,
                                        string_from_array("Expect ';' after value."));
    switch (result.res_type.type_enum) {  // 이 시점부터 value.union_type.binary.right 값 변경됨
        case RES_OK:
            Object obj;
            switch (value.type_enum) {
                case EXPR_LITERAL:
                    switch (value.union_type.literal.value.obj_type) {
                        case OBJ_STRING:
                            obj.obj_type = OBJ_STRING;
                            obj.obj_value.obj_string = value.union_type.literal.value.obj_value.obj_string;
                            break;
                        case OBJ_NULL:
                            obj.obj_type = OBJ_NULL;
                            obj.obj_value.obj_null = value.union_type.literal.value.obj_value.obj_null;
                            break;
                        case OBJ_BOOL:
                            obj.obj_type = OBJ_BOOL;
                            obj.obj_value.obj_bool = value.union_type.literal.value.obj_value.obj_bool;
                            break;
                        case OBJ_FLOAT:
                            obj.obj_type = OBJ_FLOAT;
                            obj.obj_value.obj_float = value.union_type.literal.value.obj_value.obj_float;
                            break;
                    }
                    break;
                case EXPR_VAR:
                    // TODO: 잘못된 반환값
                    Object *obj_ptr = (Object*)(environment_get(value.union_type.variable.name.lexeme)->address);
                    break;
            }
//            0x00000000005ff9e0
//            30 fc 5f 00   00 00 00 00   f0 f7 5f 00   00 00 00 00
//            f0 42 98 00   00 00 00 00

            Stmt stmt = {
                    .type_enum = STMT_PRINT,
                    .union_type.stmt_print.expression = {
                            .type_enum = value.type_enum,
                            .union_type = value.union_type,
                    }
            };
            return stmt;
        case RES_SYNTAX_EXPECT_SEMICOLON:
            raise_error(RES_SYNTAX_EXPECT_SEMICOLON, true, result.res_type.message);
            break;
        default:
            raise_error(RES_INTERNAL_UNEXPECTED_ARGUMENT, true, result.res_type.message);
            break;
    }
}

Stmt var_declaration(void) {
    TokenResult name_result = parser_consume(TOKEN_IDENTIFIER,
                                             RES_SYNTAX_EXPECT_NAME,
                                             string_from_array("Expect variable name"));
    Token name;
    switch (name_result.res_type.type_enum) {
        case RES_OK:
            name = name_result.val;
            break;
        case RES_SYNTAX_EXPECT_NAME:
            raise_error(RES_SYNTAX_EXPECT_NAME, true, name_result.res_type.message);
            break;
    }

    // var <identifier> = <expression>;
    // 선언과 동시에 초기화 해야함
    TokenResult eq_result = parser_consume(TOKEN_EQUAL,
                                           RES_SYNTAX_EXPECT_EQUAL,
                                           string_from_array("Variable must be initialized when declared"));
    switch (eq_result.res_type.type_enum) {
        case RES_SYNTAX_EXPECT_EQUAL:
            raise_error(RES_SYNTAX_EXPECT_EQUAL, true, eq_result.res_type.message);
            break;
    }

    Expr initializer = expression();
    parser_advance();
    TokenResult result = parser_consume(TOKEN_SEMICOLON,
                                        RES_SYNTAX_EXPECT_SEMICOLON,
                                        string_from_array("Expect ';' after variable declaration"));
    switch (result.res_type.type_enum) {
        case RES_OK:
            break;
        case RES_SYNTAX_EXPECT_SEMICOLON:
            raise_error(RES_SYNTAX_EXPECT_SEMICOLON, true, result.res_type.message);
            break;
    }

    Stmt s = {
            .type_enum = STMT_VAR,
            .union_type.stmt_var = {
                    .name = name,
                    .initializer = initializer,
            },
    };
    return s;
}

Stmt statement_expression(void) {
    Expr expr = expression();
    TokenResult result = parser_consume(TOKEN_SEMICOLON,
                                        RES_SYNTAX_EXPECT_SEMICOLON,
                                        string_from_array("Expect ';' after expression."));
    switch (result.res_type.type_enum) {
        case RES_OK:
            break;
        case RES_SYNTAX_EXPECT_SEMICOLON:
            raise_error(RES_SYNTAX_EXPECT_SEMICOLON, true, result.res_type.message);
            break;
    }
    Stmt stmt;
    stmt.type_enum = STMT_EXPRESSION;
    stmt.union_type.stmt_expression.expression = expr;
    return stmt;
}

Expr assignment(void) {
    Expr expr = equality();

    switch (parser_peek().TokenTypes) {
        case TOKEN_EQUAL:
            Token equals = parser_previous();
            Expr value = assignment();

            Token name = expr.union_type.variable.name;
            Expr result = {
                    .type_enum = EXPR_ASSIGN,
                    .union_type.assign = {
                            .name = name,
                            .value = &value,
                    }
            };
            return result;
    }

    return expr;
}

Expr equality(void) {
    Expr expr = comparison();

    switch (parser_peek().TokenTypes) {
        case TOKEN_BANG_EQUAL:
        case TOKEN_EQUAL_EQUAL:
            Expr right = comparison();
            Token _operator = parser_previous();
            ExprBinary binary = expr.union_type.binary;
            binary.left = &expr;
            binary.op = _operator;
            binary.operator_enum = (BinaryOperatorEnum) parser_peek().TokenTypes;
            binary.right = &right;

            expr.type_enum = EXPR_BINARY;
            expr.union_type.binary = binary;
    }
    return expr;
}

Expr comparison() {
    Expr expr = term();

    switch (parser_peek().TokenTypes) {
        case TOKEN_GREATER:
        case TOKEN_GREATER_EQUAL:
        case TOKEN_LESS:
        case TOKEN_LESS_EQUAL:
            Token operator = parser_previous();
            Expr right = term();
            ExprBinary binary = expr.union_type.binary;
            binary.left = &expr;
            binary.op = operator;
            binary.operator_enum = (BinaryOperatorEnum) parser_peek().TokenTypes;
            binary.right = &right;

            expr.type_enum = EXPR_BINARY;
            expr.union_type.binary = binary;
    }
    return expr;
}

Expr term(void) {
    Expr expr = factor();

    switch (parser_peek().TokenTypes) {
        case TOKEN_MINUS:
        case TOKEN_PLUS:
            parser_advance();
            Token operator = parser_peek();
            parser_advance();
            Expr left = expr;
            Expr right = factor();
            right.union_type.literal.value = object_new(right.union_type.literal.value.obj_value,
                                                        right.union_type.literal.value.obj_type);
            ExprBinary binary = {
                    .op = operator,
                    .operator_enum = operator.literal_or_operator.val.operator.operator_enum.binary,
                    .left = &left,
                    .right = &right
            };

            expr.type_enum = EXPR_BINARY;
            expr.union_type.binary = binary;
    }
    return expr;
}

Expr factor(void) {
    Expr expr = unary();

    switch (parser_peek().TokenTypes) {
        case TOKEN_SLASH:
        case TOKEN_STAR:
            Token operator = parser_previous();
            Expr right = unary();
            ExprBinary binary = expr.union_type.binary;
            binary.left = &expr;
            binary.op = operator;
            binary.operator_enum = (BinaryOperatorEnum) parser_peek().TokenTypes;
            binary.right = &right;

            expr.type_enum = EXPR_BINARY;
            expr.union_type.binary = binary;
    }
    return expr;
}

Expr unary(void) {
    Expr expr = primary();

    if (true) {
        switch (parser_peek().TokenTypes) {
            case TOKEN_BANG:
            case TOKEN_MINUS:
                Token operator = parser_peek();
                Expr right = primary();
                ExprUnary unary = expr.union_type.unary;
                unary.op = operator;
                unary.operator_enum = (UnaryOperatorEnum) parser_peek().TokenTypes;
                unary.right = &right;

                expr.type_enum = EXPR_UNARY;
                expr.union_type.unary = unary;
        }
    }
    return expr;
}

Expr primary(void) {
    ExprLiteral literal;
    ObjectType obj_new_arg;
    Object obj;
    Expr expr;

    switch (parser_peek().TokenTypes) {
        case TOKEN_BOOL:
            if (strncmp(parser_peek().lexeme.address, "true", 4) == 0) {
                obj_new_arg.obj_bool = true;
            }
            else {
                obj_new_arg.obj_bool = false;
            }
            obj = object_new(obj_new_arg, OBJ_BOOL);
            literal.value = obj;
            break;
        case TOKEN_NULL:
            obj_new_arg.obj_null = NULL;
            obj = object_new(obj_new_arg, OBJ_NULL);
            literal.value = obj;
            break;
        case TOKEN_NUMBER:
            obj_new_arg.obj_float = parser_peek().literal_or_operator.val.literal._float;
            obj = object_new(obj_new_arg, OBJ_FLOAT);
            literal.value = obj;
            break;
        case TOKEN_STRING:
            obj_new_arg.obj_string = parser_peek().literal_or_operator.val.literal.string;
            obj = object_new(obj_new_arg, OBJ_STRING);
            literal.value = obj;
            break;
        case TOKEN_LEFT_PAREN:
            parser_advance();  // TODO: 잘못된 주소 참조
            expr = expression();
            String err_msg = string_from_array("Expected ')'.");
            TokenResult result = parser_consume(TOKEN_RIGHT_PAREN, RES_SYNTAX_EXPECT_PAREN, err_msg);

            switch (result.res_type.type_enum) {
                case RES_OK:
                    break;
                case RES_SYNTAX_EXPECT_PAREN:
                    raise_error(RES_SYNTAX_EXPECT_PAREN, true, result.res_type.message);
                break;
            }

            ExprGrouping grouping;
            switch (expr.type_enum) {
                case EXPR_BINARY:
                    grouping.expression.binary = expr.union_type.binary;
                    break;
                case EXPR_UNARY:
                    grouping.expression.unary = expr.union_type.unary;
                    break;
                case EXPR_LITERAL:
                    grouping.expression.literal = expr.union_type.literal;
                    break;
            }

            expr.union_type.grouping = grouping;
            expr.type_enum = EXPR_GROUPING;

            return expr;
        case TOKEN_IDENTIFIER:
            expr.type_enum = EXPR_VAR;
            expr.union_type.variable.name = parser_peek();
            return expr;

        // operators
        // E: Stack overflow
        case TOKEN_MINUS:
            expr.type_enum = EXPR_UNARY;
            parser_advance();
            struct ExprStruct rvalue_minus = expression();
            expr.union_type.unary.right = &rvalue_minus;
            expr.union_type.unary.operator_enum = OP_UNARY_MINUS;
            expr.union_type.unary.op = parser_previous();
            return expr;
        case TOKEN_BANG:
            expr.type_enum = EXPR_UNARY;
            expr.union_type.unary.op = parser_peek();
            parser_advance();
            struct ExprStruct rvalue_bang = expression();
            expr.union_type.unary.right = &rvalue_bang;
            expr.union_type.unary.operator_enum = OP_UNARY_BANG;
            return expr;

        // keywords
        case TOKEN_PRINT:
            break;

        // default
        default:
            raise_error(RES_SYNTAX_EXPECT_EXPRESSION, true, result.res_type.message);
            break;
    }

    parser_advance();
    expr.union_type.literal = literal;
    expr.type_enum = EXPR_LITERAL;
    return expr;
}

cvector_vector_type(Stmt) parser_parse(void) {
    cvector_vector_type(Stmt) statements = NULL;
    while (!parser_is_at_end()) {
        cvector_push_back(statements, declaration());
    }

    return statements;
}

TokenResult parser_consume(TokenTypeEnum type, Results expected, String message) {
    if (parser_check(type)) {
        TokenResult tr = TokenResult_unwrap(parser_advance());
        tr.res_type.type_enum = RES_OK;
        return tr;
    }

    TokenResult result;
    result.res_type.type_enum = expected;
    result.res_type.message = message;
    return result;
}

bool parser_match(TokenTypeEnum type) {
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

bool parser_check_next(TokenTypeEnum type) {
    if (parser_is_at_end()) return false;
    return parser_peek_next().TokenTypes == type;
}

Token parser_retreat(void) {
    return parser_tokens[parser_current--];
}

Token parser_advance(void) {
    if (!parser_is_at_end()) parser_current++;
    return parser_previous();
}

bool parser_is_at_end(void) {
    if (parser_peek().TokenTypes == TOKEN_END_OF_FILE) return true;
    return false;
}

Token parser_previous(void) {
    return parser_tokens[parser_current - 1];
}

Token parser_peek(void) {
    return parser_tokens[parser_current];
}

Token parser_peek_next(void) {
    Token eof_token = {
        .TokenTypes = TOKEN_END_OF_FILE,
    };
    if (cvector_size(parser_tokens) <= parser_current + 1) return eof_token;
    return parser_tokens[parser_current+1];
}

#endif // INTERPRETER_PARSER_H
