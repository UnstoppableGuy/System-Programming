#include "framework.h"
#include "lab_7.h"

#define MAX_LOADSTRING 100

#define IDI_LISTBOX 1

//#define MUTEX

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

DWORD THREAD_ID_1;
DWORD THREAD_ID_2;

HANDLE hHandle;
static HWND edit;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

DWORD writeToListBox(LPVOID*);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB7));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB7));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

DWORD WINAPI PassTextToEditEvent(wchar_t* str) {
    while (TRUE) {
        WaitForSingleObject(hHandle, INFINITE);
        SetWindowText(edit, str);
        SetEvent(hHandle);
        Sleep(500);
    }
    return 0;
}

CRITICAL_SECTION cs;

DWORD WINAPI PassTextToEditCritSect(wchar_t* str) {
    while (TRUE) {
        EnterCriticalSection(&cs);
        SetWindowText(edit, str);
        LeaveCriticalSection(&cs);

        Sleep(500);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HANDLE hThreads[3];

    switch (message)
    {
    case WM_CREATE:
    {
        edit = CreateWindow(L"edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            ES_LEFT,
            100, 140, 220, 30,
            hWnd, NULL, hInst, NULL);

        hHandle = CreateEvent(
            NULL,               // default security attributes
            FALSE,              // manual-reset event
            FALSE,              // initial state is nonsignaled
            TEXT("WriteEvent")  // object name
        );

        /*hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PassTextToEditEvent, (LPVOID)L"First", NULL, NULL);
        hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PassTextToEditEvent, (LPVOID)L"Second", NULL, NULL);
        hThreads[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PassTextToEditEvent, (LPVOID)L"Third", NULL, NULL);
        SetEvent(hHandle);*/
        InitializeCriticalSection(&cs);
        hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PassTextToEditCritSect, (LPVOID)L"First", NULL, NULL);
        hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PassTextToEditCritSect, (LPVOID)L"Second", NULL, NULL);
        hThreads[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PassTextToEditCritSect, (LPVOID)L"Third", NULL, NULL);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);
        DeleteCriticalSection(&cs);
        CloseHandle(hHandle);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}