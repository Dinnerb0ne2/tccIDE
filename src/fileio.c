#include <fileio.h>
#include <editor.h>
#include <ide.h>

BOOL FileIO_New(void) {
    g_szFilePath[0] = L'\0';
    Editor_Clear(g_hEditor);
    SetWindowTextW(g_hMainWnd, L"C IDE - New File");
    SetWindowTextW(g_hStatusBar, L"New file");
    return TRUE;
}

BOOL FileIO_Open(HWND hWnd, WCHAR* filePath) {
    OPENFILENAMEW ofn = {sizeof(ofn), hWnd, NULL, L"C Files\0*.c\0All Files\0*.*\0",
                         NULL, 0, 0, filePath, MAX_PATH, NULL, 0,
                         NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST};
    
    if (!GetOpenFileNameW(&ofn)) return FALSE;
    
    HANDLE hFile = CreateFileW(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBoxW(hWnd, L"Open failed", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    
    DWORD size = GetFileSize(hFile, NULL);
    char* buf = (char*)malloc(size + 1);
    DWORD bytesRead;
    
    ReadFile(hFile, buf, size, &bytesRead, NULL);
    buf[size] = '\0';
    CloseHandle(hFile);
    
    Editor_SetText(g_hEditor, buf);
    free(buf);
    
    SetWindowTextW(g_hStatusBar, L"File opened");
    return TRUE;
}

BOOL FileIO_Save(HWND hWnd, HWND hEditor, WCHAR* filePath) {
    if (!wcslen(filePath)) return FileIO_SaveAs(hWnd, hEditor, filePath);
    
    char* content = Editor_GetText(hEditor);
    HANDLE hFile = CreateFileW(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBoxW(hWnd, L"Save failed", L"Error", MB_OK | MB_ICONERROR);
        free(content);
        return FALSE;
    }
    
    DWORD bytesWritten;
    WriteFile(hFile, content, strlen(content), &bytesWritten, NULL);
    CloseHandle(hFile);
    free(content);
    
    SetWindowTextW(g_hStatusBar, L"File saved");
    return TRUE;
}

BOOL FileIO_SaveAs(HWND hWnd, HWND hEditor, WCHAR* filePath) {
    OPENFILENAMEW ofn = {sizeof(ofn), hWnd, NULL, L"C Files\0*.c\0All Files\0*.*\0",
                         NULL, 0, 0, filePath, MAX_PATH, NULL, 0,
                         NULL, OFN_OVERWRITEPROMPT};
    
    if (!GetSaveFileNameW(&ofn)) return FALSE;
    
    if (!wcsrchr(filePath, L'.')) wcscat(filePath, L".c");
    return FileIO_Save(hWnd, hEditor, filePath);
}