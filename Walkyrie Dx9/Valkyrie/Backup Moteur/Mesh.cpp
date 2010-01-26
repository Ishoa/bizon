#include "Mesh.h"
#include "Calculsmaths.h"

/*
CMesh::CMesh()
{
	m_pMesh = NULL;
	m_pMeshMaterielles = NULL;
	m_pMeshTextures = NULL;
	m_NumMaterielles = 0;


	m_fRX = 0.0f;
	m_fRY = 0.0f;
	m_fRZ = 0.0f;

	m_fTX = 0.0f;
	m_fTY = 0.0f;
	m_fTZ = 0.0f;





}*/


CMesh::CMesh(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pMesh = NULL;
	m_pMeshMaterielles = NULL;
	m_pMeshTextures = NULL;
	m_NumMaterielles = 0;


	DXEtats = NULL;

	m_fRX = 0.0f;
	m_fRY = 0.0f;
	m_fRZ = 0.0f;

	m_fTX = 0.0f;
	m_fTY = 0.0f;
	m_fTZ = 0.0f;

	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
	m_fScaleZ = 1.0f;

	m_bIsAlphaBlended   = false;

	m_pFilename = NULL;

	/*

	D3DXMATRIXA16 Identite;
	D3DXMatrixIdentity(&Identite);
	SetWorldMatrix(&Identite);
	*/
}


CMesh::~CMesh()
{
}

// Cette fonction definit la matrice d'affichage locale pour l'objet.
void CMesh::SetWorldMatrix(D3DXMATRIXA16* ObjectMatrix)
{

/*
	CMaths::CalculMatrixTransformationToXYZ(ObjectMatrix, 
		&m_fRX, &m_fRY,&m_fRZ,
		&m_fTX, &m_fTY,&m_fTZ);
	
*/

	D3DXVECTOR3 vScale;
	D3DXQUATERNION qRotation;
	D3DXVECTOR3 vTranslation;

	D3DXMatrixDecompose( &vScale, &qRotation, &vTranslation,ObjectMatrix);
	//D3DXQuaternionRotationMatrix( &qRotation, ObjectMatrix);

	float q0 = qRotation.x;
	float q1 = qRotation.y;
	float q2 = qRotation.z;
	float q3 = qRotation.w;

	m_fRX = atan((2*((q0*q1)+(q2*q3)))/(1-2*((q1*q1)+(q2*q2))));
	m_fRY = asin(2*((q0*q2)-(q3*q1)));
	m_fRZ = atan((2*((q0*q3)+(q1*q2)))/(1-2*((q2*q2)+(q3*q3))));
/*

	m_fTX = ObjectMatrix->_41 ;
	m_fTY = ObjectMatrix->_42 ;
	m_fTZ = ObjectMatrix->_43 ;
*/
	m_fTX = vTranslation.x ;
	m_fTY = vTranslation.y ;
	m_fTZ = vTranslation.z ;

}


// Cette fonction construit la matrice d'affichage locale pour l'objet.
D3DXMATRIXA16 CMesh::GetWorldMatrix()
{
/*
	D3DXMATRIXA16 mRotationMatrix;
	D3DXMATRIXA16 ObjectMatrix;


	D3DXMatrixRotationQuaternion( &mRotationMatrix,&m_qRotation);
	D3DXMatrixIdentity( &ObjectMatrix );

	D3DXMatrixMultiply( &ObjectMatrix, &mRotationMatrix, &ObjectMatrix );

	ObjectMatrix._41 = m_vTranslation.x;
	ObjectMatrix._42 = m_vTranslation.y;
	ObjectMatrix._43 = m_vTranslation.z;

	return ObjectMatrix;
*/

	D3DXMATRIXA16 RX;
	D3DXMATRIXA16 RY;
	D3DXMATRIXA16 RZ;
	D3DXMATRIXA16 ObjectMatrix;

	D3DXMatrixIdentity( &RX );
	D3DXMatrixIdentity( &RY );
	D3DXMatrixIdentity( &RZ );
	D3DXMatrixIdentity( &ObjectMatrix );

	D3DXMatrixRotationX( &RX, m_fRX );
	D3DXMatrixRotationY( &RY, m_fRY );
	D3DXMatrixRotationZ( &RZ, m_fRZ );

	D3DXMatrixMultiply( &ObjectMatrix, &RX, &ObjectMatrix );
	D3DXMatrixMultiply( &ObjectMatrix, &RY, &ObjectMatrix );
	D3DXMatrixMultiply( &ObjectMatrix, &RZ, &ObjectMatrix );

	ObjectMatrix._41 = m_fTX;
	ObjectMatrix._42 = m_fTY;
	ObjectMatrix._43 = m_fTZ;

	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY,m_fScaleZ);

	D3DXMatrixMultiply( &ObjectMatrix, &matScale, &ObjectMatrix );

	return ObjectMatrix;
}

// Chargement d'un Model 3D à d'un fichier *.x
bool CMesh::ChargerMesh(LPSTR NomFichier)
{
	TCHAR        Directory[MAX_PATH];
	LPD3DXBUFFER D3DXMtrlBuffer = NULL;

	// On contrôle qu'un nom de fichier ait bien été défini.

	if ( NomFichier == NULL )
	{
		Console<<" erreur initialisation du Mesh: nom de fichier null"<<NomFichier<<endl;
		return false;
	}
	else
	{
		m_pFilename = NomFichier;

		// Si oui, on recherche si un nom de répertoire est défini dans le nom
		// du fichier, et on copie celui-ci s'il existe.
		TCHAR Buffer[MAX_PATH];

		// On commence par rechercher la longeur du nom du fichier.
		size_t NameLength = lstrlen( m_pFilename );

		// On crée ensuite une boucle qui parcourt tout le nom.
		for ( size_t i = 0; i < NameLength; i++ )
		{
			// On copie le nom du fichier, caractère par caractère, dans un
			// buffer local.
			Buffer[i] = m_pFilename[i];

			// Puis, on teste si le caractère courant est un symbole de
			// séparation de répertoire.
			if ( m_pFilename[i] == '\\' )
			{
				// Si c'est le cas, on copie le buffer local dans la variable
				// Directory, et on ajoute le symbole de fin de chaîne.
				lstrcpyn( Directory, Buffer, MAX_PATH );
				lstrcpyn( Directory + i + 1, "\0", MAX_PATH - i - 1 );
			}
		}
	}

	// On tente de charger le fichier .x en mémoire.
	if( FAILED( D3DXLoadMeshFromX( m_pFilename,
		D3DXMESH_SYSTEMMEM,
		m_pD3DDevice,
		NULL,
		&D3DXMtrlBuffer,
		NULL,
		&m_NumMaterielles,
		&m_pMesh ) ) )
	{
		Console<<"Erreur Chargement Mesh de collision"<<endl;
		return false;
	}

	// On teste si une donnée de matériau existe pour cet objet.
	if ( D3DXMtrlBuffer != NULL && m_NumMaterielles > 0 )
	{
		// Si oui, on extrait les données du matériau contenu dans le fichier.
		D3DXMATERIAL* D3DXMaterials =
			(D3DXMATERIAL*)D3DXMtrlBuffer->GetBufferPointer();

		m_pMeshMaterielles = new D3DMATERIAL9[m_NumMaterielles];

		// On teste si le tableau contenant les matériaux est correctement
		// initialisé en mémoire.
		if ( m_pMeshMaterielles == NULL )
		{
			Console<<"Erreur Chargement Materiel(s) du Mesh de collision"<<endl;
			return false;
		}

		m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_NumMaterielles];

		// On teste si le tableau contenant les textures est correctement
		// initialisé en mémoire.
		if ( m_pMeshTextures == NULL )
		{
			Console<<"Erreur Chargement texture(s) du Mesh de collision"<<endl;
			return false;
		}

		// On passe en revue tous les matériaux existants.
		for( DWORD i = 0; i < m_NumMaterielles; i++ )
		{
			// On copie le matériau présent en mémoire.
			m_pMeshMaterielles[i] = D3DXMaterials[i].MatD3D;

			// Puis, on copie la lumière ambiante depuis la lumière diffuse,
			// car Direct3D ne le fait pas automatiquement.
			m_pMeshMaterielles[i].Ambient = m_pMeshMaterielles[i].Diffuse;

			m_pMeshTextures[i] = NULL;

			// On tente ensuite de créer la texture pour le matériau.
			if( D3DXMaterials[i].pTextureFilename           != NULL &&
				lstrlen( D3DXMaterials[i].pTextureFilename ) > 0 )
			{
				// On doit ajouter le chemin du répertoire où se trouve l'objet
				// au nom du fichier de la texture. On commence par créer une
				// variable, nommée TextureName, qui contiendra le nom complet.
				TCHAR TextureName[MAX_PATH];

				// Puis, on obtient la longeur du nom du répertoire.
				const int DirNameLength = lstrlen( Directory );

				// On copie ensuite le chemin du répertoire dans la variable
				// TextureName, ...
				lstrcpyn( TextureName, Directory, MAX_PATH );

				// ... puis, on copie le nom du fichier de la texture dans la
				// variable TextureName, après le répertoire.
				lstrcpyn( TextureName + DirNameLength,
					D3DXMaterials[i].pTextureFilename,
					MAX_PATH - DirNameLength );

				// On tente ensuite de charger la texture en mémoire.
				if( FAILED( D3DXCreateTextureFromFile
					( m_pD3DDevice,
					TextureName,
					&m_pMeshTextures[i] ) ) )
				{
					// Si le chargement échoue, on initialise le pointeur de
					// texture du matériau courant à NULL, ainsi on peut quand
					// même finir de charger l'objet.
					Console<<"Erreur Création Texture(s) du Mesh de collision"<<endl;
					m_pMeshTextures[i] = NULL;
				}
			}
		}

		// Pour finir, on libère les ressources utilisées par le buffer
		// de matériaux.
		D3DXMtrlBuffer->Release();
	}
	else
	{
		// Si aucun matériau n'est défini dans le fichier, on en crée un
		// par défaut.
		m_NumMaterielles = 1;

		m_pMeshMaterielles = new D3DMATERIAL9[m_NumMaterielles];

		// On teste si le tableau contenant les matériaux est correctement
		// initialisé en mémoire.
		if ( m_pMeshMaterielles == NULL )
		{
			Console<<"Erreur Chargement Materiel(s) du Mesh de collision"<<endl;
			return false;
		}

		m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_NumMaterielles];

		// On teste si le tableau contenant les textures est correctement
		// initialisé en mémoire.
		if ( m_pMeshTextures == NULL )
		{
			Console<<"Erreur Chargement texture(s) du Mesh de collision"<<endl;
			return false;
		}

		memset( m_pMeshMaterielles, 0, sizeof( D3DMATERIAL9 ) );

		// On configure le paramètre ambiant à 1.0f. On laisse les autres à 0.
		m_pMeshMaterielles[0].Ambient.r = 1.0f;
		m_pMeshMaterielles[0].Ambient.g = 1.0f;
		m_pMeshMaterielles[0].Ambient.b = 1.0f;

		// Aucune texture disponible pour ce matériau.
		m_pMeshTextures[0] = NULL;
	}

	DXEtats = new CDXPileEtats(m_pD3DDevice);
	// Enfin, on retourne le message de réussite.
	return true;
}


// Destruction du Model 3D
void CMesh::Release()
{
	// Destruction du tableaux de matérielles
	

	// Destruction des textures
	if(m_pMeshTextures)
	{
		for(DWORD i = 0; i < m_NumMaterielles; i++)
		{
			if(m_pMeshTextures[i]) 
				m_pMeshTextures[i]->Release();
		}
		SAFE_DELETE_ARRAY(m_pMeshTextures);
		SAFE_DELETE_ARRAY(m_pMeshMaterielles);
	}

	// Destruction du Mesh
	SAFE_RELEASE(m_pMesh);
}


// Rendu du Model 3D
void CMesh::RenduMesh(bool SaveState)
{


	// On demande de sauvegarder l'état du Device Direct3D.
	if(SaveState)
		DXEtats->PushDeviceProperties();

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

	// On teste ensuite si le moteur d'alpha blending est actif.
	if ( m_bIsAlphaBlended == true )
	{
		// Si oui, on configure l'alpha blending pour le rendu.
		SetAlphaBlending();
	}

	// On configure ensuite la matrice de monde pour cet objet.
	D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &WorldMatrix );

	// Puis, on s'assure que le sampler pour les textures est configuré
	// correctement pour le rendu de l'objet.
	m_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	m_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

	// On initialise Stage de rendu

	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);


	// On parcours chaque partie du Mesh
	for(DWORD i=0; i<m_NumMaterielles; i++)
	{
		// On active le matérielle et la texture de la partie à dessiner
        m_pD3DDevice->SetMaterial(&m_pMeshMaterielles[i]);
        m_pD3DDevice->SetTexture(0, m_pMeshTextures[i]);
        
        // Dessin de la partie du Mesh
        m_pMesh->DrawSubset(i);
	}

	// Enfin, on restaure l'état d'origine du Device Direct3D.
	if(SaveState)
		DXEtats->PopDeviceProperties();
}


// Définit l'état du moteur d'Aplha Blending.
void CMesh::SetAlphaBlending( bool Value )
{
	m_bIsAlphaBlended = Value;
}


// Retourne l'état du moteur d'aplha Blending.
bool CMesh::IsAlphaBlended()
{
	return m_bIsAlphaBlended;
}


// Cette fonction configure l'affichage pour permettre la transparence.
void CMesh::SetAlphaBlending()
{
	// En définissant la source et la destination de l'alpha blending à ONE,
	// l'objet devient transparent. Attention toutefois, le Z-Buffer est
	// désactivé pour cette opération, l'objet doit donc être rendu à la
	// fin de la scène, éventuellement trié par ordre de profondeur s'il y a
	// plusieurs objets de ce type à dessiner, afin d'obtenir un affichage
	// correct.
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );
}


// Cette fonction permet de restaurer l'objet après un changement de Device.
bool CMesh::RestoreMesh()
{
	Release();

	// Puis, on tente de réinitialiser l'objet.
	return ChargerMesh(m_pFilename);
}

// Obtient l'angle de rotation de l'objet sur l'axe X.
float CMesh::GetRotationX()
{
	return m_fRX;
}

// Définit l'angle de rotation de l'objet sur l'axe X.
void CMesh::SetRotationX( float Value )
{
	if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
	{
		m_fRX = Value;

	}
	else
	{
		m_fRX = 0;
	}
}

// Obtient l'angle de rotation de l'objet sur l'axe Y.
float CMesh::GetRotationY()
{
	return m_fRY;
}

// Définit l'angle de rotation de l'objet sur l'axe Y.
void CMesh::SetRotationY( float Value )
{
	if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
	{
		m_fRY = Value;
	}
	else
	{
		m_fRY = 0;
	}
}

// Obtient l'angle de rotation de l'objet sur l'axe Z.
float CMesh::GetRotationZ()
{
	return m_fRZ;
}

// Définit l'angle de rotation de l'objet sur l'axe Z.
void CMesh::SetRotationZ( float Value )
{
	if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
	{
		m_fRZ = Value;
	}
	else
	{
		m_fRZ = 0;
	}
}

// Définit l'angle de rotation de l'objet sur l'axe X Y Z.
void CMesh::SetRotationXYZ(  float fXValue, float fYValue, float fZValue)
{
	SetRotationX(fXValue);
	SetRotationY(fYValue);
	SetRotationZ(fZValue);
}

// Obtient la valeur de la position de l'objet sur l'axe X.
float CMesh::GetTranslationX()
{
	return m_fTX;
}

// Définit la valeur de la position de l'objet sur l'axe X.
void CMesh::SetTranslationX( float Value )
{
	m_fTX = Value;
}

// Obtient la valeur de la position de l'objet sur l'axe Y.
float CMesh::GetTranslationY()
{
	return m_fTY;
}

// Définit la valeur de la position de l'objet sur l'axe Y.
void CMesh::SetTranslationY( float Value )
{
	m_fTY = Value;
}

// Obtient la valeur de la position de l'objet sur l'axe Z.
float CMesh::GetTranslationZ()
{
	return m_fTZ;
}

// Définit la valeur de la position de l'objet sur l'axe Z.
void CMesh::SetTranslationZ( float Value )
{
	m_fTZ = Value;
}

// Définit la valeur de la position de l'objet sur l'axe X Y Z.
void CMesh::SetTranslationXYZ(  float fXValue, float fYValue, float fZValue)
{
	SetTranslationX(fXValue);
	SetTranslationY(fYValue);
	SetTranslationZ(fZValue);
	
}

float CMesh::GetScaleX()
{
	return m_fScaleX;
}

void  CMesh::SetScaleX    ( float Value )
{
	 m_fScaleX = Value;
}

float CMesh::GetScaleY()
{

	return m_fScaleY;
}

void  CMesh::SetScaleY    ( float Value )
{
	m_fScaleY = Value;
}

float CMesh::GetScaleZ    ()
{
	return m_fScaleZ;
}

void  CMesh::SetScaleZ    ( float Value )
{

	m_fScaleZ = Value;
}

void  CMesh::SetScaleXYZ  ( float fXValue, float fYValue, float fZValue)
{
	SetScaleX(fXValue);
	SetScaleY(fYValue);
	SetScaleZ(fZValue);
}