// D3D Sprite Tutorial
//
////////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4996)

#pragma comment(linker, "/subsystem:windows")
#pragma comment(lib, "Gdiplus.lib")

#include <windows.h>
#include <mmsystem.h>

#include <gdiplus.h>
using namespace Gdiplus;

char				m_sCls[128]		;
HINSTANCE			m_hInst		= NULL;
HWND				m_hWnd		= NULL;
DWORD				m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
DWORD				m_dScnX		= 800;			// Screen Width
DWORD				m_dScnY		= 600;			// Screen Height
BOOL				m_bShowCusor= TRUE;			// Show Cusor

//Window+Device관련 함수들
INT		Create(HINSTANCE hInst);
INT		Run();
void	Cleanup();

LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


HBRUSH m_brush;
HFONT  m_font;

char   m_message1[] = "GDI FONT 테스트";
WCHAR  m_message2[] = L"GDI PLUS FONT 테스트";
WCHAR  m_font_family [] = L"나눔바른고딕OTF";

void OnPaint(HDC hdc);


class CGdiPlusStarter
{
protected:
	ULONG_PTR m_gpToken;
public:
	bool m_bSuccess;
	CGdiPlusStarter()
	{
		GdiplusStartupInput gpsi;
		m_bSuccess=(GdiplusStartup(&m_gpToken, &gpsi, NULL) == Ok);
	}
	~CGdiPlusStarter()
	{
		GdiplusShutdown(m_gpToken);
	}
};
CGdiPlusStarter g_gps;

INT Create( HINSTANCE hInst)
{
	if(!g_gps.m_bSuccess)
		return -1;


	////////////////////////////////////////////////////////////////////////////
	//
	m_brush = CreateSolidBrush(RGB(0, 100, 150));

	LOGFONT	logFont=
	{
		30,						//Height;
		0,						//Width;
		0,						//Escapement;
		0,						//Orientation;
		0,						//Weight;
		0,						//Italic;
		0,						//Underline;
		0,						//StrikeOut;
		DEFAULT_CHARSET,		//CharSet;
		OUT_TT_ONLY_PRECIS,		//OutPrecision;
		0,						//ClipPrecision;
		ANTIALIASED_QUALITY,	//Quality;
		FF_DONTCARE,			//PitchAndFamily;
		"나눔바른고딕OTF"		//FaceName[LF_FACESIZE];
	};
	m_font = CreateFontIndirect(&logFont);


	//
	////////////////////////////////////////////////////////////////////////////

	m_hInst	= hInst;
	strcpy(m_sCls, "D3D Tutorial");

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
		, m_sCls
	};

	RegisterClass( &wc );

	RECT rc;									//Create the application's window

	SetRect( &rc, 0, 0, m_dScnX, m_dScnY);
	AdjustWindowRect( &rc, m_dWinStyle, FALSE );

	int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
	int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);

	m_hWnd = CreateWindow( m_sCls
		, m_sCls
		, m_dWinStyle
		, (iScnSysW - (rc.right - rc.left))/2
		, (iScnSysH - (rc.bottom - rc.top))/2
		, (rc.right - rc.left)
		, (rc.bottom - rc.top)
		, GetDesktopWindow()
		, NULL
		, m_hInst
		, NULL );

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCusor);

	return 0;
}


void Cleanup()
{
	////////////////////////////////////////////////////////////////////////////
	//
	if(m_font) { DeleteObject(m_font); m_font = NULL; }
	if(m_brush) { DeleteObject(m_brush); m_brush = NULL; }

	//
	////////////////////////////////////////////////////////////////////////////
}


LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		//if( !PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE ) )
		//	continue;

		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		//HDC hdc = GetDC(m_hWnd);
		//if(!hdc)
		//	continue;
		//OnPaint(hdc);
		//ReleaseDC(m_hWnd, hdc);
	}


	UnregisterClass( m_sCls, m_hInst);
	return 0;
}

LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return MsgProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	if(FAILED(Create(hInst)))
		return -1;

	return Run();
}


void OnPaint(HDC hdc)
{
	HFONT oldFont = (HFONT)SelectObject(hdc, m_font);
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	FillRect(hdc, &rc, m_brush);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(114, 78, 51));
	for(int y=0; y<3; ++y)
	{
		for(int x=0; x<3; ++x)
			TextOut(hdc, 40+x-1, 100+y-1, m_message1, strlen(m_message1));
	}
	SetTextColor(hdc, RGB(253, 245, 232));
	TextOut(hdc, 40, 100, m_message1, strlen(m_message1));
	SetTextColor(hdc, RGB(0, 0, 0));
	TextOut(hdc, 40+1, 200+1, m_message1, strlen(m_message1));
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 40, 200, m_message1, strlen(m_message1));
	SelectObject(hdc, oldFont);


	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	FontFamily fontFamily(m_font_family);
	StringFormat strformat;

	GraphicsPath path;
	path.AddString(m_message2, wcslen(m_message2), &fontFamily,
	FontStyleRegular, 30, Gdiplus::Point(40, 300), &strformat);

	Rect bounds;
	path.GetBounds(&bounds);

	//for(int i=1; i<2; ++i)
	{
		Pen pen(Color(128, 114,78,51), 2.0f);
		pen.SetLineJoin(LineJoinRound);
		graphics.DrawPath(&pen, &path);
	}

	SolidBrush brush(Color(255, 253,245,232));
	graphics.FillPath(&brush, &path);
}
