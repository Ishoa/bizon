#include "SceneVertexBuffers.h"
#include "..\..\Valkyrie\Moteur\CameraTrackBall.h"
#include "..\..\Valkyrie\Moteur\CameraVolLibre.h"

#define NB_STAGE 2

// ############################## Vertex Positions #############################

CSceneVertexBuffer::Vertex g_cubeVertices[] =
{
	{-1.0f, 1.0f,-1.0f }, // 0
	{ 1.0f, 1.0f,-1.0f }, // 1
	{-1.0f,-1.0f,-1.0f }, // 2
	{ 1.0f,-1.0f,-1.0f }, // 3
	
	{-1.0f, 1.0f, 1.0f }, // 4
	{-1.0f,-1.0f, 1.0f }, // 5
	{ 1.0f, 1.0f, 1.0f }, // 6
	{ 1.0f,-1.0f, 1.0f }, // 7
	
	{-1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },

	{-1.0f, 1.0f,-1.0f },
	{ 1.0f, 1.0f,-1.0f },
	
	{-1.0f,-1.0f, 1.0f },
	{-1.0f,-1.0f,-1.0f },
	{ 1.0f,-1.0f, 1.0f },
	{ 1.0f,-1.0f,-1.0f },
	
	{ 1.0f, 1.0f,-1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f,-1.0f },
	{ 1.0f,-1.0f, 1.0f },
	
	{-1.0f, 1.0f,-1.0f },
	{-1.0f,-1.0f,-1.0f },
	{-1.0f, 1.0f, 1.0f },
	{-1.0f,-1.0f, 1.0f }
};


CSceneVertexBuffer::Vertex g_cubeVertices_indexed[] =
{
	{-1.0f, 1.0f,-1.0f}, // 0
	{ 1.0f, 1.0f,-1.0f}, // 1
	{-1.0f,-1.0f,-1.0f}, // 2
	{ 1.0f,-1.0f,-1.0f}, // 3
	{-1.0f, 1.0f, 1.0f}, // 4
	{-1.0f,-1.0f, 1.0f}, // 5
	{ 1.0f, 1.0f, 1.0f}, // 6
	{ 1.0f,-1.0f, 1.0f}  // 7
};

WORD g_cubeIndices[] =
{
	0, 1, 2, 3, // Quad 0
	4, 5, 6, 7, // Quad 1
	4, 6, 0, 1, // Quad 2
	5, 2, 7, 3, // Quad 3
	1, 6, 3, 7, // Quad 4
	0, 2, 4, 5  // Quad 5
};


CSceneVertexBuffer::Color g_cubeColors[] =
{
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 ) },

	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 ) },

	{ D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 ) },

	{ D3DCOLOR_COLORVALUE( 1.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 1.0, 0.0, 1.0 ) },

	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 1.0, 1.0 ) },

	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 1.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 1.0, 1.0 ) }
};

CSceneVertexBuffer::Color g_cubeColors_indexed[] =
{
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 ) },

	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 ) },
	{ D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 ) },
};

CSceneVertexBuffer::TexCoord g_cubeTexCoords[] =
{
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },

	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 1.0f },

	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },

    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 1.0f },

	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },

	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 1.0f }
};

CSceneVertexBuffer::TexCoord g_cubeTexCoords_indexed[] =
{
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },

	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 1.0f },

};
/*######################################################################*/
/*########################### SceneVertexBuffers #######################*/
/*######################################################################*/


CSceneVertexBuffer::CSceneVertexBuffer(CMoteur* pMoteur):CScene(pMoteur)
{
	m_pVertexBuffer   = NULL;
	m_pColorBuffer    = NULL;
	m_pTexCoordBuffer = NULL;
	m_pIndexBuffer = NULL;

	m_bUseIndexedGeometry = false;

	m_QualiterFiltrage = 1;
}

CSceneVertexBuffer::~CSceneVertexBuffer()
{
	Console << "Deconstructeur CSceneVertexBuffer" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneVertexBuffer::ConfirmeFonction3D(D3DCAPS9* pCaps)
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
bool CSceneVertexBuffer::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	m_pCameraParDefaut = new CCameraTrackBall(this);
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 0.0f, 5.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return true;
}

// Création des objets 3D de la scène
bool CSceneVertexBuffer::CreationObjet()
{
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if(!m_pTexture.chargerTexture( m_pD3DDevice, "..\\Donnees\\Textures\\env2.bmp" ))
	{
		Console<<" erreur chargement Env2.bmp"<<endl;
		return false;
	}

	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);

	//
	// Create a vertex buffer that contains only the cube's vertex data
	//

	m_pD3DDevice->CreateVertexBuffer( sizeof(g_cubeVertices_indexed), 0, 0, D3DPOOL_DEFAULT, &m_pVertexBufferIndexed, NULL );
	
	void *pVerticesIndexed = NULL;

    m_pVertexBufferIndexed->Lock( 0, sizeof(g_cubeVertices_indexed), (void**)&pVerticesIndexed, 0 );
    memcpy( pVerticesIndexed, g_cubeVertices_indexed, sizeof(g_cubeVertices_indexed) );
    m_pVertexBufferIndexed->Unlock();

	m_pD3DDevice->CreateVertexBuffer( 24*sizeof(Vertex), 0, 0, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL );
	
	void *pVertices = NULL;

    m_pVertexBuffer->Lock( 0, sizeof(g_cubeVertices), (void**)&pVertices, 0 );
    memcpy( pVertices, g_cubeVertices, sizeof(g_cubeVertices) );
    m_pVertexBuffer->Unlock();


	//
	// Create a vertex buffer that contains only the cube's color data
	//

	m_pD3DDevice->CreateVertexBuffer( sizeof(g_cubeColors_indexed), 0, 0, D3DPOOL_DEFAULT, &m_pColorBufferIndexed, NULL );
	void *pColorsIndexed = NULL;

	m_pColorBufferIndexed->Lock( 0, sizeof(g_cubeColors_indexed), (void**)&pColorsIndexed, 0 );
	memcpy( pColorsIndexed, g_cubeColors_indexed, sizeof(g_cubeColors_indexed) );
	m_pColorBufferIndexed->Unlock();


	m_pD3DDevice->CreateVertexBuffer( sizeof(g_cubeColors_indexed), 0, 0, D3DPOOL_DEFAULT, &m_pColorBuffer, NULL );
	void *pColors = NULL;

	m_pColorBuffer->Lock( 0, sizeof(g_cubeColors), (void**)&pColors, 0 );
	memcpy( pColors, g_cubeColors, sizeof(g_cubeColors) );
	m_pColorBuffer->Unlock();

	//
	// Create a vertex buffer that contains only the cube's texture coordinate data
	//

	m_pD3DDevice->CreateVertexBuffer( sizeof(g_cubeTexCoords_indexed), 0, 0, D3DPOOL_DEFAULT, &m_pTexCoordBufferIndexed, NULL );
	void *pTexCoordsIndexed = NULL;

	m_pTexCoordBufferIndexed->Lock( 0, sizeof(g_cubeTexCoords_indexed), (void**)&pTexCoordsIndexed, 0 );
	memcpy( pTexCoordsIndexed, g_cubeTexCoords_indexed, sizeof(g_cubeTexCoords_indexed) );
	m_pTexCoordBufferIndexed->Unlock();


	m_pD3DDevice->CreateVertexBuffer( sizeof(g_cubeTexCoords), 0, 0, D3DPOOL_DEFAULT, &m_pTexCoordBuffer, NULL );
	void *pTexCoords = NULL;

	m_pTexCoordBuffer->Lock( 0, sizeof(g_cubeTexCoords), (void**)&pTexCoords, 0 );
	memcpy( pTexCoords, g_cubeTexCoords, sizeof(g_cubeTexCoords) );
	m_pTexCoordBuffer->Unlock();

	//
	// Create an index buffer to use with our indexed vertex buffer...
	//

	m_pD3DDevice->CreateIndexBuffer( 24*sizeof(WORD),
									 0,
									 D3DFMT_INDEX16,
									 D3DPOOL_DEFAULT,
									 &m_pIndexBuffer,
									 NULL );
	void *pIndices = NULL;

	m_pIndexBuffer->Lock( 0, sizeof(g_cubeIndices), (void**)&pIndices, 0 );
	memcpy( pIndices, g_cubeIndices, sizeof(g_cubeIndices) );
	m_pIndexBuffer->Unlock();


	//
	// Create a vertex declaration so we can describe to Direct3D how we'll 
	// be passing our data to it.
	//

	D3DVERTEXELEMENT9 dwDecl[] = 
	{
		//  Stream  Offset         Type                   Method                 Usage          Usage Index       
		{     0,      0,    D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0      },
		{     1,      0,    D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,        0      },
		{     2,      0,    D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0      },
		D3DDECL_END()
	};

	m_pD3DDevice->CreateVertexDeclaration( dwDecl, &m_pVertexDeclaration );

	// On initialise la lumière 0
	//SetLumiere(0, &m_Lumiere, true);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	DxDataBase.affichageStatus();
	return true;
}

// Destruction des resource utilisé par les objets
void CSceneVertexBuffer::DestructionObjet()
{

	m_pTexture.Release();
	SAFE_RELEASE(m_pVertexBuffer); 
	SAFE_RELEASE(m_pColorBuffer); 
	SAFE_RELEASE(m_pTexCoordBuffer); 
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBufferIndexed);
	SAFE_RELEASE(m_pColorBufferIndexed);
	SAFE_RELEASE(m_pTexCoordBufferIndexed);
	SAFE_RELEASE(m_pVertexDeclaration);

}


// Destruction des objets
void CSceneVertexBuffer::Destruction()
{

}


// Rendu 3D de la scène
void CSceneVertexBuffer::Rendu3D()
{

    m_pD3DDevice->SetTexture( 0, m_pTexture.getDXTexture() );
	m_pD3DDevice->SetVertexDeclaration( m_pVertexDeclaration );
	
	if(m_bUseIndexedGeometry == true)
	{
		m_pD3DDevice->SetStreamSource( 0, m_pVertexBufferIndexed,   0, sizeof(Vertex) );
		m_pD3DDevice->SetStreamSource( 1, m_pColorBufferIndexed,    0, sizeof(Color) );
		m_pD3DDevice->SetStreamSource( 2, m_pTexCoordBufferIndexed, 0, sizeof(TexCoord) );
		m_pD3DDevice->SetIndices( m_pIndexBuffer );

		m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, 4,  0, 2 );
		m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, 4,  4, 2 );
		m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, 4,  8, 2 );
		m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, 4, 12, 2 );
		m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, 4, 16, 2 );
		m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, 4, 20, 2 );
	}
	else
	{
		m_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer,   0, sizeof(Vertex) );
		m_pD3DDevice->SetStreamSource( 1, m_pColorBuffer,    0, sizeof(Color) );
		m_pD3DDevice->SetStreamSource( 2, m_pTexCoordBuffer, 0, sizeof(TexCoord) );

		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 );
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  4, 2 );
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  8, 2 );
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 12, 2 );
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 16, 2 );
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 20, 2 );
	}
}


// Calculs des animations de la scène
void CSceneVertexBuffer::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
}

// Test des entrées des périphériques
void CSceneVertexBuffer::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
{
	// On test si on a changé un paramètre de qualité
	if(KEYDOWN(DIK_F1))
	{
		m_QualiterFiltrage = 0;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F2))
	{
		m_QualiterFiltrage = 1;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F3))
	{
		m_QualiterFiltrage = 2;
		SetFiltreTexture(m_QualiterFiltrage);
	}

	if(KEYDOWN(DIK_F4))
	{
		m_bUseIndexedGeometry = true;
	}

	if(KEYDOWN(DIK_F5))
	{
		m_bUseIndexedGeometry = false;
	}

}

// Appliqué les paramètres de filtrage de texture
void CSceneVertexBuffer::SetFiltreTexture(int Type)
{
	switch(Type)
	{
		// Filtrage Bilinéaire
	case 0:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		}

		break;

		// Filtrage Tilinéaire
	case 1:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}
		break;

		// Filtrage Anisotropic
	case 2:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 16);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
		}
		break;
	}
}
