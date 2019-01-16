// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

class CApplication
{
private:
	CApplication();
public:
	TCHAR		m_sCls[128]	;
	HINSTANCE	m_hInst		;
	HWND		m_hWnd		;
	DWORD		m_dWinStyle	;
	DWORD		m_dScnX		;			// Screen Width
	DWORD		m_dScnY		;			// Screen Height

	bool		m_bShowCusor;			// Show Cusor
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

