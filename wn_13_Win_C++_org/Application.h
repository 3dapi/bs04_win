// Application.h: interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_


class CApplication
{
public:
	HINSTANCE	m_hInst;														// current instance
	HWND		m_hWnd;															// Window Handle
	TCHAR		m_sCls[128];													// The title bar text

	INT			m_iScnW;														// Window Width
	INT			m_iScnH;														// Window Height

public:
	CApplication();
	virtual ~CApplication();

	INT		Create(HINSTANCE hInst);
	INT		Run();

	LRESULT					MsgProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
};

#endif