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

	#define ID_R1			(WM_USER+1)
	#define ID_R2			(WM_USER+2)
	#define ID_R3			(WM_USER+3)
	#define ID_R4			(WM_USER+4)
	#define ID_R5			(WM_USER+5)
	#define ID_R6			(WM_USER+6)
	#define ID_SCR_RED		(WM_USER+7)
	#define ID_SCR_GREEN	(WM_USER+8)
	#define ID_SCR_BLUE		(WM_USER+9)


	m_nGraph=0;
	m_nColor=0;


	CreateWindow("button","Graph",WS_CHILD | WS_VISIBLE | BS_GROUPBOX
		,5,5,120,110
		,m_hWnd
		,(HMENU)0
		, m_hInst, NULL);

	CreateWindow("button","Color",WS_CHILD | WS_VISIBLE | BS_GROUPBOX
		,145,5,120,110
		, m_hWnd
		, (HMENU)1
		, m_hInst, NULL);

	m_hWndRdo[0]=CreateWindow("button","Rectangle"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP
		, 10,20,100,30
		, m_hWnd
		, (HMENU)ID_R1
		, m_hInst, NULL);

	m_hWndRdo[1]=CreateWindow("button","Ellipse"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 10,50,100,30
		, m_hWnd
		, (HMENU)ID_R2
		, m_hInst, NULL);

	m_hWndRdo[2]=CreateWindow("button","Line"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 10,80,100,30
		, m_hWnd
		, (HMENU)ID_R3
		, m_hInst, NULL);

	m_hWndRdo[3]=CreateWindow("button","Black"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP
		, 150,20,100,30
		, m_hWnd
		, (HMENU)ID_R4
		, m_hInst, NULL);

	m_hWndRdo[4]=CreateWindow("button","Red"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 150,50,100,30
		, m_hWnd
		, (HMENU)ID_R5
		, m_hInst, NULL);

	m_hWndRdo[5]=CreateWindow("button","Blue"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 150,80,100,30
		, m_hWnd, (HMENU)ID_R6
		, m_hInst, NULL);

	CheckRadioButton(m_hWnd, ID_R1, ID_R3, ID_R1);
	CheckRadioButton(m_hWnd, ID_R4, ID_R6, ID_R4);



	m_hWndScrol[0]=CreateWindow("scrollbar",NULL
		, WS_CHILD | WS_VISIBLE | SBS_HORZ
		, 400,10,200,20
		, m_hWnd
		, (HMENU)ID_SCR_RED
		, m_hInst, NULL);

	m_hWndScrol[1]=CreateWindow("scrollbar",NULL
		, WS_CHILD | WS_VISIBLE | SBS_HORZ
		, 400,40,200,20
		, m_hWnd
		, (HMENU)ID_SCR_GREEN
		, m_hInst, NULL);

	m_hWndScrol[2]=CreateWindow("scrollbar",NULL
		, WS_CHILD | WS_VISIBLE | SBS_VERT
		, 650,100,20,200
		, m_hWnd
		, (HMENU)ID_SCR_BLUE
		, m_hInst, NULL);

	SetScrollRange(	m_hWndScrol[0],SB_CTL,0,255,TRUE);
	SetScrollPos(	m_hWndScrol[0],SB_CTL,0,TRUE);
	SetScrollRange(	m_hWndScrol[1],SB_CTL,0,255,TRUE);
	SetScrollPos(	m_hWndScrol[1],SB_CTL,0,TRUE);
	SetScrollRange(	m_hWndScrol[2],SB_CTL,0,255,TRUE);
	SetScrollPos(	m_hWndScrol[2],SB_CTL,0,TRUE);

	m_Red	= 0;
	m_Green	= 0;
	m_Blue	= 0;


	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCusor);

	return S_OK;
}


void CApplication::Cleanup()
{
	int i=0;

	for(i=0; i<6; ++i)
	{
		DestroyWindow(m_hWndRdo[i]);
		m_hWndRdo[i] = NULL;
	}

	for(i=0; i<3; ++i)
	{
		DestroyWindow(m_hWndScrol[i]);
		m_hWndScrol[i] = NULL;
	}



	if(m_hInst)
		UnregisterClass( m_sCls, m_hInst);
}



LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush1, OldBrush1;
	HBRUSH MyBrush2, OldBrush2;
	int TempPos;

	switch( msg )
	{
		case WM_PAINT:
		{
			hdc=BeginPaint(hWnd,&ps);

			switch (m_nColor)
			{
			case 0:
				MyBrush1=CreateSolidBrush(RGB(0,0,0));
				break;
			case 1:
				MyBrush1=CreateSolidBrush(RGB(255,0,0));
				break;
			case 2:
				MyBrush1=CreateSolidBrush(RGB(0,0,255));
				break;
			}

			OldBrush1=(HBRUSH)SelectObject(hdc,MyBrush1);

			switch (m_nGraph)
			{
			case 0:
				Rectangle(hdc,10,200,200,300);
				break;
			case 1:
				Ellipse(hdc,10,200,200,300);
				break;
			case 2:
				MoveToEx(hdc,10,200,NULL);
				LineTo(hdc,200,300);
				break;
			}

			SelectObject(hdc,OldBrush1);
			DeleteObject(MyBrush1);


			MyBrush2=CreateSolidBrush(RGB(m_Red,m_Green,m_Blue));
			OldBrush2=(HBRUSH)SelectObject(hdc,MyBrush2);
			Rectangle(hdc,400,100,610,300);
			SelectObject(hdc,OldBrush2);
			DeleteObject(MyBrush2);
			EndPaint(hWnd,&ps);
			break;
		}

		case WM_COMMAND:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				switch (LOWORD(wParam))
				{
				case ID_R1:
					m_nGraph=0;
					break;
				case ID_R2:
					m_nGraph=1;
					break;
				case ID_R3:
					m_nGraph=2;
					break;
				case ID_R4:
					m_nColor=0;
					break;
				case ID_R5:
					m_nColor=1;
					break;
				case ID_R6:
					m_nColor=2;
					break;
				}

				InvalidateRect(hWnd, NULL, TRUE);
			}

			break;
		}


		case WM_VSCROLL:
		case WM_HSCROLL:
		{
			if ((HWND)lParam == m_hWndScrol[0])
				TempPos = m_Red;

			if ((HWND)lParam == m_hWndScrol[1])
				TempPos = m_Green;

			if ((HWND)lParam == m_hWndScrol[2])
				TempPos = m_Blue;

			switch (LOWORD(wParam))
			{
				case SB_LINELEFT:
					TempPos=max(0,TempPos-1);
					break;
				case SB_LINERIGHT:
					TempPos=min(255,TempPos+1);
					break;
				case SB_PAGELEFT:
					TempPos=max(0,TempPos-10);
					break;
				case SB_PAGERIGHT:
					TempPos=min(255,TempPos+10);
					break;
				case SB_THUMBTRACK:
					TempPos=HIWORD(wParam);
					break;
			}

			if ((HWND)lParam == m_hWndScrol[0])
				m_Red=TempPos;

			if ((HWND)lParam == m_hWndScrol[1])
				m_Green=TempPos;

			if ((HWND)lParam == m_hWndScrol[2])
				m_Blue=TempPos;

			SetScrollPos((HWND)lParam,SB_CTL,TempPos,TRUE);
			InvalidateRect(hWnd,NULL,FALSE);

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





#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif