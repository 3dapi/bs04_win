#ifndef _MCCOMMON_H_
#define _MCCOMMON_H_

#pragma once

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>
#include <math.h>

#include "App.h"
#include "resource.h"


#define GHINST					g_pApp->m_hInst
#define GHWND					g_pApp->m_hWnd
#define GMAIN					g_pApp





#include "Main.h"

extern CMain* g_pApp;

#endif