#include "SceneDemoMipMapping.h"
#include "..\..\Valkyrie\Moteur\CameraTrackBall.h"


CSceneDemoMipMapping::Vertex g_quadVertices[] =
{
	{-1.0f, 0.0f,-1.0f,  0.0f, 1.0f },
	{-1.0f, 0.0f, 1.0f,  0.0f, 0.0f },
	{ 1.0f, 0.0f,-1.0f,  1.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f,  1.0f, 0.0f }
};

CSceneDemoMipMapping::CSceneDemoMipMapping(CMoteur* pMoteur):CScene(pMoteur)
{

	m_eMipFilterType = FILTER_TYPE_LINEAR;
	m_eMinFilterType = FILTER_TYPE_LINEAR;
	m_eMagFilterType = FILTER_TYPE_LINEAR;

	m_pVertexBuffer  = NULL;
	m_pMipMapTexture = NULL;

	m_bChangeFilters = true;
	m_nAnisotropy    = 1;
	m_fMipMapLodBias = 0.0f;
}

CSceneDemoMipMapping::~CSceneDemoMipMapping()
{
	Console << "Deconstructeur CSceneDemoMipMapping" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneDemoMipMapping::ConfirmeFonction3D(D3DCAPS9* pCaps)
{
	if(!(pCaps->TextureCaps & D3DPTEXTURECAPS_PROJECTED))
	{
		Console << "Le peripherique ne permet pas la projection de texture" << endl;
		return false;
	}
	if(pCaps->MaxTextureWidth < 1024 || pCaps->MaxTextureHeight < 1024)
	{
		Console << "Le peripherique ne permet pas l'utilisation de texture de taille egale a 1024" << endl;
		return false;
	}
	if(!(pCaps->TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP))
	{
		Console << "Le peripherique ne permet pas l'utilisation du bump mapping" << endl;
		return false;
	}
	if(pCaps->MaxSimultaneousTextures < 2)
	{
		Console << "Le peripherique ne permet pas l'utilisation de 2 etages de texture" << endl;
		return false;
	}
	if(pCaps->MaxUserClipPlanes < 1)
	{
		Console << "Le peripherique ne permet pas l'utilisation de plan de clipping" << endl;
		return false;
	}
	return true;
}

// Initialisation de la scène 3D
bool CSceneDemoMipMapping::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	m_pCameraParDefaut = new CCameraTrackBall(this);
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 0.0f, 10.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	return true;
}

// Création des objets 3D de la scène
bool CSceneDemoMipMapping::CreationObjet()
{

	// de Base la lumiere est activé
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	loadMipMapTexture();

	m_pD3DDevice->CreateVertexBuffer( 4*sizeof(Vertex),0, Vertex::FVF_Flags,
		D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL );
	void *pVertices = NULL;

	m_pVertexBuffer->Lock( 0, sizeof(g_quadVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, g_quadVertices, sizeof(g_quadVertices) );
	m_pVertexBuffer->Unlock();


	// Initialisation de la lumière 1 et activation
	D3DXCreateFont(m_pD3DDevice, 14, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial", &m_pD3DxFont);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	return true;
}


void CSceneDemoMipMapping::loadMipMapTexture( void )
{
	// 256 = red
	// 128 = blue
	//  64 = green
	//  32 = yellow
	//  16 = purple
	//   8 = light blue

	LPDIRECT3DTEXTURE9 pTexArray[6];

	// Chargement des 6 mipmap personnalise
	D3DXCreateTextureFromFile( m_pD3DDevice, "..\\Donnees\\Textures\\tex256.bmp", &pTexArray[0] );
	D3DXCreateTextureFromFile( m_pD3DDevice, "..\\Donnees\\Textures\\tex128.bmp", &pTexArray[1] );
	D3DXCreateTextureFromFile( m_pD3DDevice, "..\\Donnees\\Textures\\tex64.bmp",  &pTexArray[2] );
	D3DXCreateTextureFromFile( m_pD3DDevice, "..\\Donnees\\Textures\\tex32.bmp",  &pTexArray[3] );
	D3DXCreateTextureFromFile( m_pD3DDevice, "..\\Donnees\\Textures\\tex16.bmp",  &pTexArray[4] );
	D3DXCreateTextureFromFile( m_pD3DDevice, "..\\Donnees\\Textures\\tex8.bmp",   &pTexArray[5] );

	// Chargement de la texture de base
	D3DXCreateTextureFromFile( m_pD3DDevice, "..\\Donnees\\Textures\\texbase256.bmp", &m_pMipMapTexture );

	LPDIRECT3DSURFACE9 pDestSurface = NULL;
	LPDIRECT3DSURFACE9 pSrcSurface  = NULL;

	int nNumLevels = m_pMipMapTexture->GetLevelCount();
	int i;

	for( i = 0; i < 6; ++i )
	{
		m_pMipMapTexture->GetSurfaceLevel( i, &pDestSurface );
		pTexArray[i]->GetSurfaceLevel( 0, &pSrcSurface );

		//chargement des mipmap personnalise pour la texture de base

		D3DXLoadSurfaceFromSurface( pDestSurface, NULL, NULL, 
			pSrcSurface,  NULL, NULL, 
			D3DX_DEFAULT, 0);

		pSrcSurface->Release();
		pDestSurface->Release();
	}

	for( i = 0; i < 6; ++i )
		pTexArray[i]->Release();

	m_bChangeFilters = true;
}


// Destruction des resource utilisé par les objets
void CSceneDemoMipMapping::DestructionObjet()
{

	m_pVertexBuffer->Release();
	m_pMipMapTexture->Release();
	m_pD3DxFont->Release();
}

// Destruction des objets
void CSceneDemoMipMapping::Destruction()
{

}

// Rendu 3D de la scène
void CSceneDemoMipMapping::Rendu3D()
{
	if( m_bChangeFilters == true )
	{
		setMinificationFilter();
		setMagnificationFilter();
		setMipMapFilter();
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, m_nAnisotropy );
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD)(&m_fMipMapLodBias)));
		m_bChangeFilters = false;
	}

	m_pD3DDevice->SetTexture( 0, m_pMipMapTexture);
	m_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(Vertex) );
	m_pD3DDevice->SetFVF( Vertex::FVF_Flags );

	D3DXMATRIX matFloor;
	float x = 0.0f;
	float z = 0.0f;

	for( int i = 0; i < 25; ++i )
	{
		for( int j = 0; j < 25; ++j )
		{
			D3DXMatrixTranslation( &matFloor, x, 0.0f, z );
			m_pD3DDevice->SetTransform( D3DTS_WORLD, &matFloor );
			m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

			x += 2.0f;
		}
		x  =  0.0f;
		z += 2.0f;
	}

	afficherInterface();

}

// Calculs des animations de la scène
void CSceneDemoMipMapping::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
}

// Test des entrées des périphériques
void CSceneDemoMipMapping::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
{

	if(KEYDOWN(DIK_F1))
	{
		switch(m_eMinFilterType)
		{
			case FILTER_TYPE_NONE : 
				m_eMinFilterType = FILTER_TYPE_POINT; 
				break;
			case FILTER_TYPE_POINT : 
				m_eMinFilterType = FILTER_TYPE_LINEAR; 
				break;
			case FILTER_TYPE_LINEAR : 
				m_eMinFilterType = FILTER_TYPE_ANISOTROPIC; 
				break;
			case FILTER_TYPE_ANISOTROPIC : 
				m_eMinFilterType = FILTER_TYPE_NONE; 
				break;
		}
		  m_bChangeFilters = true;
	}

	if(KEYDOWN(DIK_F2))
	{
		  switch(m_eMagFilterType)
		  {
		  case FILTER_TYPE_NONE : 
			  m_eMagFilterType = FILTER_TYPE_POINT; 
			  break;
		  case FILTER_TYPE_POINT : 
			  m_eMagFilterType = FILTER_TYPE_LINEAR; 
			  break;
		  case FILTER_TYPE_LINEAR : 
			  m_eMagFilterType = FILTER_TYPE_ANISOTROPIC; 
			  break;
		  case FILTER_TYPE_ANISOTROPIC : 
			  m_eMagFilterType = FILTER_TYPE_NONE; 
			  break;
		  }
		  m_bChangeFilters = true;
	}

	if(KEYDOWN(DIK_F3))
	{
		  switch(m_eMipFilterType)
		  {
		  case FILTER_TYPE_NONE : 
			  m_eMipFilterType = FILTER_TYPE_POINT; 
			  break;
		  case FILTER_TYPE_POINT : 
			  m_eMipFilterType = FILTER_TYPE_LINEAR; 
			  break;
		  case FILTER_TYPE_LINEAR : 
			  m_eMipFilterType = FILTER_TYPE_ANISOTROPIC; 
			  break;
		  case FILTER_TYPE_ANISOTROPIC : 
			  m_eMipFilterType = FILTER_TYPE_NONE; 
			  break;
		  }
		  m_bChangeFilters = true;
	}

}

void CSceneDemoMipMapping::setMagnificationFilter()
{  
	switch (m_eMagFilterType)
	{
	case FILTER_TYPE_NONE :
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
		break;
	case FILTER_TYPE_POINT : 
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		break;
	case FILTER_TYPE_LINEAR : 
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		break;
	case FILTER_TYPE_ANISOTROPIC :
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		break;
	}		
}


void CSceneDemoMipMapping::setMinificationFilter()
{  
	switch (m_eMinFilterType)
	{
	case FILTER_TYPE_NONE : 
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE); break;
	case FILTER_TYPE_POINT :
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		break;
	case FILTER_TYPE_LINEAR :
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		 break;
	case FILTER_TYPE_ANISOTROPIC :
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		break;
	}
}


void CSceneDemoMipMapping::setMipMapFilter()
{


	switch (m_eMipFilterType)
	{
		case FILTER_TYPE_NONE : 
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			break;
		case FILTER_TYPE_POINT :
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			break;
		case FILTER_TYPE_LINEAR : 
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			break;
		case FILTER_TYPE_ANISOTROPIC : 
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			break;
	}
		
}


void  CSceneDemoMipMapping::afficherInterface()
{



	RECT destRect1;
	SetRect( &destRect1, 5, 5, 0, 0 );

	RECT destRect2;
	SetRect( &destRect2, 5, 20, 0, 0 );

	RECT destRect3;
	SetRect( &destRect3, 5, 35, 0, 0 );

	static char strMinFilter[255];
	static char strMagFilter[255];
	static char strMipFilter[255];

	if( m_eMinFilterType == FILTER_TYPE_NONE )
		sprintf( strMinFilter, "D3DSAMP_MINFILTER = D3DTEXF_NONE    (Change: F1)" );
	else if( m_eMinFilterType == FILTER_TYPE_POINT )
		sprintf( strMinFilter, "D3DSAMP_MINFILTER = D3DTEXF_POINT    (Change: F1)" );
	else if( m_eMinFilterType == FILTER_TYPE_LINEAR )
		sprintf( strMinFilter, "D3DSAMP_MINFILTER = D3DTEXF_LINEAR    (Change: F1)" );
	else if( m_eMinFilterType == FILTER_TYPE_ANISOTROPIC )
		sprintf( strMinFilter, "D3DSAMP_MINFILTER = D3DTEXF_ANISOTROPIC    (Change: F1)" );

	if( m_eMagFilterType == FILTER_TYPE_NONE )
		sprintf( strMagFilter, "D3DSAMP_MAGFILTER = D3DTEXF_NONE    (Change: F2)" );
	else if( m_eMagFilterType == FILTER_TYPE_POINT )
		sprintf( strMagFilter, "D3DSAMP_MAGFILTER = D3DTEXF_POINT    (Change: F2)" );
	else if( m_eMagFilterType == FILTER_TYPE_LINEAR )
		sprintf( strMagFilter, "D3DSAMP_MAGFILTER = D3DTEXF_LINEAR    (Change: F2)" );
	else if( m_eMagFilterType == FILTER_TYPE_ANISOTROPIC )
		sprintf( strMagFilter, "D3DSAMP_MAGFILTER = D3DTEXF_ANISOTROPIC    (Change: F2)" );

	if( m_eMipFilterType == FILTER_TYPE_NONE )
		sprintf( strMipFilter, "D3DSAMP_MIPFILTER = D3DTEXF_NONE    (Change: F3)" );
	else if( m_eMipFilterType == FILTER_TYPE_POINT )
		sprintf( strMipFilter, "D3DSAMP_MIPFILTER = D3DTEXF_POINT    (Change: F3)" );
	else if( m_eMipFilterType == FILTER_TYPE_LINEAR )
		sprintf( strMipFilter, "D3DSAMP_MIPFILTER = D3DTEXF_LINEAR    (Change: F3)" );
	else if( m_eMipFilterType == FILTER_TYPE_ANISOTROPIC )
		sprintf( strMipFilter, "D3DSAMP_MIPFILTER = D3DTEXF_ANISOTROPIC    (Change: F3)" );

	m_pD3DxFont->DrawText( NULL, strMinFilter, -1, &destRect1, DT_NOCLIP, 
		D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );

	m_pD3DxFont->DrawText( NULL, strMagFilter, -1, &destRect2, DT_NOCLIP, 
		D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );

	m_pD3DxFont->DrawText( NULL, strMipFilter, -1, &destRect3, DT_NOCLIP, 
		D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );


	static char strDescription[255];
	sprintf( strDescription, "Description du MipMapping :\nCarré Blanc = texture de base\nCarré rouge = texture résolution 256 pixels\nCarré bleu = texture résolution 128 pixels\nCarré vert = texture résolution 64 pixels\nCarré jaune = texture résolution 32 pixels\nCarré magenta = texture résolution 16 pixels\nCarré cyan = texture résolution 8 pixels\n" );

	RECT destRect4;
	SetRect( &destRect4, 5, 50, 0, 0 );

	m_pD3DxFont->DrawText( NULL, strDescription, -1, &destRect4, DT_NOCLIP, 
		D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );

}