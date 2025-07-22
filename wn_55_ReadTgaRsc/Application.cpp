// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;


void* MemoryRead(void* pOut, void* pIn, int iSize)
{
	memcpy(pOut, pIn, iSize);

	return (char*)pIn + iSize;
}


//
BYTE* ReadTGAFromMemory(void* pMemory,BYTE** pImgId, BYTE** pColorMap, BYTE* pImgType, SHORT* pImgWidth, SHORT* pImgHeight, BYTE* pImgBit)
{

	BYTE		cIdLength	;
	BYTE		cMapType	;
	BYTE		ImgType		;

	WORD		cMapOrg		;
	WORD		cMapLen		;
	BYTE		cMapSize	;

	WORD		ImgOrgX		;
	WORD		ImgOrgY		;
	WORD		ImgW		;
	WORD		ImgH		;
	BYTE		ImgPixel	;
	BYTE		ImgDes		;

	long		imageSize	;			// size of TGA image
	int			colorMode	;			// 4 for RGBA, 3 for RGB
	BYTE*		pPixel		= NULL;		// the TGA Pixel Data

	void*		pMem= pMemory;

	pMem = MemoryRead(&cIdLength, pMem, sizeof(BYTE) );							// To Read Id Length
	pMem = MemoryRead(&cMapType	, pMem, sizeof(BYTE) );							// To Read Color Map Type
	pMem = MemoryRead(&ImgType	, pMem, sizeof(BYTE) );							// To Read Image Type

	pMem = MemoryRead(&cMapOrg	, pMem, sizeof(SHORT) );						// To Read Color Map Origin
	pMem = MemoryRead(&cMapLen	, pMem, sizeof(SHORT) );						// To Read Color Map Length
	pMem = MemoryRead(&cMapSize	, pMem, sizeof(BYTE	) );						// To Read Color Map Entry Size

	pMem = MemoryRead(&ImgOrgX	, pMem, sizeof(SHORT) );						// To Read Image X Origin
	pMem = MemoryRead(&ImgOrgY	, pMem, sizeof(SHORT) );						// To Read Image Y Origin
	pMem = MemoryRead(&ImgW		, pMem, sizeof(SHORT) );						// To Read Image Width
	pMem = MemoryRead(&ImgH		, pMem, sizeof(SHORT) );						// To Read Image Height
	pMem = MemoryRead(&ImgPixel	, pMem, sizeof(BYTE) );							// To Read Pixel Depth
	pMem = MemoryRead(&ImgDes	, pMem, sizeof(BYTE) );							// To Read Image Descriptor


	if(0<cIdLength && pImgId)
	{
		BYTE* pId = new BYTE[cIdLength+1];

		memset(pId, 0, cIdLength+1);
		pMem = MemoryRead(pId, pMem, cIdLength);
		*pImgId	= pId;
	}

	int	ColorMapSize = cMapLen * cMapSize;

	if(ColorMapSize>0 && pColorMap)
	{
		BYTE*  pImgMap = new BYTE[ColorMapSize];
		pMem = MemoryRead(pImgMap, pMem, ColorMapSize );							// To Read Image Descriptor
		*pColorMap	= pImgMap;
	}

	// for our purposes, the image type should be either a 2 or a 3
	if ((ImgType != 2) && (ImgType!= 3))
	{
		return NULL;
	}



	// colormode -> 3 = BGR, 4 = BGRA
	colorMode = ImgPixel / 8;

	// the TGA data RGB
	if(3 == colorMode)
	{
		imageSize = ImgW * ImgH * colorMode;
		pPixel = (BYTE*)malloc(imageSize);
		pMem = MemoryRead(pPixel, pMem, sizeof(BYTE) * imageSize );
	}

	// the TGA data RGBA
	else if( 4 == colorMode)
	{
		// RGBA --> RGB
		BYTE* tPixel = NULL;
		INT iSize = ImgW * ImgH;

		imageSize = iSize * 3;
		pPixel = (BYTE*)malloc(imageSize);
		tPixel = (BYTE*)malloc(iSize * colorMode);

		pMem = MemoryRead(tPixel, pMem, sizeof(BYTE) * iSize * colorMode );

		for (int i=0, j=0; i < iSize; ++i, ++j)
		{
			int idx1 =  i*3;
			int idx2 =  i*4;

			pPixel[idx1+0] = tPixel[idx2 + 1];
			pPixel[idx1+1] = tPixel[idx2 + 2];
			pPixel[idx1+2] = tPixel[idx2 + 3];
		}
	}

	if(pImgType		)	*pImgType	= ImgType	;
	if(pImgWidth	)	*pImgWidth	= ImgW		;
	if(pImgHeight	)	*pImgHeight	= ImgH		;
	if(pImgBit		)	*pImgBit	= ImgPixel	;

	return pPixel;
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

	memset(m_sTga, 0, sizeof(m_sTga));
	m_pPixel		= NULL;

	m_ImgId		= 0;
	m_ImgType	= 0;
	m_ImgWidth	= 0;
	m_ImgHeight	= 0;
	m_ImgBit	= 0;
	m_ImgMap	= NULL;
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



	HRSRC hrsc = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_TGA1), "TGA");
	HGLOBAL hglobal = LoadResource(GetModuleHandle(NULL), hrsc);

	void* pMemory = LockResource(hglobal);

	m_pPixel = (BYTE*)ReadTGAFromMemory(pMemory, &m_ImgId, &m_ImgMap, &m_ImgType, &m_ImgWidth, &m_ImgHeight, &m_ImgBit);

	UnlockResource(hglobal);
	FreeResource(hglobal);


	return S_OK;
}


void CApplication::Cleanup()
{
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
	if (!m_pPixel)
		return;

	BITMAPINFOHEADER	ih;

	HDC hdc=GetDC(m_hWnd);

	memset(&ih, 0, sizeof(ih));

	ih.biSize			= 40;
    ih.biWidth			= m_ImgWidth;
    ih.biHeight			= m_ImgHeight;
    ih.biPlanes			= 1;
    ih.biBitCount		= m_ImgBit;
    ih.biCompression	= 0;
    ih.biSizeImage		= m_ImgWidth * m_ImgHeight * m_ImgBit;

	int					bx		= ih.biWidth;
	int					by		= ih.biHeight;
	BYTE*				pPixel	= m_pPixel;

	SetDIBitsToDevice(
		hdc						// handle to DC
		, 0						// x-coord of destination upper-left corner
		, 0						// y-coord of destination upper-left corner
		, bx					// source rectangle width
		, by					// source rectangle height
		, 0						// x-coord of source lower-left corner
		, 0						// y-coord of source lower-left corner
		, 0						// first scan line in array
		, by					// number of scan lines
		, pPixel				// array of DIB bits
		, (BITMAPINFO*)&ih		// bitmap information
		, DIB_RGB_COLORS		// RGB or palette indexes
		);


	StretchDIBits(
		hdc						// handle to DC
		, 200					// x-coord of destination upper-left corner
		, 200					// y-coord of destination upper-left corner
		, bx*2					// width of destination rectangle
		, by*2					// height of destination rectangle
		, 0						// x-coord of source upper-left corner
		, 0						// y-coord of source upper-left corner
		, bx					// width of source rectangle
		, by					// height of source rectangle
		, pPixel				// bitmap bits
		, (BITMAPINFO*)&ih		// bitmap data
		, DIB_RGB_COLORS		// usage options
		, SRCCOPY				// raster operation code
		);


	ReleaseDC(m_hWnd, hdc);
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif