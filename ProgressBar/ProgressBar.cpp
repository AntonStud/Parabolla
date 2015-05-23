// ProgressBar.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ProgressBar.h"
#include <commctrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

BOOL fDraw = FALSE;
POINT ptPrevious;

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
	LoadString(hInstance, IDC_PROGRESSBAR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROGRESSBAR));

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

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROGRESSBAR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PROGRESSBAR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	//RECT rc;
	HDC hdc;
	std::string value, anotherValue;
	int val;

	POINT currPoint;

	static HWND progress;
	static HWND startBtn, upBtn, downBtn, trackbar;
	static HWND velocityInit, angle, currX, currY;

	static int i, xView, yView;
	//static double y;
	//static char Buf[2];

	static int time;

	double SOmeDouvl;

	static std::shared_ptr<Ball> ball = std::make_shared<Ball>();

	//POINT aptStar[6] = { 50, 2, 2, 98, 98, 33, 2, 33, 98, 98, 50, 2 };

	//HBRUSH hbrWhite = (HBRUSH)(WHITE_BRUSH);
	//HBRUSH hbrGray = (HBRUSH)(GRAY_BRUSH);

	switch (message)
	{
	case WM_CREATE:

		progress = CreateWindowEx(0, PROGRESS_CLASS, _T(""), WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
			20, 380, 200, 50, hWnd, (HMENU)ID_PROGRESSBAR, hInst, 0);

		SendMessage(progress, PBM_SETRANGE, 0, MAKELPARAM(0, 10));
		SendMessage(progress, PBM_SETSTEP, (WPARAM)1, 0);

		startBtn = CreateWindowEx(0, _T("BUTTON"), _T("START"), WS_CHILD | WS_VISIBLE,
			270, 435, 60, 30, hWnd, (HMENU)ID_BTN_START, hInst, 0);

		upBtn = CreateWindowEx(0, _T("BUTTON"), _T("UP"), WS_CHILD | WS_VISIBLE,
			190, 435, 60, 30, hWnd, (HMENU)ID_BTN_UP, hInst, 0);

		downBtn = CreateWindowEx(0, _T("BUTTON"), _T("DOWN"), WS_CHILD | WS_VISIBLE,
			20, 435, 60, 30,  hWnd, (HMENU)ID_BTN_DOWN, hInst, 0);

		trackbar = CreateWindowEx(0, TRACKBAR_CLASS, _T(""),
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_HORZ | TBS_TOP /*| TBS_TOOLTIPS*/
			| TBS_ENABLESELRANGE/*| TBS_NOTHUMB*/ /*| TBS_ENABLESELRANGE*/,
			340, 380, 200, 50, hWnd, (HMENU)ID_TRACKBAR, hInst, 0);

		SendMessage(trackbar, TBM_SETRANGE, 0, MAKELPARAM(1, 179));

		velocityInit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
			WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
			220, 380, 30, 50, hWnd, (HMENU)ID_VELOCITY_EDIT, hInst, 0);

		angle = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
			WS_CHILD | WS_VISIBLE | ES_READONLY,
			540, 380, 30, 50, hWnd, (HMENU)ID_ANGLE_EDIT, hInst, 0);

		currX = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
			WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
			260, 380, 30, 30, hWnd, (HMENU)ID_X, hInst, 0);

		currY = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
			WS_CHILD | WS_VISIBLE | ES_READONLY,
			290, 380, 30, 30, hWnd, (HMENU)ID_Y, hInst, 0);
		
		val = SendMessage(progress, PBM_GETPOS, 0, 0);

		value = std::to_string(val);

		SendMessage(velocityInit, WM_SETTEXT, 0, (LPARAM)value.c_str());

		val = SendMessage(trackbar, TBM_GETPOS, 0, 0);

		anotherValue = std::to_string(val);

		SendMessage(angle, WM_SETTEXT, 0, (LPARAM)anotherValue.c_str());

		val = ball->GetCurrCoord().x;

		value = std::to_string(val);

		SendMessage(currX, WM_SETTEXT, 0, (LPARAM)value.c_str());

		val = ball->GetCurrCoord().y;

		value = std::to_string(val);

		SendMessage(currY, WM_SETTEXT, 0, (LPARAM)value.c_str());

		break;

	case WM_HSCROLL:

		val = SendMessage(trackbar, TBM_GETPOS, 0, 0);

		value = std::to_string(val);

		SendMessage(angle, WM_SETTEXT, 0, (LPARAM)value.c_str());

		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{

		case ID_BTN_UP:

			SendMessage(progress, PBM_DELTAPOS, 1, 0);

			val = SendMessage(progress, PBM_GETPOS, 0, 0);

			value = std::to_string(val);

			SendMessage(velocityInit, WM_SETTEXT, 0, (LPARAM)value.c_str());

			break;

		case ID_BTN_DOWN:
			//if (wmEvent == BN_CLICKED)
			
			SendMessage(progress, PBM_DELTAPOS, -1, 0);

			val = SendMessage(progress, PBM_GETPOS, 0, 0);

			value = std::to_string(val);

			SendMessage(velocityInit, WM_SETTEXT, 0, (LPARAM)value.c_str());

			break;

		case ID_BTN_START:

			val = SendMessage(trackbar, TBM_GETPOS, 0, 0);

			ball->SetDegree(val);

			val = SendMessage(progress, PBM_GETPOS, 0, 0);

			ball->SetVeloc(val);

			ball->ComputeVelosCos();

			ball->ComputeVelosSin();

			//SOmeDouvl = ball->GetmultVelosCos();

			//value = std::to_string(SOmeDouvl);

			//MessageBox(hWnd, value.c_str(), "", 0);

			time = 0;

			SetTimer(hWnd, ID_TIMER, 10, NULL);

		/*	val = SendMessage(progress, PBM_GETPOS, 0, 0);

			value = std::to_string(val);

			SendMessage(velocityInit, WM_SETTEXT, 0, (LPARAM)value.c_str());*/

			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	//case WM_LBUTTONDOWN:
	//	fDraw = TRUE;
	//	ptPrevious.x = LOWORD(lParam);
	//	ptPrevious.y = HIWORD(lParam);
	//	return 0L;

	//case WM_LBUTTONUP:
	//	if (fDraw)
	//	{
	//		hdc = GetDC(hWnd);
	//		MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL);
	//		//LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
	//		ReleaseDC(hWnd, hdc);
	//	}
	//	fDraw = FALSE;
	//	return 0L;

	//case WM_MOUSEMOVE:
	//	if (fDraw)
	//	{
	//		hdc = GetDC(hWnd);
	//		MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL);
	//		LineTo(hdc, ptPrevious.x = LOWORD(lParam),
	//			ptPrevious.y = HIWORD(lParam));
	//		ReleaseDC(hWnd, hdc);
	//	}

	case WM_SIZE:
		xView = LOWORD(lParam);
		yView = HIWORD(lParam);

		break;

	case WM_TIMER:
		switch (wParam)
		{
		case ID_TIMER:

			ball->ComputeCoord(time++);
			
			currPoint = ball->GetCurrCoord();

			val = ball->GetCurrCoord().x;

			value = std::to_string(val);

			//MessageBox(hWnd, value.c_str(), "", 0);

			SendMessage(currX, WM_SETTEXT, 0, (LPARAM)value.c_str());

			val = currPoint.y;

			value = std::to_string(val);

			SendMessage(currY, WM_SETTEXT, 0, (LPARAM)value.c_str());

			break;

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


//MapWindowPoints
//GetClientRect
//RECT
//BeginPaint, GetDC или функцию GetDCEx
//SetWindowRgn, SelectClipPath и SelectClipRgn
//WS_CLIPCHILDREN и WS_CLIPSIBLINGS
//ShowWindow
//SetWindowPos.
//CS_VREDRAW или CS_HREDRAW
/*GetClientRect(hWnd, &rc);
SetMapMode(hdc, MM_ANISOTROPIC);
SetWindowExtEx(hdc, 100, 100, NULL);
SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
Polyline(hdc, aptStar, 3);
*/
/*
if (IsIconic(hWnd))
{
GetClientRect(hWnd, &rc);
SetMapMode(hdc, MM_ANISOTROPIC);
SetWindowExtEx(hdc, 100, 100, NULL);
SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
Polyline(hdc, aptStar, 6);
}
else
{
TextOut(hdc, 0, 0, _T("Hello, Windows!"), 15);
}*/

//GetClientRect(hWnd, &rc);
//SetMapMode(hdc, MM_ANISOTROPIC);
//SetWindowExtEx(hdc, 100, 100, NULL);
//SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
////FillRect(hdc, &rc, hbrWhite);

//for (int i = 0; i < 13; i++)
//{
//	int x = (i * 40) % 100;
//	int y = ((i * 40) / 100) * 20;
//	SetRect(&rc, x, y, x + 20, y + 20);
//	FillRect(hdc, &rc, hbrGray);
//}