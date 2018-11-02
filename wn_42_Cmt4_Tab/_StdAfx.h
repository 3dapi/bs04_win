#ifndef __STDAFX_H_
#define __STDAFX_H_


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <Mmsystem.h>

#include <commctrl.h>

#define SAFE_DESTROY_WINDOW(p)	{if(p){	DestroyWindow(p);(p)=NULL;	}}

#include "resource.h"


#define GHINST					g_pApp->m_hInst
#define GHWND					g_pApp->m_hWnd
#define GMAIN					g_pApp



#include "Application.h"

extern CApplication*	g_pApp;
#endif