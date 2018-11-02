// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_



struct PNG_PIXEL
{
	BYTE*	pPixel;
	INT		nWidth;
	INT		nHeight;
	INT		nChannel;

	BYTE	BgColorR;
	BYTE	BgColorG;
	BYTE	BgColorB;

	PNG_PIXEL()
	{
		pPixel	= NULL;
		nWidth	= 0;
		nHeight	= 0;
		nChannel= 0;

		BgColorR= 0;
		BgColorG= 0;
		BgColorB= 0;
	}

	PNG_PIXEL(const PNG_PIXEL& r)
	{
		SAFE_FREE(	pPixel	);

		pPixel	= r.pPixel	;
		nWidth	= r.nWidth	;
		nHeight	= r.nHeight	;
		nChannel= r.nChannel;

		BgColorR= r.BgColorR;
		BgColorG= r.BgColorG;
		BgColorB= r.BgColorB;
	}

	PNG_PIXEL& operator=(const PNG_PIXEL& r)
	{
		SAFE_FREE(	pPixel	);

		pPixel	= r.pPixel	;
		nWidth	= r.nWidth	;
		nHeight	= r.nHeight	;
		nChannel= r.nChannel;

		BgColorR= r.BgColorR;
		BgColorG= r.BgColorG;
		BgColorB= r.BgColorB;

		return *this;
	}

	void Destroy()
	{
		SAFE_FREE(	pPixel	);
	}

	void SetPixelData(BYTE* _pxl)
	{
		SAFE_FREE(	pPixel	);
		pPixel	= _pxl;
	}
};




class CApplication
{
public:
	TCHAR		m_sCls[128]	;
	HINSTANCE	m_hInst		;
	HWND		m_hWnd		;
	DWORD		m_dWinStyle	;
	DWORD		m_dScnX		;			// Screen Width
	DWORD		m_dScnY		;			// Screen Height

	bool		m_bShowCusor;			// Show Cusor

public:
	CApplication();

	//Window
	INT		Create(HINSTANCE hInst);
	INT		Run();
	void	Cleanup();

	LRESULT					MsgProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT WINAPI	WndProc(HWND, UINT, WPARAM, LPARAM);

public:
	char		m_sPng[MAX_PATH];
	PNG_PIXEL	m_Png;

public:
	INT		FileOpen();
	void	Render();
};


INT LoadPngFile(PNG_PIXEL*	pPngOut, char* sFileName);
INT LoadImageFile(HWND hwnd, char* sFileName, PNG_PIXEL* pPngOut);


#endif