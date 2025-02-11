#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H

#include "../../include/parser/parser.h"
#include <cctype>
#include <string>

class VariableDeclaration {
public:
    static void handleVariableDeclaration(Lexer &lexer, bool isConst);

    // Function to trim leading and trailing whitespaces
    static std::string trimWhitespace(const std::string &str);
};

#endif // VARIABLEDECLARATION_H
