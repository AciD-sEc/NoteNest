// Auther : Tanay 
#include <windows.h>
#include <commdlg.h>  // For open/save file dialogs
#include <stdio.h>    // For file operations
#include <wchar.h>    // For wide-character file handling

#define ID_FILE_NEW 1
#define ID_FILE_OPEN 2
#define ID_FILE_SAVE 3
#define ID_FILE_EXIT 4

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
void AddMenus(HWND hwnd);
void AddControls(HWND hwnd);
void DoFileOpen(HWND hwnd);
void DoFileSave(HWND hwnd);

// Global variable for the text area
HWND hEdit;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"NotepadApp";  // Use wide string here
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"NotepadApp", L"Simple Notepad", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                  100, 100, 800, 600, NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_COMMAND:
            switch (wp) {
                case ID_FILE_NEW:
                    SetWindowText(hEdit, L"");  // Clears the text box
                    break;
                case ID_FILE_OPEN:
                    DoFileOpen(hwnd);
                    break;
                case ID_FILE_SAVE:
                    DoFileSave(hwnd);
                    break;
                case ID_FILE_EXIT:
                    PostQuitMessage(0);  // Closes the application
                    break;
            }
            break;
        case WM_CREATE:
            AddMenus(hwnd);
            AddControls(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

void AddMenus(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hFileMenu, MF_STRING, ID_FILE_NEW, L"New");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, L"Open");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_SAVE, L"Save");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, L"Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd) {
    hEdit = CreateWindowW(L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | 
                          ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                          0, 0, 800, 600, hwnd, NULL, NULL, NULL);
}

void DoFileOpen(HWND hwnd) {
    OPENFILENAME ofn;
    wchar_t fileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(fileName);
    ofn.lpstrFilter = L"Text Files\0*.TXT\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        FILE *file;
        _wfopen_s(&file, fileName, L"r");
        if (file) {
            wchar_t buffer[1024];
            ZeroMemory(buffer, sizeof(buffer));
            fread(buffer, sizeof(wchar_t), 1024, file);
            SetWindowTextW(hEdit, buffer);
            fclose(file);
        }
    }
}

void DoFileSave(HWND hwnd) {
    OPENFILENAME ofn;
    wchar_t fileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(fileName);
    ofn.lpstrFilter = L"Text Files\0*.TXT\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetSaveFileNameW(&ofn)) {
        FILE *file;
        _wfopen_s(&file, fileName, L"w");
        if (file) {
            int length = GetWindowTextLengthW(hEdit);
            wchar_t *buffer = new wchar_t[length + 1];
            GetWindowTextW(hEdit, buffer, length + 1);
            fwrite(buffer, sizeof(wchar_t), length, file);
            delete[] buffer;
            fclose(file);
        }
    }
}
