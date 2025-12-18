#include <ide.h>
#include <resources.h>

void UI_CreateMenu(HWND hWnd) {
    HMENU hMenu = CreateMenu();
    HMENU hFile = CreatePopupMenu();
    HMENU hEdit = CreatePopupMenu();
    HMENU hCompile = CreatePopupMenu();
    
    AppendMenuW(hFile, MF_STRING, IDM_FILE_NEW, L"&New\tCtrl+N");
    AppendMenuW(hFile, MF_STRING, IDM_FILE_OPEN, L"&Open...\tCtrl+O");
    AppendMenuW(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFile, MF_STRING, IDM_FILE_SAVE, L"&Save\tCtrl+S");
    AppendMenuW(hFile, MF_STRING, IDM_FILE_SAVEAS, L"&Save As...");
    AppendMenuW(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFile, MF_STRING, IDM_FILE_EXIT, L"E&xit");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFile, L"&File");
    
    AppendMenuW(hEdit, MF_STRING, IDM_EDIT_UNDO, L"&Undo\tCtrl+Z");
    AppendMenuW(hEdit, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hEdit, MF_STRING, IDM_EDIT_CUT, L"&Cut\tCtrl+X");
    AppendMenuW(hEdit, MF_STRING, IDM_EDIT_COPY, L"&Copy\tCtrl+C");
    AppendMenuW(hEdit, MF_STRING, IDM_EDIT_PASTE, L"&Paste\tCtrl+V");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hEdit, L"&Edit");
    
    AppendMenuW(hCompile, MF_STRING, IDM_COMPILE_RUN, L"&Run\tF5");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hCompile, L"&Compile");
    
    SetMenu(hWnd, hMenu);
}

void UI_CreateStatusBar(HWND hWnd) {
    g_hStatusBar = CreateWindowExW(0, STATUSCLASSNAME, L"Ready",
                                   WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                                   0, 0, 0, 0, hWnd, (HMENU)IDC_STATUSBAR, g_hInst, NULL);
}

void UI_CreateLayout(HWND hWnd) {
    g_hEditor = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN |
                                ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL,
                                0, 0, 800, 500, hWnd, (HMENU)IDC_EDITOR, g_hInst, NULL);
    
    g_hOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY |
                                ES_AUTOVSCROLL | WS_VSCROLL,
                                0, 500, 800, 150, hWnd, (HMENU)IDC_OUTPUT, g_hInst, NULL);
    
    // Set monospace font
    HFONT hFont = CreateFontW(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                              FIXED_PITCH | FF_MODERN, L"Consolas");
    SendMessage(g_hEditor, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hOutput, WM_SETFONT, (WPARAM)hFont, TRUE);
}