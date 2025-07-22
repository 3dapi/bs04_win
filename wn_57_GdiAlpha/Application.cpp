// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;


BYTE* LoadTGAFile(const char* filename, TGAHEADER& tgaH)
{
	FILE*		fp;
	BYTE		ucharBad;					// garbage data
	SHORT		sintBad;					// garbage data
	long		imageSize;					// size of TGA image
	int			colorMode;					// 4 for RGBA, 3 for RGB
	BYTE*		pSrc		= NULL;			// the TGA data RGB

	// open the TGA file
	fp = fopen(filename, "rb");

	if (!fp)
		return NULL;

	// read first two bytes of garbage
	fread(&ucharBad, sizeof(BYTE), 1, fp);
	fread(&ucharBad, sizeof(BYTE), 1, fp);

	// read in the image type
	fread(&tgaH.imageTypeCode, sizeof(tgaH.imageTypeCode), 1, fp);

	// for our purposes, the image type should be either a 2 or a 3
	if ((tgaH.imageTypeCode != 2) && (tgaH.imageTypeCode != 3))
	{
		fclose(fp);
		return NULL;
	}

	// read 13 bytes of garbage data
	fread(&sintBad, sizeof(sintBad), 1, fp);
	fread(&sintBad, sizeof(sintBad), 1, fp);
	fread(&ucharBad, sizeof(ucharBad), 1, fp);
	fread(&sintBad, sizeof(sintBad), 1, fp);
	fread(&sintBad, sizeof(sintBad), 1, fp);

	// read image dimensions
	fread(&tgaH.imageWidth, sizeof(tgaH.imageWidth), 1, fp);
	fread(&tgaH.imageHeight, sizeof(tgaH.imageHeight), 1, fp);

	// read bit depth
	fread(&tgaH.bitCount, sizeof(tgaH.bitCount), 1, fp);

	// read garbage
	fread(&ucharBad, sizeof(ucharBad), 1, fp);


	colorMode = tgaH.bitCount / 8;
	imageSize = tgaH.imageWidth * tgaH.imageHeight * colorMode;

	pSrc = (BYTE*)malloc(imageSize);

	fread(pSrc, sizeof(BYTE), imageSize, fp);
	fclose(fp);

	BYTE* pDst = (BYTE*)malloc(imageSize);
	for (int j=0; j< tgaH.imageHeight; ++j)
	{
		for (int i=0; i< tgaH.imageWidth; ++i)
		{
			int src = (j * tgaH.imageWidth + i)*3;
			int dst = ((tgaH.imageHeight-j-1) * tgaH.imageWidth + i) * colorMode;
			int g   = pSrc[src+0];
			int b   = pSrc[src+1];
			int r   = pSrc[src+2];
			pDst[dst+0] = r;
			pDst[dst+1] = g;
			pDst[dst+2] = b;
		}
	}
	free(pSrc);
	return pDst;
}



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
	m_pPixel	= NULL;
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
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
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
	if(m_bitmap)
	{
		DeleteObject(m_bitmap);
		m_bitmap = {};
	}
	SAFE_FREE(	m_pPixel	);

	if(m_hInst)
		UnregisterClass( m_sCls, m_hInst);
}


INT CApplication::Run()
{
	MSG msg;
	memset( &msg, 0, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
		if( GetMessage( &msg, NULL, 0U, 0U) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	UnregisterClass( m_sCls, m_hInst);
	return 1;
}





LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_CREATE:
		{
			CreateFromTga("ground.tga");
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			OnDraw(BeginPaint(hWnd, &ps));
			EndPaint(hWnd, &ps);
			return 0;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_F_OPEN:
				{
					if(SUCCEEDED(FileOpen()))
					{
						CreateFromTga(m_sTga);
						InvalidateRect(hWnd, 0, 1);
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




int CApplication::FileOpen()
{
	TCHAR	sDir[MAX_PATH]="\0";
	OPENFILENAME OFN;
	TCHAR sF[MAX_PATH];
	TCHAR lpstrFile[MAX_PATH]="";

	GetCurrentDirectory(MAX_PATH, sDir);

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=m_hWnd;
	OFN.lpstrFilter	="Taga Files(*.tga)\0*.tga\0All Files(*.*)\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=MAX_PATH;

	OFN.lpstrTitle="Choose the file...";
	OFN.lpstrFileTitle=sF;
	OFN.nMaxFileTitle=MAX_PATH;

	if( !GetOpenFileName(&OFN))
		return -1;

	if(!strlen(OFN.lpstrFile))
		return -1;

	SetCurrentDirectory(sDir);

	m_sTga = (const char*)OFN.lpstrFile;

	return 1;

}



void CApplication::OnDraw(HDC hdc)
{
	HDC mDC=CreateCompatibleDC(hdc);
	auto oldBmp=SelectObject(mDC, m_bitmap);

	HBRUSH hBrush = CreateSolidBrush(RGB(0xFF, 0xFF, 0));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, 50, 50, 500, 400);
	SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	DeleteObject(hBrush);

	//WINGDIAPI BOOL  WINAPI BitBlt(
	//	_In_ HDC hdc,
	//	_In_ int x,
	//	_In_ int y,
	//	_In_ int cx,
	//	_In_ int cy,
	//	_In_opt_ HDC hdcSrc,
	//	_In_ int x1,
	//	_In_ int y1,
	//	_In_ DWORD rop);
	//WINGDIAPI BOOL WINAPI TransparentBlt(
	//	_In_ HDC hdcDest,
	//	_In_ int xoriginDest,
	//	_In_ int yoriginDest,
	//	_In_ int wDest,
	//	_In_ int hDest,
	//	_In_ HDC hdcSrc,
	//	_In_ int xoriginSrc,
	//	_In_ int yoriginSrc,
	//	_In_ int wSrc,
	//	_In_ int hSrc,
	//	_In_ UINT crTransparent);
	//BOOL AlphaBlend(
	//  HDC           hdcDest,
	//  int           xoriginDest,
	//  int           yoriginDest,
	//  int           wDest,
	//  int           hDest,
	//  HDC           hdcSrc,
	//  int           xoriginSrc,
	//  int           yoriginSrc,
	//  int           wSrc,
	//  int           hSrc,
	//  BLENDFUNCTION ftn
	//);

	BitBlt        (hdc,  10,  10, m_Tga.imageWidth  , m_Tga.imageHeight   , mDC, 0, 0, SRCCOPY);
	TransparentBlt(hdc, 270,  10, m_Tga.imageWidth/2, m_Tga.imageHeight/2 , mDC, 0, 0, m_Tga.imageWidth, m_Tga.imageHeight, RGB(255,0,255));
	AlphaBlend    (hdc,  10, 270, m_Tga.imageWidth  , 200                 , mDC, 0, 0, m_Tga.imageWidth, m_Tga.imageHeight, {AC_SRC_OVER, 0, 120, AC_SRC_OVER});
	AlphaBlend    (hdc, 270, 270, m_Tga.imageWidth  , 200                 , mDC, 0, 0, m_Tga.imageWidth, m_Tga.imageHeight, {AC_SRC_OVER, 0, 20, AC_SRC_ALPHA});


	SelectObject(mDC, oldBmp);
	DeleteObject(mDC);
}

void CApplication::CreateFromTga(const std::string& filename)
{
	SAFE_FREE(m_pPixel);
	m_pPixel = LoadTGAFile(filename.c_str(), m_Tga);
	int pxl_w = m_Tga.imageWidth;
	int pxl_h = m_Tga.imageHeight;
	int pxl_b = m_Tga.bitCount;
	void* lpbits = (void*)m_pPixel;
	if(!lpbits)
		return;

	BITMAPINFOHEADER	ih{};
	ih.biSize			= 40;
	ih.biWidth			= m_Tga.imageWidth;
	ih.biHeight			= m_Tga.imageHeight;
	ih.biPlanes			= 1;
	ih.biBitCount		= m_Tga.bitCount;
	ih.biCompression	= 0;
	ih.biSizeImage		= m_Tga.imageWidth * m_Tga.imageHeight*m_Tga.bitCount/8;

	BITMAPINFO*			bmp_info= (BITMAPINFO*)&ih;

	HDC dc = GetDC(NULL);
	m_bitmap=CreateDIBitmap(dc, &ih, CBM_INIT, (const void*)m_pPixel, (BITMAPINFO*)&ih, DIB_RGB_COLORS);
	ReleaseDC(NULL, dc);
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif

