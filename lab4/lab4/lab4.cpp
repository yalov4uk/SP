// lab4.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "lab4.h"
#include <math.h>

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����

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
    LoadStringW(hInstance, IDC_LAB4, szWindowClass, MAX_LOADSTRING);
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

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart)//������ �����������
{
	BITMAP bm;
	HDC hdcMem = CreateCompatibleDC(hdc); // ������ �������� ������, ����������� � ���������� �����������

	SelectObject(hdcMem, hBitmap);// �������� ����������� bitmap � �������� ������
	// ��� ��������� ������ ������������� ��� �� ����� �����������, ��� ������������ � ��������� �����������
	SetMapMode(hdcMem, GetMapMode(hdc));
	
	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bm);// ���������� ������� �����������

	POINT ptSize{ bm.bmWidth, bm.bmHeight }, ptOrg{ 0, 0 };
	// ����������� ���������� ���������� � ���������� ��� ���������� ������
	DPtoLP(hdc, &ptSize, 1);
	// ����������� ���������� ���������� � ���������� ��� ��������� ������
	DPtoLP(hdcMem, &ptOrg, 1);
	// ������ ����������� bitmap
	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);

	DeleteDC(hdcMem);// ������� �������� ������
}

//  ����������:  ������������ ��������� � ������� ����.
//  WM_COMMAND � ���������� ���� ����������
//  WM_PAINT � ���������� ������� ����
//  WM_DESTROY � ��������� ��������� � ������ � ���������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int my_bmp[]{ IDB_BITMAP1, IDB_BITMAP2, IDB_BITMAP3, IDB_BITMAP4 };// ������ ��������������� bitmap��
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
		SetTimer(hWnd, 1, 100, 0);//2 ������������
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