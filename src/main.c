#include "error.h"
#include "environment.h"
#include "interpreter.h"
#include "scanner.h"
#include "parser.h"
#include "input.h"

#define loop while(1)

void runFile(char* filename);
void runPrompt(void);
void run(char* source);

void init(char* source);
void clean(void);

/*
 * 사용 가능한 연산자:
 *      단항 연산자:
 *          -: <NUMBER>
 *          !: <BOOL>
 *
 */

int main(void) {
//    runPrompt();
    char *source = "print !!\"asdf\";";
    init(source);
    run(source);
    clean();
    system("pause");
    return 0;
}

void runPrompt(void) {
    loop {
        // ReSharper disable once CppDFAEndlessLoop
        char *input = readline(" > ");
        init(input);
        run(input);
    }
}

void init(char* source) {
    environment_init();
    interpreter_init();
    error_handler_init();
    scanner_init(source);
}

void clean(void) {
//    이중 해제됨
//    for (size_t i = 0; i < cvector_size(string_list); i++) {
//        free(cvector_at(string_list, i)->address);  // SIGTRAP
//    }
//    for (size_t i = 0; i < cvector_size(Scanner._tokens); i++) {
//        free(cvector_at(Scanner._tokens, i)->address);  // SIGTRAP
//    }
    cvector_free(string_list);
    cvector_free(Scanner._tokens);
//    cvector_free(parser_tokens);  // Segmentation fault, 빈 벡터: 할당 해제할 필요 없음
    cvector_free(Interpreter.expressions);
    cvector_free(Interpreter.statements);
//    cvector_free(box_list);
    exit(0);
}

void print_tokens(void) {
    // 토큰화된 토큰들을 출력
    printf("Tokens:\n");
    for (size_t i = 0; i < cvector_size(Scanner._tokens); i++) {
        Token *token = cvector_at(Scanner._tokens, i);
        // "print\253\253\253\253..."
        // 70 72 69 6e   74 ab ab ab
        char* s = t_literal_to_string(*token);
        printf("%s ", s);
    }
    printf("\n");
}

void run(char* source) {
    parser_tokens = scanner_tokenize(source);

    print_tokens();
    // print <Literal>;
    cvector_vector_type(Stmt) statements = parser_parse();

    interpret(statements);
}
