// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>

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
	std::string	m_sTga;
	BYTE*		m_pPixel;
	TGAHEADER	m_Tga;
	HBITMAP		m_bitmap{};

public:
	INT		FileOpen();
	void	OnDraw(HDC hdc);

protected:
	void CreateFromTga(const std::string& filename);
};


BYTE* LoadTGAFile(const char* filename, TGAHEADER& tgaH);


#endif