#pragma once
#include "..\..\Valkyrie\Moteur\Scene.h"
#include "..\..\Valkyrie\Moteur\CameraTrackBall.h"

class CSceneShader : public CScene
{
public:

	struct Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
		D3DCOLOR diffuse;
		float tu1, tv1;
		float tu2, tv2;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2
		};
	};

	D3DXMATRIXA16 m_MatriceGeneral;							// Matrice de transformation 

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;				// vertex buffer - stream 0
	LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;					// index buffer
	CLPTexture      m_pTexture;						// Texture
	LPDIRECT3DVERTEXSHADER9 m_pVertexShader;				// Vertex Shader
	LPDIRECT3DPIXELSHADER9  m_pPixelShader;					// Pixel Shader
	LPD3DXCONSTANTTABLE     m_pConstantTableVertexShader;   // table des constantes pour le Vertex Shader
	LPD3DXCONSTANTTABLE     m_pConstantTablePixelShader;    // table des constantes pour le Pixel Shader

	int m_nvert;
	int m_nfac;
	int m_ndiv;
	int m_QualiterFiltrage;									// type de filtrage des textures

	CLumiere m_Lumiere;

	CSceneShader(CMoteur* pMoteur);
	~CSceneShader();

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