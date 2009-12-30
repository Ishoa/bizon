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


CMesh::CMesh(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();
	m_pMesh = NULL;
	m_pMeshMaterielles = NULL;
	m_pMeshTextures = NULL;
	m_NumMaterielles = 0;
	
	m_pMeshLowDetail = NULL;
	m_pMeshMateriellesLowDetail = NULL;
	m_NumMateriellesLowDetail = 0;

	m_vMaxMesh =  D3DXVECTOR3 (0.0f, 0.0f,0.0f);
	m_vMinMesh = D3DXVECTOR3 (0.0f, 0.0f,0.0f);

// 	m_pIB = NULL;
// 	m_pVB = NULL;
	m_pDecl = NULL;

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

	//D3DXMatrixDecompose( &vScale, &qRotation, &vTranslation,ObjectMatrix);
	D3DXQuaternionRotationMatrix( &qRotation, ObjectMatrix);

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
	LPD3DXBUFFER pAdjacencyBuffer = NULL;

	// On contrôle qu'un nom de fichier ait bien été défini.

	Release();

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
		&pAdjacencyBuffer,
		&D3DXMtrlBuffer,
		NULL,
		&m_NumMaterielles,
		&m_pMesh ) ) )
	{
		Console<<"Erreur Chargement Mesh de collision"<<endl;
		return false;
	}


	// Optimize the mesh for performance
	if( FAILED(m_pMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		( DWORD* )pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
	{
		SAFE_RELEASE( pAdjacencyBuffer );
		SAFE_RELEASE( D3DXMtrlBuffer );
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

		m_pMeshTextures = new CLPTexture[m_NumMaterielles];

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
				if(!m_pMeshTextures[i].chargerTexture(m_pD3DDevice,TextureName))
				{
					// Si le chargement échoue, on initialise le pointeur de
					// texture du matériau courant à NULL, ainsi on peut quand
					// même finir de charger l'objet.
					Console<<"Erreur Création Texture(s) du Mesh de collision"<<endl;

				}
			}
		}


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

		m_pMeshTextures = new CLPTexture[m_NumMaterielles];

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

	}


	SAFE_RELEASE( pAdjacencyBuffer );
	SAFE_RELEASE( D3DXMtrlBuffer );

	// Extract data from m_pMesh for easy access
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	m_dwNumVertices = m_pMesh->GetNumVertices();
	m_dwNumFaces = m_pMesh->GetNumFaces();
	m_dwBytesPerVertex = m_pMesh->GetNumBytesPerVertex();
// 	m_pMesh->GetIndexBuffer( &m_pIB );
// 	m_pMesh->GetVertexBuffer( &m_pVB );
	m_pMesh->GetDeclaration( decl );
	m_pD3DDevice->CreateVertexDeclaration( decl, &m_pDecl );


	//Calcul Rapide Bounding Sphere

	struct MeshVertex { D3DXVECTOR3 p;};
	MeshVertex *pVertices;
	LPD3DXMESH pMeshTemporaire= NULL;

	if(FAILED(m_pMesh->CloneMeshFVF( D3DXMESH_MANAGED,
		D3DFVF_XYZ,
		m_pD3DDevice, &pMeshTemporaire)))
		return S_FALSE;

	// Lock the geometry buffers
	pMeshTemporaire->LockVertexBuffer( 0L, (LPVOID*)&pVertices );
	int nNBVertices = pMeshTemporaire->GetNumVertices();

	m_vMaxMesh = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vMinMesh = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// Puis, on extrait chaque sommet.
	for ( int i = 0; i < nNBVertices; i++ )
	{
		D3DXVECTOR3 vCurrent = pVertices[i].p;

		m_vMaxMesh = CMaths::GetMaxValue( m_vMaxMesh, vCurrent );
		m_vMinMesh = CMaths::GetMinValue( m_vMinMesh, vCurrent );
	}

	pMeshTemporaire->UnlockVertexBuffer();

	SAFE_RELEASE(pMeshTemporaire);

	DXEtats = new CDXPileEtats(m_pD3DDevice);
	// Enfin, on retourne le message de réussite.
	return true;
}


bool CMesh::ChargerMeshLowDetail(LPSTR NomFichier)
{
	TCHAR        Directory[MAX_PATH];
	LPD3DXBUFFER D3DXMtrlBuffer = NULL;


	// On contrôle qu'un nom de fichier ait bien été défini.

	if ( NomFichier == NULL )
	{
		Console<<" erreur initialisation du Mesh faible detail: nom de fichier null"<<NomFichier<<endl;
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
		D3DXMESH_MANAGED,
		m_pD3DDevice,
		NULL,
		&D3DXMtrlBuffer,
		NULL,
		&m_NumMateriellesLowDetail,
		&m_pMeshLowDetail ) ) )
	{
		Console<<"Erreur Chargement Mesh de collision"<<endl;
		return false;
	}




	// On teste si une donnée de matériau existe pour cet objet.
	if ( D3DXMtrlBuffer != NULL && m_NumMateriellesLowDetail > 0 )
	{
		// Si oui, on extrait les données du matériau contenu dans le fichier.
		D3DXMATERIAL* D3DXMaterials =
			(D3DXMATERIAL*)D3DXMtrlBuffer->GetBufferPointer();

		m_pMeshMateriellesLowDetail = new D3DMATERIAL9[m_NumMateriellesLowDetail];

		// On teste si le tableau contenant les matériaux est correctement
		// initialisé en mémoire.
		if ( m_pMeshMateriellesLowDetail == NULL )
		{
			Console<<"Erreur Chargement Materiel(s) du Mesh de collision"<<endl;
			return false;
		}


		// On passe en revue tous les matériaux existants.
		for( DWORD i = 0; i < m_NumMateriellesLowDetail; i++ )
		{
			// On copie le matériau présent en mémoire.
			m_pMeshMateriellesLowDetail[i] = D3DXMaterials[i].MatD3D;

			// Puis, on copie la lumière ambiante depuis la lumière diffuse,
			// car Direct3D ne le fait pas automatiquement.
			m_pMeshMateriellesLowDetail[i].Ambient = m_pMeshMaterielles[i].Diffuse;

		}

		// Pour finir, on libère les ressources utilisées par le buffer
		// de matériaux.
		D3DXMtrlBuffer->Release();
	}
	else
	{
		// Si aucun matériau n'est défini dans le fichier, on en crée un
		// par défaut.
		m_NumMateriellesLowDetail = 1;

		m_pMeshMateriellesLowDetail = new D3DMATERIAL9[m_NumMateriellesLowDetail];

		// On teste si le tableau contenant les matériaux est correctement
		// initialisé en mémoire.
		if ( m_pMeshMateriellesLowDetail == NULL )
		{
			Console<<"Erreur Chargement Materiel(s) du Mesh de collision"<<endl;
			return false;
		}


		memset( m_pMeshMateriellesLowDetail, 0, sizeof( D3DMATERIAL9 ) );

		// On configure le paramètre ambiant à 1.0f. On laisse les autres à 0.
		m_pMeshMateriellesLowDetail[0].Ambient.r = 1.0f;
		m_pMeshMateriellesLowDetail[0].Ambient.g = 1.0f;
		m_pMeshMateriellesLowDetail[0].Ambient.b = 1.0f;

	}

	// Enfin, on retourne le message de réussite.
	return true;
}


bool CMesh::ChargerMeshLowDetail(LPD3DXMESH pMeshLow,D3DMATERIAL9* pMeshMateriellesLowDetail,DWORD NumMateriellesLowDetail)
{
	return false;
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
				m_pMeshTextures[i].Release();
		}
	}

	SAFE_DELETE_ARRAY(m_pMeshTextures);
	SAFE_DELETE_ARRAY(m_pMeshMaterielles);
	SAFE_DELETE_ARRAY(m_pMeshMateriellesLowDetail)

	// Destruction du Mesh
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMeshLowDetail);

// 	SAFE_RELEASE( m_pIB );
// 	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pDecl );

	m_NumMaterielles = 0;
}


void CMesh::SimpleRenduMesh(bool textured)
{
	// On parcours chaque partie du Mesh
	for(DWORD i=0; i<m_NumMaterielles; i++)
	{
		// On active le matérielle et la texture de la partie à dessiner
		m_pD3DDevice->SetMaterial(&m_pMeshMaterielles[i]);
		if(textured)
			m_pD3DDevice->SetTexture(0, m_pMeshTextures[i].getDXTexture());

		// Dessin de la partie du Mesh
		m_pMesh->DrawSubset(i);
	}
}

// Rendu du Model 3D
void CMesh::RenduMeshSampler(bool SaveState)
{


	if(m_pMesh != NULL 
		&& BoundingBoxIsVisibleByCamera()
		//&& BoundingSphereIsVisibleByCamera()
		)
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

		D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
		// On configure ensuite la matrice de monde pour cet objet.
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
			m_pD3DDevice->SetTexture(0, m_pMeshTextures[i].getDXTexture());

			// Dessin de la partie du Mesh
			m_pMesh->DrawSubset(i);
		}

		// Enfin, on restaure l'état d'origine du Device Direct3D.
		if(SaveState)
			DXEtats->PopDeviceProperties();
	}
 	//else
 	//	Console<<"Mesh \""<<m_pFilename<<"\" éléminé par FRUSTRUM CULLING"<<endl;

}

void CMesh::RenduMeshLowDetail(bool SaveState)
{


	if(m_pMeshLowDetail != NULL)
	{

		if(BoundingSphereIsVisibleByCamera())
		{
			// On demande de sauvegarder l'état du Device Direct3D.
			if(SaveState)
				DXEtats->PushDeviceProperties();

			m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

			m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
			m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

			// On configure ensuite la matrice de monde pour cet objet.
			D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
			m_pD3DDevice->SetTransform( D3DTS_WORLD, &WorldMatrix );

			// On parcours chaque partie du Mesh
			for(DWORD i=0; i<m_NumMateriellesLowDetail; i++)
			{
				// On active le matérielle et la texture de la partie à dessiner
				m_pD3DDevice->SetMaterial(&m_pMeshMateriellesLowDetail[i]);
				m_pD3DDevice->SetTexture(0,NULL);

				// Dessin de la partie du Mesh
				m_pMeshLowDetail->DrawSubset(i);
			}

			// Enfin, on restaure l'état d'origine du Device Direct3D.
			if(SaveState)
				DXEtats->PopDeviceProperties();
		}
	}
	else
	{
		RenduMeshSampler(SaveState);
	}
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


//-----------------------------------------------------------------------------
// Convert the mesh to the format specified by the given vertex declarations.
//-----------------------------------------------------------------------------
HRESULT CMesh::SetVertexDecl( const D3DVERTEXELEMENT9* pDecl,
							  bool bAutoComputeNormals, bool bAutoComputeTangents,
							  bool bSplitVertexForOptimalTangents )
{
	LPD3DXMESH pTempMesh = NULL;

	if( m_pMesh )
	{
		if( FAILED( m_pMesh->CloneMesh( m_pMesh->GetOptions(), pDecl,
			m_pD3DDevice, &pTempMesh ) ) )
		{
			SAFE_RELEASE( pTempMesh );
			return E_FAIL;
		}
	}


	// Check if the old declaration contains a normal.
	bool bHadNormal = false;
	bool bHadTangent = false;
	D3DVERTEXELEMENT9 aOldDecl[MAX_FVF_DECL_SIZE];
	if( m_pMesh && SUCCEEDED( m_pMesh->GetDeclaration( aOldDecl ) ) )
	{
		for( UINT index = 0; index < D3DXGetDeclLength( aOldDecl ); ++index )
		{
			if( aOldDecl[index].Usage == D3DDECLUSAGE_NORMAL )
			{
				bHadNormal = true;
			}
			if( aOldDecl[index].Usage == D3DDECLUSAGE_TANGENT )
			{
				bHadTangent = true;
			}
		}
	}

	// Check if the new declaration contains a normal.
	bool bHaveNormalNow = false;
	bool bHaveTangentNow = false;
	D3DVERTEXELEMENT9 aNewDecl[MAX_FVF_DECL_SIZE];
	if( pTempMesh && SUCCEEDED( pTempMesh->GetDeclaration( aNewDecl ) ) )
	{
		for( UINT index = 0; index < D3DXGetDeclLength( aNewDecl ); ++index )
		{
			if( aNewDecl[index].Usage == D3DDECLUSAGE_NORMAL )
			{
				bHaveNormalNow = true;
			}
			if( aNewDecl[index].Usage == D3DDECLUSAGE_TANGENT )
			{
				bHaveTangentNow = true;
			}
		}
	}

	SAFE_RELEASE( m_pMesh );

	if( pTempMesh )
	{
		m_pMesh = pTempMesh;

		if( !bHadNormal && bHaveNormalNow && bAutoComputeNormals )
		{
			// Compute normals in case the meshes have them
			D3DXComputeNormals( m_pMesh, NULL );
		}

		if( bHaveNormalNow && !bHadTangent && bHaveTangentNow && bAutoComputeTangents )
		{
			ID3DXMesh* pNewMesh;
			HRESULT hr;

			DWORD* rgdwAdjacency = NULL;
			rgdwAdjacency = new DWORD[m_pMesh->GetNumFaces() * 3];
			if( rgdwAdjacency == NULL )
				return E_OUTOFMEMORY;
			if(FAILED( m_pMesh->GenerateAdjacency( 1e-6f, rgdwAdjacency ) ))
				return S_FALSE;

			float fPartialEdgeThreshold;
			float fSingularPointThreshold;
			float fNormalEdgeThreshold;
			if( bSplitVertexForOptimalTangents )
			{
				fPartialEdgeThreshold = 0.01f;
				fSingularPointThreshold = 0.25f;
				fNormalEdgeThreshold = 0.01f;
			}
			else
			{
				fPartialEdgeThreshold = -1.01f;
				fSingularPointThreshold = 0.01f;
				fNormalEdgeThreshold = -1.01f;
			}

			// Compute tangents, which are required for normal mapping
			hr = D3DXComputeTangentFrameEx( m_pMesh,
				D3DDECLUSAGE_TEXCOORD, 0,
				D3DDECLUSAGE_TANGENT, 0,
				D3DX_DEFAULT, 0,
				D3DDECLUSAGE_NORMAL, 0,
				0, rgdwAdjacency,
				fPartialEdgeThreshold, fSingularPointThreshold, fNormalEdgeThreshold,
				&pNewMesh, NULL );

			SAFE_DELETE_ARRAY( rgdwAdjacency );
			if( FAILED( hr ) )
				return hr;

			SAFE_RELEASE( m_pMesh );
			m_pMesh = pNewMesh;
		}
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
HRESULT CMesh::SetFVF(DWORD dwFVF )
{
	LPD3DXMESH pTempMesh = NULL;

	if( m_pMesh != NULL)
	{
		if( FAILED( m_pMesh->CloneMeshFVF( m_pMesh->GetOptions(), dwFVF,
			m_pD3DDevice, &pTempMesh ) ) )
		{
			SAFE_RELEASE( pTempMesh );
			return E_FAIL;
		}

		DWORD dwOldFVF = 0;
		dwOldFVF = m_pMesh->GetFVF();
		SAFE_RELEASE( m_pMesh );
		m_pMesh = pTempMesh;

		// Compute normals if they are being requested and
		// the old mesh does not have them.
		if( !( dwOldFVF & D3DFVF_NORMAL ) && dwFVF & D3DFVF_NORMAL )
		{
			D3DXComputeNormals( m_pMesh, NULL );
		}
	}

	return S_OK;
}



//-----------------------------------------------------------------------------
void CMesh::RenduMesh( bool bDrawOpaqueSubsets,
					   bool bDrawAlphaSubsets )
{

	D3DXVECTOR4 vCurrentMin (m_vMinMesh.x,m_vMinMesh.y,m_vMinMesh.z,1.0f);
	D3DXVECTOR4 vCurrentMax (m_vMaxMesh.x,m_vMaxMesh.y,m_vMaxMesh.z,1.0f);

	D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();

	D3DXVec4Transform(&vCurrentMin, &vCurrentMin, &WorldMatrix );
	D3DXVec4Transform(&vCurrentMax, &vCurrentMax, &WorldMatrix );

	// On calcule, puis on inscrit le centre de la sphère.
	D3DXVECTOR3 vPositionBoundingSphere = D3DXVECTOR3( ( vCurrentMin.x + vCurrentMax.x ) / 2.0f,
		( vCurrentMin.y + vCurrentMax.y ) / 2.0f,
		( vCurrentMin.z + vCurrentMax.z ) / 2.0f ) ;

	// Ensuite, on calcule le rayon de la sphère.
	D3DXVECTOR3 vRadiusVector = D3DXVECTOR3( ( vCurrentMax.x - vCurrentMin.x ) / 2.0f,
		( vCurrentMax.y - vCurrentMin.y ) / 2.0f,
		( vCurrentMax.z - vCurrentMin.z ) / 2.0f );

	float fRayonBoundingSphere = D3DXVec3Length( &vRadiusVector );

	if(m_pMesh != NULL && m_pScene->GetCameraCourante()->IsVisible(&vPositionBoundingSphere,fRayonBoundingSphere))
	{
		// Frist, draw the subsets without alpha
		if( bDrawOpaqueSubsets )
		{
			for( DWORD i = 0; i < m_NumMaterielles; i++ )
			{
				if( m_bUseMaterials )
				{
					if( m_pMeshMaterielles[i].Diffuse.a < 1.0f )
						continue;
					m_pD3DDevice->SetMaterial( &m_pMeshMaterielles[i] );
					m_pD3DDevice->SetTexture( 0, m_pMeshTextures[i].getDXTexture() );
				}
				m_pMesh->DrawSubset( i );
			}
		}

		// Then, draw the subsets with alpha
		if( bDrawAlphaSubsets && m_bUseMaterials )
		{
			for( DWORD i = 0; i < m_NumMaterielles; i++ )
			{
				if( m_pMeshMaterielles[i].Diffuse.a == 1.0f )
					continue;

				// Set the material and texture
				m_pD3DDevice->SetMaterial( &m_pMeshMaterielles[i] );
				m_pD3DDevice->SetTexture( 0, m_pMeshTextures[i].getDXTexture() );
				m_pMesh->DrawSubset( i );
			}
		}

	}

}




//-----------------------------------------------------------------------------
void CMesh::RenduMesh( ID3DXEffect* pEffect,
							   D3DXHANDLE hTexture,
							   D3DXHANDLE hDiffuse,
							   D3DXHANDLE hAmbient,
							   D3DXHANDLE hSpecular,
							   D3DXHANDLE hEmissive,
							   D3DXHANDLE hPower,
							   bool bDrawOpaqueSubsets,
							   bool bDrawAlphaSubsets )
{

	DXEtats->PushDeviceProperties();

	D3DXVECTOR4 vCurrentMin (m_vMinMesh.x,m_vMinMesh.y,m_vMinMesh.z,1.0f);
	D3DXVECTOR4 vCurrentMax (m_vMaxMesh.x,m_vMaxMesh.y,m_vMaxMesh.z,1.0f);

	D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();

	D3DXVec4Transform(&vCurrentMin, &vCurrentMin, &WorldMatrix );
	D3DXVec4Transform(&vCurrentMax, &vCurrentMax, &WorldMatrix );

	// On calcule, puis on inscrit le centre de la sphère.
	D3DXVECTOR3 vPositionBoundingSphere = D3DXVECTOR3( ( vCurrentMin.x + vCurrentMax.x ) / 2.0f,
		( vCurrentMin.y + vCurrentMax.y ) / 2.0f,
		( vCurrentMin.z + vCurrentMax.z ) / 2.0f ) ;

	// Ensuite, on calcule le rayon de la sphère.
	D3DXVECTOR3 vRadiusVector = D3DXVECTOR3( ( vCurrentMax.x - vCurrentMin.x ) / 2.0f,
		( vCurrentMax.y - vCurrentMin.y ) / 2.0f,
		( vCurrentMax.z - vCurrentMin.z ) / 2.0f );

	float fRayonBoundingSphere = D3DXVec3Length( &vRadiusVector );

	if(m_pMesh != NULL && m_pScene->GetCameraCourante()->IsVisible(&vPositionBoundingSphere,fRayonBoundingSphere))
	{

		UINT cPasses;
		// Frist, draw the subsets without alpha
		if( bDrawOpaqueSubsets )
		{
			pEffect->Begin( &cPasses, 0 );
			for( UINT p = 0; p < cPasses; ++p )
			{
				pEffect->BeginPass( p );
				for( DWORD i = 0; i < m_NumMaterielles; i++ )
				{
					if( m_bUseMaterials )
					{
						if( m_pMeshMaterielles[i].Diffuse.a < 1.0f )
							continue;
						if( hTexture )
							pEffect->SetTexture( hTexture, m_pMeshTextures[i].getDXTexture() );
						// D3DCOLORVALUE and D3DXVECTOR4 are data-wise identical.
						// No conversion is needed.
						if( hDiffuse )
							pEffect->SetVector( hDiffuse, ( D3DXVECTOR4* )&m_pMeshMaterielles[i].Diffuse );
						if( hAmbient )
							pEffect->SetVector( hAmbient, ( D3DXVECTOR4* )&m_pMeshMaterielles[i].Ambient );
						if( hSpecular )
							pEffect->SetVector( hSpecular, ( D3DXVECTOR4* )&m_pMeshMaterielles[i].Specular );
						if( hEmissive )
							pEffect->SetVector( hEmissive, ( D3DXVECTOR4* )&m_pMeshMaterielles[i].Emissive );
						if( hPower )
							pEffect->SetFloat( hPower, m_pMeshMaterielles[i].Power );
						pEffect->CommitChanges();
					}
					m_pMesh->DrawSubset( i );
				}
				pEffect->EndPass();
			}
			pEffect->End();
		}

		// Then, draw the subsets with alpha
		if( bDrawAlphaSubsets && m_bUseMaterials )
		{
			pEffect->Begin( &cPasses, 0 );
			for( UINT p = 0; p < cPasses; ++p )
			{
				pEffect->BeginPass( p );
				for( DWORD i = 0; i < m_NumMaterielles; i++ )
				{
					if( m_bUseMaterials )
					{
						if( m_pMeshMaterielles[i].Diffuse.a == 1.0f )
							continue;
						if( hTexture )
							pEffect->SetTexture( hTexture, m_pMeshTextures[i].getDXTexture() );
						// D3DCOLORVALUE and D3DXVECTOR4 are data-wise identical.
						// No conversion is needed.
						if( hDiffuse )
							pEffect->SetVector( hDiffuse, ( D3DXVECTOR4* )&m_pMeshMaterielles[i].Diffuse );
						if( hAmbient )
							pEffect->SetVector( hAmbient, ( D3DXVECTOR4* )&m_pMeshMaterielles[i].Ambient );
						if( hSpecular )
							pEffect->SetVector( hSpecular, ( D3DXVECTOR4* )&m_pMeshMaterielles[i].Specular );
						if( hEmissive )
							pEffect->SetVector( hEmissive, ( D3DXVECTOR4* )&m_pMeshMaterielles[i].Emissive );
						if( hPower )
							pEffect->SetFloat( hPower, m_pMeshMaterielles[i].Power );
						pEffect->CommitChanges();
					}
					m_pMesh->DrawSubset( i );
				}
				pEffect->EndPass();
			}
			pEffect->End();
		}
	}

	DXEtats->PopDeviceProperties();
}


bool CMesh::BoundingBoxIsVisibleByCamera()
{
	D3DXVECTOR4 vCurrentMin (m_vMinMesh.x,m_vMinMesh.y,m_vMinMesh.z,1.0f);
	D3DXVECTOR4 vCurrentMax (m_vMaxMesh.x,m_vMaxMesh.y,m_vMaxMesh.z,1.0f);

	D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();

	D3DXVec4Transform(&vCurrentMin, &vCurrentMin, &WorldMatrix );
	D3DXVec4Transform(&vCurrentMax, &vCurrentMax, &WorldMatrix );

	// On calcule, puis on inscrit le centre de la sphère.
	D3DXVECTOR3 vPositionBoundingSphere = D3DXVECTOR3( ( vCurrentMin.x + vCurrentMax.x ) / 2.0f,
		( vCurrentMin.y + vCurrentMax.y ) / 2.0f,
		( vCurrentMin.z + vCurrentMax.z ) / 2.0f ) ;

	// Ensuite, on calcule le rayon de la sphère.
	D3DXVECTOR3 vRadiusVector = D3DXVECTOR3( ( vCurrentMax.x - vCurrentMin.x ) / 2.0f,
		( vCurrentMax.y - vCurrentMin.y ) / 2.0f,
		( vCurrentMax.z - vCurrentMin.z ) / 2.0f );

	float fRayonBoundingSphere = D3DXVec3Length( &vRadiusVector );

	return m_pScene->GetCameraCourante()->IsVisible(&vCurrentMin,&vCurrentMax);
}

bool CMesh::BoundingSphereIsVisibleByCamera()
{
	D3DXVECTOR4 vCurrentMin (m_vMinMesh.x,m_vMinMesh.y,m_vMinMesh.z,1.0f);
	D3DXVECTOR4 vCurrentMax (m_vMaxMesh.x,m_vMaxMesh.y,m_vMaxMesh.z,1.0f);

	D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();

	D3DXVec4Transform(&vCurrentMin, &vCurrentMin, &WorldMatrix );
	D3DXVec4Transform(&vCurrentMax, &vCurrentMax, &WorldMatrix );

	// On calcule, puis on inscrit le centre de la sphère.
	D3DXVECTOR3 vPositionBoundingSphere = D3DXVECTOR3( ( vCurrentMin.x + vCurrentMax.x ) / 2.0f,
		( vCurrentMin.y + vCurrentMax.y ) / 2.0f,
		( vCurrentMin.z + vCurrentMax.z ) / 2.0f ) ;

	// Ensuite, on calcule le rayon de la sphère.
	D3DXVECTOR3 vRadiusVector = D3DXVECTOR3( ( vCurrentMax.x - vCurrentMin.x ) / 2.0f,
		( vCurrentMax.y - vCurrentMin.y ) / 2.0f,
		( vCurrentMax.z - vCurrentMin.z ) / 2.0f );

	float fRayonBoundingSphere = D3DXVec3Length( &vRadiusVector );

	return m_pScene->GetCameraCourante()->IsVisible(&vPositionBoundingSphere,fRayonBoundingSphere);
}