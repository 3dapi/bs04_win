// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#define _WIN32_WINNT 0x0400

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
			Render();
		}
	}

	UnregisterClass( m_sCls, m_hInst);

	return 1;
}





LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	HDC hdc;
	char str[256];
	static int x;
	static int y;
	static BOOL bnowDraw=FALSE;

	switch( msg )
	{
		case WM_PAINT:
		{
			break;
		}

		case WM_MOUSEWHEEL:
		{
			short v = short(HIWORD(wParam));
			sprintf(str, "Wheel Z value:%d", v);
			SetWindowText(hWnd, str);
			return 0;
		}


		case WM_LBUTTONDOWN:
		{
			x=LOWORD(lParam);
			y=HIWORD(lParam);

			sprintf(str, "L Button Down %d %d", x, y);
			SetWindowText(hWnd, str);

			x=LOWORD(lParam);
			y=HIWORD(lParam);
			bnowDraw=TRUE;

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			if (bnowDraw==TRUE)
			{
				hdc=GetDC(hWnd);
				MoveToEx(hdc,x,y,NULL);
				x=LOWORD(lParam);
				y=HIWORD(lParam);
				LineTo(hdc,x,y);
				ReleaseDC(hWnd,hdc);
			}

			return 0;
		}

		case WM_LBUTTONUP:
		{
			x=LOWORD(lParam);
			y=HIWORD(lParam);

			sprintf(str, "L Button Up %d %d", x, y);
			SetWindowText(hWnd, str);

			bnowDraw=FALSE;

			return 0;
		}

		case WM_LBUTTONDBLCLK:
		{
			x=LOWORD(lParam);
			y=HIWORD(lParam);

			sprintf(str, "L Button Double Click  %d %d", x, y);
			SetWindowText(hWnd, str);

			return 0;
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



INT CApplication::Render()
{

	return 1;
}





#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif