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
							 WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
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

	static int cxChar, cxCaps, cyChar;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;

	static int cxClient, cyClient, nVscrollPos, nMaxWidth;
	int nVertPos(0), nHorzPos(0), nPaintBeg(0), nPaintEnd(0);
	SCROLLINFO si;

	switch(message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hWnd, hdc);

		nMaxWidth = 40 * cxChar + 20 * cxCaps;

		return 0;

	case WM_SIZE:
		cyClient = HIWORD(lParam);
		cxClient = LOWORD(lParam);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = NUMLINES - 1;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + nMaxWidth / cxChar;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

		return 0;

	case WM_VSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);
		nVertPos = si.nPos;

		switch(LOWORD(wParam))
		{
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBPOSITION:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);
		if(si.nPos != nVertPos)
		{
			ScrollWindow(hWnd, 0, cyChar * (nVertPos - si.nPos), NULL, NULL);
			UpdateWindow(hWnd);
		}

		return 0;

	case WM_HSCROLL:
		si.cbSize = sizeof (si) ;
		si.fMask  = SIF_ALL ;
		GetScrollInfo (hWnd, SB_HORZ, &si) ;
		nHorzPos = si.nPos ;
		switch (LOWORD (wParam))
		{
		case   SB_LINELEFT:
			si.nPos -= 1 ;
			break ;
		case   SB_LINERIGHT:
			si.nPos += 1 ;
			break ;
		case   SB_PAGELEFT:
			si.nPos -= si.nPage ;
			break ;
		case   SB_PAGERIGHT:
			si.nPos += si.nPage ;
			break ;
		case   SB_THUMBPOSITION:
			si.nPos = si.nTrackPos ;
			break ;
		default :
			break ;
		}
		si.fMask = SIF_POS ;
		SetScrollInfo (hWnd, SB_HORZ, &si, TRUE) ;
		GetScrollInfo (hWnd, SB_HORZ, &si) ;
		if (si.nPos != nHorzPos)
		{
			ScrollWindow (hWnd, cxChar * (nHorzPos - si.nPos), 0,
				NULL, NULL) ;
		}

		return 0 ;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_HOME:
			SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0);
			break;
		case VK_END:
			SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);
			break;
		case VK_PRIOR:
			SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
			break;
		case VK_NEXT:
			SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);
			break;
		case VK_UP:
			SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
			break;
		case VK_DOWN:
			SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
			break;
		case VK_LEFT:
			SendMessage(hWnd, WM_HSCROLL, SB_PAGELEFT, 0);
			break;
		case VK_RIGHT:
			SendMessage(hWnd, WM_HSCROLL, SB_PAGERIGHT, 0);
			break;

		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd, SB_VERT, &si);
		nVertPos = si.nPos;
		GetScrollInfo(hWnd, SB_HORZ, &si);
		nHorzPos = si.nPos;
		nPaintBeg = max(0, nVertPos + ps.rcPaint.top / cyChar);
		nPaintEnd = min(NUMLINES - 1, nVertPos + ps.rcPaint.bottom / cyChar);

		for(int i = nPaintBeg; i <= nPaintEnd; ++i)
		{
			int x = cxChar * ( 1 - nHorzPos);
			int y = cyChar * (i - nVertPos);

			TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
			TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].Index)));
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