// lab6.cpp: определяет точку входа для приложения.
//11.3.11 6 шариков по кругу

#include "stdafx.h"
#include "lab6.h"
#include <math.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hwnd;
struct winparams 
{ // структура для передачи параметров в объединённом виде для потока в windows
	int x; // указатель на первый файл
	int y; // указатель на втоорй файл
};

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
    LoadStringW(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
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
//
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
   //2 кнопки: start, stop
   CreateWindowEx(NULL, L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 5, 100, 20, hWnd, (HMENU)1001, hInstance, NULL);
   CreateWindowEx(NULL, L"BUTTON", L"Stop", WS_VISIBLE | WS_CHILD | WS_BORDER, 105, 5, 100, 20, hWnd, (HMENU)1002, hInstance, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}	

DWORD WINAPI Func_Thread(LPVOID lpParameter)//функция потока
{
	struct winparams strct = *((struct winparams*)lpParameter);
	HDC hdc = GetDC(hwnd);
	int a = 0, x0 = strct.x, y0 = strct.y;//получение начальных координат
	while (true)//бесконечная отрисовка окружностей
	{
		Sleep(10);
		strct.x = x0 + 30 * cos(50 * a);	
		strct.y = y0 + 30 * sin(50 * a);
		Ellipse(hdc, strct.x, strct.y, strct.x + 10, strct.y + 10);
		a += 1;
	}
	return 0;
}

//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND — обработать меню приложения
//  WM_DESTROY — отправить сообщение о выходе и вернуться
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static winparams coord;
	static bool flag;//статус потоков
	static HANDLE hThreads[7];
    switch (message)
    {
    case WM_COMMAND:
        {		
            switch (LOWORD(wParam))
            {
			case 1001://start
				hwnd = hWnd;//для функции потока
				if (flag)//если потоки уже были запущены
					for (int i = 1; i < 7; i++)
						ResumeThread(hThreads[i]);//декрементируем их счётчики
				else
				{
					for (int i = 1; i < 7; i++)//запускаем потоки впервые
					{
						coord.x = i * 100;
						coord.y = 200;//coord - аргумент передаваемый в функцию потока
						hThreads[i] = CreateThread(NULL, 0, &Func_Thread, &coord, 0, NULL);
						Sleep(100);
					}
					if (!flag) 
						flag = true;
				}
				break;
			case 1002://stop
				if (flag)
				{
					for (int i = 1; i < 7; i++)//останавливаем потоки, инкрементируем счётчики потоков
						SuspendThread(hThreads[i]);
					flag = false;
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
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