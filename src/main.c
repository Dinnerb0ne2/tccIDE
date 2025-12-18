#include <ide.h>
#include <resources.h>
#include <fileio.h>
#include <editor.h>
#include <compiler.h>

// Global variables
HINSTANCE g_hInst;
HWND g_hMainWnd;
HWND g_hEditor;
HWND g_hOutput;
HWND g_hStatusBar;
WCHAR g_szFilePath[MAX_PATH] = L"";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    g_hInst = hInstance;
    
    // Initialize common controls
    INITCOMMONCONTROLSEX icex = {sizeof(INITCOMMONCONTROLSEX), ICC_BAR_CLASSES};
    InitCommonControlsEx(&icex);
    
    // Register window class
    WNDCLASSEXW wc = {sizeof(WNDCLASSEXW), CS_HREDRAW | CS_VREDRAW, MainWndProc,
                      0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION),
                      LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1),
                      NULL, IDE_CLASS_NAME, LoadIcon(NULL, IDI_APPLICATION)};
                      
    if (!RegisterClassExW(&wc)) {
        MessageBoxW(NULL, L"Window registration failed", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    // Create main window
    g_hMainWnd = CreateWindowExW(0, IDE_CLASS_NAME, L"C IDE - TCC Based",
                                 WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                                 CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
                                 NULL, NULL, hInstance, NULL);
                                 
    if (!g_hMainWnd) return 1;
    
    ShowWindow(g_hMainWnd, nCmdShow);
    UpdateWindow(g_hMainWnd);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            extern void UI_CreateMenu(HWND);
            extern void UI_CreateStatusBar(HWND);
            extern void UI_CreateLayout(HWND);
            
            UI_CreateMenu(hWnd);
            UI_CreateStatusBar(hWnd);
            UI_CreateLayout(hWnd);
            SetWindowTextW(g_hStatusBar, L"Ready");
            return 0;
        }
        
        case WM_SIZE: {
            RECT rc;
            GetClientRect(hWnd, &rc);
            int statusH = 20, outputH = 150;
            
            SetWindowPos(g_hEditor, NULL, 0, 0, rc.right, rc.bottom - statusH - outputH, SWP_NOZORDER);
            SetWindowPos(g_hOutput, NULL, 0, rc.bottom - statusH - outputH, rc.right, outputH, SWP_NOZORDER);
            SetWindowPos(g_hStatusBar, NULL, 0, rc.bottom - statusH, rc.right, statusH, SWP_NOZORDER);
            return 0;
        }
        
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDM_FILE_NEW:
                    FileIO_New();
                    break;
                    
                case IDM_FILE_OPEN: {
                    WCHAR path[MAX_PATH];
                    if (FileIO_Open(hWnd, path)) {
                        wcscpy(g_szFilePath, path);
                        WCHAR title[MAX_PATH + 20];
                        wsprintfW(title, L"C IDE - %s", path);
                        SetWindowTextW(hWnd, title);
                    }
                    break;
                }
                
                case IDM_FILE_SAVE:
                    if (wcslen(g_szFilePath))
                        FileIO_Save(hWnd, g_hEditor, g_szFilePath);
                    else
                        FileIO_SaveAs(hWnd, g_hEditor, g_szFilePath);
                    break;
                    
                case IDM_FILE_SAVEAS:
                    FileIO_SaveAs(hWnd, g_hEditor, g_szFilePath);
                    break;
                    
                case IDM_FILE_EXIT:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    break;
                    
                case IDM_EDIT_CUT:
                    Editor_Cut(g_hEditor);
                    break;
                    
                case IDM_EDIT_COPY:
                    Editor_Copy(g_hEditor);
                    break;
                    
                case IDM_EDIT_PASTE:
                    Editor_Paste(g_hEditor);
                    break;
                    
                case IDM_EDIT_UNDO:
                    Editor_Undo(g_hEditor);
                    break;
                    
                case IDM_COMPILE_RUN: {
                    SetWindowTextW(g_hOutput, L"");
                    char* code = Editor_GetText(g_hEditor);
                    char result[8192] = {0};
                    
                    if (Compiler_RunCode(code, result, sizeof(result)) == 0) {
                        SetWindowTextA(g_hOutput, result);
                        SetWindowTextW(g_hStatusBar, L"Success");
                    } else {
                        SetWindowTextA(g_hOutput, Compiler_GetLastError());
                        SetWindowTextW(g_hStatusBar, L"Error");
                    }
                    
                    free(code);
                    break;
                }
            }
            return 0;
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hWnd, msg, wParam, lParam);
}