#include <windows.h>
#include <math.h>

#define NUM 1000
#define TWOPI (2 * 3.14159)

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	TCHAR* szClassName = TEXT("DrawLine");
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;

	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("register windows class error!"), szClassName, MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindow(szClassName, 
							TEXT("DrawLine"), 
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							NULL,
							NULL,
							hInstance,
							NULL);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while(GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void DrawSin(HDC hdc, int cxClient, int cyClient)
{
	POINT apt[NUM];
	MoveToEx(hdc, 0, cyClient / 2, NULL);
	LineTo(hdc, cxClient, cyClient / 2);
	for(int i = 0; i < NUM; ++i)
	{
		apt[i].x = i * cxClient / NUM;
		apt[i].y = (int) (cyClient / 2 * (1 - sin (TWOPI * i / NUM))) ;
	}
	Polyline(hdc, apt, sizeof(apt) / sizeof(apt[0]));
}

static POINT aptBizier[4];
void DrawBezier(HDC hdc)
{
	PolyBezier(hdc, aptBizier, sizeof(aptBizier) / sizeof(aptBizier[0]));
	MoveToEx(hdc, aptBizier[0].x, aptBizier[0].y, NULL);
	LineTo(hdc, aptBizier[1].x, aptBizier[1].y);
	MoveToEx(hdc, aptBizier[2].x, aptBizier[2].y, NULL);
	LineTo(hdc, aptBizier[3].x, aptBizier[3].y);
}

void DrawRect(HDC hdc, int cxClient, int cyClient)
{
	Rectangle(hdc, cxClient / 8, cyClient / 8, 7 * cxClient / 8, 7 * cyClient / 8);
	MoveToEx(hdc, 0, 0, NULL);
	LineTo(hdc, cxClient, cyClient);
	MoveToEx(hdc, 0, cyClient, NULL);
	LineTo(hdc, cxClient, 0);
	Ellipse(hdc, cxClient / 8, cyClient / 8, 7 * cxClient / 8, 7 * cyClient / 8);
	RoundRect(hdc, cxClient / 4, cyClient / 4, 3 * cxClient / 4, 3 * cyClient / 4, cxClient / 4, cyClient / 4);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;
	

	switch(message)
	{
	case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			aptBizier[0].x = cxClient / 4;
			aptBizier[0].y = cyClient / 2;
			aptBizier[1].x = cxClient / 2;
			aptBizier[1].y = cyClient / 4;
			aptBizier[2].x = cxClient / 2;
			aptBizier[2].y = 3 * cyClient / 4;
			aptBizier[3].x = 3 * cxClient / 4;
			aptBizier[3].y = cyClient / 2;
			return 0;
		}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		{
			if(wParam & MK_LBUTTON || wParam & MK_RBUTTON)
			{
				hdc = GetDC(hwnd);
				SelectObject(hdc, GetStockObject(WHITE_PEN));
				DrawBezier(hdc);

				if(wParam & MK_LBUTTON)
				{
					aptBizier[1].x = LOWORD(lParam);
					aptBizier[1].y = HIWORD(lParam);
				}

				if(wParam & MK_RBUTTON)
				{
					aptBizier[2].x = LOWORD(lParam);
					aptBizier[2].y = HIWORD(lParam);
				}

				SelectObject(hdc, GetStockObject(BLACK_PEN));
				DrawBezier(hdc);
				ReleaseDC(hwnd, hdc);
			}
		}
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			//DrawSin(hdc, cxClient, cyClient);

			//DrawRect(hdc, cxClient, cyClient);

			DrawBezier(hdc);

			EndPaint(hwnd, &ps);
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}