#ifndef _CFIRE_H_
#define _CFIRE_H_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Particlesystem.h"

class CFire
{
protected:

	CParticleSystem* m_pFire;

	LPDIRECT3DDEVICE9 m_pD3DDevice;


public:

	CFire(LPDIRECT3DDEVICE9 pD3DDevice);
	~CFire();

	bool CreateAndInit(char **g_pTextNameFire , int nTailleFire,char **g_pTextNameSmoke , int nTailleSmoke);
	void DestructionObjet();
	void Rendu3D();
	void Animation(float fDeltaTemps);

	void SetPositionDepart(D3DXVECTOR3 vPositionDepart){m_pFire->SetPosition(vPositionDepart);}
	
};

#endif