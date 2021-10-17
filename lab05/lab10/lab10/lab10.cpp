
#include "framework.h"
#include "lab10.h"

#define MAX_LOADSTRING 100
//my defines
#define RBUTTON11 1
#define RBUTTON12 2
#define RBUTTON13 3
#define RBUTTON21 4
#define RBUTTON22 5
#define RBUTTON23 6
#define RBUTTON24 7
#define CHECKBOX  8

#define DRAW_MESSAGE L"DrawMessage"
#define COLOR_MESSAGE L"ColorMessage"
#define SHAPE_MESSAGE L"ShapeMessage"

// Global Variables:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND main_hWnd;// это хэндл другого окна
UINT WM_DRAW;
UINT WM_COLOR;
UINT WM_SHAPE;

void debug(int lol)
{
	char b[15];
	_itoa_s((int)lol, b, 15, 10);
	SetWindowTextA(main_hWnd, b);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB10, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB10));

	MSG msg;

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



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB10));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB10);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 250, 250, nullptr, nullptr, hInstance, nullptr);

	CreateWindow(L"button", L"Red",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 10, 80, 30, hWnd, (HMENU)RBUTTON11, hInstance, NULL);
	CreateWindow(L"button", L"Green",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 35, 80, 30, hWnd, (HMENU)RBUTTON12, hInstance, NULL);
	CreateWindow(L"button", L"Blue",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 60, 80, 30, hWnd, (HMENU)RBUTTON13, hInstance, NULL);

	CreateWindow(L"button", L"Diamond",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		110, 10, 80, 30, hWnd, (HMENU)RBUTTON21, hInstance, NULL);
	CreateWindow(L"button", L"Square",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		110, 35, 80, 30, hWnd, (HMENU)RBUTTON22, hInstance, NULL);
	CreateWindow(L"button", L"Circle",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		110, 60, 80, 30, hWnd, (HMENU)RBUTTON23, hInstance, NULL);
	CreateWindow(L"button", L"Star",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		110, 85, 80, 30, hWnd, (HMENU)RBUTTON24, hInstance, NULL);

	CheckRadioButton(hWnd, RBUTTON11, RBUTTON13, RBUTTON11);
	CheckRadioButton(hWnd, RBUTTON21, RBUTTON24, RBUTTON21);

	CreateWindow(L"button", L"Draw",
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		10, 150, 80, 30, hWnd, (HMENU)CHECKBOX, hInstance, NULL);

	WM_DRAW = RegisterWindowMessage((LPCWSTR)DRAW_MESSAGE);
	WM_COLOR = RegisterWindowMessage((LPCWSTR)COLOR_MESSAGE);
	WM_SHAPE = RegisterWindowMessage((LPCWSTR)SHAPE_MESSAGE);

	main_hWnd = FindWindow(NULL, L"lab10_main");

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	int wmEvent;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:

		if (wmId >= RBUTTON11 && wmId <= RBUTTON13)
		{

			if (main_hWnd == NULL) break;


			SendMessage(main_hWnd, WM_COLOR, 0, wmId - RBUTTON11);

			CheckRadioButton(hWnd, RBUTTON11, RBUTTON13, wmId);
		}
		else if (wmId >= RBUTTON21 && wmId <= RBUTTON24)
		{
			if (main_hWnd == NULL) break;


			SendMessage(main_hWnd, WM_SHAPE, 0, wmId - RBUTTON21);

			CheckRadioButton(hWnd, RBUTTON21, RBUTTON24, wmId);
		}
		else switch (wmId)
		{
		case CHECKBOX:
		{
			if (main_hWnd == NULL) break;


			SendMessage(main_hWnd, WM_DRAW, 0, 0);

			HWND hWndCheck = GetDlgItem(hWnd, CHECKBOX);
			LRESULT state = SendMessage(hWndCheck, BM_GETCHECK, 0, 0);
			//debug(state);
			if (state == BST_CHECKED)
			{
				SendMessage(main_hWnd, WM_DRAW, 0, 1);
			}
			if (state == BST_UNCHECKED)
			{
				SendMessage(main_hWnd, WM_DRAW, 0, 0);
			}
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
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
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
