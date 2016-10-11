// lab3.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "lab3.h"
#include <iostream>
#include <ctime>

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
HDWP Button_1, Button_2;

// ��������� ���������� �������, ���������� � ���� ������ ����:
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
    // ������������� ���������� �����
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB3, szWindowClass, MAX_LOADSTRING);
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

   Button_1 = CreateWindowEx(NULL, L"BUTTON", L"Button 1", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 600, 5, 45, 45, hWnd, (HMENU)1001, hInstance, NULL);
   Button_2 = CreateWindowEx(NULL, L"BUTTON", L"Button 2", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 700, 5, 45, 45, hWnd, (HMENU)1002, hInstance, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void PrintPicture(HDC hdc, int x1, int y1)//������� ������
{
	POINT poly1[3];
	POINT poly2[3];

	HPEN hPen;
	srand(time(0));
	int x = rand() % 2;
	if (x)
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // ������� �������� �������� �����
	else 
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // ������� �������� ������� �����
	SelectObject(hdc, hPen);

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x1 + 100, y1 - 100);
	LineTo(hdc, x1 + 100 * 2, y1);
	Arc(hdc, x1 + 100, y1 - 100 / 2, x1 + 100 * 2, y1 + 100 / 2, x1 + 100 * 2, y1, x1 + 100, y1);
	Arc(hdc, x1, y1 - 100 / 2, x1 + 100, y1 + 100 / 2, x1 + 100, y1, x1, y1);
	Rectangle(hdc, x1, y1, x1 + 100 * 2, y1 + 100 * 2);
	Rectangle(hdc, x1 + 100 / 4, y1 + 100 / 4, x1 + 100, y1 + 100);
	Ellipse(hdc, x1 + 100 / 10 * 7, y1 + 100 / 10 * 5, x1 + 100 / 10 * 8, y1 + 100 / 10 * 7);
	Ellipse(hdc, x1 + 100 / 10 * 7, y1 + 100 / 10 * 6, x1 + 100 / 10 * 8, y1 + 100 / 10 * 8);

	poly1[0].x = 100;
	poly1[0].y = 300;
	poly2[0].x = 300;
	poly2[0].y = 300;

	poly1[1].x = 50;
	poly1[1].y = 300;
	poly2[1].x = 350;
	poly2[1].y = 300;

	poly1[2].x = 50;
	poly1[2].y = 350;
	poly2[2].x = 350;
	poly2[2].y = 350;

	Polyline(hdc, poly1, 3);
	Polyline(hdc, poly2, 3);

	MoveToEx(hdc, x1 - 100 / 2, y1 + 100 / 2 * 5, NULL);
	LineTo(hdc, x1, y1 + 100 / 2 * 5);
	LineTo(hdc, x1, y1 + 100 / 4 * 9);
	LineTo(hdc, x1 + 100 / 2, y1 + 100 / 4 * 9);
	LineTo(hdc, x1 + 100 / 2, y1 + 100 / 2 * 5);
	LineTo(hdc, x1 + 100, y1 + 100 / 2 * 5);
	LineTo(hdc, x1 + 100, y1 + 100 / 4 * 9);
	LineTo(hdc, x1 + 100 / 2 * 3, y1 + 100 / 4 * 9);
	LineTo(hdc, x1 + 100 / 2 * 3, y1 + 100 / 2 * 5);
	LineTo(hdc, x1 + 100 * 2, y1 + 100 / 2 * 5);
	LineTo(hdc, x1 + 100 * 2, y1 + 100 / 4 * 9);
	LineTo(hdc, x1 + 100 / 2 * 5, y1 + 100 / 4 * 9);
}

void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap)//������ �����������
{
	BITMAP bm;
	POINT  ptSize, ptOrg;
	HDC hMemDC = CreateCompatibleDC(hDC);// ������� �������� ������, ����������� � ���������� �����������
	HBITMAP hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);// �������� ����������� bitmap � �������� ������
	if (hOldbm)// ���� �� ���� ������, ���������� ������
	{
		// ��� ��������� ������ ������������� ��� ��
		// ����� �����������, ��� ������������ �
		// ��������� �����������
		SetMapMode(hMemDC, GetMapMode(hDC));
		// ���������� ������� �����������
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);//��������� ����������� � bm

		ptSize.x = bm.bmWidth;   // ������
		ptSize.y = bm.bmHeight;  // ������
		// ����������� ���������� ���������� � ���������� ��� ���������� ������
		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;
		// ����������� ���������� ���������� � ���������� ��� ��������� ������
		DPtoLP(hMemDC, &ptOrg, 1);
		// ������ ����������� bitmap
		BitBlt(hDC, x, y, ptSize.x, ptSize.y, hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);
		// ��������������� �������� ������
		SelectObject(hMemDC, hOldbm);
	}
	// ������� �������� ������
	DeleteDC(hMemDC);
}

void DrawButton(LPDRAWITEMSTRUCT lpInfo)//���������� ������
{
	HBITMAP hbm;
	int ResourceID = IDB_BITMAP4;
	// ������������ ��������� WM_DRAWITEM ������ ���� ��� ��������� �� ������ 
	if (lpInfo->CtlType == ODT_BUTTON)
	{
		switch (lpInfo->CtlID)//������� �� ������
		{
		case 1001:
		{
			if (lpInfo->itemState & ODS_SELECTED)
				ResourceID = IDB_BITMAP3;//������� ���������
			else
				ResourceID = IDB_BITMAP4;//������� ���������
			break;
		}
		case 1002:
		{
			if (lpInfo->itemState & ODS_SELECTED)
				ResourceID = IDB_BITMAP4;
			else
				ResourceID = IDB_BITMAP3;
			break;
		}
		default:
			return;
		}
		// ��������� ����������� ������ �� �������� ����������
		hbm = LoadBitmap(hInst, MAKEINTRESOURCE(ResourceID));
		// ��� ������ ������ �� ������
		if (hbm && ((lpInfo->itemAction & ODA_DRAWENTIRE) || (lpInfo->itemAction & ODA_SELECT)))
		// ���� ������ ������� � �� ���� ������� ������������, �������� ������� DrawBitmap
			DrawBitmap(lpInfo->hDC, (lpInfo->rcItem).left, (lpInfo->rcItem).top, hbm);// ������ ������
	}
	DeleteObject(hbm);// ������� ����������� ������, ��� ��� ��� ��� ������ �� �����
}

//  ����������:  ������������ ��������� � ������� ����.
//  WM_COMMAND � ���������� ���� ����������
//  WM_PAINT � ���������� ������� ����
//  WM_DESTROY � ��������� ��������� � ������ � ���������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool flag;
    switch (message)
    {
	case WM_DRAWITEM:
		{
			DrawButton((LPDRAWITEMSTRUCT)lParam);// �������������� ������
			break;
		}
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))//��������� ������� ������
            {
			case 1001:
				if (!flag) 
					flag = true;
				InvalidateRect(hWnd, 0, 0);
				break;
			case 1002:
				if (flag) 
					flag = false;
				InvalidateRect(hWnd, 0, 0);
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
			break;
        }
    case WM_PAINT:
        {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			if (flag)
				PrintPicture(hdc, 100, 100);//����������� �����
			else//�������� �����
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
			}
			EndPaint(hWnd, &ps);
			break;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}