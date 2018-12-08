// D3D Sprite Tutorial
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)

#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
#include "GdiFont.h"
#include "GdiPlusFont.h"

char    m_fontFile[] ="_font/NanumBarunGothic.otf";
char    m_msg_gdi_text[] = "가나다라마바사 ABCDEFGHIJ abcdefghij1234567890: gdi draw text";
char    m_msg_gdi_plus_text[] = "가나다라마바사 ABCDEFGHIJ abcdefghij1234567890: gdi plus: text";
char    m_msg_gdi_plus_path[] = "가나다라마바사 ABCDEFGHIJ abcdefghij1234567890: gdi plus: path";

GdiFont*  font_gdi1;
GdiFont*  font_gdi2;
GdiPlusFont* font_gdi_plus;

void OnPaint(HDC hdc)
{
	GdiPlus::clearColor(XRGB(0, 100, 160));
	GdiPlus::beginGraphics(hdc);

	if(font_gdi1)
	{
		font_gdi1->drawText(hdc, m_msg_gdi_text, 10, 20, RGB(253, 245, 232));
	}

	if(font_gdi2)
	{
		font_gdi2->drawTextWithShadow(hdc, m_msg_gdi_text, 10, 50, RGB(253, 245, 232), RGB(114, 78, 51));
	}
	if(font_gdi2)
	{
		font_gdi2->drawTextWithOutline(hdc, m_msg_gdi_text, 10, 80, RGB(253, 245, 232), RGB(114, 78, 51));
	}

	if(font_gdi_plus)
	{
		font_gdi_plus->drawPath(m_msg_gdi_plus_path, 10.0F, 200.0F, XRGB(253, 245, 232));
		font_gdi_plus->drawText(m_msg_gdi_plus_text, 10.0F, 240.0F, XRGB(253, 245, 232));
	}

	GdiPlus::endGraphics();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//Window

HWND		m_hWnd		= NULL;
DWORD		m_dScnX		= 800;			// Screen Width
DWORD		m_dScnY		= 600;			// Screen Height
BOOL		m_bShowCusor= TRUE;			// Show Cusor

INT		Run();
void	Cleanup();
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

INT Create()
{
	// for gdiplus
	if(!GdiPlus::getInstance()->isSuccess())
		return -1;

	// create gdi font
	font_gdi1 = GdiFont::createWithPoint(m_fontFile, 15);
	if(!font_gdi1)
		return -1;

	font_gdi2 = GdiFont::createWithPixel(m_fontFile, 20);
	if(!font_gdi2)
		return -1;

	font_gdi_plus = GdiPlusFont::create(m_fontFile, 20.0f);
	if(!font_gdi_plus)
		return -1;

	// create window
	std::string clzz_name = "draw font";
	HINSTANCE hInst =(HINSTANCE)GetModuleHandle(NULL);
	WNDCLASS wc =								// Register the window class
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, hInst
		, NULL
		, LoadCursor(NULL,IDC_ARROW)
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, NULL
		, clzz_name.c_str()
	};

	RegisterClass( &wc );

	RECT rc;									//Create the application's window
	DWORD style = WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	SetRect( &rc, 0, 0, m_dScnX, m_dScnY);
	AdjustWindowRect( &rc, style, FALSE );

	int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
	int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);

	m_hWnd = CreateWindow( clzz_name.c_str()
		, clzz_name.c_str()
		, style
		, (iScnSysW - (rc.right - rc.left))/2
		, (iScnSysH - (rc.bottom - rc.top))/2
		, (rc.right - rc.left)
		, (rc.bottom - rc.top)
		, GetDesktopWindow()
		, NULL
		, hInst
		, NULL );

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCusor);
	return 0;
}


void Cleanup()
{
	delete font_gdi1;
	delete font_gdi2;
	delete font_gdi_plus;
	GdiFont::removeAllHFontResource();
}

LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch(msg)
	{
		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				case VK_ESCAPE:
				{
					SendMessage(hWnd, WM_DESTROY, 0, 0);
					break;
				}
			}
			return 0;
		}
		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage(0);
			return 0;
		}
		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc=BeginPaint(hWnd, &ps);
			OnPaint(hdc);
			EndPaint(hWnd, &ps);
			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT Run()
{
	// Enter the message loop
	MSG msg={0};
	memset( &msg, 0, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		continue;

		//HDC hdc = GetDC(m_hWnd);
		//if(!hdc)
		//	continue;
		//OnPaint(hdc);
		//ReleaseDC(m_hWnd, hdc);
	}

	return 0;
}

INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, INT )
{
	if(FAILED(Create()))
		return -1;
	return Run();
}


