// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


BOOL CALLBACK MainDlgProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch( msg )
	{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON1:
					MessageBox(hDlg, "You Pressed Button 1", "Msg", 0);
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					return TRUE;
			}

			return FALSE;
		}

		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return FALSE;
}


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
}