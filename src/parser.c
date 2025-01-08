#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple symbol table for variables
typedef struct {
    char *name;
    char *type; // Variable type (e.g., "int", "str", "bool")
    int is_const; // (for now) 1 for const, 0 for let
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
void add_variable(const char *name, const char *type, void *value, int is_const) {
    variables[variable_count].name = strdup(name);
    variables[variable_count].type = strdup(type);
    variables[variable_count].is_const = is_const;

    if (strcmp(type, "int") == 0) {
        variables[variable_count].value.int_value = *(int *)value;
    } else if (strcmp(type, "str") == 0) {
        variables[variable_count].value.str_value = strdup((char *)value);
    } else if (strcmp(type, "bool") == 0) {
        variables[variable_count].value.bool_value = *(int *)value;
    }

    variable_count++;
}

// Free memory allocated for variables
void cleanup_variables() {
    for (int i = 0; i < variable_count; i++) {
        free(variables[i].name);
        free(variables[i].type);
        if (strcmp(variables[i].type, "str") == 0) {
            free(variables[i].value.str_value);
        }
    }
    variable_count = 0;
}

// Evaluate an expression
int evaluate_expression(Token *token) {
    if (token->type == TOKEN_INTEGER_LITERAL) {
        return atoi(token->lexeme);
    } else if (token->type == TOKEN_IDENTIFIER) {
        Variable *var = find_variable(token->lexeme);
        if (var && strcmp(var->type, "int") == 0) {
            return var->value.int_value;
        }
    }
    fprintf(stderr, "Error: Invalid expression at line %d, column %d\n", token->line, token->column);
    return 0;
}

// Parse and execute a statement
// Helper function to handle variable declarations
void handle_variable_declaration(Lexer *lexer, int is_const) {
    Token *name = lex_next_token(lexer); // Variable name
    Token *assign = lex_next_token(lexer); // '='
    Token *value = lex_next_token(lexer); // Value

    // Check for incomplete declaration
    if (!name || !assign || !value) {
        fprintf(stderr, "Error: Incomplete variable declaration\n");
        destroy_token(name);
        destroy_token(assign);
        destroy_token(value);
        return;
    }

    // Ensure '=' is present
    if (assign->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Error: Expected '=' after variable name at line %d, column %d\n",
                assign->line, assign->column);
        destroy_token(name);
        destroy_token(assign);
        destroy_token(value);
        return;
    }

    // Determine the type and value of the variable
    const char *type = NULL;
    void *value_ptr = NULL;

    if (value->type == TOKEN_INTEGER_LITERAL) {
        type = "int";
        int int_value = evaluate_expression(value);
        value_ptr = &int_value;
    } else if (value->type == TOKEN_STRING_LITERAL) {
        type = "str";
        value_ptr = value->lexeme;
    } else if (value->type == TOKEN_TRUE || value->type == TOKEN_FALSE) {
        type = "bool";
        int bool_value = (value->type == TOKEN_TRUE);
        value_ptr = &bool_value;
    } else {
        fprintf(stderr, "Error: Invalid value for variable '%s' at line %d, column %d\n",
                name->lexeme, value->line, value->column);
        destroy_token(name);
        destroy_token(assign);
        destroy_token(value);
        return;
    }

    // Add the variable to the symbol table
    add_variable(name->lexeme, type, value_ptr, is_const);
    printf("=> %s (%s) = ", name->lexeme, type);

    // Print the value based on its type
    if (strcmp(type, "int") == 0) {
        printf("%d\n", *(int *)value_ptr);
    } else if (strcmp(type, "str") == 0) {
        printf("\"%s\"\n", (char *)value_ptr);
    } else if (strcmp(type, "bool") == 0) {
        printf("%s\n", *(int *)value_ptr ? "true" : "false");
    }

    // Clean up tokens
    destroy_token(name);
    destroy_token(assign);
    destroy_token(value);
}

// Helper function to handle print statements
void handle_print_statement(Lexer *lexer) {
    Token *lparen = lex_next_token(lexer); // '('
    Token *arg = lex_next_token(lexer); // Argument (string or variable)
    Token *rparen = lex_next_token(lexer); // ')'

    // Check for incomplete print statement
    if (!lparen || !arg || !rparen) {
        fprintf(stderr, "Error: Incomplete print statement\n");
        destroy_token(lparen);
        destroy_token(arg);
        destroy_token(rparen);
        return;
    }

    // Ensure '(' is present
    if (lparen->type != TOKEN_LPAREN) {
        fprintf(stderr, "Error: Expected '(' after 'print' at line %d, column %d\n",
                lparen->line, lparen->column);
        destroy_token(lparen);
        destroy_token(arg);
        destroy_token(rparen);
        return;
    }

    // Ensure ')' is present
    if (rparen->type != TOKEN_RPAREN) {
        fprintf(stderr, "Error: Expected ')' after print argument at line %d, column %d\n",
                rparen->line, rparen->column);
        destroy_token(lparen);
        destroy_token(arg);
        destroy_token(rparen);
        return;
    }

    // Handle the print argument
    if (arg->type == TOKEN_STRING_LITERAL) {
        printf("=> %s\n", arg->lexeme);
    } else if (arg->type == TOKEN_IDENTIFIER) {
        Variable *var = find_variable(arg->lexeme);
        if (var) {
            if (strcmp(var->type, "int") == 0) {
                printf("=> %d\n", var->value.int_value);
            } else if (strcmp(var->type, "str") == 0) {
                printf("=> %s\n", var->value.str_value);
            } else if (strcmp(var->type, "bool") == 0) {
                printf("=> %s\n", var->value.bool_value ? "true" : "false");
            }
        } else {
            fprintf(stderr, "Error: Undefined variable '%s' at line %d, column %d\n",
                    arg->lexeme, arg->line, arg->column);
        }
    } else {
        fprintf(stderr, "Error: Invalid argument to print at line %d, column %d\n",
                arg->line, arg->column);
    }

    // Clean up tokens
    destroy_token(lparen);
    destroy_token(arg);
    destroy_token(rparen);
}

// Main function to execute statements
void execute_statement(Lexer *lexer) {
    Token *token = lex_next_token(lexer);

    if (token->type == TOKEN_LET || token->type == TOKEN_INT || token->type == TOKEN_STR || token->type == TOKEN_BOOL) {
        // Variable declaration: let x = 5; or int x = 5;
        const char *type = NULL;

        if (token->type == TOKEN_LET) {
            type = "var"; // Default for `let`
        } else if (token->type == TOKEN_INT) {
            type = "int";
        } else if (token->type == TOKEN_STR) {
            type = "str";
        } else if (token->type == TOKEN_BOOL) {
            type = "bool";
        }

        Token *name = lex_next_token(lexer); // Variable name
        if (name->type != TOKEN_IDENTIFIER) {
            fprintf(stderr, "Error: Expected variable name after '%s' at line %d, column %d\n", token->lexeme, name->line, name->column);
            destroy_token(name);
            destroy_token(token);
            return;
        }

        Token *assign = lex_next_token(lexer); // '='
        if (assign->type != TOKEN_ASSIGN) {
            fprintf(stderr, "Error: Expected '=' after variable name at line %d, column %d\n", assign->line, assign->column);
            destroy_token(name);
            destroy_token(assign);
            destroy_token(token);
            return;
        }

        Token *value = lex_next_token(lexer); // Value
        if (type == NULL) {
            fprintf(stderr, "Error: Unexpected type in declaration\n");
            destroy_token(name);
            destroy_token(assign);
            destroy_token(value);
            destroy_token(token);
            return;
        }

        // Handle assignment based on type
        if (strcmp(type, "int") == 0 && value->type == TOKEN_INTEGER_LITERAL) {
            int int_value = atoi(value->lexeme);
            add_variable(name->lexeme, type, &int_value, 1);
            printf("=> %s (int) = %d\n", name->lexeme, int_value);
        } else if (strcmp(type, "str") == 0 && value->type == TOKEN_STRING_LITERAL) {
            add_variable(name->lexeme, type, value->lexeme, 1);
            printf("=> %s (str) = \"%s\"\n", name->lexeme, value->lexeme);
        } else if (strcmp(type, "bool") == 0 && (value->type == TOKEN_TRUE || value->type == TOKEN_FALSE)) {
            int bool_value = (value->type == TOKEN_TRUE);
            add_variable(name->lexeme, type, &bool_value, 1);
            printf("=> %s (bool) = %s\n", name->lexeme, bool_value ? "true" : "false");
        } else {
            fprintf(stderr, "Error: Type mismatch or invalid value for variable '%s'\n", name->lexeme);
        }

        destroy_token(name);
        destroy_token(assign);
        destroy_token(value);
    } else if (token->type == TOKEN_PRINT) {
        // Handle print logic (as defined earlier)
    } else {
        fprintf(stderr, "Error: Unknown statement starting with '%s' at line %d, column %d\n", token->lexeme, token->line, token->column);
    }

    destroy_token(token);
}
