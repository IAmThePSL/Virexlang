#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

// Lexer structure definition
typedef struct Lexer Lexer;

// Lexer API
Lexer *create_lexer(FILE *file);
void destroy_lexer(Lexer *lexer);
Token *lex_next_token(Lexer *lexer);

#endif // LEXER_H