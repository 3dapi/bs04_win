

#include "_StdAfx.h"


CMain* g_pApp  = NULL;


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CMain d3dApp;

	d3dApp.m_hInst =hInst;
	g_pApp  = &d3dApp;

	if( FAILED( d3dApp.Create( hInst ) ) )
		return 0;

	return d3dApp.Run();
}




LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static	WPARAM		wparHi;
	static	WPARAM		wparLo;
	static	HWND		hw;

	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);
	hw = (HWND)lParam;

	switch( msg )
	{
	case WM_PAINT:
		{
			HDC hDC = GetDC( hWnd );
			TCHAR strMsg[MAX_PATH];
			wsprintf( strMsg, TEXT("Right screen") );
			RECT rct;
			GetClientRect( hWnd, &rct );
			DrawText( hDC, strMsg, -1, &rct, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
			ReleaseDC( hWnd, hDC );

			break;
		}


	case WM_COMMAND:
		{
			switch(wparLo)
			{
				case ID_COLOR_RED:
					break;

				case ID_COLOR_GREEN:
					break;


				case IDM_COLOR_BLUE:
					break;
			}


			break;
		}

	}

	return CApplication::MsgProc( hWnd, msg, wParam, lParam );
}



INT_PTR CALLBACK WndAbout(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi;
	WPARAM		wparLo;

	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);

	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			return TRUE;
		}


		case WM_COMMAND:
		{
			switch(wparLo)
			{
			case IDOK:
				{
					SendMessage(hWnd, WM_CLOSE, 0,0);
					break;
				}
			}

			break;
		}


		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			break;
		}


	}

	return(FALSE);
}