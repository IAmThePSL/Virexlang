#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H

#include "../../include/parser/parser.h"
#include <ctype.h>

void handle_variable_declaration(Lexer *lexer, int is_const);

// Function to trim leading and trailing whitespaces
char *trimWhitespace(char *str);

#endif // VARIABLEDECLARATION_H
