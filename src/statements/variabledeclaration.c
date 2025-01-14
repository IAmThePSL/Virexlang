#include "variabledeclaration.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/parser.h" // Ensure we are using the correct Variable definition

char *trimWhitespace(char *str) {
    while (isspace(*str)) str++;
    if (*str == '\0') return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

// Parse variable type from string
char* parseVariableType(const char *typeStr) {
    // Return a string representing the type (e.g., "int", "str", etc.)
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

    var->type = strdup(parseVariableType(type)); // Set type as string
    var->name = strdup(name); // Set name

    switch (strcmp(var->type, "int")) {
        case 0:
            var->value.int_value = atoi(value); // Parse int value
            break;
        case 1: // If type is "str"
            if (value[0] == '"' && value[strlen(value) - 1] == '"') {
                value[strlen(value) - 1] = '\0'; // Remove trailing quote
                var->value.str_value = strdup(&value[1]); // Skip leading quote
            } else {
                fprintf(stderr, "Invalid string value: %s\n", value);
                return false;
            }
            break;
        case 2: // If type is "bool"
            var->value.bool_value = (strcmp(value, "true") == 0);
            break;
        default:
            fprintf(stderr, "Unknown variable type: %s\n", type);
            return false;
    }

    return true;
}

// Print a variable
void printVariable(const Variable *var) {
    printf("Variable Name: %s\n", var->name);
    printf("Variable Type: %s\n", var->type);
    switch (strcmp(var->type, "int")) {
        case 0: printf("Value: %d\n", var->value.int_value); break;
        case 1: printf("Value: \"%s\"\n", var->value.str_value); break;
        case 2: printf("Value: %s\n", var->value.bool_value ? "true" : "false"); break;
        default: printf("Unknown value\n");
    }
}

// Free allocated memory for a variable
void freeVariable(Variable *var) {
    free(var->name);
    free(var->type);
    if (strcmp(var->type, "str") == 0) {
        free(var->value.str_value);
    }
}