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
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
		, MAKEINTRESOURCE(IDR_MENU1)
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
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_F_EXIT:
					DestroyWindow(hWnd);
					break;
			}

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




void CApplication::Render()
{
	HDC hdc,hMemDC;

	hdc=GetDC(m_hWnd);

	BYTE Bits[]=
	{
			0xc3,0xff,0xbd,0xff
		,	0x66,0xff,0x5a,0xff
		,	0x5a,0xff,0x66,0xff
		,	0xbd,0xff,0xc3,0xff
	};

	HBITMAP hBitmap,hOldBitmap;
	BITMAP bit;


	hMemDC=CreateCompatibleDC(hdc);

	// DDB 생성
	// hBitmap=CreateBitmap(8,8,1,1,Bits);

	// 구조체로 DDB 생성
	bit.bmType=0;
	bit.bmWidth=8;
	bit.bmHeight=8;
	bit.bmWidthBytes=2;
	bit.bmBitsPixel=1;
	bit.bmPlanes=1;
	bit.bmBits=Bits;
	// hBitmap=CreateBitmapIndirect(&bit);

	hBitmap=CreateBitmap(
		8				// Specifies the bitmap width, in pixels.
		,8				// Specifies the bitmap height, in pixels.
		,1				// Specifies the number of color planes used by the device. The value of this parameter must be 1.
		,1				// Specifies the number of bits required to identify the color of a single pixel.
		,NULL
	);

	SetBitmapBits(hBitmap,16,Bits);

	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
	BitBlt(hdc,2,2,8,8,hMemDC,0,0,SRCCOPY);
	SelectObject(hMemDC,hOldBitmap);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	ReleaseDC(m_hWnd, hdc);
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif