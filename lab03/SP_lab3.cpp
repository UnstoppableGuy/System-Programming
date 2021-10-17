// SP_lab3.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_lab3.h"
#include <stdlib.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_SPLAB3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB3));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLAB3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
BOOL _stdcall DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    POINT pt;
    MoveToEx(hdc, x1, y1, &pt);
    return LineTo(hdc, x2, y2);
}

void DrawRand(BOOL (_stdcall*f)(HDC, int, int, int, int), HDC hdc, RECT rc)
{
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    f(hdc, rand() % width, rand() % height, rand() % width, rand() % height);
}

void DrawPolyRand(HDC hdc, RECT rc)
{
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    POINT arr3[3];
    arr3[0] = { rand() % width, rand() % height };
    arr3[1] = { rand() % width, rand() % height };
    arr3[2] = { rand() % width, rand() % height };
    Polyline(hdc, arr3, 3);
}

void Draw(HWND hWnd, RECT rc, HDC hdc)
{
    GetClientRect(hWnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
  
    for (int i = 0; i < 4; i++)
    {
        DrawRand(Ellipse, hdc, rc);
        DrawRand(DrawLine, hdc, rc);
        DrawRand(Rectangle, hdc, rc);
        Arc(hdc, rand() % width, rand() % height, rand() % width, rand() % height, rand() % width, rand() % height, rand() % width, rand() % height);
        DrawPolyRand(hdc, rc);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;
    GetClientRect(hWnd, &rc);
    static HWND b1;
    static HWND b2;
    switch (message)
    {
    case WM_CREATE:
        b1 = CreateWindow(L"Button", L"Click", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            30, 20, 110, 40, hWnd, (HMENU)1, hInst, NULL);
        b2 = CreateWindow(L"Button", L"Click", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            150, 20, 110, 40, hWnd, (HMENU)2, hInst, NULL);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case 1:
            hdc = GetWindowDC(hWnd);
            Draw(hWnd, rc, hdc);
            break;
        case 2:
            GetClientRect(hWnd, &rc);
            InvalidateRect(hWnd, &rc, TRUE);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DRAWITEM:
        LPDRAWITEMSTRUCT lpInfo;
        lpInfo = (LPDRAWITEMSTRUCT)lParam;
        if (lpInfo->CtlID == 1) {
            RECT rcItem;
            rcItem = lpInfo->rcItem;

            FillRect(lpInfo->hDC, &rcItem, CreateSolidBrush(RGB(7, 7, 7)));

            int width = rcItem.right - rcItem.left;
            int height = rcItem.bottom - rcItem.top;

            RECT rcIm;
            rcIm = { width / 2 - width / 3, height / 2 - height / 3,
                width / 2 + width / 3, height / 2 + height / 3 };

            Rectangle(lpInfo->hDC, rcIm.left, rcIm.top, rcIm.right, rcIm.bottom);
            FillRect(lpInfo->hDC, &rcIm, CreateSolidBrush(RGB(66, 128, 7)));
        }
        if (lpInfo->CtlID == 2) {
            RECT rcItem;
            rcItem = lpInfo->rcItem;

            FillRect(lpInfo->hDC, &rcItem, CreateSolidBrush(RGB(7, 7, 7)));
            int width = rcItem.right - rcItem.left;
            int height = rcItem.bottom - rcItem.top;

            RECT rcIm;
            rcIm = { width / 2 - width / 3, height / 2 - height / 3,
               width / 2 + width / 3, height / 2 + height / 3 };

            Rectangle(lpInfo->hDC, rcIm.left, rcIm.top, rcIm.right, rcIm.bottom);
            FillRect(lpInfo->hDC, &rcIm, CreateSolidBrush(RGB(128, 66, 7)));
        }
        break;
    case WM_DESTROY:
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
