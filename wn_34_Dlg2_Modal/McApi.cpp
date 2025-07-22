// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CApplication*	g_pApp;


BOOL CALLBACK InfoDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{

	CApplication	app;

	g_pApp = &app;

	if(FAILED(app.Create(hInst)))
		return -1;

	return app.Run();

	return 1;
}




BOOL CALLBACK InfoDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_INITDIALOG:
		{
			SetDlgItemText(hDlg,IDC_STR,g_pApp->m_str);
			SetDlgItemInt(hDlg,IDC_X,g_pApp->m_x,FALSE);
			SetDlgItemInt(hDlg,IDC_Y,g_pApp->m_y,FALSE);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch (wParam)
			{
				case IDOK:
				{
					GetDlgItemText(hDlg,IDC_STR, g_pApp->m_str,128);
					g_pApp->m_x=GetDlgItemInt(hDlg,IDC_X,NULL,FALSE);
					g_pApp->m_y=GetDlgItemInt(hDlg,IDC_Y,NULL,FALSE);
					EndDialog(hDlg,IDOK);

					return TRUE;
				}

				case IDCANCEL:
				{
					EndDialog(hDlg,IDCANCEL);

					return TRUE;
				}
			}

			break;
		}
	}

	return FALSE;
}



LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;

			hdc=BeginPaint(hWnd, &ps);

			TextOut(hdc,m_x,m_y,m_str,strlen(m_str));

			EndPaint(hWnd, &ps);

			break;
		}

		case WM_LBUTTONDOWN:
		{
			if (DialogBox(m_hInst,MAKEINTRESOURCE(IDD_DIALOG1),	hWnd,InfoDlgProc)==IDOK)
				InvalidateRect(hWnd, NULL, TRUE);

			MessageBox(hWnd, "Modal Dialog Finished", "Msg", 0);

			return 0;
		}

		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				case VK_ESCAPE:
				{
					SendMessage(hWnd, WM_DESTROY, 0,0);
					break;
				}
			}

			return 0;
		}

		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}