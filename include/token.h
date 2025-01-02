#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include "token_types.h"

// Token structure
typedef struct {
    TokenType type;     // Type of the token
    char *lexeme;       // The string representation of the token
    int line;           // Line number where the token appears
    int column;         // Column number where the token starts
} Token;

// Function to create a new token
Token *create_token(TokenType type, const char *lexeme, int line, int column);

// Function to destroy a token and free its memory
void destroy_token(Token *token);

#endif // TOKEN_H
