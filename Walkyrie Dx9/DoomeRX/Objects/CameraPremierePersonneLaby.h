// Classe pour la gestion d'une caméra
#pragma once
#include "CameraPremierePersonne.h"
#include "EnvCollision.h"

class CScene;


class CCameraPremierePersonneLaby : public CCameraPremierePersonne
{
	CEnvCollision* m_pEnvCollision;

public:

	CCameraPremierePersonneLaby();
	CCameraPremierePersonneLaby(CScene* pScene,CEnvCollision* pEnvCollision, float fHauteur = 1.0f);
	~CCameraPremierePersonneLaby();

	virtual D3DXVECTOR3 CheckPosition(D3DXVECTOR3, D3DXVECTOR3);
};
