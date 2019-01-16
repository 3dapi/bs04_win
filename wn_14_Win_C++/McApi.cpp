// D3D Sprite toturial
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CApplication* pApp = CApplication::GetInstance();
	if(FAILED(pApp->Create(hInst)))
		return -1;

	return pApp->Run();
}