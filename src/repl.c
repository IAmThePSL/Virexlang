// Normally used in interpretted langs (e.g., Python) but we use it now too cus we can :)
#include <stdio.h>
#include <string.h>
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/token_types.h"
#include "../include/parser.h"
#include "statements/allstatements.h"

// REPL loop
void repl() {
    printf("Welcome to the Virex REPL!\n");
    printf("Type '.exit' to quit.\n");

    char input[256];
    while (1) {
        printf(">> "); // Prompt
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n"); // Handle Ctrl+D
            break;
        }

        // Remove newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Exit condition
        if (strcmp(input, ".exit") == 0) {
            break;
        }

        // Create a lexer for the input
        FILE *input_stream = fmemopen(input, strlen(input), "r");
        if (!input_stream) {
            perror("Error creating input stream");
            continue;
        }

        Lexer *lexer = create_lexer(input_stream);

        // Parse and execute the statement
        execute_statement(lexer);

        // Clean up
        destroy_lexer(lexer);
        fclose(input_stream);
    }

    printf("Goodbye!\n");
}

int main() {
    repl();
    return 0;
}
