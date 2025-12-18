#ifndef EDITOR_H
#define EDITOR_H

#include <windows.h>

// Create editor control
HWND Editor_Create(HWND hParent, int x, int y, int width, int height);

// Editor operations
void Editor_SetText(HWND hEditor, const char* text);
char* Editor_GetText(HWND hEditor);
void Editor_Clear(HWND hEditor);
void Editor_Cut(HWND hEditor);
void Editor_Copy(HWND hEditor);
void Editor_Paste(HWND hEditor);
void Editor_Undo(HWND hEditor);

#endif