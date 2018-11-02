// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CApplication*	g_pApp;

HANDLE		g_hMutex		= NULL;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	g_hMutex = OpenMutex(SYNCHRONIZE, FALSE, EXC_MTX);							// 동일한 프로그램이 실행되고 있는지 확인

	if(g_hMutex)
	{
		MessageBox(0, "중복실행", "Error", 0);
		return -1;
	}

	g_hMutex = CreateMutex(NULL, FALSE, EXC_MTX);

	if (GetLastError() == ERROR_ALREADY_EXISTS)									//여기서도 걸린다.
	{
		MessageBox(0, "중복실행", "Error", 0);
		return -1;
	}



	CApplication	app;

	g_pApp = &app;

	if(FAILED(app.Create(hInst)))
		return -1;




	return app.Run();
}