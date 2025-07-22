// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////


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


#define ID_BUTTON1	(WM_USER+1)
#define ID_BUTTON2	(WM_USER+2)

	m_hWndBtn1 = CreateWindow("button","Click Me"
		, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 20,20,100,25
		, m_hWnd
		, (HMENU)(ID_BUTTON1)
		, m_hInst, NULL);

	m_hWndBtn2 = CreateWindow("button","Me Two"
		, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 20,50,100,25
		, m_hWnd
		, (HMENU)(ID_BUTTON2)
		, m_hInst, NULL);


#define ID_CHECK1	(WM_USER+11)
#define ID_CHECK2	(WM_USER+12)

	m_hWndCheck1 = CreateWindow("button","Check 1"
			, WS_CHILD | WS_VISIBLE | BS_CHECKBOX
			, 160,20,160,25
			, m_hWnd
			, (HMENU)(ID_CHECK1)
			, m_hInst, NULL);

	m_hWndCheck2 = CreateWindow("button","Check 2"
			, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON
			, 160,50,160,25
			, m_hWnd
			, (HMENU)(ID_CHECK2)
			, m_hInst, NULL);

#define ID_COMBO	(WM_USER+22)

	m_hWndCombo	= CreateWindow("combobox", NULL
			, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN
			, 20,100,100,200
			, m_hWnd
			, (HMENU)ID_COMBO
			, m_hInst, NULL);


	char Items[][15]={"Item1","Item2","Item3","Item4","Itme5"};

	for (int i=0;i<5;i++)
		SendMessage(m_hWndCombo,CB_ADDSTRING,0,(LPARAM)Items[i]);


	return S_OK;
}


void CApplication::Cleanup()
{
	if(m_hWndBtn1)
	{
		DestroyWindow(m_hWndBtn1);
		m_hWndBtn1 = NULL;
	}

	if(m_hWndBtn2)
	{
		DestroyWindow(m_hWndBtn2);
		m_hWndBtn2 = NULL;
	}

	if(m_hWndCheck1)
	{
		DestroyWindow(m_hWndCheck1);
		m_hWndCheck1 = NULL;
	}

	if(m_hWndCheck2)
	{
		DestroyWindow(m_hWndCheck2);
		m_hWndCheck2 = NULL;
	}

	if(m_hWndCombo)
	{
		DestroyWindow(m_hWndCombo);
		m_hWndCombo = NULL;
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
			HDC hdc;
			PAINTSTRUCT ps;

			hdc=BeginPaint(hWnd,&ps);

			EndPaint(hWnd,&ps);
			return 0;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_BUTTON1:
					MessageBox(0,"1 Button Clicked","Button",0);
					break;

				case ID_BUTTON2:
					MessageBox(0,"2 Button Clicked","Button",0);
					break;

				case ID_CHECK1:
					MessageBox(0,"1 Checked","Check",0);
					SendMessage(m_hWndCheck1,BM_SETCHECK,BST_CHECKED,0);
					SendMessage(m_hWndCheck2,BM_SETCHECK,BST_UNCHECKED,0);
					break;

				case ID_CHECK2:
					MessageBox(0,"2 Checked","Check",0);
					SendMessage(m_hWndCheck1,BM_SETCHECK,BST_UNCHECKED,0);
					SendMessage(m_hWndCheck2,BM_SETCHECK,BST_CHECKED,0);
					break;

				case ID_COMBO:
				{
					char str[128];

					switch (HIWORD(wParam))
					{
						case CBN_SELCHANGE:
						{
							int iSel=SendMessage(m_hWndCombo, CB_GETCURSEL,0,0);
							SendMessage(m_hWndCombo, CB_GETLBTEXT, iSel, (LPARAM)str);
							SetWindowText(hWnd, str);
							break;
						}

						case CBN_EDITCHANGE:
						{
							GetWindowText(m_hWndCombo, str, 128);
							SetWindowText(hWnd,str);
							break;
						}
					}

					break;
				}

			}// switch(LOWORD(wParam))

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