#ifndef COMPILER_H
#define COMPILER_H 

#include <ide.h>

BOOL Compiler_Init(void);
void Compiler_Cleanup(void);
BOOL Compiler_CompileAndRun(const char* code);
const char* Compiler_GetLastError(void);

#endif // COMPILER_H