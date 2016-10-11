#include <windows.h>
#include <tchar.h>
#include "resource.h"

#define MOVE_KOORDINATE 1
#define START_KOORDINATE_X 350
#define START_KOORDINATE_Y 200
#define LIMIT_KOORDINATE_X 250
#define LIMIT_KOORDINATE_Y 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
LPCWSTR My_cursor = L"C:\\Users\\Валера\\Desktop\\Университет\\СП\\Labs\\lab1\\lab1\\cursor.cur";

int APIENTRY _tWinMain(HINSTANCE This, // Дескриптор текущего приложения
						HINSTANCE Prev, // В современных системах всегда 0
						LPTSTR cmd, // Командная строка
						int mode) // Режим отображения окна
{
	HWND hWnd; // Дескриптор главного окна программы
	MSG msg; // Структура для хранения сообщения
	WNDCLASS wc; // Класс окна
				 // Определение класса окна
	wc.hInstance = This;
	wc.lpszClassName = WinName; // Имя класса окна
	wc.lpfnWndProc = WndProc; // Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль окна
	wc.hIcon = LoadIcon(This, MAKEINTRESOURCE(IDI_ICON1)); // Пользовательская иконка
	wc.hCursor = LoadCursorFromFile(My_cursor); // Пользовательский курсор
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // меню
	wc.cbClsExtra = 0; // Нет дополнительных данных класса
	wc.cbWndExtra = 0; // Нет дополнительных данных окна
					   // Заполнение окна белым цветом
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc)) // Регистрация класса окна
		return 0; 			 
	// Создание окна						
	hWnd = CreateWindow(WinName, // Имя класса окна
		_T("Lab1"), // Заголовок окна
		WS_OVERLAPPEDWINDOW, // Стиль окна
		CW_USEDEFAULT,// x
		CW_USEDEFAULT,// y Размеры окна
		CW_USEDEFAULT,// Width
		CW_USEDEFAULT,// Height
		HWND_DESKTOP, // Дескриптор родительского окна
		NULL, // Будем использовать меня класса
		This, // Дескриптор приложения
		NULL); // Дополнительной информации нет
	ShowWindow(hWnd, mode); //Показать окно
	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);// Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg); // Посылает сообщение функции WndProc()
	}
	return 0;
}

// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
							WPARAM wParam, LPARAM lParam)
{ // Обработчик сообщений
	static int x = 0, y = 0;
	static bool flag;
	HDC hdc; //дескриптор контекста устройства
	PAINTSTRUCT ps; // структура, содержащая информацию о клиентской области (размеры, цвет и тп)
	switch (message)
	{
		case WM_COMMAND:
			// Проверка идентификатора командного пункта меню.
			switch (wParam) 
			{
				case ID_MENU_START:
					flag = true;
					SetTimer(hWnd, 1, 1, 0);
					break;
				case ID_MENU_STOP:
					flag = false;
					KillTimer(hWnd, 1);
					break;
			}
			break;
		case WM_TIMER:
			if (x != LIMIT_KOORDINATE_X && y == 0)
				x += MOVE_KOORDINATE;
			else if (x == LIMIT_KOORDINATE_X && y != LIMIT_KOORDINATE_Y)
				y += MOVE_KOORDINATE;
			else if (x != 0 && y == LIMIT_KOORDINATE_Y)
				x -= MOVE_KOORDINATE;
			else if(x == 0 && y != 0)
				y -= MOVE_KOORDINATE;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps); // Получаем индекс контекста устройства
			if (flag)
				// Выводим текстовую строку
				TextOut(hdc, START_KOORDINATE_X + x, START_KOORDINATE_Y + y, L"Hello from WM_PAINT", 18);
			EndPaint(hWnd, &ps); // Отдаем индекс контекста устройства
			break;
		case WM_DESTROY: 
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
			break; 
		default: 
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}