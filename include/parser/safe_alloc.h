#ifndef SAFE_ALLOC_H
#define SAFE_ALLOC_H

#include <cstdlib>
#include <new>  // For std::bad_alloc

// Function to allocate memory safely, throwing an exception on failure
inline void* safe_malloc(std::size_t size) {
    void* ptr = std::malloc(size);  // Use std::malloc from C++ standard library
    if (!ptr) {
        throw std::bad_alloc();  // Standard C++ way to handle allocation failure
    }
    return ptr;
}

#endif // SAFE_ALLOC_H
