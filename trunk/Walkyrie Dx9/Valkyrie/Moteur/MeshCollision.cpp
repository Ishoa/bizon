#include "MeshCollision.h"


CMeshCollision::CMeshCollision(CScene* pScene):CObjetDeCollision(),CMesh(pScene)
{
	m_bAffichageBoundingBox = false;
	m_bAffichageBoundingSphere = false;
}


// Destructeur de la classe CMeshCollision.
CMeshCollision::~CMeshCollision()
{

}


bool CMeshCollision::ChargerPolygonList()
{
	// On cr�e ensuite la liste des polygones de l'objet.
	m_pPolygonList = new CPolygonList();

	// Puis, on teste si la liste a �t� correctement initialis�e.
	if ( m_pPolygonList == NULL )
	{
		Console<<"Erreur Creation PolygonList du Mesh de collision"<<endl;
		return false;
	}

	LPD3DXMESH Clone = NULL;

	// On cr�e une copie de l'objet ne contenant que les valeurs XYZ.
	if ( FAILED( m_pMesh->CloneMeshFVF( D3DXMESH_SYSTEMMEM,
		D3DFVF_XYZ,
		m_pD3DDevice,
		&Clone ) ) )
	{
		Console<<"Erreur Clonage du Mesh de collision"<<endl;
		return false;
	}

	// Puis, on commande la cr�ation de la liste � partir de la copie.
	bool Result = m_pPolygonList->CreateListFromMesh( Clone );

	// On lib�re ensuite la ressource utilis�e par la copie.
	if ( Clone != NULL )
	{
		Clone->Release();
		Clone = NULL;
	}

	// On teste si une erreur est survenue pendant la cr�ation de la liste.
	if ( Result == false )
	{
		// Si c'est le cas, on retourne une erreur.
		return false;
	}

	// Puis, on commande la construction des objets de collisions.
	m_pPolygonList->CreateBoundingBox   ();
	m_pPolygonList->CreateBoundingSphere();


	m_pPolygonList->GetBoundingBox().CreateVirtualObject( m_pD3DDevice, 0, 0, 255 );
	m_pPolygonList->GetBoundingSphere().CreateVirtualObject( m_pD3DDevice, 0, 0, 255 );

	return true;
};

bool CMeshCollision::ChargerPolygonListLowDetail()
{
	// On cr�e ensuite la liste des polygones de l'objet.
	m_pPolygonListLowDetail = new CPolygonList();

	// Puis, on teste si la liste a �t� correctement initialis�e.
	if ( m_pPolygonListLowDetail == NULL )
	{
		Console<<"Erreur Creation PolygonList du Mesh de collision"<<endl;
		return false;
	}

	LPD3DXMESH Clone = NULL;

	// On cr�e une copie de l'objet ne contenant que les valeurs XYZ.
	if ( FAILED( m_pMeshLowDetail->CloneMeshFVF( D3DXMESH_SYSTEMMEM,
		D3DFVF_XYZ,
		m_pD3DDevice,
		&Clone ) ) )
	{
		Console<<"Erreur Clonage du Mesh de collision"<<endl;
		return false;
	}

	// Puis, on commande la cr�ation de la liste � partir de la copie.
	bool Result = m_pPolygonListLowDetail->CreateListFromMesh( Clone );

	// On lib�re ensuite la ressource utilis�e par la copie.
	if ( Clone != NULL )
	{
		Clone->Release();
		Clone = NULL;
	}

	// On teste si une erreur est survenue pendant la cr�ation de la liste.
	if ( Result == false )
	{
		// Si c'est le cas, on retourne une erreur.
		return false;
	}

	// Puis, on commande la construction des objets de collisions.
	m_pPolygonListLowDetail->CreateBoundingBox   ();
	m_pPolygonListLowDetail->CreateBoundingSphere();


	m_pPolygonListLowDetail->GetBoundingBox().CreateVirtualObject( m_pD3DDevice, 0, 0, 255 );
	m_pPolygonListLowDetail->GetBoundingSphere().CreateVirtualObject( m_pD3DDevice, 0, 0, 255 );

	return true;
}

// Cette fonction permet l'initialisation de l'objet.
bool CMeshCollision::ChargerMesh(LPSTR NomFichier)
{
	return CMesh::ChargerMesh(NomFichier) && ChargerPolygonList();
}


bool CMeshCollision::ChargerMeshLowDetail(LPSTR NomFichier)
{
	// Enfin, on retourne le message de r�ussite.
	return CMesh::ChargerMeshLowDetail(NomFichier) && ChargerPolygonListLowDetail();
}


void CMeshCollision::RenduMeshSampler(bool SaveState)
{
	CMesh::RenduMeshSampler(SaveState);
	
	D3DXMATRIXA16 mWorldMatrix;
	D3DXMatrixIdentity(&mWorldMatrix);

	if(m_bAffichageBoundingBox)
		m_pPolygonList->GetBoundingBox().RenderVirtualObject(CMesh::GetWorldMatrix(),SaveState);

	if(m_bAffichageBoundingSphere)
		m_pPolygonList->GetBoundingSphere().RenderVirtualObject(CMesh::GetWorldMatrix(),SaveState);//GetWorldMatrix());
		
}

D3DXMATRIXA16 CMeshCollision::GetWorldMatrix()
{
	return CMesh::GetWorldMatrix();
}

void CMeshCollision::Release()
{
	CMesh::Release();
	CObjetDeCollision::Release();

}