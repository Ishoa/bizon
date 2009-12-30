#include "SceneShadowVolume.h"
#include "../Moteur/CameraVolLibre.h"

#define NB_STAGE 2


CSceneShadowVolume::CSceneShadowVolume(CMoteur* pMoteur):CScene(pMoteur)
{
	m_QualiterFiltrage = 1;
	m_pInterface = NULL;
	m_pSprite = NULL;
	m_pTeapot = NULL;
	m_pAvion = NULL;
	m_pArche = NULL;
	m_pTest = NULL;
	m_pShape = NULL;
	m_bRenduShadowVolume = false;
	m_bShadowManagerLight0 = true;
	m_bShadowManagerLight1 = false;
	m_pSphereMesh   = NULL;
	m_fAngleLum = 0.0f;
	m_bPause = false;
	m_bWireFrame = false;
}

CSceneShadowVolume::~CSceneShadowVolume()
{
	Console << "Deconstructeur CSceneTemplate" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneShadowVolume::ConfirmeFonction3D(D3DCAPS9* pCaps)
{

	if( !( pCaps->StencilCaps & D3DSTENCILCAPS_TWOSIDED ) )
	{
		Console << "Le peripherique ne permet pas le D3DSTENCILCAPS_TWOSIDED" << endl;
	}
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
bool CSceneShadowVolume::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	//m_pCameraParDefaut = new CCameraTrackBall(this);
	m_pCameraParDefaut = new CCameraVolLibre(this);	
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 15.0f, 5.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	m_Lumiere.SetPonctuelle(&D3DXVECTOR3(3.0f, 12.0f, 3.0f),50.0f);
	m_Lumiere.SetAmbiante(0.5f, 0.5f, 0.5f);
	m_Lumiere.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_LumiereParDefaut.SetPonctuelle(&D3DXVECTOR3(3.0f, 12.0f, 3.0f),50.0f);
	m_LumiereParDefaut.SetAmbiante(0.5f, 0.5f, 0.5f);
	m_LumiereParDefaut.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_pSprite = new CSprite3D(this);
	m_pTeapot = new CMesh(this);
	m_pAvion = new CMesh(this);
	m_pArche = new CMesh(this);
	m_pTest = new CMesh(this);
	m_pShape = new CMesh(this);

	m_mat.SetAmbianteDiffuse(1.0f, 1.0f, 0.0f);

	m_CShadowVolumeManagerLight0 = new CShadowVolumeManager(this);
	m_CShadowVolumeManagerLight1 = new CShadowVolumeManager(this);

	// de Base la lumiere est activé
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pInterface = new CInterface(this);

	m_CShadowVolumeManagerLight0->setTwoSidedStencilsAvailable(false);

	return true;


}

// Création des objets 3D de la scène
bool CSceneShadowVolume::CreationObjet()
{
	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);
	
	// Create a sphere mesh to represent a point light.
	D3DXCreateSphere(m_pD3DDevice,0.5f, 8, 8, &m_pSphereMesh, NULL);

	// Initialisation de la lumière 1 et activation

	m_CShadowVolumeManagerLight0->InitShadowVolumeManager(0x7f000000);
	m_CShadowVolumeManagerLight1->InitShadowVolumeManager(0x7b000000);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();


	if(!m_pTeapot->ChargerMesh("..\\Mesh\\teapot.x"))
	{	
		Console<<"erreur chargement teapot"<<endl;
		return false;
	}

	// Chargement de l'avion 
	if(!m_pAvion->ChargerMesh("..\\Mesh\\airplane 2.x"))
	{	
		Console<<"erreur chargement avion"<<endl;
		return false;
	}

	// Chargement de l'avion 
	if(!m_pArche->ChargerMesh("..\\Mesh\\Archway_Max.x"))
	{	
		Console<<"erreur chargement Archway_Max.x"<<endl;
		return false;
	}

	// Chargement de l'avion 
	if(!m_pShape->ChargerMesh("..\\Mesh\\shapes1.x"))
	{	
		Console<<"erreur chargement shapes1.x"<<endl;
		return false;
	}

	if(!m_pTest->ChargerMesh("..\\Mesh\\room2.x"))
	//if(!m_pTest->ChargerMesh("..\\Mesh\\Cube.x"))
	{	
		Console<<"erreur chargement test"<<endl;
		return false;
	}

	if(!m_pSprite->ChargerSprites3D("..\\Donnees\\Textures\\Floor.jpg",50.0f,50.0f,5.0f,5.0f))
		return false;

	m_pSprite->SetRotationX(D3DXToRadian(90.0f));

	m_pTeapot->SetTranslationY(5.0f);
	m_pTeapot->SetTranslationZ(10.0f);
	m_pTeapot->SetRotationX(D3DXToRadian(45.0f));
	m_pTeapot->SetRotationY(D3DXToRadian(45.0f));

	m_pAvion->SetTranslationY(6.0f);
	m_pAvion->SetTranslationX(10.0f);
	m_pAvion->SetScaleXYZ(0.5f,0.5f,0.5f);

	m_pArche->SetScaleXYZ(0.2f,0.2f,0.2f);

	m_pShape->SetTranslationY(3.0f);
	m_pShape->SetTranslationX(-10.0F);
	m_pShape->SetScaleXYZ(0.2f,0.2f,0.2f);

	m_pTest->SetTranslationY(3.0f);
	m_pTest->SetTranslationX(10.0f);
	m_pTest->SetTranslationZ(10.0f);
	//m_pTest->SetScaleXYZ(5.0f,5.0f,5.0f);

	m_CShadowVolumeManagerLight0->Reset();
	m_CShadowVolumeManagerLight1->Reset();

	m_CShadowVolumeManagerLight0->InitLight(m_LumiereParDefaut.GetLumiere()->Position);
 	m_CShadowVolumeManagerLight1->InitLight(m_Lumiere.GetLumiere()->Position);

	m_CShadowVolumeManagerLight0->AddAndBuildShadowVolumes(m_pTeapot);
	m_CShadowVolumeManagerLight1->AddAndBuildShadowVolumes(m_pTeapot);
 	m_CShadowVolumeManagerLight0->AddAndBuildShadowVolumes(m_pAvion);
  	m_CShadowVolumeManagerLight1->AddAndBuildShadowVolumes(m_pAvion);

 	m_CShadowVolumeManagerLight0->AddAndBuildShadowVolumes(m_pArche);
	m_CShadowVolumeManagerLight1->AddAndBuildShadowVolumes(m_pArche);

  	m_CShadowVolumeManagerLight0->AddAndBuildShadowVolumes(m_pShape);
	m_CShadowVolumeManagerLight1->AddAndBuildShadowVolumes(m_pShape);

  	m_CShadowVolumeManagerLight0->AddAndBuildShadowVolumes(m_pTest);
	m_CShadowVolumeManagerLight1->AddAndBuildShadowVolumes(m_pTest);

	if(!m_pInterface->Creation())
		return false;

	SetLumiere(0, &m_LumiereParDefaut, true);
	SetLumiere(1, &m_Lumiere, true);

	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );

	return true;
}

// Destruction des resource utilisé par les objets
void CSceneShadowVolume::DestructionObjet()
{
	m_pTeapot->Release();
	m_pAvion->Release();
	m_pArche->Release();
	m_pShape->Release();
	m_pTest->Release();
	m_pInterface->Release();
	m_pSprite->Release();
	m_pSphereMesh->Release();
	
	m_CShadowVolumeManagerLight0->clearShadowVolumes();
	m_CShadowVolumeManagerLight1->clearShadowVolumes();

	m_CShadowVolumeManagerLight0->Release();
	m_CShadowVolumeManagerLight1->Release();


}

// Destruction des objets
void CSceneShadowVolume::Destruction()
{

}

void CSceneShadowVolume::DebutRendu3D()
{


	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0 );

	m_pD3DDevice->BeginScene();
};

// Rendu 3D de la scène
void CSceneShadowVolume::Rendu3D()
{

	//
	// Using the light's object-space position, build the shadow volume 
	// from the teapot's DX9 mesh.
	//

	if(m_bShadowManagerLight0 && !m_bPause)
	{
		m_CShadowVolumeManagerLight0->Reset();
		m_CShadowVolumeManagerLight0->InitLight(m_LumiereParDefaut.GetLumiere()->Position);
		m_CShadowVolumeManagerLight0->ReBuildShadowVolume();
	}

	if(m_bShadowManagerLight1 && !m_bPause)
	{
		m_CShadowVolumeManagerLight1->Reset();
		m_CShadowVolumeManagerLight1->InitLight(m_Lumiere.GetLumiere()->Position);
		m_CShadowVolumeManagerLight1->ReBuildShadowVolume();
	}

	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	// Rendu de la Teapot
	m_pTeapot->RenduMeshSampler();
	m_pAvion->RenduMeshSampler();
	m_pArche->RenduMeshSampler();
	m_pShape->RenduMeshSampler();
	m_pTest->RenduMeshSampler();

	m_pSprite->RenderSprite3D();

	if (m_bWireFrame)
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	else
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );


// 	m_CShadowVolumeManagerLight0->RenduShadow();
// 	m_CShadowVolumeManagerLight1->RenduShadow();

	if(m_bShadowManagerLight0)
		m_CShadowVolumeManagerLight0->renderShadowToStencilBuffer();

	if(m_bShadowManagerLight1)
	m_CShadowVolumeManagerLight1->renderShadowToStencilBuffer();

	if(m_bShadowManagerLight0)
		m_CShadowVolumeManagerLight0->renderShadowToScene();

	if(m_bShadowManagerLight1)
		m_CShadowVolumeManagerLight1->renderShadowToScene();


	if(m_bRenduShadowVolume)
	{
		if(m_bShadowManagerLight0)
			m_CShadowVolumeManagerLight0->RenduShadowVolume();
		if(m_bShadowManagerLight1)
			m_CShadowVolumeManagerLight1->RenduShadowVolume();
	}

	SetMaterielle( &m_mat );
	m_pD3DDevice->SetTexture(0,NULL);

	D3DXMATRIX matWorld;
	
	if(m_bShadowManagerLight0)
	{
		D3DXMatrixIdentity( &matWorld );
		D3DXMatrixTranslation( &matWorld, m_LumiereParDefaut.GetLumiere()->Position.x,
			m_LumiereParDefaut.GetLumiere()->Position.y,
			m_LumiereParDefaut.GetLumiere()->Position.z );

		m_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pSphereMesh->DrawSubset(0);
	}

	if(m_bShadowManagerLight1)
	{
		D3DXMatrixIdentity( &matWorld );
		D3DXMatrixTranslation( &matWorld, m_Lumiere.GetLumiere()->Position.x,
			m_Lumiere.GetLumiere()->Position.y,
			m_Lumiere.GetLumiere()->Position.z );
		
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pSphereMesh->DrawSubset(0);
	}
	m_pInterface->afficher();
}

// Calculs des animations de la scène
void CSceneShadowVolume::Animation(double TempsEcouler, double DeltaTemps)
{

	D3DXVECTOR3 LumPos = m_LumiereParDefaut.GetLumiere()->Position;
	if(!m_bPause)
	{
		m_fAngleLum += 0.2f;
		if(m_fAngleLum > 360.0f)
			m_fAngleLum = 0.0f;
	}
	m_pTest->SetRotationY(D3DXToRadian(m_fAngleLum));
	m_LumiereParDefaut.SetPonctuelle(&D3DXVECTOR3(8 * cos(D3DXToRadian(m_fAngleLum)), LumPos.y, 8 * sin(D3DXToRadian(m_fAngleLum))),50.0f);

	m_Lumiere.SetPonctuelle(&D3DXVECTOR3(8 * cos(D3DXToRadian(360.0f - m_fAngleLum)), LumPos.y, 8 * sin(D3DXToRadian(360.0f - m_fAngleLum))),50.0f);
	
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();


}

// Test des entrées des périphériques
void CSceneShadowVolume::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
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

		m_bRenduShadowVolume = true;
	}

	if(KEYDOWN(DIK_F5))
	{

		m_bRenduShadowVolume = false;
	}
	if(KEYDOWN(DIK_F6))
	{

		m_CShadowVolumeManagerLight0->setTwoSidedStencilsAvailable(true);
	}

	if(KEYDOWN(DIK_F7))
	{

		m_CShadowVolumeManagerLight0->setTwoSidedStencilsAvailable(false);
	}

	if(KEYDOWN(DIK_F8))
	{

		m_bWireFrame = true;
	}
	if(KEYDOWN(DIK_F9))
	{

		m_bWireFrame = false;
	}


	if(KEYDOWN(DIK_1))
	{

		m_bShadowManagerLight0 = true;
	}

	if(KEYDOWN(DIK_2))
	{

		m_bShadowManagerLight0 = false;
	}

	if(KEYDOWN(DIK_3))
	{

		m_bShadowManagerLight1 = true;
	}

	if(KEYDOWN(DIK_4))
	{

		m_bShadowManagerLight1 = false;
	}

	if(KEYDOWN(DIK_9))
	{
		m_bPause = true;
	}

	if(KEYDOWN(DIK_0))
	{
		m_bPause = false;
	}
}

// Appliqué les paramètres de filtrage de texture
void CSceneShadowVolume::SetFiltreTexture(int Type)
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
