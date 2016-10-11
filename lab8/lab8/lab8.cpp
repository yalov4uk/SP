#include "stdafx.h"
#include "lab8.h"
#include <tlhelp32.h>

#define MAX_LOADSTRING		100
#define PRIORITY_IDLE		151
#define PRIORITY_NORMAL		152
#define PRIORITY_HIGH		153

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

HWND listbox_proc, listbox_module;

ATOM MyRegisterClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID PrintProcessList();
VOID PrintModuleList(DWORD);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine, _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1080, 550, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;

	listbox_proc = CreateWindowEx(NULL, L"LISTBOX", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | LBS_EXTENDEDSEL | LBS_STANDARD, 20, 25, 500, 400, hWnd, (HMENU)1001, hInst, NULL);
	listbox_module = CreateWindowEx(NULL, L"LISTBOX", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | LBS_EXTENDEDSEL | LBS_STANDARD, 540, 25, 500, 400, hWnd, NULL, hInst, NULL);

	PrintProcessList();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

VOID PrintProcessList()//����� ���� ��������� � �� �����������
{
	SendMessage(listbox_proc, LB_RESETCONTENT, NULL, NULL);//�������� lb1

	TCHAR buff[1024];
	PROCESSENTRY32 ProcessEntry;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//������� ������� ���������� ���������

	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);//Before calling the Process32First function, set this member to sizeof(PROCESSENTRY32)
	Process32First(hSnapshot, &ProcessEntry);//���������� � processentry 1�� �������
	DWORD id_proc;

	do
	{
		id_proc = ProcessEntry.th32ProcessID;//�������� ������������� ��������
		switch (GetPriorityClass(OpenProcess(PROCESS_ALL_ACCESS, FALSE, id_proc)))//����� ����� ���������� ��������
		{
		case IDLE_PRIORITY_CLASS:
			wsprintf(buff, L"Name: %s Priority: %s", ProcessEntry.szExeFile, L"LOW");
			break;
		case NORMAL_PRIORITY_CLASS:
			wsprintf(buff, L"Name: %s Priority: %s", ProcessEntry.szExeFile, L"NORMAL");
			break;
		case HIGH_PRIORITY_CLASS:
			wsprintf(buff, L"Name: %s Priority: %s", ProcessEntry.szExeFile, L"HIGH");
			break;
		case REALTIME_PRIORITY_CLASS:
			wsprintf(buff, L"Name: %s Priority: %s", ProcessEntry.szExeFile, L"REALTIME");
			break;
		default:
			wsprintf(buff, L"Name: %s Priority: %s", ProcessEntry.szExeFile, L"UNDEFINED");
			break;
		}

		DWORD ind = SendMessage(listbox_proc, LB_ADDSTRING, NULL, (LPARAM)buff);
		SendMessage(listbox_proc, LB_SETITEMDATA, (WPARAM)ind, (LPARAM)id_proc);//������������� ������������ ����� ��������
	} while (Process32Next(hSnapshot, &ProcessEntry));//���� ���� ��� ��������

	CloseHandle(hSnapshot);//����������� �������
}

VOID PrintModuleList(DWORD dwProcessId)//����� ������� ���������� ��������
{
	SendMessage(listbox_module, LB_RESETCONTENT, NULL, NULL);//�������� lb2
	MODULEENTRY32 ModuleEntry;
	TCHAR buff[1024];

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);//������� ������� ���������� ��������
	if (INVALID_HANDLE_VALUE == hSnapshot)
		return;

	ModuleEntry.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &ModuleEntry);//��������� 1�� ������ � ModuleEntry
	do
	{
		wsprintf(buff, L"%s", ModuleEntry.szModule);//���������� �������� ������ � buff
		SendMessage(listbox_module, LB_ADDSTRING, NULL, (LPARAM)buff);
	} while (Module32Next(hSnapshot, &ModuleEntry));//������� � ���������� ������

	CloseHandle(hSnapshot);//����������� �������
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static DWORD id;
	static HANDLE hProcess;//���������� ��������
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1001://��� �� ��������
			int item_num;
			wchar_t buff[1000];
			if (SendMessage(listbox_proc, LB_GETSELITEMS, 1, (LPARAM)&item_num) > 0)
			{//���� ������� ������
				//����� id ��������
				id = SendMessage(listbox_proc, LB_GETITEMDATA, (WPARAM)item_num, NULL);
				PrintModuleList(id);//� ������� ��� ������
			}
			break;
		case PRIORITY_IDLE://��� �� ��������� ���� low
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);//�������� �������
			SetPriorityClass(hProcess, IDLE_PRIORITY_CLASS);//������������� ���������
			CloseHandle(hProcess);//����������� ������
			PrintProcessList();//���������� ���������
			break;
		case PRIORITY_NORMAL://��� �� ��������� ���� normal
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
			SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
			CloseHandle(hProcess);
			PrintProcessList();
			break;
		case PRIORITY_HIGH://��� �� ��������� ���� high
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
			SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
			CloseHandle(hProcess);
			PrintProcessList();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CONTEXTMENU://��� ����� �������
		int item_num;
		if (SendMessage(listbox_proc, LB_GETSELITEMS, 1, (LPARAM)&item_num) > 0)
		{//����� id ���������� ��������
			HMENU hMenu = CreatePopupMenu();//�������� ������� ����
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);//�������� ������� �� id
			DWORD priority_cl = GetPriorityClass(hProcess);//�������� ���������
			TCHAR low[1024];
			TCHAR norm[1024];
			TCHAR high[1024];
			//���������� ������ �������� ���� �� ��������� ������
			wsprintf(low, L"LOW %s", priority_cl == IDLE_PRIORITY_CLASS ? L"<-" : L"");
			wsprintf(norm, L"NORMAL %s", priority_cl == NORMAL_PRIORITY_CLASS ? L"<-" : L"");
			wsprintf(high, L"HIGH %s", priority_cl == HIGH_PRIORITY_CLASS ? L"<-" : L"");
			//��������� ������ � ����, ��������� ������ ������ � ���������� id
			AppendMenu(hMenu, MFT_STRING, PRIORITY_IDLE, low);
			AppendMenu(hMenu, MFT_STRING, PRIORITY_NORMAL, norm);
			AppendMenu(hMenu, MFT_STRING, PRIORITY_HIGH, high);
			//��������� ����
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
			DestroyMenu(hMenu);//����������� ������
			CloseHandle(hProcess);
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