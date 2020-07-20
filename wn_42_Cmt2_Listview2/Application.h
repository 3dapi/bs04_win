// Interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

class CApplication
{
public:
	HINSTANCE	m_hInst		{};

	HWND		m_hList		{};
	HWND		m_hMale		{};
	HIMAGELIST	m_hImgSm	{};
	HIMAGELIST	m_hImgLa	{};

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