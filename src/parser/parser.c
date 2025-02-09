#include "../../include/parser/parser.h"
#include "../../include/lexer/lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../../include/statements/ifstatement.h"
#include "../../include/statements/whilestatement.h"
#include "../../include/statements/printstatement.h"
#include "../../include/statements/variabledeclaration.h"

// Global variable store
Variable variables[100];
int variable_count = 0;  

void execute_statement(Lexer *lexer) {
    // Get the current token
    Token current_token = *lex_next_token(lexer);

    // Simple handling print statements
    if (current_token.type == TOKEN_PRINT) {
        handle_print_statement(lexer);
    }
    else if (current_token.type == TOKEN_IF) {
        handle_if_statement(lexer);
    }
    else if (current_token.type == TOKEN_WHILE) {
        handle_while_statement(lexer);
    }
    else if (current_token.type == TOKEN_LET || current_token.type == TOKEN_BOOL || current_token.type == TOKEN_INT || current_token.type == TOKEN_STR) {
        handle_variable_declaration(lexer, 0);
    }
    else if (current_token.type == TOKEN_CONST) {
        handle_variable_declaration(lexer, 1);
    }
    else{
        // Handle invalid statement or error
        fprintf(stderr, "Error: Unknown statement type.\n");
    }

    destroy_token(&current_token); // Ensure token is destroyed
}

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
        int result = evaluate_expression(create_token(TOKEN_BOOL, expr + 1, 0, 0));
        destroy_token(condition); // Ensure token is destroyed
        return result;
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
            destroy_token(condition); // Ensure token is destroyed
            return left_value == right_value;
        } else {
            destroy_token(condition); // Ensure token is destroyed
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
            destroy_token(condition); // Ensure token is destroyed
            return left_value != right_value;
        } else {
            destroy_token(condition); // Ensure token is destroyed
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
        destroy_token(condition); // Ensure token is destroyed
        return left_value > right_value;
    }

    if (strstr(expr, "<")) {
        char *left = strtok(expr, "<");
        char *right = strtok(NULL, "<");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        destroy_token(condition); // Ensure token is destroyed
        return left_value < right_value;
    }

    if (strstr(expr, ">=")) {
        char *left = strtok(expr, ">=");
        char *right = strtok(NULL, ">=");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        destroy_token(condition); // Ensure token is destroyed
        return left_value >= right_value;
    }

    if (strstr(expr, "<=")) {
        char *left = strtok(expr, "<=");
        char *right = strtok(NULL, "<=");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        destroy_token(condition); // Ensure token is destroyed
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
        destroy_token(condition); // Ensure token is destroyed
        return left_value + right_value;
    }

    if (strchr(expr, '-')) {
        char *left = strtok(expr, "-");
        char *right = strtok(NULL, "-");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        destroy_token(condition); // Ensure token is destroyed
        return left_value - right_value;
    }

    if (strchr(expr, '*')) {
        char *left = strtok(expr, "*");
        char *right = strtok(NULL, "*");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        double left_value = atof(left);
        double right_value = atof(right);
        destroy_token(condition); // Ensure token is destroyed
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
            destroy_token(condition); // Ensure token is destroyed
            return 0;
        }
        destroy_token(condition); // Ensure token is destroyed
        return left_value / right_value;
    }

    // Handle logical AND (&&)
    if (strstr(expr, "&&")) {
        char *left = strtok(expr, "&&");
        char *right = strtok(NULL, "&&");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        int result = evaluate_expression(create_token(TOKEN_BOOL, left, 0, 0)) &&
                     evaluate_expression(create_token(TOKEN_BOOL, right, 0, 0));
        destroy_token(condition); // Ensure token is destroyed
        return result;
    }

    // Handle logical OR (||)
    if (strstr(expr, "||")) {
        char *left = strtok(expr, "||");
        char *right = strtok(NULL, "||");
        left = trimWhitespace(left);
        right = trimWhitespace(right);
        int result = evaluate_expression(create_token(TOKEN_BOOL, left, 0, 0)) ||
                     evaluate_expression(create_token(TOKEN_BOOL, right, 0, 0));
        destroy_token(condition); // Ensure token is destroyed
        return result;
    }

    // Handle boolean values (e.g., "true" or "false")
    if (strcmp(expr, "true") == 0) {
        destroy_token(condition); // Ensure token is destroyed
        return 1;
    }
    if (strcmp(expr, "false") == 0) {
        destroy_token(condition); // Ensure token is destroyed
        return 0;
    }

    // Default case: Return the evaluated number (this can be extended for other data types)
    int result = is_number(expr) ? atof(expr) : 0;
    destroy_token(condition); // Ensure token is destroyed
    return result;
}
