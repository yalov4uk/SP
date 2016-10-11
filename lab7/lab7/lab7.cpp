// lab7.cpp: ���������� ����� ����� ��� ����������.
//12.3.9 Event and Critical section

#include "stdafx.h"
#include "lab7.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
HWND listbox1, listbox2;
HANDLE event;
CRITICAL_SECTION CriticalSection;

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
    LoadStringW(hInstance, IDC_LAB7, szWindowClass, MAX_LOADSTRING);
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

   listbox1 = CreateWindowEx(NULL, L"LISTBOX", L"lstbox1", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_EXTENDEDSEL | LBS_STANDARD, 10, 40, 200, 300, hWnd, NULL, hInst, NULL);
   listbox2 = CreateWindowEx(NULL, L"LISTBOX", L"lstbox2", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_EXTENDEDSEL | LBS_STANDARD, 300, 40, 200, 300, hWnd, NULL, hInst, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

DWORD WINAPI Func_Thread(LPVOID lpParameter)//������� ������
{
	while (true)
		if (int(lpParameter) == 1 || int(lpParameter) == 2)
		{
			WaitForSingleObject(event, INFINITE);//�������� ������� true
			ResetEvent(event);//������ = false
			if (int(lpParameter) == 1)
				SendMessage(listbox1, LB_INSERTSTRING, 0, (LPARAM)L"First");
			else
				SendMessage(listbox1, LB_INSERTSTRING, 0, (LPARAM)L"Second");
			Sleep(1000);
			SetEvent(event);//������ = true
		}
		else
		{
			EnterCriticalSection(&CriticalSection);//���� � ����������� ������
			if (int(lpParameter) == 3)
				SendMessage(listbox2, LB_INSERTSTRING, 0, (LPARAM)L"First");
			else
				SendMessage(listbox2, LB_INSERTSTRING, 0, (LPARAM)L"Second");
			Sleep(1000);
			LeaveCriticalSection(&CriticalSection);//����� �� ����������� ������
			Sleep(100);//����� ����� ������� ������ ����� � ����������� ������
		}
	return 0;
}

//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//  ����������:  ������������ ��������� � ������� ����.
//  WM_DESTROY � ��������� ��������� � ������ � ���������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThreads[4];
    switch (message)
    {
	case WM_CREATE://��� �������� ���� ������ 2 listboxa
		InitializeCriticalSection(&CriticalSection);
		event = CreateEvent(NULL, FALSE, TRUE, NULL);//��������� ��������� - ����������

		hThreads[0] = CreateThread(NULL, 0, &Func_Thread, (LPVOID)1, 0, NULL);//event, first
		hThreads[1] = CreateThread(NULL, 0, &Func_Thread, (LPVOID)2, 0, NULL);//event, second
		
		hThreads[2] = CreateThread(NULL, 0, &Func_Thread, (LPVOID)3, 0, NULL);//section, first
		hThreads[3] = CreateThread(NULL, 0, &Func_Thread, (LPVOID)4, 0, NULL);//section, second
		break;
    case WM_DESTROY:
		DeleteCriticalSection(&CriticalSection);//����������� ������
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}