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
LPCWSTR My_cursor = L"C:\\Users\\������\\Desktop\\�����������\\��\\Labs\\lab1\\lab1\\cursor.cur";

int APIENTRY _tWinMain(HINSTANCE This, // ���������� �������� ����������
						HINSTANCE Prev, // � ����������� �������� ������ 0
						LPTSTR cmd, // ��������� ������
						int mode) // ����� ����������� ����
{
	HWND hWnd; // ���������� �������� ���� ���������
	MSG msg; // ��������� ��� �������� ���������
	WNDCLASS wc; // ����� ����
				 // ����������� ������ ����
	wc.hInstance = This;
	wc.lpszClassName = WinName; // ��� ������ ����
	wc.lpfnWndProc = WndProc; // ������� ����
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ����
	wc.hIcon = LoadIcon(This, MAKEINTRESOURCE(IDI_ICON1)); // ���������������� ������
	wc.hCursor = LoadCursorFromFile(My_cursor); // ���������������� ������
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // ����
	wc.cbClsExtra = 0; // ��� �������������� ������ ������
	wc.cbWndExtra = 0; // ��� �������������� ������ ����
					   // ���������� ���� ����� ������
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc)) // ����������� ������ ����
		return 0; 			 
	// �������� ����						
	hWnd = CreateWindow(WinName, // ��� ������ ����
		_T("Lab1"), // ��������� ����
		WS_OVERLAPPEDWINDOW, // ����� ����
		CW_USEDEFAULT,// x
		CW_USEDEFAULT,// y ������� ����
		CW_USEDEFAULT,// Width
		CW_USEDEFAULT,// Height
		HWND_DESKTOP, // ���������� ������������� ����
		NULL, // ����� ������������ ���� ������
		This, // ���������� ����������
		NULL); // �������������� ���������� ���
	ShowWindow(hWnd, mode); //�������� ����
	// ���� ��������� ���������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);// ������� ���������� ����� ������� �������
		DispatchMessage(&msg); // �������� ��������� ������� WndProc()
	}
	return 0;
}

// ������� ������� ���������� ������������ ��������
// � �������� ��������� �� ������� ��� ������� ����������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
							WPARAM wParam, LPARAM lParam)
{ // ���������� ���������
	static int x = 0, y = 0;
	static bool flag;
	HDC hdc; //���������� ��������� ����������
	PAINTSTRUCT ps; // ���������, ���������� ���������� � ���������� ������� (�������, ���� � ��)
	switch (message)
	{
		case WM_COMMAND:
			// �������� �������������� ���������� ������ ����.
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
			hdc = BeginPaint(hWnd, &ps); // �������� ������ ��������� ����������
			if (flag)
				// ������� ��������� ������
				TextOut(hdc, START_KOORDINATE_X + x, START_KOORDINATE_Y + y, L"Hello from WM_PAINT", 18);
			EndPaint(hWnd, &ps); // ������ ������ ��������� ����������
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