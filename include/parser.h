#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"
#include <stdbool.h>

// Function to parse and execute a statement
void execute_statement(Lexer *lexer);

typedef struct {
    char *name;      // Variable name
    char *type;      // Variable type (e.g., "int", "str", "bool")
    int is_const;    // 1 for const, 0 for let
    union {
        int int_value;
        char *str_value;
        bool bool_value;
    } value;         // Value stored in the variable
} Variable;

// Declare external variables
extern Variable variables[100];
extern int variable_count;

// Functions for variable management
Variable *find_variable(const char *name);
void add_variable(const char *name, const char *type, void *value, int is_const);
void cleanup_variables();
int evaluate_expression(Token *condition);

#endif // PARSER_H
