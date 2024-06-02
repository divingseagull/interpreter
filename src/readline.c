char* readline(char *prompt) {
    char buffer[4000] = {0};
    unsigned int idx = 0;
    char *ptr = malloc(sizeof(buffer));

    while (true) {
        char c = (char)getchar();

        if (c == '\n' || c == EOF) {
            break;
        } else {
            buffer[idx++] = c;
        }
    }

    return ptr;
}
