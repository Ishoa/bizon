#include "SceneShader.h"

#define NB_STAGE 2


CSceneShader::CSceneShader(CMoteur* pMoteur):CScene(pMoteur)
{
	m_QualiterFiltrage = 1;

	m_pVertexBuffer = NULL;   
	m_pIndexBuffer = NULL;      
	m_pVertexShader = NULL;   
	m_pPixelShader = NULL;   
	m_pConstantTableVertexShader = NULL;  
	m_pConstantTablePixelShader = NULL;   
}

CSceneShader::~CSceneShader()
{
	Console << "Deconstructeur CSceneTemplate" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneShader::ConfirmeFonction3D(D3DCAPS9* pCaps)
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

	if (!(pCaps->PixelShaderVersion  >= D3DPS_VERSION(2,0)) && !(pCaps->VertexShaderVersion >= D3DVS_VERSION(2,0)) ) 
	{

		Console<<"VS2 or PS2 HW not available\nSwitching to reference driver"<<endl;
		return false;
	}

	return true;
}

// Initialisation de la scène 3D
bool CSceneShader::Initialisation()
{

	if(m_pCameraParDefaut != NULL)
		delete m_pCameraParDefaut;
	m_pCameraParDefaut = new CCameraTrackBall(this);
	//m_pCameraParDefaut = new CCameraVolLibre(this);	
	m_pCameraParDefaut->Creation();

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(0.0f, 0.0f, 5.0f));
	m_pCameraParDefaut->SetCible(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	m_Lumiere.SetPonctuelle(&D3DXVECTOR3(5.0f,5.0f,5.0f),50.0f);
	// de Base la lumiere est activé
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	return true;
}

// Création des objets 3D de la scène
bool CSceneShader::CreationObjet()
{
	m_ndiv = 8; // nombre de decoupage pour un demi-cercle
	m_nvert = (m_ndiv+1)*(2*m_ndiv+1);
	m_nfac = m_ndiv*(2*m_ndiv)*2*3;

	void     *ptr = NULL;

	//////////////////////////////////////////////////////////////////////////
	// creation de la sphere
	//////////////////////////////////////////////////////////////////////////
	{
		// creation de la sphere
		Vertex*  sph = new Vertex[m_nvert];
		float   ph=-D3DX_PI/2, th=-D3DX_PI/2, da = D3DX_PI/m_ndiv;
		int       i,j,k=0;
		for(i=0; i<=m_ndiv;i++,ph+=da) 
		{
			for(j=0,th=0;j<=2*m_ndiv;j++,th+=da,k++) 
			{
				sph[k].Position = D3DXVECTOR3( cosf(th)*cosf(ph), sinf(th)*cosf(ph), sinf(ph) );
				sph[k].Normal = sph[k].Position;
				sph[k].diffuse = D3DCOLOR_XRGB(255,255,255);
				//sph[k].tu1 = asinf(sph[k].Position.x)+0.5f;
				//sph[k].tv1 = asinf(sph[k].Position.y)+0.5f;
				sph[k].tu1 = j/(float)(m_ndiv);
				sph[k].tv1 = i/(float)(m_ndiv);
			}
		}

		// creation du vertex buffer
		m_pD3DDevice->CreateVertexBuffer( m_nvert*sizeof(Vertex), 0, 0, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );
		m_pVertexBuffer->Lock( 0, m_nvert*sizeof(Vertex), (void**)&ptr, 0 );
		memcpy(ptr, sph, m_nvert*sizeof(Vertex) );
		m_pVertexBuffer->Unlock();
	}

	// creation de l'index buffer
	  
	short int* ind=new short int[m_nfac];
	int i,j,k=0,d=0,n=2*m_ndiv;
	//for(j=0; j<n; j++) { ind[k++]=d+n+1+j; ind[k++]=d+j; ind[k++]=d+n+1+j+1; }
	//d+=n+1;
	for(i=0;i<m_ndiv/*-2*/;i++,d+=n+1) for(j=0; j<n; j++) {
		ind[k++]=d+n+1+j;   ind[k++]=d+j; ind[k++]=d+n+1+j+1; 
		ind[k++]=d+n+1+j+1; ind[k++]=d+j; ind[k++]=d+j+1;
	}
	// for(j=0; j<n; j++) { ind[k++]=d+n+1+j+1; ind[k++]=d+j; ind[k++]=d+j+1; }

	m_pD3DDevice->CreateIndexBuffer(m_nfac*sizeof(short int),0,D3DFMT_INDEX16, D3DPOOL_MANAGED,&m_pIndexBuffer,NULL);
	m_pIndexBuffer->Lock(0,m_nfac*sizeof(short int),&ptr,0);
	memcpy(ptr,ind,m_nfac*sizeof(short int));
	m_pIndexBuffer->Unlock();   
	

	// fixe le FVF (normalement dans la boucle de rendu, mais on a qu'un seul objet)
	m_pD3DDevice->SetFVF( Vertex::FVF_Flags );
	m_pD3DDevice->SetIndices(m_pIndexBuffer);
	m_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(Vertex) );


	// chargement d'une texture
	//if FAILED( D3DXCreateTextureFromFile( m_pD3DDevice, "..\\Donnees\\misc\\Earth_Diffuse.dds", &m_pTexture)) {
	if (!m_pTexture.chargerTexture( m_pD3DDevice, "..\\Donnees\\Textures\\checker.bmp"))
	{
		Console<<"Erreur chargement Checker.bmp"<<endl;
		return false;
	}

	m_pD3DDevice->SetTexture(0,m_pTexture.getDXTexture());


	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);


	// Initialisation de la lumière 1 et activation

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	HRESULT       s;
	LPD3DXBUFFER    pShadObj, pErrMsg;

	///////////////////////////////////////////////////////////////////////////
	// compilation du vertex shader
	s = D3DXCompileShaderFromFile("..\\Shader\\BasicTransformAndLightningSpecular.fx",NULL,NULL,"VSmain","vs_2_0",0,&pShadObj,&pErrMsg,&m_pConstantTableVertexShader);
	if FAILED(s) {/*
		char *ErrMsg = (char *)(pErrMsg ? pErrMsg->GetBufferPointer() : DXGetErrorDescription9(s));
		Console<<" HLSL compilation Vertex Shader error : "<<ErrMsg<<endl;
		SAFE_RELEASE(pErrMsg);*/	
		return false;
	}
	SAFE_RELEASE(pErrMsg);	
	// creation du vertex shader
	m_pD3DDevice->CreateVertexShader( (DWORD *)pShadObj->GetBufferPointer(), &m_pVertexShader );

	///////////////////////////////////////////////////////////////////////////
	// compilation du pixel shader
	s = D3DXCompileShaderFromFile("..\\Shader\\BasicTransformAndLightningSpecular.fx",NULL,NULL,"PSmain","ps_2_0",0,&pShadObj,&pErrMsg,&m_pConstantTablePixelShader);
	if FAILED(s) {/*
		char *ErrMsg = (char *)(pErrMsg ? pErrMsg->GetBufferPointer() : DXGetErrorDescription9(s));
		Console<<" HLSL compilation Pixel Shader error : "<<ErrMsg<<endl;
		SAFE_RELEASE(pErrMsg);	*/
		return false;
	}
	SAFE_RELEASE(pErrMsg);	
	// creation du vertex shader
	m_pD3DDevice->CreatePixelShader( (DWORD *)pShadObj->GetBufferPointer(), &m_pPixelShader );

	DxDataBase.affichageStatus();
	return true;
}

// Destruction des resource utilisé par les objets
void CSceneShader::DestructionObjet()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	m_pTexture.Release();
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

// Destruction des objets
void CSceneShader::Destruction()
{

}

// Rendu 3D de la scène
void CSceneShader::Rendu3D()
{
	

	// position de l'observateur dans le repere de l'objet
	D3DXVECTOR4 Opos,Lpos;
	D3DXMATRIXA16 MWorld,MIMWorld,MView,MProj,Mfull,TMfull;

	D3DXMatrixIdentity(&MWorld);


	float det=D3DXMatrixDeterminant(&MWorld);
	D3DXMatrixInverse(&MIMWorld,&det,&MWorld);

	MProj = m_pCameraParDefaut->GetMatriceProjection();
	MView = m_pCameraParDefaut->GetMatriceVision();
	Mfull = MWorld * MView * MProj;
	D3DXMatrixTranspose(&TMfull,&Mfull); // transposé pour le shader

	D3DXVECTOR3 ObsPos = m_pCameraParDefaut->GetPosition(); // position de la source dans le repere global

	D3DXVec3Transform(&Opos,&ObsPos,&MIMWorld); // position de la lampe dans le repere local de l'objet (eviter d'avoir a transformer la normale)
	// caracteristique de la source
	
	D3DXVECTOR3 LightPos = m_Lumiere.GetLumiere()->Position; // position de la source dans le repere global
	D3DXVec3Transform(&Lpos,&LightPos,&MIMWorld); // position de la lampe dans le repere local de l'objet (eviter d'avoir a transformer la normale)


	D3DCOLORVALUE LightCol={1.f, 1.f, 1.f, 1.f};
	D3DXVECTOR3 LightCff = D3DXVECTOR3( 20.f, 0.f, 0.f );

	// fixe le vertex shader
	
	m_pD3DDevice->SetVertexShaderConstantF(0,(float *)&TMfull,4);  // Transformation complete (c0,c1,c2,c3)
	
	D3DXVec3Transform(&Lpos,&LightPos,&MIMWorld); // position de la lampe dans le repere local de l'objet (eviter d'avoir a transformer la normale)
	m_pD3DDevice->SetVertexShaderConstantF(4,(float *)&LightPos,1); // c4
	m_pD3DDevice->SetVertexShader(m_pVertexShader);

	// fixe le pixel shader
	m_pD3DDevice->SetPixelShaderConstantF(0,(float *)&LightCol,1);
	m_pD3DDevice->SetPixelShaderConstantF(1,(float *)&Opos,1);
	m_pD3DDevice->SetPixelShaderConstantF(2,(float *)&LightCff,1);
	m_pD3DDevice->SetPixelShader(m_pPixelShader);


	// dessin de la primitive
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_nvert,0,m_nfac/3);

}

// Calculs des animations de la scène
void CSceneShader::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
}

// Test des entrées des périphériques
void CSceneShader::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
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
void CSceneShader::SetFiltreTexture(int Type)
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
