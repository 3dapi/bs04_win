// Implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#include "_StdAfx.h"
#include "GdiSprite.h"

static CApplication*	g_pD3DApp;

CApplication::CApplication()
{
	g_pD3DApp	= this;
	
	strcpy(m_sCls, "McApi Tutorial");

	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	m_dScnX		= 900;
	m_dScnY		= 450;

	m_bShowCursor= true;
	m_hClearBrush = CreateSolidBrush(RGB(0, 100, 150));
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

	m_spriteMario = new GdiSprite;
	m_spriteMario->init("res/mario_sprite.bmp");

	return S_OK;
}


void CApplication::Cleanup()
{
	ReleaseFrameBuffer();

	if(m_hInst)
		UnregisterClass( m_sCls, m_hInst);
}



INT CApplication::Run()
{
	m_timeCur = GetTickCount64();
	m_timeOld = m_timeCur;
	m_timeDelta = 0.0F;

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
			m_timeOld = m_timeCur;
			m_timeCur = GetTickCount64();
			m_timeDelta = (m_timeCur- m_timeOld) * 0.001F;
			m_timeTick += m_timeDelta;

			static int index = 0;

			if(0.1f <= m_timeTick)
			{
				m_timeTick -= 0.125f;
				++index;
				index %= (12*4);
			}

			int indexX = index %12;
			int indexY = index /12;
			int sprite_x = 60 * indexX;
			int sprite_y = 80 * indexY;
			int sprite_w = 60;
			int sprite_h = 80;
			float scale = 4.0F;

			char buf[1024]{};
			sprintf(buf, "%d %d\n", indexX, indexY);
			OutputDebugStringA(buf);

			ClearBackBuffer();
			m_spriteMario->Draw(m_backBuffer.dc, {sprite_x, sprite_y}, {sprite_w, sprite_h}, {200, 60}, scale);


			HDC hdc = GetDC(m_hWnd);
			PresentToFrontBuffer(hdc);
			ReleaseDC(m_hWnd, hdc);
		}
	}
	
	UnregisterClass( m_sCls, m_hInst);
	
	return msg.wParam;
}





LRESULT WINAPI CApplication::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc(hWnd, msg, wParam, lParam);
}

LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_SIZE:
		{
			RECT rc{};
			GetClientRect(hWnd, &rc);
			int width = rc.right  - rc.left;
			int height = rc.bottom - rc.top;
			if(width == 0 || height == 0)
				return 0;
			ReleaseFrameBuffer();
			HDC hdc = GetDC(hWnd);
			CreateFrameBuffer(hdc, width, height);
			return 0;	
		}
		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
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

void CApplication::CreateFrameBuffer(HDC hdc, INT width, INT height)
{
	m_backBuffer.dc     = CreateCompatibleDC(hdc);
	m_backBuffer.bmp    = CreateCompatibleBitmap(hdc, width, height);
	m_backBuffer.bmpOld = (HBITMAP)SelectObject(m_backBuffer.dc, m_backBuffer.bmp);
	m_backBuffer.size   = {width, height};
}

void CApplication::ReleaseFrameBuffer()
{
	if(m_backBuffer.dc)
	{
		SelectObject(m_backBuffer.dc, m_backBuffer.bmpOld);
		DeleteObject(m_backBuffer.bmp);
		DeleteDC(m_backBuffer.dc);
		m_backBuffer.dc = nullptr;
	}
}

void CApplication::ClearBackBuffer()
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	// 화면을 흰색으로 채움.
	FillRect(m_backBuffer.dc, &rc, m_hClearBrush);
}

void CApplication::PresentToFrontBuffer(HDC hdc)
{
	BitBlt(hdc, 0, 0, m_backBuffer.size.cx, m_backBuffer.size.cy, m_backBuffer.dc, 0, 0, SRCCOPY);
}


#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif
