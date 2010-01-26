#pragma once
#include "..\..\Valkyrie\Moteur\Scene.h"

#include "..\..\Valkyrie\Moteur\VariableGlobale.h"
#include "..\..\Valkyrie\Moteur\VariableGlobalDX.h"

#include "..\..\Valkyrie\Moteur\Interface.h"
#include "..\..\Valkyrie\Moteur\Bilboarding.h"
#include "..\..\Valkyrie\Moteur\Sprites3D.h"
#include "..\..\Valkyrie\Moteur\CameraVolLibre.h"
#include "..\..\Valkyrie\Moteur\Mesh.h"

class CSceneTest : public CScene
{
public:


	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	int m_QualiterFiltrage;					// type de filtrage des textures

	CMesh*  m_pMesh;
	CBilboarding* m_pBilboarding;
	CSprite3D* m_pSprite;
	CInterface* m_pInterface;

	bool m_bAffichageLowDetail;

	CSceneTest(CMoteur* pMoteur);
	~CSceneTest();

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
