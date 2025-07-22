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


	m_hBtn		= NULL;
	m_hTip		= NULL;
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




	// Tool Tip만들기
	InitCommonControls();
	TOOLINFO ti;

	m_hBtn=CreateWindow("button","버튼",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				50,50,150,100,m_hWnd,(HMENU)0,m_hInst,NULL);


	m_hTip=CreateWindow(TOOLTIPS_CLASS,NULL,0,
				CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
				m_hWnd,NULL,m_hInst,NULL);

	ti.cbSize=sizeof(TOOLINFO);
	ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
	ti.hwnd=m_hWnd;
	ti.uId=(WPARAM)m_hBtn;
	ti.lpszText="연습 버튼 입니다";

	SendMessage(m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);


	m_rcTst.left=200;
	m_rcTst.top =200;
	m_rcTst.right =300;
	m_rcTst.bottom=260;

	ti.uFlags=TTF_SUBCLASS;
	ti.hwnd=m_hWnd;
	ti.uId=0;
	ti.lpszText="사각 영역 입니다";
	ti.rect=m_rcTst;

	SendMessage(m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);


	return S_OK;
}


void CApplication::Cleanup()
{
	SAFE_DESTROY_WINDOW(m_hBtn);
	SAFE_DESTROY_WINDOW(m_hTip);

	if(m_hInst)
		UnregisterClass( m_sCls, m_hInst);
}


INT CApplication::Run()
{
	MSG msg;
	memset( &msg, 0, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
//		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		if( GetMessage(&msg, NULL, 0U, 0U) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
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
			PAINTSTRUCT ps;
			HDC hdc;
			hdc=BeginPaint(hWnd, &ps);
			Rectangle(hdc,m_rcTst.left, m_rcTst.top, m_rcTst.right, m_rcTst.bottom);
			EndPaint(hWnd, &ps);
			break;
		}


		case WM_LBUTTONDOWN:
			SendMessage(m_hTip,TTM_ACTIVATE,(WPARAM)TRUE,0);
			return 0;

		case WM_RBUTTONDOWN:
			SendMessage(m_hTip,TTM_ACTIVATE,(WPARAM)FALSE,0);
			return 0;

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