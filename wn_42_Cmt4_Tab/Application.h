// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_


INT_PTR CALLBACK WndWrk		(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK WndWrk1	(HWND, UINT, WPARAM, LPARAM);


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

	HWND		m_hwWrk;
	HWND		m_hwWrk1;
	HWND		m_hwWrkTb;														// Base Work Tab

	HIMAGELIST	m_IL;															// Image List for Tab and List control
	INT			m_nTab;

public:
	CApplication();

	//Window
	INT		Create(HINSTANCE hInst);
	INT		Run();
	void	Cleanup();

	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
};

#endif