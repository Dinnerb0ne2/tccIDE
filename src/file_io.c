#include <file_io.h>

BOOL FileIO_Open(HWND hwnd, char* buffer, size_t size) {
    OPENFILENAME ofn = {0};
    char filename[MAX_PATH] = {0};
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "C Files (*.c)\0*.c\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = "c";
    
    if (!GetOpenFileName(&ofn)) return FALSE;
    
    HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;
    
    DWORD bytesRead;
    BOOL result = ReadFile(hFile, buffer, size - 1, &bytesRead, NULL);
    if (result) {
        buffer[bytesRead] = '\0';
        strcpy(g_ide.currentFile, filename);
    }
    
    CloseHandle(hFile);
    return result;
}

BOOL FileIO_Save(HWND hwnd, const char* buffer, const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;
    
    DWORD bytesWritten;
    BOOL result = WriteFile(hFile, buffer, strlen(buffer), &bytesWritten, NULL);
    CloseHandle(hFile);
    return result;
}

BOOL FileIO_SaveAs(HWND hwnd, const char* buffer, char* filename) {
    OPENFILENAME ofn = {0};
    char fileBuffer[MAX_PATH] = {0};
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "C Files (*.c)\0*.c\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = "c";
    
    if (!GetSaveFileName(&ofn)) return FALSE;
    
    strcpy(filename, fileBuffer);
    return FileIO_Save(hwnd, buffer, filename);
}