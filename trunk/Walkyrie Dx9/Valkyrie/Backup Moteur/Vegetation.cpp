#include "Vegetation.h"

CVegetation::CVegetation()
{
}

CVegetation::CVegetation(CTerrain* pTerrain)
{
	m_pTerrain		= pTerrain;
	m_pD3DDevice	= m_pTerrain->Get3DDevice();

	m_pVB			= NULL;
	m_pTxFleur		= NULL;
	m_pTxPlante		= NULL;
	m_pTxArbre1		= NULL;
	m_pTxArbre2		= NULL;
	m_pTxArbre3		= NULL;

	m_pFichierFleur = NULL;
	m_pFichierPlante = NULL;
	m_pFichierArbreType1 = NULL;
	m_pFichierArbreType2 = NULL;
	m_pFichierArbreType3 = NULL;

	m_NbPlante = 500;
	m_NbFleur  = 450;
	m_NbArbre1 = 150;
	m_NbArbre2 = 150;
	m_NbArbre3 = 150;

	m_NbArbre  = m_NbArbre1 + m_NbArbre2 + m_NbArbre3;

	m_NbVertex = 4 * 3;
}

CVegetation::~CVegetation()
{
	delete[] m_pVertex;
	delete[] m_pArbre;
	delete[] m_pPlante;
	delete[] m_pFleur;
}

// Création de la végétation
bool CVegetation::Creation(float Taille)
{
	// Création du tableau de vertex
	m_pVertex = new CUSTOMVERTEXV[m_NbVertex];
	int c = 0;

	// quad pour un arbre
	m_pVertex[c].x = 1.0f*Taille;
	m_pVertex[c].y = 2.0f*Taille;
	m_pVertex[c].z = 0.0f;
	m_pVertex[c].tx1 = 1.0f;
	m_pVertex[c].ty1 = 0.0f;
	c++;
	m_pVertex[c].x = -1.0f*Taille;
	m_pVertex[c].y = 2.0f*Taille;
	m_pVertex[c].z = 0.0f;
	m_pVertex[c].tx1 = 0.0f;
	m_pVertex[c].ty1 = 0.0f;
	c++;
	m_pVertex[c].x = 1.0f*Taille;
	m_pVertex[c].y = 0.0f;
	m_pVertex[c].z = 0.0f;
	m_pVertex[c].tx1 = 1.0f;
	m_pVertex[c].ty1 = 1.0f;
	c++;
	m_pVertex[c].x = -1.0f*Taille;
	m_pVertex[c].y = 0.0f;
	m_pVertex[c].z = 0.0f;
	m_pVertex[c].tx1 = 0.0f;
	m_pVertex[c].ty1 = 1.0f;
	c++;

	Taille = 0.5f;
	// quad pour une plante
	m_pVertex[c].x = 1.0f*Taille;
	m_pVertex[c].y = 4.0f*Taille;
	m_pVertex[c].z = -0.5f;
	m_pVertex[c].tx1 = 0.5f;
	m_pVertex[c].ty1 = 0.0f;
	c++;
	m_pVertex[c].x = -1.0f*Taille;
	m_pVertex[c].y = 4.0f*Taille;
	m_pVertex[c].z = -0.5f;
	m_pVertex[c].tx1 = 0.0f;
	m_pVertex[c].ty1 = 0.0f;
	c++;
	m_pVertex[c].x = 1.0f*Taille;
	m_pVertex[c].y = 0.0f;
	m_pVertex[c].z = 0.0f;
	m_pVertex[c].tx1 = 0.5f;
	m_pVertex[c].ty1 = 1.0f;
	c++;
	m_pVertex[c].x = -1.0f*Taille;
	m_pVertex[c].y = 0.0f;
	m_pVertex[c].z = 0.0f;
	m_pVertex[c].tx1 = 0.0f;
	m_pVertex[c].ty1 = 1.0f;
	c++;

	Taille = 1.0f;
	// quad pour une fleur
	m_pVertex[c].x = 1.0f*Taille;
	m_pVertex[c].y = 2.0f*Taille;
	m_pVertex[c].z = -0.5f;
	m_pVertex[c].tx1 = 1.0f;
	m_pVertex[c].ty1 = 0.0f;
	c++;
	m_pVertex[c].x = -1.0f*Taille;
	m_pVertex[c].y = 2.0f*Taille;
	m_pVertex[c].z = -0.5f;
	m_pVertex[c].tx1 = 0.0f;
	m_pVertex[c].ty1 = 0.0f;
	c++;
	m_pVertex[c].x = 1.0f*Taille;
	m_pVertex[c].y = 0.0f;
	m_pVertex[c].z = 0.0f;
	m_pVertex[c].tx1 = 1.0f;
	m_pVertex[c].ty1 = 1.0f;
	c++;
	m_pVertex[c].x = -1.0f*Taille;
	m_pVertex[c].y = 0.0f;
	m_pVertex[c].z = 0.0f;
	m_pVertex[c].tx1 = 0.0f;
	m_pVertex[c].ty1 = 1.0f;
	c++;

	return true;
}


bool CVegetation::SetMapFleur(char* FichierMapFleur, int nNbFleur, char * pFichierFleur)
{
	// Initialisation du générateur de nombre aléatoire
	srand((unsigned)time(NULL));

	m_NbFleur = nNbFleur;
	m_pFichierFleur = pFichierFleur;

	// On récupère des informations sur le terrain
	int TerrainX = m_pTerrain->GetNbPointX() - 1;
	int TerrainY = m_pTerrain->GetNbPointY() - 1;

	bool bPositionValide = false;
	float DeltaX = 0.0f;
	float DeltaZ = 0.0f;
	float x = 0.0f;
	float z = 0.0f;
	float RatioLargeurImage = 0.0f;
	float RatioHauteurImage = 0.0f;

	// Même principe pour les fleurs
	CImageTGA MapFleur;

	if(!MapFleur.Charger(FichierMapFleur))
		return false;

	m_pFleur = new D3DXVECTOR3[m_NbFleur];

	RatioLargeurImage = (MapFleur.GetLargeur() - 1) / (float)TerrainX;
	RatioHauteurImage = (MapFleur.GetHauteur() - 1) / (float)TerrainY;

	for(int i=0; i<m_NbFleur ; i++)
	{
		do
		{
			x = (rand() / (float)RAND_MAX) * TerrainX;
			z = (rand() / (float)RAND_MAX) * TerrainY;
			if(MapFleur.GetPixel((int)(x * RatioLargeurImage), (int)(z * RatioHauteurImage)) < 128)
				bPositionValide = false;
			else
			{
				bPositionValide = true;
				for(int j=0; j < i; j++)
				{
					DeltaX = fabs(x - m_pFleur[j].x);
					DeltaZ = fabs(z - m_pFleur[j].z);

					if(1.0f > powf(DeltaX, 2) + powf(DeltaZ, 2))
					{
						bPositionValide = false;
						j = i;
					}
				}
			}
		}while(!bPositionValide);
		m_pFleur[i].y = m_pTerrain->GetHauteur(x, z);
		m_pFleur[i].x = x;
		m_pFleur[i].z = z;
	}

	// Création des vertex pour la végétation
	return true;
}


bool CVegetation::SetMapPlante(char* FichierMapPlante, int nNbPlante, char * pFichierPlante)
{
	// Initialisation du générateur de nombre aléatoire
	srand((unsigned)time(NULL));

	m_NbPlante = nNbPlante;
	m_pFichierPlante = pFichierPlante;

	// On récupère des informations sur le terrain
	int TerrainX = m_pTerrain->GetNbPointX() - 1;
	int TerrainY = m_pTerrain->GetNbPointY() - 1;

	bool bPositionValide = false;
	float DeltaX = 0.0f;
	float DeltaZ = 0.0f;
	float x = 0.0f;
	float z = 0.0f;
	float RatioLargeurImage = 0.0f;
	float RatioHauteurImage = 0.0f;

	// Même principe pour les plantes
	CImageTGA MapPlante;
	if(!MapPlante.Charger(FichierMapPlante))
		return false;
	m_pPlante = new D3DXVECTOR3[m_NbPlante];
	RatioLargeurImage = (MapPlante.GetLargeur() - 1) / (float)TerrainX;
	RatioHauteurImage = (MapPlante.GetHauteur() - 1) / (float)TerrainY;
	for(int i=0; i<m_NbPlante ; i++)
	{
		do
		{
			x = (rand() / (float)RAND_MAX) * TerrainX;
			z = (rand() / (float)RAND_MAX) * TerrainY;
			if(MapPlante.GetPixel((int)(x * RatioLargeurImage), (int)(z * RatioHauteurImage)) < 128)
				bPositionValide = false;
			else
			{
				bPositionValide = true;
				for(int j=0; j < i; j++)
				{
					DeltaX = fabs(x - m_pPlante[j].x);
					DeltaZ = fabs(z - m_pPlante[j].z);

					if(0.5f > powf(DeltaX, 2) + powf(DeltaZ, 2))
					{
						bPositionValide = false;
						j = i;
					}
				}
			}
		}while(!bPositionValide);
		m_pPlante[i].y = m_pTerrain->GetHauteur(x, z);
		m_pPlante[i].x = x;
		m_pPlante[i].z = z;
	}

	return true;
}

bool CVegetation::SetMapArbre(char* FichierMapArbre, int nNbArbre, char * pFichierArbreType1,char * pFichierArbreType2,char * pFichierArbreType3)
{
	// Initialisation du générateur de nombre aléatoire
	srand((unsigned)time(NULL));

	if(pFichierArbreType2 != NULL)
	{
		m_NbArbre1 = (int)(nNbArbre/2);
		m_NbArbre2 = (int)(nNbArbre/2);
		m_NbArbre3 = 0;
	}
	if(pFichierArbreType2 != NULL && pFichierArbreType3 != NULL)
	{
		m_NbArbre1 = (int)(nNbArbre/3);
		m_NbArbre2 = (int)(nNbArbre/3);
		m_NbArbre3 = (int)(nNbArbre/3);
	}

	m_NbArbre = m_NbArbre1 + m_NbArbre2 + m_NbArbre3;

	m_pFichierArbreType1 = pFichierArbreType1;
	m_pFichierArbreType2 = pFichierArbreType2;
	m_pFichierArbreType3 = pFichierArbreType3;


	// On récupère des informations sur le terrain
	int TerrainX = m_pTerrain->GetNbPointX() - 1;
	int TerrainY = m_pTerrain->GetNbPointY() - 1;

	bool bPositionValide = false;
	float DeltaX = 0.0f;
	float DeltaZ = 0.0f;
	float x = 0.0f;
	float z = 0.0f;
	float RatioLargeurImage = 0.0f;
	float RatioHauteurImage = 0.0f;

	// On charge la map de probabilité des arbres
	CImageTGA MapArbre;
	if(!MapArbre.Charger(FichierMapArbre))
		return false;

	// Création du tableau de postion des arbre
	m_pArbre = new D3DXVECTOR3[m_NbArbre];

	// On calcule le ratio entre l'image et le terrain
	RatioLargeurImage = (MapArbre.GetLargeur() - 1) / (float)TerrainX;
	RatioHauteurImage = (MapArbre.GetHauteur() - 1) / (float)TerrainY;

	// On parrcour le tableau d'arbre
	for(int i=0; i<m_NbArbre ; i++)
	{
		// On boucle tant que la postion de l'arbre n'est pas valide
		do
		{
			// On génère une postion aléatoire pour un arbre
			x = (rand() / (float)RAND_MAX) * TerrainX;
			z = (rand() / (float)RAND_MAX) * TerrainY;
			// On test si la probalilité de l'arbre est supérieur à 128 (image en 256 niveaux de gris)
			if(MapArbre.GetPixel((int)(x * RatioLargeurImage), (int)(z * RatioHauteurImage)) > 128)
				bPositionValide = false;
			else
			{
				// Si la postion est valide on test avec la position de tout les autres arbres
				// pour vérifier si il n'est pas tros proche
				bPositionValide = true;
				for(int j=0; j < i; j++)
				{
					DeltaX = fabs(x - m_pArbre[j].x);
					DeltaZ = fabs(z - m_pArbre[j].z);
					// Test si la distance au carré entre les deux arbre est supérieur à 8.0
					if(6.0f > powf(DeltaX, 2) + powf(DeltaZ, 2))
					{
						bPositionValide = false;
						j = i;
					}
				}
			}
		}while(!bPositionValide);
		// On récupère la hauteur du terrain à la position donnée
		m_pArbre[i].y = m_pTerrain->GetHauteur(x, z);
		m_pArbre[i].x = x;
		m_pArbre[i].z = z;
	}
	return true;
}


// Libération des ressources utilisé pour la végétation
void CVegetation::Release()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pTxFleur);
	SAFE_RELEASE(m_pTxPlante);
	SAFE_RELEASE(m_pTxArbre1);
	SAFE_RELEASE(m_pTxArbre2);
	SAFE_RELEASE(m_pTxArbre3);
}

// Rendu de la végétation
void CVegetation::RenduVegetation()
{
	// On active le Vertex Buffer
	m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEXV));
	// On initiallise le type de vertex
	m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEXV);
	// On initialise le type de test sur l'alpha
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// Initialisation des stages de rendu (opération sur les textures)
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	// On récupère la caméra courante
	CCamera* pCamera = m_pTerrain->GetCameraCourante();
	D3DXVECTOR3 Position  = pCamera->GetPosition();
	D3DXVECTOR3 Direction = pCamera->GetCible() - Position;
	float Angle = 0.0f;

	// On calcule l'angle pour orienter la végétation face à la caméra
    if(Direction.x < 0.0f)
        D3DXMatrixRotationY(&m_MatriceGeneral, -atanf(Direction.z/Direction.x)+D3DX_PI/2);
    else
        D3DXMatrixRotationY(&m_MatriceGeneral, -atanf(Direction.z/Direction.x)-D3DX_PI/2);

	// Seuil de transparencre de la texture
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x000000A0);


	if(m_pTxArbre1 != NULL)
	{
		// On active la texture d'un arbre
		m_pD3DDevice->SetTexture(0, m_pTxArbre1);
		// On parcour le tableau avec des arbres de type 1
		for(int i=0; i<m_NbArbre1 ; i++)
		{
			// Calule de l'ange entre l'arbre et la caméras
			Angle = (m_pArbre[i].x-Position.x)*Direction.x+(m_pArbre[i].z-Position.z)*Direction.z;
			// Si Angle > 0.0 alors l'arbre est devant la caméra sinon il est derrière
			if(0.0f < Angle)
			{
				m_MatriceGeneral._41 = m_pArbre[i].x;
				m_MatriceGeneral._42 = m_pArbre[i].y;
				m_MatriceGeneral._43 = m_pArbre[i].z;
				m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceGeneral);
				m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}
	}

	// Même chose avec les arbres de type 2
	if(m_pTxArbre2 != NULL)
	{
		m_pD3DDevice->SetTexture(0, m_pTxArbre2);

		for(int i=m_NbArbre1; i<m_NbArbre1+m_NbArbre2 ; i++)
		{
			Angle = (m_pArbre[i].x-Position.x)*Direction.x+(m_pArbre[i].z-Position.z)*Direction.z;
			if(0.0f < Angle)
			{
				m_MatriceGeneral._41 = m_pArbre[i].x;
				m_MatriceGeneral._42 = m_pArbre[i].y;
				m_MatriceGeneral._43 = m_pArbre[i].z;
				m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceGeneral);
				m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}
	}

	// Même chose avec les arbres de type 3
	if(m_pTxArbre3 != NULL)
	{
		m_pD3DDevice->SetTexture(0, m_pTxArbre3);
		for(int i=m_NbArbre1+m_NbArbre2; i<m_NbArbre ; i++)
		{
			Angle = (m_pArbre[i].x-Position.x)*Direction.x+(m_pArbre[i].z-Position.z)*Direction.z;
			if(0.0f < Angle)
			{	m_MatriceGeneral._41 = m_pArbre[i].x;
				m_MatriceGeneral._42 = m_pArbre[i].y;
				m_MatriceGeneral._43 = m_pArbre[i].z;
				m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceGeneral);
				m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}
	}

	// Même chose avec les plantes
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000040);
	if(m_pTxPlante != NULL)
	{
		m_pD3DDevice->SetTexture(0, m_pTxPlante);
		for(int i=0; i<m_NbPlante ; i++)
		{
			Angle = (m_pPlante[i].x-Position.x)*Direction.x+(m_pPlante[i].z-Position.z)*Direction.z;
			if(0.0f < Angle)
			{
				m_MatriceGeneral._41 = m_pPlante[i].x;
				m_MatriceGeneral._42 = m_pPlante[i].y;
				m_MatriceGeneral._43 = m_pPlante[i].z;
				m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceGeneral);
				m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
			}
		}
	}


	// Même chose avec les fleurs
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000080);
	if(m_pTxPlante != NULL)
	{
		m_pD3DDevice->SetTexture(0, m_pTxFleur);
		for(int i=0; i<m_NbFleur; i++)
		{
			Angle = (m_pFleur[i].x-Position.x)*Direction.x+(m_pFleur[i].z-Position.z)*Direction.z;
			if(0.0f < Angle)
			{
				m_MatriceGeneral._41 = m_pFleur[i].x;
				m_MatriceGeneral._42 = m_pFleur[i].y;
				m_MatriceGeneral._43 = m_pFleur[i].z;
				m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceGeneral);
				m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
			}
		}
	}

	// On remet les paramètres dans l'état initiale
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3DXMatrixIdentity(&m_MatriceGeneral);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceGeneral);
}

// Charger les données de la végétation dans la mémoire video
bool CVegetation::ChargerVegetation()
{
	VOID* pVertex = NULL;
	// Création du vertex buffer
	m_pD3DDevice->CreateVertexBuffer(m_NbVertex*sizeof(CUSTOMVERTEXV), 0, D3DFVF_CUSTOMVERTEXV, D3DPOOL_DEFAULT, &m_pVB, NULL);
	// On de bloqué un buffer pour transférer nos vertex vers la mémoire video
	m_pVB->Lock(0, m_NbVertex*sizeof(CUSTOMVERTEXV), (void**)&pVertex, 0);
	// Copie des vertex dans la mémoire video	
	memcpy(pVertex, m_pVertex, m_NbVertex*sizeof(CUSTOMVERTEXV));
	// On libère le buffer bloqué
	m_pVB->Unlock();

	// Chargement des textures
	if(m_pFichierFleur != NULL)
		D3DXCreateTextureFromFile(m_pD3DDevice, m_pFichierFleur,  &m_pTxFleur);
	if(m_pFichierPlante != NULL)
		D3DXCreateTextureFromFile(m_pD3DDevice, m_pFichierPlante, &m_pTxPlante);
	if(m_pFichierArbreType1 != NULL)
		D3DXCreateTextureFromFile(m_pD3DDevice, m_pFichierArbreType1, &m_pTxArbre1);
	if(m_pFichierArbreType2 != NULL)
		D3DXCreateTextureFromFile(m_pD3DDevice, m_pFichierArbreType2, &m_pTxArbre2);
	if(m_pFichierArbreType3 != NULL)
		D3DXCreateTextureFromFile(m_pD3DDevice, m_pFichierArbreType3, &m_pTxArbre3);

	return true;
}