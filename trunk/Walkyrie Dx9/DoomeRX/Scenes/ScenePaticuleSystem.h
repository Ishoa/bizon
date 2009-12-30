#pragma once
#include "..\Moteur\Scene.h"
#include "CameraVolLibre.h"
#include "Sprites3D.h"
#include "InterfaceParticule.h"
#include "FuseeArtifice.h"
#include "FuseeMissileExplode.h"
#include "JetDeau.h"
#include "Geyzer.h"
#include "Fire.h"

#include "Panorama.h"

#define NBFEUXARTIFICE 3
#define NBJETDEAU 5
#define NBGEYZER 5
#define NBFIRE 2

class CSceneParticuleSystem : public CScene
{
public:


	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	int m_QualiterFiltrage;					// type de filtrage des textures

	CInterfaceParticule* m_pInterface;

	CPanorama* m_pSpace;					// SkyBox space

	CFuseeArtifice* m_pFuseeArtifice[NBFEUXARTIFICE];
	CFuseeMissileExplode* m_pFuseeExplode;
	CJetDeau* m_pJetDeau[NBJETDEAU];
	CGeyzer* m_pGeyzer[NBGEYZER];

	CFire* m_pFire[NBFIRE];

	float m_fRed,m_fBlue,m_fGreen,m_fAlpha;

	CSprite3D* m_pSprite;

	bool m_bTir;

	CSceneParticuleSystem(CMoteur* pMoteur);
	~CSceneParticuleSystem();

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
