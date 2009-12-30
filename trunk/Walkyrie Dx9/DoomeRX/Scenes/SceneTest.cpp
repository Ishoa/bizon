#include "SceneTest.h"

#define NB_STAGE 2


CSceneTest::CSceneTest(CMoteur* pMoteur):CScene(pMoteur)
{
	m_QualiterFiltrage = 1;
	m_pInterface = NULL;
	m_pBilboarding = NULL;
	m_pSprite = NULL;
	m_pMesh = NULL;

	m_bAffichageLowDetail = false;
}

CSceneTest::~CSceneTest()
{
	Console << "Deconstructeur CSceneTemplate" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneTest::ConfirmeFonction3D(D3DCAPS9* pCaps)
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
bool CSceneTest::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	//m_pCameraParDefaut = new CCameraTrackBall(this);
	m_pCameraParDefaut = new CCameraVolLibre(this);	
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 1.0f, -5.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_LumiereParDefaut.SetPonctuelle(&D3DXVECTOR3(0.0f, 5.0f, 0.0f),50.0f);
	m_LumiereParDefaut.SetAmbiante(0.5f, 0.5f, 0.5f);
	m_LumiereParDefaut.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_MaterielleParDefaut.SetDiffuse(1.0f,1.0f,1.0f);

	// de Base la lumiere est activé
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pBilboarding = new CBilboarding(this);
	m_pSprite = new CSprite3D(this);
	m_pInterface = new CInterface(this);
	m_pMesh = new CMesh(this);
	

	return true;


}

// Création des objets 3D de la scène
bool CSceneTest::CreationObjet()
{
	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);


	/*###############################################################################*/

	if(!m_pMesh->ChargerMesh("..\\Mesh\\tree.x"))
	{
		Console<<"erreur chargement teapot"<<endl;
		return false;
	}


	if(!m_pMesh->ChargerMeshLowDetail("..\\Mesh\\tree_bounding.x"))
	{
		Console<<"erreur chargement teapot"<<endl;
		return false;
	}


	m_pMesh->SetScaleXYZ(0.1f,0.1f,0.1f);
	m_pMesh->SetTranslationY(1.0f);
	if(!m_pSprite->ChargerSprites3D("..\\Donnees\\Textures\\Floor.jpg",10.0f,10.0f,5.0f,5.0f))
		return false;

	m_pSprite->SetRotationX(D3DXToRadian(90.0f));
	//m_pSprite->SetTranslationXYZ(5.0f,0.0f,5.0f);
	if(!m_pBilboarding->ChargerSprites3D("..\\Donnees\\Terrain\\Map02\\Arbre1.dds"))
	{
		Console<<"erreur chargerment sprite"<<endl;
		return false;
	}
	m_pBilboarding->SetTranslationY(1.0f);

	// Initialisation de la lumière 1 et activation
	SetLumiere(0, &m_LumiereParDefaut, true);
	SetMaterielle(&m_MaterielleParDefaut);
	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();


	if(!m_pInterface->Creation())
		return false;

	DxDataBase.affichageStatus();

	return true;
}

// Destruction des resource utilisé par les objets
void CSceneTest::DestructionObjet()
{
	m_pMesh->Release();
	m_pInterface->Release();
	m_pBilboarding->Release();
	m_pSprite->Release();
}

// Destruction des objets
void CSceneTest::Destruction()
{
	SAFE_DELETE(m_pSprite);
}

// Rendu 3D de la scène
void CSceneTest::Rendu3D()
{
	if(!m_bAffichageLowDetail)
		m_pMesh->RenduMeshSampler();
	else
		m_pMesh->RenduMeshLowDetail();
	m_pSprite->RenderSprite3D();
	//m_pBilboarding->RenderSprite3D();
	m_pInterface->afficher();
}

// Calculs des animations de la scène
void CSceneTest::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
}

// Test des entrées des périphériques
void CSceneTest::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
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
		m_bAffichageLowDetail = true;
	}
	if(KEYDOWN(DIK_F5))
	{
		m_bAffichageLowDetail = false;
	}
}

// Appliqué les paramètres de filtrage de texture
void CSceneTest::SetFiltreTexture(int Type)
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
