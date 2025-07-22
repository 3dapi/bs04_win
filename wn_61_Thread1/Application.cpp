// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;

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

	m_bShowCursor= true;

	m_dThread	= 0;
	m_hThread	= 0;
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
				case ID_THREAD_CREATE:
				{
					MessageBox(hWnd, "Create Thread", "Msg",0);

//					m_hThread = CreateThread(NULL, 0, ThreadFunc, (void*)this, 0, &m_dThread);
					m_hThread = CreateThread(NULL, 0, ThreadFunc, (void*)this, CREATE_SUSPENDED, &m_dThread);
					ResumeThread(m_hThread);

					//쓰레드 우선순위설정
					SetThreadPriority(m_hThread, THREAD_PRIORITY_NORMAL);
					break;
				}

				case ID_THREAD_SUSPEND:
				{
					MessageBox(hWnd, "Suspend Thread", "Msg",0);

					if(m_hThread)
						SuspendThread(m_hThread);
					break;
				}

				case ID_THREAD_RESUME:
				{
					MessageBox(hWnd, "Resume Thread", "Msg",0);

					if(m_hThread)
						ResumeThread(m_hThread);
					break;
				}

				case ID_THREAD_DESTROY:
				{
					if(!m_hThread)
						break;

					MessageBox(hWnd, "Terminate Thread", "Msg",0);

					SuspendThread(m_hThread);
//					DWORD dExitCode;
//					GetExitCodeThread(m_hThread, &dExitCode);
//					ExitThread(dExitCode);
					CloseHandle(m_hThread);

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







// 스레드 함수에서는 색상을 계속 변경해 가며 사각형을 그린다.
DWORD WINAPI ThreadFunc(void* pParam)
{
	CApplication*	pApp = (CApplication*)pParam;
	HWND	hWnd;
	HDC		hdc;
	BYTE	Blue;
	HBRUSH	hBrush, hOldBrush;

	hWnd  = pApp->m_hWnd;

	hdc=GetDC(hWnd);

	while(1)
	{
		Blue++;
		Sleep(1);
		hBrush = CreateSolidBrush(RGB(0,0,Blue));
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc,10,20,400,400);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
	}

	ReleaseDC(hWnd, hdc);
	return 0;
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif