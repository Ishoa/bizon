#pragma once
#include "Scene.h"
#include "Boundingsphere.h"
#include "Boundingbox.h"
#include "Sprites3D.h"
#include "MeshCollision.h"
#include "Interface.h"


class CSceneTeapot : public CScene
{
public:
	
	CMesh *m_pTeapot;
	CSprite3D* m_pSprite;

	CLumiere m_Lumiere;					// Lumière pour éclairer


	CInterface* m_pInterface;

	int m_QualiterFiltrage;			    // type de filtrage des textures

	CSceneTeapot(CMoteur* pMoteur);
	~CSceneTeapot();
	
	bool ConfirmeFonction3D(D3DCAPS9* pCaps);
	bool Initialisation();
	bool CreationObjet();
	void DestructionObjet();
	void Rendu3D();
	void Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick);
	void Animation(double TempsEcouler, double DeltaTemps);
	void Destruction(); 
	void SetFiltreTexture(int Type);
};
