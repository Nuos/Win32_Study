#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int CDECL MessageBoxPrintf(TCHAR* szCaption, TCHAR* szFmt, ...)
{
	TCHAR szBuffer[1024];
	va_list arg;
	va_start(arg, szFmt);
	_vsntprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), szFmt, arg);
	va_end(arg);

	return MessageBox(NULL, szBuffer, szCaption, MB_OK);
}

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	int nCxScreen(0), nCyScreen(0);
	nCxScreen = GetSystemMetrics(SM_CXSCREEN);
	nCyScreen = GetSystemMetrics(SM_CYSCREEN);

	MessageBoxPrintf(TEXT("ScreenSize"), TEXT("The Screen is %i pixels wide by %i pixels high!"), nCxScreen, nCyScreen);

	return 0;
}