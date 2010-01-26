#include "CameraVolLibre.h"

CCameraVolLibre::CCameraVolLibre():CCamera()
{
	Console << "Constructeur CCameraVolLibre()" << endl;
}

CCameraVolLibre::CCameraVolLibre(CScene* pScene):CCamera(pScene)
{

}

CCameraVolLibre::~CCameraVolLibre()
{
	Console << "Deconstructeur CCameraVolLibre()" << endl;
}

void CCameraVolLibre::Animation(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick, float DeltaTemps)
{

	// Variation de l'angle de la caméras en fonction de la souris
	// Regard en haut ou en bas
	m_AngleX -= pEtatSouris->lX * 0.002f;
	// Regard à droite ou à gauche
	m_AngleY -= pEtatSouris->lY * 0.002f;

	//AngleY : ]-PI/2 , PI/2[
	if(m_AngleY > 1.57f) m_AngleY = 1.57f;
	if(m_AngleY < -1.57f) m_AngleY = -1.57f;

	// Coef pour régler la vitesse de la caméra
	float coef = 20.0f * DeltaTemps;

	// déplacement du vecteur position en fonction du clavier
	// Reculer
	if(EtatClavier[DIK_DOWN] & 0x80 || EtatClavier[DIK_S] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lY > 0))
	{
		m_VecPosition.x -= cosf(m_AngleY) * cosf(m_AngleX) * coef;
		m_VecPosition.y -= sinf(m_AngleY) * coef;
		m_VecPosition.z -= cosf(m_AngleY) * sinf(m_AngleX) * coef;
	}
	// Avancer
	if(EtatClavier[DIK_UP] & 0x80 || EtatClavier[DIK_W] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lY < 0))
	{
		m_VecPosition.x += cosf(m_AngleY) * cosf(m_AngleX) * coef;
		m_VecPosition.y += sinf(m_AngleY) * coef;
		m_VecPosition.z += cosf(m_AngleY) * sinf(m_AngleX) * coef;
	}
	// Déplacement sur la gauche
	if(EtatClavier[DIK_LEFT] & 0x80 || EtatClavier[DIK_A] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lX < 0))
	{
		m_VecPosition.x -= sinf(m_AngleX) * coef;
		m_VecPosition.z += cosf(m_AngleX) * coef;
	}
	// Déplacement sur la droite
	if(EtatClavier[DIK_RIGHT] & 0x80 || EtatClavier[DIK_D] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lX > 0))
	{
		m_VecPosition.x += sinf(m_AngleX) * coef;
		m_VecPosition.z -= cosf(m_AngleX) * coef;
	}



	// Calcul du vecteur cible en fonction des angles X et Y
	m_VecCible.x = m_VecPosition.x + cosf(m_AngleY) * cosf(m_AngleX);
	m_VecCible.y = m_VecPosition.y + sinf(m_AngleY);
	m_VecCible.z = m_VecPosition.z + cosf(m_AngleY) * sinf(m_AngleX);

	// Création de la matrice de vision en fonction des vecteurs
	D3DXMatrixLookAtLH( &m_MatriceTransVision, 
						&m_VecPosition,
						&m_VecCible,
						&m_VecOrientation 
						);
	// On applique la matrice
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceTransVision );
}