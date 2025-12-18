#ifndef FILEIO_H
#define FILEIO_H

#include <windows.h>

// File operations
BOOL FileIO_New(void);
BOOL FileIO_Open(HWND hWnd, WCHAR* filePath);
BOOL FileIO_Save(HWND hWnd, HWND hEditor, WCHAR* filePath);
BOOL FileIO_SaveAs(HWND hWnd, HWND hEditor, WCHAR* filePath);

#endif