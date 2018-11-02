// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_hwImgSc	= NULL;
}


HRESULT CMain::Init()
{
	SendMessage( m_hWnd, WM_PAINT, 0, 0 );

	m_hwImgSc = CreateDialog(GHINST, MAKEINTRESOURCE(IDD_IMGSRC), m_hWnd, WndImgSrc);
	ShowWindow(m_hwImgSc, SW_SHOW);

	SetFocus(m_hWnd);

	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_DESTROY_WINDOW(m_hwImgSc);

	return S_OK;
}


void CMain::Render()
{
	HDC hdc;

	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;

	HWND	hWnd = GetDlgItem(m_hwImgSc, IDC_TX);
	hdc=GetDC(hWnd);

	MemDC=CreateCompatibleDC(hdc);
	MyBitmap=LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);

	StretchBlt(hdc,5,5,240,240,MemDC,0,0,255,255,SRCCOPY);
	SelectObject(MemDC,OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);

	ReleaseDC(m_hwImgSc, hdc);


	hWnd = GetDlgItem(m_hwImgSc, IDC_BUTTON1);
	hdc=GetDC(hWnd);

	MemDC=CreateCompatibleDC(hdc);
	MyBitmap=LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);

	StretchBlt(hdc,5,5,120,120,MemDC,0,0,255,255,SRCCOPY);
	SelectObject(MemDC,OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);

	ReleaseDC(m_hwImgSc, hdc);


}