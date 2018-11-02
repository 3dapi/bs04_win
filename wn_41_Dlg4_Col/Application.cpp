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
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_F_OPEN:
				{
					FileOpen();
					break;
				}

				case IDM_F_COLOR:
					SetColor();
					break;

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




void CApplication::FileOpen()
{
	OPENFILENAME	OFN;
	TCHAR			sDir[MAX_PATH]="\0";
	char			str[300];
	char			lpstrFile[MAX_PATH]="";
	char			szFileTitle[MAX_PATH];
	char			InitDir[MAX_PATH];


	int i=1;


	GetCurrentDirectory(MAX_PATH, sDir);

	GetWindowsDirectory(InitDir,MAX_PATH);

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner	=m_hWnd;
	OFN.lpstrFilter	="Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0";
	OFN.lpstrFile	=lpstrFile;
	OFN.nMaxFile	=MAX_PATH;
	OFN.lpstrTitle	="파일을 선택해 주십시요";

	OFN.nMaxFile=10000;
	OFN.Flags=OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	OFN.lpstrFileTitle=szFileTitle;
	OFN.nMaxFileTitle=MAX_PATH;
	OFN.lpstrDefExt="abc";
	OFN.lpstrInitialDir=InitDir;

	if (GetOpenFileName(&OFN)!=0)
	{
		char *p;
		char Name[MAX_PATH];
		char szTmp[MAX_PATH];

		p=lpstrFile;
		strcpy(Name, p);

		if (*(p+strlen(Name)+1)==0)
			wsprintf(str,"%s 파일 하나만 선택했습니다",Name);

		else
		{
			wsprintf(str,"%s 디렉토리에 있는 다음 파일들이 선택되었습니다"
				"\r\n\r\n", Name);

			while(1)
			{
				p=p+strlen(Name)+1;
				if (*p==0)
					break;
				strcpy(Name,p);
				wsprintf(szTmp,"%d번째 파일 = %s \r\n",i++,Name);
				strcat(str,szTmp);
			}
		}

		MessageBox(m_hWnd,str,"선택한 파일", 0);
	}

	else if (CommDlgExtendedError()==FNERR_BUFFERTOOSMALL)
			MessageBox(m_hWnd,"버퍼 크기가 너무 작습니다","에러", 0);


	SetCurrentDirectory(sDir);
}



void CApplication::SetColor()
{
	static COLORREF Color=RGB(0,0,255);
	static COLORREF crTemp[16];
	CHOOSECOLOR COL;
	HBRUSH MyBrush, OldBrush;


	memset(&COL, 0, sizeof(CHOOSECOLOR));
	COL.lStructSize = sizeof(CHOOSECOLOR);
	COL.hwndOwner=m_hWnd;
	COL.lpCustColors=crTemp;

	if( TRUE != ChooseColor(&COL))
	{
		return;
	}

	HDC	hdc= GetDC(m_hWnd);
	MyBrush=CreateSolidBrush(COL.rgbResult);
	OldBrush=(HBRUSH)SelectObject(hdc,MyBrush);
	Rectangle(hdc,10,10,600,400);
	SelectObject(hdc,OldBrush);
	DeleteObject(MyBrush);

	ReleaseDC(m_hWnd, hdc);
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif