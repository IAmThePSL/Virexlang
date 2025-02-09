#include "../../include/statements/whilestatement.h"
#include "../../include/parser/parser.h" // For evaluate_expression
#include <stdio.h>

// Declare the evaluate_expression function
int evaluate_expression(Token *condition);

// Handle while statements
void handle_while_statement(Lexer *lexer) {
    Token *condition_token = lex_next_token(lexer);
    if (!condition_token || condition_token->type != TOKEN_LPAREN) {
        fprintf(stderr, "Error: Expected '(' after 'while'\n");
        return;
    }

    // Parse the condition inside the parentheses
    Token *condition = lex_next_token(lexer);
    if (!condition) {
        fprintf(stderr, "Error: Expected condition after '('\n");
        return;
    }

    Token *rparen = lex_next_token(lexer);
    if (!rparen || rparen->type != TOKEN_RPAREN) {
        fprintf(stderr, "Error: Expected ')' after condition\n");
        return;
    }

    // Loop: Execute as long as the condition is true
    while (evaluate_expression(condition)) {
        // Execute the while block
        Token *lbrace = lex_next_token(lexer);
        if (!lbrace || lbrace->type != TOKEN_LBRACE) {
            fprintf(stderr, "Error: Expected '{' after 'while' condition\n");
            return;
        }

        // Execute statements inside the while block
        while (1) {
            Token *token = lex_next_token(lexer);
            if (!token || token->type == TOKEN_RBRACE) {
                break; // End of while block
            }
            execute_statement(lexer); // Execute each statement in the block
            destroy_token(token);
        }

        // Re-evaluate condition (for while loops)
        condition = lex_next_token(lexer);
        if (!condition) {
            fprintf(stderr, "Error: Expected condition after while block\n");
            return;
        }
    }
}
