// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CApplication*	g_pApp;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CApplication	app;

	g_pApp = &app;

	if(FAILED(app.Create(hInst)))
		return -1;

	return app.Run();
}