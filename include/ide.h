#ifndef IDE_H
#define IDE_H

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <commctrl.h>

// Global window class name
#define IDE_CLASS_NAME L"CIDE_MainWindow"

// Global handles
extern HINSTANCE g_hInst;
extern HWND g_hMainWnd;
extern HWND g_hEditor;
extern HWND g_hOutput;
extern HWND g_hStatusBar;
extern WCHAR g_szFilePath[MAX_PATH];

// Main window procedure
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

#endif