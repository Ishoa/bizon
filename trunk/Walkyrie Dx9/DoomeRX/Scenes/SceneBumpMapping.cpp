#include "SceneBumpMapping.h"
#include "CameraTrackBall.h"

#define NB_STAGE 2

const int NUM_VERTICES = 36;

CBumpMappingObjet::Vertex g_cubeVertices[NUM_VERTICES] =
{
	//     x     y     z      nx    ny    nz                         r    g    b    a        tu1 tv1    tu2 tv2

		
	// Front Face
		{-1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },
		{ 1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },
		{-1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, },
		
		{-1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, },
		{ 1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },
		{ 1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },

		// Back Face
		{-1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },
		{-1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },
		{ 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },

		{ 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },
		{-1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },
		{ 1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, },



		// Top Face
		{-1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },
		{ 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },
		{-1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, },

		
		{-1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, },
		{ 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },
		{ 1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },

		// Bottom Face
		{-1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, },
		{-1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },
		{ 1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },

		{ 1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },		
		{-1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },
		{ 1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },


		// Right Face
		{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },
		{ 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },
		{ 1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, },
		
		
		{ 1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, },
		{ 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },
		{ 1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },


		// Left Face
		{-1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,0.0f, 1.0f,0.0f, },
		{-1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },
		{-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },
		
		
		{-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,0.0f, 0.0f,0.0f, },
		{-1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   1.0f,1.0f, 1.0f,1.0f, },
		{-1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),   0.0f,1.0f, 0.0f,1.0f, }
		
	
};


CSceneBumpMapping::CSceneBumpMapping(CMoteur* pMoteur):CScene(pMoteur)
{
	m_QualiterFiltrage = 2;
	m_pSphereMesh   = NULL;
	m_pObjetBumpMappingFloor = NULL;
	m_pObjetBumpMappingCross = NULL;
	m_pObjetBumpMappingStone = NULL;
	m_pObjetBumpMappingTerre = NULL;

	m_eTYPEAFFICHAGE = STONE;
}

CSceneBumpMapping::~CSceneBumpMapping()
{
	Console << "Deconstructeur CSceneBumpMapping" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneBumpMapping::ConfirmeFonction3D(D3DCAPS9* pCaps)
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
bool CSceneBumpMapping::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	m_pCameraParDefaut = new CCameraTrackBall(this);
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 0.0f, -10.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f,0.0f));

	m_Lumiere.SetPonctuelle(&D3DXVECTOR3(5.0f, 5.0f, 5.0f), 100.0f, 0.8f);
	m_Lumiere.SetAmbiante(0.9f, 0.9f, 0.9f);
	m_Lumiere.SetDiffuse(1.0f, 1.0f, 1.0f);
	m_pD3DDevice->LightEnable(0,TRUE);

	m_mat.SetAmbianteDiffuse(0.4f, 0.4f, 0.4f);

	//m_pD3DDevice->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_COLORVALUE(0.5,0.5,0.5,1.0));

	m_pObjetBumpMappingStone = new CBumpMappingObjet(this);
	m_pObjetBumpMappingFloor = new CBumpMappingObjet(this);
	m_pObjetBumpMappingCross = new CBumpMappingObjet(this);
	m_pObjetBumpMappingTerre = new CBumpMappingObjet(this);

	// de Base la lumiere est activé
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);



	return true;
}

// Création des objets 3D de la scène
bool CSceneBumpMapping::CreationObjet()
{
	// Create a sphere mesh to represent a point light.
	D3DXCreateSphere(m_pD3DDevice, 0.05f, 8, 8, &m_pSphereMesh, NULL);

	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);

	if(!m_pObjetBumpMappingCross->CreationObjectVertexBuffer(g_cubeVertices,NUM_VERTICES, 
													"..\\Donnees\\Bump Mapping\\quad_dot3\\cross.bmp",
													"..\\Donnees\\Bump Mapping\\quad_dot3\\cross_normal.bmp"))
	{
		Console<<"erreur chargement objet bump mapping Cross"<<endl;
		return false;
	}

	
	if(!m_pObjetBumpMappingFloor->CreationObjectVertexBuffer(g_cubeVertices,NUM_VERTICES, 
													"..\\Donnees\\Bump Mapping\\quad_dot3\\floor.bmp",
													"..\\Donnees\\Bump Mapping\\quad_dot3\\normal_floor.bmp"))
	{
		Console<<"erreur chargement objet bump mapping Floor"<<endl;
		return false;
	}


	if(!m_pObjetBumpMappingStone->CreationObjectVertexBuffer(g_cubeVertices,NUM_VERTICES, 
													"..\\Donnees\\Bump Mapping\\quad_dot3\\stone_wall.bmp",
													"..\\Donnees\\Bump Mapping\\quad_dot3\\stone_wall_normal_map.bmp"))

	{
		Console<<"erreur chargement objet bump mapping Stone"<<endl;
		return false;
	}
	



	float m_fTaille = 1.0f;
	int m_ndiv=10;
	int m_nvert = (m_ndiv+1)*(2*m_ndiv+1);
	int m_nfac = ((m_ndiv-2)*(2*m_ndiv)*2 + 2*(2*m_ndiv))*3;

/*
	float x, y, z;
	float nx, ny, nz;
	DWORD diffuse;
	float tu1, tv1;
	float tu2, tv2;
*/

	// definition d'une sphere
	CBumpMappingObjet::Vertex*  sph = new CBumpMappingObjet::Vertex[m_nvert];
	//VERTEX_PLANETE  sph[nvert];
	{
		float   ph=-D3DX_PI/2, th, da = D3DX_PI/m_ndiv;
		float	r, pht, tht;
		int	    i,j,k=0;
		for(i=0; i<=m_ndiv;i++,ph+=da) {
			for(j=0,th=0;j<=2*m_ndiv;j++,th+=da,k++) {
				sph[k].x = cosf(th)*cosf(ph);
				sph[k].y = sinf(th)*cosf(ph);
				sph[k].z = sinf(ph); 
				sph[k].nx = sph[k].x;
				sph[k].ny = sph[k].y;
				sph[k].nz = sph[k].z;
				sph[k].x *= m_fTaille;
				sph[k].y *= m_fTaille;
				sph[k].z *= m_fTaille;
				sph[k].diffuse = D3DCOLOR_XRGB(255,255,255);
				//sph[k].tu1 = i/(float)m_ndiv;
				//sph[k].tv1 = j/(float)(2*m_ndiv);


				r  = sqrtf( (sph[k].x * sph[k].x) + (sph[k].y * sph[k].y) + (sph[k].z * sph[k].z) );
				pht = asinf( sph[k].z / r );
				if (cosf(pht) != 0.f) tht = atan2f( sph[k].y , sph[k].x );
				else tht = 0.f;
				sph[k].tu1 = 1.f - fabsf( tht / D3DX_PI );
				sph[k].tv1 = pht / D3DX_PI + 0.5f;

				sph[k].tu2 = sph[k].tu1;
				sph[k].tv2 = sph[k].tv1;
			}
		}
	}


	if(!m_pObjetBumpMappingTerre->CreationObjectVertexBuffer(g_cubeVertices,NUM_VERTICES, //sph,m_nvert, 
		"..\\Donnees\\Bump Mapping\\earth\\Earth__land_ocean_ice_2048.jpg",
		"..\\Donnees\\Bump Mapping\\earth\\earthNormalMap_2048.png"))

	{
		Console<<"erreur chargement objet bump mapping Terre"<<endl;
		return false;
	}



	SetMaterielle( &m_mat );
	// Initialisation de la lumière 0 et activation
	SetLumiere(0, &m_Lumiere, true);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();
	
	DxDataBase.affichageStatus();
	return true;
}

// Destruction des resource utilisé par les objets
void CSceneBumpMapping::DestructionObjet()
{
	m_pObjetBumpMappingStone->Release();
	m_pObjetBumpMappingCross->Release();
	m_pObjetBumpMappingFloor->Release();
	m_pObjetBumpMappingTerre->Release();
	m_pSphereMesh->Release();
}

// Destruction des objets
void CSceneBumpMapping::Destruction()
{

	SAFE_DELETE(m_pObjetBumpMappingStone);
	SAFE_DELETE(m_pObjetBumpMappingCross);
	SAFE_DELETE(m_pObjetBumpMappingFloor);
	SAFE_DELETE(m_pObjetBumpMappingTerre);
}

// Rendu 3D de la scène
void CSceneBumpMapping::Rendu3D()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	switch(m_eTYPEAFFICHAGE)
	{
		case STONE:m_pObjetBumpMappingStone->RenduModel(m_Lumiere, &matWorld);break;
		case CROSS:m_pObjetBumpMappingCross->RenduModel(m_Lumiere, &matWorld);break;
		case FLOOR:m_pObjetBumpMappingFloor->RenduModel(m_Lumiere, &matWorld);break;
		case TERRE:m_pObjetBumpMappingTerre->RenduModel(m_Lumiere, &matWorld);break;
		default : break;
	}
	

	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixTranslation( &matWorld, m_Lumiere.GetLumiere()->Position.x,
		m_Lumiere.GetLumiere()->Position.y,
		m_Lumiere.GetLumiere()->Position.z );

	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
	m_pSphereMesh->DrawSubset(0);

}

// Calculs des animations de la scène
void CSceneBumpMapping::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
}

// Test des entrées des périphériques
void CSceneBumpMapping::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
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

	if(KEYDOWN(DIK_1))
	{
		m_eTYPEAFFICHAGE = STONE;
	}

	if(KEYDOWN(DIK_2))
	{
		m_eTYPEAFFICHAGE = CROSS;
	}

	if(KEYDOWN(DIK_3))
	{
		m_eTYPEAFFICHAGE = FLOOR;
	}

	if(KEYDOWN(DIK_4))
	{
		m_eTYPEAFFICHAGE = TERRE;
	}

	if(KEYDOWN(DIK_B))
	{
				
		m_pObjetBumpMappingStone->m_bDoDot3BumpMapping = true;
		m_pObjetBumpMappingCross->m_bDoDot3BumpMapping = true;
		m_pObjetBumpMappingFloor->m_bDoDot3BumpMapping = true;
		m_pObjetBumpMappingTerre->m_bDoDot3BumpMapping = true;
	}
	if(KEYDOWN(DIK_N))
	{

		m_pObjetBumpMappingStone->m_bDoDot3BumpMapping = false;
		m_pObjetBumpMappingCross->m_bDoDot3BumpMapping = false;
		m_pObjetBumpMappingFloor->m_bDoDot3BumpMapping = false;
		m_pObjetBumpMappingTerre->m_bDoDot3BumpMapping = false;

	}

}

// Appliqué les paramètres de filtrage de texture
void CSceneBumpMapping::SetFiltreTexture(int Type)
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
