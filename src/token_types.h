#include "str.h"

#ifndef INTERPRETER_TOKEN_TYPES_H
#define INTERPRETER_TOKEN_TYPES_H


enum TokenTypeEnum {
    LEFT_PAREN      =   0,
    RIGHT_PAREN     =   1,
    COMMA           =   2,
    DOT             =   3,
    MINUS           =   4,
    PLUS            =   5,
    SLASH           =   6,
    STAR            =   7,
    PERCENT         =   8,
    SEMICOLON       =   9,
    COLON           =  10,
    BANG            =  11,
    BANG_EQUAL      =  12,
    EQUAL           =  13,
    EQUAL_EQUAL     =  14,
    GREATER         =  15,
    GREATER_EQUAL   =  16,
    LESS            =  17,
    LESS_EQUAL      =  18,
    TRUE            =  19,
    FALSE           =  20,
    IDENTIFIER      =  21,
    STRING          =  22,
    INT             =  23,
    FLOAT           =  24,
    AND             =  25,
    OR              =  26,
    NOT             =  27,
    // IF, ELSE,
    // FOR, WHILE,
    // FUNC, CLASS
    _NULL           = 254,
    END_OF_FILE     = 255
};

typedef union LiteralUnion {
    Str *string;
    int _int;
    float _float;
    void *null;
} _literal;

struct Token {
    enum TokenTypeEnum TokenTypes;
    char *lexeme;
    _literal literal;
    unsigned long long line;
};


#endif //INTERPRETER_TOKEN_TYPES_H
