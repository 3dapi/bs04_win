

#pragma comment(linker, "/subsystem:console")

#include <Windows.h>
#include <stdio.h>


#define MY_TIMER		(WM_USER +400)
#define TIME_INTERVAL	300


HINSTANCE	g_hInst;
HWND		g_hWnd;
char		g_sCls[120]="Timer";

DWORD	g_start = GetTickCount();


void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR nID, DWORD dwTime)
{
	// uMsg is WM_TIMER 0x0113
	// dwTime is same to GetTickcount() value;
	// DWORD	ddTime = GetTickCount();

	static	DWORD dBgn = dwTime;

	int v1 = int(dwTime-g_start) - TIME_INTERVAL;
	int v2 = ((dwTime-dBgn)/TIME_INTERVAL) * TIME_INTERVAL;
	int v3 = v1 - v2;

	printf("--- %4d  %4d  %4d\n", v1, v2, v3);
}


UINT MySetTimer(HWND hWnd, UINT nID, UINT Interval, TIMERPROC lpTimerFunc)
{
	return ::SetTimer(hWnd, nID, Interval, lpTimerFunc);
}


UINT MyKillTimer(HWND hWnd, UINT nID)
{
	return ::KillTimer(hWnd, nID);
}















LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
int main(int argc, char** argv)
{
	g_hInst= GetModuleHandle(NULL);

	WNDCLASS wc=
	{
		CS_HREDRAW | CS_VREDRAW
			,	(WNDPROC)WndProc
			, 0
			, 0
			, g_hInst
			, LoadIcon(NULL,IDI_APPLICATION)
			, LoadCursor(NULL,IDC_ARROW)
			, (HBRUSH)GetStockObject(WHITE_BRUSH)
			, NULL
			, g_sCls
	};

	if( 0 == RegisterClass(&wc))
		return -1;


	g_hWnd=CreateWindow(g_sCls
					, g_sCls
					, WS_OVERLAPPEDWINDOW
					, 10, 10, 100, 100
					, NULL, NULL
					, g_hInst
					, NULL);


	if(NULL == g_hWnd)
		return -1;

	ShowWindow(g_hWnd, SW_SHOW);


	g_start = GetTickCount();
	MySetTimer(NULL, MY_TIMER, 1 * TIME_INTERVAL, TimerProc);


	MSG msg;
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return(DefWindowProc(hWnd,uMsg,wParam,lParam));
}