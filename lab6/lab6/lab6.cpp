// lab6.cpp: ���������� ����� ����� ��� ����������.
//11.3.11 6 ������� �� �����

#include "stdafx.h"
#include "lab6.h"
#include <math.h>

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
HWND hwnd;
struct winparams 
{ // ��������� ��� �������� ���������� � ����������� ���� ��� ������ � windows
	int x; // ��������� �� ������ ����
	int y; // ��������� �� ������ ����
};

// ��������� ���������� �������, ���������� � ���� ������ ����:
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
    // ������������� ���������� �����
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ��������� ������������� ����������:
    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    MSG msg;

    // ���� ��������� ���������:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

//  ����������: ������������ ����� ����.
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

//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
      return FALSE;
   //2 ������: start, stop
   CreateWindowEx(NULL, L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 5, 100, 20, hWnd, (HMENU)1001, hInstance, NULL);
   CreateWindowEx(NULL, L"BUTTON", L"Stop", WS_VISIBLE | WS_CHILD | WS_BORDER, 105, 5, 100, 20, hWnd, (HMENU)1002, hInstance, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}	

DWORD WINAPI Func_Thread(LPVOID lpParameter)//������� ������
{
	struct winparams strct = *((struct winparams*)lpParameter);
	HDC hdc = GetDC(hwnd);
	int a = 0, x0 = strct.x, y0 = strct.y;//��������� ��������� ���������
	while (true)//����������� ��������� �����������
	{
		Sleep(10);
		strct.x = x0 + 30 * cos(50 * a);	
		strct.y = y0 + 30 * sin(50 * a);
		Ellipse(hdc, strct.x, strct.y, strct.x + 10, strct.y + 10);
		a += 1;
	}
	return 0;
}

//  ����������:  ������������ ��������� � ������� ����.
//  WM_COMMAND � ���������� ���� ����������
//  WM_DESTROY � ��������� ��������� � ������ � ���������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static winparams coord;
	static bool flag;//������ �������
	static HANDLE hThreads[7];
    switch (message)
    {
    case WM_COMMAND:
        {		
            switch (LOWORD(wParam))
            {
			case 1001://start
				hwnd = hWnd;//��� ������� ������
				if (flag)//���� ������ ��� ���� ��������
					for (int i = 1; i < 7; i++)
						ResumeThread(hThreads[i]);//�������������� �� ��������
				else
				{
					for (int i = 1; i < 7; i++)//��������� ������ �������
					{
						coord.x = i * 100;
						coord.y = 200;//coord - �������� ������������ � ������� ������
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
					for (int i = 1; i < 7; i++)//������������� ������, �������������� �������� �������
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