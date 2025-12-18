#ifndef EDITOR_H
#define EDITOR_H

#include <ide.h>

BOOL Editor_Init(HWND parent);
HWND Editor_GetHandle(void);
void Editor_SetText(const char* text);
char* Editor_GetText(void);
void Editor_Clear(void);
void Editor_SetModified(BOOL modified);
BOOL Editor_GetModified(void);
void Editor_SetFont(void);

#endif // EDITOR_H