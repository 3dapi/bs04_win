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


	HWND		m_hWndBtn1;
	HWND		m_hWndBtn2;

	HWND		m_hWndCheck1;
	HWND		m_hWndCheck2;

	HWND		m_hWndCombo;

public:
	CApplication();

	//Window
	INT		Create(HINSTANCE hInst);
	INT		Run();
	void	Cleanup();

	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

protected:
};

#endif