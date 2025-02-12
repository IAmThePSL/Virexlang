#include <iostream>
#include <fstream>
#include <memory>
#include "../include/lexer/lexer.h"
#include "../include/lexer/token.h"
#include "../include/lexer/token_types.h"
#include "../include/parser/safe_alloc.h"

// Function to print token details
void print_token(const Token &token) {
    std::cout << "Token(Type: " << token.getType() 
              << ", Lexeme: '" << token.getLexeme() 
              << "', Line: " << token.getLine() 
              << ", Column: " << token.getColumn() << ")\n";
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return EXIT_FAILURE;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::perror("Error opening file");
        return EXIT_FAILURE;
    }

    Lexer lexer(argv[1]);  // Create the lexer
    Token token = lexer.lex_next_token();  // Get the first token

    std::cout << "Lexing file: " << argv[1] << "\n";
    while (token.getType() != TOKEN_EOF) {
        print_token(token);  // Print the token details
        token = lexer.lex_next_token();  // Get the next token
    }

    // Print the EOF token and exit
    print_token(token);  // Print EOF token
    file.close();  // Close the file
    return EXIT_SUCCESS;
}
