#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H

#include "../../include/parser.h"
#include <ctype.h>

void handle_variable_declaration(Lexer *lexer, int is_const);

// Function to trim leading and trailing whitespaces
// static char *trimWhitespace(char *str) {
//     while (isspace(*str)) str++;
//     if (*str == '\0') return str;
//     char *end = str + strlen(str) - 1;
//     while (end > str && isspace(*end)) end--;
//     *(end + 1) = '\0';
//     return str;
// }

char *trimWhitespace(char *str);

#endif // VARIABLEDECLARATION_H