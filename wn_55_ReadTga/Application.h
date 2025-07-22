// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_



struct TGAHEADER
{
	BYTE	imageTypeCode;
	SHORT	imageWidth;
	SHORT	imageHeight;
	BYTE	bitCount;
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
	char		m_sTga[512];
	BYTE*		m_pPixel;
	TGAHEADER	m_Tga;

public:
	INT		FileOpen();
	void	Render();
};


BYTE*		LoadTGAFile(char *filename, TGAHEADER& tgaH);


#endif