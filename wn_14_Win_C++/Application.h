// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>

class CApplication
{
private:
	CApplication();
public:
	std::string	m_sCls		;
	HINSTANCE	m_hInst		;
	HWND		m_hWnd		;
	DWORD		m_dWinStyle	;
	DWORD		m_dScnX		;			// Screen Width
	DWORD		m_dScnY		;			// Screen Height

	bool		m_bShowCursor;			// Show Cursor
public:
	//Window
	INT		Create(HINSTANCE hInst);
	INT		Run();
	void	Cleanup();
	
public:
	static CApplication* GetInstance();						// singleton instance
	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);	// 메시지 처리함수
};

#endif

