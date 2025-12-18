#include <output.h>

static HWND s_hwndOutput = NULL;

BOOL Output_Init(HWND parent) {
    s_hwndOutput = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE |
        ES_AUTOVSCROLL | ES_READONLY,
        0, 0, 100, 100,
        parent,
        NULL,
        g_hInstance,
        NULL
    );
    
    if (s_hwndOutput) {
        g_ide.hwndOutput = s_hwndOutput;
        Output_SetFont();
        return TRUE;
    }
    return FALSE;
}

HWND Output_GetHandle(void) {
    return s_hwndOutput;
}

void Output_Clear(void) {
    SetWindowText(s_hwndOutput, "");
}

void Output_Append(const char* text) {
    int len = GetWindowTextLength(s_hwndOutput);
    SendMessage(s_hwndOutput, EM_SETSEL, len, len);
    SendMessage(s_hwndOutput, EM_REPLACESEL, FALSE, (LPARAM)text);
}

void Output_SetFont(void) {
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             FIXED_PITCH | FF_MODERN, "Consolas");
    SendMessage(s_hwndOutput, WM_SETFONT, (WPARAM)hFont, TRUE);
}