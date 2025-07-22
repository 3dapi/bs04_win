// D3D Sprite Tutorial
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)

#pragma comment(linker, "/subsystem:windows")
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Dwrite.lib")
#include <vector>
#include <string>

#include <Dwrite.h>
#include <windows.h>
#include <mmsystem.h>
#include <gdiplus.h>

class GdiPlusStarter
{
protected:
	ULONG_PTR m_gpToken;
	Gdiplus::PrivateFontCollection* m_fontCollection;
	bool m_bSuccess;
public:
	GdiPlusStarter() : m_bSuccess(0), m_fontCollection(NULL)
	{
		Gdiplus::GdiplusStartupInput gpsi;
		m_bSuccess = Gdiplus::GdiplusStartup(&m_gpToken, &gpsi, NULL) == Gdiplus::Ok;
		if(m_bSuccess)
			m_fontCollection = new Gdiplus::PrivateFontCollection;
	}
	~GdiPlusStarter()
	{
		if(m_fontCollection)
			delete m_fontCollection;
		Gdiplus::GdiplusShutdown(m_gpToken);
	}
	Gdiplus::PrivateFontCollection* FontCollection() { return m_fontCollection; }
	const bool IsSuccess() const { return m_bSuccess; }
};



char				m_sCls[128]		;
HINSTANCE			m_hInst		= NULL;
HWND				m_hWnd		= NULL;
DWORD				m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
DWORD				m_dScnX		= 800;			// Screen Width
DWORD				m_dScnY		= 600;			// Screen Height
BOOL				m_bShowCursor= TRUE;			// Show Cursor

//Window+Device관련 함수들
INT		Create(HINSTANCE hInst);
INT		Run();
void	Cleanup();

LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

static std::wstring ToWideString(const char* str)
{
	int wcs_len =
	MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	std::wstring ret_wstr(wcs_len-1, 0);
	MultiByteToWideChar(CP_ACP, 0, str, -1, &ret_wstr[0], wcs_len);
	return ret_wstr;
}

static std::string ToMultiByteString(const wchar_t* w_str)
{
	int str_len =
	WideCharToMultiByte( CP_ACP, 0, w_str, -1, NULL, 0, NULL, NULL );
	std::string ret_str(str_len-1, 0);
	WideCharToMultiByte( CP_ACP, 0, w_str, -1, &ret_str[0], str_len, NULL, NULL );
	return ret_str;
}

HFONT   m_font;
LOGFONT m_FontLog={0};
char    m_fontFile[] ="_font/NanumBarunGothic.otf";
char    m_message1[] = "GDI FONT 테스트";
char    m_message2[] = "GDI PLUS FONT 테스트";
static GdiPlusStarter	g_gdiStarter;
Gdiplus::FontFamily*	g_gdiFontFamily = NULL;


static HRESULT GetLogFontFromFileName(const char* fontFileName, _Out_ LOGFONT* logFont)
{
	std::wstring ret_wstr = ToWideString(fontFileName);
	IDWriteFactory* dwriteFactory = NULL;
    IDWriteFontFile* fontFile = NULL;
    IDWriteFontFace* fontFace = NULL;
    IDWriteGdiInterop* gdiInterop = NULL;

	// Create the factory objects.
	HRESULT hr=S_OK;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)(&dwriteFactory) );
	if(FAILED(hr))
		return -1;

    hr = dwriteFactory->GetGdiInterop(&gdiInterop);
	if(FAILED(hr))
		return -1;

    // Open the file and determine the font type.
    hr = dwriteFactory->CreateFontFileReference(ret_wstr.c_str(), nullptr, &fontFile);
	if(FAILED(hr))
		return -1;

    BOOL isSupportedFontType = false;
    DWRITE_FONT_FILE_TYPE fontFileType;
    DWRITE_FONT_FACE_TYPE fontFaceType;
    UINT32 numberOfFaces = 0;
    hr= fontFile->Analyze(&isSupportedFontType, &fontFileType, &fontFaceType, &numberOfFaces);
	if(FAILED(hr))
		return -1;

    if (!isSupportedFontType)
        return DWRITE_E_FILEFORMAT;

    // Set up a font face from the array of font files (just one)
	IDWriteFontFile* fontFileArray[] = {fontFile};

    hr = dwriteFactory->CreateFontFace(
			fontFaceType,
			ARRAYSIZE(fontFileArray), // file count
			&fontFileArray[0], // or GetAddressOf if WRL ComPtr
			0, // faceIndex
			DWRITE_FONT_SIMULATIONS_NONE,
			&fontFace );
	if(FAILED(hr))
		return -1;

    // Get the necessary logical font information.
	LOGFONTW wlogFont;
    hr = gdiInterop->ConvertFontFaceToLOGFONT(fontFace, &wlogFont);
	if(FAILED(hr))
		return -1;


	std::string strFaceName = ToMultiByteString(wlogFont.lfFaceName);

	logFont->lfHeight         = wlogFont.lfHeight        ;
    logFont->lfWidth          = wlogFont.lfWidth         ;
    logFont->lfEscapement     = wlogFont.lfEscapement    ;
    logFont->lfOrientation    = wlogFont.lfOrientation   ;
    logFont->lfWeight         = wlogFont.lfWeight        ;
    logFont->lfItalic         = wlogFont.lfItalic        ;
    logFont->lfUnderline      = wlogFont.lfUnderline     ;
    logFont->lfStrikeOut      = wlogFont.lfStrikeOut     ;
    logFont->lfCharSet        = wlogFont.lfCharSet       ;
    logFont->lfOutPrecision   = wlogFont.lfOutPrecision  ;
    logFont->lfClipPrecision  = wlogFont.lfClipPrecision ;
    logFont->lfQuality        = wlogFont.lfQuality       ;
    logFont->lfPitchAndFamily = wlogFont.lfPitchAndFamily;
	strcpy(logFont->lfFaceName, strFaceName.c_str());

	hr = fontFace->Release();
	hr = fontFile->Release();
	hr = gdiInterop->Release();
	hr = dwriteFactory->Release();
	return S_OK;
}

static int CreatePrivateFontFamily(const char* font_file)
{
	HRESULT ret = 0;
	ret = AddFontResourceEx(m_fontFile, FR_PRIVATE, 0);
	ret = GetLogFontFromFileName(m_fontFile, &m_FontLog);
	if(0>ret)
		return -1;

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




	//std::string full_path = FullPath(font_file);
	std::wstring wstr_file = ToWideString(font_file);
	ret = g_gdiStarter.FontCollection()->AddFontFile(wstr_file.c_str());
	int count = g_gdiStarter.FontCollection()->GetFamilyCount();
	if(0>=count)
		return -1;

	int found_count = 0;
	g_gdiFontFamily = new Gdiplus::FontFamily[count];
	g_gdiStarter.FontCollection()->GetFamilies(count, g_gdiFontFamily, &found_count);
	return 0;
}

void OnGdiPaint(HDC hdc);


INT Create( HINSTANCE hInst)
{
	if(!g_gdiStarter.IsSuccess())
		return -1;

	if(0>CreatePrivateFontFamily(m_fontFile))
		return -1;

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
	::ShowCursor(m_bShowCursor);

	return 0;
}


void Cleanup()
{
	////////////////////////////////////////////////////////////////////////////
	//
	if(g_gdiFontFamily) { delete [] g_gdiFontFamily; g_gdiFontFamily = NULL; }
	//
	////////////////////////////////////////////////////////////////////////////

	//RemoveFontResource(m_fontFile);
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
			OnGdiPaint(hdc);
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

		HDC hdc = GetDC(m_hWnd);
		if(!hdc)
			continue;
		OnGdiPaint(hdc);
		ReleaseDC(m_hWnd, hdc);
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


void OnGdiPaint(HDC hdc)
{
	Gdiplus::Graphics graphics(hdc);
	graphics.Clear(Gdiplus::Color(255, 0, 100, 150));
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

	HFONT oldFont = (HFONT)SelectObject(hdc, m_font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(114, 78, 51));
	for(int y=0; y<3; ++y)
	{
		for(int x=0; x<3; ++x)
		{
			TextOut(hdc, 10+x-1, 100+y-1, m_message1, strlen(m_message1));

			RECT rc_dt ={10+x-1, 200+y-1, 400, 260};
			DrawText(hdc, m_message1, -1, &rc_dt, DT_LEFT|DT_TOP);
		}
	}
	SetTextColor(hdc, RGB(253, 245, 232));
	TextOut(hdc, 10, 100, m_message1, strlen(m_message1));
	RECT rc_dt ={10, 200, 400, 260};
	DrawText(hdc, m_message1, -1, &rc_dt, DT_LEFT|DT_TOP);
	SelectObject(hdc, oldFont);


	Gdiplus::StringFormat strformat;
	Gdiplus::GraphicsPath path;

	std::wstring wstr_msg2 = ToWideString(m_message2);
	path.AddString(wstr_msg2.c_str(), wcslen(wstr_msg2.c_str()), g_gdiFontFamily,
	Gdiplus::FontStyleRegular, 30, Gdiplus::Point(10, 300), &strformat);

	Gdiplus::Rect bounds;
	path.GetBounds(&bounds);

	{
		Gdiplus::Pen pen(Gdiplus::Color(80, 114,78,51), 4.0f);
		pen.SetLineJoin(Gdiplus::LineJoinRound);
		graphics.DrawPath(&pen, &path);
	}
	{
		Gdiplus::Pen pen(Gdiplus::Color(160, 114,78,51), 2.0f);
		pen.SetLineJoin(Gdiplus::LineJoinRound);
		graphics.DrawPath(&pen, &path);
	}
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 253,245,232));
	graphics.FillPath(&brush, &path);
}
