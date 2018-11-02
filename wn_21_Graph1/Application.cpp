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
			OnPaint();

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



void CApplication::OnPaint()
{
	HDC hdc;
	PAINTSTRUCT ps;


	HBRUSH MyBrush,OldBrush;
	HPEN MyPen, OldPen;


	// Brush 생성
	LOGBRUSH lBrush =
	{
		BS_SOLID,
		RGB(0,0,255),
		NULL,
	};

//	MyBrush	= CreateSolidBrush(RGB(0,255,0));
	MyBrush	= CreateBrushIndirect(&lBrush);

	LOGPEN lPen =
	{
		PS_DOT,
		{10, 0,},
		RGB(255,0, 255),
	};

//	MyPen	= CreatePen(PS_SOLID, 10, RGB(255,0,255));
	MyPen	= CreatePenIndirect(&lPen);


	hdc=BeginPaint(m_hWnd,&ps);


	// DC의 Brush와 Pen을 새로운 객체로 설정하고 이전의
	// Brush와 Pen 객체를 저장
	OldBrush=(HBRUSH)SelectObject(hdc,MyBrush);
	OldPen	=(HPEN)SelectObject(hdc, MyPen);

	// 도형을 그린다.
	MoveToEx(hdc,350,250,NULL);	LineTo(hdc,280,300);
	Rectangle(hdc,400,100,450,180);
	Ellipse(hdc,400,300,550,380);




	POINT pt1[]=
	{
		{200, 10},
		{300, 90},
		{250,200},
		{150,200},
		{100, 90},
		{200, 10}
	};

	POINT pt2[]=
	{
		{ 30, 40},
		{240,150},
		{ 40,190},
		{140, 20},
		{180,210}
	};

	Polyline(hdc, pt1, 6);
	SetPolyFillMode(hdc, ALTERNATE);
	Polygon(hdc, pt2, 5);


	// 이전의 Brush, Pen 객체를 돌려줌
	SelectObject(hdc,OldBrush);
	SelectObject(hdc,OldPen);

	EndPaint(m_hWnd,&ps);


	// 생성한 브러쉬, 펜 객체를 소멸시킴
	DeleteObject(MyBrush);
	DeleteObject(MyPen);
}