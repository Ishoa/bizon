#pragma once
#include "..\Moteur\Scene.h"
#include "Interface.h"
#include "Sprites3D.h"

#define MAX_LIGHTS 4

class CSceneEffectDX : public CScene
{
public:


	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	CSprite3D* m_pSprite;
	LPD3DXMESH m_pSphereMesh;
	
	float m_fDeltatime;
	bool m_bTexture;

	CMesh* m_pCar;

	ID3DXEffect* g_pEffect;            // D3DX effect interface

	CLumiere m_pLumiere[MAX_LIGHTS];


	CInterface* m_pInterface;

	int m_nLightActive;

	CSceneEffectDX(CMoteur* pMoteur);
	~CSceneEffectDX();

	bool ConfirmeFonction3D(D3DCAPS9* pCaps);
	bool Initialisation();
	bool CreationObjet();
	void DestructionObjet();
	void Rendu3D();
	void Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick);
	void Animation(double TempsEcouler, double DeltaTemps);
	void Destruction(); 
	void DebutRendu3D();
};
