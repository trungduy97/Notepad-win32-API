// 1512064.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512064.h"

#define MAX_LOADSTRING 100

bool isopen = false;
bool path = false;


void LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName);
void Save(char* name, HWND Tx);

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_1512064, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_1512064));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU2);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


HWND hWndEdit;
int nHeight, nWidth;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		hWndEdit = CreateWindow(L"EDIT", L"", ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE | WS_CHILD, 0, 0, rect.right, rect.bottom, hWnd, 0, 0, 0);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_EXIT32787:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_NEW32783:
			EnableMenuItem(GetMenu(hWnd), ID_FILE_SAVE32786, MF_BYCOMMAND | MF_DISABLED);
			SetWindowText(hWndEdit, L"");
			break;
		case ID_FILE_OPEN32785:
			{
			WCHAR szFile[260];
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"Text Documents (*.txt)\0*.TXT\0All Files (*.*)\0*.*\0";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn))
			{
				LoadTextFileToEdit(hWndEdit, ofn.lpstrFile);
				isopen = true;
				path = ofn.lpstrFile;
				SetWindowText(hWnd, ofn.lpstrFile);
			}
			break;
			}
		case ID_EDIT_DELETE:
			SendMessage(hWndEdit, WM_CLEAR, 0, 0);
			break;
		case ID_EDIT_UNDO32791:
			SendMessage(hWndEdit, WM_UNDO, 0, 0);
			break;
		case ID_EDIT_COPPY:
			SendMessage(hWndEdit, WM_COPY, 0, 0);
			break;
		case ID_EDIT_CUT32792:
			SendMessage(hWndEdit, WM_CUT, 0, 0);
			break;
		case ID_EDIT_PASTE32794:
			SendMessage(hWndEdit, WM_PASTE, 0, 0);
			break;
		case ID_EDIT_SELECTALL32796:
			SendMessage(hWndEdit, EM_SETSEL, 0, -1);
			break;
		case ID_FORMAT_FONT32790:
			HDC hdc;
			CHOOSEFONT cf;            // common dialog box structure
			static LOGFONT lf;        // logical font structure
			static DWORD rgbCurrent;  // current text color
			HFONT hfont, hfontPrev;
			DWORD rgbPrev;

			// Initialize CHOOSEFONT
			ZeroMemory(&cf, sizeof(cf));
			cf.lStructSize = sizeof (cf);
			cf.hwndOwner = hWnd;
			cf.lpLogFont = &lf;
			cf.rgbColors = rgbCurrent;
			cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

			if (ChooseFont(&cf))
			{
				hfont = CreateFontIndirect(cf.lpLogFont);
				rgbCurrent = cf.rgbColors;
				SendMessage(hWndEdit, WM_SETFONT, (WPARAM)hfont, TRUE);
				SetTextColor((HDC)hWndEdit, rgbCurrent);
			}
			break;
		case ID_HELP_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;	
		case ID_FILE_SAVE32786:
		{
			Save("output.txt", hWndEdit);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		nWidth = LOWORD(lParam);
		nHeight = HIWORD(lParam);
		MoveWindow(hWndEdit, 0, 0, nWidth, nHeight, true);
		ShowWindow(hWndEdit, SW_SHOWNORMAL);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
	HANDLE hFile;

	hFile = CreateFileW(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;

		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xFFFFFFFF)
		{
			LPWSTR pszFileText;

			pszFileText = (WCHAR*)GlobalAlloc(GPTR, dwFileSize + 1);
			if (pszFileText != NULL)
			{
				DWORD dwRead;

				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0; // Add null terminator
					SetWindowText(hEdit, pszFileText);
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
}

void Save(char* name, HWND Tx) {
	wfstream f;
	f.open(name, ios::out);
	WCHAR* buffer = NULL;
	int size = 0;
	size = GetWindowTextLength(Tx);
	buffer = new WCHAR[size + 1];
	GetWindowText(Tx, buffer, size + 1);
	f << buffer;
	f.close();
}


