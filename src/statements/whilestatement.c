#include "whilestatement.h"
#include "../../include/parser.h" // For evaluate_expression
#include <stdio.h>

// Declare the evaluate_expression function
int evaluate_expression(Token *condition);

void handle_while_statement(Lexer *lexer) {
    Token *lparen = lex_next_token(lexer); // '('
    Token *condition = lex_next_token(lexer); // Condition
    Token *rparen = lex_next_token(lexer); // ')'
    Token *lbrace = lex_next_token(lexer); // '{'

    // Validate syntax
    if (!lparen || !condition || !rparen || !lbrace ||
        lparen->type != TOKEN_LPAREN ||
        rparen->type != TOKEN_RPAREN ||
        lbrace->type != TOKEN_LBRACE) {
        fprintf(stderr, "Error: Invalid while loop syntax\n");
        destroy_token(lparen);
        destroy_token(condition);
        destroy_token(rparen);
        destroy_token(lbrace);
        return;
    }

    // Evaluate the condition
    while (evaluate_expression(condition)) {
        // Execute the loop body
        execute_statement(lexer);

        // Reset the lexer to re-evaluate the condition
        rewind(lexer->file);
        lexer->current_char = fgetc(lexer->file);
    }

    // Clean up tokens
    destroy_token(lparen);
    destroy_token(condition);
    destroy_token(rparen);
    destroy_token(lbrace);
}