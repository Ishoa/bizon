
#ifndef _CDSHADOWVOLUME_
#define _CDSHADOWVOLUME_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"
#include "Mesh.h"

struct ShadowVertex
{
	D3DXVECTOR4 p;
	D3DCOLOR    color;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZRHW | D3DFVF_DIFFUSE
	};
};


class CShadowVolume
{
public:

	CShadowVolume();
	void    Reset() { m_dwNumVertices = 0L; }
	HRESULT buildShadowVolume( CMesh* pObject, D3DXVECTOR3 vLight );
	HRESULT ReBuildShadowVolume(D3DXVECTOR3 vLight);
	HRESULT render( LPDIRECT3DDEVICE9 pd3dDevice );
	DWORD	dwTriangles() { return (m_dwNumVertices / 3); };
	void	clear();

private:

	void addEdge( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 );

	CMesh*	m_pMesh;
	D3DXVECTOR3 m_pVertices[32000]; // Vertex data for rendering shadow volume
	DWORD       m_dwNumVertices;
};

#endif //_CDSHADOWVOLUME_