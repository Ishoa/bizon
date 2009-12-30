#pragma once
#include "..\Moteur\Scene.h"
#include "Interface.h"

struct FloorVertex
{
	float x, y, z;
	float nx, ny, nz;
	float tu, tv;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
	};
};

FloorVertex g_quadVertices[] =
{
	{-1.0f, 0.0f,-1.0f,  0.0f,1.0f,0.0f,  0.0f, 1.0f },
	{-1.0f, 0.0f, 1.0f,  0.0f,1.0f,0.0f,  0.0f, 0.0f },
	{ 1.0f, 0.0f,-1.0f,  0.0f,1.0f,0.0f,  1.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f,  0.0f,1.0f,0.0f,  1.0f, 0.0f }
};



class ShadowVolume
{
public:

	void    reset() { m_dwNumVertices = 0L; }
	HRESULT buildShadowVolume( LPD3DXMESH pObject, D3DXVECTOR3 vLight );
	HRESULT render( LPDIRECT3DDEVICE9 pd3dDevice );

private:

	void addEdge( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 );

	D3DXVECTOR3 m_pVertices[32000]; // Vertex data for rendering shadow volume
	DWORD       m_dwNumVertices;
};

struct ShadowVertex2
{
	D3DXVECTOR4 p;
	D3DCOLOR    color;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZRHW | D3DFVF_DIFFUSE
	};
};

//-----------------------------------------------------------------------------
// Name: render
// Desc:
//-----------------------------------------------------------------------------
HRESULT ShadowVolume::render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	pd3dDevice->SetFVF( D3DFVF_XYZ );

	return pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_dwNumVertices/3,
		m_pVertices, sizeof(D3DXVECTOR3) );
}

//-----------------------------------------------------------------------------
// Name: buildShadowVolume()
// Desc: Takes a mesh as input, and uses it to build a shadow volume. The
//       technique used considers each triangle of the mesh, and adds it's
//       edges to a temporary list. The edge list is maintained, such that
//       only silohuette edges are kept. Finally, the silohuette edges are
//       extruded to make the shadow volume vertex list.
//-----------------------------------------------------------------------------
HRESULT ShadowVolume::buildShadowVolume( LPD3DXMESH pMesh, D3DXVECTOR3 vLight )
{
	// Note: the MeshVertex format depends on the FVF of the mesh
	struct MeshVertex { D3DXVECTOR3 p, n; }; // Works fine for teapot, but won't do for all meshes!

	MeshVertex *pVertices;
	WORD       *pIndices;

	// Lock the geometry buffers
	pMesh->LockVertexBuffer( 0L, (LPVOID*)&pVertices );
	pMesh->LockIndexBuffer( 0L, (LPVOID*)&pIndices );
	DWORD dwNumFaces    = pMesh->GetNumFaces();

	// Allocate a temporary edge list
	WORD *pEdges = new WORD[dwNumFaces*6];

	if( pEdges == NULL )
	{
		pMesh->UnlockVertexBuffer();
		pMesh->UnlockIndexBuffer();
		return E_OUTOFMEMORY;
	}

	DWORD dwNumEdges = 0;

	// For each face
	for( DWORD i = 0; i < dwNumFaces; ++i )
	{
		WORD wFace0 = pIndices[3*i+0];
		WORD wFace1 = pIndices[3*i+1];
		WORD wFace2 = pIndices[3*i+2];

		D3DXVECTOR3 v0 = pVertices[wFace0].p;
		D3DXVECTOR3 v1 = pVertices[wFace1].p;
		D3DXVECTOR3 v2 = pVertices[wFace2].p;

		// Transform vertices or transform light?
		D3DXVECTOR3 vCross1(v2-v1);
		D3DXVECTOR3 vCross2(v1-v0);
		D3DXVECTOR3 vNormal;
		D3DXVec3Cross( &vNormal, &vCross1, &vCross2 );

		if( D3DXVec3Dot( &vNormal, &vLight ) >= 0.0f )
		{
			addEdge( pEdges, dwNumEdges, wFace0, wFace1 );
			addEdge( pEdges, dwNumEdges, wFace1, wFace2 );
			addEdge( pEdges, dwNumEdges, wFace2, wFace0 );
		}
	}

	for( DWORD i = 0; i < dwNumEdges; ++i )
	{
		D3DXVECTOR3 v1 = pVertices[pEdges[2*i+0]].p;
		D3DXVECTOR3 v2 = pVertices[pEdges[2*i+1]].p;
		D3DXVECTOR3 v3 = v1 - vLight*10;
		D3DXVECTOR3 v4 = v2 - vLight*10;

		// Add a quad (two triangles) to the vertex list
		m_pVertices[m_dwNumVertices++] = v1;
		m_pVertices[m_dwNumVertices++] = v2;
		m_pVertices[m_dwNumVertices++] = v3;

		m_pVertices[m_dwNumVertices++] = v2;
		m_pVertices[m_dwNumVertices++] = v4;
		m_pVertices[m_dwNumVertices++] = v3;
	}

	// Delete the temporary edge list
	delete[] pEdges;

	// Unlock the geometry buffers
	pMesh->UnlockVertexBuffer();
	pMesh->UnlockIndexBuffer();

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: addEdge()
// Desc: Adds an edge to a list of silohuette edges of a shadow volume.
//-----------------------------------------------------------------------------
void ShadowVolume::addEdge( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 )
{
	// Remove interior edges (which appear in the list twice)
	for( DWORD i = 0; i < dwNumEdges; ++i )
	{
		if( ( pEdges[2*i+0] == v0 && pEdges[2*i+1] == v1 ) ||
			( pEdges[2*i+0] == v1 && pEdges[2*i+1] == v0 ) )
		{
			if( dwNumEdges > 1 )
			{
				pEdges[2*i+0] = pEdges[2*(dwNumEdges-1)+0];
				pEdges[2*i+1] = pEdges[2*(dwNumEdges-1)+1];
			}

			--dwNumEdges;
			return;
		}
	}

	pEdges[2*dwNumEdges+0] = v0;
	pEdges[2*dwNumEdges+1] = v1;
	dwNumEdges++;
}

//#############################################################################

class CSceneTestShadow : public CScene
{
public:


	D3DLIGHT9 g_light0;

	LPDIRECT3DVERTEXBUFFER9 g_pFloorVB;
	LPDIRECT3DTEXTURE9      g_pFloorTex;
	D3DMATERIAL9            g_pFloorMtrl;

	LPD3DXMESH          g_pTeapotMesh ;
	D3DMATERIAL9       *g_pTeapotMtrls;
	LPDIRECT3DTEXTURE9 *g_pTeapotTextures;
	unsigned long       g_dwTeapotNumMtrls ;

	D3DXMATRIXA16            m_matTeapot;
	LPDIRECT3DVERTEXBUFFER9  m_pBigSquareVB;
	ShadowVolume            *m_pShadowVolume;

	bool g_bMakeShadowVolumeVisible;
	bool g_bTwoSidedStencilsAvailable;


	D3DXMATRIXA16 m_MatriceGeneral;		// Matrice de transformation 

	int m_QualiterFiltrage;					// type de filtrage des textures

	CInterface* m_pInterface;

	CSceneTestShadow(CMoteur* pMoteur);
	~CSceneTestShadow();

	bool ConfirmeFonction3D(D3DCAPS9* pCaps);
	bool Initialisation();
	bool CreationObjet();
	void DestructionObjet();
	void Rendu3D();
	void Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick);
	void Animation(double TempsEcouler, double DeltaTemps);
	void Destruction(); 
	void SetFiltreTexture(int Type);

	void renderShadowToStencilBuffer(void);
	void renderShadowToScene(void);
};
