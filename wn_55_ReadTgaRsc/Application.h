// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_


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
	char		m_sTga[512];
	BYTE*		m_pPixel;
//	TGAHEADER	m_Tga;

	BYTE*		m_ImgId;
	BYTE		m_ImgType;
	SHORT		m_ImgWidth;
	SHORT		m_ImgHeight;
	BYTE		m_ImgBit;
	BYTE*		m_ImgMap;

//	struct TGAHEADER
//{
//	BYTE	imageTypeCode;
//	SHORT	imageWidth;
//	SHORT	imageHeight;
//	BYTE	bitCount;
//};


public:
	void	Render();
};


#endif