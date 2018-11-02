#ifndef __STDAFX_H_
#define __STDAFX_H_


#ifdef NDEBUG
#pragma comment(lib, "../lib/zlib123.lib")
#pragma comment(lib, "../lib/png124.lib")
#else
#pragma comment(lib, "../lib/zlib123_.lib")
#pragma comment(lib, "../lib/png124_.lib")
#endif



//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// application includes
#include "../include/png.h"

#include "resource.h"


#define SAFE_DESTROY_WINDOW(p)	{if(p){	DestroyWindow(p);(p)=NULL;	}}
#define SAFE_FREE(p)			{if(p){	free(p);	(p) = NULL;		}}


#include "Application.h"

#endif