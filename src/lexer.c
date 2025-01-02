#include "../include/token.h"
#include "../include/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEXEME_LENGTH 256

typedef struct Lexer {
  FILE *file;
  int current_char;
  int line;
  int column;
} Lexer;

// create a new lexer
Lexer *create_lexer(FILE *file) {
  Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
  if (!lexer) {
    fprintf(stderr, "Error: Memory allocation failed for lexer\n");
    exit(EXIT_FAILURE);
  }

  lexer->file = file;
  lexer->current_char = fgetc(file); // Read the first character
  lexer->line = 1; // Start at line 1
  lexer->column = 1; // Start at column 1

  return lexer;
}

// destroy the lexer
void destroy_lexer(Lexer *lexer) {
  if (lexer) {
    fclose(lexer->file); // Close the file
    free(lexer); // Free the lexer object
  }
}

// Advance to the next character
int lex_advance(Lexer *lexer) {
  if (lexer->current_char == '\n') {
    lexer->line++; // Increment the line number
    lexer->column = 1; // Reset the column number
  } else {
    lexer->column++; // Increment the column number
  }
  lexer->current_char = fgetc(lexer->file); // Read the next character
  return lexer->current_char;
}

// Skip whitespace characters
void skip_whitespace(Lexer *lexer) {
  while (isspace(lexer->current_char)) {
    lex_advance(lexer);
  }
}

// Skip comments (single-line and multi-line)
void skip_comments(Lexer *lexer) {
    if (lexer->current_char == '/' && fgetc(lexer->file) == '/') {
        // single-line comment
        while (lexer->current_char != '\n' && lexer->current_char != EOF) {
            lex_advance(lexer);
        }
    } else if (lexer->current_char == '/' && fgetc(lexer->file) == '*') {
        // multi-line comment
        lex_advance(lexer); // skip '*'
        while (lexer->current_char != EOF) {
            if (lexer->current_char == '*' && fgetc(lexer->file) == '/') {
                lex_advance(lexer); // skip '/'
                break;
            }
            lex_advance(lexer);
        }
    }
}

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

  // Check if the lexeme is a keyword
  if (strcmp(lexeme, "int") == 0) return create_token(TOKEN_INT, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "str") == 0) return create_token(TOKEN_STR, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "bool") == 0) return create_token(TOKEN_BOOL, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "let") == 0) return create_token(TOKEN_LET, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "const") == 0) return create_token(TOKEN_CONST, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "function") == 0) return create_token(TOKEN_FUNCTION, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "if") == 0) return create_token(TOKEN_IF, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "else") == 0) return create_token(TOKEN_ELSE, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "for") == 0) return create_token(TOKEN_FOR, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "while") == 0) return create_token(TOKEN_WHILE, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "return") == 0) return create_token(TOKEN_RETURN, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "class") == 0) return create_token(TOKEN_CLASS, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "import") == 0) return create_token(TOKEN_IMPORT, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "true") == 0) return create_token(TOKEN_TRUE, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "false") == 0) return create_token(TOKEN_FALSE, lexeme, lexer->line, start_column);
  if (strcmp(lexeme, "null") == 0) return create_token(TOKEN_NULL, lexeme, lexer->line, start_column);

  // Otherwise, it's an identifier
  return create_token(TOKEN_IDENTIFIER, lexeme, lexer->line, start_column);
}

// Lex a number
Token *lex_number(Lexer *lexer) {
    char lexeme[MAX_LEXEME_LENGTH] = {0};
    int length = 0;
    int start_column = lexer->column;

    while (isdigit(lexer->current_char)) {
        if (length < MAX_LEXEME_LENGTH - 1) {
            lexeme[length++] = lexer->current_char;
        } else {
            fprintf(stderr, "Error: Numeric literal exceeds max length at line %d, column %d\n", lexer->line, start_column);
            exit(EXIT_FAILURE);
        }
        lex_advance(lexer);
    }
    lexeme[length] = '\0';
    return create_token(TOKEN_INTEGER_LITERAL, lexeme, lexer->line, start_column);
}

// Lex string literals
Token *lex_string(Lexer *lexer) {
    char lexeme[MAX_LEXEME_LENGTH] = {0};
    int length = 0;
    int start_column = lexer->column;

    lex_advance(lexer); // Skip opening quote
    while (lexer->current_char != '"' && lexer->current_char != EOF) {
        if (lexer->current_char == '\\') { // Handle escape sequences
            lex_advance(lexer);
            switch (lexer->current_char) {
                case 'n': lexeme[length++] = '\n'; break;
                case 't': lexeme[length++] = '\t'; break;
                case '"': lexeme[length++] = '"'; break;
                case '\\': lexeme[length++] = '\\'; break;
                default:
                    fprintf(stderr, "Error: Unknown escape sequence '\\%c' at line %d, column %d\n",
                            lexer->current_char, lexer->line, lexer->column);
                    exit(EXIT_FAILURE);
            }
        } else if (length < MAX_LEXEME_LENGTH - 1) {
            lexeme[length++] = lexer->current_char;
        } else {
            fprintf(stderr, "Error: String literal exceeds max length at line %d, column %d\n",
                    lexer->line, start_column);
            exit(EXIT_FAILURE);
        }
        lex_advance(lexer);
    }

    if (lexer->current_char == '"') {
        lex_advance(lexer); // Skip closing quote
        lexeme[length] = '\0';
        return create_token(TOKEN_STRING_LITERAL, lexeme, lexer->line, start_column);
    } else {
        fprintf(stderr, "Error: Unterminated string literal at line %d, column %d\n",
                lexer->line, start_column);
        exit(EXIT_FAILURE);
    }
}

// Main tokenization function
Token *lex_next_token(Lexer *lexer) {
    while (lexer->current_char != EOF) {
        if (isspace(lexer->current_char)) {
            skip_whitespace(lexer);
            continue;
        }

        if (lexer->current_char == '/' && (fgetc(lexer->file) == '/' || fgetc(lexer->file) == '*')) {
            skip_comments(lexer);
            continue;
        }

        if (isalpha(lexer->current_char)) {
            return lex_identifier_or_keyword(lexer);
        }

        if (isdigit(lexer->current_char)) {
            return lex_number(lexer);
        }

        if (lexer->current_char == '"') {
            return lex_string(lexer);
        }

        // Handle single-character tokens (e.g., operators, delimiters)
        switch (lexer->current_char) {
            case '+': lex_advance(lexer); return create_token(TOKEN_PLUS, "+", lexer->line, lexer->column - 1);
            case '-': lex_advance(lexer); return create_token(TOKEN_MINUS, "-", lexer->line, lexer->column - 1);
            // Add other single-character tokens here...
            default:
                fprintf(stderr, "Error: Unknown character '%c' at line %d, column %d\n",
                        lexer->current_char, lexer->line, lexer->column);
                exit(EXIT_FAILURE);
        }
    }

    return create_token(TOKEN_EOF, "EOF", lexer->line, lexer->column);
}