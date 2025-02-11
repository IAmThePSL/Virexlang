#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <vector>
#include <string>
#include "token.h"

class Lexer {
public:
    explicit Lexer(const std::string &filename);
    ~Lexer();

    Token lex_next_token();
    void ungetToken(const Token &token);

    
private:
    std::ifstream inputStream;
    std::ifstream file;
    int line;
    int column;
    char currentChar;
    std::vector<Token> tokenBuffer;

    void advance();
    void skipWhitespace();
};

#endif // LEXER_H
