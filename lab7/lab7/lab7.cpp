// lab7.cpp: определяет точку входа для приложения.
//12.3.9 Event and Critical section

#include "stdafx.h"
#include "lab7.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND listbox1, listbox2;
HANDLE event;
CRITICAL_SECTION CriticalSection;

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
    LoadStringW(hInstance, IDC_LAB7, szWindowClass, MAX_LOADSTRING);
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

   listbox1 = CreateWindowEx(NULL, L"LISTBOX", L"lstbox1", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_EXTENDEDSEL | LBS_STANDARD, 10, 40, 200, 300, hWnd, NULL, hInst, NULL);
   listbox2 = CreateWindowEx(NULL, L"LISTBOX", L"lstbox2", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_EXTENDEDSEL | LBS_STANDARD, 300, 40, 200, 300, hWnd, NULL, hInst, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

DWORD WINAPI Func_Thread(LPVOID lpParameter)//функция потока
{
	while (true)
		if (int(lpParameter) == 1 || int(lpParameter) == 2)
		{
			WaitForSingleObject(event, INFINITE);//ожидание сигнала true
			ResetEvent(event);//сигнал = false
			if (int(lpParameter) == 1)
				SendMessage(listbox1, LB_INSERTSTRING, 0, (LPARAM)L"First");
			else
				SendMessage(listbox1, LB_INSERTSTRING, 0, (LPARAM)L"Second");
			Sleep(1000);
			SetEvent(event);//сигнал = true
		}
		else
		{
			EnterCriticalSection(&CriticalSection);//вход в критическую секцию
			if (int(lpParameter) == 3)
				SendMessage(listbox2, LB_INSERTSTRING, 0, (LPARAM)L"First");
			else
				SendMessage(listbox2, LB_INSERTSTRING, 0, (LPARAM)L"Second");
			Sleep(1000);
			LeaveCriticalSection(&CriticalSection);//выход из критической секции
			Sleep(100);//дадим время другому потоку войти в критическую секцию
		}
	return 0;
}

//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_DESTROY — отправить сообщение о выходе и вернуться
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThreads[4];
    switch (message)
    {
	case WM_CREATE://при создании окна делаем 2 listboxa
		InitializeCriticalSection(&CriticalSection);
		event = CreateEvent(NULL, FALSE, TRUE, NULL);//начальное состояние - сигнальное

		hThreads[0] = CreateThread(NULL, 0, &Func_Thread, (LPVOID)1, 0, NULL);//event, first
		hThreads[1] = CreateThread(NULL, 0, &Func_Thread, (LPVOID)2, 0, NULL);//event, second
		
		hThreads[2] = CreateThread(NULL, 0, &Func_Thread, (LPVOID)3, 0, NULL);//section, first
		hThreads[3] = CreateThread(NULL, 0, &Func_Thread, (LPVOID)4, 0, NULL);//section, second
		break;
    case WM_DESTROY:
		DeleteCriticalSection(&CriticalSection);//освобождаем память
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}