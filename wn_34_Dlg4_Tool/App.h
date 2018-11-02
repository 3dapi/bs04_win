// File: App.h
// Desc:
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

class CApplication
{
public:
	HINSTANCE		m_hInst;											// Hinstance
	HWND			m_hWnd;												// The main app window
	HDC				m_hDC;
	HMENU			m_hMenu;											// App menu bar (stored here when fullscreen)

	DWORD			m_dwWindowStyle;									// Saved window style for mode switches
	RECT			m_rcWindowBounds;									// Saved window bounds for mode switches
	RECT			m_rcWindowClient;									// Saved client area size for mode switches

	TCHAR*			m_strWindowTitle;									// Title for the app's window
	DWORD			m_dwCreationWidth;									// Width used to create window
	DWORD			m_dwCreationHeight;									// Height used to create window

	CApplication();
	HRESULT			Create( HINSTANCE);
	INT				Run();
	virtual LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);

	virtual HRESULT Init()				{ return S_OK; }
	virtual HRESULT Destroy()			{ return S_OK; }

	virtual void	Render()		{}
};

#endif