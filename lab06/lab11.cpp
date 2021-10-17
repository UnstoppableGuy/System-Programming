// lab11.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab11.h"
#include <cmath>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
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

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB11));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

#define START_BUTTON_ID 1001
#define STOP_BUTTON_ID 1002

int winWidth = 300;
const int center_x0 = 200, center_y0 = 200;

HANDLE threads[8];

struct ThreadArgs {
	int time;
	double speed;
	double angle;
	int x, y;
} args[8];

DWORD WINAPI RotateStick(ThreadArgs *args) {
	for (;;) {
		args->time = (args->time + 1) % 125;
		args->x = int(center_x0 + args->time * args->speed * cos(args->angle));
		args->y = int(center_y0 + args->time * args->speed * sin(args->angle));
		Sleep(20);
	}
	return 0;
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 400, 400, nullptr, nullptr, hInstance, nullptr);

   CreateWindow(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE,
	   20, 10, 150, 30, hWnd, (HMENU)START_BUTTON_ID, hInstance, nullptr);
   CreateWindow(L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE,
	   200, 10, 150, 30, hWnd, (HMENU)STOP_BUTTON_ID, hInstance, nullptr);

   for (int i = 0; i < 8; ++i) {
	   args[i].time = 0;
	   args[i].speed = double(i + 1) / 8;
	   args[i].angle = 2 * 3.14151 / 8 * i;
	   args[i].x = center_x0;
	   args[i].y = center_y0;
   }

   for (int i = 0; i < 8; ++i) {
	   threads[i] = CreateThread(nullptr, 0u, (LPTHREAD_START_ROUTINE)RotateStick, args + i, 0, nullptr);
	   SuspendThread(threads[i]);
   }

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

bool active = false;

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
			case START_BUTTON_ID:
				if (!active) {
					for (int i = 0; i < 8; ++i) {
						ResumeThread(threads[i]);
					}

					SetTimer(hWnd, NULL, 20, nullptr);

					active = true;
				}
				break;
			case STOP_BUTTON_ID:
				if (active) {
					for (int i = 0; i < 8; ++i) {
						SuspendThread(threads[i]);
					}
					active = false;
				}
				break;
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
	case WM_TIMER:
		InvalidateRect(hWnd, nullptr, true);
		if (active)
			SetTimer(hWnd, NULL, 20, nullptr);
		break;
	case WM_SIZE:
		winWidth = LOWORD(lParam);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hdc, brush);

			for (int i = 0; i < 8; ++i) {
				MoveToEx(hdc, center_x0, center_y0, nullptr);
				LineTo(hdc, args[i].x, args[i].y);
			}
			
			EndPaint(hWnd, &ps);
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

// Обработчик сообщений для окна "О программе".
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
