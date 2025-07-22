
#include "_StdAfx.h"

INT_PTR CALLBACK WndImgSrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM	wparHi;
	WPARAM	wparLo;

	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);

	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			RECT rt1;
			RECT rt2;

			INT iWidth;
			INT iHeight;
			INT iX;
			INT iY;

			GetWindowRect(GMAIN->m_hWnd, &rt1);
			GetWindowRect(hWnd, &rt2);

			iWidth = rt2.right - rt2.left;
			iHeight= rt2.bottom- rt2.top;
			iX = rt1.left - iWidth;
			iY = rt1.top;

			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);

			break;
		}


		case WM_PAINT:
		{
			break;
		}

		case WM_COMMAND:
		{

			break;
		}

		case WM_CLOSE:
		{
			break;
		}
	}

	return(FALSE);
}