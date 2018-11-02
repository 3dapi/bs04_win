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
	virtual ~CApplication();
	
	//Window
	INT		Create(HINSTANCE hInst);
	INT		Run();
	void	Cleanup();
	
public:

	// WndProc���� �ʿ��� static ����
	static CApplication* g_pApp;
	static CApplication* GetAppInstance();
	static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

	// �޽��� ó���Լ�
	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
};

#endif

