// lab2.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab2.h"


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
WCHAR buffer[MAX_LOADSTRING];
WCHAR buffer1[MAX_LOADSTRING];
HWND listbox1, listbox2, edit1;
int num, number;
int indexes[MAX_LOADSTRING];

// Отправить объявления функций, включенных в этот модуль кода:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); //LAB2
	LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);//Lab2
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
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = NULL;
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

   CreateWindowEx(NULL, L"STATIC", L"input:", WS_VISIBLE | WS_CHILD, 610, 10, 150, 20, hWnd, NULL, hInstance, NULL);
   CreateWindowEx(NULL, L"BUTTON", L"Add", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 5, 100, 20, hWnd, (HMENU)1001, hInstance, NULL);
   CreateWindowEx(NULL, L"BUTTON", L"Clear", WS_VISIBLE | WS_CHILD | WS_BORDER, 115, 5, 100, 20, hWnd, (HMENU)1002, hInstance, NULL);
   CreateWindowEx(NULL, L"BUTTON", L"ToRight", WS_VISIBLE | WS_CHILD | WS_BORDER, 225, 5, 100, 20, hWnd, (HMENU)1003, hInstance, NULL);
   CreateWindowEx(NULL, L"BUTTON", L"Delete", WS_VISIBLE | WS_CHILD | WS_BORDER, 335, 5, 100, 20, hWnd, (HMENU)1004, hInstance, NULL);
   listbox1 = CreateWindowEx(NULL, L"LISTBOX", L"lstbox1", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_EXTENDEDSEL | LBS_STANDARD, 10, 40, 200, 200, hWnd, NULL, hInstance, NULL);
   listbox2 = CreateWindowEx(NULL, L"LISTBOX", L"lstbox2", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_EXTENDEDSEL | LBS_STANDARD, 310, 40, 200, 200, hWnd, NULL, hInstance, NULL);
   edit1 = CreateWindowEx(NULL, L"EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 610, 40, 150, 20, hWnd, NULL, hInstance, NULL);
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
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
			case 1001://add
				if (SendMessage(edit1, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer))//строка из editа
				{
					number = SendMessage(listbox1, LB_GETCOUNT, 0, 0);//кол-во эл-ов в LB1
					if (number)	//тут перезапишем всё что имелось раньше
						for (int i = 0; i < number; i++)
						{
							SendMessage(listbox1, LB_GETTEXT, i, (LPARAM)buffer1);
							if (wcscmp(buffer, buffer1) == 0)//проверка на уникальность
								break;
							else if (i == number - 1)
							{
								SendMessage(listbox1, LB_INSERTSTRING, 0, (LPARAM)buffer);//добавляем новый элемент
								break;
							}
						}
					else
						SendMessage(listbox1, LB_INSERTSTRING, 0, (LPARAM)buffer);//сработает когда LB2 - пуст
				}
				break;
			case 1002://clear
				SendMessage(listbox1, LB_RESETCONTENT, 0, 0);
				SendMessage(listbox2, LB_RESETCONTENT, 0, 0);
				break;
			case 1003://to right
				if (SendMessage(listbox1, LB_GETSELITEMS, sizeof(indexes), (LPARAM)indexes))//если есть выбранный элемент
				{
					SendMessage(listbox1, LB_GETTEXT, indexes[0], (LPARAM)buffer);//записываем его в buffer
					number = SendMessage(listbox2, LB_GETCOUNT, 0, 0);//кол-во эл-ов в LB2
					if (number)//оставляем всё что было
						for (int i = 0; i < number; i++)
						{
							SendMessage(listbox2, LB_GETTEXT, i, (LPARAM)buffer1);
							if (wcscmp(buffer, buffer1) == 0)//проверка на уникальность
								break;
							else if (i == number - 1)
							{
								SendMessage(listbox2, LB_INSERTSTRING, 0, (LPARAM)buffer);
								break;
							}
						}
					else
						SendMessage(listbox2, LB_INSERTSTRING, 0, (LPARAM)buffer);//сработает когда LB2 - пуст
				}
				break;
			case 1004://delete
				if (num = SendMessage(listbox1, LB_GETSELITEMS, sizeof(indexes), (LPARAM)indexes))//получаем количество выбранных элементов
					while(num)
						SendMessage(listbox1, LB_DELETESTRING, indexes[--num], 0);//удаляем элемент
				if (num = SendMessage(listbox2, LB_GETSELITEMS, sizeof(indexes), (LPARAM)indexes))
					while (num)
						SendMessage(listbox2, LB_DELETESTRING, indexes[--num], 0);
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