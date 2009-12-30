#include "SceneSystemeSolaire.h"
#include "CameraTrackBall.h"

#define NB_STAGE 7
#define DIV_ANNEAU 100
#define NB_ANNEAU 6
float g_pfTaille[NB_ANNEAU]={30.0f,15.0f,45.0f,55.0f,65.0f,6.0f};
// Terre // Mars //  Mercure //Venus // Pluton // Lune


CSceneSystemSolaire::CSceneSystemSolaire(CMoteur* pMoteur):CScene(pMoteur)
{
	m_pTerre = NULL;
	m_pLune = NULL;
	m_pMars = NULL;
	m_pSpace = NULL;
	m_pSoleil = NULL;
	m_pMercure = NULL;
	m_pPluton = NULL;
	m_pVenus = NULL;

	m_pInterface = NULL;

	m_QualiterFiltrage = 1;
	m_nChoix = 0;
	m_blightActive = true;

	m_fSpeedmodifier = 0.5f;
	m_bOrbitOn = true;

	m_pVertexBuffer   = NULL;
}

CSceneSystemSolaire::~CSceneSystemSolaire()
{
	Console << "Deconstructeur CSceneSystemSolaire" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneSystemSolaire::ConfirmeFonction3D(D3DCAPS9* pCaps)
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
bool CSceneSystemSolaire::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	m_pCameraParDefaut = new CCameraTrackBall(this);
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 0.0f, 70.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pSpace = new CPanorama(this);

	m_pTerre = new CPlanete(this);
	m_pLune = new CPlanete(this);
	m_pMars = new CPlanete(this);
	m_pSoleil = new CPlanete(this);
	m_pMercure = new CPlanete(this);
	m_pPluton = new CPlanete(this);
	m_pVenus = new CPlanete(this);

	float fTailleSoleil = 10.0f;

	
	m_LumiereC.SetPonctuelle(&D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 0.8f);
	m_LumiereC.SetAmbiante(0.2f, 0.2f, 0.2f);
	m_LumiereC.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_LumiereN.SetPonctuelle(&D3DXVECTOR3(0.0f, fTailleSoleil, 0.0f), 100.0f, 0.8f);
	m_LumiereN.SetAmbiante(0.2f, 0.2f, 0.2f);
	m_LumiereN.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_LumiereS.SetPonctuelle(&D3DXVECTOR3(0.0f, -fTailleSoleil, 0.0f), 100.0f, 0.8f);
	m_LumiereS.SetAmbiante(0.2f, 0.2f, 0.2f);
	m_LumiereS.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_LumiereE.SetPonctuelle(&D3DXVECTOR3(fTailleSoleil, 0.0f, 0.0f), 100.0f, 0.8f);
	m_LumiereE.SetAmbiante(0.2f, 0.2f, 0.2f);
	m_LumiereE.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_LumiereO.SetPonctuelle(&D3DXVECTOR3(-fTailleSoleil, 0.0f, 0.0f), 100.0f, 0.8f);
	m_LumiereO.SetAmbiante(0.2f, 0.2f, 0.2f);
	m_LumiereO.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_LumiereF.SetPonctuelle(&D3DXVECTOR3(0.0f, 0.0f, fTailleSoleil), 100.0f, 0.8f);
	m_LumiereF.SetAmbiante(0.2f, 0.2f, 0.2f);
	m_LumiereF.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_LumiereB.SetPonctuelle(&D3DXVECTOR3(0.0f, 0.0f, -fTailleSoleil), 100.0f, 0.8f);
	m_LumiereB.SetAmbiante(0.2f, 0.2f, 0.2f);
	m_LumiereB.SetDiffuse(1.0f, 1.0f, 1.0f);


	//m_pD3DDevice->SetRenderState(D3DRS_POINTSIZE,5.0f);

	/*
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE,true);
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR,D3DCOLOR_XRGB(255,255,255));
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART,10.0f);
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND,50.0f);
	m_pD3DDevice->SetRenderState( D3DRS_RANGEFOGENABLE, true);
	*/

	m_pD3DDevice->LightEnable(0,TRUE);
	m_pD3DDevice->LightEnable(1,TRUE);
	m_pD3DDevice->LightEnable(2,TRUE);
	m_pD3DDevice->LightEnable(3,TRUE);
	m_pD3DDevice->LightEnable(4,TRUE);
	m_pD3DDevice->LightEnable(5,TRUE);
	m_pD3DDevice->LightEnable(6,TRUE);

	D3DXMatrixIdentity(&m_MSunMatrix);
	D3DXMatrixIdentity(&m_MMarsMatrix);
	D3DXMatrixIdentity(&m_MmEarthMatrix);
	D3DXMatrixIdentity(&m_MMoonMatrix);
	D3DXMatrixIdentity(&m_MMercurenMatrix);
	D3DXMatrixIdentity(&m_MPlutonMatrix);
	D3DXMatrixIdentity(&m_MVenusMatrix);


	m_pInterface = new CInterfaceSystemSolaire(this); 

	return true;
}

// Création des objets 3D de la scène
bool CSceneSystemSolaire::CreationObjet()
{
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, true);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,true);

	if(!m_pInterface->Creation())
		return false;

		// definition d'une sphere
	VERTEX_ANNEAU* cercle = new VERTEX_ANNEAU[DIV_ANNEAU*NB_ANNEAU];
	{
		
		
		int i;
		for( int k = 0; k<NB_ANNEAU ; k++)
		{
			float   ph=-D3DX_PI/2, da = (D3DX_PI*2)/DIV_ANNEAU;
			for(i=k * DIV_ANNEAU; i< (k * DIV_ANNEAU)+DIV_ANNEAU-1;i++,ph+=da) 
			{
					cercle[i].P.x = cosf(ph)*g_pfTaille[k];
					cercle[i].P.y = 0.0f;
					cercle[i].P.z = sinf(ph)*g_pfTaille[k]; 
					cercle[i].diffuse = D3DCOLOR_XRGB(255,255,0);

			}
			cercle[i] = cercle[k * DIV_ANNEAU];
		}
	}

	int sizeBuffer = DIV_ANNEAU*NB_ANNEAU*sizeof(VERTEX_ANNEAU);
	
	// creation du vertex buffer
    void *ptr = NULL;
	m_pD3DDevice->CreateVertexBuffer( sizeBuffer,0, D3DFVF_VERTEX_ANNEAU,D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );
    m_pVertexBuffer->Lock( 0, sizeBuffer, (void**)&ptr, 0 );
    memcpy(ptr, cercle, sizeBuffer);
    m_pVertexBuffer->Unlock();

	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);

	// Chargement de l espace
	if(!m_pSpace->ChargerMesh("..\\Donnees\\SkyBox\\Space2\\skybox2.x"))
//	if(!m_pSpace->ChargerMesh("..\\Donnees\\Ciel\\skybox2.x"))
	{
		Console<<"erreur chargement skybox"<<endl;
		return false;
	}


	// Chargement du Soleil
	if(!m_pSoleil->Creation(CPlanete::SOLEIL, 3.0f, 16, "..\\Donnees\\Bump Mapping\\Sun\\sunmap2.jpg",	
												        "..\\Donnees\\Bump Mapping\\Sun\\sunbumap.jpg"))
	{	
		Console<<"erreur chargement Soleil"<<endl;
		return false;
	}

	// Chargement de terre 
	if(!m_pTerre->Creation(CPlanete::TERRE, 2.0f, 16, "..\\Donnees\\Bump Mapping\\earth\\earthmap1k.jpg",		
													  "..\\Donnees\\Bump Mapping\\earth\\earthbump1k.jpg"))
	{	
		Console<<"erreur chargement Terre"<<endl;
		return false;
	}


	// Chargement de la Lune 
	if(!m_pLune->Creation(CPlanete::LUNE, 1.0f, 16, "..\\Donnees\\Bump Mapping\\Moon\\moonmap1k.jpg",	
													"..\\Donnees\\Bump Mapping\\Moon\\moonbump1k.jpg"))
	{	
		Console<<"erreur chargement Lune"<<endl;
		return false;
	}
	
	// Chargement de Mars 
	if(!m_pMars->Creation(CPlanete::MARS, 2.5f, 16, "..\\Donnees\\Bump Mapping\\Mars\\mars_1k_color.jpg",	
													 "..\\Donnees\\Bump Mapping\\Mars\\marsbump1k.jpg"))
	{	
		Console<<"erreur chargement Mars"<<endl;
		return false;
	}

	// Chargement de Mercure 
	if(!m_pMercure->Creation(CPlanete::MERCURE, 2.5f, 16, "..\\Donnees\\Bump Mapping\\Mercury\\mercurymap.jpg",	
													 "..\\Donnees\\Bump Mapping\\Mercury\\mercurybump.jpg"))
	{	
		Console<<"erreur chargement Mercure"<<endl;
		return false;
	}

	// Chargement de Pluton 
	if(!m_pPluton->Creation(CPlanete::PLUTON, 2.5f, 16, "..\\Donnees\\Bump Mapping\\Pluton\\plutomap1k.jpg",	
													 "..\\Donnees\\Bump Mapping\\Pluton\\plutobump1k.jpg"))
	{	
		Console<<"erreur chargement Pluton"<<endl;
		return false;
	}


	// Chargement de Venus 
	if(!m_pVenus->Creation(CPlanete::VENUS, 2.5f, 16, "..\\Donnees\\Bump Mapping\\Venus\\venusmap.jpg",	
													 "..\\Donnees\\Bump Mapping\\Venus\\venusbump.jpg"))
	{	
		Console<<"erreur chargement Venus"<<endl;
		return false;
	}

	// Initialisation de la lumière 1 et activation

	// On initialise la lumière 0
	SetLumiere(0, &m_LumiereC, true);
	
	SetLumiere(1, &m_LumiereN, true);
	SetLumiere(2, &m_LumiereS, true);
	SetLumiere(3, &m_LumiereE, true);
	SetLumiere(4, &m_LumiereO, true);
	SetLumiere(5, &m_LumiereF, true);
	SetLumiere(6, &m_LumiereB, true);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	DxDataBase.affichageStatus();
	return true;
}

// Destruction des resource utilisé par les objets
void CSceneSystemSolaire::DestructionObjet()
{
	m_pInterface->Release();
	m_pTerre->Release();
	m_pSpace->Release();
	m_pLune->Release();
	m_pMars->Release();
	m_pSoleil->Release();
	m_pMercure->Release();
	m_pPluton->Release();
	m_pVenus->Release();
	m_pVertexBuffer->Release(); 
}

// Destruction des objets
void CSceneSystemSolaire::Destruction()
{
	delete m_pInterface;
	delete m_pTerre;
	delete m_pLune;
	delete m_pMars;
	delete m_pSoleil;
	delete m_pMercure;
	delete m_pPluton;
	delete m_pVenus;
	delete m_pSpace;
}

// Rendu 3D de la scène
void CSceneSystemSolaire::Rendu3D()
{


	switch(m_nChoix)
	{
		case 0:m_pCameraCourante->setWorldMatrice(&m_MSunMatrix);break;
		case 1:m_pCameraCourante->setWorldMatrice(&m_MmEarthMatrix);break;
		case 2:m_pCameraCourante->setWorldMatrice(&m_MMoonMatrix);break;
		case 3:m_pCameraCourante->setWorldMatrice(&m_MMarsMatrix);break;
		case 4:m_pCameraCourante->setWorldMatrice(&m_MMercurenMatrix);break;
		case 5:m_pCameraCourante->setWorldMatrice(&m_MVenusMatrix);break;
		case 6:m_pCameraCourante->setWorldMatrice(&m_MPlutonMatrix);break;
		default:break;
	}
	
	
	m_pCameraCourante->SetMatrice();
	// Rendu de la skybox space
	m_pSpace->RenduMeshSampler();

	m_pD3DDevice->SetTexture(0,NULL);
	D3DXMATRIX mBaseAnneau;	
	D3DXMatrixIdentity(&mBaseAnneau);

	m_pD3DDevice->SetTransform( D3DTS_WORLD, &mBaseAnneau ); 

	m_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer,   0, sizeof(VERTEX_ANNEAU) );
	m_pD3DDevice->SetFVF( D3DFVF_VERTEX_ANNEAU );
	for (int i = 0 ; i<(NB_ANNEAU-1)*DIV_ANNEAU ; i+=DIV_ANNEAU)
		m_pD3DDevice->DrawPrimitive(D3DPT_LINESTRIP,i,DIV_ANNEAU-1);



    //
    // Cache rotational positions between frames...
    //

    static float fSunSpin    = 0.0f;  

    static float fEarthSpin  = 0.0f;    
    static float fEarthOrbit = 0.0f;

    static float fMoonSpin   = 0.0f;
    static float fMoonOrbit  = 0.0f;

	static float fMarsSpin   = 0.0f;
	static float fMarsOrbit  = 60.0f;

	static float fMercureSpin   = 0.0f;
	static float fMercureOrbit  = 150.0f;

	static float fPlutonSpin   = 0.0f;
	static float fPlutonOrbit  = 210.0f;

	static float fVenusSpin   = 0.0f;
	static float fVenusOrbit  = 260.0f;

	double DeltaTemps = m_Temps.TempsEcouler();
	//Console<<"time : "<<DeltaTemps<<endl;
	//Console<<"speed spin : "<<m_fSpeedmodifier<<endl;

    if( m_bOrbitOn == true )
    {
        fSunSpin += m_fSpeedmodifier * (DeltaTemps * 10.0f);

		fMarsSpin  += m_fSpeedmodifier * (DeltaTemps * 50.0f);
        fMarsOrbit += m_fSpeedmodifier * (DeltaTemps * 20.0f);

        fEarthSpin  += m_fSpeedmodifier * (DeltaTemps * 100.0f);
        fEarthOrbit += m_fSpeedmodifier * (DeltaTemps * 20.0f) ;

        fMoonSpin  += m_fSpeedmodifier * (DeltaTemps * 50.0f);
        fMoonOrbit += m_fSpeedmodifier * (DeltaTemps * 200.0f);


		fMercureSpin   += m_fSpeedmodifier * (DeltaTemps * 50.0f);
		fMercureOrbit  += m_fSpeedmodifier * (DeltaTemps * 20.0f);

		fPlutonSpin   += m_fSpeedmodifier * (DeltaTemps * 50.0f);
		fPlutonOrbit  += m_fSpeedmodifier * (DeltaTemps * 20.0f);

		fVenusSpin   += m_fSpeedmodifier * (DeltaTemps * 50.0f);
		fVenusOrbit  += m_fSpeedmodifier * (DeltaTemps * 20.0f);

    }
	// On active le calcule de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, m_blightActive);

	// ############################## SOLEIL ############################

    D3DXMATRIX mSunSpinRotation;
    
    D3DXMatrixRotationY( &mSunSpinRotation, D3DXToRadian( fSunSpin ) );
    
    // Now, concatenate them together...

    m_MSunMatrix = mSunSpinRotation;

    m_pD3DDevice->SetTransform( D3DTS_WORLD, &m_MSunMatrix ); 
	
    m_pSoleil->RenduModel();



    D3DXMATRIX mMarsTranslationToOrbit;
    D3DXMATRIX mMarsSpinRotation;
    D3DXMATRIX mMarsOrbitRotation;

	// ########################### MARS ###############################

    D3DXMatrixRotationY( &mMarsSpinRotation, D3DXToRadian( fMarsSpin ) );
    D3DXMatrixTranslation( &mMarsTranslationToOrbit, 0.0f, 0.0f, g_pfTaille[1]);
    D3DXMatrixRotationY( &mMarsOrbitRotation, D3DXToRadian( fMarsOrbit ) );


    m_MMarsMatrix =       // 1. Spin the Earth on its own axis.
                   mMarsTranslationToOrbit * // 2. Then translate it away from the origin (where the Sun's at)
                   mMarsOrbitRotation;       // 3. and rotate it again to make it orbit the origin (or the Sun).

    m_pD3DDevice->SetTransform( D3DTS_WORLD, &(mMarsSpinRotation * m_MMarsMatrix) );
    m_pMars->RenduModel();
	
	// ########################## MERCURE #############################

	D3DXMATRIX mMercureTranslationToOrbit;
    D3DXMATRIX mMercureSpinRotation;
    D3DXMATRIX mMercureOrbitRotation;

    D3DXMatrixRotationY( &mMercureSpinRotation, D3DXToRadian( fMercureSpin ) );
    D3DXMatrixTranslation( &mMercureTranslationToOrbit, 0.0f, 0.0f, g_pfTaille[2] );
    D3DXMatrixRotationY( &mMercureOrbitRotation, D3DXToRadian( fMercureOrbit ) );


    m_MMercurenMatrix =       // 1. Spin the Earth on its own axis.
                   mMercureTranslationToOrbit * // 2. Then translate it away from the origin (where the Sun's at)
                   mMercureOrbitRotation;       // 3. and rotate it again to make it orbit the origin (or the Sun).

    m_pD3DDevice->SetTransform( D3DTS_WORLD, &(mMercureSpinRotation * m_MMercurenMatrix) );
    m_pMercure->RenduModel();

	// ########################## VENUS ################################

	D3DXMATRIX mVenusTranslationToOrbit;
    D3DXMATRIX mVenusSpinRotation;
    D3DXMATRIX mVenusOrbitRotation;
	
    D3DXMatrixRotationY( &mVenusSpinRotation, D3DXToRadian( fVenusSpin ) );
    D3DXMatrixTranslation( &mVenusTranslationToOrbit, 0.0f, 0.0f, g_pfTaille[3] );
    D3DXMatrixRotationY( &mVenusOrbitRotation, D3DXToRadian( fVenusOrbit ) );


    m_MVenusMatrix =       // 1. Spin the Earth on its own axis.
                   mVenusTranslationToOrbit * // 2. Then translate it away from the origin (where the Sun's at)
                   mVenusOrbitRotation;       // 3. and rotate it again to make it orbit the origin (or the Sun).

    m_pD3DDevice->SetTransform( D3DTS_WORLD, &(mVenusSpinRotation * m_MVenusMatrix) );
    m_pVenus->RenduModel();

	// ######################### PLUTON ################################

	D3DXMATRIX mPlutonTranslationToOrbit;
    D3DXMATRIX mPlutonSpinRotation;
    D3DXMATRIX mPlutonOrbitRotation;

    D3DXMatrixRotationY( &mPlutonSpinRotation, D3DXToRadian( fPlutonSpin ) );
    D3DXMatrixTranslation( &mPlutonTranslationToOrbit, 0.0f, 0.0f, g_pfTaille[4] );
    D3DXMatrixRotationY( &mPlutonOrbitRotation, D3DXToRadian( fPlutonOrbit ) );


    m_MPlutonMatrix =							// 1. Spin the Earth on its own axis.
                   mPlutonTranslationToOrbit * // 2. Then translate it away from the origin (where the Sun's at)
                   mPlutonOrbitRotation;       // 3. and rotate it again to make it orbit the origin (or the Sun).

    m_pD3DDevice->SetTransform( D3DTS_WORLD, &(mPlutonSpinRotation * m_MPlutonMatrix) );
    m_pPluton->RenduModel();

	// ########################## TERRE #############################

    D3DXMATRIX mEarthTranslationToOrbit;
    D3DXMATRIX mEarthSpinRotation;
    D3DXMATRIX mEarthOrbitRotation;

    D3DXMatrixRotationY( &mEarthSpinRotation, D3DXToRadian( fEarthSpin ) );
    D3DXMatrixTranslation( &mEarthTranslationToOrbit, 0.0f, 0.0f, g_pfTaille[0] );
    D3DXMatrixRotationY( &mEarthOrbitRotation, D3DXToRadian( fEarthOrbit ) );


    m_MmEarthMatrix =        // 1. Spin the Earth on its own axis.
                   mEarthTranslationToOrbit * // 2. Then translate it away from the origin (where the Sun's at)
                   mEarthOrbitRotation;       // 3. and rotate it again to make it orbit the origin (or the Sun).

    m_pD3DDevice->SetTransform( D3DTS_WORLD, &(mEarthSpinRotation *m_MmEarthMatrix) );
    m_pTerre->RenduModel();

	// ######################## LUNE ################################

    D3DXMATRIX mMoonTranslationToOrbit;
    D3DXMATRIX mMoonSpinRotation;
    D3DXMATRIX mMoonOrbitRotation;
    

    D3DXMatrixRotationY( &mMoonSpinRotation, D3DXToRadian( fMoonSpin ) );
    D3DXMatrixRotationY( &mMoonOrbitRotation, D3DXToRadian( fMoonOrbit ) );
    D3DXMatrixTranslation( &mMoonTranslationToOrbit, 0.0f, 0.0f, g_pfTaille[5] );

    
    m_MMoonMatrix =					        // 1. Spin the Moon on its own axis.
                  mMoonTranslationToOrbit *  // 2. Then translate it away from the origin (pretending that the Earth is there)
                  mMoonOrbitRotation *       // 3. and rotate it again to make it orbit the origin (or the pretend Earth).
                  
                  mEarthTranslationToOrbit *   // 4. Now, translate out to where the Earth is really at
                  mEarthOrbitRotation;       // 5. and move with it by matching its orbit of the Earth.

    m_pD3DDevice->SetTransform( D3DTS_WORLD, &(mMoonSpinRotation *m_MMoonMatrix) );
    m_pLune->RenduModel();
	m_pD3DDevice->DrawPrimitive(D3DPT_LINESTRIP,(NB_ANNEAU-1)*DIV_ANNEAU,DIV_ANNEAU-1);
	

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	switch(m_nChoix)
	{
	case 0:m_pInterface->afficher(m_pSoleil);break;
	case 1:m_pInterface->afficher(m_pTerre);;break;
	case 2:m_pInterface->afficher(m_pLune);break;
	case 3:m_pInterface->afficher(m_pMars);break;
	case 4:m_pInterface->afficher(m_pMercure);break;
	case 5:m_pInterface->afficher(m_pVenus);break;
	case 6:m_pInterface->afficher(m_pPluton);break;
	default:break;
	}
	

}

// Calculs des animations de la scène
void CSceneSystemSolaire::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
}

// Test des entrées des périphériques
void CSceneSystemSolaire::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
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
		m_pTerre->augmenterdScale(0.001f);
		m_pLune->augmenterdScale(0.001f);
		m_pMars->augmenterdScale(0.001f);
		m_pSoleil->augmenterdScale(0.001f);
	}
	if(KEYDOWN(DIK_2))
	{
		m_pTerre->diminuerdScale(0.001f);
		m_pLune->diminuerdScale(0.001f);
		m_pMars->diminuerdScale(0.001f);
		m_pSoleil->diminuerdScale(0.001f);
	}

	if(KEYDOWN(DIK_3))
	{
		m_pTerre->augmenterbScale(0.001f);
		m_pLune->augmenterbScale(0.001f);
		m_pMars->augmenterbScale(0.001f);
		m_pSoleil->augmenterbScale(0.001f);
	}
	if(KEYDOWN(DIK_4))
	{
		m_pTerre->diminuerbScale(0.001f);
		m_pLune->diminuerbScale(0.001f);
		m_pMars->diminuerbScale(0.001f);
		m_pSoleil->diminuerbScale(0.001f);
	}

	if(KEYDOWN(DIK_5))
	{
		m_blightActive = true;
	}
	if(KEYDOWN(DIK_6))
	{
		m_blightActive = false;
	}

	if(KEYDOWN(DIK_7))
	{
		m_fSpeedmodifier-=0.01;
	}
	if(KEYDOWN(DIK_8))
	{
		m_fSpeedmodifier+=0.01;
	}

	if(KEYDOWN(DIK_F5))//SOLEIL
		m_nChoix = 0;

	if(KEYDOWN(DIK_F6))//TERRE
		m_nChoix = 1;

	if(KEYDOWN(DIK_F7))//LUNE
		m_nChoix = 2;

	if(KEYDOWN(DIK_F8))//MARS
		m_nChoix = 3;

	if(KEYDOWN(DIK_F9))//MERCURE
		m_nChoix = 4;

	if(KEYDOWN(DIK_F10))//VENUS	
		m_nChoix = 5;

	if(KEYDOWN(DIK_F11))//PLUTON
		m_nChoix = 6;

	if(KEYDOWN(DIK_H))
		m_pInterface->m_bControles = true;
	if(KEYDOWN(DIK_N))
		m_pInterface->m_bControles = false;

}

// Appliqué les paramètres de filtrage de texture
void CSceneSystemSolaire::SetFiltreTexture(int Type)
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
