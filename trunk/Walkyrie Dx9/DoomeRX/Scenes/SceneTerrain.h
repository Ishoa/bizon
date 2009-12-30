
// Classe pour la gestion d'une scène 3D

#pragma once
#include "Scene.h"
#include "..\MiniGames\InterfacesBattlefly.h"

#include "CameraVolLibre.h"
#include "..\Objects\CameraPremierePersonneTerrain.h"
#include "CameraVolLibre.h"

#include "MeshCollision.h"
#include "Panorama.h"

#include "Terrain.h"
#include "ModelEau.h"
#include "Vegetation.h"

class CSceneTerrain : public CScene
{
public:

	enum ESound
	{
		ANIMAUX1 = 0,
		ANIMAUX2,
		ANIMAUX3,
		ANIMAUX4,
		ANIMAUX5,
		MOTEUR_AVION
	};

	CMesh* m_pAvion;					// Avion
	CPanorama* m_pCiel;					// SkyBox
	CTerrain* m_pTerrain;				// Terrain
	CModelEau* m_pModelEau;				// ModelEau
	CVegetation* m_pVegetation;			// Vegetation
	
	CInterfaceBattleFly* m_pInterface; // Pointeur sur l'Interface

	D3DXMATRIXA16 m_MatTransAvion;		// Matrice de transformation pour l'avion 
	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	CCamera*     m_pCamera1;			// Camera au sol
	CCamera*     m_pCamera2;			// Camera sur l'aile de l'avion
	CCamera*     m_pCamera3;			// Camera derrière l'avion

	CLumiere m_Lumiere;					// Lumière pour éclairer l'avion

	float m_Rotation;					// Angle de rotation de l'avion
	float m_Px;							// Postion X par rapport à la rotation
	float m_Pz;							// Postion Z par rapport à la rotation
	int m_FiltreTexture;				// Type de filtrage de texture
	bool m_bRenduFilDeFer;				// Si rendu en fil de fer

	float m_fIntervalleAnimaux;
	float m_fIntervalleMoteur;


	CSceneTerrain(CMoteur* pMoteur);
	~CSceneTerrain();
	
	bool Initialisation();
	bool CreationObjet();
	void Destruction();
	void DestructionObjet();
	void Rendu3D();
	void RenduInterface();
	void Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick);
	void Animation(double TempsEcouler, double DeltaTemps);
	void SetFiltreTexture();
};
