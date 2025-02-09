#ifndef TOKEN_H
#define TOKEN_H

#include "token_types.h"

typedef struct Token {
    TokenType type;
    char *lexeme;
    int line;
    int column;
} Token;

// Function prototypes
Token *create_token(TokenType type, const char *lexeme, int line, int column);
void destroy_token(Token *token);

#endif // TOKEN_H
