#include "../../include/lexer/token.h"
#include "../../include/parser/safe_alloc.h"

// initialize a Token object
Token::Token(TokenType type, const std::string& lexeme, int line, int column)
    : type(type), lexeme(lexeme), line(line), column(column) {
    // No need to manually allocate memory for lexeme as std::string handles it
}

// Token creation function
Token* create_token(TokenType type, const std::string& lexeme, int line, int column) {
    return new Token(type, lexeme, line, column);  // use the constructor to create a new Token
}

// Token destruction function
void destroy_token(Token* token) {
    if (token) {
        delete token;
    }
}
