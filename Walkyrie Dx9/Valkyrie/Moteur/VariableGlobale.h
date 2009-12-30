#define WIN32_LEAN_AND_MEAN

#ifndef _VARIABLEGLOBAL_
#define _VARIABLEGLOBAL_

#include <Windows.h>
#include <windowsx.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include <mmsystem.h> // fonction time (winmm.lib)
#include "MesureTemps.h"
#include "ImageTGA.h"
#include "resource.h"

#include "Console.h"
extern CConsole Console;


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

typedef short unsigned int  shortindex;

#endif //_VARIABLEGLOBAL_