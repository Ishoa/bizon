#pragma once
#include "..\..\Valkyrie\Moteur\Scene.h"
#include "..\Objects\BumpMappingObjet.h"

class CSceneBumpMapping : public CScene
{
public:


	enum ETYPEAFFICHAGE
	{
		STONE = 0,
		CROSS,
		FLOOR,
		TERRE
	};

	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 
	LPD3DXMESH m_pSphereMesh;

	CBumpMappingObjet* m_pObjetBumpMappingFloor;
	CBumpMappingObjet* m_pObjetBumpMappingCross;
	CBumpMappingObjet* m_pObjetBumpMappingStone;
	CBumpMappingObjet* m_pObjetBumpMappingTerre;

	CLumiere m_Lumiere;
	CMaterielle m_mat;
	int m_QualiterFiltrage;		
	ETYPEAFFICHAGE m_eTYPEAFFICHAGE;

	// type de filtrage des textures

	CSceneBumpMapping(CMoteur* pMoteur);
	~CSceneBumpMapping();

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
