#ifndef OUTPUT_H
#define OUTPUT_H

#include <ide.h>

BOOL Output_Init(HWND parent);
HWND Output_GetHandle(void);
void Output_Clear(void);
void Output_Append(const char* text);
void Output_SetFont(void);

#endif // OUTPUT_H