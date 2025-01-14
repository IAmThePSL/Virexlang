#include "../include/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a token
Token *create_token(TokenType type, const char *lexeme, int line, int column) {
    Token *token = (Token *)malloc(sizeof(Token));
    if (!token) {
        fprintf(stderr, "Error: Memory allocation failed for token\n");
        exit(EXIT_FAILURE);  // Handle failure by exiting the program
    }
    token->type = type;
    token->lexeme = strdup(lexeme);  // strdup handles memory allocation and copying
    if (!token->lexeme) {
        free(token);  // Free token memory if lexeme allocation fails
        fprintf(stderr, "Error: Memory allocation failed for lexeme\n");
        exit(EXIT_FAILURE);  // Exit if lexeme allocation fails
    }
    token->line = line;
    token->column = column;
    return token;
}

// Destroy a token
void destroy_token(Token *token) {
    if (token) {
        free(token->lexeme);  // Free the lexeme memory
        free(token);  // Free the token structure
    }
}
