// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


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
		, LoadIcon(NULL, IDI_APPLICATION)
		, LoadCursor(NULL, IDC_ARROW)
		, (HBRUSH)GetStockObject(WHITE_BRUSH)//(HBRUSH)(COLOR_BTNFACE+1)
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



	InitCommonControls();




#define ID_TOOLBAR		(WM_USER + 100)
#define ID_BTN_CIRCLE	(WM_USER + 101)
#define ID_BTN_RECT		(WM_USER + 102)
#define ID_BTN_LINE		(WM_USER + 103)


	TBBUTTON ToolBtn[3];

	m_Shape=ID_BTN_CIRCLE;

	for (int i=0;i<3;i++)
	{
		ToolBtn[i].iBitmap=i;
		ToolBtn[i].idCommand=ID_BTN_CIRCLE+i;
		ToolBtn[i].fsState=TBSTATE_ENABLED;
		ToolBtn[i].fsStyle=TBSTYLE_GROUP;
		ToolBtn[i].dwData=0;
		ToolBtn[i].iString=0;
	}

	m_hToolBar=CreateToolbarEx(m_hWnd
		, WS_CHILD | WS_VISIBLE | WS_BORDER  | TBSTYLE_TOOLTIPS		// 반드시 WS_CHILD가 붙어야..
		, ID_TOOLBAR
		, 3															// Bmp Number
		, m_hInst
		, IDB_BITMAP1
		, ToolBtn													// TBBUTTON Pointer
		, 3															// Button Number
		, 16,16														// Button Width Height
		, 30,30														// BMP Width Height
		, sizeof(TBBUTTON));

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCursor);

	return S_OK;
}


void CApplication::Cleanup()
{
	SAFE_DESTROY_WINDOW(m_hToolBar);

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
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_BTN_CIRCLE:
					m_Shape=LOWORD(wParam);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case ID_BTN_RECT:
					m_Shape=LOWORD(wParam);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case ID_BTN_LINE:
					m_Shape=LOWORD(wParam);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

			}

			break;
		}

		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case TTN_NEEDTEXT:
				{
					switch (wParam)
					{
						case ID_BTN_CIRCLE:
							((LPTOOLTIPTEXT)lParam)->lpszText="Circle";
							break;
						case ID_BTN_RECT:
							((LPTOOLTIPTEXT)lParam)->lpszText="Rectangle";
							break;
						case ID_BTN_LINE:
							((LPTOOLTIPTEXT)lParam)->lpszText="Line";
							break;
					}

					break;
				}
			}

			break;
		}


		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;

			hdc=BeginPaint(hWnd, &ps);

			switch (m_Shape)
			{
				case ID_BTN_CIRCLE:
					Ellipse(hdc,10,50,200,240);
					break;
				case ID_BTN_RECT:
					Rectangle(hdc,10,50,200,240);
					break;
				case ID_BTN_LINE:
					MoveToEx(hdc,10,50,NULL);
					LineTo(hdc,200,240);
					break;
			}

			EndPaint(hWnd, &ps);

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



#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif