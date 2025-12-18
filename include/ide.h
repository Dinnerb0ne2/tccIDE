#ifndef IDE_H
#define IDE_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RichEdit structures and constants (from richedit.h)
typedef struct _charformat {
    UINT cbSize;
    DWORD dwMask;
    DWORD dwEffects;
    LONG yHeight;
    LONG yOffset;
    COLORREF crTextColor;
    BYTE bCharSet;
    BYTE bPitchAndFamily;
    CHAR szFaceName[32];
} CHARFORMATA;

#define CFM_FACE 0x20000000
#define CFM_SIZE 0x80000000
#define SCF_ALL 0x0004
#define EM_SETCHARFORMAT (WM_USER + 57)

// File dialog structures and constants (from commdlg.h)
typedef struct tagOFNA {
    DWORD lStructSize;
    HWND hwndOwner;
    HINSTANCE hInstance;
    LPCSTR lpstrFilter;
    LPSTR lpstrCustomFilter;
    DWORD nMaxCustFilter;
    DWORD nFilterIndex;
    LPSTR lpstrFile;
    DWORD nMaxFile;
    LPSTR lpstrFileTitle;
    DWORD nMaxFileTitle;
    LPCSTR lpstrInitialDir;
    LPCSTR lpstrTitle;
    DWORD Flags;
    WORD nFileOffset;
    WORD nFileExtension;
    LPCSTR lpstrDefExt;
    LPARAM lCustData;
    LRESULT (CALLBACK *lpfnHook)(HWND, UINT, WPARAM, LPARAM);
    LPCSTR lpTemplateName;
} OPENFILENAMEA, *LPOPENFILENAMEA;

typedef OPENFILENAMEA OPENFILENAME;
#define OFN_FILEMUSTEXIST 0x00001000
#define OFN_PATHMUSTEXIST 0x00000800
#define OFN_OVERWRITEPROMPT 0x00000002

// Function declarations for file dialogs
BOOL WINAPI GetOpenFileNameA(LPOPENFILENAMEA);
BOOL WINAPI GetSaveFileNameA(LPOPENFILENAMEA);

// IDE definitions
#define IDE_CLASS "C_IDE_CLASS"
#define IDE_TITLE "C IDE - TCC"
#define IDE_WIDTH 1024
#define IDE_HEIGHT 768

#define IDM_FILE_NEW 1001
#define IDM_FILE_OPEN 1002
#define IDM_FILE_SAVE 1003
#define IDM_FILE_EXIT 1004
#define IDM_COMPILE_RUN 2001
#define IDM_BUILD_COMPILE 2002

typedef struct {
    HWND hwndMain;
    HWND hwndEditor;
    HWND hwndOutput;
    char currentFile[MAX_PATH];
    BOOL isFileModified;
} IDE_STATE;

extern IDE_STATE g_ide;
extern HINSTANCE g_hInstance;

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

#endif // IDE_H