#include <stdbool.h>
#include <stdio.h>

#include "cvector.h"

#include "scanner.h"
#include "input.h"
#include "token_types.h"
#include "parser.h"
#include "interpreter.h"
#include "error.h"

#define INPUT_MAX_LENGTH 4000

bool hadError = false;

//void runFile(char* filename);
void runPrompt(void);
void run(char* source);


int main(void) {
//    runPrompt();
    run("1<=1.5");
    return 0;
}

//void runFile(char* filename) {
//
//}

void runPrompt(void) {
    while (true) {
        char *input = readline(" > ");
        run(input);

        hadError = false;
    }
}

void run(char* source) {
    scanner_init(source);
    scanner_tokenize(source);
    for (size_t i = 0; i < cvector_size(Scanner._tokens); i++) {
        Token *token = cvector_at(Scanner._tokens, i);
        char* s = t_literal_to_string(*token);
        printf("%s ", s);
        // free(s);
    }
    if (hadError) {
        // exit
    }
}
