// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


// This File was first created by Willem van Schaik and modified by Heesung Oh(GaLic Studio).
// You can find Original Source from www.pnglib.com.

// This code is released under the libpng license.
// For conditions of distribution and use, see the disclaimer
// and license in png.h



#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;


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

	memset(m_sPng, 0, sizeof m_sPng);
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
	::ShowCursor(m_bShowCusor);


	return S_OK;
}


void CApplication::Cleanup()
{
	m_Png.Destroy();

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
						PNG_PIXEL	tpPng;

						if(SUCCEEDED(LoadPngFile(&tpPng, m_sPng)))
						{
							INT		nSizePxl;
							INT		n;

							BYTE*	pSrc;
							BYTE*	pDst;


							nSizePxl = tpPng.nWidth * tpPng.nHeight;

							pDst = (BYTE*)malloc(nSizePxl * 3);
							pSrc = (BYTE*)tpPng.pPixel;

							// for 32 or 24 bit
							// R,G, B to B, G, R
							// PNG Order: r, g, b, a
							// Window, B, G, R
							for(n = 0; n < nSizePxl; ++n)
							{
								int idx1 =  n * 3;
								int idx2 =  n * tpPng.nChannel;

								pDst[idx1+0] = pSrc[idx2 + 2];
								pDst[idx1+1] = pSrc[idx2 + 1];
								pDst[idx1+2] = pSrc[idx2 + 0];
							}

							// 픽셀 복사
							m_Png = tpPng;
							m_Png.Destroy();
							m_Png.SetPixelData(pDst);

							char sTmp [MAX_PATH]={0};
							sprintf (sTmp, "vcPng - %s", strrchr(m_sPng, '\\') + 1);
							SetWindowText (hWnd, sTmp);
						}

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
	OFN.lpstrFilter	="Png Files(*.png)\0*.png\0All Files(*.*)\0*.*\0";
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

	strcpy(m_sPng, OFN.lpstrFile);

	return 1;

}



void CApplication::Render()
{
	BYTE*		pPixel	= (BYTE*)m_Png.pPixel;
	HDC			hdc		= NULL;
	BITMAPINFOHEADER	bh		= {0};

	if (!pPixel)
		return;

	hdc=GetDC(m_hWnd);


	// Caution: 이미지의 폭이 홀수이면 이를 4의 배수로 늘려서
	// 색상을 재조정해야 한다.
	// 따라서 이미지의 크기를 4의 배수로 정하는 것이 가장 좋다.
	// 아래는정식으로 4의 배수로 만드는 방법이 아니라 편법이다.
	//long	nWidth = ((3 * m_Png.nWidth + 3L) >> 2) << 2;
	//nWidth	/=3;

	// 이미지를 4의 배수로 조정하고 이렇게 하는 것이 좋다.
	long nWidth		= m_Png.nWidth;

	bh.biSize		= sizeof(BITMAPINFOHEADER);
	bh.biWidth		= nWidth;
	bh.biHeight		= -((long) m_Png.nHeight);
	bh.biPlanes		= 1;
	bh.biBitCount	= 24;
    bh.biSizeImage	= nWidth * m_Png.nHeight * m_Png.nChannel;

	int	bx			= nWidth;
	int	by			= m_Png.nHeight;


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
		, (BITMAPINFO*)&bh		// bitmap information
		, DIB_RGB_COLORS		// RGB or palette indexes
		);


	StretchDIBits(
		hdc						// handle to DC
		, bx					// x-coord of destination upper-left corner
		, by					// y-coord of destination upper-left corner
		, bx*10/4				// width of destination rectangle
		, by*10/4				// height of destination rectangle
		, 0						// x-coord of source upper-left corner
		, 0						// y-coord of source upper-left corner
		, m_Png.nWidth			// width of source rectangle
		, m_Png.nHeight		// height of source rectangle
		, pPixel				// bitmap bits
		, (BITMAPINFO*)&bh		// bitmap data
		, DIB_RGB_COLORS		// usage options
		, SRCCOPY				// raster operation code
		);


	ReleaseDC(m_hWnd, hdc);
}





INT LoadPngFile(PNG_PIXEL*	pPngOut, char* sFileName)
{
	FILE*			fp;
	png_byte        pbSig[8];
	int             iColorType;
	double          dGamma;
	png_color_16*	pBackground;
	png_uint_32		ulRowBytes;

	png_uint_32		png_w = 0;
	png_uint_32		png_h = 0;
	int				png_b = 0;
	png_uint_32		png_d = 0;
	png_uint_32		png_z = 0;
	png_byte*		png_p = 0;
	png_byte**		ppbRowPointers = NULL;

	png_structp		png_ptr = NULL;
	png_infop		info_ptr = NULL;

	fp = fopen(sFileName, "rb");

	if (NULL == fp)
		return -1;


	// first check the eight byte PNG signature
	fread(pbSig, 1, 8, fp);

	if (!png_check_sig(pbSig, 8))
		goto LOAD_IMAGE_ERROR;

	// create the two png(-info) structures
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);

	if (!png_ptr)
		goto LOAD_IMAGE_ERROR;


	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr)
		goto LOAD_IMAGE_ERROR;


	// initialize the png structure
	png_set_read_fn(png_ptr, (png_voidp)fp, NULL);



	png_set_sig_bytes(png_ptr, 8);


	// read all PNG info up to image data
	png_read_info(png_ptr, info_ptr);

	// get width, height, bit-depth and color-type
	png_get_IHDR(png_ptr
		, info_ptr
		, &png_w
		, &png_h
		, &png_b
		, &iColorType
		, NULL, NULL, NULL);

	// expand images of all color-type and bit-depth to 3x8 bit RGB images
	// let the library process things like alpha, transparency, background

	if (png_b == 16)
		png_set_strip_16(png_ptr);

	if (iColorType == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);

	if (png_b < 8)
		png_set_expand(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);

	if (iColorType == PNG_COLOR_TYPE_GRAY ||
		iColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	// set the background color to draw transparent and alpha images over.
	if (png_get_bKGD(png_ptr, info_ptr, &pBackground))
	{
		png_set_background(png_ptr, pBackground, PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
		pPngOut->BgColorR	= (byte) pBackground->red;
		pPngOut->BgColorG	= (byte) pBackground->green;
		pPngOut->BgColorB	= (byte) pBackground->blue;
	}

	// if required set gamma conversion
	if (png_get_gAMA(png_ptr, info_ptr, &dGamma))
		png_set_gamma(png_ptr, (double) 2.2, dGamma);

	// after the transformations have been registered update info_ptr data
	png_read_update_info(png_ptr, info_ptr);

	// get again width, height and the new bit-depth and color-type
	png_get_IHDR(png_ptr
		, info_ptr
		, &png_w
		, &png_h
		, &png_b
		, &iColorType
		, NULL, NULL, NULL);


	// row_bytes is the width x number of channels
	ulRowBytes = png_get_rowbytes(png_ptr, info_ptr);
	png_d = png_get_channels(png_ptr, info_ptr);


	// now we can allocate memory to store the image
	png_p = (png_byte*)malloc(ulRowBytes * png_h * sizeof(png_byte));
	if(!png_p)
		goto LOAD_IMAGE_ERROR;


	// and allocate memory for an array of row-pointers
	ppbRowPointers = (png_bytepp)malloc(png_h * sizeof(png_bytep));

	if( NULL == ppbRowPointers)
		goto LOAD_IMAGE_ERROR;




	// set the individual row-pointers to point at the correct offsets
	for(unsigned i = 0; i < png_h; i++)
		ppbRowPointers[i] = png_p + i * ulRowBytes;


	// now we can go ahead and just read the whole image
	png_read_image(png_ptr, ppbRowPointers);

	// read the additional chunks in the PNG file (not really needed)
	png_read_end(png_ptr, NULL);

	// yepp, done
	fclose (fp);


	// and we're done
	SAFE_FREE(	ppbRowPointers	);

	// free
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);


	// Setup Output Data
	int dst_w = ((png_w + 3L) >> 2) << 2;
	if(dst_w == png_w)
	{
		pPngOut->nWidth = png_w;
		pPngOut->nHeight= png_h;
		pPngOut->nChannel= png_d;
		pPngOut->pPixel = png_p;
		return 0;
	}

	png_byte* dst_p = (png_byte*)malloc(dst_w * png_d * png_h);
	memset(dst_p, 0xFF, dst_w * png_d * png_h);

	for(int j=0; j<png_h; ++j)
	{
		int idx_src = j * png_w * png_d;
		int idx_dst = j * dst_w * png_d;

		memcpy(&dst_p[idx_dst], &png_p[idx_src], png_w * png_d);

		png_byte* xx_p = &dst_p[idx_dst + png_w * png_d];

		if(4 == png_d)
		{
			for(int i=0; i<dst_w-png_w; ++i)
			{
				//xx_p[i * png_d + 0] = 0;		// red
				//xx_p[i * png_d + 1] = 0;		// green
				//xx_p[i * png_d + 2] = 0;		// blue
				xx_p[i * png_d + 3] = 0xFF;
			}
		}
	}
	free(png_p);

	pPngOut->nWidth = dst_w;
	pPngOut->nHeight= png_h;
	pPngOut->nChannel= png_d;
	pPngOut->pPixel = dst_p;
	return 0;

LOAD_IMAGE_ERROR:

	fclose(fp);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	SAFE_FREE(	ppbRowPointers	);

	return -1;
}











#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif
