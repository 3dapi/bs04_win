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
	void	Render();


public:
	DWORD	m_dTh[50];
	HANDLE	m_hTh[50];
	INT		m_nValue;
};


#endif