#include "ShadowVolume.h"


CShadowVolume::CShadowVolume()
{
	m_dwNumVertices = 0;
	m_pMesh = NULL;
}


//-----------------------------------------------------------------------------
// Name: render
// Desc:
//-----------------------------------------------------------------------------

HRESULT CShadowVolume::render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	pd3dDevice->SetFVF( D3DFVF_XYZ );
	pd3dDevice->SetTransform( D3DTS_WORLD, &m_pMesh->GetWorldMatrix() );
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
HRESULT CShadowVolume::buildShadowVolume( CMesh* pMesh, D3DXVECTOR3 vLight )
{

	if(m_pMesh == NULL)
		m_pMesh = pMesh;

	// Note: the MeshVertex format depends on the FVF of the mesh

	//struct MeshVertex { D3DXVECTOR3 p, n; FLOAT tu, tv; };
	struct MeshVertex { D3DXVECTOR3 p;};

	MeshVertex *pVertices;
	WORD       *pIndices;

	LPD3DXMESH pMeshTemporaire= NULL;

	if(FAILED(m_pMesh->GetDXMesh()->CloneMeshFVF( D3DXMESH_MANAGED,
									D3DFVF_XYZ,
									m_pMesh->GetDevice3D(), &pMeshTemporaire)))
		return S_FALSE;


// 	D3DXVECTOR3 vLightInWorldSpace( vLight);
// 	D3DXVECTOR3 vLightInObjectSpace;
// 	D3DXMATRIXA16 matInverse;
// 	D3DXMatrixInverse( &matInverse, NULL, &m_pMesh->GetWorldMatrix() );
// 	D3DXVec3TransformNormal( &vLightInObjectSpace, &vLightInWorldSpace, &matInverse );


	D3DXVECTOR3 vLightInObjectSpace;
	D3DXMATRIXA16 matInverse;
	D3DXMatrixInverse( &matInverse, NULL, &m_pMesh->GetWorldMatrix() );
	//multiply the lights position, to go from world->model space
	vLightInObjectSpace.x = vLight.x*matInverse._11 + vLight.y*matInverse._21 + vLight.z*matInverse._31 + matInverse._41;
	vLightInObjectSpace.y = vLight.x*matInverse._12 + vLight.y*matInverse._22 + vLight.z*matInverse._32 + matInverse._42;
	vLightInObjectSpace.z = vLight.x*matInverse._13 + vLight.y*matInverse._23 + vLight.z*matInverse._33 + matInverse._43;



	// Lock the geometry buffers
	pMeshTemporaire->LockVertexBuffer( 0L, (LPVOID*)&pVertices );
	pMeshTemporaire->LockIndexBuffer( 0L, (LPVOID*)&pIndices );
	DWORD dwNumFaces    = pMeshTemporaire->GetNumFaces();

	// Allocate a temporary edge list
	WORD *pEdges = new WORD[dwNumFaces*6];

	if( pEdges == NULL )
	{
		pMeshTemporaire->UnlockVertexBuffer();
		pMeshTemporaire->UnlockIndexBuffer();
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

		if( D3DXVec3Dot( &vNormal, &vLightInObjectSpace ) >= 0.0f )
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
		D3DXVECTOR3 v3 = v1 - vLightInObjectSpace*50;
		D3DXVECTOR3 v4 = v2 - vLightInObjectSpace*50;

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
	pMeshTemporaire->UnlockVertexBuffer();
	pMeshTemporaire->UnlockIndexBuffer();

	pMeshTemporaire->Release();
	return S_OK;
}


HRESULT CShadowVolume::ReBuildShadowVolume(D3DXVECTOR3 vLight)
{

	if(m_pMesh != NULL)
	{
		buildShadowVolume( m_pMesh,vLight);
		return S_OK;
	}
	else
		return S_FALSE;
}

//-----------------------------------------------------------------------------
// Name: addEdge()
// Desc: Adds an edge to a list of silohuette edges of a shadow volume.
//-----------------------------------------------------------------------------
void CShadowVolume::addEdge( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 )
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


void CShadowVolume::clear()
{

	m_pMesh = NULL;
	m_dwNumVertices = 0;
}	