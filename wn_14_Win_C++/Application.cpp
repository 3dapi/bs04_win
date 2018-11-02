// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CApplication* CApplication::g_pApp = NULL;


CApplication* CApplication::GetAppInstance()
{
	return g_pApp;
}

CApplication::CApplication()
{
	g_pApp	= this;
	
	strcpy(m_sCls, "McApi Window");

	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	m_dScnX		= 800;
	m_dScnY		= 600;

	m_bShowCusor= true;
}


CApplication::~CApplication()
{

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
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, NULL
		, m_sCls
	};
	
	RegisterClass( &wc );
	
	RECT rc;									//Create the application's window
	
	SetRect( &rc, 0, 0, m_dScnX, m_dScnY);
	AdjustWindowRect( &rc, m_dWinStyle, FALSE);
		
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


LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
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


LRESULT WINAPI CApplication::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return CApplication::GetAppInstance()->MsgProc(hWnd, msg, wParam, lParam);
}