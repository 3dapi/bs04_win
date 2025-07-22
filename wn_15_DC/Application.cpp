// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;


CApplication::CApplication()
{
	g_pD3DApp	= this;

	strcpy(m_sCls, "McApi Tutorial");

	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	m_dScnX		= 1024;
	m_dScnY		= 768;

	m_bShowCursor= true;
}


LRESULT WINAPI CApplication::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc(hWnd, msg, wParam, lParam);
}

INT CApplication::Create( HINSTANCE hInst)
{
	m_hInst	= hInst;

	WNDCLASS wc =								// Register the window class
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, m_hInst
		, NULL
		, LoadCursor(NULL,IDC_ARROW)
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
		, NULL
		, m_sCls
	};

	RegisterClass( &wc );

	RECT rc;									//Create the application's window

	SetRect( &rc, 0, 0, m_dScnX, m_dScnY);
	AdjustWindowRect( &rc, m_dWinStyle, false );

	int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
	int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);

	m_hWnd = CreateWindow( m_sCls
		, m_sCls
		, m_dWinStyle
		,	(iScnSysW - (rc.right-rc.left))/2
		,	(iScnSysH - (rc.bottom-rc.top))/2
		,	(rc.right-rc.left)
		,	(rc.bottom-rc.top)
		, NULL
		, NULL
		, m_hInst
		, NULL );

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCursor);

	return S_OK;
}


void CApplication::Cleanup()
{
	if(m_hInst)
		UnregisterClass( m_sCls, m_hInst);
}


INT CApplication::Run()
{
	MSG msg;
	memset( &msg, 0, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
		}
	}

	UnregisterClass( m_sCls, m_hInst);

	return 1;
}





LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc=BeginPaint(hWnd,&ps);

			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc,100,100,"Beautiful Korea",15);
			SetTextAlign(hdc, TA_CENTER);
			TextOut(hdc,200,60,"Beautiful Korea",15);
			TextOut(hdc,200,80,"is",2);
			TextOut(hdc,200,100,"My Lovely Home Country",22);

//			SetTextAlign(hdc, TA_UPDATECP);
			SetTextColor(hdc, RGB(255, 0, 0) );
			TextOut(hdc,200,60,"One ASDASDFASDFASD ASDFASDFAS", strlen("One ASDASDFASDFASD ASDFASDFAS"));
			SetTextColor(hdc, RGB(255, 0, 255) );
			TextOut(hdc,200,80,"Two ADFASDFASDF asdfasdasdfaASDFASD", strlen("Two ADFASDFASDF asdfasdasdfaASDFASD"));
			SetTextColor(hdc, RGB(255, 255, 0) );
			TextOut(hdc,200,100,"Three",5);

			EndPaint(hWnd,&ps);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			OnLButtonDown();
			break;
		}

		case WM_KEYDOWN:
		{

			switch(wParam)
			{
				case VK_ESCAPE:
				{
					SendMessage(hWnd, WM_DESTROY, 0,0);
					break;
				}
			}

			return 0;

		}

		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}


void	CApplication::OnLButtonDown()
{
	HDC hdc = NULL;
	hdc=GetDC(m_hWnd);

	if(hdc)
	{
		TextOut(hdc,100,100,"Beautiful Korea",15);
		ReleaseDC(m_hWnd, hdc);
	}
}