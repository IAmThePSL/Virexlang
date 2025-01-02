#include "../include/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>

// Create a token
Token *create_token(TokenType type, const char *lexeme, int line, int column) {
    Token *token = (Token *)malloc(sizeof(Token));
    if (!token) {
        fprintf(stderr, "Error: Memory allocation failed for token\n");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->lexeme = strdup(lexeme);
    token->line = line;
    token->column = column;
    return token;
}

// Destroy a token
void destroy_token(Token *token) {
    if (token) {
        free(token->lexeme);
        free(token);
    }
}
