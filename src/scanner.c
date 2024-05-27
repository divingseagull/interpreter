#include <stdbool.h>

#include "cvector.h"

#include "token_types.h"


void scanner_add_token(enum TokenTypeEnum tokenType, void* literal);
void scanner_add_token_enum(enum TokenTypeEnum tokenType);
bool isAtEnd(void);
char peek(void);
char peekNext(void);
bool match(char expected);
char advance(void);
char *substring(size_t start, size_t end);
void scanner_scan_token(void);
bool isDigit(char c);
bool isAlpha(char c);
bool isAlphaNumeric(char c);
void number_int(void);
void number_float(void);
void string(void);
void identifier(void);
void * parse_float(char *string);


struct {
    unsigned long long _start;
    unsigned long long _current;
    unsigned long long _line;
    char *_source;
    cvector_vector_type(struct Token) _tokens;
} Scanner;

void scanner_init(char *source) {
    Scanner._start = 0;
    Scanner._current = 0;
    Scanner._line = 1;
    Scanner._source = source;
}

cvector_vector_type(struct Token) scanner_tokenize(char *source) {
    char *context = NULL;
    char *token = strtok_s(source, " ", &context);

    while (token) {
        Scanner._start = Scanner._current;
        scanner_scan_token();
    }
    struct Token t_eof = {
            END_OF_FILE,
            "",
            NULL,
            Scanner._line
    };
    cvector_push_back(Scanner._tokens, t_eof);

    return Scanner._tokens;
}

void scanner_scan_token(void) {
    switch (advance()) {
        case '(':
            scanner_add_token_enum(LEFT_PAREN);
            break;
        case ')':
            // RIGHT_PAREN
            scanner_add_token_enum(RIGHT_PAREN);
            break;
        case ',':
            // COMMA
            scanner_add_token_enum(COMMA);
            break;
        case '.':
            // DOT
            scanner_add_token_enum(DOT);
            break;
        case '-':
            // MINUS
            scanner_add_token_enum(MINUS);
            break;
        case '+':
            // PLUS
            scanner_add_token_enum(PLUS);
            break;
        case '/':
            // SLASH, COMMENT
            if (peekNext() == '/') {  // COMMENT
                while (peek() != '\n' && !isAtEnd()) {
                    Scanner._current++;
                }
            } else {  // SLASH
                scanner_add_token_enum(SLASH);
            }
            break;
        case '*':
            // STAR
            scanner_add_token_enum(STAR);
            break;
        case '%':
            // PERCENT (MODULO)
            scanner_add_token_enum(PERCENT);
            break;
        case ';':
            // SEMICOLON
            scanner_add_token_enum(SEMICOLON);
            break;
        case ':':
            scanner_add_token_enum(COLON);
            // COLON
            break;
        case '!':
            // BANG, BANG_EQUAL
            scanner_add_token_enum(
                    match('=') ? BANG_EQUAL : BANG
            );
            break;
        case '=':
            // EQUAL, EQUAL_EQUAL
            scanner_add_token_enum(
                    match('=') ? EQUAL_EQUAL : EQUAL
            );
            break;
        case '<':
            // GREATER, GREATER_EQUAL
            scanner_add_token_enum(
                    match('=') ? GREATER_EQUAL : GREATER
            );
            break;
        case '>':
            // LESS, LESS_EQUAL
            scanner_add_token_enum(
                    match('=') ? LESS_EQUAL : LESS
            );
            break;

        case ' ':
        case '\t':
        case '\r':
            break;

        case '\n':
            Scanner._line++;
            break;

        default:
            if (isDigit(peek())) {

            } else if (isAlpha(peek())) {
                identifier();
            } else {
                // error, unexpected character
            }
            break;
    }
}

void scanner_add_token_enum(enum TokenTypeEnum tokenType) {
    scanner_add_token(tokenType, NULL);
}

void scanner_add_token(enum TokenTypeEnum tokenType, void* literal) {
    char *text = substring(Scanner._start, Scanner._current);
    struct Token token = {
            tokenType,
            text,
            literal,
            Scanner._line
    };
    cvector_push_back(Scanner._tokens, token);
}

bool isAtEnd(void) {
    return Scanner._start >= (sizeof(Scanner._source) / 4);
}

char peek(void) {
    if (isAtEnd()) return '\0';
    return Scanner._source[Scanner._current];
}

char peekNext(void) {
    if (Scanner._current + 1 >= strlen(Scanner._source)) return '\0';
    return Scanner._source[Scanner._current+1];
}

bool match(char expected) {
    if (isAtEnd()) return false;
    if (peek() != expected) return false;

    Scanner._current++;
    return true;
}

char advance(void) {
    return Scanner._source[Scanner._current++];
}

char *substring(size_t start, size_t end) {
    char *buf = (char*)malloc(sizeof(char) * 1);
    char *substr = strncpy(buf, Scanner._source + start,
                         start - end);
    return substr;
}

bool isDigit(char c) {
    return ('0' <= c && c <= '9');
}

bool isAlpha(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           (c == '_');
}

bool isAlphaNumeric(char c) {
    return isDigit(c) || isAlpha(c);
}

void number_int(void) {
    while (isDigit(peek()) || peek() == '_') advance();


}

void number_float(void) {
    while (isDigit(peek()) || peek() == '_') advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();

        while (isDigit(peek())) {
            advance();
        }
    } else {
        // error, no dot
    }

    scanner_add_token(FLOAT,
                      (long double *) parse_float(substring(Scanner._start, Scanner._current)));
}

void *parse_float(char *string) {

}

void string(void) {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            Scanner._line++;
        }
        advance();
    }

    if (isAtEnd()) {
        // error: unterminated string
    }

    advance();

    char *value = substring(Scanner._start + 1, Scanner._current);
    scanner_add_token(STRING, (char *) value);
}

void identifier(void) {

}
