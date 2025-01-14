#include "printstatement.h"
#include "../../include/parser.h" // For evaluate_expression and find_variable
#include <stdio.h>

// Define global variables
Variable variables[100];
int variable_count = 0;

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
        printf("=> %s\n", arg->lexeme); // Print the string literal
    } else if (arg->type == TOKEN_IDENTIFIER) {
        Variable *var = find_variable(arg->lexeme);
        if (var) {
            if (strcmp(var->type, "int") == 0) {
                printf("=> %d\n", var->value.int_value); // Print integer value
            } else if (strcmp(var->type, "str") == 0) {
                printf("=> %s\n", var->value.str_value); // Print string value
            } else if (strcmp(var->type, "bool") == 0) {
                printf("=> %s\n", var->value.bool_value ? "true" : "false"); // Print boolean value
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