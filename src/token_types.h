#ifndef INTERPRETER_TOKEN_TYPES_H
#define INTERPRETER_TOKEN_TYPES_H

#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#define LITERAL_TO_STRING_MAX_LENGTH 100

typedef enum {
    LEFT_PAREN      =   0,
    RIGHT_PAREN     =   1,
    COMMA           =  10,
    DOT             =  11,
    MINUS           =  50,
    PLUS            =  51,
    SLASH           =  52,
    STAR            =  53,
//    PERCENT         =  54,
    BANG            =  75,
    BANG_EQUAL      =  76,
    EQUAL           =  77,
    EQUAL_EQUAL     =  78,
    GREATER         =  79,
    GREATER_EQUAL   =  80,
    LESS            =  81,
    LESS_EQUAL      =  82,
    SEMICOLON       = 100,
    COLON           = 101,
    AND             = 190,
    OR              = 191,
    NOT             = 192,
    TRUE            = 200,
    FALSE           = 201,
    IDENTIFIER      = 205,
    STRING          = 206,
//    INT             = 207,
    FLOAT           = 208,
    NUMBER          = 209,
    IF              = 120,
    ELSE            = 121,
    FOR             = 122,
    WHILE           = 123,
    FUNC            = 124,
    CLASS           = 125,
    PRINT           = 253,
    _NULL           = 254,
    END_OF_FILE     = 255,
} TokenTypeEnum;

typedef enum {
    OP_LEFT_PAREN,
    OP_RIGHT_PAREN,
} OperatorGroupingEnum;

typedef enum {
    OP_PLUS          = 51,
    OP_MINUS_BINARY  = 50,
    OP_SLASH         = 52,
    OP_STAR          = 53,
//    OP_PERCENT,
    OP_GREATER       = 79,
    OP_GREATER_EQUAL = 80,
    OP_LESS          = 81,
    OP_LESS_EQUAL    = 82,
    OP_BANG_EQUAL    = 77,
    OP_EQUAL_EQUAL   = 78,
} OperatorBinaryEnum;

typedef enum {
    OP_MINUS_UNARY,
    OP_BANG,
} OperatorUnaryEnum;

typedef union {
    OperatorBinaryEnum binary_enum;
    OperatorUnaryEnum unary_enum;
    OperatorGroupingEnum grouping_enum;
} OperatorUnion;

typedef enum {
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

typedef union {
    char *string;
//    int _int;
    float _float;
    void *null;
} LiteralUnion;

typedef union {
    LiteralUnion literal;
    OperatorUnion operator;
} LiteralOrOperator;

typedef enum {
    LITERALTYPE_NULL,
    LITERALTYPE_STRING,
//    LITERALTYPE_INT,
    LITERALTYPE_FLOAT,
    LITERALTYPE_IDENTIFIER,
    LITERALTYPE_OPERATOR,
    LITERALTYPE_KEYWORD,
} LiteralTypeEnum;

typedef struct {
    TokenTypeEnum TokenTypes;
    char *lexeme;
    LiteralOrOperator literal_or_operator;
    LiteralTypeEnum literalType;
    unsigned long long line;
    void *address;
} Token;

extern inline char *t_literal_to_string(Token token) {
    switch (token.literalType) {
        case LITERALTYPE_NULL:
            return "NULL";
        case LITERALTYPE_STRING:
            return token.literal_or_operator.literal.string;
        case LITERALTYPE_FLOAT:
            char *buffer = malloc(LITERAL_TO_STRING_MAX_LENGTH);
            sprintf(buffer, "%f", token.literal_or_operator.literal._float);
            return buffer;
        case LITERALTYPE_IDENTIFIER:
            return token.lexeme;
        case LITERALTYPE_OPERATOR:
            return token.lexeme;
        case LITERALTYPE_KEYWORD:
            return token.lexeme;
    }
}

extern inline char *t_to_string(Token token) {
    char *literal = t_literal_to_string(token);
    char *buffer = malloc(LITERAL_TO_STRING_MAX_LENGTH);
    sprintf(buffer, "lexeme: %s literal: %s", token.lexeme, literal);
    return buffer;
}

#endif // INTERPRETER_TOKEN_TYPES_H
