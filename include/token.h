#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "token_types.h"

// Token structure
typedef struct {
    TokenType type;
    char *lexeme;
    int line;
    int column;
} Token;

// Function to create a new token
Token *create_token(TokenType type, const char *lexeme, int line, int column) {
    // Allocate memory for the token
    Token *token = (Token *)malloc(sizeof(Token));
    if (!token) {
        return NULL; // Return NULL if allocation fails
    }

    // Initialize the token's fields
    token->type = type;

    // Allocate memory for the lexeme and copy the string
    token->lexeme = (char *)malloc(strlen(lexeme) + 1);
    if (!token->lexeme) {
        free(token); // Free token memory if lexeme allocation fails
        return NULL;
    }
    strcpy(token->lexeme, lexeme);

    token->line = line;
    token->column = column;

    return token;
}

// Function to destroy a token and free its memory
void destroy_token(Token *token) {
    if (token) {
        free(token->lexeme);
        free(token);
    }
}

#endif // TOKEN_H
