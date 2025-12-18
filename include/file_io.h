#ifndef FILE_IO_H
#define FILE_IO_H

#include <ide.h>

BOOL FileIO_Open(HWND hwnd, char* buffer, size_t size);
BOOL FileIO_Save(HWND hwnd, const char* buffer, const char* filename);
BOOL FileIO_SaveAs(HWND hwnd, const char* buffer, char* filename);

#endif // FILE_IO_H