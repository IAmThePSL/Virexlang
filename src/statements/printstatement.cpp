#include "../../include/statements/printstatement.h"
#include "../../include/parser/parser.h" // For evaluate_expression and find_variable
#include "../../include/lexer/lexer.h"
#include <iostream>
#include <cstring>

// Define global variables
extern Variable variables[100];
extern int variable_count;

void PrintStatement::handlePrintStatement(Lexer &lexer) {
    Token *lparen = lex_next_token(&lexer); // '('
    Token *arg = lex_next_token(&lexer); // Argument (string or variable)
    Token *rparen = lex_next_token(&lexer); // ')'

    // Check for incomplete print statement
    if (!lparen || !arg || !rparen) {
        std::cerr << "Error: Incomplete print statement" << std::endl;
        destroy_token(lparen);
        destroy_token(arg);
        destroy_token(rparen);
        return;
    }

    // Ensure '(' is present
    if (lparen->type != TOKEN_LPAREN) {
        std::cerr << "Error: Expected '(' after 'print' at line " << lparen->line 
                  << ", column " << lparen->column << std::endl;
        destroy_token(lparen);
        destroy_token(arg);
        destroy_token(rparen);
        return;
    }

    // Ensure ')' is present
    if (rparen->type != TOKEN_RPAREN) {
        std::cerr << "Error: Expected ')' after print argument at line " << rparen->line
                  << ", column " << rparen->column << std::endl;
        destroy_token(lparen);
        destroy_token(arg);
        destroy_token(rparen);
        return;
    }

    // Handle the print argument
    if (arg->type == TOKEN_STRING_LITERAL) {
        std::cout << "=> " << arg->lexeme << std::endl; // Print the string literal
    } else if (arg->type == TOKEN_IDENTIFIER) {
        Variable *var = find_variable(arg->lexeme);
        if (var) {
            if (strcmp(var->type, "int") == 0) {
                std::cout << "=> " << var->value.int_value << std::endl; // Print integer value
            } else if (strcmp(var->type, "str") == 0) {
                std::cout << "=> " << var->value.str_value << std::endl; // Print string value
            } else if (strcmp(var->type, "bool") == 0) {
                std::cout << "=> " << (var->value.bool_value ? "true" : "false") << std::endl; // Print boolean value
            }
        } else {
            std::cerr << "Error: Undefined variable '" << arg->lexeme 
                      << "' at line " << arg->line << ", column " << arg->column << std::endl;
        }
    } else {
        std::cerr << "Error: Invalid argument to print at line " << arg->line 
                  << ", column " << arg->column << std::endl;
    }

    // Clean up tokens
    destroy_token(lparen);
    destroy_token(arg);
    destroy_token(rparen);
}
