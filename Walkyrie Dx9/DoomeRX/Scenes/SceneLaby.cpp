#include "SceneLaby.h"
#include "..\..\Valkyrie\Moteur\Utils.h"

#define NB_STAGE 2
#define LARGEURMUR 8.0
#define HAUTEURMUR 5.0

//#define TEXTUREMUR "..\\Donnees\\Wall.jpg"
#define TEXTUREMUR "..\\Donnees\\Textures\\env2.bmp"

#define TEXTUREDEFAULTMUR NULL


CMapDeMur::MUR_FVF g_pMapDeMur [] =
{
	//1
	{
		7*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (0.0f,HAUTEURMUR/2,3.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(0.0f),D3DXToRadian(0.0f)),
		7.0f,1.0f
		
	},
	//2
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-0.5*LARGEURMUR,HAUTEURMUR/2,2*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(-90.0f),D3DXToRadian(0.0f))
	},
	//3
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (0.5*LARGEURMUR,HAUTEURMUR/2,2*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(90.0f),D3DXToRadian(0.0f))
	},
	//4
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-1*LARGEURMUR,HAUTEURMUR/2,1.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(0.0f),D3DXToRadian(0.0f))
	},
	//5
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (1*LARGEURMUR,HAUTEURMUR/2,1.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(0.0f),D3DXToRadian(0.0f))
	},
	//6
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-1.5*LARGEURMUR,HAUTEURMUR/2,1*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(-90.0f),D3DXToRadian(0.0f))
	},

	//7
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (1.5*LARGEURMUR,HAUTEURMUR/2,1*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(90.0f),D3DXToRadian(0.0f))
	},

	//8
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-2*LARGEURMUR,HAUTEURMUR/2,0.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(0.0f),D3DXToRadian(0.0f))
	},
	//9
	{
		7*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-3.5*LARGEURMUR,HAUTEURMUR/2,0.0f),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(-90.0f),D3DXToRadian(0.0f)),
		7.0f,1.0f
	},
	//10
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-2*LARGEURMUR,HAUTEURMUR/2,-0.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(180.0f),D3DXToRadian(0.0f))
	},
	//11
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-1.5*LARGEURMUR,HAUTEURMUR/2,-1.0*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(270.0f),D3DXToRadian(0.0f))
	},
	//12
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-1*LARGEURMUR,HAUTEURMUR/2,-1.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(180.0f),D3DXToRadian(0.0f))
	},
	//13
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-0.5*LARGEURMUR,HAUTEURMUR/2,-2*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(270.0f),D3DXToRadian(0.0f))
	},

	//14
	{
		7*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (0.0f,HAUTEURMUR/2,-3.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(180.0f),D3DXToRadian(0.0f)),
		7.0f,1.0f
	},
	//15
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (0.5*LARGEURMUR,HAUTEURMUR/2,-2.0*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(90.0f),D3DXToRadian(0.0f))
	},
	//16
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (1.0*LARGEURMUR,HAUTEURMUR/2,-1.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(180.0f),D3DXToRadian(0.0f))
	},
	//17
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (1.5*LARGEURMUR,HAUTEURMUR/2,-1*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(90.0f),D3DXToRadian(0.0f))
	},
	//18
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (2.0*LARGEURMUR,HAUTEURMUR/2,-0.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(180.0f),D3DXToRadian(0.0f))
	},
	//19
	{
		7*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (3.5*LARGEURMUR,HAUTEURMUR/2,0.0f),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(90.0f),D3DXToRadian(0.0f)),
		7.0f,1.0f
	},
	//20
	{
		LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (2.0*LARGEURMUR,HAUTEURMUR/2,0.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(0.0f),D3DXToRadian(0.0f))
	},
	//21
	{
		2*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (2.5*LARGEURMUR,HAUTEURMUR/2,1.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(-90.0f),D3DXToRadian(0.0f)),
		2.0f,1.0f
	},
	//22
	{
		2*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (1.5*LARGEURMUR,HAUTEURMUR/2,2.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(180.0f),D3DXToRadian(0.0f)),
		2.0f,1.0f
	},
	//23
	{
		2*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-2.5*LARGEURMUR,HAUTEURMUR/2,1.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(90.0f),D3DXToRadian(0.0f)),
		2.0f,1.0f
	},
	//24
	{
		2*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-1.5*LARGEURMUR,HAUTEURMUR/2,2.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(180.0f),D3DXToRadian(0.0f)),
		2.0f,1.0f
	},
	//25
	{
		2*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-2.5*LARGEURMUR,HAUTEURMUR/2,-1.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(90.0f),D3DXToRadian(0.0f)),
		2.0f,1.0f
	},
	//26
	{
		2*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (-1.5*LARGEURMUR,HAUTEURMUR/2,-2.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(0.0f),D3DXToRadian(0.0f)),
		2.0f,1.0f
	},
	//27
	{
		2*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (2.5*LARGEURMUR,HAUTEURMUR/2,-1.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(-90.0f),D3DXToRadian(0.0f)),
		2.0f,1.0f
	},
	//28
	{
		2*LARGEURMUR,
		HAUTEURMUR,
		TEXTUREDEFAULTMUR,
		D3DXVECTOR3 (1.5*LARGEURMUR,HAUTEURMUR/2,-2.5*LARGEURMUR),
		D3DXVECTOR3 (D3DXToRadian(0.0f),D3DXToRadian(0.0f),D3DXToRadian(0.0f)),
		2.0f,1.0f
	}
};




CSceneLaby::CSceneLaby(CMoteur* pMoteur):CScene(pMoteur)
{
	m_pSol = NULL;
	m_pPlafond = NULL;
	m_pMapDeMur = NULL;
	m_pSpace = NULL;
	m_pEnv = NULL;

	m_pMesh = NULL;
	m_pMesh2 = NULL;
	m_pMesh3 = NULL;

	m_bAffichageLowDetail = false;

	m_QualiterFiltrage = 2;
	m_fIntervallePas = 1.0f;
	m_fIntervalleShoot = 1.0f;
	
}

CSceneLaby::~CSceneLaby()
{
	Console << "Deconstructeur CSceneLaby" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneLaby::ConfirmeFonction3D(D3DCAPS9* pCaps)
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
bool CSceneLaby::Initialisation()
{

	m_pSol = new CSprite3D(this);
	m_pPlafond = new CSprite3D(this);
	m_pMapDeMur = new CMapDeMur(this);

	m_pEnv = new CEnvCollision();
	
	m_pMesh = new CMeshCollision(this);
	m_pMesh2 = new CMeshCollision(this);
	m_pMesh3 = new CMeshCollision(this);

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;

	//m_pCameraParDefaut = new CCameraPremierePersonne(this);
	m_pCameraParDefaut = new CCameraPremierePersonneLaby(this,m_pEnv);
	//m_pCameraParDefaut = new CCamera(this);
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 3.0f, -7.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pCameraParDefaut->SetVitesseMouvement(15.0f);


	m_Lumiere.SetPonctuelle(&D3DXVECTOR3(0.0f,10.0f, 0.0f),100.0f);
	m_Lumiere.SetAmbiante(0.6f, 0.6f, 0.6f);
	m_Lumiere.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_Lumiere2.SetDirectionnelle(&D3DXVECTOR3(5.0f, 5.0f, 0.0f));
	m_Lumiere2.SetAmbiante(0.5f, 0.5f, 0.5f);
	m_Lumiere2.SetDiffuse(1.0f, 1.0f, 1.0f);


	m_pInterface = new CInterfaceLaby(this);
	m_pSpace = new CPanorama(this);

	//m_pSpace = new CMesh(this);

	if(m_pMoteurAudio != NULL)
	{
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\prepare.wav",PREPARE_TO_FIGHT))
			return false;
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\PasPied.WAV",STEP))
			return false;
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\PasPied.WAV",ATMOSPHERE))
			return false;
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\Fire1.WAV",FIRE))
			return false;
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\clic.WAV",RELOAD))
			return false;	

		if(!m_pMoteurAudio->LoadSound("..\\Sons\\grenade.wav",GRENADE))
			return false;	
		if(!m_pMoteurAudio->LoadSound("..\\Sons\\die5.wav",EXPLOSION))
			return false;
	}
	else
	{
		Console<<"Le moteur Audio étant defaillant, les sons n'ont pas été initialisés correctement"<<endl;
	}
	

	return true;
}

// Création des objets 3D de la scène
bool CSceneLaby::CreationObjet()
{
	Console<<"Création Objets Scene"<<endl;
	Console<<".";
	
	if(!m_pInterface->Creation())
		return false;

	m_pInterface->ammo = 50;

	Console<<".";

	if(!m_pSpace->ChargerMesh("..\\Donnees\\SkyBox\\Space2\\skybox2.x"))
		//	if(!m_pSpace->ChargerModel("..\\Donnees\\Ciel\\skybox2.x"))
	{
		Console<<"erreur chargement skybox"<<endl;
		return false;
	}

	Console<<".";

	// Chargement de l'avion 
	if(!m_pMesh->ChargerMesh("..\\Mesh\\64-Ford-Thunderbird.x"))
	//if(!m_pMesh->ChargerMesh("..\\Mesh\\teapot.x"))
	{	
		Console<<"erreur chargement teapot"<<endl;
		return false;
	}
	m_pMesh->SetTranslationY(1.0f);
	m_pMesh->SetTranslationX(-5.0f);
	m_pMesh->SetRotationX(D3DXToRadian(-90.0f));
	m_pMesh->SetScaleXYZ(4.0f,4.0f,4.0f);

	Console<<".";

	// Chargement de l'avion 
	if(!m_pMesh2->ChargerMesh("..\\Mesh\\Audi-Avus.x"))
	{	
		Console<<"erreur chargement Audi-Avus.x"<<endl;
		return false;
	}
	m_pMesh2->SetTranslationY(1.0f);
	m_pMesh2->SetTranslationX(5.0f);
	m_pMesh2->SetRotationX(D3DXToRadian(-90.0f));
	m_pMesh2->SetScaleXYZ(4.0f,4.0f,4.0f);

	Console<<".";


	if(!m_pMesh3->ChargerMesh("..\\Mesh\\tree.x"))
	{
		Console<<"erreur chargement Sapin"<<endl;
		return false;
	}


	if(!m_pMesh3->ChargerMeshLowDetail("..\\Mesh\\tree_bounding.x"))
	{
		Console<<"erreur chargement sapin"<<endl;
		return false;
	}


	m_pMesh3->SetScaleXYZ(0.2f,0.2f,0.2f);
	m_pMesh3->SetTranslationY(2.0f);
/*
	
	if(!m_pMesh3->ChargerMesh("..\\Mesh\\Tie-Fighter.x"))
	{	
		Console<<"erreur chargement Tie-Fighter.x"<<endl;
		return false;
	}
	m_pMesh3->SetTranslationY(1.0f);
	m_pMesh3->SetScaleXYZ(2.0f,2.0f,2.0f);
*/

	Console<<".";



	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);
	m_pSol->ChargerSprites3D("..\\Donnees\\Textures\\Floor.jpg",7*LARGEURMUR,7*LARGEURMUR,2*LARGEURMUR,2*LARGEURMUR);
	m_pSol->SetRotationX(D3DXToRadian(90.0f));
	//m_pSol->SetTranslationZ(5*LARGEURMUR);

	Console<<".";

	m_pPlafond->SetRotationX(D3DXToRadian(-90.0f));
	//m_pPlafond->SetTranslationZ(5*LARGEURMUR);
	m_pPlafond->SetTranslationY(4.0f);
	m_pPlafond->ChargerSprites3D("..\\Donnees\\Textures\\Roof.jpg",7*LARGEURMUR,7*LARGEURMUR);

	Console<<".";

 	SetLumiere(0, &m_Lumiere, true);
 	SetLumiere(1, &m_Lumiere2, true);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	if(!m_pMapDeMur->ChargerMapDeMur("..\\Donnees\\Textures\\Wall.jpg",g_pMapDeMur,sizeof(g_pMapDeMur)/sizeof(g_pMapDeMur[0])))
	{
		Console<<"erreur chargement de la map"<<endl;
		return false;
	}

	Console<<".";

	if(!m_pMapDeMur->ChargerEnvCollision(m_pEnv))
	{
		Console<<"Erreur chargement Environnement de Collision map de mur"<<endl;
		return false;
	}

	Console<<".";

	if(!m_pEnv->AddObjetDeCollision(m_pMesh,CEnvCollision::BOUNDING_BOX,0.8f,CCalculsCollisions::COLLISION_AND_SLIDING))
	{
		Console<<"Erreur chargement Environnement de Collision ThunderBird"<<endl;
		return false;
	}

	Console<<".";

	if(!m_pEnv->AddObjetDeCollision(m_pMesh2,CEnvCollision::BOUNDING_BOX,0.7f,CCalculsCollisions::COLLISION_AND_SLIDING))
	{
		Console<<"Erreur chargement Environnement de Collision Audi"<<endl;
		return false;
	}

	Console<<".";

	if(!m_pEnv->AddObjetDeCollision(m_pMesh3,CEnvCollision::BOUNDING_BOX,0.7f,CCalculsCollisions::COLLISION_ONLY))
	{
		Console<<"Erreur chargement Environnement de Collision Tie Fighter"<<endl;
		return false;
	}
	
	Console<<"."<<endl;

	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, true);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	m_pMoteurAudio->PlaySound(PREPARE_TO_FIGHT);
	
	DxDataBase.affichageStatus();
	return true;
}

// Destruction des resource utilisé par les objets
void CSceneLaby::DestructionObjet()
{
	m_pEnv->Release();
	m_pMesh->Release();
	m_pMesh2->Release();
	m_pMesh3->Release();

	m_pSol->Release();
	m_pPlafond->Release();
	m_pMapDeMur->Release();
	m_pSpace->Release();
	m_pInterface->Release();

}

// Destruction des objets
void CSceneLaby::Destruction()
{

}

// Rendu 3D de la scène
void CSceneLaby::Rendu3D()
{

	// Rendu de la skybox space
	m_pSpace->RenduMeshSampler();

	SetMaterielle(&m_Materielle);
	m_pSol->RenderSprite3D();
	//m_pPlafond->RenderSprite3D();
	m_pMapDeMur->RenduMapDeMur();

	if(m_bAffichageLowDetail)
	{
		m_pMesh->RenduMeshLowDetail();
		m_pMesh2->RenduMeshLowDetail();
		m_pMesh3->RenduMeshLowDetail();
	}
	else
	{
		m_pMesh->RenduMeshSampler();
		m_pMesh2->RenduMeshSampler();
		m_pMesh3->RenduMeshSampler();
	}


	m_pInterface->afficher();
}

// Calculs des animations de la scène
void CSceneLaby::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();

}

// Test des entrées des périphériques
void CSceneLaby::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
{


	double DeltaTemps = m_Temps.TempsEcouler();
	//Console << m_pInterface->m_fps<<endl;
	m_fIntervallePas+=1/m_pInterface->m_fps;
	m_fIntervalleShoot+=1/m_pInterface->m_fps;

	//Console<<m_fIntervallePas<<endl;
	if(KEYDOWN(DIK_A) || KEYDOWN(DIK_S) || KEYDOWN(DIK_W) || KEYDOWN(DIK_D))
		if(m_fIntervallePas > 0.5f)
		{
				m_pMoteurAudio->PlaySound(STEP);
				m_fIntervallePas = 0.0f;
		}

	if(pEtatSouris->rgbButtons[0] & 0x80)
	{
		if(m_fIntervalleShoot > 0.1f)
		{
			if(m_pInterface->ammo > 0)
			{
				m_pMoteurAudio->PlaySound(FIRE,true);
				m_pInterface->m_bshoot = true;
				m_pInterface->ammo--;
			}
			else
				m_pMoteurAudio->PlaySound(RELOAD);

			m_fIntervalleShoot = 0.0f;
		}
	}

	if(KEYDOWN(DIK_R))
	{
		m_pInterface->ammo = 50;
	}

	if(KEYDOWN(DIK_G))
	{
/*
		unsigned int* Sequence;
		Sequence = new unsigned int[2];
		Sequence[0] = GRENADE;
		Sequence[1] = EXPLOSION;

		m_pMoteurAudio->PlaySoundSequence(Sequence,2);
*/
		m_pMoteurAudio->PlaySound(GRENADE);
		m_pMoteurAudio->PlaySound(EXPLOSION);
	}

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

	// Test pour voir si on à changé de mode de rendu
	if(KEYDOWN(DIK_F5))
	{
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	if(KEYDOWN(DIK_F6))
	{
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	if(KEYDOWN(DIK_F7))
	{
		m_bAffichageLowDetail = false;
	}

	if(KEYDOWN(DIK_F8))
	{
		m_bAffichageLowDetail = true;
	}



	if(KEYDOWN(DIK_F10))
	{
		m_pMesh->SetBoundingBox(true);
		m_pMesh2->SetBoundingBox(true);
		m_pMesh3->SetBoundingBox(true);
	}

	if(KEYDOWN(DIK_F9))
	{
		m_pMesh->SetBoundingBox(false);
		m_pMesh2->SetBoundingBox(false);
		m_pMesh3->SetBoundingBox(false);
	}

	if(KEYDOWN(DIK_0))
	{
		m_pMesh->SetBoundingSphere(true);
		m_pMesh2->SetBoundingSphere(true);
		m_pMesh3->SetBoundingSphere(true);
	}

	if(KEYDOWN(DIK_9))
	{
		m_pMesh->SetBoundingSphere(false);
		m_pMesh2->SetBoundingSphere(false);
		m_pMesh3->SetBoundingSphere(false);
	}

	if(KEYDOWN(DIK_H))
		m_pInterface->m_bControles = true;
	if(KEYDOWN(DIK_N))
		m_pInterface->m_bControles = false;
}

// Appliqué les paramètres de filtrage de texture
void CSceneLaby::SetFiltreTexture(int Type)
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
