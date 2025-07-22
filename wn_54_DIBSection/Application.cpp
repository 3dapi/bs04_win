// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"


static CApplication*	g_pD3DApp;



HBITMAP MakeDIBSection(BYTE** pOut, char* sFile)
{
	FILE*		fp;
	LONG		lFile=0;
	HBITMAP		hBit;


	BITMAPFILEHEADER	fh		= {0};
	BITMAPINFOHEADER	ih		= {0};
	BITMAPINFO*			bi		= NULL;
	BYTE*				pPixel	= NULL;
	long				lSize	= 0;

	//	lSize = sizeof(BITMAPINFO);
	//	lSize = sizeof(BITMAPINFOHEADER);


	fp= fopen(sFile, "rb");

	fseek(fp, 0, SEEK_END);
	lFile = ftell(fp);
	fseek(fp, 0, SEEK_SET);


	fread(&fh, sizeof(BITMAPFILEHEADER), 1, fp);
//	fread(&ih, sizeof(BITMAPINFOHEADER), 1, fp);

	lSize = fh.bfOffBits- sizeof(BITMAPFILEHEADER);

	bi = (BITMAPINFO*)malloc(lSize*4);
	memset(bi, 0, lSize*4);

	fread(bi, lSize, 1, fp);

	// DIB 섹션을 만들고 버퍼 메모리를 할당한다.
	HDC hDC = GetDC(NULL);
	hBit=CreateDIBSection(hDC, bi, DIB_RGB_COLORS, (void**)&pPixel,NULL,0);
	ReleaseDC(NULL, hDC);

	long lReadable = lFile - (fh.bfOffBits- sizeof(BITMAPFILEHEADER) );
	long lRemain   = fh.bfSize - fh.bfOffBits;

	lSize = lReadable<lRemain ? lReadable : lRemain ;

	// 4의 배수로 맞춘다.
	lSize >>=2;
	lSize <<=2;

	fread(pPixel, lSize, 1, fp);
	fclose(fp);

	free(bi);

	*pOut = pPixel;

	return hBit;
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

	memset(m_sBmp, 0, sizeof(m_sBmp));
	m_hBit		= NULL;
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
	::ShowCursor(m_bShowCursor);


	return S_OK;
}


void CApplication::Cleanup()
{
	if(m_hBit)
	{
		DeleteObject(m_hBit);
		m_hBit = NULL;
	}

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
						if(m_hBit)
						{
							DeleteObject(m_hBit);
							m_hBit = NULL;
						}

						m_hBit = MakeDIBSection(&m_pPixel, m_sBmp);
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



void CApplication::Render()
{
	if(!m_hWnd)
		return;

	HDC hdc = GetDC(m_hWnd);

	if(m_hBit)
	{
		HDC MemDC;
		HBITMAP OldBitmap;
		int bx,by;
		BITMAP bit;

		BOOL hr;

		MemDC=CreateCompatibleDC(hdc);
		OldBitmap=(HBITMAP)SelectObject(MemDC, m_hBit);

		GetObject(m_hBit,sizeof(BITMAP),&bit);
		bx=bit.bmWidth;
		by=bit.bmHeight;

		hr = BitBlt(hdc, 100, 50,bx+100,by+50,MemDC,0,0,SRCCOPY);

		SelectObject(MemDC,OldBitmap);
		DeleteDC(MemDC);
	}


	ReleaseDC(m_hWnd, hdc);
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif