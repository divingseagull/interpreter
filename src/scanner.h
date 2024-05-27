#include <stdbool.h>

#ifndef INTERPRETER_SCANNER_H
#define INTERPRETER_SCANNER_H


bool isAtEnd(void);
void scanner_init(char *source);
void scanner_tokenize(char *source);
void scanner_scan_token(void);

#endif //INTERPRETER_SCANNER_H
