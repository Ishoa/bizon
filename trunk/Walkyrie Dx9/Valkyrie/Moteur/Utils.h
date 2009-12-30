#ifndef _UTILS_H_
#define _UTILS_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h> // fonction time (winmm.lib)
#include <stdio.h>

// variable qui contient timeGetTime() à chaque render
extern UINT UItime;

namespace DXOutils
{
	// prototypes
	double GetFrequency(void);
	double GetTimer(void);
	void InitTimer(void);

	double GetFPS();

}

#endif //_UTILS_H_