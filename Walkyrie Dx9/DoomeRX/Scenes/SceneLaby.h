#pragma once
#include "..\Moteur\Scene.h"
#include "CameraVolLibre.h"
#include "CameraTrackBall.h"
#include "..\Objects\CameraPremierePersonneLaby.h"
#include "InterfacesLaby.h"
#include "MapDeMur.h"
#include "Panorama.h"
#include "MeshCollision.h"

class CSceneLaby : public CScene
{
public:

	enum ESound
	{
		PREPARE_TO_FIGHT = 0,
		STEP,
		ATMOSPHERE,
		FIRE,
		RELOAD,
		GRENADE,
		EXPLOSION

	};

	CMeshCollision *m_pMesh;
	CMeshCollision *m_pMesh2;
	CMeshCollision *m_pMesh3;

	CMapDeMur* m_pMapDeMur;
	CSprite3D* m_pSol;
	CSprite3D* m_pPlafond;

	CPanorama* m_pSpace;
	CEnvCollision * m_pEnv;

	CLumiere m_Lumiere;					// Lumière pour éclairer
	CLumiere m_Lumiere2;					// Lumière pour éclairer

	CMaterielle m_Materielle;

	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	int m_QualiterFiltrage;					// type de filtrage des textures

	CInterfaceLaby* m_pInterface; // Pointeur sur l'Interface

	float m_fIntervallePas;
	float m_fIntervalleShoot;

	bool m_bAffichageLowDetail;

	CSceneLaby(CMoteur* pMoteur);
	~CSceneLaby();

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
