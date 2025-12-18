#include <editor.h>
#include <resources.h>

HWND Editor_Create(HWND hParent, int x, int y, int width, int height) {
    return CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
                           WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN |
                           ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL,
                           x, y, width, height, hParent, (HMENU)IDC_EDITOR,
                           GetModuleHandle(NULL), NULL);
}

void Editor_SetText(HWND hEditor, const char* text) {
    SetWindowTextA(hEditor, text);
}

char* Editor_GetText(HWND hEditor) {
    int len = GetWindowTextLengthA(hEditor);
    char* buf = (char*)malloc(len + 1);
    if (buf) GetWindowTextA(hEditor, buf, len + 1);
    return buf;
}

void Editor_Clear(HWND hEditor) {
    SetWindowTextA(hEditor, "");
}

void Editor_Cut(HWND hEditor) {
    SendMessage(hEditor, WM_CUT, 0, 0);
}

void Editor_Copy(HWND hEditor) {
    SendMessage(hEditor, WM_COPY, 0, 0);
}

void Editor_Paste(HWND hEditor) {
    SendMessage(hEditor, WM_PASTE, 0, 0);
}

void Editor_Undo(HWND hEditor) {
    SendMessage(hEditor, WM_UNDO, 0, 0);
}