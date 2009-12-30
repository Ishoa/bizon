#include "ModelEau.h"

inline DWORD F2DW(float f) {return *((DWORD *)&f);}

// Constructeur
CModelEau::CModelEau(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pVB = NULL;
	m_pIB = NULL;
	m_pTexBump = NULL;
	m_RenduMiroir = false;
	m_pEnvMiroir = new CEnvMiroir(pScene);
	D3DXMatrixIdentity(&m_MatTransEau);


	m_fLargeur			= 512.0f;	// largeur de l'eau
	m_fLongeur			= 512.0f;	// longeur de l'eau
	m_fPosition_Eau_X  = 97.0f;	// position en x de l'eau
	m_fPosition_Eaux_Y  = -3.0f;	// position en y de l'eau
}

// Déconstructeur
CModelEau::~CModelEau()
{	
	delete m_pEnvMiroir;
}

// Création de l'eau
bool CModelEau::CreationEau(float fPositionX, float fPositionY, float fPositionZ, float fLongeur, float fLargeur)
{
	m_fLargeur			= fLargeur;	// largeur de l'eau
	m_fLongeur			= fLongeur / NB_ZONE_EAU;	// longeur de l'eau

	m_fPosition_Eau_X  = fPositionX;	// position en x de l'eau
	m_fPosition_Eaux_Y  = fPositionY;	// position en y de l'eau
	m_fPosition_Eau_Z = fPositionZ;

	// Vertex pour une zone
	Donnee[0].p = D3DXVECTOR3(m_fPosition_Eau_X - m_fLargeur/2.0f, m_fPosition_Eaux_Y, m_fLongeur);
	Donnee[0].t = D3DXVECTOR2(0.0f, 0.0f);
	Donnee[1].p = D3DXVECTOR3(m_fPosition_Eau_X + m_fLargeur/2.0f, m_fPosition_Eaux_Y, m_fLongeur);
	Donnee[1].t = D3DXVECTOR2(1.0f*COEF_TEXTURE_X, 0.0f);
	Donnee[2].p = D3DXVECTOR3(m_fPosition_Eau_X + m_fLargeur/2.0f, m_fPosition_Eaux_Y, 0.0f);
	Donnee[2].t = D3DXVECTOR2(1.0f*COEF_TEXTURE_X, 1.0f*COEF_TEXTURE_Y);
	Donnee[3].p = D3DXVECTOR3(m_fPosition_Eau_X - m_fLargeur/2.0f, m_fPosition_Eaux_Y, 0.0f);
	Donnee[3].t = D3DXVECTOR2(0.0f, 1.0f*COEF_TEXTURE_Y);

	// On crée un plan par rapport au point de la zone
	D3DXPlaneFromPoints(&m_Plan, &Donnee[0].p, &Donnee[1].p, &Donnee[2].p);
	// Création d'un matrice de réflexion
	D3DXMatrixReflect(&m_MatriceReflection, &m_Plan);
	return true;
}

// Charge les données dans la mémoire vidéo
bool CModelEau::ChargerEau()
{
	VOID* pVerticesVideo = NULL;
	// Création du vertex buffer
	m_pD3DDevice->CreateVertexBuffer(NB_VERTEX_EAU*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX_EAUX, D3DPOOL_DEFAULT, &m_pVB, NULL);
	// Copie des vertex du modèl dans la mémoire video
	m_pVB->Lock(0, NB_VERTEX_EAU*sizeof(CUSTOMVERTEX), (void**)&pVerticesVideo, 0);
	memcpy(pVerticesVideo, Donnee, NB_VERTEX_EAU*sizeof(CUSTOMVERTEX));
	m_pVB->Unlock();
	// Création d'un tableau pour stocker les indices
	WORD* pIndices = new WORD[NB_INDICE_EAU];
	WORD* pt = pIndices;

	// On va remplire le tableau d'indice pour former une liste de triangle de type D3DPT_TRIANGLELIST
	*pt++ = 0;
	*pt++ = 1;
	*pt++ = 2;
	*pt++ = 2;
	*pt++ = 3;
	*pt++ = 0;

	VOID* pIndicesVideo = NULL;
	// Création de l'index buffer
	m_pD3DDevice->CreateIndexBuffer(NB_INDICE_EAU*sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);
	m_pIB->Lock(0, NB_INDICE_EAU*sizeof(WORD), (void**)&pIndicesVideo, 0);
	// Copie des indices dans la mémoire video
	memcpy(pIndicesVideo, pIndices, NB_INDICE_EAU*sizeof(WORD));
	m_pIB->Unlock();

	delete[] pIndices;

	// Création de l'objet Miroir 
	m_pEnvMiroir->Creation(&m_Plan);
	// Chargement de la texture
	if(FAILED(D3DXCreateTextureFromFileEx(m_pD3DDevice, FcihierTxBump, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, D3DFMT_V8U8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &m_pTexBump)))
		return FALSE;

	// Création d'un plan pour le clipping
	D3DXPLANE PlanClipping;
	D3DXPlaneFromPoints(&PlanClipping, &D3DXVECTOR3(0.0f, m_fPosition_Eaux_Y-0.5f, 0.0f), &D3DXVECTOR3(0.0f, m_fPosition_Eaux_Y-0.5f, 1.0f), &D3DXVECTOR3(1.0f, m_fPosition_Eaux_Y-0.5f, 1.0f));
	m_pD3DDevice->SetClipPlane(0, PlanClipping);
	return true;
}

// Libération des ressources
void CModelEau::Release()
{
	SAFE_RELEASE(m_pTexBump);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	m_pEnvMiroir->Release();
}

// Mise à jour des informations des zones
void CModelEau::MiseAJourDonnees(double TempsEcouler)
{
	static float OffsetZoneLongeur = -float(m_fLongeur)/2.0f;
	static float OffsetZoneLargeur = float(m_fLargeur)/2.0f;
	static D3DXVECTOR3 PositionCamera;
	static float OffsetModelLongeur;
	//static float OffsetModelLargeur;

	// On récupère la position de la caméra
	PositionCamera = m_pScene->GetCameraCourante()->GetPosition();

	//int fZ = int(PositionCamera.z);
	float fZ = 0.0f;

	// Mise à jour des informations des zones
	for(int i = 0 ; i < NB_ZONE_EAU ; i++)
	{
		// Calcul de la matrice de trasformation du model
		OffsetModelLongeur = float(m_fLongeur * i + fZ * m_fLongeur) + OffsetZoneLongeur;
		//OffsetModelLargeur = float(m_fLargeur* i + fZ * m_fLargeur) + OffsetZoneLargeur;
		D3DXMatrixTranslation(&m_Matrice[i], OffsetZoneLargeur, 0.0f, OffsetModelLongeur);
	}

	// Calcul de la matrice de transformation de l'eau
	m_MatTransEau._32 = (float)TempsEcouler / VITESSE_EAU;
}

// Rendu de la texture de réflexion
void CModelEau::RenduEnvMiroir()
{
	// On active le clliping
	m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	m_RenduMiroir = true;
	m_pEnvMiroir->RenduMiroir();
	m_RenduMiroir = false;
	// On déactive le clipping
	m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
}

// Rendu de la roche
void CModelEau::RenduEau()
{
	// Déactivation de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// On n'affiche pas lae sol pendant le rendu du miroir
	if(m_RenduMiroir == false)
	{
		// Activation de la transparence
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
		// Niveau de transparence de la texture
		m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0x90000000);
		
		int nEtage = 0;


		// Etage 0 : activation des paramètres du bump mapping
		m_pD3DDevice->SetTexture(nEtage, m_pTexBump);
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_BUMPENVMAT00, F2DW(0.01f));
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_BUMPENVMAT11, F2DW(0.01f));
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_BUMPENVLSCALE, F2DW(1.0f));
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f));
		m_pD3DDevice->SetTransform(D3DTS_TEXTURE0, &m_MatTransEau);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

		nEtage++;

		// Etage 1 : affichage de la texture de réflexion
		m_pD3DDevice->SetTexture(nEtage, m_pEnvMiroir->GetMiroir());
		m_pD3DDevice->SetTextureStageState (nEtage, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (nEtage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState (nEtage, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (nEtage, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
		m_pD3DDevice->SetTransform(D3DTS_TEXTURE1, &m_pScene->GetCameraCourante()->GetMatriceProjectionTexture());
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED |D3DTTFF_COUNT3);
		m_pD3DDevice->SetSamplerState(nEtage, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP);
		m_pD3DDevice->SetSamplerState(nEtage, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP);
		
		nEtage ++;
		
		// Etage 2 : fin des opérations
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		m_pD3DDevice->SetTextureStageState(nEtage, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
		// Initialisation des buffers
		m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX_EAUX);
		m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
		m_pD3DDevice->SetIndices(m_pIB);
		// Rendu de l'eau
		for(int i = 0 ; i < NB_ZONE_EAU ; i++)
		{
			m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_Matrice[i]);
			m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NB_VERTEX_EAU, 0, NB_TRIANGLE_EAU);
		}
		// On remet les paramètres dans l'état initiale
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
		m_pD3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP);
		m_pD3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}