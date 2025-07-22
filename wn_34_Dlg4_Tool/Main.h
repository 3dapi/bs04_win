// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

#define SAFE_DESTROY_WINDOW(p)	{if(p)DestroyWindow(p);	}


INT_PTR CALLBACK WndImgSrc	(HWND, UINT, WPARAM, LPARAM);


class CMain : public CApplication
{
public:
	HWND		m_hwImgSc;

public:
	CMain();
    virtual HRESULT Init();
    virtual HRESULT Destroy();
	virtual LRESULT	MsgProc(HWND,UINT,WPARAM,LPARAM);

	virtual void	Render();
};

#endif