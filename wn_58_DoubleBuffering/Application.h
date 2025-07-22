// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

class GdiSprite;
struct FrameBuffer
{
	HDC			dc		{};
	HBITMAP		bmp		{};
	HBITMAP		bmpOld	{};
	SIZE		size	{};
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
	bool		m_bShowCursor;			// Show Cursor
	
protected:
	HBRUSH		m_hClearBrush	{};
	FrameBuffer	m_backBuffer	{};
	GdiSprite*	m_spriteMario	{};

	ULONGLONG	m_timeCur		{};
	ULONGLONG	m_timeOld		{};
	float		m_timeDelta		{};
	float		m_timeTick		{};
	
public:
	CApplication();
	
	//Window
	INT		Create(HINSTANCE hInst);
	INT		Run();
	void	Cleanup();
	
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

protected:
	void CreateFrameBuffer(HDC hdc, INT width, INT height);
	void ReleaseFrameBuffer();
	void ClearBackBuffer();
	void PresentToFrontBuffer(HDC hdc);
};

#endif