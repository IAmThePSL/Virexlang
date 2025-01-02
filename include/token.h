#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

// Token types enumeration
typedef enum {
    TOKEN_INT,
    TOKEN_STR,
    TOKEN_BOOL,
    TOKEN_LET,
    TOKEN_CONST,
    TOKEN_FUNCTION,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_CLASS,
    TOKEN_IMPORT,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_ASSIGN,

    TOKEN_IDENTIFIER,
    TOKEN_INTEGER_LITERAL,
    TOKEN_STRING_LITERAL,

    TOKEN_OPERATOR, // do we actually need this?
    TOKEN_DELIMITER, // same goes for this one...

    TOKEN_ERROR,
    TOKEN_EOF,
    TOKEN_SEMICOLON,
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char *lexeme;
    int line;
    int column;
} Token;

// Token API
extern Token *create_token(TokenType type, const char *lexeme, int line, int column);
extern void destroy_token(Token *token);

#endif // TOKEN_H
