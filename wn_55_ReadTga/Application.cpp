// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;


BYTE* LoadTGAFile(char *filename, TGAHEADER& tgaH)
{
	FILE*		fp;
	BYTE		ucharBad;					// garbage data
	SHORT		sintBad;					// garbage data
	long		imageSize;					// size of TGA image
	int			colorMode;					// 4 for RGBA, 3 for RGB
	BYTE*		pPixel3		= NULL;			// the TGA data RGB
	BYTE*		pPixel4		= NULL;			// the TGA data RGBA

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

	// colormode -> 3 = BGR, 4 = BGRA
	colorMode = tgaH.bitCount / 8;
	imageSize = tgaH.imageWidth * tgaH.imageHeight * colorMode;

	pPixel4 = (BYTE*)malloc(imageSize);

	fread(pPixel4, sizeof(BYTE), imageSize, fp);
	fclose(fp);


	return pPixel4;

//	if(colorMode <=3)
//		return pPixel4;
//
//
//	pPixel3		= (BYTE*)malloc(tgaH.imageWidth * tgaH.imageHeight * 3);
//
//	// change BGRA to BGR
//
//	tgaH.bitCount =24;
//
//	for (int i = 0; i < imageSize/4; ++i)
//	{
//		int idx1 =  i*3;
//		int idx2 =  i*4;
//
//		pPixel3[idx1+0] = pPixel4[idx2 + 0];
//		pPixel3[idx1+1] = pPixel4[idx2 + 1];
//		pPixel3[idx1+2] = pPixel4[idx2 + 2];
//	}
//
//	free(pPixel4);
//
//	return pPixel3;
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

	m_bShowCusor= true;

	memset(m_sTga, 0, sizeof(m_sTga));
	m_pPixel		= NULL;
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
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_F_OPEN:
				{
					if(SUCCEEDED(FileOpen()))
					{
						SAFE_FREE(m_pPixel);
						m_pPixel = LoadTGAFile(m_sTga, m_Tga);
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

	strcpy(m_sTga, OFN.lpstrFile);

	return 1;

}



void CApplication::Render()
{
	if (!m_pPixel)
		return;

	BITMAPINFOHEADER	ih;

	HDC hdc=GetDC(m_hWnd);

	memset(&ih, 0, sizeof(ih));

	ih.biSize			= 40;
    ih.biWidth			= m_Tga.imageWidth;
    ih.biHeight			= m_Tga.imageWidth;
    ih.biPlanes			= 1;
    ih.biBitCount		= m_Tga.bitCount;
    ih.biCompression	= 0;
    ih.biSizeImage		= m_Tga.imageWidth * m_Tga.imageHeight*m_Tga.bitCount/8;

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