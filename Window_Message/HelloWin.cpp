/*
 ----------------------
| 前缀  | 类别
| CS	| 窗口类别样式
| CW	| 建立窗口
| DT	| 绘制文字
| IDI	| 图示ID
| IDC	| 游标ID
| MB	| 消息框
| SND	| 声音
| WM	| 窗口消息
| WS	| 窗口样式
 ----------------------
*/

#include <windows.h>
#include "SysMetrics.h"

#pragma comment(lib, "winmm.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uint, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	static TCHAR szWndClassName[] = TEXT("HelloWin");

	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = szWndClassName;

	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires windows NT!"), szWndClassName, MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindow(szWndClassName,
							 TEXT("The Hello Program"),
							 WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	//RECT rect;

	static int cxChar, cxCaps, cyChar;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;

	static int cyClient, nVscrollPos;

	switch(message)
	{
	case WM_CREATE:
		//PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hWnd, hdc);

		SetScrollRange(hWnd, SB_VERT, 0, NUMLINES - 1, FALSE);
		SetScrollPos(hWnd, SB_VERT, nVscrollPos, FALSE);

		return 0;

	case WM_SIZE:
		cyClient = HIWORD(lParam);

		return 0;

	case WM_VSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:
			nVscrollPos -= 1;
			break;
		case SB_LINEDOWN:
			nVscrollPos += 1;
			break;
		case SB_PAGEUP:
			nVscrollPos -= cyClient / cyChar;
			break;
		case SB_PAGEDOWN:
			nVscrollPos += cyClient / cyChar;
			break;
		case SB_THUMBPOSITION:
			nVscrollPos = HIWORD(wParam);
			break;
		default:
			break;
		}
		nVscrollPos = max(0, min(nVscrollPos, NUMLINES - 1));
		if(nVscrollPos != GetScrollPos(hWnd, SB_VERT))
		{
			SetScrollPos(hWnd, SB_VERT, nVscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		/*GetClientRect(hWnd, &rect);
		DrawText(hdc, TEXT("Hello Windows"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);*/
		for(int  i = 0; i < NUMLINES; ++i)
		{
			int y = cyChar * (i - nVscrollPos);
			TextOut(hdc, 0, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
			TextOut(hdc, 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			TextOut(hdc, 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].Index)));
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}