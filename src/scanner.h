#ifndef INTERPRETER_SCANNER_H
#define INTERPRETER_SCANNER_H

#include <stdbool.h>

#include "cvector.h"
#include "hashmap.h"

#include "token_types.h"
#include "scanner.h"


void scanner_init(char *source);
void scanner_add_token(TokenTypeEnum tokenType, LiteralUnion *literal, LiteralTypeEnum literalType);
void scanner_add_token_enum(TokenTypeEnum tokenType, LiteralTypeEnum literalType);
bool scanner_is_at_end(void);
char scanner_peek(void);
char scanner_peek_next(void);
bool scanner_match(char expected);
char scanner_advance(void);
char scanner_retreat(void);
char *scanner_substring(size_t start, size_t end);
void scanner_scan_token(void);
bool scanner_is_digit(char c);
bool scanner_is_alpha(char c);
bool scanner_is_alpha_numeric(char c);
void scanner_number(void);
void scanner_string(void);
void scanner_identifier(void);
void *parse_float(char *string);

struct keywords {
    char *keyword_str;
    KeywordEnum keyword_value;
};

struct {
    unsigned long long _start;
    unsigned long long _current;
    unsigned long long _line;
    char *_source;
    cvector_vector_type(Token) _tokens;
} Scanner;

inline void scanner_init(char *source) {
    Scanner._start = 0;
    Scanner._current = 0;
    Scanner._line = 1;
    Scanner._source = source;
}

extern inline cvector_vector_type(Token) scanner_tokenize(char *source) {
    while (!scanner_is_at_end()) {
        Scanner._start = Scanner._current;
        scanner_scan_token();
    }
    const Token t_eof = {
            END_OF_FILE,
            "",
            { .null =  NULL },
            LiteralTypeString[LITERALTYPE_NULL],
            Scanner._line,
    };
    cvector_push_back(Scanner._tokens, t_eof);

    return Scanner._tokens;
}

inline void scanner_scan_token(void) {
    switch (scanner_peek()) {
        case '(':
            scanner_add_token_enum(LEFT_PAREN, LITERALTYPE_OPERATOR);
            break;
        case ')':
            // RIGHT_PAREN
            scanner_add_token_enum(RIGHT_PAREN, LITERALTYPE_OPERATOR);
            break;
        case ',':
            // COMMA
            scanner_add_token_enum(COMMA, LITERALTYPE_OPERATOR);
            break;
        case '.':
            // DOT
            scanner_add_token_enum(DOT, LITERALTYPE_OPERATOR);
            break;
        case '-':
            // MINUS
            scanner_add_token_enum(MINUS, LITERALTYPE_OPERATOR);
            break;
        case '+':
            // PLUS
            scanner_add_token_enum(PLUS, LITERALTYPE_OPERATOR);
            break;
        case '/':
            // SLASH, COMMENT
            if (scanner_peek_next() == '/') {  // COMMENT
                while (scanner_peek() != '\n' && !scanner_is_at_end()) {
                    Scanner._current++;
                }
            } else {  // SLASH
                scanner_add_token_enum(SLASH, LITERALTYPE_OPERATOR);
            }
            break;
        case '*':
            // STAR
            scanner_add_token_enum(STAR, LITERALTYPE_OPERATOR);
            break;
        case '%':
            // PERCENT (MODULO)
            scanner_add_token_enum(PERCENT, LITERALTYPE_OPERATOR);
            break;
        case ';':
            // SEMICOLON
            scanner_add_token_enum(SEMICOLON, LITERALTYPE_OPERATOR);
            break;
        case ':':
            scanner_add_token_enum(COLON, LITERALTYPE_OPERATOR);
            // COLON
            break;
        case '!':
            // BANG, BANG_EQUAL
            scanner_add_token_enum(
                    scanner_match('=') ? BANG_EQUAL : BANG,
                    LITERALTYPE_OPERATOR
            );
            break;
        case '=':
            // EQUAL, EQUAL_EQUAL
            scanner_add_token_enum(
                    scanner_match('=') ? EQUAL_EQUAL : EQUAL,
                    LITERALTYPE_OPERATOR
            );
            break;
        case '<':
            // GREATER, GREATER_EQUAL
            scanner_add_token_enum(
                    scanner_match('=') ? GREATER_EQUAL : GREATER,
                    LITERALTYPE_OPERATOR
            );
            break;
        case '>':
            // LESS, LESS_EQUAL
            scanner_add_token_enum(
                    scanner_match('=') ? LESS_EQUAL : LESS,
                    LITERALTYPE_OPERATOR
            );
            break;

        case ' ':
        case '\t':
            break;

        case '\n':
            Scanner._line++;
            break;

        case '"':
            scanner_string();
            break;

        default:
            if (scanner_is_digit(scanner_peek())) {
                scanner_number();
            } else if (scanner_is_alpha(scanner_peek())) {
                scanner_identifier();
            } else {
                // error, unexpected character
            }
            break;
    }
    scanner_advance();
}

inline void scanner_add_token_enum(TokenTypeEnum tokenType, LiteralTypeEnum literalType) {
    LiteralUnion u = {
        .null = NULL
    };
    scanner_add_token(tokenType, &u, literalType);
}

inline void scanner_add_token(TokenTypeEnum tokenType, LiteralUnion *literal, LiteralTypeEnum literalType) {
    char *substring = scanner_substring(Scanner._start, Scanner._current);
    char *text = calloc(strlen(substring)+1, sizeof(char));
    strcpy(text, substring);
    // free(substring);
    const Token token = {
        .TokenTypes = tokenType,
        .lexeme = text,
        .literalType = LiteralTypeString[literalType],
        .line = Scanner._line,
        .literal = {
            .null = literal->null,
            ._int = literal->_int,
            ._float = literal->_float
        }
    };
    cvector_push_back(Scanner._tokens, token);
}

inline bool scanner_is_at_end(void) {
    return Scanner._start >= strlen(Scanner._source);
}

inline char scanner_peek(void) {
    if (scanner_is_at_end()) return '\0';
    return Scanner._source[Scanner._current];
}

inline char scanner_peek_next(void) {
    if (Scanner._current + 1 >= strlen(Scanner._source)) return '\0';
    return Scanner._source[Scanner._current+1];
}

inline bool scanner_match(char expected) {
    if (scanner_is_at_end()) return false;
    if ((scanner_peek_next() != expected)) return false;

    scanner_advance();
    return true;
}

inline char scanner_advance(void) {
    return Scanner._source[Scanner._current++];
}

inline char scanner_retreat(void) {
    return Scanner._source[Scanner._current--];
}

inline char *scanner_substring(size_t start, size_t end) {
    char *buf = calloc(end - start + 2, sizeof(char));
    memcpy(buf, Scanner._source + start, end - start + 1);
    buf[end - start + 1] = '\0';
    return buf;
}

inline bool scanner_is_digit(char c) {
    return ('0' <= c && c <= '9');
}

inline bool scanner_is_alpha(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           (c == '_');
}

inline bool scanner_is_alpha_numeric(char c) {
    return scanner_is_digit(c) || scanner_is_alpha(c);
}

inline void scanner_number(void) {
    bool isFloat = false;
    while (scanner_is_digit(scanner_peek()) || scanner_peek() == '_') scanner_advance();
    if (scanner_peek() == ' ') scanner_retreat();

    if (scanner_peek() == '.' && scanner_is_digit(scanner_peek_next())) {
        isFloat = true;
        scanner_advance();

        while (scanner_is_digit(scanner_peek())) {
            scanner_advance();
        }
    }

    void *end_ptr = NULL;
    char *substring = scanner_substring(Scanner._start, Scanner._current);  // need to deallocation

    if (isFloat) {
        float literal = strtof(substring, end_ptr);
        LiteralUnion u = {
            ._float = literal
        };
        scanner_add_token(
            NUMBER,
            &u,
            LITERALTYPE_FLOAT
        );
    } else {
        int literal = strtod(substring, end_ptr);
        LiteralUnion u = {
            ._int = literal
        };
        scanner_add_token(
            NUMBER,
            &u,
            LITERALTYPE_INT
        );
    }
    // free(substring);
}

inline void scanner_string(void) {
    while (scanner_peek() != '"' && !scanner_is_at_end()) {
        if (scanner_peek() == '\n') {
            Scanner._line++;
        }
        scanner_advance();
    }

    if (scanner_is_at_end()) {
        // error: unterminated scanner_string
    }

    scanner_advance();

    char *value = scanner_substring(Scanner._start + 1, Scanner._current);  // need to deallocation
    LiteralUnion u = {
        .string = value
    };
    scanner_add_token(STRING, &u, LITERALTYPE_STRING);
}

inline void scanner_identifier(void) {
    while (scanner_is_alpha_numeric(scanner_peek())) scanner_advance();

    char *text = scanner_substring(Scanner._start, Scanner._current);
    // hashmap_new(sizeof(struct keywords), sizeof(KeywordEnum) / 4, )
    free(text);
}



#endif //INTERPRETER_SCANNER_H
