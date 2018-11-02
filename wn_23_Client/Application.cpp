// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////


#pragma warning(disable: 4996)

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

	m_bShowCusor= true;
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
	::ShowCursor(m_bShowCusor);

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
	TCHAR	sMsg[128];

	static DWORD dBgn = timeGetTime();
	DWORD dEnd = timeGetTime();

	if(dEnd > (dBgn+300))
	{
		HDC	hDC;
		hDC = GetDC(m_hWnd);




		dBgn = dEnd;
		RECT	rt;
		::GetClientRect(m_hWnd, &rt);

		sprintf(sMsg, "Clent RECT: %4ld %4ld %4ld  %4ld"
			, rt.left
			, rt.top
			, rt.right
			, rt.bottom);

		SetWindowText(m_hWnd, sMsg);



//		::GetWindowRect(GetDesktopWindow(), &rt);
		::GetWindowRect(m_hWnd, &rt);

		sprintf(sMsg, "Window RECT: %4ld %4ld %4ld  %4ld(%4d, %4d) "
			, rt.left
			, rt.top
			, rt.right
			, rt.bottom

			, rt.right - rt.left
			, rt.bottom- rt.top
			);

		TextOut(hDC, 100, 100, sMsg, strlen(sMsg));



		POINT ScnPt ={400, 300};
		::ScreenToClient(m_hWnd, &ScnPt);


		sprintf(sMsg, "Screen to Client: %4d %4d "
			, ScnPt.x
			, ScnPt.y
			);

		TextOut(hDC, 100, 140, sMsg, strlen(sMsg));

		ReleaseDC(m_hWnd, hDC);
	}

	return 1;
}
