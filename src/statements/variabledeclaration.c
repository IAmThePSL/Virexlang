#include "../../include/statements/variabledeclaration.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/parser/parser.h"

// Trim leading and trailing whitespaces
char *trimWhitespace(char *str) {
    while (isspace(*str)) str++;
    if (*str == '\0') return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

// Parse variable type from string
const char* parseVariableType(const char *typeStr) {
    if (strcmp(typeStr, "int") == 0) return "int";
    if (strcmp(typeStr, "str") == 0) return "str";
    if (strcmp(typeStr, "bool") == 0) return "bool";
    return "unknown"; // Default type if unrecognized
}

// Parse a variable declaration
bool parseVariableDeclaration(const char *declaration, Variable *var) {
    char type[32], name[32], value[256];
    if (sscanf(declaration, "%31s %31s = %[^\n]", type, name, value) != 3) {
        fprintf(stderr, "Invalid declaration: %s\n", declaration);
        return false;
    }

    // Set the variable type and name
    const char *parsedType = parseVariableType(type);
    var->type = strdup(parsedType);
    var->name = strdup(name);

    // Handle variable values based on type
    if (strcmp(parsedType, "int") == 0) {
        var->value.int_value = atoi(value);
    } else if (strcmp(parsedType, "str") == 0) {
        if (value[0] == '"' && value[strlen(value) - 1] == '"') {
            value[strlen(value) - 1] = '\0'; // Remove trailing quote
            var->value.str_value = strdup(&value[1]); // Skip leading quote
        } else {
            fprintf(stderr, "Invalid string value: %s\n", value);
            free(var->type);
            free(var->name);
            return false;
        }
    } else if (strcmp(parsedType, "bool") == 0) {
        var->value.bool_value = (strcmp(value, "true") == 0);
    } else {
        fprintf(stderr, "Unknown variable type: %s\n", type);
        free(var->type);
        free(var->name);
        return false;
    }

    return true;
}

// Print a variable's details
void printVariable(const Variable *var) {
    printf("Variable Name: %s\n", var->name);
    printf("Variable Type: %s\n", var->type);
    if (strcmp(var->type, "int") == 0) {
        printf("Value: %d\n", var->value.int_value);
    } else if (strcmp(var->type, "str") == 0) {
        printf("Value: \"%s\"\n", var->value.str_value);
    } else if (strcmp(var->type, "bool") == 0) {
        printf("Value: %s\n", var->value.bool_value ? "true" : "false");
    } else {
        printf("Unknown value\n");
    }
}

// Free allocated memory for a variable
void freeVariable(Variable *var) {
    // Store the type comparison result before freeing
    bool is_string = (strcmp(var->type, "str") == 0);
    
    free(var->name);
    free(var->type);
    if (is_string) {
        free(var->value.str_value);
    }
}
