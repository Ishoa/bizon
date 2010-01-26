#pragma once
#include "..\..\Valkyrie\Moteur\Scene.h"
#include "..\..\Valkyrie\Moteur\Interface.h"

class CSceneTemplate : public CScene
{
public:
	

	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	int m_QualiterFiltrage;					// type de filtrage des textures

	CInterface* m_pInterface;

	CSceneTemplate(CMoteur* pMoteur);
	~CSceneTemplate();
	
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
