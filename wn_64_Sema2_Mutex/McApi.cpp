// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CApplication*	g_pApp;

HANDLE		g_hSmph		= NULL;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	g_hSmph=CreateSemaphore(NULL, 0, 1, EXC_MTX);


	CApplication	app;

	g_pApp = &app;

	if(FAILED(app.Create(hInst)))
		return -1;




	return app.Run();
}