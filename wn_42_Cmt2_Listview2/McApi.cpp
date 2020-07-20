// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

CApplication*	g_pApp;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CApplication	app;

	g_pApp = &app;

	if(FAILED(app.Create(hInst)))
		return -1;

	return app.Run();
}