// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;


BOOL CALLBACK MlessDlgProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{

			return TRUE;
		}

	case WM_CLOSE:
		DestroyWindow(hDlg);
		return TRUE;
	}

	return FALSE;
}





CApplication::CApplication()
{
	g_pD3DApp	= this;
	
	strcpy(m_sCls, "McApi Tutorial");

	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	m_dScnX		= 400;
	m_dScnY		= 300;

	m_bShowCursor= true;

	m_hWndDlg	= 0;
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


	m_hWndDlg=CreateDialog(m_hInst
							,MAKEINTRESOURCE(IDD_DIALOG1)
							, m_hWnd
							, MlessDlgProc);

	ShowWindow(m_hWndDlg,SW_SHOW);
	InvalidateRect(m_hWndDlg, NULL, FALSE);


	return S_OK;
}


void CApplication::Cleanup()
{
	if(m_hWndDlg)
	{
		DestroyWindow(m_hWndDlg);
		m_hWndDlg = NULL;
	}

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
			// Render BMP in Dialog Button
			HWND hBtn = GetDlgItem(m_hWndDlg, IDC_MYBTN);
			HDC hdc = GetDC(hBtn);

			HDC MemDC=CreateCompatibleDC(hdc);
			HBITMAP MyBitmap;
			HBITMAP OldBitmap;

			MyBitmap=LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BTN2));
			OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);

			StretchBlt(hdc,5,5,240,240,MemDC,0,0,255,255,SRCCOPY);
			SelectObject(MemDC,OldBitmap);
			DeleteObject(MyBitmap);
			DeleteDC(MemDC);

//			Rectangle( hdc, 10,10, 100, 100);
			ReleaseDC(hBtn, hdc);
		}
	}
	
	UnregisterClass( m_sCls, m_hInst);
	
	return 1;
}





LRESULT WINAPI CApplication::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc(hWnd, msg, wParam, lParam);
}

LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;

			hdc=BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);

			break;	
		}

	case WM_LBUTTONDOWN:
		{

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



#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif



