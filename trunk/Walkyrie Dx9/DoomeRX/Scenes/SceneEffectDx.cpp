#include "SceneEffectDX.h"
#include "CameraVolLibre.h"

#define NB_STAGE 2

CSceneEffectDX::CSceneEffectDX(CMoteur* pMoteur):CScene(pMoteur)
{
	m_pInterface = NULL;
	m_pSprite = NULL;
	m_pSphereMesh   = NULL;
	m_pCar = NULL;

	g_pEffect = NULL; 
}

CSceneEffectDX::~CSceneEffectDX()
{
	Console << "Deconstructeur CSceneTemplate" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneEffectDX::ConfirmeFonction3D(D3DCAPS9* pCaps)
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
bool CSceneEffectDX::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	//m_pCameraParDefaut = new CCameraTrackBall(this);
	m_pCameraParDefaut = new CCameraVolLibre(this);	
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 1.0f, -5.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pLumiere[0].SetPonctuelle(&D3DXVECTOR3(5.0f, 5.0f, 5.0f), 100.0f, 0.8f);
	m_pLumiere[0].SetAmbiante(0.9f, 0.9f, 0.9f);
	m_pLumiere[0].SetDiffuse(1.0f, 0.0f, 0.0f);

	m_pLumiere[1].SetPonctuelle(&D3DXVECTOR3(-5.0f, 5.0f, 5.0f), 100.0f, 0.8f);
	m_pLumiere[1].SetAmbiante(0.9f, 0.9f, 0.9f);
	m_pLumiere[1].SetDiffuse(0.0f, 1.0f, 0.0f);

	m_pLumiere[2].SetPonctuelle(&D3DXVECTOR3(5.0f, 5.0f, -5.0f), 100.0f, 0.8f);
	m_pLumiere[2].SetAmbiante(0.9f, 0.9f, 0.9f);
	m_pLumiere[2].SetDiffuse(0.0f, 0.0f, 1.0f);

	m_pLumiere[3].SetPonctuelle(&D3DXVECTOR3(-5.0f, 5.0f, -5.0f), 100.0f, 0.8f);
	m_pLumiere[3].SetAmbiante(0.9f, 0.9f, 0.9f);
	m_pLumiere[3].SetDiffuse(1.0f, 0.0f, 1.0f);

	m_pSprite = new CSprite3D(this);
	m_pCar = new CMesh(this);

	// de Base la lumiere est activé
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pInterface = new CInterface(this);
	
	m_bTexture = true;
	m_nLightActive = MAX_LIGHTS;

	return true;



}

// Création des objets 3D de la scène
bool CSceneEffectDX::CreationObjet()
{
	// Create a sphere mesh to represent a point light.
	D3DXCreateSphere(m_pD3DDevice, 0.05f, 8, 8, &m_pSphereMesh, NULL);

	if(!m_pSprite->ChargerSprites3D("..\\Donnees\\Textures\\Floor.jpg",10.0f,10.0f,5.0f,5.0f))
		return false;
	m_pSprite->SetRotationX(D3DXToRadian(90.0f));


	//if(!m_pCar->ChargerMesh("..\\Mesh\\airplane 2.x"))
	//if(!m_pCar->ChargerMesh("..\\Mesh\\Cube.x"))	
	if(!m_pCar->ChargerMesh("..\\Media\\Tiny\\tiny.x"))
	//if(!m_pCar->ChargerMesh("..\\Mesh\\Audi-Avus.x"))
		return false;

  	if(FAILED(m_pCar->SetFVF(D3DFVF_XYZ | D3DFVF_TEX0 |D3DFVF_NORMAL)))
  		return false;

	
// 	if(FAILED(m_pCar->SetVertexDecl()))
// 		return false;

	m_pCar->SetTranslationY(1.0f);
	m_pCar->SetRotationX(D3DXToRadian(-90.0f));
	m_pCar->SetScaleXYZ(0.01f,0.01f,0.01f);
	//m_pCar->SetScaleXYZ(4.0f,4.0f,4.0f);

	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

	if(FAILED(D3DXCreateEffectFromFile( m_pD3DDevice, "..\\Shader\\BasicHLSL.fx", NULL, NULL, dwShaderFlags, NULL, &g_pEffect, NULL ) ))
	{
		return false;
	}

	if(!m_pInterface->Creation())
		return false;

	// Initialisation de la lumière 1 et activation
// 	SetLumiere(0, &m_pLumiere[0], true);
// 	SetLumiere(0, &m_pLumiere[1], true);
// 	SetLumiere(0, &m_pLumiere[2], true);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	return true;
}

// Destruction des resource utilisé par les objets
void CSceneEffectDX::DestructionObjet()
{
	SAFE_RELEASE(m_pInterface);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pCar);
	SAFE_RELEASE(g_pEffect);

}

// Destruction des objets
void CSceneEffectDX::Destruction()
{
	SAFE_DELETE(m_pCar);
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(g_pEffect);

}

void CSceneEffectDX::DebutRendu3D()
{

	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0 );

	m_pD3DDevice->BeginScene();
};

// Rendu 3D de la scène
void CSceneEffectDX::Rendu3D()
{


	D3DXMATRIX mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;

	D3DXMATRIXA16 mWorldViewProjection;
	D3DXVECTOR3 vLightPos[MAX_LIGHTS];
	D3DXCOLOR vLightDiffuse[MAX_LIGHTS];

	UINT iPass, cPasses;

	mWorld = m_pCar->GetWorldMatrix();
	mProj = m_pCameraCourante->GetMatriceProjection();
	mView = m_pCameraCourante->GetMatriceVision();
	
	mWorldViewProjection = mWorld * mView * mProj;

	vLightPos[0]= m_pLumiere[0].GetLumiere()->Position;
	vLightPos[1]= m_pLumiere[1].GetLumiere()->Position;
	vLightPos[2]= m_pLumiere[2].GetLumiere()->Position;
	vLightPos[3]= m_pLumiere[3].GetLumiere()->Position;

	vLightDiffuse[0]= m_pLumiere[0].GetLumiere()->Diffuse;
	vLightDiffuse[1]= m_pLumiere[1].GetLumiere()->Diffuse;
	vLightDiffuse[2]= m_pLumiere[2].GetLumiere()->Diffuse;
	vLightDiffuse[3]= m_pLumiere[3].GetLumiere()->Diffuse;

	g_pEffect->SetValue( "g_LightDir", vLightPos, sizeof( D3DXVECTOR3 ) * MAX_LIGHTS );
	g_pEffect->SetValue( "g_LightDiffuse", vLightDiffuse, sizeof( D3DXVECTOR4 ) * MAX_LIGHTS );

	g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) ;
	g_pEffect->SetMatrix( "g_mWorld", &mWorld ) ;
	g_pEffect->SetFloat( "g_fTime", m_fDeltatime  );

	D3DXCOLOR vWhite = D3DXCOLOR( 1, 1, 1, 1 );
	g_pEffect->SetValue( "g_MaterialDiffuseColor", &vWhite, sizeof( D3DXCOLOR ) ) ;
	g_pEffect->SetFloat( "g_fTime", 0.01f ) ;
// 	g_pEffect->SetInt( "g_nNumlights", m_nLightActive ) ;
// 	g_pEffect->SetBool("g_bTexture",true);
// 	g_pEffect->SetBool("g_bAnimate",true);

	//m_pCar->RenduMeshSampler();

	switch( m_nLightActive )
	{
		case 1:
			if(m_bTexture)
				g_pEffect->SetTechnique( "RenderSceneWithTexture1Light" ) ;
			else
				g_pEffect->SetTechnique( "RenderSceneNoTexture1Light" ) ;
			break;
		case 2:
			if(m_bTexture)
				g_pEffect->SetTechnique( "RenderSceneWithTexture2Light" ) ;
			else
				g_pEffect->SetTechnique( "RenderSceneNoTexture2Light" ) ;
			break;
		case 3:
			if(m_bTexture)
				g_pEffect->SetTechnique( "RenderSceneWithTexture3Light" ) ;
			else
				g_pEffect->SetTechnique( "RenderSceneNoTexture3Light" ) ;
			break;

		case 4:
			if(m_bTexture)
				g_pEffect->SetTechnique( "RenderSceneWithTexture4Light" ) ;
			else
				g_pEffect->SetTechnique( "RenderSceneNoTexture4Light" ) ;
			break;

		case 0:
				g_pEffect->SetTechnique( "RenderSceneNoTextureNoLight" ) ;
			break;
		
	}

	//m_pCar->GetDXMesh()->DrawSubset( 0 ) ;
	m_pCar->RenduMesh(g_pEffect,"g_MeshTexture","g_MaterialDiffuseColor","g_MaterialAmbientColor",NULL,NULL,NULL,true,true);


	m_pSprite->RenderSprite3D();

	for(int i=0; i<m_nLightActive; i++)
	{
		D3DXMatrixIdentity( &mWorld );
		D3DXMatrixTranslation( &mWorld, m_pLumiere[i].GetLumiere()->Position.x,
			m_pLumiere[i].GetLumiere()->Position.y,
			m_pLumiere[i].GetLumiere()->Position.z );

		m_pD3DDevice->SetTransform( D3DTS_WORLD, &mWorld );
		m_pSphereMesh->DrawSubset(0);
	}

	m_pInterface->afficher();
}

// Calculs des animations de la scène
void CSceneEffectDX::Animation(double TempsEcouler, double DeltaTemps)
{
	m_fDeltatime = (float)DeltaTemps;

	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();

	if(FAILED(g_pEffect->OnLostDevice()))
		Console<<"Device Effect Perdu"<<endl;
}

// Test des entrées des périphériques
void CSceneEffectDX::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
{

	if(KEYDOWN(DIK_3))
	{
		m_bTexture = true;
	}

	if(KEYDOWN(DIK_4))
	{
		m_bTexture = false;
	}

	if(KEYDOWN(DIK_0))
	{
			m_nLightActive = 0;
	}

	if(KEYDOWN(DIK_1))
	{
		m_nLightActive = 1;
	}

	if(KEYDOWN(DIK_2))
	{
		m_nLightActive = 2;
	}

	if(KEYDOWN(DIK_3))
	{
		m_nLightActive = 3;
	}

	if(KEYDOWN(DIK_4))
	{
		m_nLightActive = 4;
	}
}
