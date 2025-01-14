#include "../include/token.h"
#include "../include/token_types.h"
#include "../include/lexer.h"
#include "../include/malloc.h" // includes the safe_malloc function
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEXEME_LENGTH 256
#define KEYWORD_COUNT 16


// This is defined in the lexer.h header file
// So, no need to define it here again.
// typedef struct Lexer {
//     FILE *file;
//     int current_char;
//     int line;
//     int column;
// } Lexer;

// Hash table for keywords
typedef struct Keyword {
    const char *word;
    TokenType type;
} Keyword;

const Keyword keywords[KEYWORD_COUNT] = {
    {"int", TOKEN_INT},       {"str", TOKEN_STR},     {"bool", TOKEN_BOOL},
    {"let", TOKEN_LET},       {"const", TOKEN_CONST}, {"function", TOKEN_FUNCTION},
    {"if", TOKEN_IF},         {"else", TOKEN_ELSE},   {"for", TOKEN_FOR},
    {"while", TOKEN_WHILE},   {"return", TOKEN_RETURN},
    {"class", TOKEN_CLASS},   {"import", TOKEN_IMPORT},
    {"true", TOKEN_TRUE},     {"false", TOKEN_FALSE}, {"null", TOKEN_NULL}};

// Create a new lexer
Lexer *create_lexer(FILE *file) {
    Lexer *lexer = (Lexer *)safe_malloc(sizeof(Lexer));
    lexer->file = file;
    lexer->current_char = fgetc(file);
    lexer->line = 1;
    lexer->column = 1;
    return lexer;
}

// Destroy the lexer
void destroy_lexer(Lexer *lexer) {
    if (lexer) {
        fclose(lexer->file);
        free(lexer);
    }
}

// Advance to the next character
int lex_advance(Lexer *lexer) {
    if (lexer->current_char == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->current_char = fgetc(lexer->file);
    return lexer->current_char;
}

// Skip whitespace
void skip_whitespace(Lexer *lexer) {
    while (isspace(lexer->current_char)) {
        lex_advance(lexer);
    }
}

// Skip comments
void skip_comments(Lexer *lexer) {
    if (lexer->current_char == '#') {
        // Single-line comment
        while (lexer->current_char != '\n' && lexer->current_char != EOF) {
            lex_advance(lexer);
        }
        if (lexer->current_char == '\n') {
            lex_advance(lexer); // Skip the newline
        }
    } else if (lexer->current_char == '/') {
        lex_advance(lexer); // Advance past '/'
        if (lexer->current_char == '*') {
            // Multi-line comment
            lex_advance(lexer); // Advance past '*'
            while (lexer->current_char != EOF) {
                if (lexer->current_char == '*' && fgetc(lexer->file) == '/') {
                    lex_advance(lexer); // Advance past '/'
                    break;
                }
                lex_advance(lexer);
            }
        }
    }
}

// Keyword lookup
TokenType lookup_keyword(const char *lexeme) {
    for (int i = 0; i < KEYWORD_COUNT; i++) {
        if (strcmp(lexeme, keywords[i].word) == 0) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENTIFIER;
}

// Lex identifiers or keywords
Token *lex_identifier_or_keyword(Lexer *lexer) {
    char lexeme[MAX_LEXEME_LENGTH] = {0};
    int length = 0;
    int start_column = lexer->column;

    while (isalnum(lexer->current_char) || lexer->current_char == '_') {
        if (length < MAX_LEXEME_LENGTH - 1) {
            lexeme[length++] = lexer->current_char;
        }
        lex_advance(lexer);
    }
    lexeme[length] = '\0';

    TokenType type = lookup_keyword(lexeme);
    return create_token(type, lexeme, lexer->line, start_column);
}

// Lex numbers
Token *lex_number(Lexer *lexer) {
    char lexeme[MAX_LEXEME_LENGTH] = {0};
    int length = 0;
    int start_column = lexer->column;

    while (isdigit(lexer->current_char)) {
        if (length < MAX_LEXEME_LENGTH - 1) {
            lexeme[length++] = lexer->current_char;
        }
        lex_advance(lexer);
    }
    lexeme[length] = '\0';

    return create_token(TOKEN_INTEGER_LITERAL, lexeme, lexer->line, start_column);
}

// Lex strings
Token *lex_string(Lexer *lexer) {
    char lexeme[MAX_LEXEME_LENGTH] = {0};
    int length = 0;
    int start_column = lexer->column;

    lex_advance(lexer);  // Skip opening quote
    while (lexer->current_char != '"' && lexer->current_char != EOF) {
        if (lexer->current_char == '\\') {  // Handle escape sequences
            lex_advance(lexer);
            switch (lexer->current_char) {
                case 'n': lexeme[length++] = '\n'; break;
                case 't': lexeme[length++] = '\t'; break;
                case 'r': lexeme[length++] = '\r'; break;
                case 'b': lexeme[length++] = '\b'; break;
                case 'f': lexeme[length++] = '\f'; break;
                case '0': lexeme[length++] = '\0'; break;
                case '"': lexeme[length++] = '"'; break;
                case '\\': lexeme[length++] = '\\'; break;
                default:
                    fprintf(stderr, "Error: Unknown escape sequence '\\%c' at line %d, column %d\n",
                            lexer->current_char, lexer->line, lexer->column);
                    lexeme[length++] = lexer->current_char; // Include the unknown escape sequence as-is
                    break;
            }
        } else if (length < MAX_LEXEME_LENGTH - 1) {
            lexeme[length++] = lexer->current_char;
        } else {
            fprintf(stderr, "Error: String literal exceeds max length at line %d, column %d\n",
                    lexer->line, start_column);
            break;
        }
        lex_advance(lexer);
    }

    if (lexer->current_char == '"') {
        lex_advance(lexer);  // Skip closing quote
        lexeme[length] = '\0';
        return create_token(TOKEN_STRING_LITERAL, lexeme, lexer->line, start_column);
    } else {
        fprintf(stderr, "Error: Unterminated string literal at line %d, column %d\n",
                lexer->line, start_column);
        return create_token(TOKEN_ERROR, "Unterminated string", lexer->line, start_column);
    }
}

// Lex next token
Token *lex_next_token(Lexer *lexer) {
    skip_whitespace(lexer);

    if (lexer->current_char == EOF) {
        return create_token(TOKEN_EOF, "EOF", lexer->line, lexer->column);
    }

    // Handle comments
    if (lexer->current_char == '#' || lexer->current_char == '/') {
        skip_comments(lexer);
        return lex_next_token(lexer); // Recursively get the next token after skipping comments
    }

    if (isalpha(lexer->current_char)) {
        return lex_identifier_or_keyword(lexer);
    }

    if (isdigit(lexer->current_char)) {
        return lex_number(lexer);
    }

    // Handle string literals
    if (lexer->current_char == '"') {
        return lex_string(lexer);
    }

    // Single-character tokens
    switch (lexer->current_char) {
        case '+': lex_advance(lexer); return create_token(TOKEN_PLUS, "+", lexer->line, lexer->column - 1);
        case '-': lex_advance(lexer); return create_token(TOKEN_MINUS, "-", lexer->line, lexer->column - 1);
        case '*': lex_advance(lexer); return create_token(TOKEN_ASTERISK, "*", lexer->line, lexer->column - 1);
        case '/': lex_advance(lexer); return create_token(TOKEN_SLASH, "/", lexer->line, lexer->column - 1);
        case '%': lex_advance(lexer); return create_token(TOKEN_PERCENT, "%", lexer->line, lexer->column - 1);
        case '=': lex_advance(lexer); return create_token(TOKEN_ASSIGN, "=", lexer->line, lexer->column - 1);
        case ';': lex_advance(lexer); return create_token(TOKEN_SEMICOLON, ";", lexer->line, lexer->column - 1);
        case '(': lex_advance(lexer); return create_token(TOKEN_LPAREN, "(", lexer->line, lexer->column - 1);
        case ')': lex_advance(lexer); return create_token(TOKEN_RPAREN, ")", lexer->line, lexer->column - 1);
        default:
            fprintf(stderr, "Error: Unknown character '%c' at line %d, column %d\n",
                    lexer->current_char, lexer->line, lexer->column);
            lex_advance(lexer);
            return create_token(TOKEN_ERROR, "Unknown character", lexer->line, lexer->column);
    }
}
