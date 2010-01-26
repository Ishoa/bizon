// Classe pour la gestion d'une scène 3D

#pragma once
#include "..\..\Valkyrie\Moteur\Scene.h"

class CSceneBattleFly : public CScene
{
public:
	CMesh* m_pAvion;					// Avion
	CMesh* m_pCiel;					// SkyBox
	D3DXMATRIXA16 m_MatTransAvion;		// Matrice de transformation pour l'avion 
	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	CCamera*     m_pCamera;			// Camera 

	CLumiere m_Lumiere;					// Lumière pour éclairer l'avion
	
	bool m_bRenduFilDeFer;				// Si rendu en fil de fer

	CSceneBattleFly(CMoteur* pMoteur);
	~CSceneBattleFly();
	
	bool Initialisation();
	bool CreationObjet();
	void DestructionObjet();
	void Rendu3D();
	void Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris);
	void Animation(double TempsEcouler, double DeltaTemps);
	void DebutRendu3D();
};
