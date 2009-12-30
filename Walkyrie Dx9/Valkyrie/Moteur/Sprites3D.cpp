#include "Sprites3D.h"

// Constructeur de la classe CSprite.
CSprite3D::CSprite3D(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();
	m_pVertexBuffer    = NULL;

	m_fWidth           = 1.0f;
	m_fHeight          = 1.0f;

	m_fRX = 0.0f;
	m_fRY = 0.0f;
	m_fRZ = 0.0f;

	m_fTX = 0.0f;
	m_fTY = 0.0f;
	m_fTZ = 0.0f;

	m_bIsAlphaBlended   = false;


	DXEtats = NULL;
}


// Destructeur de la classe E_Sprite.
CSprite3D::~CSprite3D()
{

}


// Définit la texture du sprite.
void CSprite3D::SetTexture( CLPTexture Texture )
{
	m_pTexture = Texture;
}


// Obtient la largeur du sprite.
float CSprite3D::GetWidth()
{
	return m_fWidth;
}

// Définit la largeur du sprite.
void CSprite3D::SetWidth( float Value )
{
	m_fWidth = Value;
}

// Obtient la hauteur du sprite.
float CSprite3D::GetHeight()
{
	return m_fHeight;
}

// Définit la hauteur du sprite.
void CSprite3D::SetHeight( float Value )
{
	m_fHeight = Value;
}

// Obtient l'angle de rotation du sprite sur l'axe X.
float CSprite3D::GetRotationX()
{
	return m_fRX;
}

// Définit l'angle de rotation du sprite sur l'axe X.
void CSprite3D::SetRotationX( float Value )
{
	if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
	{
		m_fRX = Value;
	}
	else
	{
		m_fRX = 0;
	}
}

// Obtient l'angle de rotation du sprite sur l'axe Y.
float CSprite3D::GetRotationY()
{
	return m_fRY;
}

// Définit l'angle de rotation du sprite sur l'axe Y.
void CSprite3D::SetRotationY( float Value )
{
	if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
	{
		m_fRY = Value;
	}
	else
	{
		m_fRY = 0;
	}
}

// Obtient l'angle de rotation du sprite sur l'axe Z.
float CSprite3D::GetRotationZ()
{
	return m_fRZ;
}

// Définit l'angle de rotation du sprite sur l'axe Z.
void CSprite3D::SetRotationZ( float Value )
{
	if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
	{
		m_fRZ = Value;
	}
	else
	{
		m_fRZ = 0;
	}
}

// Définit l'angle de rotation du sprite sur l'axe XYZ.
void CSprite3D::SetRotationXYZ( float fXValue ,float fYValue , float fZValue)
{
	SetRotationX(fXValue);
	SetRotationY(fYValue);
	SetRotationZ(fZValue);
}


// Obtient la valeur de la position du sprite sur l'axe X.
float CSprite3D::GetTranslationX()
{
	return m_fTX;
}

// Définit la valeur de la position du sprite sur l'axe X.
void CSprite3D::SetTranslationX( float Value )
{
	m_fTX = Value;
}

// Obtient la valeur de la position du sprite sur l'axe Y.
float CSprite3D::GetTranslationY()
{
	return m_fTY;
}

// Définit la valeur de la position du sprite sur l'axe Y.
void CSprite3D::SetTranslationY( float Value )
{
	m_fTY = Value;
}

// Obtient la valeur de la position du sprite sur l'axe Z.
float CSprite3D::GetTranslationZ()
{
	return m_fTZ;
}

// Définit la valeur de la position du sprite sur l'axe Z.
void CSprite3D::SetTranslationZ( float Value )
{
	m_fTZ = Value;
}

// Définit la valeur de la position du sprite sur l'axe XYZ.
void CSprite3D::SetTranslationXYZ( float fXValue ,float fYValue , float fZValue)
{
	SetTranslationX(fXValue);
	SetTranslationY(fYValue);
	SetTranslationZ(fZValue);
}


// Cette fonction construit la matrice d'affichage locale pour le sprite.
D3DXMATRIXA16 CSprite3D::GetWorldMatrix()
{
	D3DXMATRIXA16 RX;
	D3DXMATRIXA16 RY;
	D3DXMATRIXA16 RZ;
	D3DXMATRIXA16 SpriteMatrix;

	// On initialise toutes les matrices.
	D3DXMatrixIdentity( &RX );
	D3DXMatrixIdentity( &RY );
	D3DXMatrixIdentity( &RZ );
	D3DXMatrixIdentity( &SpriteMatrix );

	D3DXMatrixRotationX( &RX, m_fRX );
	D3DXMatrixRotationY( &RY, m_fRY );
	D3DXMatrixRotationZ( &RZ, m_fRZ );

	// Ensuite, on combine les différentes matrices, pour créer la matrice
	// finale. Attention, l'ordre des opérations est important.
	D3DXMatrixMultiply( &SpriteMatrix, &RX, &SpriteMatrix );
	D3DXMatrixMultiply( &SpriteMatrix, &RY, &SpriteMatrix );
	D3DXMatrixMultiply( &SpriteMatrix, &RZ, &SpriteMatrix );

	// On combine la translation directement dans la matrice.
	SpriteMatrix._41 = m_fTX;
	SpriteMatrix._42 = m_fTY;
	SpriteMatrix._43 = m_fTZ;

	// Pour finir, on retourne la matrice finale.
	return SpriteMatrix;
}

// Cette fonction initialise le sprite.
bool CSprite3D::ChargerSprites3D(LPSTR pFileTexture, float fWidth, float fHeight, float fRepeatTextureX,float fRepeatTextureY)
{
	m_pFileTexture = pFileTexture;

	m_fWidth = fWidth;
	m_fHeight = fHeight;
	D3DXVECTOR3 pVertex[4];
	D3DXVECTOR3 pNormal;

	// On commence par créer les coordonnées du sprite.
	pVertex[0] = D3DXVECTOR3( -m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f );
	pVertex[1] = D3DXVECTOR3( -m_fWidth / 2.0f,  m_fHeight / 2.0f, 0.0f );
	pVertex[2] = D3DXVECTOR3(  m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f );
	pVertex[3] = D3DXVECTOR3(  m_fWidth / 2.0f,  m_fHeight / 2.0f, 0.0f );

	pNormal = D3DXVECTOR3( -1.0f, 0.0f, 0.0f );
	
	// Puis, on initialise le VertexBuffer.
	if( FAILED( m_pD3DDevice->CreateVertexBuffer( 4 * sizeof( SPRITE3D_VERTEX ),
													D3DUSAGE_WRITEONLY,
													SPRITE3D_VERTEX::FVF,
													D3DPOOL_MANAGED,
													&m_pVertexBuffer,
													NULL ) ) )
	{
		// Si l'initialisation échoue, on retourne une erreur.
		return false;
	}

	SPRITE3D_VERTEX* p_Vertices;

	// On verrouille ensuite le VertexBuffer, pour écrire les données.
	if( FAILED( m_pVertexBuffer->Lock( 0, 0, (void**)&p_Vertices, 0 ) ) )
	{
		// Si l'opération échoue, on retourne une erreur.
		return false;
	}

	// On crée ici le polygone, en copiant les données dans le VertexBuffer.
	p_Vertices[0].Position = pVertex[0];
	p_Vertices[0].Normal   = pNormal;
	p_Vertices[0].Tu       = 0.0f;
	p_Vertices[0].Tv       = fRepeatTextureY;
	p_Vertices[0].Color    = 0xFFFFFFFF;

	p_Vertices[1].Position = pVertex[1];
	p_Vertices[1].Normal   = pNormal;
	p_Vertices[1].Tu       = 0.0f;
	p_Vertices[1].Tv       = 0.0f;
	p_Vertices[1].Color    = 0xFFFFFFFF;

	p_Vertices[2].Position = pVertex[2];
	p_Vertices[2].Normal   = pNormal;
	p_Vertices[2].Tu       = fRepeatTextureX;
	p_Vertices[2].Tv       = fRepeatTextureY;
	p_Vertices[2].Color    = 0xFFFFFFFF;

	p_Vertices[3].Position = pVertex[3];
	p_Vertices[3].Normal   = pNormal;
	p_Vertices[3].Tu       = fRepeatTextureX;
	p_Vertices[3].Tv       = 0.0f;
	p_Vertices[3].Color    = 0xFFFFFFFF;

	// Puis, on libère l'accès au VertexBuffer.
	m_pVertexBuffer->Unlock();

	// On teste si un fichier de texture a été spécifié.
	if ( m_pFileTexture != NULL )
	{
		
		// Si c'est le cas, on tente de charger celui-ci.
		if(!m_pTexture.chargerTexture(m_pD3DDevice,m_pFileTexture))
		{
			// Si l'opération échoue, on retourne une erreur.
			return false;
		}

	}

	m_vMaxMesh = pVertex[0];
	m_vMinMesh = pVertex[3];

	DXEtats = new CDXPileEtats(m_pD3DDevice);

	// Si tout s'est bien passé, on retourne un message de réussite.
	return true;
}

// Cette fonction permet le rendu du sprite.
void CSprite3D::RenderSprite3D(bool SaveState)
{

	if(BoundingBoxIsVisibleByCamera())
	{
		// On demande de sauvegarder l'état du Device Direct3D.
		if(SaveState)
			DXEtats->PushDeviceProperties();

		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		// On met à jour la matrice de monde, pour placer l'objet dans l'espace.

		// On active D3DTADDRESS_CLAMP pour une bonne jointure des textures de la sky box
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP);


		D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
		//D3DXMatrixIdentity(&WorldMatrix);
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &WorldMatrix );

		// Puis, on indique à Direct3D l'emplacement du VertexBuffer.
		m_pD3DDevice->SetStreamSource( 0,
			m_pVertexBuffer,
			0,
			sizeof( SPRITE3D_VERTEX ) );

		// On indique ensuite le type de FVF (Flexible Vertex Format) à utiliser.
		m_pD3DDevice->SetFVF( SPRITE3D_VERTEX::FVF );

		// On indique l'emplacement de la texture à appliquer.
		m_pD3DDevice->SetTexture( 0, m_pTexture.getDXTexture() );

		// Pour finir, on commande à Direct3D le rendu de la primitive.
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		// Enfin, on restaure l'état d'origine du Device Direct3D.
		if(SaveState)
			DXEtats->PopDeviceProperties();
	}
}

// Restaure le sprite après la construction d'un nouveau Device Direct3D.
bool CSprite3D::RestoreSprite3D()
{
	// On détruit le VertexBuffer, si celui-ci existe.
	if ( m_pVertexBuffer != NULL )
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}

	m_pTexture.Release();


	// Enfin, on reconstruit l'objet.
	return ChargerSprites3D(m_pFileTexture,m_fWidth,m_fHeight);
}

void CSprite3D::Release()
{

	// Nettoie les ressources utilisées par le VertexBuffer.
	if ( m_pVertexBuffer != NULL )
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}

	m_pTexture.Release();

}

// Cette fonction configure l'affichage pour permettre la transparence.
void CSprite3D::SetAlphaBlending()
{
	// En définissant la source et la destination de l'alpha blending à ONE,
	// l'objet devient transparent. Attention toutefois, le Z-Buffer est
	// désactivé pour cette opération, l'objet doit donc être rendu à la
	// fin de la scène, éventuellement trié par ordre de profondeur s'il y a
	// plusieurs objets de ce type à dessiner, afin d'obtenir un affichage
	// correct.
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );
}



bool CSprite3D::BoundingBoxIsVisibleByCamera()
{
	D3DXVECTOR4 vCurrentMin (m_vMinMesh.x,m_vMinMesh.y,m_vMinMesh.z,1.0f);
	D3DXVECTOR4 vCurrentMax (m_vMaxMesh.x,m_vMaxMesh.y,m_vMaxMesh.z,1.0f);

	D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();

	D3DXVec4Transform(&vCurrentMin, &vCurrentMin, &WorldMatrix );
	D3DXVec4Transform(&vCurrentMax, &vCurrentMax, &WorldMatrix );

	// On calcule, puis on inscrit le centre de la sphère.
	D3DXVECTOR3 vPositionBoundingSphere = D3DXVECTOR3( ( vCurrentMin.x + vCurrentMax.x ) / 2.0f,
		( vCurrentMin.y + vCurrentMax.y ) / 2.0f,
		( vCurrentMin.z + vCurrentMax.z ) / 2.0f ) ;

	// Ensuite, on calcule le rayon de la sphère.
	D3DXVECTOR3 vRadiusVector = D3DXVECTOR3( ( vCurrentMax.x - vCurrentMin.x ) / 2.0f,
		( vCurrentMax.y - vCurrentMin.y ) / 2.0f,
		( vCurrentMax.z - vCurrentMin.z ) / 2.0f );

	float fRayonBoundingSphere = D3DXVec3Length( &vRadiusVector );

	return m_pScene->GetCameraCourante()->IsVisible(&vCurrentMin,&vCurrentMax);
}

bool CSprite3D::BoundingSphereIsVisibleByCamera()
{
	D3DXVECTOR4 vCurrentMin (m_vMinMesh.x,m_vMinMesh.y,m_vMinMesh.z,1.0f);
	D3DXVECTOR4 vCurrentMax (m_vMaxMesh.x,m_vMaxMesh.y,m_vMaxMesh.z,1.0f);

	D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();

	D3DXVec4Transform(&vCurrentMin, &vCurrentMin, &WorldMatrix );
	D3DXVec4Transform(&vCurrentMax, &vCurrentMax, &WorldMatrix );

	// On calcule, puis on inscrit le centre de la sphère.
	D3DXVECTOR3 vPositionBoundingSphere = D3DXVECTOR3( ( vCurrentMin.x + vCurrentMax.x ) / 2.0f,
		( vCurrentMin.y + vCurrentMax.y ) / 2.0f,
		( vCurrentMin.z + vCurrentMax.z ) / 2.0f ) ;

	// Ensuite, on calcule le rayon de la sphère.
	D3DXVECTOR3 vRadiusVector = D3DXVECTOR3( ( vCurrentMax.x - vCurrentMin.x ) / 2.0f,
		( vCurrentMax.y - vCurrentMin.y ) / 2.0f,
		( vCurrentMax.z - vCurrentMin.z ) / 2.0f );

	float fRayonBoundingSphere = D3DXVec3Length( &vRadiusVector );

	return m_pScene->GetCameraCourante()->IsVisible(&vPositionBoundingSphere,fRayonBoundingSphere);
}

