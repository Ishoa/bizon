#include "CameraTrackBall.h"

CCameraTrackBall::CCameraTrackBall():CCamera()
{
	Console << "Constructeur CCameraTrackBall()" << endl;
}

CCameraTrackBall::CCameraTrackBall(CScene* pScene):CCamera(pScene)
{
	
}

CCameraTrackBall::~CCameraTrackBall()
{
	Console << "Deconstructeur CCameraTrackBall()" << endl;
}


void CCameraTrackBall::Animation(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick, float DeltaTemps)
{

	// Variation de l'angle de la cam�ras en fonction de la souris
	// Regard en haut ou en bas
	m_AngleX -= pEtatSouris->lX * 0.002f;
	// Regard � droite ou � gauche
	m_AngleY -= pEtatSouris->lY * 0.002f;


	// Coef pour r�gler la vitesse de la cam�ra
	float coef = 5.0f * DeltaTemps;

	float distance = sqrt(
		(m_VecPosition.x-m_VecCible.x)*(m_VecPosition.x-m_VecCible.x)
		+ (m_VecPosition.y-m_VecCible.y)*(m_VecPosition.y-m_VecCible.y)
		+ (m_VecPosition.z-m_VecCible.z)*(m_VecPosition.z-m_VecCible.z));
	//Console<<"distance : "<<distance<<endl;


	// Reculer
	if(EtatClavier[DIK_DOWN] & 0x80 || EtatClavier[DIK_S] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lY > 0))
	{
		m_AngleY -= coef;
	}

	// Avancer
	if(EtatClavier[DIK_UP] & 0x80 || EtatClavier[DIK_Z] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lY < 0))
	{
		m_AngleY += coef;
	}

	// D�placement sur la gauche
	if(EtatClavier[DIK_LEFT] & 0x80 || EtatClavier[DIK_Q] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lX < 0))
	{
		m_AngleX -= coef;
	}

	// D�placement sur la droite
	if(EtatClavier[DIK_RIGHT] & 0x80 || EtatClavier[DIK_D] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lX > 0))
	{
		m_AngleX += coef;

	}

	if(m_AngleY > 1.57f) m_AngleY = 1.57f;
	if(m_AngleY < -1.57f) m_AngleY = -1.57f;

	m_VecPosition.x = sinf(m_AngleX) * cos(m_AngleY)* distance;
	m_VecPosition.y = sinf(m_AngleY) * distance;
	m_VecPosition.z = -cosf(m_AngleX)* cos(m_AngleY) * distance;

	
	// d�placement du vecteur position en fonction du clavier

	if(EtatClavier[DIK_PRIOR] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[4] & 0x80))
	{
		m_VecPosition *=  (1-coef) ;
	}

	if(EtatClavier[DIK_NEXT] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->rgbButtons[6] & 0x80))
	{
		m_VecPosition *=  (1+coef) ;
	}


	if(pEtatSouris->lZ < 0)
	{
		m_VecPosition *=  (1+10*coef) ;

	}

	if(pEtatSouris->lZ > 0)
	{

		m_VecPosition *=  (1-10*coef) ;

	}


    D3DXVECTOR4 Pos(m_VecPosition.x,m_VecPosition.y,m_VecPosition.z,1.0f);
	D3DXVECTOR4 Cible(m_VecCible.x,m_VecCible.y,m_VecCible.z,1.0f);
	D3DXVECTOR4 resTmpPos;
	D3DXVECTOR4 resTmpCible;

	D3DXVec4Transform(&resTmpPos, &Pos, &m_WorldMatrice);
	D3DXVec4Transform(&resTmpCible, &Cible, &m_WorldMatrice);

	D3DXVECTOR3 resPos(resTmpPos.x,resTmpPos.y,resTmpPos.z);
	D3DXVECTOR3 resCible(resTmpCible.x,resTmpCible.y,resTmpCible.z);
	
	//Console<<" cilbe :"<<resCible.x<<" // "<<resCible.y<<" // "<<resCible.z<<endl;
	//Console<<" position :"<<resPos.x<<" // "<<resPos.y<<" // "<<resPos.z<<endl;

	// Cr�ation de la matrice de vision en fonction des vecteurs
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
						&resPos,
						&resCible,
						&m_VecOrientation 
						);

	// On applique la matrice
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision );
}