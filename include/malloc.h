#include <stdio.h>
#include <stdlib.h>

// Validate all memory allocations and handle failures gracefully.
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}