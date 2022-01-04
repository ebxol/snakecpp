// snake.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "snake.h"
#include <math.h>
#include <fstream>
#include <iomanip>
#include <ctime>

//using namespace std; //only one library with scope resolution is used, so using is ok here

#define MAX_LOADSTRING 100

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
	LoadString(hInstance, IDC_SNAKE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKE));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SNAKE);
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

   //hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	//CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	  hWnd = CreateWindowEx(NULL, szTitle, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);


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

void UnitVector(coord_t &vec)
{
	float mag = sqrt( float(vec.X*vec.X) + float(vec.Y*vec.Y));
	vec.X = int(vec.X / mag) > 0 ? int(vec.X / mag) : vec.X;
	vec.Y = int(vec.Y / mag) > 0 ? int(vec.Y / mag) : vec.Y;
}

void UnitVector(float &X, float &Y)
{
	float mag = sqrt(X*X + Y*Y);
	X = X / mag;
	Y = Y / mag;
}

float GetAngleWC(int *X, int *Y)
{
	float mag = (float)sqrt((*X)*(*X) + (*Y)*(*Y));
	float vAng = (float)acos(float(*X)/mag);

	if (*Y > 0)
	{
		vAng *= -1;
		vAng += 2 * (float)PI;
	}
		
	return vAng;
}

void CartesianToWindows(const int &WX, const int &WY, int &X, int &Y) //lol
{
	if ((X < 0 && WX > 0) || (X > 0 && WX < 0))
		X *= -1;

	if ((Y < 0 && WY > 0) || (Y > 0 && WY < 0))
		Y *= -1;
}

void PolarToRectangular(const float &theta, const int &rad, int &X, int &Y)
{
	X = int(rad*cos(theta));
	Y = int(rad*sin(theta));
}

int Snake::GetCoord(bool bY)
{
	if (bY)
		return YVal;

	else return XVal;
}

void Snake::GetVelocity(coord_t &v)
{
	v.X = Vv.X;
	v.Y = Vv.Y;
}

int Snake::FindOutlyingCoords()
{
	return (Length + HeadRadius);
}

void Snake::UpdateVals()
{
	//update vals
	Snake::OldX = Snake::XVal;
	Snake::OldY = Snake::YVal;
}

void Snake::Animate(HDC hdc, HWND hWnd, HBRUSH brush)
{
	RECT trc;	//trc needs to cover outermost extent of outermost ellipses on both sides
	int trcx = FindOutlyingCoords();
	trc.left = Snake::OldX - trcx;
	trc.top = Snake::OldY - trcx;
	trc.right = Snake::OldX + trcx;
	trc.bottom = Snake::OldY + trcx;

	//cover old ellipse
	FillRect(hdc, &trc, brush);
	//draw new ellipse
	brush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));

	//get velocity direction for drawing
	coord_t uv;
	GetVelocity(uv);
	float vAngle = GetAngleWC(&uv.X, &uv.Y) - float(PI);
	int drawX = 0, drawY = 0;

	//DRAW

	//head
	Ellipse(hdc, -HeadRadius + Snake::XVal, -HeadRadius + Snake::YVal, HeadRadius + Snake::XVal, HeadRadius + Snake::YVal);

	//body
	PolarToRectangular(vAngle, Length / 2, drawX, drawY);
	CartesianToWindows(-uv.X, -uv.Y, drawX, drawY);
	XBody = XVal + drawX;
	YBody = YVal + drawY;
	Ellipse(hdc, BodyRadius + XBody, BodyRadius + YBody, -BodyRadius + XBody, -BodyRadius + YBody); //was fixed at -16

	//tail
	PolarToRectangular(vAngle, Length - int(sqrt(Length) / 2), drawX, drawY);	//slight adjustment to polar distance
	CartesianToWindows(-uv.X, -uv.Y, drawX, drawY);
	XTail = XVal + drawX;
	YTail = YVal + drawY;
	Ellipse(hdc, TailRadius + XTail, TailRadius + YTail, -TailRadius + XTail, -TailRadius + YTail); //was fixed at -32
}

void Snake::MoveToward(HDC hdc, HWND hWnd, POINT dest)
{
	HBRUSH brush;
	bool bDoAnim = true;

	brush = (HBRUSH)SelectObject(hdc, GetStockObject(WHITE_BRUSH));

	Animate(hdc, hWnd, brush);

	UpdateVals();

	int newVX = (int(dest.x) - XVal);
	int newVY = (int(dest.y) - YVal);

	//get winsize, store it in rect 
	RECT rect;
	GetClientRect(hWnd, &rect);

	if (newVX > 10 || newVX < -10)
	{
		do
		{
			newVX = int(float(newVX) / 2.f);

		} while (abs(newVX) > 10);
	}

	if (newVY > 10 || newVY < -10)
	{
		do
		{
			newVY = int(float(newVY) / 2.f);

		} while (abs(newVY) > 10);
	}

	//dont want weird behaviour
	if (XVal + HeadRadius > rect.right || XVal + HeadRadius < rect.left || XVal < 0)	//**BUMP EVENT**
	{
		Vv.X = -Vv.X;
		bDoAnim = false;

		if (dest.x < rect.right || dest.x > rect.left)
		{

		}

	}

	if (YVal + HeadRadius > rect.bottom || YVal + HeadRadius < rect.top || YVal < 0)
	{
		Vv.Y = -Vv.Y;
		bDoAnim = false;

		if (dest.y < rect.bottom || dest.y > rect.top)
		{
			
		}
	}

	if (bDoAnim)
	{
		Vv.X = newVX;
		Vv.Y = newVY;
	}

	XVal += Vv.X;
	YVal += Vv.Y;

	//old brush goes back
	SelectObject(hdc, brush);
}

void Snake::Idle(HDC hdc, HWND hWnd)
{
	HBRUSH brush;

	brush = (HBRUSH)SelectObject(hdc, GetStockObject(WHITE_BRUSH));

	Animate(hdc, hWnd, brush);

	UpdateVals();

	//new coordinates
	Snake::XVal += Vv.X;
	Snake::YVal += Vv.Y;

	//get winsize, store it in rect 
	RECT rect;
	GetClientRect(hWnd, &rect);

	//if the circle is off the edge, then reverse direction
	if (XVal + HeadRadius > rect.right || XVal + HeadRadius < rect.left || XVal < 0)	//**BUMP EVENT**
	{
		Vv.X = -Vv.X;
	}

	if (YVal + HeadRadius > rect.bottom || YVal + HeadRadius < rect.top || YVal < 0)
	{
		Vv.Y = -Vv.Y;
	}

	//old brush goes back
	SelectObject(hdc, brush);
}

//create a snake
Snake greysnake(0, 0, 16, 12, 8, 64, 5, 5);
Snake *gsptr;

template<class T> float Distance(T X1, T Y1, int X2, int Y2)
{
	int iX1 = (int)X1;
	int iY1 = (int)Y1;

	return (float)sqrt((Y2 - Y1)*(Y2 - Y1) + (X2 - X1)*(X2 - X1));
}

template<class T> void LogTypeData(T a, T b, char *id)
{
	static bool bLogged = false;

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	if (!bLogged)
	{
		std::ofstream slog("SNAKE.LOG");
		slog << "SNAKE DATA LOG " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << std::endl;
		slog.close();
		bLogged = true;
	}

	std::fstream rlog("SNAKE.LOG");

    if (rlog.is_open())
	{
		rlog.seekp(0, std::ios::end);
		rlog << id << " ";
		rlog << "(" << a << "," << b << ")" << std::endl;

		rlog.close();
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	POINT ucp;
	POINT scp;
	static int logtime = 0;

	switch (message)
	{

	case WM_CREATE:
			//create the timer for 1/10 seconds, 3rd parameter; 2nd parameter identifies timer, 4th is function call
			SetTimer(hWnd, 1, 20, NULL);
			//point to snake
			gsptr = &greysnake;
			break;

	case WM_TIMER: //timer goes off on callback, NULL function call is set previously
		hdc = GetDC(hWnd);
		logtime++;

		if (gsptr != NULL)
		{
			bool bDest = false;

			if (GetCursorPos(&ucp) && GetCursorPos(&scp))	//ucp comparable, scp actual
			{
				ScreenToClient(hWnd, &scp);

				if (scp.x != 0 && scp.y != 0 && Distance(scp.x, scp.y, greysnake.GetCoord(0), greysnake.GetCoord(1)) < 192)
				{
					bDest = true;
					greysnake.MoveToward(hdc, hWnd, scp);
				}
			}

			if (!bDest)
			greysnake.Idle(hdc, hWnd);
		}
		else
		{
			//WHY IS THERE NO SNAKE?
			LogTypeData<int>(0, 0, "ERROR: NO 355");
		}

		if (logtime > 10)
		{
			logtime = 0;
			LogTypeData<long>(ucp.x, ucp.y, "POINT");
			LogTypeData<int>(greysnake.GetCoord(0), greysnake.GetCoord(1), "COORD");
		}

		//release hdc
		ReleaseDC(hWnd, hdc);
		break;

	case WM_DESTROY:
		//det timer and program
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
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

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;

	default:
		//all other messages
		return(DefWindowProc(hWnd, message, wParam, lParam));
	}
	return 0;
}
