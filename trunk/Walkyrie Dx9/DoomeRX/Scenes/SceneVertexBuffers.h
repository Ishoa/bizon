#pragma once
#include "..\..\Valkyrie\Moteur\Scene.h"
#include "..\..\Valkyrie\Moteur\Texture.h"

class CSceneVertexBuffer : public CScene
{
public:
	
	struct Vertex
	{
		float x, y, z;
	};

	struct Color
	{
		DWORD color;
	};

	struct TexCoord
	{
		float tu, tv;
	};


	CLPTexture m_pTexture;

	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DVERTEXBUFFER9 m_pColorBuffer;
	LPDIRECT3DVERTEXBUFFER9 m_pTexCoordBuffer;

	LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBufferIndexed;
	LPDIRECT3DVERTEXBUFFER9 m_pColorBufferIndexed;
	LPDIRECT3DVERTEXBUFFER9 m_pTexCoordBufferIndexed;

	//CLumiere m_Lumiere;
	int m_QualiterFiltrage;					// type de filtrage des textures
	bool m_bUseIndexedGeometry;

	CSceneVertexBuffer(CMoteur* pMoteur);
	~CSceneVertexBuffer();
	
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
