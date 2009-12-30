#pragma once
#include "..\Moteur\Scene.h"

class CSceneDemoMipMapping : public CScene
{
public:

	enum EFilterTypes
	{
		FILTER_TYPE_NONE = 0,
		FILTER_TYPE_POINT,
		FILTER_TYPE_LINEAR,
		FILTER_TYPE_ANISOTROPIC
	};

	struct Vertex
	{
		float x, y, z;
		float tu, tv;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
		};
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9      m_pMipMapTexture;

	LPD3DXFONT m_pD3DxFont;

	EFilterTypes m_eMipFilterType;
	EFilterTypes m_eMinFilterType;
	EFilterTypes m_eMagFilterType;
	bool m_bChangeFilters;

	int   m_nAnisotropy;
	float m_fMipMapLodBias;


	CSceneDemoMipMapping(CMoteur* pMoteur);
	~CSceneDemoMipMapping();

	void loadMipMapTexture( void );
	void setMagnificationFilter();
	void setMinificationFilter();
	void setMipMapFilter();

	void  afficherInterface();

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
