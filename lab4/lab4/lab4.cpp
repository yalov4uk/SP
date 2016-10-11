// lab4.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab4.h"
#include <math.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
      return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart)//рисует изображение
{
	BITMAP bm;
	HDC hdcMem = CreateCompatibleDC(hdc); // создаём контекст данных, совместимый с контекстом отображения

	SelectObject(hdcMem, hBitmap);// Выбираем изображение bitmap в контекст памяти
	// Для контекста памяти устанавливаем тот же режим отображения, что используется в контексте отображения
	SetMapMode(hdcMem, GetMapMode(hdc));
	
	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bm);// Определяем размеры изображения

	POINT ptSize{ bm.bmWidth, bm.bmHeight }, ptOrg{ 0, 0 };
	// Преобразуем координаты устройства в логические для устройства вывода
	DPtoLP(hdc, &ptSize, 1);
	// Преобразуем координаты устройства в логические для контекста памяти
	DPtoLP(hdcMem, &ptOrg, 1);
	// Рисуем изображение bitmap
	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);

	DeleteDC(hdcMem);// Удаляем контекст памяти
}

//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int my_bmp[]{ IDB_BITMAP1, IDB_BITMAP2, IDB_BITMAP3, IDB_BITMAP4 };// массив индетификаторов bitmapов
	static int xPos = 0, yPos = 0, x = 0, y = 0, n = 0, nx = 1, ny = 1, del;
    switch (message)
    {
	case WM_TIMER:
		del = sqrt(abs((x - xPos)) + abs(y - yPos));
		if (!del)
			break;
		nx = abs(x - xPos) / del;
		ny = abs(y - yPos) / del;

		if (x != xPos || y != yPos)
		{
			if (x > xPos) 
				x -= nx;
			else if (x < xPos)
				x += nx;
			if (y > yPos)
				y -= ny;
			else if (y < yPos)
				y += ny;
		}
		else
			KillTimer(hWnd, 1);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		SetTimer(hWnd, 1, 100, 0);//2 миллисекунды
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			DrawBitmap(hdc, LoadBitmap(hInst, MAKEINTRESOURCE(my_bmp[n])), x, y);
			if (++n == 4) 
				n = 0;
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