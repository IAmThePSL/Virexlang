#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

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

    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_ASTERISK,     // *
    TOKEN_SLASH,        // /
    TOKEN_PERCENT,      // %
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_LBRACKET,     // [
    TOKEN_RBRACKET,     // ]
    TOKEN_ASSIGN,       // =

    TOKEN_IDENTIFIER,       // Variable names, function names, etc.
    TOKEN_INTEGER_LITERAL,  // Integer constants
    TOKEN_STRING_LITERAL,   // String constants

    TOKEN_OPERATOR,         // General operators (optional)
    TOKEN_DELIMITER,        // General delimiters (optional)

    TOKEN_ERROR,            // Invalid token
    TOKEN_EOF,              // End of file
    TOKEN_SEMICOLON,        // ;
} TokenType;

#endif // TOKEN_TYPES_H