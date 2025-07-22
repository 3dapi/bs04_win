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


#define ID_EDIT		(WM_USER+1)
#define ID_LIST		(WM_USER+2)


	m_hWndEdit=CreateWindow("edit",NULL
		, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
		, 10,10,200,25
		, m_hWnd
		, (HMENU)ID_EDIT
		,m_hInst, NULL);

	m_hWndList=CreateWindow("listbox",NULL
		, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY
		, 300,10,100,200
		, m_hWnd
		, (HMENU)ID_LIST
		, m_hInst, NULL);

	char Items[][15]={"Item1","Item2","Item3","Item4","Itme5"};
	for (int i=0;i<5;i++)
		SendMessage(m_hWndList,LB_ADDSTRING,0,(LPARAM)Items[i]);

	HWND hStatic = CreateWindow("static", "Hello world", WS_CHILD | WS_VISIBLE
		, 500,500,200,25
		, m_hWnd
		, (HMENU)-1
		, m_hInst,NULL);

	SetWindowText(hStatic, "Static is very Simple!!!");

	return S_OK;
}


void CApplication::Cleanup()
{
	if(m_hWndEdit)
	{
		DestroyWindow(m_hWndEdit);
		m_hWndEdit = NULL;
	}

	if(m_hWndList)
	{
		DestroyWindow(m_hWndList);
		m_hWndList = NULL;
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
	static char str[128]="왼쪽 클릭:에디트 이동, 오른쪽 클릭:보임/숨김";
	static int nTop=10;
	static BOOL bShow=TRUE;
	switch( msg )
	{
		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;

			hdc=BeginPaint(hWnd,&ps);
			TextOut(hdc,100,300,str,strlen(str));
			EndPaint(hWnd,&ps);
			break;
		}

		case WM_LBUTTONDOWN:
			nTop+=10;
			MoveWindow(m_hWndEdit,10,nTop,200,25,TRUE);
			break;

		case WM_RBUTTONDOWN:
			if (bShow)
			{
				bShow=FALSE;
				ShowWindow(m_hWndEdit,SW_HIDE);
			}
			else
			{
				bShow=TRUE;
				ShowWindow(m_hWndEdit,SW_SHOW);
			}

			break;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_EDIT:
				{


					switch (HIWORD(wParam))
					{
						case EN_CHANGE:
							GetWindowText(m_hWndEdit,str,128);
							SetWindowText(hWnd,str);
							break;
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