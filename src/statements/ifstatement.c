#include "../../include/parser.h"

// Declare the evaluate_expression function
int evaluate_expression(Token *condition);

void handle_if_statement(Lexer *lexer) {
    Token *lparen = lex_next_token(lexer); // '('
    Token *condition = lex_next_token(lexer); // Condition
    Token *rparen = lex_next_token(lexer); // ')'
    Token *lbrace = lex_next_token(lexer); // '{'

    // Validate syntax
    if (!lparen || !condition || !rparen || !lbrace ||
        lparen->type != TOKEN_LPAREN ||
        rparen->type != TOKEN_RPAREN ||
        lbrace->type != TOKEN_LBRACE) {
        fprintf(stderr, "Error: Invalid if statement syntax\n");
        destroy_token(lparen);
        destroy_token(condition);
        destroy_token(rparen);
        destroy_token(lbrace);
        return;
    }

    // Evaluate the condition
    if (evaluate_expression(condition)) {
        // Execute the if body
        while (1) {
            Token *token = lex_next_token(lexer);
            if (token->type == TOKEN_RBRACE) {
                destroy_token(token);
                break; // End of if body
            }
            execute_statement(lexer);
            destroy_token(token);
        }
    } else {
        // Skip the if body
        while (1) {
            Token *token = lex_next_token(lexer);
            if (token->type == TOKEN_RBRACE) {
                destroy_token(token);
                break; // End of if body
            }
            destroy_token(token);
        }
    }

    // Clean up tokens
    destroy_token(lparen);
    destroy_token(condition);
    destroy_token(rparen);
    destroy_token(lbrace);
}