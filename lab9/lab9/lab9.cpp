// lab9.cpp: ���������� ����� ����� ��� ����������.
//14.3.2 ����� �������������� ������ � �������

#include "stdafx.h"
#include "lab9.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
HWND hList;

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
    LoadStringW(hInstance, IDC_LAB9, szWindowClass, MAX_LOADSTRING);
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

void Scan(HKEY root=HKEY_CURRENT_USER, LPWSTR subkey=L"") //����������� ������� ������ ������������� ������
{
	HKEY key;
	//��������� subkey; � key ���������� ���������� ��������� �����
	int err = RegOpenKeyEx(root, subkey, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &key);

	if (err)//��� ������
		return;

	int index = 0;
	char* name[10000];
	//�������� ���������� �������� �� ��� �������� ��������� ����� �������
	while ((err = RegEnumKey(key, index++, (LPWSTR)name, 10000)) != ERROR_NO_MORE_ITEMS)
		if (lstrlen(subkey) == 0)
			Scan(root, (LPWSTR)name);
		else 
		{
			char path[10000];
			swprintf((LPWSTR)path, L"%s\\%s", subkey, name);
			Scan(root, (LPWSTR)path);
		}

	index = 0;
	int type, size = 1000, namesize = 1000;
	char data[1000];
	//������ �������� �� ������� ����� ���������� � data
	while (!(err = RegEnumValue(key, index++, (LPWSTR)name, (LPDWORD)&namesize, 0, (LPDWORD)&type, (LPBYTE)data, (LPDWORD)&size))) 
	{
		char str[10000];
		swprintf((LPWSTR)str, L"%s\\%s = %s\n", subkey, name, data);//������ ����: ����, ��������, ��������/����
		if (lstrlen((LPWSTR)data) > 2)
			if (wcsstr((LPWSTR)data, L"C:")) //���� ����� �� ����� C
			{
				//�������� ������� ����
				if (CreateFile((LPWSTR)data, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) == INVALID_HANDLE_VALUE) 
				{
					WIN32_FIND_DATA d;
					//���� ����
					if (FindFirstFile((LPWSTR)data, &d) == INVALID_HANDLE_VALUE)
						//���� �� ������� - ��������� � listbox
						SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);
				}
			}
		namesize = size = 1000;
	}
	RegCloseKey(key);
}


DWORD WINAPI StartScan(void* p) //������� ������
{
	Scan();
	return NULL;
}

//  ����������:  ������������ ��������� � ������� ����.
//  WM_DESTROY � ��������� ��������� � ������ � ���������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		hList = CreateWindowEx(NULL, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 0, 0, 1000, 400, hWnd, (HMENU)1001, hInst, NULL);
		CreateThread(NULL, 1000000000, &StartScan, NULL, NULL, 0);//������ ������
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}