#ifndef INTERPRETER_SCANNER_H
#define INTERPRETER_SCANNER_H

#include <stdbool.h>

#include "cvector.h"

#include "types_token.h"
#include "str.h"


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
static void scanner_scan_token(void);
bool scanner_is_digit(char c);
bool scanner_is_alpha(char c);
bool scanner_is_alpha_numeric(char c);
static void scanner_number(void);
void scanner_string(void);
void scanner_identifier(void);


struct Scanner {
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
    Token t_eof = {
            .TokenTypes = TOKEN_END_OF_FILE,
            .lexeme = string_empty(),
            .literal_or_operator = {
                    .val.literal.null = NULL,
                    .t = LITERAL,
            },
            .literalType = LITERALTYPE_NULL,
            .line = Scanner._line,
            .address = &t_eof
    };
    cvector_push_back(Scanner._tokens, t_eof);

    return Scanner._tokens;
}

inline void scanner_scan_token(void) {
    switch (scanner_peek()) {
        case '(':
            scanner_add_token_enum(TOKEN_LEFT_PAREN, LITERALTYPE_OPERATOR);
            break;
        case ')':
            // TOKEN_RIGHT_PAREN
            scanner_add_token_enum(TOKEN_RIGHT_PAREN, LITERALTYPE_OPERATOR);
            break;
        case ',':
            // TOKEN_COMMA
            scanner_add_token_enum(TOKEN_COMMA, LITERALTYPE_OPERATOR);
            break;
        case '.':
            // TOKEN_DOT
            scanner_add_token_enum(TOKEN_DOT, LITERALTYPE_OPERATOR);
            break;
        case '-':
            // TOKEN_MINUS
            scanner_add_token_enum(TOKEN_MINUS, LITERALTYPE_OPERATOR);
            break;
        case '+':
            // TOKEN_PLUS
            scanner_add_token_enum(TOKEN_PLUS, LITERALTYPE_OPERATOR);
            break;
        case '/':
            // TOKEN_SLASH, DOUBLE_SLASH
            if (scanner_peek_next() == '/') {  // DOUBLE_SLASH
                while (scanner_peek() != '\n' && !scanner_is_at_end()) {
                    scanner_advance();
                }
            } else {  // TOKEN_SLASH
                scanner_add_token_enum(TOKEN_SLASH, LITERALTYPE_OPERATOR);
            }
            break;
        case '*':
            // TOKEN_STAR
            scanner_add_token_enum(TOKEN_STAR, LITERALTYPE_OPERATOR);
            break;
//        case '%':
//            // PERCENT (MODULO)
//            scanner_add_token_enum(PERCENT, LITERALTYPE_OPERATOR);
//            break;
        case ';':
            // TOKEN_SEMICOLON
            scanner_add_token_enum(TOKEN_SEMICOLON, LITERALTYPE_OPERATOR);
            break;
        case ':':
            scanner_add_token_enum(TOKEN_COLON, LITERALTYPE_OPERATOR);
            // TOKEN_COLON
            break;
        case '!':
            // TOKEN_BANG, TOKEN_BANG_EQUAL
            scanner_add_token_enum(
                    scanner_match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG,
                    LITERALTYPE_OPERATOR
            );
            break;
        case '=':
            // TOKEN_EQUAL, TOKEN_EQUAL_EQUAL
            scanner_add_token_enum(
                    scanner_match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL,
                    LITERALTYPE_OPERATOR
            );
            break;
        case '<':
            // TOKEN_GREATER, TOKEN_GREATER_EQUAL
            scanner_add_token_enum(
                    scanner_match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER,
                    LITERALTYPE_OPERATOR
            );
            break;
        case '>':
            // TOKEN_LESS, TOKEN_LESS_EQUAL
            scanner_add_token_enum(
                    scanner_match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS,
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
    Token token = {
        .TokenTypes = tokenType,
        .lexeme = string_from_array(text),
        .literalType = literalType,
        .line = Scanner._line,
        .literal_or_operator.t = LITERAL,
        .address = &token
    };
    switch (literalType) {
        case LITERALTYPE_NULL:
            token.literal_or_operator.val.literal.null = literal->null;
            break;
        case LITERALTYPE_STRING:
            token.literal_or_operator.val.literal.string = literal->string;
            break;
        case LITERALTYPE_FLOAT:
            token.literal_or_operator.val.literal._float = literal->_float;
            break;
        case LITERALTYPE_BOOL:
            token.literal_or_operator.val.literal._bool = literal->_bool;
            break;
    }
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
    // start - end = 0일 경우에도 1글자도 토큰화 할 수 있도록 최소한의 크기를 보장
    size_t size = (end - start) > 1 ? (end - start) : 1;
    char *buf = calloc(size, sizeof(char));
    memcpy(buf, Scanner._source + start, size);
    buf[size] = '\0';
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
    while (scanner_is_digit(scanner_peek())) scanner_advance();

    if (scanner_peek() == '.' && scanner_is_digit(scanner_peek_next())) {
        isFloat = true;
        scanner_advance();

        while (scanner_is_digit(scanner_peek())) {
            scanner_advance();
        }
    }

    if (!scanner_is_digit(scanner_peek())) scanner_retreat();

    void *end_ptr = NULL;
    char *substring = scanner_substring(Scanner._start, Scanner._current);  // need to deallocation

    isFloat = true;
    if (isFloat) {
        float literal = strtof(substring, end_ptr);
        LiteralUnion u = {
            ._float = literal
        };
        scanner_add_token(
                TOKEN_NUMBER,
                &u,
                LITERALTYPE_FLOAT
        );
    }/* else {
        int literal = strtod(substring, end_ptr);
        LiteralUnion u = {
            ._int = literal
        };
        scanner_add_token(
            TOKEN_NUMBER,
            &u,
            LITERALTYPE_INT
        );
    }*/
    // free(substring);
}

inline void scanner_string(void) {
    scanner_advance();
    while (scanner_peek() != '"' && !scanner_is_at_end()) {
        if (scanner_peek() == '\n') {
            Scanner._line++;
        }
        scanner_advance();
    }

    if (scanner_is_at_end()) {
        // error: unterminated scanner_string
    }

    // 시작한 지점은 겹따옴표, 끝나는 지점은 scanner_advance를 호출하지 않기 때문에 +1
    char *value = scanner_substring(Scanner._start, Scanner._current+1);
    LiteralUnion u = {
        .string = value
    };
    scanner_add_token(TOKEN_STRING, &u, LITERALTYPE_STRING);
}

inline void scanner_identifier(void) {
    // TODO: 변수 식별자는 맨 앞글자가 잘림
    while (scanner_is_alpha_numeric(scanner_advance())) {
        if (scanner_peek() == ';' ||
            scanner_peek() == ' ') break;
    };

    char *text = scanner_substring(Scanner._start, Scanner._current);

    LiteralUnion u = {
            .string = text
    };
    TokenTypeEnum tokenType;
    if (strcmp(text, "true") == 0) {
        LiteralUnion literal_true = {
                ._bool = true,
        };
        scanner_add_token(TOKEN_BOOL, &literal_true, LITERALTYPE_BOOL);
        if (scanner_peek() == ';') scanner_retreat();
        return;
    }
    if (strcmp(text, "false") == 0) {
        LiteralUnion literal_false = {
                ._bool = false,
        };
        scanner_add_token(TOKEN_BOOL, &literal_false, LITERALTYPE_BOOL);
        if (scanner_peek() == ';') scanner_retreat();
        return;
    }
    // keywords
    else if (strcmp(text, "and") == 0) {
        tokenType = TOKEN_AND;
    }
    else if (strcmp(text, "or") == 0) {
        tokenType = TOKEN_OR;
    }
    else if (strcmp(text, "not") == 0) {
        tokenType = TOKEN_NOT;
    }
    else if (strcmp(text, "if") == 0) {
        tokenType = TOKEN_IF;
    }
    else if (strcmp(text, "else") == 0) {
        tokenType = TOKEN_ELSE;
    }
    else if (strcmp(text, "for") == 0) {
        tokenType = TOKEN_FOR;
    }
    else if (strcmp(text, "while") == 0) {
        tokenType = TOKEN_WHILE;
    }
    else if (strcmp(text, "var") == 0) {
        tokenType = TOKEN_VAR;
    }
    else if (strcmp(text, "print") == 0) {
        tokenType = TOKEN_PRINT;
    }
    else {
        tokenType = TOKEN_IDENTIFIER;
        scanner_add_token(tokenType, &u, LITERALTYPE_IDENTIFIER);
        if (scanner_peek() == ';') scanner_retreat();
        return;
    }
    scanner_add_token(tokenType, &u, LITERALTYPE_KEYWORD);
    if (scanner_peek() == ';') scanner_retreat();
}

#endif //INTERPRETER_SCANNER_H
