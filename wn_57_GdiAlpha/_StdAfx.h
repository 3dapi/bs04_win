#ifndef __STDAFX_H_
#define __STDAFX_H_


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

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

#include "resource.h"


#define SAFE_DESTROY_WINDOW(p)	{if(p){	DestroyWindow(p);(p)=NULL;	}}
#define SAFE_FREE(p)			{if(p){	free(p);	(p) = NULL;		}}


#include "Application.h"

#endif