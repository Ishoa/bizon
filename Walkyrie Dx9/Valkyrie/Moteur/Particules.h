#ifndef _PARTICULE_H_
#define _PARTICULE_H_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

class CParticule;

/*
void (*ProcessFonction)(CParticule* part);
void (*ReleaseFonction)(CParticule* part);
void (*InitFonction)(CParticule* part);
*/

class CParticule
{

/*
	ProcessFonction m_pProcessFonction;
	ReleaseFonction m_ReleaseFonction;
	InitFonction m_InitFonction;
*/

public:
	CParticule();
	~CParticule();

};

class CParticuleTime : public CParticule
{
protected:

	float m_fCurrentTime;
	float m_fLife;
	float m_fPourcentLife;

public:

	CParticuleTime();
	~CParticuleTime();
};


class CParticuleBilboard : public CParticuleTime
{
	
public:
	CParticuleBilboard();
	~CParticuleBilboard();
};



#endif
