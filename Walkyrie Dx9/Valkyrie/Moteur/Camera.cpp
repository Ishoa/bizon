#include "Camera.h"

CCamera::CCamera()
{
	Console << "Constructeur CCamera()" << endl;
	m_bMouvementUtilisateur = false;
}


CCamera::CCamera(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();
	m_AngleX = 0.0f;
	m_AngleY = 0.0f;
	m_Ratio = 0.0f;
	m_fvitesse = 100.0f;

	m_fZNear = 1.0f;
	m_fZFar = 1000.f;

	m_bMouvementUtilisateur = false;
	D3DXMatrixIdentity(&m_WorldMatrice);
}

CCamera::~CCamera()
{
}

// Création de la caméra (valeur par defaut)
void CCamera::Creation()
{
	m_Ratio = m_pScene->GetRatioFenetre();
	// On crée la matrice de projetction
	// angle d'ouverture de la caméra : 45°
	// plan le proche : 0.1f
	// plan le plus loin : 1000.0f
	D3DXMatrixPerspectiveFovLH(&m_MatriceTransProj, D3DX_PI*0.25f, m_Ratio, m_fZNear, m_fZFar);
	// On applique la matrice
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_MatriceTransProj);

	m_AngleX = D3DX_PI/2.0f;
	m_AngleY = 0.0f;

	// Initialisation des vecteurs
	m_VecPosition = D3DXVECTOR3(0.0f, 10.0f, -20.0f); 
	m_VecCible.x = m_VecPosition.x + cosf(m_AngleY) * cosf(m_AngleX);
	m_VecCible.y = m_VecPosition.y + sinf(m_AngleY);
	m_VecCible.z = m_VecPosition.z + cosf(m_AngleY) * sinf(m_AngleX);
	m_VecOrientation = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


	
    D3DXVECTOR4 Pos(m_VecPosition.x,m_VecPosition.y,m_VecPosition.z,1.0f);
	D3DXVECTOR4 Cible(m_VecCible.x,m_VecCible.y,m_VecCible.z,1.0f);
	D3DXVECTOR4 resTmpPos;
	D3DXVECTOR4 resTmpCible;

	D3DXVec4Transform(&resTmpPos, &Pos, &m_WorldMatrice);
	D3DXVec4Transform(&resTmpCible, &Cible, &m_WorldMatrice);

	D3DXVECTOR3 resPos(resTmpPos.x,resTmpPos.y,resTmpPos.z);
	D3DXVECTOR3 resCible(resTmpCible.x,resTmpCible.y,resTmpCible.z);

	// Création de la matrice de vision 
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
		                &resPos,
                        &resCible,
                        &m_VecOrientation 
						);	
	// On applique la matrice
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision);	
}

// Création de la caméra
void CCamera::Creation(D3DXVECTOR3* VecPosition, D3DXVECTOR3* VecCible, D3DXVECTOR3* VecOrientation)
{
	// Initialisation des vecteurs
	m_VecPosition = *VecPosition;
	m_VecCible = *VecCible;
	m_VecOrientation = *VecOrientation;

	m_Ratio = m_pScene->GetRatioFenetre();
	// On crée la matrice de projetction
	// angle d'ouverture de la caméra : 45°
	// plan le proche : 0.1f
	// plan le plus loin : 1000.0f
	D3DXMatrixPerspectiveFovLH(&m_MatriceTransProj, D3DX_PI*0.25f, m_Ratio, m_fZNear, m_fZFar);
	// On applique la matrice
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_MatriceTransProj);

	D3DXVECTOR4 Pos(m_VecPosition.x,m_VecPosition.y,m_VecPosition.z,1.0f);
	D3DXVECTOR4 Cible(m_VecCible.x,m_VecCible.y,m_VecCible.z,1.0f);
	D3DXVECTOR4 resTmpPos;
	D3DXVECTOR4 resTmpCible;

	D3DXVec4Transform(&resTmpPos, &Pos, &m_WorldMatrice);
	D3DXVec4Transform(&resTmpCible, &Cible, &m_WorldMatrice);

	D3DXVECTOR3 resPos(resTmpPos.x,resTmpPos.y,resTmpPos.z);
	D3DXVECTOR3 resCible(resTmpCible.x,resTmpCible.y,resTmpCible.z);

	// Création de la matrice de vision 
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
		                &resPos,
                        &resCible,
                        &m_VecOrientation 
						);	
	// On applique la matrice
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision);	
}

void CCamera::SetPosition(D3DXVECTOR3* VecPosition)
{
	m_VecPosition = *VecPosition;
	D3DXVECTOR3 cible(m_VecPosition.x - m_VecCible.x,
		m_VecPosition.y - m_VecCible.y,
		m_VecPosition.z - m_VecCible.z);

	D3DXVECTOR3 ciblex(cible.x,0.0f,cible.z);

	D3DXVec3Normalize( &cible, &cible );
	D3DXVec3Normalize( &ciblex, &ciblex );

	float mx = D3DXVec3Dot( &D3DXVECTOR3(-1.0f,0.0f,0.0f) ,&ciblex);

	float my = D3DXVec3Dot( &cible,&ciblex);

	if(cible.z<0)	
		m_AngleX = acosf(mx);
	else
		m_AngleX = - acosf(mx);

	if(cible.y<0)
		m_AngleY = acosf(my);
	else
		m_AngleY = -acosf(my);

}



void CCamera::SetCible(D3DXVECTOR3* VecCible)
{
	m_VecCible = *VecCible;
	D3DXVECTOR3 cible(m_VecPosition.x - m_VecCible.x,
					  m_VecPosition.y - m_VecCible.y,
					  m_VecPosition.z - m_VecCible.z);

	D3DXVECTOR3 ciblex(cible.x,0.0f,cible.z);

	D3DXVec3Normalize( &cible, &cible );
	D3DXVec3Normalize( &ciblex, &ciblex );

	float mx = D3DXVec3Dot( &D3DXVECTOR3(-1.0f,0.0f,0.0f) ,&ciblex);

	float my = D3DXVec3Dot( &cible,&ciblex);

	if(cible.z<0)	
 		m_AngleX = acosf(mx);
	else
		m_AngleX = - acosf(mx);

	if(cible.y<0)
		m_AngleY = acosf(my);
	else
		m_AngleY = -acosf(my);

}

// Animation de la caméras en fonction des déplacement du clavier et de la souris 
void CCamera::Animation(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick, float DeltaTemps)
{
	// Variation de l'angle de la caméras en fonction de la souris
	// Regard en haut ou en bas
	m_AngleX -= pEtatSouris->lX * 0.002f;
	// Regard à droite ou à gauche
	m_AngleY -= pEtatSouris->lY * 0.002f;

	//AngleY : ]-PI/2 , PI/2[
	if(m_AngleY > 1.56f) m_AngleY = 1.56f;
	if(m_AngleY < -1.56f) m_AngleY = -1.56f;

	// Coef pour régler la vitesse de la caméra
	float coef = m_fvitesse * DeltaTemps;

	// déplacement du vecteur position en fonction du clavier
	// Reculer
	if(EtatClavier[DIK_DOWN] & 0x80)
	{
		m_VecPosition.x -= cosf(m_AngleY) * cosf(m_AngleX) * coef;
		m_VecPosition.y -= sinf(m_AngleY) * coef;
		m_VecPosition.z -= cosf(m_AngleY) * sinf(m_AngleX) * coef;
	}
	// Avancer
	if(EtatClavier[DIK_UP] & 0x80)
	{
		m_VecPosition.x += cosf(m_AngleY) * cosf(m_AngleX) * coef;
		m_VecPosition.y += sinf(m_AngleY) * coef;
		m_VecPosition.z += cosf(m_AngleY) * sinf(m_AngleX) * coef;
	}
	// Déplacement sur la gauche
	if(EtatClavier[DIK_LEFT] & 0x80)
	{
		m_VecPosition.x -= sinf(m_AngleX) * coef;
		m_VecPosition.z += cosf(m_AngleX) * coef;
	}
	// Déplacement sur la droite
	if(EtatClavier[DIK_RIGHT] & 0x80)
	{
		m_VecPosition.x += sinf(m_AngleX) * coef;
		m_VecPosition.z -= cosf(m_AngleX) * coef;
	}

	// Calcul du vecteur cible en fonction des angles X et Y
	m_VecCible.x = m_VecPosition.x + cosf(m_AngleY) * cosf(m_AngleX);
	m_VecCible.y = m_VecPosition.y + sinf(m_AngleY);
	m_VecCible.z = m_VecPosition.z + cosf(m_AngleY) * sinf(m_AngleX);

	D3DXVECTOR4 Pos(m_VecPosition.x,m_VecPosition.y,m_VecPosition.z,1.0f);
	D3DXVECTOR4 Cible(m_VecCible.x,m_VecCible.y,m_VecCible.z,1.0f);
	D3DXVECTOR4 resTmpPos;
	D3DXVECTOR4 resTmpCible;

	D3DXVec4Transform(&resTmpPos, &Pos, &m_WorldMatrice);
	D3DXVec4Transform(&resTmpCible, &Cible, &m_WorldMatrice);

	D3DXVECTOR3 resPos(resTmpPos.x,resTmpPos.y,resTmpPos.z);
	D3DXVECTOR3 resCible(resTmpCible.x,resTmpCible.y,resTmpCible.z);

	// Création de la matrice de vision en fonction des vecteurs
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
						&resPos,
						&resCible,
						&m_VecOrientation 
						);
	// On applique la matrice
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision );

	// Création des plans de projection
	SetPlanProjection();
}

void CCamera::setWorldMatrice(D3DXMATRIX* mat)
{
	m_WorldMatrice = *mat;

}


// Applique les matrices de transformation
void CCamera::SetMatrice()
{
	D3DXVECTOR4 Pos(m_VecPosition.x,m_VecPosition.y,m_VecPosition.z,1.0f);
	D3DXVECTOR4 Cible(m_VecCible.x,m_VecCible.y,m_VecCible.z,1.0f);
	D3DXVECTOR4 resTmpPos;
	D3DXVECTOR4 resTmpCible;

	D3DXVec4Transform(&resTmpPos, &Pos, &m_WorldMatrice);
	D3DXVec4Transform(&resTmpCible, &Cible, &m_WorldMatrice);

	D3DXVECTOR3 resPos(resTmpPos.x,resTmpPos.y,resTmpPos.z);
	D3DXVECTOR3 resCible(resTmpCible.x,resTmpCible.y,resTmpCible.z);


	// Création de la matrice de vision 
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
						&resPos,
						&resCible,
						&m_VecOrientation 
						);
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision );

	m_Ratio = m_pScene->GetRatioFenetre();

	// Création de la matrice de projetction
	D3DXMatrixPerspectiveFovLH(&m_MatriceTransProj, D3DX_PI*0.25f, m_Ratio, m_fZNear, m_fZFar);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_MatriceTransProj);

	// Création des plans de projection
	SetPlanProjection();
}

// Applique les matrices de transformation pour le rendu d'une texture cube mapping
void CCamera::SetMatriceCubeMap(DWORD nFace)
{
    switch(nFace)
    {
        case D3DCUBEMAP_FACE_POSITIVE_X:
            m_VecCible = D3DXVECTOR3(1.0f + m_VecPosition.x, m_VecPosition.y, m_VecPosition.z);
            m_VecOrientation = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_X:
            m_VecCible = D3DXVECTOR3(-1.0f + m_VecPosition.x, m_VecPosition.y, m_VecPosition.z);
            m_VecOrientation = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Y:
            m_VecCible = D3DXVECTOR3(m_VecPosition.x, 1.0f + m_VecPosition.y, m_VecPosition.z);
            m_VecOrientation = D3DXVECTOR3(0.0f, 0.0f,-1.0f);
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Y:
            m_VecCible = D3DXVECTOR3(m_VecPosition.x, -1.0f + m_VecPosition.y, m_VecPosition.z);
            m_VecOrientation = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Z:
            m_VecCible = D3DXVECTOR3(m_VecPosition.x, m_VecPosition.y, 1.0f + m_VecPosition.z);
            m_VecOrientation = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Z:
            m_VecCible = D3DXVECTOR3(m_VecPosition.x, m_VecPosition.y, -1.0f + m_VecPosition.z);
            m_VecOrientation = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
            break;
    }

	D3DXVECTOR4 Pos(m_VecPosition.x,m_VecPosition.y,m_VecPosition.z,1.0f);
	D3DXVECTOR4 Cible(m_VecCible.x,m_VecCible.y,m_VecCible.z,1.0f);
	D3DXVECTOR4 resTmpPos;
	D3DXVECTOR4 resTmpCible;

	D3DXVec4Transform(&resTmpPos, &Pos, &m_WorldMatrice);
	D3DXVec4Transform(&resTmpCible, &Cible, &m_WorldMatrice);

	D3DXVECTOR3 resPos(resTmpPos.x,resTmpPos.y,resTmpPos.z);
	D3DXVECTOR3 resCible(resTmpCible.x,resTmpCible.y,resTmpCible.z);

    // Création de la matrice de vision 
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
						&resPos,
						&resCible,
						&m_VecOrientation 
						);
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision );

	m_Ratio = m_pScene->GetRatioFenetre();

	// Création de la matrice de projetction
	D3DXMatrixPerspectiveFovLH(&m_MatriceTransProj, D3DX_PI*0.5f, 1.0f, 1.0f, 2500.0f);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_MatriceTransProj);

	// Création des plans de projection
	SetPlanProjection();
}

// Applique les matrices de transformation pour le rendu d'une texture sphère mapping
void CCamera::SetMatriceSphereMap()
{
	D3DXVECTOR4 Pos(m_VecPosition.x,m_VecPosition.y,m_VecPosition.z,1.0f);
	D3DXVECTOR4 Cible(m_VecCible.x,m_VecCible.y,m_VecCible.z,1.0f);
	D3DXVECTOR4 resTmpPos;
	D3DXVECTOR4 resTmpCible;

	D3DXVec4Transform(&resTmpPos, &Pos, &m_WorldMatrice);
	D3DXVec4Transform(&resTmpCible, &Cible, &m_WorldMatrice);

	D3DXVECTOR3 resPos(resTmpPos.x,resTmpPos.y,resTmpPos.z);
	D3DXVECTOR3 resCible(resTmpCible.x,resTmpCible.y,resTmpCible.z);

    // Création de la matrice de vision 
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
						&resPos,
						&resCible,
						&m_VecOrientation 
						);
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision );

	// Création de la matrice de projetction
	D3DXMatrixPerspectiveFovLH(&m_MatriceTransProj, D3DX_PI*0.025f, 1.0f, 0.1f, 100.0f);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_MatriceTransProj);
}

// Applique les matrices de transformation pour le rendu d'une texture, pour un miroir
void CCamera::SetMatriceMiroir(D3DXMATRIXA16* MatriceReflection)
{
	
	D3DXVECTOR4 Pos(m_VecPosition.x,m_VecPosition.y,m_VecPosition.z,1.0f);
	D3DXVECTOR4 Cible(m_VecCible.x,m_VecCible.y,m_VecCible.z,1.0f);
	D3DXVECTOR4 resTmpPos;
	D3DXVECTOR4 resTmpCible;

	D3DXVec4Transform(&resTmpPos, &Pos, &m_WorldMatrice);
	D3DXVec4Transform(&resTmpCible, &Cible, &m_WorldMatrice);

	D3DXVECTOR3 resPos(resTmpPos.x,resTmpPos.y,resTmpPos.z);
	D3DXVECTOR3 resCible(resTmpCible.x,resTmpCible.y,resTmpCible.z);

    // Création de la matrice de vision 
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
						&resPos,
						&resCible,
						&m_VecOrientation 
						);

	// On transforme la matirce de vision
	D3DXMatrixMultiply(&m_MatriceTransVision, MatriceReflection, &m_MatriceTransVision);
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision );

	m_Ratio = m_pScene->GetRatioFenetre();

	// Création de la matrice de projetction
	D3DXMatrixPerspectiveFovLH(&m_MatriceTransProj, D3DX_PI*0.25f, m_Ratio, m_fZNear, m_fZFar);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_MatriceTransProj);

	// Création des plans de projection
	SetPlanProjection();
}

// Calcule de la matrice de projection pour projeter une texture
D3DXMATRIXA16 CCamera::GetMatriceProjectionTexture()
{
	/*D3DXMATRIXA16 scale;
	D3DXMATRIXA16 offset;
	D3DXMATRIXA16 mat;
	D3DXMATRIXA16 matProj;

	m_Ratio = m_pScene->GetRatioFenetre();
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI*0.25f, m_Ratio, 0.0f, 1.0f);
	D3DXMatrixScaling(&scale, 0.5f, -0.5f, 1.0f);
	D3DXMatrixTranslation(&offset, 1.0f, 1.0f, 0.0f);
	
	D3DXMatrixMultiply(&mat, &matProj, &offset);
	D3DXMatrixMultiply(&mat, &mat, &scale);

	return mat;*/

	m_Ratio = m_pScene->GetRatioFenetre();

	D3DXMATRIXA16 Matrice;
	float yScale = -0.5f / tan(D3DX_PI*0.25f*0.5f);
	float xScale = -yScale / m_Ratio;

	Matrice._11 = xScale; Matrice._12 =   0.0f; Matrice._13 = 0.0f; Matrice._14 = 0.0f;
	Matrice._21 =   0.0f; Matrice._22 = yScale; Matrice._23 = 0.0f; Matrice._24 = 0.0f;
	Matrice._31 =   0.5f; Matrice._32 =   0.5f; Matrice._33 = 1.0f; Matrice._34 = 1.0f;
	Matrice._41 =   0.0f; Matrice._42 =   0.0f; Matrice._43 = 0.0f; Matrice._44 = 0.0f;
	return Matrice;
}

// Calcule de la matrice de transformation pour l'affichage d'une texture cubique
D3DXMATRIXA16 CCamera::GetMatriceTextureCube()
{
	D3DXMATRIXA16 Matrice;
	Matrice = m_MatriceTransVision;
	Matrice._41 = 0.0f;
	Matrice._42 = 0.0f;
	Matrice._43 = 0.0f;
	D3DXMatrixInverse(&Matrice, NULL, &Matrice);
	return Matrice;
}

// Calcule des plans de la pyramide de projection
void CCamera::SetPlanProjection()
{
	D3DXMATRIXA16 ViewProjection = m_MatriceTransVision * m_MatriceTransProj;
            
    // plan de gauche
    m_PlanProjection[0].a = ViewProjection._14 + ViewProjection._11;
    m_PlanProjection[0].b = ViewProjection._24 + ViewProjection._21;
    m_PlanProjection[0].c = ViewProjection._34 + ViewProjection._31;
    m_PlanProjection[0].d = ViewProjection._44 + ViewProjection._41;

    // plan de droite
    m_PlanProjection[1].a = ViewProjection._14 - ViewProjection._11;   
    m_PlanProjection[1].b = ViewProjection._24 - ViewProjection._21;
    m_PlanProjection[1].c = ViewProjection._34 - ViewProjection._31;
    m_PlanProjection[1].d = ViewProjection._44 - ViewProjection._41;

    // plan du haut
    m_PlanProjection[2].a = ViewProjection._14 - ViewProjection._12;   
    m_PlanProjection[2].b = ViewProjection._24 - ViewProjection._22;
    m_PlanProjection[2].c = ViewProjection._34 - ViewProjection._32;
    m_PlanProjection[2].d = ViewProjection._44 - ViewProjection._42;

    // plan du bas
    m_PlanProjection[3].a = ViewProjection._14 + ViewProjection._12;   
    m_PlanProjection[3].b = ViewProjection._24 + ViewProjection._22;
    m_PlanProjection[3].c = ViewProjection._34 + ViewProjection._32;
    m_PlanProjection[3].d = ViewProjection._44 + ViewProjection._42;


	D3DXVECTOR3 vDirZNear(m_VecCible.x - m_VecPosition.x,m_VecCible.y - m_VecPosition.y,m_VecCible.z -m_VecPosition.z);
	D3DXVec3Normalize(&vDirZNear,&vDirZNear);
	D3DXVECTOR3 m_PosZNear = m_VecPosition +(vDirZNear*m_fZNear);

	float D = -(m_PosZNear.x * vDirZNear.x + m_PosZNear.y * vDirZNear.y + m_PosZNear.z * vDirZNear.z);

	// Premier plan
	m_PlanProjection[4].a = vDirZNear.x;   
	m_PlanProjection[4].b = vDirZNear.y;
	m_PlanProjection[4].c = vDirZNear.z;
	m_PlanProjection[4].d = D;

	D3DXVECTOR3 m_PosZFar = m_VecPosition +(vDirZNear*m_fZFar);

	D = -(m_PosZFar.x * -vDirZNear.x + m_PosZFar.y * -vDirZNear.y + m_PosZFar.z * -vDirZNear.z);

	// Arriere plan
	m_PlanProjection[5].a = -vDirZNear.x;   
	m_PlanProjection[5].b = -vDirZNear.y;
	m_PlanProjection[5].c = -vDirZNear.z;
	m_PlanProjection[5].d = D;


	// Normalisation des plans
    for ( int i = 0; i < 6; i++ )
    {
		D3DXPlaneNormalize(&m_PlanProjection[i], &m_PlanProjection[i]);
    }
}

// Retorune si la sphère est dans la pyramide de projection
bool CCamera::IsVisible(D3DXVECTOR3* Position, float Rayon)
{
	for(int i = 0 ; i < 6 ; i++)
	{
		// Calcule de la position de la sphère par rapport a un plan
		float fDist = D3DXPlaneDotCoord(&m_PlanProjection[i], Position) + Rayon;
		if (fDist < 0.0f)
            return false;
	}
	return true;
}


// Retorune si la sphère est dans la pyramide de projection
bool CCamera::IsVisible(D3DXVECTOR4* Min, D3DXVECTOR4* Max)
{

	D3DXVECTOR3 vCorner[8];

	vCorner[0] = D3DXVECTOR3(Min->x,Min->y,Min->z);
	vCorner[1] = D3DXVECTOR3(Max->x,Min->y,Min->z);
	vCorner[2] = D3DXVECTOR3(Max->x,Max->y,Min->z);
	vCorner[3] = D3DXVECTOR3(Min->x,Max->y,Min->z);
	vCorner[4] = D3DXVECTOR3(Min->x,Min->y,Max->z);
	vCorner[5] = D3DXVECTOR3(Max->x,Min->y,Max->z);
	vCorner[6] = D3DXVECTOR3(Max->x,Max->y,Max->z);
	vCorner[7] = D3DXVECTOR3(Min->x,Max->y,Max->z);

	int iTotalIn = 0;

	for(int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for(int i = 0; i < 8; ++i) {

			// test this point against the planes
			float fDist = D3DXPlaneDotCoord(&m_PlanProjection[p], &vCorner[i]);
			if ( fDist < 0.0f) 
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if(iInCount == 0)
			return false;

		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}

	
	// so if iTotalIn is 6, then all are inside the view
	if(iTotalIn == 6)
		return true;

	// we must be partly in then otherwise
	return true;
}