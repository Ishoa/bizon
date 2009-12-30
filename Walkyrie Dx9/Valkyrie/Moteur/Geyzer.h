#ifndef _GEYZER_H_
#define _GEYZER_H_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Particlesystem.h"

class CGeyzer
{
protected:

	CParticleSystem* m_pGeyzer;

	LPDIRECT3DDEVICE9 m_pD3DDevice;

	float m_fTimeBeforeLancement;
	float m_fCurrentTime;


public:

	CGeyzer(LPDIRECT3DDEVICE9 pD3DDevice);
	~CGeyzer();

	bool CreateAndInit(char **g_pTextNameGeyzer , int nTailleGeyzer);
	void DestructionObjet();
	void Rendu3D();
	void Animation(float fDeltaTemps);

	void SetPositionDepart(D3DXVECTOR3 vPositionDepart){m_pGeyzer->SetPosition(vPositionDepart);}
	void SetDirectionJet(D3DXVECTOR3 vDirectionjet){m_pGeyzer->SetVelocity(vDirectionjet);}
	void SetCoeffZoneJet(float fVelocity){m_pGeyzer->SetVelocityVar(fVelocity);}
	void SetColor(D3DXCOLOR vColor){m_pGeyzer->SetColor(vColor);}
	void SetTimeBeforeLancement(float fTime){m_fTimeBeforeLancement = fTime;}
};

#endif