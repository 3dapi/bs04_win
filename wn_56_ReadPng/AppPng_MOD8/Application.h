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

	PNG_PIXEL()
	{
		pPixel	= NULL;
		nWidth	= 0;
		nHeight	= 0;
		nChannel= 0;
	}

	PNG_PIXEL(const PNG_PIXEL& r)
	{
		SAFE_FREE(	pPixel	);

		pPixel	= r.pPixel	;
		nWidth	= r.nWidth	;
		nHeight	= r.nHeight	;
		nChannel= r.nChannel;
	}

	PNG_PIXEL& operator=(const PNG_PIXEL& r)
	{
		SAFE_FREE(	pPixel	);

		pPixel	= r.pPixel	;
		nWidth	= r.nWidth	;
		nHeight	= r.nHeight	;
		nChannel= r.nChannel;
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

	bool		m_bShowCursor;			// Show Cursor

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


#pragma pack(push,1)
struct COLOR3
{
	unsigned char r,g,b;
	COLOR3(unsigned char _r=0, unsigned char _g=0, unsigned char _b=0):r(_r), g(_g), b(_b){}
};
struct COLOR4
{
	unsigned char r,g,b,a;
	COLOR4(unsigned char _r=0, unsigned char _g=0, unsigned char _b=0, unsigned char _a=0):r(_r), g(_g), b(_b), a(_a){}
};
#pragma pack(pop)


INT LoadPngFile(PNG_PIXEL*	pPngOut, char* sFileName);
INT LoadPngFile(unsigned char** img_p, int* img_w, int* img_h, int* img_d, const char* sFileName);
INT LoadImageFile(HWND hwnd, char* sFileName, PNG_PIXEL* pPngOut);
INT SavePngToFile(const char* sFileName, unsigned char* img_p, int img_w, int img_h, int img_d);


#endif