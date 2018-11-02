// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"



static CApplication*	g_pD3DApp;

extern HANDLE g_hSmph;


DWORD WINAPI ThreadFunc(void* pParam);


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

	memset(m_dTh, 0, sizeof(m_dTh));
	memset(m_hTh, 0, sizeof(m_hTh));

	m_nValue	= 0;
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
	CloseHandle(g_hSmph);

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
				case ID_THREAD_CREATE:
				{
					MessageBox(hWnd, "Create Thread", "Msg",0);

					ReleaseSemaphore(g_hSmph, 1, NULL);							// Semaphore count 1증가

					for(int i=0; i<3; ++i)
					{
						m_hTh[i] = (HANDLE)_beginthreadex(NULL, 0
							, (unsigned (__stdcall *) (void *))ThreadFunc
							, (void*)this
							, 0, (unsigned*)&m_dTh[i]);
					}

					break;
				}
				case ID_THREAD_DESTROY:
				{
					for(int i=0; i<3; ++i)
					{
						if(m_hTh[i])
						{
							SuspendThread(m_hTh[i]);
							CloseHandle(m_hTh[i]);
						}
					}

					break;
				}


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
}







DWORD WINAPI ThreadFunc(void* pParam)
{
	CApplication*	pApp = (CApplication*)pParam;
	HWND	hWnd;
	HDC		hdc;

	hWnd  = pApp->m_hWnd;
	hdc=GetDC(hWnd);
	int nCnt =0;



	while(nCnt<100)
	{
		WaitForSingleObject(g_hSmph, INFINITE);									// Semaphore count 1감소
		Sleep(500);

		++nCnt;
		++pApp->m_nValue;
		char sBuf[128];

		sprintf(sBuf, "%d", pApp->m_nValue);
		SetWindowText(pApp->m_hWnd, sBuf);

		ReleaseSemaphore(g_hSmph, 1, NULL);										// Semaphore count 1증가
	}



	ReleaseDC(hWnd, hdc);


	return 0;
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif