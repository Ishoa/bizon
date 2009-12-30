#include "ScenePaticuleSystem.h"
#include "Particlesystem.h"

#define NB_STAGE 2
 

char* g_pTextName[] =
{
	"..\\Donnees\\Particule\\shine0.bmp",
	//"..\\Donnees\\Particule\\particle2.bmp",
	"..\\Donnees\\Particule\\shine1.bmp",
	"..\\Donnees\\Particule\\shine2.bmp",
	"..\\Donnees\\Particule\\shine3.bmp",
	"..\\Donnees\\Particule\\shine4.bmp",
	"..\\Donnees\\Particule\\shine5.bmp",
	"..\\Donnees\\Particule\\shine6.bmp",
	"..\\Donnees\\Particule\\shine7.bmp",
	"..\\Donnees\\Particule\\shine8.bmp",
	"..\\Donnees\\Particule\\shine9.bmp",
};


char* g_pTextSmoke[] = 
{	
	//"..\\Donnees\\Particule\\shine0.bmp",
	"..\\Donnees\\Particule\\smoke.jpg",
};

char* g_pTextWater[] = 
{	
	//"..\\Donnees\\Particule\\shine0.bmp",
	"..\\Donnees\\Particule\\waterdrops.bmp",
	//"..\\Donnees\\Particule\\point.bmp"
};

char* g_pTextGeyzer[] = 
{	
	"..\\Donnees\\Particule\\shine0.bmp",
	//"..\\Donnees\\Particule\\waterdrops.bmp",
	//"..\\Donnees\\Particule\\point.bmp"
};

char* g_pTextFire[] = 
{	
	"..\\Donnees\\Particule\\fire.bmp",
	//"..\\Donnees\\Particule\\waterdrops.bmp",
	//"..\\Donnees\\Particule\\point.bmp"
};

CSceneParticuleSystem::CSceneParticuleSystem(CMoteur* pMoteur):CScene(pMoteur)
{
	m_QualiterFiltrage = 2;
	m_pInterface = NULL;

	for (int i=0; i< NBFEUXARTIFICE; i++)
		m_pFuseeArtifice[i]=NULL;

	m_pFuseeExplode = NULL;

	for (int i=0; i< NBJETDEAU; i++)
		m_pJetDeau[i] = NULL;

	for (int i=0; i< NBJETDEAU; i++)
		m_pGeyzer[i] = NULL;

	for (int i=0; i< NBFIRE; i++)
		m_pFire[i] = NULL;

	m_fRed = 1.0f;
	m_fBlue = 1.0f;
	m_fGreen = 1.0f;
	m_fAlpha = 1.0f;
}

CSceneParticuleSystem::~CSceneParticuleSystem()
{
	Console << "Deconstructeur CSceneParticuleSystem" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneParticuleSystem::ConfirmeFonction3D(D3DCAPS9* pCaps)
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
bool CSceneParticuleSystem::Initialisation()
{

	m_pCameraParDefaut = new CCameraVolLibre(this);	
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 1.0f, -5.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	for (int i=0; i< NBFEUXARTIFICE; i++)
		m_pFuseeArtifice[i] = new CFuseeArtifice(m_pD3DDevice);

	m_pFuseeExplode = new CFuseeMissileExplode(this);

	for (int i=0; i< NBJETDEAU; i++)
		m_pJetDeau[i]  = new CJetDeau(m_pD3DDevice);

	for (int i=0; i< NBJETDEAU; i++)
		m_pGeyzer[i] = new CGeyzer(m_pD3DDevice);

	for (int i=0; i< NBFIRE; i++)
		m_pFire[i] = new CFire(m_pD3DDevice);

	m_pSprite = new CSprite3D(this);
	m_pSpace = new CPanorama(this);
	m_pInterface = new CInterfaceParticule(this);
	return true;


}

// Création des objets 3D de la scène
bool CSceneParticuleSystem::CreationObjet()
{
	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);

	// Initialisation de la lumière 1 et activation


	//########################################################################################

	int nTaille = sizeof(g_pTextName)/sizeof(g_pTextName[0]);

	for (int i=0; i< NBFEUXARTIFICE; i++)
		if(!m_pFuseeArtifice[i]->CreateAndInit(g_pTextName,nTaille,g_pTextName,nTaille))
			return false;

	nTaille = sizeof(g_pTextSmoke)/sizeof(g_pTextSmoke[0]);

	if(!m_pFuseeExplode->CreateAndInit(g_pTextSmoke,nTaille,g_pTextSmoke,nTaille))
		return false;

	nTaille = sizeof(g_pTextWater)/sizeof(g_pTextWater[0]);

	for (int i=0; i< NBJETDEAU; i++)	
		if(!m_pJetDeau[i]->CreateAndInit(g_pTextWater,nTaille))
			return false;

	nTaille = sizeof(g_pTextGeyzer)/sizeof(g_pTextGeyzer[0]);

	for (int i=0; i< NBJETDEAU; i++)
		if(!m_pGeyzer[i]->CreateAndInit(g_pTextGeyzer,nTaille))
			return false;

	nTaille = sizeof(g_pTextFire)/sizeof(g_pTextFire[0]);
	int ntailleSecondaire = sizeof(g_pTextSmoke)/sizeof(g_pTextSmoke[0]);
	for (int i=0; i< NBFIRE; i++)
		if(!m_pFire[i]->CreateAndInit(g_pTextFire,nTaille,g_pTextSmoke,ntailleSecondaire))
			return false;


	if(!m_pSprite->ChargerSprites3D("..\\Donnees\\Textures\\Floor.jpg",50.0f,50.0f,10.0f,10.0f))
		return false;

	m_pSprite->SetRotationX(D3DXToRadian(90.0f));
	
	if(!m_pSpace->ChargerMesh("..\\Donnees\\SkyBox\\Space2\\skybox2.x"))
	//if(!m_pSpace->ChargerMesh("..\\Donnees\\SkyBox\\Ciel\\skybox2.x"))
		
		return false;

	if(!m_pInterface->Creation())
		return false;

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	DxDataBase.affichageStatus();

	// de Base la lumiere est activé
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// FEUX ARTIFICES 

	m_pFuseeArtifice[0]->SetPositionDepart(D3DXVECTOR3(0.0f, 0.0f, 10.0f));
	m_pFuseeArtifice[0]->SetBoucle(true);
	m_pFuseeArtifice[0]->SetTimeAvantLancement(0.0f);
	m_pFuseeArtifice[0]->SetDirrectionFusee(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	m_pFuseeArtifice[1]->SetTimeAvantLancement(1.0f);
	m_pFuseeArtifice[1]->SetBoucle(true);
	m_pFuseeArtifice[1]->SetPositionDepart(D3DXVECTOR3(1.0f, 0.0f, 10.0f));
	m_pFuseeArtifice[1]->SetDirrectionFusee(D3DXVECTOR3(0.3f, 1.0f, 0.0f));

	m_pFuseeArtifice[2]->SetTimeAvantLancement(2.0f);
	m_pFuseeArtifice[2]->SetBoucle(true);
	m_pFuseeArtifice[2]->SetPositionDepart(D3DXVECTOR3(-1.0f, 0.0f, 10.0f));
	m_pFuseeArtifice[2]->SetDirrectionFusee(D3DXVECTOR3(-0.3f, 1.0f, 0.0f));


	// JET D'EAU


	m_pJetDeau[0]->SetPositionDepart(D3DXVECTOR3(4.0f, 0.0f, 4.0f));
	m_pJetDeau[0]->SetDirectionJet(D3DXVECTOR3 (-5.0f,10.0f,-5.0f));
	m_pJetDeau[0]->SetCoeffZoneJet(0.8f);

	m_pJetDeau[1]->SetPositionDepart(D3DXVECTOR3(4.0f, 0.0f, -4.0f));
	m_pJetDeau[1]->SetDirectionJet(D3DXVECTOR3 (-5.0f,10.0f,5.0f));
	m_pJetDeau[1]->SetCoeffZoneJet(0.8f);

	m_pJetDeau[2]->SetPositionDepart(D3DXVECTOR3(-4.0f, 0.0f, 4.0f));
	m_pJetDeau[2]->SetDirectionJet(D3DXVECTOR3 (5.0f,10.0f,-5.0f));
	m_pJetDeau[2]->SetCoeffZoneJet(0.8f);

	m_pJetDeau[3]->SetPositionDepart(D3DXVECTOR3(-4.0f, 0.0f, -4.0f));
	m_pJetDeau[3]->SetDirectionJet(D3DXVECTOR3 (5.0f,10.0f,5.0f));
	m_pJetDeau[3]->SetCoeffZoneJet(0.8f);

	m_pJetDeau[4]->SetPositionDepart(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pJetDeau[4]->SetCoeffZoneJet(1.0f);

	// GEYZER

	m_pGeyzer[0]->SetPositionDepart(D3DXVECTOR3(0.0f, 0.0f, 15.0f));
	
	m_pGeyzer[1]->SetPositionDepart(D3DXVECTOR3(4.0f, 0.0f, 15.0f));

	m_pGeyzer[2]->SetPositionDepart(D3DXVECTOR3(8.0f, 0.0f, 15.0f));

	m_pGeyzer[3]->SetPositionDepart(D3DXVECTOR3(-4.0f, 0.0f, 15.0f));

	m_pGeyzer[4]->SetPositionDepart(D3DXVECTOR3(-8.0f, 0.0f, 15.0f));

	// Tir Missile
	m_bTir = false;

	// Fire
	
	m_pFire[0]->SetPositionDepart(D3DXVECTOR3(-15.0f, 0.0f, 0.0f));
	m_pFire[1]->SetPositionDepart(D3DXVECTOR3(15.0f, 0.0f, 0.0f));

	return true;
}

// Destruction des resource utilisé par les objets
void CSceneParticuleSystem::DestructionObjet()
{
		for (int i=0; i< NBFEUXARTIFICE; i++)
			m_pFuseeArtifice[i]->DestructionObjet();

		m_pFuseeExplode->DestructionObjet();

		for (int i=0; i< NBJETDEAU; i++)
			m_pJetDeau[i]->DestructionObjet();

		for (int i=0; i< NBGEYZER; i++)
			m_pGeyzer[i]->DestructionObjet();

		for (int i=0; i< NBFIRE; i++)
			m_pFire[i]->DestructionObjet();

		m_pSpace->Release();
		m_pSprite->Release();
		m_pInterface->Release();
}

// Destruction des objets
void CSceneParticuleSystem::Destruction()
{
	for (int i=0; i< NBFEUXARTIFICE; i++)
		delete m_pFuseeArtifice[i];

	delete m_pFuseeExplode;

	for (int i=0; i< NBJETDEAU; i++)
		delete m_pJetDeau[i];

	for (int i=0; i< NBGEYZER; i++)
		delete m_pGeyzer[i];

	for (int i=0; i< NBFIRE; i++)
		delete m_pFire[i];

	delete m_pSpace;
	delete m_pSprite;
	delete m_pInterface;
}	

// Rendu 3D de la scène
void CSceneParticuleSystem::Rendu3D()
{
	//m_pSpace->RenduMesh();

	m_pSprite->RenderSprite3D();
	
	for (int i=0; i< NBFEUXARTIFICE; i++)
		m_pFuseeArtifice[i]->Rendu3D();

	m_pFuseeExplode->Rendu3D();

	for (int i=0; i< NBJETDEAU; i++)
		m_pJetDeau[i]->Rendu3D();

	for (int i=0; i< NBGEYZER; i++)
		m_pGeyzer[i]->Rendu3D();

	for (int i=0; i< NBFIRE; i++)
		m_pFire[i]->Rendu3D();

	m_pInterface->afficher();
}

// Calculs des animations de la scène
void CSceneParticuleSystem::Animation(double TempsEcouler, double DeltaTemps)
{
	CParticleSystem::ResetComteurParticule();

	for (int i=0; i< NBFEUXARTIFICE; i++)
		m_pFuseeArtifice[i]->Animation((float)DeltaTemps);

	m_pFuseeExplode->Animation((float)DeltaTemps, m_bTir);
	
	for (int i=0; i< NBJETDEAU; i++)
		m_pJetDeau[i]->Animation((float)DeltaTemps);

	for (int i=0; i< NBGEYZER; i++)
		m_pGeyzer[i]->Animation((float)DeltaTemps);

	for (int i=0; i< NBFIRE; i++)
		m_pFire[i]->Animation((float)DeltaTemps);

	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();

	m_bTir = false;

}

// Test des entrées des périphériques
void CSceneParticuleSystem::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
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

	if(pEtatSouris->rgbButtons[0] & 0x80)
		m_bTir = true;

/*
	float fVal = 0.001f;

	if(KEYDOWN(DIK_1))
	{
		m_fRed += fVal;
		if (m_fRed > 1.0f)
			m_fRed = 1.0f;
		m_pJetDeau->SetColor(D3DXCOLOR (m_fRed,m_fGreen,m_fBlue,m_fAlpha));
	}
	if(KEYDOWN(DIK_2))
	{
		m_fRed -= fVal;
		if (m_fRed < 0.0f)
			m_fRed = 0.0f;
		m_pJetDeau->SetColor(D3DXCOLOR (m_fRed,m_fGreen,m_fBlue,m_fAlpha));
	}

	if(KEYDOWN(DIK_3))
	{
		m_fBlue += fVal;
		if (m_fBlue > 1.0f)
			m_fBlue = 1.0f;
		m_pJetDeau->SetColor(D3DXCOLOR (m_fRed,m_fGreen,m_fBlue,m_fAlpha));
	}
	if(KEYDOWN(DIK_4))
	{
		m_fBlue -= fVal;
		if (m_fBlue < 0.0f)
			m_fBlue = 0.0f;
		m_pJetDeau->SetColor(D3DXCOLOR (m_fRed,m_fGreen,m_fBlue,m_fAlpha));
	}

	if(KEYDOWN(DIK_5))
	{
		m_fGreen += fVal;
		if (m_fGreen > 1.0f)
			m_fGreen = 1.0f;
		m_pJetDeau->SetColor(D3DXCOLOR (m_fRed,m_fGreen,m_fBlue,m_fAlpha));
	}
	if(KEYDOWN(DIK_6))
	{
		m_fGreen -= fVal;
		if (m_fGreen < 0.0f)
			m_fGreen = 0.0f;
		m_pJetDeau->SetColor(D3DXCOLOR (m_fRed,m_fGreen,m_fBlue,m_fAlpha));
	}

	if(KEYDOWN(DIK_7))
	{
		m_fAlpha += fVal;
		if (m_fAlpha > 1.0f)
			m_fAlpha = 1.0f;
		m_pJetDeau->SetColor(D3DXCOLOR (m_fRed,m_fGreen,m_fBlue,m_fAlpha));
	}
	if(KEYDOWN(DIK_8))
	{
		m_fAlpha -= fVal;
		if (m_fAlpha < 0.0f)
			m_fAlpha = 0.0f;
		m_pJetDeau->SetColor(D3DXCOLOR (m_fRed,m_fGreen,m_fBlue,m_fAlpha));
	}*/

}

// Appliqué les paramètres de filtrage de texture
void CSceneParticuleSystem::SetFiltreTexture(int Type)
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
