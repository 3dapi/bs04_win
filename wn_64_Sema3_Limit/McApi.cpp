// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CApplication*	g_pApp;

HANDLE		g_hSmph		= NULL;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	g_hSmph = OpenSemaphore(SYNCHRONIZE, FALSE, EXC_MTX);

	if(g_hSmph)
	{
		// 동일한 프로그램이 실행되고 있는지 확인
		MessageBox(0, "중복실행", "Error", 0);
	}


	g_hSmph=CreateSemaphore(NULL, 4, 10, EXC_MTX);

	if (WaitForSingleObject(g_hSmph,0) == WAIT_TIMEOUT)
	{
		MessageBox(0, "4개 초과 중복실행", "Error", 0);
		CloseHandle(g_hSmph);

		return -1;
	}



	CApplication	app;

	g_pApp = &app;

	if(FAILED(app.Create(hInst)))
		return -1;




	return app.Run();
}