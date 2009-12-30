#pragma once

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Scene.h"
#include "Lumiere.h"

class CBumpMappingObjet
{

public :

	struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
		DWORD diffuse;
		float tu1, tv1;
		float tu2, tv2;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2
		};
	};

	//====================================================================


	CScene* m_pScene;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	CLPTexture m_pTexture;
	CLPTexture m_pNormalMapTexture;

	
	D3DXVECTOR3* m_pTangents;
	D3DXVECTOR3* m_pBiNormals;
	D3DXVECTOR3* m_pNormals;

	int m_nNbVertex;
	bool m_bDoDot3BumpMapping;
	bool m_bTexture;
	//=====================================================================

	CBumpMappingObjet();
	CBumpMappingObjet(CScene* pScene);
	~CBumpMappingObjet();

	bool CreationObjectVertexBuffer(Vertex* pVertexBuffer, int nNbVertex, char* NomFichierTexture,char* NomFichierNormalMap);
	bool CreationObjectVertexBufferI(Vertex* pVertexBuffer, int nNbVertex, char* NomFichierTexture,char* NomFichierNormalMap);

	virtual void Release();
	virtual void RenduModel(CLumiere lumiere,D3DXMATRIX* matWorld);

private :

	DWORD encodeVectorAsDWORDColor(D3DXVECTOR3* vVector);
	void computeTangentsMatricesForEachVertex();
	void createTangentSpaceVectors( D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3,
									float v1u, float v1v, float v2u, float v2v, float v3u, float v3v, 
									D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal );

};
