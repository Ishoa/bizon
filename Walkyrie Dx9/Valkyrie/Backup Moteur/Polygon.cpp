#include "Polygon.h"

// Constructeur de la classe E_Polygon.
CPolygon::CPolygon()
{
    m_pBoundingBox = NULL;
	m_pBoundingSphere =  NULL;

    m_pVertex[0]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    m_pVertex[1]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    m_pVertex[2]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

// Deuxième constructeur de la classe E_Polygon.
CPolygon::CPolygon( D3DXVECTOR3 Vertex1,
                      D3DXVECTOR3 Vertex2,
                      D3DXVECTOR3 Vertex3 )
{
    m_pBoundingBox = NULL;
	m_pBoundingSphere = NULL;

    m_pVertex[0]   = Vertex1;
    m_pVertex[1]   = Vertex2;
    m_pVertex[2]   = Vertex3;
}

// Destructeur de la classe E_Polygon.
CPolygon::~CPolygon()
{
    // On nettoie les ressources de l'objet BoundingBox.
    if ( m_pBoundingBox != NULL )
    {
        delete m_pBoundingBox;
        m_pBoundingBox = NULL;
    }

	// On nettoie les ressources de l'objet BoundingSphere.
	if ( m_pBoundingSphere != NULL )
	{
		delete m_pBoundingSphere;
		m_pBoundingSphere = NULL;
	}

	
}

// Obtient le sommet à l'index spécifié.
D3DXVECTOR3 CPolygon::GetVertex( int Index )
{
    // On applique un masque binaire de 3 sur l'index (valeur binaire 0011),
    // ceci afin de garantir que l'utilisateur ne pourra jamais dépasser les
    // valeurs limites du tableau de sommets. C'est ce que l'on appelle un
    // tableau circulaire.
    return m_pVertex[Index&3];
}

// Définit le sommet à l'index spécifié.
bool CPolygon::SetVertex( int Index, D3DXVECTOR3 Vertex )
{
    // On applique un masque binaire de 3 sur l'index (valeur binaire 0011),
    // ceci afin de garantir que l'utilisateur ne pourra jamais dépasser les
    // valeurs limites du tableau de sommets. C'est ce que l'on appelle un
    // tableau circulaire.
    m_pVertex[Index&3] = Vertex;

    UpdateBounding();

    return true;
}

// Obtient la coordonnée du premier sommet du polygone.
D3DXVECTOR3 CPolygon::GetVertex1()
{
    return m_pVertex[0];
}

// Définit la coordonnée du premier sommet du polygone.
void CPolygon::SetVertex1( D3DXVECTOR3 Value )
{
    m_pVertex[0] = Value;
    UpdateBounding();
}

// Obtient la coordonnée du deuxième sommet du polygone.
D3DXVECTOR3 CPolygon::GetVertex2()
{
    return m_pVertex[1];
}

// Définit la coordonnée du deuxième sommet du polygone.
void CPolygon::SetVertex2( D3DXVECTOR3 Value )
{
    m_pVertex[1] = Value;
    UpdateBounding();
}

// Obtient la coordonnée du troisième sommet du polygone.
D3DXVECTOR3 CPolygon::GetVertex3()
{
    return m_pVertex[2];
}

// Définit la coordonnée du troisième sommet du polygone.
void CPolygon::SetVertex3( D3DXVECTOR3 Value )
{
    m_pVertex[2] = Value;
    UpdateBounding();
}

// Obtient la boîte de collision du polygone.
CBoundingBox* CPolygon::GetBoundingBox()
{
    if ( m_pBoundingBox == NULL )
    {
        m_pBoundingBox = new CBoundingBox();

        if ( m_pBoundingBox == NULL )
        {
            return NULL;
        }
    }

    return m_pBoundingBox;
}

// Obtient la sphere de collision du polygone.
CBoundingSphere* CPolygon::GetBoundingSphere()
{
	if ( m_pBoundingSphere == NULL )
	{
		m_pBoundingSphere = new CBoundingSphere();

		if ( m_pBoundingSphere == NULL )
		{
			return NULL;
		}
	}

	return m_pBoundingSphere;
}

// Cette fonction met à jour les données de la boîte de collisions.
void CPolygon::UpdateBounding()
{
    if ( m_pBoundingBox != NULL )
    {
        for ( int i = 0; i < 3; i++ )
        {
            m_pBoundingBox->SetMax
                ( CMaths::GetMaxValue( m_pVertex[i],
                                        m_pBoundingBox->GetMax() ) );

            m_pBoundingBox->SetMin
                ( CMaths::GetMinValue( m_pVertex[i],
                                        m_pBoundingBox->GetMin() ) );
        }
    }

	if ( m_pBoundingSphere != NULL )
	{
		D3DXVECTOR3 center = GetCenter(); 
		m_pBoundingSphere->SetCenter( center);
		float fRadius = (float)sqrt(  (m_pVertex[0].x-center.x)*(m_pVertex[0].x-center.x)+
										(m_pVertex[0].y-center.y)*(m_pVertex[0].y-center.y)+
										(m_pVertex[0].z-center.z)*(m_pVertex[0].z-center.z));
		m_pBoundingSphere->SetRadius(fRadius);

	}

}

// Obtient le plan du polygone.
D3DXPLANE CPolygon::GetPlane()
{
    D3DXPLANE thePlane;

    D3DXPlaneFromPoints( &thePlane, &m_pVertex[0], &m_pVertex[1], &m_pVertex[2] );

    return thePlane;
}

// Obtient le point central du polygone.
D3DXVECTOR3 CPolygon::GetCenter()
{
    return D3DXVECTOR3
        ( ( ( m_pVertex[0].x + m_pVertex[1].x + m_pVertex[2].x ) / 3.0f ),
          ( ( m_pVertex[0].y + m_pVertex[1].y + m_pVertex[2].y ) / 3.0f ),
          ( ( m_pVertex[0].z + m_pVertex[1].z + m_pVertex[2].z ) / 3.0f ) );
}

// Obtient un clone du polygone.
CPolygon CPolygon::GetClone()
{
    return CPolygon( m_pVertex[0], m_pVertex[1], m_pVertex[2] );
}

// Obtient un clone du polygone transformé selon une matrice donnée.
CPolygon CPolygon::ApplyMatrix( D3DXMATRIXA16 &Matrix )
{
    D3DXVECTOR3 TVertex[3];

    D3DXVec3TransformCoord( &TVertex[0], &m_pVertex[0], &Matrix );
    D3DXVec3TransformCoord( &TVertex[1], &m_pVertex[1], &Matrix );
    D3DXVec3TransformCoord( &TVertex[2], &m_pVertex[2], &Matrix );

    return CPolygon( TVertex[0], TVertex[1], TVertex[2] );
}
