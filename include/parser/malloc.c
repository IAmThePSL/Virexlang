#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>

// Validate all memory allocations and handle failures gracefully.
void *safe_malloc(size_t size);

#endif
