#include "../include/parser.h"
#include <ctype.h>
#include <stdbool.h>
#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statements/ifstatement.h"
#include "statements/whilestatement.h"
#include "statements/printstatement.h"
#include "statements/variabledeclaration.h"

// Global variable store
Variable variables[100];
// int variable_count = 0;  

// Find a variable by name
Variable *find_variable(const char *name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

// Add a variable to the global store
void add_variable(const char *name, const char *type, void *value, int is_const) {
    if (find_variable(name)) {
        fprintf(stderr, "Error: Variable '%s' is already defined\n", name);
        return;
    }

    Variable *var = &variables[variable_count++];
    var->name = strdup(name);
    var->type = strdup(type);
    var->is_const = is_const;

    if (strcmp(type, "int") == 0) {
        var->value.int_value = *(int *)value;
    } else if (strcmp(type, "str") == 0) {
        var->value.str_value = strdup((char *)value);
    } else if (strcmp(type, "bool") == 0) {
        var->value.bool_value = *(int *)value;
    }
}

// Cleanup all allocated memory
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

// Helper function to check if a string represents a number
bool is_number(const char *str) {
    if (!str || *str == '\0') return false;
    int i = 0;
    if (str[i] == '-') i++; // Skip leading minus sign if present
    bool has_dot = false;
    for (; str[i]; i++) {
        if (str[i] == '.') {
            if (has_dot) return false; // Multiple dots are not allowed
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// Function to evaluate simple expressions
int evaluate_expression(Token *condition) {
    // Extract the expression (lexeme from the token)
    char *expr = condition->lexeme;
    expr = trimWhitespace(expr);

    // Handle Parentheses (recursively evaluate inside them)
    if (expr[0] == '(') {
        char *closing_paren = strchr(expr + 1, ')');
        if (!closing_paren) {
            fprintf(stderr, "Error: Mismatched parentheses\n");
            return 0;
        }
        *closing_paren = '\0';  // Null-terminate the inside expression
        return evaluate_expression(create_token(TOKEN_BOOL, expr + 1, 0, 0));
    }

    // Handle basic comparisons (e.g., x == 10)
    if (strstr(expr, "==")) {
        char *left = strtok(expr, "==");
        char *right = strtok(NULL, "==");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        if (is_number(left) && is_number(right)) {
            double left_value = atof(left);
            double right_value = atof(right);
            return left_value == right_value;
        } else {
            return strcmp(left, right) == 0;  // String comparison
        }
    }

    if (strstr(expr, "!=")) {
        char *left = strtok(expr, "!=");
        char *right = strtok(NULL, "!=");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        if (is_number(left) && is_number(right)) {
            double left_value = atof(left);
            double right_value = atof(right);
            return left_value != right_value;
        } else {
            return strcmp(left, right) != 0;  // String comparison
        }
    }

    if (strstr(expr, ">")) {
        char *left = strtok(expr, ">");
        char *right = strtok(NULL, ">");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        return left_value > right_value;
    }

    if (strstr(expr, "<")) {
        char *left = strtok(expr, "<");
        char *right = strtok(NULL, "<");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        return left_value < right_value;
    }

    if (strstr(expr, ">=")) {
        char *left = strtok(expr, ">=");
        char *right = strtok(NULL, ">=");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        return left_value >= right_value;
    }

    if (strstr(expr, "<=")) {
        char *left = strtok(expr, "<=");
        char *right = strtok(NULL, "<=");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        return left_value <= right_value;
    }

    // Handle arithmetic operations (addition, subtraction, multiplication, division)
    if (strchr(expr, '+')) {
        char *left = strtok(expr, "+");
        char *right = strtok(NULL, "+");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        return left_value + right_value;
    }

    if (strchr(expr, '-')) {
        char *left = strtok(expr, "-");
        char *right = strtok(NULL, "-");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        return left_value - right_value;
    }

    if (strchr(expr, '*')) {
        char *left = strtok(expr, "*");
        char *right = strtok(NULL, "*");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        return left_value * right_value;
    }

    if (strchr(expr, '/')) {
        char *left = strtok(expr, "/");
        char *right = strtok(NULL, "/");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        if (right_value == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 0;
        }
        return left_value / right_value;
    }

    // Handle logical AND (&&)
    if (strstr(expr, "&&")) {
        char *left = strtok(expr, "&&");
        char *right = strtok(NULL, "&&");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        return evaluate_expression(create_token(TOKEN_BOOL, left, 0, 0)) &&
               evaluate_expression(create_token(TOKEN_BOOL, right, 0, 0));
    }

    // Handle logical OR (||)
    if (strstr(expr, "||")) {
        char *left = strtok(expr, "||");
        char *right = strtok(NULL, "||");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        return evaluate_expression(create_token(TOKEN_BOOL, left, 0, 0)) ||
               evaluate_expression(create_token(TOKEN_BOOL, right, 0, 0));
    }

    // Handle boolean values (e.g., "true" or "false")
    if (strcmp(expr, "true") == 0) return 1;
    if (strcmp(expr, "false") == 0) return 0;

    // Default case: Return the evaluated number (this can be extended for other data types)
    return is_number(expr) ? atof(expr) : 0;
}

// Handle variable declarations
void handle_variable_declaration(Lexer *lexer, int is_const) {
    Token *type_token = lex_next_token(lexer);
    if (!type_token || type_token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected a type for variable declaration\n");
        return;
    }

    Token *name_token = lex_next_token(lexer);
    if (!name_token || name_token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected a variable name\n");
        return;
    }

    Token *assign_token = lex_next_token(lexer);
    if (!assign_token || assign_token->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Error: Expected '=' after variable name\n");
        return;
    }

    Token *value_token = lex_next_token(lexer);
    if (!value_token) {
        fprintf(stderr, "Error: Expected a value for variable assignment\n");
        return;
    }

    if (strcmp(type_token->lexeme, "int") == 0) {
        int value = atoi(value_token->lexeme);
        add_variable(name_token->lexeme, "int", &value, is_const);
    } else if (strcmp(type_token->lexeme, "str") == 0) {
        add_variable(name_token->lexeme, "str", value_token->lexeme, is_const);
    } else if (strcmp(type_token->lexeme, "bool") == 0) {
        int value = (strcmp(value_token->lexeme, "true") == 0) ? 1 : 0;
        add_variable(name_token->lexeme, "bool", &value, is_const);
    } else if (strcmp(type_token->lexeme, "let") == 0) {
        // Type inference (basic implementation)
        if (strchr(value_token->lexeme, '"')) {
            add_variable(name_token->lexeme, "str", value_token->lexeme, is_const);
        } else if (strchr(value_token->lexeme, '.')) {
            float value = atof(value_token->lexeme);
            add_variable(name_token->lexeme, "float", &value, is_const);
        } else {
            int value = atoi(value_token->lexeme);
            add_variable(name_token->lexeme, "int", &value, is_const);
        }
    } else {
        fprintf(stderr, "Error: Unknown type '%s'\n", type_token->lexeme);
    }

    // Cleanup tokens
    destroy_token(type_token);
    destroy_token(name_token);
    destroy_token(assign_token);
    destroy_token(value_token);
}

// Main statement handler
void execute_statement(Lexer *lexer) {
    Token *token = lex_next_token(lexer);
    if (!token) {
        fprintf(stderr, "Error: Unexpected end of input\n");
        return;
    }

    switch (token->type) {
        case TOKEN_PRINT:
            handle_print_statement(lexer);
            break;

        case TOKEN_LET:
        case TOKEN_CONST:
            handle_variable_declaration(lexer, token->type == TOKEN_CONST);
            break;

        default:
            fprintf(stderr, "Error: Unknown statement starting with '%s' at line %d, column %d\n",
                    token->lexeme, token->line, token->column);
            break;
    }

    destroy_token(token);
}
