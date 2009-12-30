// Classe pour la gestion d'une caméra
#pragma once
#include "CameraPremierePersonne.h"
#include "EnvCollision.h"
#include "Terrain.h"

class CScene;


class CCameraPremierePersonneTerrain : public CCameraPremierePersonne
{
	CEnvCollision* m_pEnvCollision;
	CTerrain* m_pTerrain;

public:

	CCameraPremierePersonneTerrain();
	CCameraPremierePersonneTerrain(CScene* pScene,CTerrain* pTerrain ,CEnvCollision* pEnvCollision = NULL, float fHauteur = 1.0f);
	~CCameraPremierePersonneTerrain();

	void Animation(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick, float DeltaTemps);
	virtual D3DXVECTOR3 CheckPosition(D3DXVECTOR3, D3DXVECTOR3);
};
