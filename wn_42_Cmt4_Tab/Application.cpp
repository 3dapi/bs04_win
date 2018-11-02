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
		, (HBRUSH)(COLOR_BTNFACE + 0)
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
		, 300
		, 10
		, (rc.right-rc.left)
		, (rc.bottom-rc.top)
		, NULL
		, NULL
		, m_hInst
		, NULL );



	InitCommonControls();


	m_IL=ImageList_LoadImage(m_hInst, MAKEINTRESOURCE(IDB_WORK), 16, 1, RGB(255,255,255),IMAGE_BITMAP,0);

	m_hwWrk = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_WORK), m_hWnd, (DLGPROC)WndWrk);
	ShowWindow(m_hwWrk, SW_SHOW);

	SetFocus(m_hWnd);

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCusor);

	return S_OK;
}


void CApplication::Cleanup()
{
	SAFE_DESTROY_WINDOW(m_hwWrk);

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

			break;
		}

		case WM_NOTIFY:
		{
			break;
		}


		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;

			hdc=BeginPaint(hWnd, &ps);

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



INT_PTR CALLBACK WndWrk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rt1;
	RECT rt2;

	INT iWidth;
	INT iHeight;
	INT iX;
	INT iY;

	TCITEM tie;
	RECT trt={0,0,0,0};

	WPARAM	wparHi;
	WPARAM	wparLo;

	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);

	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			GetWindowRect(GHWND, &rt1);
			GetWindowRect(hWnd, &rt2);

			iWidth = rt2.right - rt2.left;
			iHeight=  rt2.bottom- rt2.top;

			iX = rt1.left - iWidth;
			iY = rt1.top;// + iHeight;

			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);
			GetWindowRect(hWnd, &rt2);

			GMAIN->m_hwWrkTb = GetDlgItem(hWnd, IDC_WORKTAB);

			TabCtrl_SetImageList(GMAIN->m_hwWrkTb, GMAIN->m_IL);

			tie.mask=TCIF_TEXT | TCIF_IMAGE;
			tie.pszText="Work 1";
			tie.iImage=0;
			TabCtrl_InsertItem(GMAIN->m_hwWrkTb,0,&tie);
			tie.pszText="Work 2";
			tie.iImage=1;
			TabCtrl_InsertItem(GMAIN->m_hwWrkTb,1,&tie);

			// 자식 윈도우 붙이기..
			GMAIN->m_hwWrk1 = CreateDialog(GHINST,MAKEINTRESOURCE(IDD_WORK1), hWnd, (DLGPROC)WndWrk1);

			ShowWindow(GMAIN->m_hwWrk1, SW_SHOW);
			GetWindowRect(GMAIN->m_hwWrkTb, &rt1);
			iX = 5;
			iY = 5;

			iWidth = rt1.right - rt1.left - iX * 2;
			iHeight = rt1.bottom - rt1.top - 40 - iY * 2;
			MoveWindow(GMAIN->m_hwWrk1, iX, iY, iWidth, iHeight, TRUE);

			return TRUE;
		}

		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE:
				{
					GMAIN->m_nTab=TabCtrl_GetCurSel(GMAIN->m_hwWrkTb);

					GetWindowRect(GMAIN->m_hwWrkTb, &rt1);
					iX = 5;
					iY = 5;

					iWidth = rt1.right - rt1.left - iX * 2;
					iHeight = rt1.bottom - rt1.top - 40 - iY * 2;

					if (GMAIN->m_hwWrk)
					{
						ShowWindow(GMAIN->m_hwWrk1, SW_HIDE);
					}

					if (GMAIN->m_nTab==0)
					{
						ShowWindow(GMAIN->m_hwWrk1, SW_SHOW);
						MoveWindow(GMAIN->m_hwWrk1, iX, iY, iWidth, iHeight, TRUE);
						InvalidateRect(GMAIN->m_hwWrk1, NULL, TRUE);
					}

					break;
				}
			}

			break;
		}

		case WM_CLOSE:
		{
			break;
		}
	}

	return(FALSE);
}



INT_PTR CALLBACK WndWrk1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static	WPARAM	wparHi;
	static	WPARAM	wparLo;
	static	INT		nMst=-1;
	static	INT		nSub=-1;
	static	HWND	hTree;

	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);

	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			hTree = GetDlgItem(hWnd, IDC_PTC_TREE);

			return TRUE;
		}

		case WM_NOTIFY:
		{
			TVITEMEX	TvResEx;
			TVITEMEX	TvResPr;
			HTREEITEM	hResPrn;
			TCHAR		sResCap[30] = "\0";
			TCHAR		sResPrn[30] = "\0";

			LPNMHDR hdr;
			LPNMTREEVIEW ntv;
			hdr=(LPNMHDR)lParam;
			ntv=(LPNMTREEVIEW)lParam;

			if(hdr->hwndFrom == hTree)
			{
				switch (hdr->code)
				{
					// 선택된 항목을 보여준다.
					case TVN_SELCHANGED:
					{
						TvResEx.mask=TVIF_PARAM | TVIF_TEXT;
						TvResEx.hItem=ntv->itemNew.hItem;
						TvResEx.pszText=sResCap;
						TvResEx.cchTextMax=sizeof(sResCap);
						TreeView_GetItem(hTree, &TvResEx);

						TCHAR sKind[256];
						TCHAR sName[256];

						hResPrn = TreeView_GetParent(hTree, TvResEx.hItem);

						if(hResPrn)
						{
							TvResPr.mask=TVIF_PARAM | TVIF_TEXT;
							TvResPr.hItem=hResPrn;
							TvResPr.pszText=sResPrn;
							TvResPr.cchTextMax=sizeof(sResPrn);
							TreeView_GetItem(hTree,&TvResPr);

							// 여기서 Work2의 데이터를 채워 넣는다

							TCHAR sIdMst[256];
							TCHAR sIdSub[256];

							sscanf(TvResEx.pszText, "%s %s", sIdMst, sIdSub);

							nMst = atoi(sIdMst);
							nSub = atoi(sIdSub);

						}

						else
						{
							sscanf(TvResEx.pszText, "%s %s", sKind, sName);

							SetDlgItemText(hWnd, IDC_PTC_TREE_NAME, sName);
							SetDlgItemText(hWnd, IDC_PTC_TREE_KIND, sKind);
						}

						break;
					}
				}
			}

			break;
		}	//case WM_NOTIFY:


		case WM_COMMAND:
		{
			switch(wparLo)
			{

				case IDC_PTC_TREE_SAVE:
				{
					MessageBox(hWnd,"Hello world", "Message", 0);
					break;
				}

			}

			break;
		}	// case WM_COMMAND:


		case WM_SYSCOMMAND:
		{
			break;
		}


		case WM_CLOSE:
		{
			break;
		}
	}

	return(FALSE);
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif