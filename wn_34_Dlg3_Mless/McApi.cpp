// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CApplication*	g_pApp;


BOOL CALLBACK MlessDlgProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam);


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{

	CApplication	app;

	g_pApp = &app;

	if(FAILED(app.Create(hInst)))
		return -1;

	return app.Run();

	return 1;
}




BOOL CALLBACK MlessDlgProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
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

					InvalidateRect(g_pApp->m_hWnd,NULL,TRUE);

					return TRUE;
				}

				case IDCANCEL:
				{
					DestroyWindow(hDlg);
					hDlg=NULL;
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
			if (!m_hWndDlg && !IsWindow(m_hWndDlg))
			{
				m_hWndDlg=CreateDialog(m_hInst,MAKEINTRESOURCE(IDD_DIALOG1),
					hWnd,MlessDlgProc);

				ShowWindow(m_hWndDlg,SW_SHOW);
			}

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