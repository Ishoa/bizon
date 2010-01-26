#include "Utils.h"


// pour le timer haute-resolution (calcul du FPS)
double    tstart, tstop, tcorr, tfreq;

// variable qui contient timeGetTime() à chaque render
UINT UItime;


////////////////////////////////////////////////////////
// pour le calcul du FPS en haute resolution
////////////////////////////////////////////////////////
double DXOutils::GetFrequency(void) { 
	LARGE_INTEGER timer;
	QueryPerformanceFrequency(&timer); 
	return (double)timer.QuadPart;
}

double DXOutils::GetTimer(void) {
	LARGE_INTEGER timer;
	QueryPerformanceCounter(&timer);   
	return (double)timer.QuadPart;
}

void DXOutils::InitTimer(void) { 
	tfreq = GetFrequency(); 
	tstart = GetTimer(); 
	tstop  = GetTimer(); 
	tcorr  = tstop-tstart;
}


/////////////////////////////////////////////////////////////////////////
// affichage du FPS
/////////////////////////////////////////////////////////////////////////
#define NREFRESH  100  // nombre de millisecondes entre 2 rafraichissements du FPS
double DXOutils::GetFPS() {
	static UINT  ltime=0;
	static int   nframes=0;
	static double cur_fps=0;

	// calcul du FPS
	nframes++;
	if (UItime - ltime > NREFRESH)
	{
		tstop = GetTimer();
		cur_fps = nframes / ( (tstop - tstart - tcorr) / tfreq );

		nframes = 0;
		ltime = UItime;
		tstart = GetTimer();



	}
	//if (ltime>0) 
	return cur_fps;
}
