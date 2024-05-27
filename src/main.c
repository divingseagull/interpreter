#include <stdbool.h>
#include <stdio.h>

#include "scanner.h"
#include "input.h"
#include "token_types.h"


bool hasError = false;

void runFile(char *filename);
void runPrompt(void);


int main(void) {
    char* source = (char*)get_input(">");
    scanner_init(source);
    scanner_tokenize(source);
    scanner_scan_token();
    return 0;
}

void runFile(char *filename) {

}

void runPrompt(void) {

}
