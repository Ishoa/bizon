#pragma once
#include "..\..\Valkyrie\Moteur\Scene.h"
#include "..\..\Valkyrie\Moteur\CameraVolLibre.h"
#include "..\..\Valkyrie\Moteur\Sprites3D.h"
#include "InterfaceParticule.h"
#include "..\..\Valkyrie\Moteur\FuseeArtifice.h"
#include "..\..\Valkyrie\Moteur\FuseeMissileExplode.h"
#include "..\..\Valkyrie\Moteur\JetDeau.h"
#include "..\..\Valkyrie\Moteur\Geyzer.h"
#include "..\..\Valkyrie\Moteur\Fire.h"

#include "..\..\Valkyrie\Moteur\Panorama.h"

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
