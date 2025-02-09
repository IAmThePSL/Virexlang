#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer/lexer.h"
#include "../include/lexer/token.h"
#include "../include/lexer/token_types.h"
#include "../include/parser/malloc.h"

void print_token(Token *token) {
    printf("Token(Type: %d, Lexeme: '%s', Line: %d, Column: %d)\n",
           token->type, token->lexeme, token->line, token->column);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    Lexer *lexer = create_lexer(file);
    Token *token;

    printf("Lexing file: %s\n", argv[1]);
    while (1) {
        token = lex_next_token(lexer);
        print_token(token);
        if (token->type == TOKEN_EOF) {
            destroy_token(token); // Free the EOF token
            break; // Stop after EOF
        }
        destroy_token(token); // Free non-EOF tokens
    }

    fclose(file);
    destroy_lexer(lexer);
    return EXIT_SUCCESS;
}
