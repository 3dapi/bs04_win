#ifndef __STDAFX_H_
#define __STDAFX_H_


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define WINVER		0x0500

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


#include "Application.h"

#endif