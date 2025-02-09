#include "../../include/parser/parser.h"

#define MAX_UNGET_TOKENS 256 // I actually don't know why I chose this number. Forgive me...

int evaluate_expression(Token *condition);
int unget_buffer_index = 0;

Token unget_buffer[MAX_UNGET_TOKENS];

void lex_unget_token(Lexer *lexer, Token *token){
    if (unget_buffer_index < MAX_UNGET_TOKENS) {
        unget_buffer[unget_buffer_index++] = *token;
    } else {
        fprintf(stderr, "Error: unget buffer overflow\n");
    }
}

// Handle if statements
void handle_if_statement(Lexer *lexer) {
    Token *condition_token = lex_next_token(lexer);
    if (!condition_token || condition_token->type != TOKEN_LPAREN) {
        fprintf(stderr, "Error: Expected '(' after 'if'\n");
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

    // Evaluate the condition
    int condition_result = evaluate_expression(condition);
    if (condition_result) {
        // Execute the if block
        Token *lbrace = lex_next_token(lexer);
        if (!lbrace || lbrace->type != TOKEN_LBRACE) {
            fprintf(stderr, "Error: Expected '{' after 'if' condition\n");
            return;
        }

        // Execute statements inside the if block
        while (1) {
            Token *token = lex_next_token(lexer);
            if (!token || token->type == TOKEN_RBRACE) {
                break; // End of if block
            }
            execute_statement(lexer); // Execute each statement in the block
            destroy_token(token);
        }
    } else {
        // Skip the if block
        Token *token = lex_next_token(lexer);
        if (!token || token->type != TOKEN_LBRACE) {
            fprintf(stderr, "Error: Expected '{' after 'if' condition\n");
            return;
        }

        // Skip all tokens until the matching '}'
        int brace_count = 1;
        while (brace_count > 0) {
            token = lex_next_token(lexer);
            if (!token) {
                fprintf(stderr, "Error: Unexpected end of input\n");
                return;
            }
            if (token->type == TOKEN_LBRACE) {
                brace_count++;
            } else if (token->type == TOKEN_RBRACE) {
                brace_count--;
            }
            destroy_token(token);
        }
    }

    // Check for an else block
    Token *else_token = lex_next_token(lexer);
    if (else_token && else_token->type == TOKEN_ELSE) {
        Token *lbrace = lex_next_token(lexer);
        if (!lbrace || lbrace->type != TOKEN_LBRACE) {
            fprintf(stderr, "Error: Expected '{' after 'else'\n");
            return;
        }

        // Execute statements inside the else block
        while (1) {
            Token *token = lex_next_token(lexer);
            if (!token || token->type == TOKEN_RBRACE) {
                break; // End of else block
            }
            execute_statement(lexer); // Execute each statement in the block
            destroy_token(token);
        }
    } else {
        // If there's no else block, put the token back
        lex_unget_token(lexer, else_token);
        destroy_token(else_token); // Ensure else_token is destroyed
    }

    // Cleanup tokens
    destroy_token(condition_token);
    destroy_token(condition);
    destroy_token(rparen);
}
