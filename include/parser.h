#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"

// function to parse and execute a statement
void execute_statement(Lexer *lexer);

#endif // PARSER_H