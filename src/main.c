#include <ide.h>
#include <editor.h>
#include <compiler.h>
#include <file_io.h>
#include <output.h>

IDE_STATE g_ide = {0};
HINSTANCE g_hInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    g_hInstance = hInstance;
    
    LoadLibrary("riched20.dll"); // Load RichEdit library
    
    if (!Compiler_Init()) {
        MessageBox(NULL, "Failed to initialize TCC", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = IDE_CLASS;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window registration failed", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    g_ide.hwndMain = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        IDE_CLASS,
        IDE_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        IDE_WIDTH, IDE_HEIGHT,
        NULL, NULL,
        hInstance,
        NULL
    );
    
    if (!g_ide.hwndMain) {
        MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    ShowWindow(g_ide.hwndMain, nCmdShow);
    UpdateWindow(g_ide.hwndMain);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    Compiler_Cleanup();
    return (int)msg.wParam;
}

void CreateMenus(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    HMENU hBuildMenu = CreatePopupMenu();
    
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hFileMenu, "&File");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hBuildMenu, "&Build");
    
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, "&New\tCtrl+N");
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN, "&Open\tCtrl+O");
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, "&Save\tCtrl+S");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, "E&xit");
    
    AppendMenu(hBuildMenu, MF_STRING, IDM_BUILD_COMPILE, "&Compile");
    AppendMenu(hBuildMenu, MF_STRING, IDM_COMPILE_RUN, "&Run\tF5");
    
    SetMenu(hwnd, hMenu);
}

void CreateLayout(HWND hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    
    int editorHeight = (rc.bottom - rc.top) * 7 / 10;
    
    if (!Editor_Init(hwnd)) {
        MessageBox(hwnd, "Failed to create editor", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    
    if (!Output_Init(hwnd)) {
        MessageBox(hwnd, "Failed to create output window", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    
    MoveWindow(g_ide.hwndEditor, 0, 0, rc.right, editorHeight, TRUE);
    MoveWindow(g_ide.hwndOutput, 0, editorHeight, rc.right, rc.bottom - editorHeight, TRUE);
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            CreateMenus(hwnd);
            CreateLayout(hwnd);
            return 0;
        
        case WM_SIZE:
            if (g_ide.hwndEditor && g_ide.hwndOutput) {
                int height = HIWORD(lParam);
                int editorHeight = height * 7 / 10;
                MoveWindow(g_ide.hwndEditor, 0, 0, LOWORD(lParam), editorHeight, TRUE);
                MoveWindow(g_ide.hwndOutput, 0, editorHeight, LOWORD(lParam), height - editorHeight, TRUE);
            }
            return 0;
        
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDM_FILE_NEW:
                    Editor_Clear();
                    strcpy(g_ide.currentFile, "");
                    g_ide.isFileModified = FALSE;
                    return 0;
                
                case IDM_FILE_OPEN: {
                    char buffer[65536] = {0};
                    if (FileIO_Open(hwnd, buffer, sizeof(buffer))) {
                        Editor_SetText(buffer);
                        g_ide.isFileModified = FALSE;
                    }
                    return 0;
                }
                
                case IDM_FILE_SAVE:
                    if (strlen(g_ide.currentFile) == 0) {
                        FileIO_SaveAs(hwnd, Editor_GetText(), g_ide.currentFile);
                    } else {
                        FileIO_Save(hwnd, Editor_GetText(), g_ide.currentFile);
                    }
                    g_ide.isFileModified = FALSE;
                    return 0;
                
                case IDM_COMPILE_RUN: {
                    char* code = Editor_GetText();
                    Output_Clear();
                    if (!Compiler_CompileAndRun(code)) {
                        Output_Append(Compiler_GetLastError());
                    }
                    free(code);
                    return 0;
                }
                
                case IDM_FILE_EXIT:
                    PostQuitMessage(0);
                    return 0;
            }
            break;
        
        case WM_CLOSE:
            if (g_ide.isFileModified) {
                int result = MessageBox(hwnd, "Save changes before closing?", "Confirm", MB_YESNOCANCEL | MB_ICONQUESTION);
                if (result == IDYES) {
                    SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE, 0);
                } else if (result == IDCANCEL) {
                    return 0;
                }
            }
            DestroyWindow(hwnd);
            return 0;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}