#ifndef INTERPRETER_TOKEN_TYPES_H
#define INTERPRETER_TOKEN_TYPES_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "str.h"

#define LITERAL_TO_STRING_MAX_LENGTH 100


typedef enum TokenTypeEnum {
    TOKEN_LEFT_PAREN      =   0,
    TOKEN_RIGHT_PAREN     =   1,
    TOKEN_COMMA           =  10,
    TOKEN_DOT             =  11,
    // MINUS_BINARY =  49
    TOKEN_MINUS           =  50,
    TOKEN_PLUS            =  51,
    TOKEN_SLASH           =  52,
    TOKEN_STAR            =  53,
//    PERCENT         =  54,
    TOKEN_BANG            =  75,
    TOKEN_BANG_EQUAL      =  76,
    TOKEN_EQUAL           =  77,
    TOKEN_EQUAL_EQUAL     =  78,
    TOKEN_GREATER         =  79,
    TOKEN_GREATER_EQUAL   =  80,
    TOKEN_LESS            =  81,
    TOKEN_LESS_EQUAL      =  82,
    TOKEN_SEMICOLON       = 100,
    TOKEN_COLON           = 101,
    TOKEN_AND             = 190,
    TOKEN_OR              = 191,
    TOKEN_NOT             = 192,
    TOKEN_BOOL            = 200,
    TOKEN_TRUE            = 201,
    TOKEN_FALSE           = 202,
    TOKEN_IDENTIFIER      = 205,
    TOKEN_STRING          = 206,
//    INT             = 207,
//    FLOAT           = 208,
    TOKEN_NUMBER          = 209,
    TOKEN_VAR             = 210,
    TOKEN_IF              = 120,
    TOKEN_ELSE            = 121,
    TOKEN_FOR             = 122,
    TOKEN_WHILE           = 123,
//    FUNC            = 124,
//    CLASS           = 125,
    TOKEN_PRINT           = 253,
    TOKEN_NULL           = 254,
    TOKEN_END_OF_FILE     = 255,
} TokenTypeEnum;

typedef enum BinaryOperatorEnum {
    OP_BINARY_PLUS          = 51,
    OP_BINARY_MINUS         = 50,
    OP_BINARY_SLASH         = 52,
    OP_BINARY_STAR          = 53,
//    OP_BINARY_PERCENT,
    OP_BINARY_GREATER       = 79,
    OP_BINARY_GREATER_EQUAL = 80,
    OP_BINARY_LESS          = 81,
    OP_BINARY_LESS_EQUAL    = 82,
    OP_BINARY_BANG_EQUAL    = 77,
    OP_BINARY_EQUAL_EQUAL   = 78,
} BinaryOperatorEnum;

typedef enum UnaryOperatorEnum {
    OP_UNARY_MINUS          = 50,
    OP_UNARY_BANG           = 75,
} UnaryOperatorEnum;

typedef enum OperatorTypeEnum {
    OP_TYPE_UNARY,
    OP_TYPE_BINARY,
} OperatorTypeEnum;

typedef union OperatorEnum {
    BinaryOperatorEnum binary;
    UnaryOperatorEnum unary;
} OperatorEnum;

typedef struct Operator {
    OperatorTypeEnum operator_type;
    OperatorEnum operator_enum;
} Operator;

typedef enum KeywordEnum {
    KEYWORDENUM_AND,
    KEYWORDENUM_OR,
    KEYWORDENUM_NOT,
    KEYWORDENUM_TRUE,
    KEYWORDENUM_FALSE,
    KEYWORDENUM_IF,
    KEYWORDENUM_ELSE,
    KEYWORDENUM_FOR,
    KEYWORDENUM_WHILE,
    KEYWORDENUM_FUNC,
    KEYWORDENUM_CLASS,
    KEYWORDENUM_NULL
} KeywordEnum;

typedef union LiteralUnion {
    char *string;
//    int _int;
    float _float;
    void *null;
    bool _bool;
} LiteralUnion;

typedef union LiteralOrOperatorUnion {
    LiteralUnion literal;
    Operator operator;
} LiteralOrOperatorUnion;

typedef enum LiteralOrOperatorEnum {
    LITERAL,
    OPERATOR,
} LiteralOrOperatorEnum;

typedef struct LiteralOrOperator {
    LiteralOrOperatorUnion val;
    LiteralOrOperatorEnum t;
} LiteralOrOperator;

typedef enum LiteralTypeEnum {
    LITERALTYPE_NULL,
    LITERALTYPE_STRING,
//    LITERALTYPE_INT,
    LITERALTYPE_FLOAT,
    LITERALTYPE_BOOL,
    LITERALTYPE_IDENTIFIER,
    LITERALTYPE_OPERATOR,
    LITERALTYPE_KEYWORD,
} LiteralTypeEnum;

typedef struct Token {
    TokenTypeEnum TokenTypes;
    String lexeme;
    LiteralOrOperator literal_or_operator;
    LiteralTypeEnum literalType;
    unsigned long long line;
    void *address;
} Token;

extern char *t_literal_to_string(Token token) {
    switch (token.literalType) {
        case LITERALTYPE_NULL:
            return "NULL";
        case LITERALTYPE_STRING:
            return token.literal_or_operator.val.literal.string;
        case LITERALTYPE_FLOAT:
            char *buffer = malloc(LITERAL_TO_STRING_MAX_LENGTH);
            sprintf(buffer, "%f", token.literal_or_operator.val.literal._float);
            return buffer;
        case LITERALTYPE_BOOL:
            return token.literal_or_operator.val.literal._bool ? "TOKEN_TRUE" : "TOKEN_FALSE";
        case LITERALTYPE_IDENTIFIER:
        case LITERALTYPE_OPERATOR:
        case LITERALTYPE_KEYWORD:
            return token.lexeme.address;
    }
}

extern char *t_to_string(Token token) {
    char *literal = t_literal_to_string(token);
    char *buffer = malloc(LITERAL_TO_STRING_MAX_LENGTH);
    sprintf(buffer, "lexeme: %s literal: %s", token.lexeme.address, literal);
    return buffer;
}

#endif // INTERPRETER_TOKEN_TYPES_H
