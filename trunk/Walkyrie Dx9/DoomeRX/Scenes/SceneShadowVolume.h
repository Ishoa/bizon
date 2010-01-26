#pragma once
#include "..\..\Valkyrie\Moteur\Scene.h"
#include "..\..\Valkyrie\Moteur\Interface.h"
#include "..\..\Valkyrie\Moteur\ShadowVolumeManager.h"
#include "..\..\Valkyrie\Moteur\Sprites3D.h"


class CSceneShadowVolume : public CScene
{
public:


	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	int m_QualiterFiltrage;					// type de filtrage des textures

	CInterface* m_pInterface;

	CMesh* m_pTeapot;
	CMesh* m_pAvion;
	CMesh* m_pArche;
	CMesh* m_pShape;
	CMesh* m_pTest;
	CSprite3D* m_pSprite;

	CLumiere m_Lumiere;					// Lumière pour éclairer

	bool m_bPause;

	LPD3DXMESH m_pSphereMesh;

	bool m_bRenduShadowVolume;
	bool m_bShadowManagerLight0;
	bool m_bShadowManagerLight1;

	bool m_bWireFrame;
	float m_fAngleLum;

	CMaterielle m_mat;

	CShadowVolumeManager* m_CShadowVolumeManagerLight0;
	CShadowVolumeManager* m_CShadowVolumeManagerLight1;

	CSceneShadowVolume(CMoteur* pMoteur);
	~CSceneShadowVolume();

	bool ConfirmeFonction3D(D3DCAPS9* pCaps);
	bool Initialisation();
	bool CreationObjet();
	void DestructionObjet();
	void DebutRendu3D();
	void Rendu3D();
	void Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick);
	void Animation(double TempsEcouler, double DeltaTemps);
	void Destruction(); 
	void SetFiltreTexture(int Type);
};
