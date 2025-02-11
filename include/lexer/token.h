#ifndef TOKEN_H
#define TOKEN_H

#include "token_types.h"
#include <string>

class Token {
public:
    Token(TokenType type, const std::string &lexeme, int line, int column);
    ~Token() = default;

    TokenType getType() const;
    const std::string &getLexeme() const;
    int getLine() const;
    int getColumn() const;

private:
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

#endif // TOKEN_H
