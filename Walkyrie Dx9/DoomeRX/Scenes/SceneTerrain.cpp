#include "SceneTerrain.h"

#define MAPTERRAIN	"..\\Donnees\\Terrain\\Map02\\MapTerrain.tga"
#define SURFACE_TERRAIN "..\\Donnees\\Terrain\\Map02\\Herbe.dds"

#define DENSITE_TERRE "..\\Donnees\\Terrain\\Map02\\DensiteTerre.tga"
#define TEXTURE_TERRE "..\\Donnees\\Terrain\\Map02\\Terre.dds"

#define DENSITE_EAU "..\\Donnees\\Terrain\\Map02\\DensiteEau.tga"
#define TEXTURE_EAU "..\\Donnees\\Terrain\\Map02\\Eau.jpg"

#define MAP_FLEUR "..\\Donnees\\Terrain\\Map02\\MapFleur.tga"
#define TEXTURE_FLEUR "..\\Donnees\\Terrain\\Map02\\Fleur.dds"
#define NB_FLEUR 50

#define MAP_ARBRE "..\\Donnees\\Terrain\\Map02\\MapArbre.tga"
#define TEXTURE_ARBRE1 "..\\Donnees\\Terrain\\Map02\\Arbre1.dds"
#define TEXTURE_ARBRE2 "..\\Donnees\\Terrain\\Map02\\Arbre2.dds"
#define TEXTURE_ARBRE3 "..\\Donnees\\Terrain\\Map02\\Arbre3.dds"
#define NB_ARBRE 600

#define MAP_PLANTE "..\\Donnees\\Terrain\\Map02\\MapPlante.tga"
#define TEXTURE_PLANTE "..\\Donnees\\Terrain\\Map02\\Plante.dds"
#define NB_PLANTE 50


CSceneTerrain::CSceneTerrain(CMoteur* pMoteur):CScene(pMoteur)
{
	m_pCamera1 = NULL;
	m_pCamera2 = NULL;
	m_pCamera3 = NULL;
	m_pAvion = NULL;
	m_pCiel = NULL;
	m_pTerrain = NULL;
	m_pInterface = NULL;
	m_pModelEau = NULL;
	m_pVegetation = NULL;

	m_pMoteurAudio = NULL;//m_pMoteur->GetMoteurAudio();
}

CSceneTerrain::~CSceneTerrain()
{
	Console << "Deconstructeur CSceneTerrain" << endl;
}

// Initialisation de la scène 3D
bool CSceneTerrain::Initialisation()
{
	m_Rotation = 0.0f;
	m_FiltreTexture = 3;
	m_Px = cosf(m_Rotation)*14.0f;
	m_Pz = sinf(m_Rotation)*14.0f;
	m_bRenduFilDeFer = false;

	m_Lumiere.SetDirectionnelle(&D3DXVECTOR3(2.0f, -1.0f, 0.0f));
	m_Lumiere.SetAmbiante(0.6f, 0.6f, 0.6f);
	m_Lumiere.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_pTerrain = new CTerrain(this);
	m_pVegetation = new CVegetation(m_pTerrain);
	m_pAvion = new CMesh(this);
	//	m_pAvion = new CMeshCollision(m_pD3DDevice);
	m_pCiel = new CPanorama(this);
	m_pInterface = new CInterfaceBattleFly(this); 
	m_pModelEau = new CModelEau(this);

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	m_pCameraParDefaut = new CCameraVolLibre(this);
	//m_pCameraParDefaut = new CCameraPremierePersonne(this);
	m_pCameraParDefaut->Creation();
	m_pCameraParDefaut->m_bMouvementUtilisateur = false;

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(100.0f, 100.0f, 0.0f));


	m_pCamera1 = new CCamera(this);
	m_pCamera1->Creation();
	m_pCamera1->m_bMouvementUtilisateur = true;

	m_pCamera2 = new CCameraPremierePersonneTerrain(this,m_pTerrain,NULL,2.0f);
	m_pCamera2->Creation();
	m_pCamera2->m_bMouvementUtilisateur = false;
	m_pCamera2->SetPosition(&D3DXVECTOR3(128.0f, 2.0f, 128.0f));

	m_pCamera3 = new CCamera(this);
	m_pCamera3->Creation();
	m_pCamera3->m_bMouvementUtilisateur = true;

	if(!m_pTerrain->CreationTerrain(MAPTERRAIN,50.0f))
	{
		Console<<"Erreur creation Terrain"<<endl;
		return false;
	}

	if(!m_pVegetation->Creation())
	{
		Console<<"Erreur creation Vegetation"<<endl;
		return false;
	}

	if(!m_pModelEau->CreationEau(0.0f,-3.0f,0.0f,312.0f,312.0f))
	//if(!m_pModelEau->CreationEau())
	{
		Console<<"Erreur creation Surface reflectrice d'eau "<<endl;
		return false;
	}
	
	if(m_pMoteurAudio != NULL)
	{
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\Cricket.wav",ANIMAUX1))
			return false;
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\tf2.wav",ANIMAUX2))
			return false;
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\tf3.wav",ANIMAUX3))
			return false;
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\tf4.wav",ANIMAUX4))
			return false;
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\tf6.wav",ANIMAUX5))
			return false;	

		if(!m_pMoteurAudio->LoadSound("..\\Sons\\Supercup.wav",MOTEUR_AVION))
			return false;	
	}
	else
	{
		Console<<"Le moteur Audio étant defaillant, les sons n'ont pas été initialisés correctement"<<endl;
	}

	return true;
}

// Création des objets 3D de la scène
bool CSceneTerrain::CreationObjet()
{

	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0);

	if(!m_pTerrain->SetSurface(SURFACE_TERRAIN))
	{
		Console<<"erreur texture Surface terrain"<<endl;
		return false;
	}

	if(!m_pTerrain->SetTerre(DENSITE_TERRE,TEXTURE_TERRE))
	{
		Console<<"erreur texture Terre terrain"<<endl;
		return false;
	}

	Console<<"Création DENSITE_TERRE"<<endl;
	if(!m_pTerrain->SetEaux(DENSITE_EAU,TEXTURE_EAU,-7.0f))
	{
		Console<<"erreur texture Eaux terrain"<<endl;
		return false;
	}


	// Chargement du terrain
	if(!m_pTerrain->ChargerTerrain())
		return false;

	if(!m_pInterface->Creation())
		return false;

	// Chargement de l'avion 
	if(!m_pAvion->ChargerMesh("..\\Mesh\\airplane 2.x"))
	{	
		Console<<"erreur chargement avion"<<endl;
		return false;
	}

	// Chargement de la skybox
	if(!m_pCiel->ChargerMesh("..\\Donnees\\SkyBox\\Ciel\\skybox2.x"))
	{
		Console<<"erreur chargement skybox"<<endl;
		return false;
	}


/*
	if(!m_pVegetation->SetMapFleur(MAP_FLEUR, NB_FLEUR, TEXTURE_FLEUR))
	{
		Console<<"erreur chargement Fleur"<<endl;
		return false;
	}

	if(!m_pVegetation->SetMapPlante(MAP_PLANTE, NB_PLANTE, TEXTURE_PLANTE))
	{
		Console<<"erreur chargement Plante"<<endl;
		return false;
	}
*/

	if(!m_pVegetation->SetMapArbre(MAP_ARBRE, NB_ARBRE,
								  TEXTURE_ARBRE1,
								  TEXTURE_ARBRE2,
								  TEXTURE_ARBRE3))
	{
		Console<<"erreur chargement Arbre"<<endl;
		return false;
	}

	if(!m_pVegetation->ChargerVegetation())
	{
		Console<<"erreur chargement vegetation"<<endl;
		return false;
	}


	if(!m_pModelEau->ChargerEau())
	{
		Console<<"erreur chargement Eau"<<endl;
		return false;
	}

	// Applique les paramètre de la caméras
	m_pCameraCourante->SetMatrice();
	// Applique le mode de rendu des polygones
	if(m_bRenduFilDeFer) m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	// On desactive le calcule de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// On applique le mode de filtrage de texture
	SetFiltreTexture();
	// On initialise la lumière 0 (pour l'avion)
	SetLumiere(0, &m_Lumiere, true);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();
	DxDataBase.affichageStatus();
	return true;
}

// Destruction des resource utilisé par les objets
void CSceneTerrain::DestructionObjet()
{
	m_pModelEau->Release();
	m_pAvion->Release();
	m_pCiel->Release();
	m_pTerrain->Release();
	m_pVegetation->Release();
	m_pInterface->Release();
}

// Destruction des resource utilisé par les objets
void CSceneTerrain::Destruction()
{
	Console<<"delete Objet Scene terrain"<<endl;
	SAFE_DELETE(m_pModelEau);
	SAFE_DELETE(m_pAvion);
	SAFE_DELETE(m_pCiel);
	SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(m_pVegetation);
	SAFE_DELETE(m_pInterface);
}

// Rendu 3D de la scène
void CSceneTerrain::Rendu3D()
{


	// Rendu de la skybox
	m_pCiel->RenduMeshSampler();
	
	// On applique la matrice de traformation pour placer l'avion
	//m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatTransAvion);
	//m_pAvion->SetWorldMatrix(&m_MatTransAvion);
	// On active le calcule de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Rendu de l'avion
	
	m_pAvion->RenduMeshSampler();
	
	// On déactive le calcule de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIXA16 matrix;
	D3DXMatrixIdentity(&matrix);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);

		
	m_pVegetation->RenduVegetation();

	// Rendu du terrain 3D

	m_pTerrain->RenduTerrain();



	m_pModelEau->RenduEau();

	
}


void CSceneTerrain::RenduInterface()
{
	m_pInterface->afficher();
}

// Calculs des animations de la scène
void CSceneTerrain::Animation(double TempsEcouler, double DeltaTemps)
{



	//Calcule des différentes postions de camera
	m_Rotation = (float)TempsEcouler;
	// Camera au sol
	if(m_pCameraCourante == m_pCamera1)
	{
		m_Px = cosf(m_Rotation/1.5f)*40.0f+64.0f;
		m_Pz = sinf(m_Rotation/1.5f)*40.0f+64.0f;
		m_pCamera1->SetPosition(&D3DXVECTOR3(64.0f, 35.0f, 64.0f));
		m_pCamera1->SetCible(&D3DXVECTOR3(m_Px, 45.0f, m_Pz));
		SetCamera(m_pCamera1);
	}


	// Camera sur l'aile de l'avion
	if(m_pCameraCourante == m_pCamera2)
	{
		
/*
		m_Px = cosf((m_Rotation+0.05f)/1.5f)*45.0f+64.0f;
		m_Pz = sinf((m_Rotation+0.05f)/1.5f)*45.0f+64.0f;
		m_pCamera2->SetPosition(&D3DXVECTOR3(m_Px, 60.0f, m_Pz));
		m_pCamera2->SetCible(&D3DXVECTOR3(64.0f, 40.0f, 64.0f));
		SetCamera(m_pCamera2);
*/
	}


	// Camera derrière l'avion
	if(m_pCameraCourante == m_pCamera3)
	{
		m_fIntervalleMoteur+=1/m_pInterface->m_fps;
		if(m_fIntervalleMoteur > 0.6f)
		{
			if(m_pMoteurAudio != NULL)
				m_pMoteurAudio->PlaySound(MOTEUR_AVION,true);
			m_fIntervalleMoteur = 0.0f;
		}
		

		m_Px = cosf((m_Rotation-0.3f)/1.5f)*39.5f+128.0f;
		m_Pz = sinf((m_Rotation-0.3f)/1.5f)*39.5f+128.0f;
		m_pCamera3->SetPosition(&D3DXVECTOR3(m_Px, 57.0f, m_Pz));
		m_Px = cosf((m_Rotation+0.3f)/1.5f)*39.5f+128.0f;
		m_Pz = sinf((m_Rotation+0.3f)/1.5f)*39.5f+128.0f;
		m_pCamera3->SetCible(&D3DXVECTOR3(m_Px, 57.0f, m_Pz));
		m_Px = -cosf(m_Rotation/1.5f)/1.73205f;
		m_Pz = -sinf(m_Rotation/1.5f)/1.73205f;
		m_pCamera3->SetOrientation(&D3DXVECTOR3(m_Px, 1.0f, m_Pz));
		SetCamera(m_pCamera3);
	}

	// Animation du terrain
	m_pTerrain->Animation(TempsEcouler);

	// Calcule de la matice de transformation pour l'avion
	D3DXMatrixIdentity(&m_MatriceGeneral);
	D3DXMatrixIdentity(&m_MatTransAvion);

	//D3DXMatrixRotationZ(&m_MatTransAvion, -D3DX_PI/6.0f);
	//D3DXMatrixMultiply(&m_MatriceGeneral, &m_MatriceGeneral, &m_MatTransAvion);

	D3DXMatrixRotationY(&m_MatTransAvion, -(m_Rotation/1.5f) + D3DX_PI);
	D3DXMatrixMultiply(&m_MatriceGeneral, &m_MatriceGeneral, &m_MatTransAvion);

	D3DXMatrixTranslation(&m_MatTransAvion, cosf(m_Rotation/1.5f)*40.0f+128.0f, 55.0f, sinf(m_Rotation/1.5f)*40.0f+128.0f);
	D3DXMatrixMultiply(&m_MatriceGeneral, &m_MatriceGeneral, &m_MatTransAvion);
	m_MatTransAvion = m_MatriceGeneral;

	//m_pAvion->SetRotationXYZ(0.0f,-(m_Rotation/1.5f) + D3DX_PI,-D3DX_PI/6.0f);
	//m_pAvion->SetTranslationXYZ(cosf(m_Rotation/1.5f)*40.0f+64.0f, 55.0f, sinf(m_Rotation/1.5f)*40.0f+64.0f);
	m_pAvion->SetWorldMatrix(&m_MatTransAvion);

	m_fIntervalleAnimaux+=1/m_pInterface->m_fps;
	static int sound = 0;
	if(m_fIntervalleAnimaux > 5.0f)
	{
		if(m_pMoteurAudio != NULL)
		{
			switch(sound)
			{
				case 0:
					m_pMoteurAudio->PlaySound(ANIMAUX1);
					break;
				case 1:
					m_pMoteurAudio->PlaySound(ANIMAUX2);
					break;
				case 2:
					m_pMoteurAudio->PlaySound(ANIMAUX3);
					break;
				case 3:
					m_pMoteurAudio->PlaySound(ANIMAUX4);
					break;
				case 4:
					m_pMoteurAudio->PlaySound(ANIMAUX5);
					break;
			}
		}
		sound = (sound+1)%5;
		m_fIntervalleAnimaux = 0.0f;
	}

	// Mise à jour des information des modèls
	m_pModelEau->MiseAJourDonnees(TempsEcouler);
	// On applique une transformation à la caméra (nécésaire pour le bon calcul des LOD)
	GetCameraCourante()->SetMatriceMiroir(&m_pModelEau->GetMatriceReflection());

	// Rendu de la texture de réflexion
	m_pModelEau->RenduEnvMiroir();

	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();

}

// Test des entrées des périphériques
void CSceneTerrain::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
{

	if(pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[0] & 0x80)
		Console << "CJoystick  : Touche (1) Prizee appuyé" << endl;
	if(pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[1] & 0x80)
		Console << "CJoystick  : Touche (2) Prizee appuyé" << endl;
	if(pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[2] & 0x80)
		Console << "CJoystick  : Touche (3) Prizee appuyé" << endl;
	if(pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[3] & 0x80)
		Console << "CJoystick  : Touche (4) Prizee appuyé" << endl;

	if(pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[4] & 0x80)
		Console << "CJoystick  : Touche (L2) Prizee appuyé" << endl;
	if(pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[5] & 0x80)
		Console << "CJoystick  : Touche (R2) Prizee appuyé" << endl;
	if(pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[6] & 0x80)
		Console << "CJoystick  : Touche (L1) Prizee appuyé" << endl;
	if(pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[7] & 0x80)
		Console << "CJoystick  : Touche (R1) Prizee appuyé" << endl;		


	if(pEtatJoystick!=NULL && pEtatJoystick->lX != 0 )
		Console << "CJoystick X : " <<pEtatJoystick->lX<< endl;
	if(pEtatJoystick!=NULL && pEtatJoystick->lY != 0 )
		Console << "CJoystick Y : " <<pEtatJoystick->lY<< endl;
	if(pEtatJoystick!=NULL && pEtatJoystick->lZ != 0 )
		Console << "CJoystick Z : " <<pEtatJoystick->lX<< endl;

	// Test du clavier pour voir si on a changé de caméra
	//if(EtatClavier[DIK_F1] & 0x80)
		//SetCamera(m_pCamera1);
	if(KEYDOWN(DIK_F1) || (pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[6] & 0x80))	
		SetCamera(m_pCamera1);
	if(KEYDOWN(DIK_F2) || (pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[4] & 0x80))	 
		SetCamera(m_pCamera2);
	if(KEYDOWN(DIK_F3) || (pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[7] & 0x80))	
		SetCamera(m_pCamera3);
	if(KEYDOWN(DIK_F4) || (pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[5] & 0x80))	 
		SetCamera(m_pCameraParDefaut);

	// Test pour voir si on à changé de mode de filtrage de texture
	if(KEYDOWN(DIK_F5))
	{
		m_FiltreTexture = 0;
		SetFiltreTexture();
	}
	if(KEYDOWN(DIK_F6))
	{
		m_FiltreTexture = 1;
		SetFiltreTexture();
	}
	if(KEYDOWN(DIK_F7))
	{
		m_FiltreTexture = 2;
		SetFiltreTexture();
	}
	if(KEYDOWN(DIK_F8))
	{
		m_FiltreTexture = 3;
		SetFiltreTexture();
	}

	// Test pour voir si on à changé de mode de rendu
	if(KEYDOWN(DIK_F9))
	{
		m_bRenduFilDeFer = false;
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	if(KEYDOWN(DIK_F10))
	{
		m_bRenduFilDeFer = true;
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	if(KEYDOWN(DIK_H))
		m_pInterface->m_bControles = true;
	if(KEYDOWN(DIK_N))
		m_pInterface->m_bControles = false;

}

// Appliqué les paramètres de filtrage de texture
void CSceneTerrain::SetFiltreTexture()
{
	switch(m_FiltreTexture)
	{
		// Aucun filtrage
		case 0:
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_NONE);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			break;
		// Filtrage Bilinéaire
		case 1:
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			break;
		// Filtrage Tilinéaire
		case 2:
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			break;
		// Filtrage Anisotropic
		case 3:
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAXANISOTROPY, 16);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			break;
	}
}
