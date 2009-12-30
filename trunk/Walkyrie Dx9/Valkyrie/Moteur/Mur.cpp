#include "Mur.h"

CMur::CMur(CScene* pScene):CObjetDeCollision(),CSprite3D(pScene)
{

}

CMur::~CMur()
{

}

D3DXMATRIXA16 CMur::GetWorldMatrix()
{
	return CSprite3D::GetWorldMatrix();
}


bool CMur::ChargerPolygonList()
{

	D3DXVECTOR3 pVertex[4];

	// On commence par créer les coordonnées du sprite.
	pVertex[0] = D3DXVECTOR3( -m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f );
	pVertex[1] = D3DXVECTOR3( -m_fWidth / 2.0f,  m_fHeight / 2.0f, 0.0f );
	pVertex[2] = D3DXVECTOR3(  m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f );
	pVertex[3] = D3DXVECTOR3(  m_fWidth / 2.0f,  m_fHeight / 2.0f, 0.0f );

	// On crée ensuite la liste de polygones.
	m_pPolygonList = new CPolygonList();

	// Puis, on teste que la liste soit correctement crée en mémoire.
	if ( m_pPolygonList == NULL )
	{
		// Si ce n'est pas le cas, on retourne une erreur.
		return false;
	}

	// Ensuite, on copie les données dans la liste de polygones.
	for ( int i = 0; i < 2; i++ )
	{
		// On tente l'ajout d'un nouveau polygone.
		if ( m_pPolygonList->AddPolygon( pVertex[i],
			pVertex[i+1],
			pVertex[i+2] ) == false )
		{
			// Si l'ajout échoue, on retourne une erreur.
			return false;
		}
	}


	// Puis, on commande la construction des objets de collisions.
	m_pPolygonList->CreateBoundingBox   ();
	m_pPolygonList->CreateBoundingSphere();


	m_pPolygonList->GetBoundingBox().CreateVirtualObject( m_pD3DDevice, 0, 0, 255 );
	m_pPolygonList->GetBoundingSphere().CreateVirtualObject( m_pD3DDevice, 0, 0, 255 );

	return true;
}

bool CMur::ChargerSprites3D(LPSTR pFileTexture, float fWidth , float fHeight,float fRepeatTextureX,float fRepeatTextureY)
{
	if(!CSprite3D::ChargerSprites3D(pFileTexture,fWidth,fHeight,fRepeatTextureX,fRepeatTextureY))
		return false;
	if(!ChargerPolygonList())
		return false;
	return true;
}


void CMur::Release()
{	
	CSprite3D::Release();

	if(m_pPolygonList != NULL)
	{
		m_pPolygonList->ReleaseList();
	}
	m_pPolygonListLowDetail = NULL;
}
