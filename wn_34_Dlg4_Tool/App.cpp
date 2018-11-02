//
// File: D3DApp.cpp
// Desc:
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


static CApplication* g_pD3DApp = NULL;

CApplication::CApplication()
{
	g_pD3DApp           = this;

	m_hWnd              = NULL;
	m_hMenu             = NULL;
	m_strWindowTitle    = TEXT( "EftTool" );

	m_dwCreationWidth   = 1024;
	m_dwCreationHeight  = 768;

	m_dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE;
}





LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
}





HRESULT CApplication::Create( HINSTANCE hInstance )
{
	if( m_hWnd == NULL)
	{

		// Register the windows class
		WNDCLASS wndClass =
		{
			0,
				(WNDPROC)WndProc,
				0,
				0,
				hInstance,
				LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
				LoadCursor( NULL, IDC_ARROW ),
				(HBRUSH)GetStockObject(WHITE_BRUSH),
				NULL,
				m_strWindowTitle
		};

		RegisterClass( &wndClass );

		// Set the window's initial style
		m_dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
			WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;

		// Set the window's initial width
		RECT rc;
		SetRect( &rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight );
		AdjustWindowRect( &rc, m_dwWindowStyle, true );


		// Create the render window
		m_hWnd = CreateWindow(	m_strWindowTitle
			,	m_strWindowTitle
			,	m_dwWindowStyle
			,	300//(iScnSysW - (rc.right-rc.left))/2
			,	10//(iScnSysH - (rc.bottom-rc.top))/2
			,	(rc.right-rc.left)
			,	(rc.bottom-rc.top)
			,	0
			,	LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU) )
			,	hInstance
			,	0 );
	}

	m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	GetWindowRect( m_hWnd, &m_rcWindowBounds );
	GetClientRect( m_hWnd, &m_rcWindowClient );

	HRESULT hr;
	hr = Init();
	return hr;
}



LRESULT CApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_PAINT:
		break;

	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDM_EXIT:
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			return 0;
		}
		break;

	case WM_CLOSE:
	case WM_DESTROY:
		Destroy();
		HMENU hMenu;
		hMenu = GetMenu(hWnd);
		if( hMenu != NULL )
			DestroyMenu( hMenu );
		DestroyWindow( hWnd );
		PostQuitMessage(0);
		m_hWnd = NULL;
		return 0;
}

return DefWindowProc( hWnd, uMsg, wParam, lParam );
}



INT CApplication::Run()
{
	HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

	MSG  msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( m_hWnd )
	{
		if(WM_QUIT == msg.message)
			break;

		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
		{
			if(0==TranslateAccelerator( m_hWnd, hAccel, &msg) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			Render();
		}
	}

	if( hAccel != NULL )
		DestroyAcceleratorTable( hAccel );

	return (INT)msg.wParam;
}
