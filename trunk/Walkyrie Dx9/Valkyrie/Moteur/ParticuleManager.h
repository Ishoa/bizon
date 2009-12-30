#ifndef _PARTICULEMANAGER_H_
#define _PARTICULEMANAGER_H_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Particules.h"

#define MAX_PARTICULES_TIME 50
#define MAX_PARTICULES_BILBOARDING 50

class CParticuleManager
{

	int m_nIndiceParticuleTime;
	CParticuleTime m_pTabParticuleTime[MAX_PARTICULES_TIME];

	int m_nIndiceParticuleBilboard;
	CParticuleBilboard m_pTabParticuleBilboard[MAX_PARTICULES_BILBOARDING];

public:

	CParticuleManager();
	~CParticuleManager();

	void Rendu();

	CParticuleTime* getParticuleTimeLibre();
	CParticuleBilboard* getParticuleBilboardLibre();

	void ClearParticules();;
	void ClearParticulesTime();
	void ClearParticulesBilboard();

};

#endif