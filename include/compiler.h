#ifndef COMPILER_H
#define COMPILER_H

#include <stddef.h>

// Compile and execute code
// Returns 0 on success, -1 on compilation error
int Compiler_RunCode(const char* code, char* output, size_t output_size);

// Get last compilation error
const char* Compiler_GetLastError(void);

#endif