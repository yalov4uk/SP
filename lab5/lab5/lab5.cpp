// lab5.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab5.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND RButton_1, RButton_2, RButton_3, RButton_4, RButton_5, RButton_6, RButton_7, CBox_1, handler;
int xPos, yPos, my_R, my_G, my_B;
int message_child = -1;
int color, figure = 1;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
BOOL InitInstance1(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void print(HWND hwnd, int color, int type);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))//окно с чекбоксами
        return FALSE;

	if (!InitInstance1(hInstance, nCmdShow))//окно рисования
		return FALSE;

    MSG msg;
    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}

//  НАЗНАЧЕНИЕ: регистрирует класс окна.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;
	return RegisterClassExW(&wcex);

	return RegisterClassExW(&wcex);
}

//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//   КОММЕНТАРИИ:
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)//создаём первое окно с чекбоксами
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 500, 200, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return FALSE;

	RButton_1 = CreateWindowEx(NULL, L"BUTTON", L"Red", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 5, 5, 100, 20, hWnd, (HMENU)1001, hInstance, NULL);
	RButton_2 = CreateWindowEx(NULL, L"BUTTON", L"Blue", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 5, 25, 100, 20, hWnd, (HMENU)1002, hInstance, NULL);
	RButton_3 = CreateWindowEx(NULL, L"BUTTON", L"Green", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 5, 45, 100, 20, hWnd, (HMENU)1003, hInstance, NULL);

	RButton_4 = CreateWindowEx(NULL, L"BUTTON", L"Diamond", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 200, 5, 100, 20, hWnd, (HMENU)1004, hInstance, NULL);
	RButton_5 = CreateWindowEx(NULL, L"BUTTON", L"Square", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 200, 25, 100, 20, hWnd, (HMENU)1005, hInstance, NULL);
	RButton_6 = CreateWindowEx(NULL, L"BUTTON", L"Circle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 200, 45, 100, 20, hWnd, (HMENU)1006, hInstance, NULL);
	RButton_7 = CreateWindowEx(NULL, L"BUTTON", L"Star", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 200, 65, 100, 20, hWnd, (HMENU)1007, hInstance, NULL);

	CBox_1 = CreateWindowEx(NULL, L"BUTTON", L"Draw", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 400, 65, 100, 20, hWnd, (HMENU)1008, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

BOOL InitInstance1(HINSTANCE hInstance, int nCmdShow)//создаём пустое окно для рисования
{
	hInst = hInstance;

	handler = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,//хендлер окна будет принимать сообщение
		CW_USEDEFAULT, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);

	if (!handler)
		return FALSE;

	message_child = RegisterWindowMessage(L"draw");//регистрация сообщения в системе функций

	ShowWindow(handler, nCmdShow);
	UpdateWindow(handler);

	return TRUE;
}

void print(HWND hwnd, int color, int figure) //рисовалка
{
	if (hwnd != handler)
		return;
	HBRUSH hBrush;
	HDC hdc = GetDC(hwnd);
	switch (color) //выбор цвета
	{
		case 1:
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 2:
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			break;
		case 3:
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			break;
	}
	SelectObject(hdc, hBrush);//устанавливаем кисть
	switch (figure) //выбор фигуры
	{
		case 1://ромб
			POINT poly[5];

			poly[0].x = 0;
			poly[0].y = 150;

			poly[1].x = 150;
			poly[1].y = 0;

			poly[2].x = 300;
			poly[2].y = 150;

			poly[3].x = 150;
			poly[3].y = 300;

			poly[4].x = 0;
			poly[4].y = 150;

			Polygon(hdc, poly, 5);
			break;
		case 2://квадрат
			Rectangle(hdc, 0, 0, 300, 300);
			break;
		case 3://круг
			Ellipse(hdc, 0, 0, 300, 300);
			break;
		case 4://звезда
			POINT poly1[6];

			poly1[0].x = 100;
			poly1[0].y = 250;

			poly1[1].x = 150;
			poly1[1].y = 50;

			poly1[2].x = 200;
			poly1[2].y = 250;

			poly1[3].x = 50;
			poly1[3].y = 100;

			poly1[4].x = 250;
			poly1[4].y = 100;

			poly1[5].x = 100;
			poly1[5].y = 250;

			Polygon(hdc, poly1, 6);
			break;
	}
	DeleteObject(hBrush);//освобждаем кисть
}

//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool draw;
	static RECT rc;
	if (message == message_child) 
	{
		if (wParam == 45 && lParam == 45) //если есть галочка в чекбоксе Draw
			draw = !draw;
		else //установка цвета и фигуры
		{
			if (wParam != 0) 
				color = wParam;
			if (lParam != 0) 
				figure = lParam;
		}
		GetClientRect(hWnd, &rc);
		InvalidateRect(hWnd, &rc, TRUE);
	}
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1001://выбрать 1ый цвет, отключить оставшиеся
			SendMessage(RButton_1, BM_SETCHECK, 1, 0);
			SendMessage(RButton_2, BM_SETCHECK, 0, 0);
			SendMessage(RButton_3, BM_SETCHECK, 0, 0);
			SendMessage(handler, message_child, 1, 0);
			break;
		case 1002://выбрать 2ой цвет, отключить оставшиеся
			SendMessage(RButton_1, BM_SETCHECK, 0, 0);
			SendMessage(RButton_2, BM_SETCHECK, 1, 0);
			SendMessage(RButton_3, BM_SETCHECK, 0, 0);
			SendMessage(handler, message_child, 3, 0);
			break;
		case 1003://выбрать 3ий цвет, отключить оставшиеся
			SendMessage(RButton_1, BM_SETCHECK, 0, 0);
			SendMessage(RButton_2, BM_SETCHECK, 0, 0);
			SendMessage(RButton_3, BM_SETCHECK, 1, 0);
			SendMessage(handler, message_child, 2, 0);
			break;
		case 1004://аналогично
			SendMessage(RButton_4, BM_SETCHECK, 1, 0);
			SendMessage(RButton_5, BM_SETCHECK, 0, 0);
			SendMessage(RButton_6, BM_SETCHECK, 0, 0);
			SendMessage(RButton_7, BM_SETCHECK, 0, 0);
			SendMessage(handler, message_child, 0, 1);
			break;
		case 1005:
			SendMessage(RButton_4, BM_SETCHECK, 0, 0);
			SendMessage(RButton_5, BM_SETCHECK, 1, 0);
			SendMessage(RButton_6, BM_SETCHECK, 0, 0);
			SendMessage(RButton_7, BM_SETCHECK, 0, 0);
			SendMessage(handler, message_child, 0, 2);
			break;
		case 1006:
			SendMessage(RButton_4, BM_SETCHECK, 0, 0);
			SendMessage(RButton_5, BM_SETCHECK, 0, 0);
			SendMessage(RButton_6, BM_SETCHECK, 1, 0);
			SendMessage(RButton_7, BM_SETCHECK, 0, 0);
			SendMessage(handler, message_child, 0, 3);
			break;
		case 1007:
			SendMessage(RButton_4, BM_SETCHECK, 0, 0);
			SendMessage(RButton_5, BM_SETCHECK, 0, 0);
			SendMessage(RButton_6, BM_SETCHECK, 0, 0);
			SendMessage(RButton_7, BM_SETCHECK, 1, 0);
			SendMessage(handler, message_child, 0, 4);
			break;
		case 1008://если есть галочка в чекбоксе
			SendMessage(handler, message_child, 45, 45);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		if (draw)//рисуем выбранную фигуру
			print(hWnd, color, figure);
		else//рисуем белый фон
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
			HPEN hPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255));;
			SelectObject(hdc, hBrush);
			SelectObject(hdc, hPen);
			Rectangle(hdc, 0, 0, 400, 400);
			DeleteObject(hBrush);
			DeleteObject(hPen);
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