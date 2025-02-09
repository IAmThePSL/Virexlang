#include "../../include/lexer/token.h"
#include "../../include/parser/malloc.h"
#include <string.h>
#include <stdlib.h>

Token *create_token(TokenType type, const char *lexeme, int line, int column) {
    Token *token = (Token *)safe_malloc(sizeof(Token));
    token->type = type;
    token->lexeme = strdup(lexeme);  // Use strdup to create a new copy
    token->line = line;
    token->column = column;
    return token;
}

void destroy_token(Token *token) {
    if (token) {
        free(token->lexeme);  // Free the lexeme string
        free(token);          // Free the token structure
    }
}
