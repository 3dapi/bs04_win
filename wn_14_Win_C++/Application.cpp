// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"

static std::map<HWND, CApplication*>	g_win_obj;

CApplication* CApplication::GetInstance()
{
	static CApplication app;
	return &app;
}

CApplication::CApplication()
{
	m_sCls = std::string("McApi Window");

	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	m_dScnX		= 800;
	m_dScnY		= 600;

	m_bShowCursor= true;
}

static LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(WM_NCCREATE == uMsg)
	{
		CREATESTRUCT* info = (CREATESTRUCT*)lParam;
		if(info)
		{
			CApplication* obj = (CApplication*)info->lpCreateParams;
			CApplication* c_thzz = dynamic_cast<CApplication*>(obj);
			if(c_thzz)
				g_win_obj.emplace(hWnd, c_thzz);
		}
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	// find CApplication*
	auto it = g_win_obj.find(hWnd);
	if(it == g_win_obj.end())
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	CApplication* thzz = dynamic_cast<CApplication*>(it->second);
	if(!thzz)
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	return thzz->MsgProc(hWnd, uMsg, wParam, lParam);
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
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, NULL
		, m_sCls.c_str()
	};
	
	RegisterClass( &wc );
	
	RECT rc;									//Create the application's window
	
	SetRect( &rc, 0, 0, m_dScnX, m_dScnY);
	AdjustWindowRect( &rc, m_dWinStyle, FALSE);
		
	int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
	int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);
	
	auto thzz = CApplication::GetInstance();
	m_hWnd = CreateWindow( m_sCls.c_str()
		, m_sCls.c_str()
		, m_dWinStyle
		, (iScnSysW - (rc.right-rc.left))/2
		, (iScnSysH - (rc.bottom-rc.top))/2
		, (rc.right-rc.left)
		, (rc.bottom-rc.top)
		, NULL
		, NULL
		, m_hInst
		, thzz );
	
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCursor);
	
	return S_OK;
}


void CApplication::Cleanup()
{
	if(!m_hWnd)
		return;
	if(m_hInst)
	{
		UnregisterClass( m_sCls.c_str(), m_hInst);
	}
	// remove from map
	auto it = g_win_obj.find(m_hWnd);
	if(it != g_win_obj.end())
		g_win_obj.erase(it);
	m_hWnd = {};
}


LRESULT CApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* info = (MINMAXINFO*)lParam;
			break;
		}
		case WM_NCCREATE:
		{
			CREATESTRUCT* info = (CREATESTRUCT*)lParam;
			void* thzz = {};
			if(info)
			{
				thzz = info->lpCreateParams;
			}
			break;
		}
		case WM_NCCALCSIZE:
		{
			if(wParam)
			{
				NCCALCSIZE_PARAMS* info = (NCCALCSIZE_PARAMS*)lParam;
			}
			else
			{
				RECT* info = (RECT*)lParam;
			}
			break;
		}

		case WM_CREATE:
		{
			CREATESTRUCT* info = (CREATESTRUCT*)lParam;
			void* thzz = {};
			if(info)
			{
				thzz = info->lpCreateParams;
			}
			break;
		}
		case WM_SHOWWINDOW:
		{
			if(wParam)
			{
				// shown
			}
			else
			{
				// hidden
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
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}



INT CApplication::Run()
{
	MSG msg{};
	
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
	
	UnregisterClass( m_sCls.c_str(), m_hInst);
	
	return 1;
}
