#ifndef _JETDEAU_H_
#define _JETDEAU_H_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Particlesystem.h"

class CJetDeau
{
protected:

	CParticleSystem* m_pJetDeau;

	LPDIRECT3DDEVICE9 m_pD3DDevice;


public:
	
	CJetDeau(LPDIRECT3DDEVICE9 pD3DDevice);
	~CJetDeau();

	bool CreateAndInit(char **g_pTextNameJetDeau , int nTailleJetDeau);
	void DestructionObjet();
	void Rendu3D();
	void Animation(float fDeltaTemps);

	void SetPositionDepart(D3DXVECTOR3 vPositionDepart){m_pJetDeau->SetPosition(vPositionDepart);}
	void SetDirectionJet(D3DXVECTOR3 vDirectionjet){m_pJetDeau->SetVelocity(vDirectionjet);}
	void SetCoeffZoneJet(float fVelocity){m_pJetDeau->SetVelocityVar(fVelocity);}
	void SetColor(D3DXCOLOR vColor){m_pJetDeau->SetColor(vColor);}

};

#endif