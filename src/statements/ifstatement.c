#include "../../include/parser/parser.h"
#include <stack>
#include <iostream>

constexpr int MAX_UNGET_TOKENS = 256; // I don't actually know why i chose this number... Forgive me.

int evaluate_expression(std::vector<Token>& condition);
int unget_buffer_index = 0;
std::vector<Token> unget_buffer;

void lex_unget_token(Lexer& lexer, Token token) {
    if (unget_buffer.size() < MAX_UNGET_TOKENS) {
        unget_buffer.push_back(token);
    } else {
        std::cerr << "Error: unget buffer overflow" << std::endl;
    }
}

void handle_if_statement(Lexer& lexer) {
    Token condition_token = lex_next_token(lexer);
    if (condition_token.type != TOKEN_LPAREN) {
        std::cerr << "Error: Expected '(' after 'if'" << std::endl;
        return;
    }

    std::vector<Token> condition_tokens;
    Token token;
    while ((token = lex_next_token(lexer)).type != TOKEN_RPAREN) {
        if (token.type == TOKEN_EOF) {
            std::cerr << "Error: Unexpected end of input inside condition" << std::endl;
            return;
        }
        condition_tokens.push_back(token);
    }

    int condition_result = evaluate_expression(condition_tokens);
    Token lbrace = lex_next_token(lexer);
    if (lbrace.type != TOKEN_LBRACE) {
        std::cerr << "Error: Expected '{' after 'if' condition" << std::endl;
        return;
    }

    if (condition_result) {
        while (true) {
            Token token = lex_next_token(lexer);
            if (token.type == TOKEN_RBRACE) break;
            if (token.type == TOKEN_EOF) {
                std::cerr << "Error: Unexpected end of input inside if-block" << std::endl;
                return;
            }
            execute_statement(lexer);
        }
    } else {
        std::stack<int> brace_stack;
        brace_stack.push(1);
        while (!brace_stack.empty()) {
            token = lex_next_token(lexer);
            if (token.type == TOKEN_EOF) {
                std::cerr << "Error: Unexpected end of input" << std::endl;
                return;
            }
            if (token.type == TOKEN_LBRACE) brace_stack.push(1);
            else if (token.type == TOKEN_RBRACE) brace_stack.pop();
        }
    }

    Token next_token = lex_next_token(lexer);
    if (next_token.type == TOKEN_ELSE) {
        Token else_lbrace = lex_next_token(lexer);
        if (else_lbrace.type != TOKEN_LBRACE) {
            std::cerr << "Error: Expected '{' after 'else'" << std::endl;
            return;
        }
        while (true) {
            token = lex_next_token(lexer);
            if (token.type == TOKEN_RBRACE) break;
            if (token.type == TOKEN_EOF) {
                std::cerr << "Error: Unexpected end of input inside else-block" << std::endl;
                return;
            }
            execute_statement(lexer);
        }
    } else {
        lex_unget_token(lexer, next_token);
    }
}
