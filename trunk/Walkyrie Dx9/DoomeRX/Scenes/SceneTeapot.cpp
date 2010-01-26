#include "SceneTeapot.h"
#include "..\..\Valkyrie\Moteur\CameraTrackBall.h"
#include "..\..\Valkyrie\Moteur\CameraVolLibre.h"

#define NB_STAGE 2

CSceneTeapot::CSceneTeapot(CMoteur* pMoteur):CScene(pMoteur)
{
	m_pTeapot = NULL;
	m_pSprite = NULL;
	m_pInterface = NULL;

	m_QualiterFiltrage = 1;
}

CSceneTeapot::~CSceneTeapot()
{
	Console << "Deconstructeur CSceneTeapot" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneTeapot::ConfirmeFonction3D(D3DCAPS9* pCaps)
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
bool CSceneTeapot::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	m_pCameraParDefaut = new CCameraTrackBall(this);
	//m_pCameraParDefaut = new CCameraVolLibre(this);	
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 0.0f, 7.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	
	m_Lumiere.SetPonctuelle(&D3DXVECTOR3(2.0f, -1.0f, -1.0f),50.0f);
	m_Lumiere.SetAmbiante(0.1f, 0.1f, 0.1f);
	m_Lumiere.SetDiffuse(1.0f, 0.0f, 0.0f);
	

	m_pTeapot = new CMeshCollision(this);
	//m_pTeapot = new CMesh(m_pD3DDevice);
	m_pSprite = new CSprite3D(this);

	m_pInterface = new CInterface(this);
	return true;
}


// Création des objets 3D de la scène
bool CSceneTeapot::CreationObjet()
{
	m_pSprite->ChargerSprites3D("..\\Donnees\\Textures\\Wall.jpg");

	// Chargement de l'avion 
	if(!m_pTeapot->ChargerMesh("..\\Mesh\\teapot.x"))
	{	
		Console<<"erreur chargement teapot"<<endl;
		return false;
	}


	if(!m_pInterface->Creation())
	{
		Console<<"erreur creation interface"<<endl;
		return false;
	}


	D3DXMATRIXA16 mTeaWorldMatrix;
	D3DXMATRIXA16 mTeaWMatrixRotation;
	D3DXMATRIXA16 mTeaMatrixTranslation;

	//D3DXMatrixRotationX( &mTeaWMatrixRotation, D3DXToRadian( 45.0f ));
	//D3DXMatrixRotationY( &mTeaWMatrixRotation, D3DXToRadian( 45.0f ));
	//D3DXMatrixRotationZ( &mTeaWMatrixRotation, D3DXToRadian( 45.0f ));


	//D3DXMatrixRotationYawPitchRoll( &mTeaWMatrixRotation, D3DXToRadian( 45.0f ),D3DXToRadian( 45.0f ),D3DXToRadian( 45.0f ) );
	//D3DXMatrixTranslation( &mTeaMatrixTranslation, 0.0f, 1.0f, 0.0f);
	//D3DXMatrixMultiply( &mTeaWorldMatrix, &mTeaWMatrixRotation, &mTeaMatrixTranslation );
	//m_pTeapot->SetWorldMatrix(&mTeaMatrixTranslation);



	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);

	// Initialisation de la lumière 1 et activation

	// On initialise la lumière 0
	SetLumiere(0, &m_Lumiere, true);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, true);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	return true;
}

// Destruction des resource utilisé par les objets
void CSceneTeapot::DestructionObjet()
{
	m_pTeapot->Release();
	m_pSprite->Release();
	m_pInterface->Release();
}

// Destruction des objets
void CSceneTeapot::Destruction()
{
	
}

// Rendu 3D de la scène
void CSceneTeapot::Rendu3D()
{
	// Rendu de la Teapot
	m_pTeapot->SetTranslationX(0.0f);
	m_pTeapot->RenduMeshSampler();
	m_pTeapot->SetTranslationX(-3.0f);
	m_pTeapot->RenduMeshSampler();
	m_pTeapot->SetTranslationX(3.0f);
	m_pTeapot->RenduMeshSampler();
	m_pInterface->afficher();
	//m_pSprite->RenderSprite3D();
	
}

// Calculs des animations de la scène
void CSceneTeapot::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
}

// Test des entrées des périphériques
void CSceneTeapot::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
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
}

// Appliqué les paramètres de filtrage de texture
void CSceneTeapot::SetFiltreTexture(int Type)
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
