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
    PERCENT         =  54,
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
    INT             = 207,
    FLOAT           = 208,
    NUMBER          = 209,
    IF              = 120,
    ELSE            = 121,
    FOR             = 122,
    WHILE           = 123,
    FUNC            = 124,
    CLASS           = 125,
    _NULL           = 254,
    END_OF_FILE     = 255,
} TokenTypeEnum;

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
    int _int;
    float _float;
    void *null;
} LiteralUnion;

typedef struct {
    TokenTypeEnum TokenTypes;
    char *lexeme;
    LiteralUnion literal;
    char *literalType;
    unsigned long long line;
} Token;

typedef enum {
    LITERALTYPE_NULL,
    LITERALTYPE_STRING,
    LITERALTYPE_INT,
    LITERALTYPE_FLOAT,
    LITERALTYPE_IDENTIFIER,
    LITERALTYPE_OPERATOR,
    LITERALTYPE_KEYWORD,
} LiteralTypeEnum;

char *LiteralTypeString[] = {
    [LITERALTYPE_NULL]       = "NULL",
    [LITERALTYPE_STRING]     = "STRING",
    [LITERALTYPE_INT]        = "INT",
    [LITERALTYPE_FLOAT]      = "FLOAT",
    [LITERALTYPE_IDENTIFIER] = "IDENTIFIER",
    [LITERALTYPE_OPERATOR]   = "OPERATOR",
    [LITERALTYPE_KEYWORD]    = "KEYWORD",
};


extern inline char *t_literal_to_string(Token token) {
    if (strcmp(token.literalType, LiteralTypeString[LITERALTYPE_NULL]) == 0) return "NULL";
    if (strcmp(token.literalType, LiteralTypeString[LITERALTYPE_STRING]) == 0) return token.literal.string;
    if (strcmp(token.literalType, LiteralTypeString[LITERALTYPE_INT]) == 0) {
        char *buffer = malloc(LITERAL_TO_STRING_MAX_LENGTH);
        sprintf(buffer, "%d", token.literal._int);
        return buffer;
    }
    if (strcmp(token.literalType, LiteralTypeString[LITERALTYPE_FLOAT]) == 0) {
        char *buffer = malloc(LITERAL_TO_STRING_MAX_LENGTH);
        sprintf(buffer, "%f", token.literal._float);
        return buffer;
    }
    if (strcmp(token.literalType, LiteralTypeString[LITERALTYPE_IDENTIFIER]) == 0) {
        return token.lexeme;
    }
    if (strcmp(token.literalType, LiteralTypeString[LITERALTYPE_OPERATOR]) == 0) {
        return token.lexeme;
    }
    if (strcmp(token.literalType, LiteralTypeString[LITERALTYPE_KEYWORD]) == 0) {
        return token.lexeme;
    }
}

extern inline char *t_to_string(Token token) {
    char *literal = t_literal_to_string(token);
    char *buffer = malloc(LITERAL_TO_STRING_MAX_LENGTH);
    sprintf(buffer, "type: %s lexeme: %s literal: %s", token.literalType, token.lexeme, literal);
    return buffer;
}

#endif // INTERPRETER_TOKEN_TYPES_H
