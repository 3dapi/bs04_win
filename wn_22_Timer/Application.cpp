// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;


INT CALLBACK TimerProc(HWND, UINT, UINT, DWORD);



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
		, (iScnSysW - (rc.right-rc.left))/2
		, (iScnSysH - (rc.bottom-rc.top))/2
		, (rc.right-rc.left)
		, (rc.bottom-rc.top)
		, NULL
		, NULL
		, m_hInst
		, NULL );

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCursor);


//	SetTimer(m_hWnd, 1, 10, (TIMERPROC)TimerProc);
	SetTimer(m_hWnd, 1, 10, NULL);
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

		case WM_TIMER:
		{
			char	sTime[128];
			SYSTEMTIME st;
			GetLocalTime(&st);
			sprintf(sTime, "시간 %d:%d:%d"
						, st.wHour,st.wMinute,st.wSecond);
			SetWindowText(hWnd, sTime);
			return 0;
		}

		case WM_DESTROY:
		{
			KillTimer(hWnd, 1);
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}



INT CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	HDC hdc;
	int i;
	hdc=GetDC(hWnd);

	for (i=0;i<100;i++)
	{
		SetPixel(hdc,rand()%1024, rand()%768,
		RGB( INT(rand()%256), INT(rand()%256),INT(rand()%256) ));
	}
	ReleaseDC(hWnd, hdc);

	return 1;
}



#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif