// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;



BYTE* LoadDIB(char* sFile)
{
	FILE*	fp;
	long	lSize;
	BYTE*	pData = NULL;

	fp= fopen(sFile, "rb");

	fseek(fp, 0, SEEK_END);
	lSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	pData =(BYTE*)malloc(lSize);

	fread(pData, sizeof(BYTE), lSize, fp);
	fclose(fp);

	return pData;
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

	memset(m_sBmp, 0, sizeof(m_sBmp));
	m_pData		= NULL;
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
//			HDC hdc = GetDC(m_hWnd);
//			Render(hdc);
//			ReleaseDC(m_hWnd, hdc);
		}
	}

	UnregisterClass( m_sCls, m_hInst);

	return 1;
}





LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			Render(hdc);

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
						SAFE_FREE(m_pData);

						m_pData = LoadDIB(m_sBmp);
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
	OFN.lpstrFilter	="Bitmap Files(*.bmp)\0*.bmp\0All Files(*.*)\0*.*\0";
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

	strcpy(m_sBmp, OFN.lpstrFile);

	return 1;

}



void CApplication::Render(HDC hdc)
{
	if (m_pData)
	{
//		BITMAPFILEHEADER*	fh		= (BITMAPFILEHEADER *)m_pData;
//		BITMAPINFOHEADER*	ih		= (BITMAPINFOHEADER *)(m_pData + sizeof(BITMAPFILEHEADER));
//		int					bx		= ih->biWidth;
//		int					by		= ih->biHeight;
//		BYTE*				pPixel	= m_pData + fh->bfOffBits;

		INT					hr		= 0;
		BITMAPFILEHEADER	fh		= {0};
		BITMAPINFOHEADER	ih		= {0};
		int					bx		= 0;
		int					by		= 0;
		BYTE*				pPixel	= NULL;


		CopyMemory(&fh, m_pData								, sizeof fh );
		CopyMemory(&ih, m_pData + sizeof(BITMAPFILEHEADER)	, sizeof ih );
		bx		= ih.biWidth;
		by		= ih.biHeight;
		pPixel	= m_pData + fh.bfOffBits;


		hr = SetDIBitsToDevice(
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
								,(BITMAPINFO *)&ih		// bitmap information
								, DIB_RGB_COLORS		// RGB or palette indexes
								);


		hr = StretchDIBits(
								hdc						// handle to DC
								,150					// x-coord of destination upper-left corner
								,150					// y-coord of destination upper-left corner
								,bx*2					// width of destination rectangle
								,by*2					// height of destination rectangle
								,0						// x-coord of source upper-left corner
								,0						// y-coord of source upper-left corner
								,bx						// width of source rectangle
								,by						// height of source rectangle
								,pPixel					// bitmap bits
								,(BITMAPINFO *)&ih		// bitmap data
								,DIB_RGB_COLORS			// usage options
								,SRCCOPY				// raster operation code
								);
	}
}





#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif