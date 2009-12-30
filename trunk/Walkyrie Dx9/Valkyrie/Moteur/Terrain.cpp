#include "Terrain.h"

CTerrain::CTerrain()
{
}

CTerrain::CTerrain(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pVBTerrain = NULL;

	m_NbPointX = 0;
	m_NbPointY = 0;
	m_NbMailleX = 0;
	m_NbMailleY = 0;
	m_NbVertex = 0;
	m_OffsetVertex = 0;
	m_NbTriangles = 0;
	m_RecouvrementTexure = 1.0f/4.0f;		// 4 textures par quad
	m_CoefHauteur = 255.0f/102.0f;
	m_HauteurEau = 13.2f;
	m_TranslationEau = 10.0f;
	m_MapHauteur = NULL;
	m_pVertexTerrain = NULL;

	D3DXMatrixIdentity(&m_MatTransEau);
}

CTerrain::~CTerrain()
{
	delete[] m_MapHauteur;
	delete[] m_pVertexTerrain;
}

// Création du terrain
bool CTerrain::CreationTerrain(char * FichierMapTerrain, float fDecalageHauteur)
{
	if(!ChargerMapHauteur(FichierMapTerrain, fDecalageHauteur))
		return false;
	LisserMapHauteur();

	if(!CreationMaillage())
		return false;
	return true;
}

// Charger les données du terrain dans la mémoire video
bool CTerrain::ChargerTerrain()
{

	VOID* pVertex = NULL;
	// Création du vertex buffer
	m_pD3DDevice->CreateVertexBuffer(m_NbVertex*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVBTerrain, NULL);
	// On de bloqué un buffer pour transférer nos vertex vers la mémoire video
	m_pVBTerrain->Lock(0, m_NbVertex*sizeof(CUSTOMVERTEX), (void**)&pVertex, 0);
	// Copie des vertex dans la mémoire video
	memcpy(pVertex, m_pVertexTerrain, m_NbVertex*sizeof(CUSTOMVERTEX));
	// On libère le buffer bloqué
	m_pVBTerrain->Unlock();
	// On charge la végétation
	return true;
}

bool CTerrain::SetSurface(char* FichierTextureSurface)
{
	m_pTxHerbe.Release();
	return m_pTxHerbe.chargerTexture(m_pD3DDevice,FichierTextureSurface);
}

bool CTerrain::SetTerre(char* FichierDensiteTerre,char * FichierTextureTerre)
{
	m_pTxTerre.Release();
	m_pTxDensiteTerre.Release();

	return m_pTxTerre.chargerTexture(m_pD3DDevice,FichierTextureTerre) &&
		   m_pTxDensiteTerre.chargerTexture(m_pD3DDevice,FichierDensiteTerre);
}

bool CTerrain::SetEaux(char* FichierDensiteEau,char * FichierTextureEau, float fHauteurEau)
{
	m_HauteurEau = fHauteurEau;

	m_pTxEau.Release();
	m_pTxDensiteEau.Release();

	m_pVertexTerrain[m_OffsetVertex].y = m_HauteurEau;
	m_pVertexTerrain[m_OffsetVertex+1].y = m_HauteurEau;
	m_pVertexTerrain[m_OffsetVertex+2].y = m_HauteurEau;
	m_pVertexTerrain[m_OffsetVertex+3].y = m_HauteurEau;

	return m_pTxEau.chargerTexture(m_pD3DDevice,FichierTextureEau) &&
		   m_pTxDensiteEau.chargerTexture(m_pD3DDevice,FichierDensiteEau);
}


// Libération des ressources utilisé par le terrain
void CTerrain::Release()
{
	SAFE_RELEASE(m_pVBTerrain);
	m_pTxHerbe.Release();
	m_pTxTerre.Release();
	m_pTxDensiteTerre.Release();
	m_pTxEau.Release();
	m_pTxDensiteEau.Release();
}

// Rendu du terrain
void CTerrain::RenduTerrain()
{
	// Rendu de la végétations
	//m_pVegetation->RenduVegetation();

	// On active le Vertex Buffer du terrain
	m_pD3DDevice->SetStreamSource(0, m_pVBTerrain, 0, sizeof(CUSTOMVERTEX));
	// On initiallise le type de vertex
	m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);


	// On initialise le type de blend (mélange des textures avec l'alpha)
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


	/****************************************************************/
	/************************* TERRAIN + HERBES *********************/
	/****************************************************************/

	// Initialisation des stages de rendu (opération sur les textures)
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	// On change l'index des coordonnées de textures
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

	// Activation de la texture d'herbe
	m_pD3DDevice->SetTexture(0, m_pTxHerbe.getDXTexture());

	// Rendu de la permière couche du terrain : herbe 
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_NbTriangles);

	/****************************************************************/
	/***************************** TERRE ****************************/
	/****************************************************************/

	if(m_pTxTerre.getDXTexture() != NULL && m_pTxDensiteTerre.getDXTexture() != NULL)
	{
		// On change l'index des coordonnées de textures
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

		// Initialisation des stages de rendu (opération sur les textures)
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

		m_pD3DDevice->SetTextureStageState (2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		m_pD3DDevice->SetTextureStageState (2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

		// Activation des textures
		m_pD3DDevice->SetTexture(0, m_pTxDensiteTerre.getDXTexture());
		m_pD3DDevice->SetTexture(1, m_pTxTerre.getDXTexture());

		// Rendu de la deusième couche du terrain : terre 
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_NbTriangles);
	}

	/****************************************************************/
	/****************************** EAU *****************************/
	/****************************************************************/

	if(m_pTxDensiteEau.getDXTexture() != NULL && m_pTxEau.getDXTexture() != NULL)
	{
		// Initialisation des stages de rendu (opération sur les textures)
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

		m_pD3DDevice->SetTextureStageState (2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		m_pD3DDevice->SetTextureStageState (2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

		// On désactive le culling car on peux voir dessous ou dessus l'eau
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// On applique la matrice de transformation de l'eau 
		m_pD3DDevice->SetTransform(D3DTS_TEXTURE1, &m_MatTransEau);

		// On active les transformations de texture
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

		// Activation des textures
		m_pD3DDevice->SetTexture(0, m_pTxDensiteEau.getDXTexture());
		m_pD3DDevice->SetTexture(1, m_pTxEau.getDXTexture());

		// Rendu de l'eau
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, m_OffsetVertex, 2);
	}

	/****************************************************************/
	/***************************** RESET ****************************/
	/****************************************************************/

	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	// On remet les paramètres dans l'état initiale
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetTexture(0, NULL);
	m_pD3DDevice->SetTexture(1, NULL);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	
}

// Charger la map de hauteur à partir d'une image
bool CTerrain::ChargerMapHauteur(char* FichierMapTerrain, float fDecalageHauteur)
{
	CImageTGA ImageMapTerrain;
	// On charge une image en niveau de gris représentant la map de hauteur
	if(!ImageMapTerrain.Charger(FichierMapTerrain))
		return false;
	// Initialisation des diemention du terrain
	m_NbPointX = ImageMapTerrain.GetLargeur();
	m_NbPointY = ImageMapTerrain.GetHauteur();
	m_NbMailleX = m_NbPointX - 1;
	m_NbMailleY = m_NbPointY - 1;
	// Création du tableau d'altitude du terrain
	m_MapHauteur = new float[m_NbPointX * m_NbPointY];
	// On remplis le tableau en parcourant l'image
	for(int j=0; j<m_NbPointY; j++)
	{
		for(int i=0; i<m_NbPointX; i++)
		{
			TMapHauteur(i, j) = (float)ImageMapTerrain.GetPixel(i, j) - fDecalageHauteur;
		}
	}
	return true;
}

// On va lisser le terrain en hauteur pour evite les effets d'escalier à cause des valeurs entière
void CTerrain::LisserMapHauteur()
{
	float* pMapHauteurLisser = new float[m_NbPointX * m_NbPointY];

	// Lissage de l'image sans les bordures
	for(int j=1; j<m_NbPointY-1; j++)
	{
		for(int i=1; i<m_NbPointX-1; i++)
		{
			TMapHauteurLisser(i, j) =  (TMapHauteur(i    ,     j)
									+	TMapHauteur(i    , (j-1))
									+	TMapHauteur(i    , (j+1))
									+	TMapHauteur((i-1),     j)
									+	TMapHauteur((i+1),     j)
									+	TMapHauteur((i-1), (j-1))
									+	TMapHauteur((i+1), (j+1))
									+	TMapHauteur((i+1), (j-1))
									+	TMapHauteur((i-1), (j+1))) / 9.0f;
		}
	}
	// Lissage bordure gauche
	for(int j=1; j<m_NbPointY-1; j++)
	{
		TMapHauteurLisser(0, j) =  (TMapHauteur(0,     j)
								+   TMapHauteur(0, (j-1))
								+   TMapHauteur(0, (j+1))
								+   TMapHauteur(1,     j)
								+   TMapHauteur(1, (j-1))
								+   TMapHauteur(1, (j+1))) / 6.0f;
	}
	// Lissage bordure droite
	for(int j=1; j<m_NbPointY-1; j++)
	{
		TMapHauteurLisser((m_NbPointX-1), j) =  (TMapHauteur((m_NbPointX-1),     j)
											 +   TMapHauteur((m_NbPointX-1), (j-1))
								             +   TMapHauteur((m_NbPointX-1), (j+1))
								             +   TMapHauteur((m_NbPointX-2),     j)
								             +   TMapHauteur((m_NbPointX-2), (j-1))
								             +   TMapHauteur((m_NbPointX-2), (j+1))) / 6.0f;
	}
	// Lissage bordure haut
	for(int i=1; i<m_NbPointX-1; i++)
	{
		TMapHauteurLisser(i, 0) =  (TMapHauteur(i    , 0)
								+   TMapHauteur((i+1), 0)
								+   TMapHauteur((i-1), 0)
								+   TMapHauteur(i    , 1)
								+   TMapHauteur((i+1), 1)
								+   TMapHauteur((i-1), 1)) / 6.0f;
	}
	// Lissage bordure bas
	for(int i=1; i<m_NbPointX-1; i++)
	{
		TMapHauteurLisser(i, (m_NbPointY-1)) =  (TMapHauteur(i    , (m_NbPointY-1))
								             +   TMapHauteur((i+1), (m_NbPointY-1))
								             +   TMapHauteur((i-1), (m_NbPointY-1))
								             +   TMapHauteur(i    , (m_NbPointY-2))
								             +   TMapHauteur((i+1), (m_NbPointY-2))
								             +   TMapHauteur((i-1), (m_NbPointY-2))) / 6.0f;
	}
	// Lissage des angles de l'image
	TMapHauteurLisser(0, 0) = (TMapHauteur(0, 0) + TMapHauteur(0, 1) + TMapHauteur(1, 0) + TMapHauteur(1, 1)) / 4.0f;
	TMapHauteurLisser((m_NbPointX-1), 0) = (TMapHauteur((m_NbPointX-1), 0) + TMapHauteur((m_NbPointX-1), 1) + TMapHauteur((m_NbPointX-2), 0) + TMapHauteur((m_NbPointX-2), 1)) / 4.0f;
	TMapHauteurLisser(0, (m_NbPointY-1)) = (TMapHauteur(0, (m_NbPointY-1)) + TMapHauteur(1, (m_NbPointY-1)) + TMapHauteur(0, (m_NbPointY-2)) + TMapHauteur(1, (m_NbPointY-2))) / 4.0f;
	TMapHauteurLisser((m_NbPointX-1), (m_NbPointY-1)) = (TMapHauteur((m_NbPointX-1), (m_NbPointY-1)) + TMapHauteur((m_NbPointX-2), (m_NbPointY-1)) + TMapHauteur((m_NbPointX-1), (m_NbPointY-2)) + TMapHauteur((m_NbPointX-2), (m_NbPointY-2))) / 4.0f;

	// On libère l'ancien tableau et on le remplace par le nouveau
	delete[] m_MapHauteur;
	m_MapHauteur = pMapHauteurLisser;
}

// Création du maillage du terrain
bool CTerrain::CreationMaillage()
{
	// Calcule du nombre de vertex nécésaire pour le maillage du terrain
	m_NbVertex = (m_NbMailleY * 2 + 2) * m_NbMailleX;
	// Offset pour accéder au vertex après ceux du terrain
	m_OffsetVertex = m_NbVertex;
	// Nombre de triangle du maillage
	m_NbTriangles = m_NbVertex - 2;
	// 4 vertex de plus pour le quad de l'eau
	m_NbVertex += 4;
	// Création du tableau de vertex
	m_pVertexTerrain = new CUSTOMVERTEX[m_NbVertex];

	int c = 0;
	bool Sens = true;

	m_fCenterX = m_NbMailleX/2.0f;
	m_fCenterZ = m_NbMailleY/2.0f;

	// On va remplire le tableau de vetex pour former une suite de triangle de type D3DPT_TRIANGLESTRIP
	for(int i = 0; i < m_NbMailleX; i++)
	{
		if(Sens)
		{
			for(int j = 0; j <= m_NbMailleY; j++)
			{
				m_pVertexTerrain[c].x = float(i+1);
				m_pVertexTerrain[c].y = m_MapHauteur[(i+1)+j*m_NbPointX]/m_CoefHauteur;
				m_pVertexTerrain[c].z = float(j);
				m_pVertexTerrain[c].tx1 = (i+1)/(float)m_NbMailleX;
				m_pVertexTerrain[c].ty1 =     j/(float)m_NbMailleY;
				m_pVertexTerrain[c].tx2 = (float)(i+1)*m_RecouvrementTexure;
				m_pVertexTerrain[c].ty2 = (float)    j*m_RecouvrementTexure;
				c++;
				m_pVertexTerrain[c].x = float(i);
				m_pVertexTerrain[c].y = m_MapHauteur[i+j*m_NbPointX]/m_CoefHauteur;
				m_pVertexTerrain[c].z = float(j);
				m_pVertexTerrain[c].tx1 = i/(float)m_NbMailleX;
				m_pVertexTerrain[c].ty1 = j/(float)m_NbMailleY;
				m_pVertexTerrain[c].tx2 = (float)i*m_RecouvrementTexure;
				m_pVertexTerrain[c].ty2 = (float)j*m_RecouvrementTexure;
				c++;
			}
		}
		else
		{
			for(int j = m_NbMailleY; j >= 0; j--)
			{
				m_pVertexTerrain[c].x = float(i);
				m_pVertexTerrain[c].y = m_MapHauteur[i+j*m_NbPointX]/m_CoefHauteur;
				m_pVertexTerrain[c].z = float(j);
				m_pVertexTerrain[c].tx1 = i/(float)m_NbMailleX;
				m_pVertexTerrain[c].ty1 = j/(float)m_NbMailleY;
				m_pVertexTerrain[c].tx2 = (float)i*m_RecouvrementTexure;
				m_pVertexTerrain[c].ty2 = (float)j*m_RecouvrementTexure;
				c++;
				m_pVertexTerrain[c].x = float(i+1);
				m_pVertexTerrain[c].y = m_MapHauteur[(i+1)+j*m_NbPointX]/m_CoefHauteur;
				m_pVertexTerrain[c].z = float(j);
				m_pVertexTerrain[c].tx1 = (i+1)/(float)m_NbMailleX;
				m_pVertexTerrain[c].ty1 =    j/(float)m_NbMailleY;
				m_pVertexTerrain[c].tx2 = (float)(i+1)*m_RecouvrementTexure;
				m_pVertexTerrain[c].ty2 = (float)    j*m_RecouvrementTexure;
				c++;
			}
		}
		Sens = !Sens;
	}

	// Quad pour l'eau
	m_pVertexTerrain[c].x = (float)m_NbMailleX;
	m_pVertexTerrain[c].y = m_HauteurEau;
	m_pVertexTerrain[c].z = 0.0f;
	m_pVertexTerrain[c].tx1 = 1.0f;
	m_pVertexTerrain[c].ty1 = 0.0f;
	m_pVertexTerrain[c].tx2 = (float)m_NbMailleX*m_RecouvrementTexure;
	m_pVertexTerrain[c].ty2 = 0.0f;
	c++;
	m_pVertexTerrain[c].x = 0.0f;
	m_pVertexTerrain[c].y = m_HauteurEau;
	m_pVertexTerrain[c].z = 0.0f;
	m_pVertexTerrain[c].tx1 = 0.0f;
	m_pVertexTerrain[c].ty1 = 0.0f;
	m_pVertexTerrain[c].tx2 = 0.0f;
	m_pVertexTerrain[c].ty2 = 0.0f;
	c++;
	m_pVertexTerrain[c].x = (float)m_NbMailleX;
	m_pVertexTerrain[c].y = m_HauteurEau;
	m_pVertexTerrain[c].z = (float)m_NbMailleY;
	m_pVertexTerrain[c].tx1 = 1.0f;
	m_pVertexTerrain[c].ty1 = 1.0f;
	m_pVertexTerrain[c].tx2 = (float)m_NbMailleX*m_RecouvrementTexure;
	m_pVertexTerrain[c].ty2 = (float)m_NbMailleY*m_RecouvrementTexure;
	c++;
	m_pVertexTerrain[c].x = 0.0f;
	m_pVertexTerrain[c].y = m_HauteurEau;
	m_pVertexTerrain[c].z = (float)m_NbMailleY;
	m_pVertexTerrain[c].tx1 = 0.0f;
	m_pVertexTerrain[c].ty1 = 1.0f;
	m_pVertexTerrain[c].tx2 = 0.0f;
	m_pVertexTerrain[c].ty2 = (float)m_NbMailleY*m_RecouvrementTexure;
	c++;

	return true;
}



// Animation de l'eau
void CTerrain::Animation(double TempsEcouler)
{
	m_MatTransEau._32 = (float)TempsEcouler / m_TranslationEau;
}

// Calcule de la hauteur d'un point du terrain
float CTerrain::GetHauteur(float x, float z)
{

	
	float	h, fx, fz;
	int		xint, zint;

	xint = (int)x;
	zint = (int)z;

	if(xint<0 || xint>m_NbMailleX || zint<0 || zint>m_NbMailleY )
	{
		return 0.0f;
	}

	fx = x - xint;
	fz = z - zint;
	
	h = TMapHauteur(    xint,     zint)/m_CoefHauteur  * (1.0f-fx) * (1.0f-fz)
	  + TMapHauteur((xint+1),     zint)/m_CoefHauteur  *       fx  * (1.0f-fz)
	  + TMapHauteur(    xint, (zint+1))/m_CoefHauteur  * (1.0f-fx) * fz
	  + TMapHauteur((xint+1), (zint+1))/m_CoefHauteur  *       fx  * fz;

	return h;
}

