// Classe pour la gestion d'une caméra
#pragma once

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Scene.h"
#include "Camera.h"

class CScene;


class CCameraPremierePersonne : public CCamera
{

protected:
		float m_fHauteur;
		float m_fVariation;

public:

	CCameraPremierePersonne();
	CCameraPremierePersonne(CScene* pScene, float fhauteur = 0.0f);
	~CCameraPremierePersonne();

	virtual void SetPosition(D3DXVECTOR3* VecPosition);
	virtual void Animation(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick, float DeltaTemps);

	virtual D3DXVECTOR3 CheckPosition(D3DXVECTOR3, D3DXVECTOR3);
};
