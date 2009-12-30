#pragma once
#include "Scene.h"
#include "..\Objects\Planete.h"
#include "Panorama.h"
#include "InterfacesSystemeSolaire.h"

#define D3DFVF_VERTEX_ANNEAU ( D3DFVF_XYZ | D3DFVF_DIFFUSE )

class CSceneSystemSolaire : public CScene
{
public:
	struct VERTEX_ANNEAU {
		D3DVECTOR   P;
		DWORD diffuse;
	};

	

	D3DXMATRIX m_MSunMatrix;
	D3DXMATRIX m_MMarsMatrix;
	D3DXMATRIX m_MmEarthMatrix;
	D3DXMATRIX m_MMoonMatrix;
	D3DXMATRIX m_MMercurenMatrix;
	D3DXMATRIX m_MPlutonMatrix;
	D3DXMATRIX m_MVenusMatrix;

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;

	int m_QualiterFiltrage;					// type de filtrage des textures

	CPanorama* m_pSpace;					// SkyBox space
	CPlanete * m_pTerre;
	CPlanete * m_pLune;
	CPlanete * m_pMars;
	CPlanete * m_pSoleil;
	CPlanete * m_pMercure;
	CPlanete * m_pPluton;
	CPlanete * m_pVenus;

	CLumiere m_LumiereC;					// Lumière du soleil
	CLumiere m_LumiereN;					// Lumière du soleil
	CLumiere m_LumiereS;					// Lumière du soleil
	CLumiere m_LumiereE;					// Lumière du soleil
	CLumiere m_LumiereO;					// Lumière du soleil
	CLumiere m_LumiereF;					// Lumière du soleil
	CLumiere m_LumiereB;					// Lumière du soleil

	int m_nChoix;

	bool m_blightActive;

	float m_fSpeedmodifier;
	bool  m_bOrbitOn;

	CInterfaceSystemSolaire* m_pInterface; // Pointeur sur l'Interface

	CSceneSystemSolaire(CMoteur* pMoteur);
	~CSceneSystemSolaire();
	
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
