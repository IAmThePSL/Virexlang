#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple symbol table for variables
typedef struct {
    char *name;
    char *type; // Variable type (e.g., "int", "str", "bool")
    union {
        int int_value;
        char *str_value;
        int bool_value;
    } value;
} Variable;

Variable variables[100];
int variable_count = 0;

// Find a variable by name
Variable *find_variable(const char *name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

// Add a new variable to the symbol table
void add_variable(const char *name, const char *type, void *value) {
    variables[variable_count].name = strdup(name);
    variables[variable_count].type = strdup(type);

    if (strcmp(type, "int") == 0) {
        variables[variable_count].value.int_value = *(int *)value;
    } else if (strcmp(type, "str") == 0) {
        variables[variable_count].value.str_value = strdup((char *)value);
    } else if (strcmp(type, "bool") == 0) {
        variables[variable_count].value.bool_value = *(int *)value;
    }

    variable_count++;
}

// Parse and execute a statement
void execute_statement(Lexer *lexer) {
    Token *token = lex_next_token(lexer);

    if (token->type == TOKEN_LET || token->type == TOKEN_CONST || 
        token->type == TOKEN_INT || token->type == TOKEN_STR || token->type == TOKEN_BOOL) {
        // Variable declaration
        const char *type = (token->type == TOKEN_INT) ? "int" :
                           (token->type == TOKEN_STR) ? "str" : 
                           (token->type == TOKEN_BOOL) ? "bool" : "var";

        Token *name = lex_next_token(lexer); // Variable name
        Token *assign = lex_next_token(lexer); // '='
        Token *value = lex_next_token(lexer); // Value

        if (assign->type != TOKEN_ASSIGN) {
            fprintf(stderr, "Error: Expected '=' after variable name\n");
            destroy_token(name);
            destroy_token(assign);
            destroy_token(value);
            destroy_token(token);
            return;
        }

        if (strcmp(type, "int") == 0 && value->type == TOKEN_INTEGER_LITERAL) {
            int int_value = atoi(value->lexeme);
            add_variable(name->lexeme, type, &int_value);
            printf("=> %s (int) = %d\n", name->lexeme, int_value);
        } else if (strcmp(type, "str") == 0 && value->type == TOKEN_STRING_LITERAL) {
            add_variable(name->lexeme, type, value->lexeme);
            printf("=> %s (str) = \"%s\"\n", name->lexeme, value->lexeme);
        } else if (strcmp(type, "bool") == 0 && 
                   (strcmp(value->lexeme, "true") == 0 || strcmp(value->lexeme, "false") == 0)) {
            int bool_value = strcmp(value->lexeme, "true") == 0;
            add_variable(name->lexeme, type, &bool_value);
            printf("=> %s (bool) = %s\n", name->lexeme, bool_value ? "true" : "false");
        } else {
            fprintf(stderr, "Error: Type mismatch or invalid value for variable '%s'\n", name->lexeme);
        }

        destroy_token(name);
        destroy_token(assign);
        destroy_token(value);
    } else if (token->type == TOKEN_PRINT) {
        // Print statement: print("Hello, Virex!");
        Token *lparen = lex_next_token(lexer); // '('
        Token *string = lex_next_token(lexer); // String or variable
        Token *rparen = lex_next_token(lexer); // ')'

        if (string->type == TOKEN_STRING_LITERAL) {
            printf("=> %s\n", string->lexeme);
        } else if (string->type == TOKEN_IDENTIFIER) {
            Variable *var = find_variable(string->lexeme);
            if (var) {
                if (strcmp(var->type, "int") == 0) {
                    printf("=> %d\n", var->value.int_value);
                } else if (strcmp(var->type, "str") == 0) {
                    printf("=> %s\n", var->value.str_value);
                } else if (strcmp(var->type, "bool") == 0) {
                    printf("=> %s\n", var->value.bool_value ? "true" : "false");
                }
            } else {
                fprintf(stderr, "Error: Undefined variable '%s'\n", string->lexeme);
            }
        } else {
            fprintf(stderr, "Error: Invalid argument to print\n");
        }

        destroy_token(lparen);
        destroy_token(string);
        destroy_token(rparen);
    } else {
        fprintf(stderr, "Error: Unknown statement starting with '%s'\n", token->lexeme);
    }

    destroy_token(token);
}