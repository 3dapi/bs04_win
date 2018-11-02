// WinApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"


CApplication*	g_pApp=NULL;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
 	CApplication	mainApp;

	g_pApp = &mainApp;

	mainApp.Create(hInstance);

	return mainApp.Run();
}