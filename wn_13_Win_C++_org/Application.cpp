// Application.cpp: implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


static CApplication*	g_pMainApp=NULL;


CApplication::CApplication()
{
	g_pMainApp = this;

	m_hInst	= NULL;
	m_hWnd	= NULL;

	m_iScnW	= 800;
	m_iScnH	= m_iScnW *3/4;

	strcpy(m_sCls, "McApi Toturial");
}

CApplication::~CApplication()
{

}


INT	CApplication::Create(HINSTANCE hInst)
{
	m_hInst = hInst;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInst;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= m_sCls;
	wcex.hIconSm		= 0;

	if( 0==RegisterClassEx(&wcex))
		return -1;

	m_hWnd = CreateWindow(m_sCls, m_sCls
		,	WS_OVERLAPPED| WS_CAPTION | WS_SYSMENU
		,	CW_USEDEFAULT, 0
		,	m_iScnW, m_iScnH
		,	NULL, NULL, m_hInst, NULL);

   if (!m_hWnd)
      return -1;


   ShowWindow(m_hWnd, SW_SHOW);
   UpdateWindow(m_hWnd);

	return 1;
}

INT CApplication::Run()
{
	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}




LRESULT CALLBACK CApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return g_pMainApp->MsgProc(hWnd, message, wParam, lParam);
}






LRESULT CApplication::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc;
			RECT rt;

			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);
			DrawText(hdc, "æ»≥Á«œººø‰", strlen("æ»≥Á«œººø‰"), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			return 0;

		case WM_LBUTTONDBLCLK:
			MessageBox(hWnd, "WM_LBUTTONDBLCLK", "Msg", 0);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

   }

   return DefWindowProc(hWnd, message, wParam, lParam);
}



