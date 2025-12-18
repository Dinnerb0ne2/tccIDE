#include <editor.h>
#include <ide.h>

static HWND s_hwndEditor = NULL;

BOOL Editor_Init(HWND parent) {
    LoadLibrary("riched20.dll");
    
    s_hwndEditor = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "RichEdit20A",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
        ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NOHIDESEL,
        0, 0, 100, 100,
        parent,
        NULL,
        g_hInstance,
        NULL
    );
    
    if (s_hwndEditor) {
        Editor_SetFont();
        g_ide.hwndEditor = s_hwndEditor;
        return TRUE;
    }
    return FALSE;
}

HWND Editor_GetHandle(void) {
    return s_hwndEditor;
}

void Editor_SetText(const char* text) {
    SetWindowText(s_hwndEditor, text);
}

char* Editor_GetText(void) {
    int length = GetWindowTextLength(s_hwndEditor);
    char* buffer = malloc(length + 1);
    if (buffer) {
        GetWindowText(s_hwndEditor, buffer, length + 1);
    }
    return buffer;
}

void Editor_Clear(void) {
    SetWindowText(s_hwndEditor, "");
}

void Editor_SetModified(BOOL modified) {
    g_ide.isFileModified = modified;
}

BOOL Editor_GetModified(void) {
    return g_ide.isFileModified;
}

void Editor_SetFont(void) {
    CHARFORMATA cf = {0};
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_FACE | CFM_SIZE;
    cf.yHeight = 180; // 9pt
    strcpy(cf.szFaceName, "Consolas");
    SendMessage(s_hwndEditor, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
}