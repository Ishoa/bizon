#include "CameraPremierePersonneTerrain.h"

#define VARIATION 0.013f
#define AMPLITUDEVARIATION 0.5f


CCameraPremierePersonneTerrain::CCameraPremierePersonneTerrain():CCameraPremierePersonne()
{
	m_pEnvCollision = NULL;
	m_pTerrain = NULL;
}


CCameraPremierePersonneTerrain::CCameraPremierePersonneTerrain(CScene* pScene,CTerrain* pTerrain,CEnvCollision* pEnvCollision, float fHauteur):CCameraPremierePersonne(pScene,fHauteur)
{
	m_pEnvCollision = pEnvCollision;
	m_pTerrain = pTerrain;
}

CCameraPremierePersonneTerrain::~CCameraPremierePersonneTerrain()
{

}




void CCameraPremierePersonneTerrain::Animation(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick, float DeltaTemps)
{

	D3DXVECTOR3 NextPosition = m_VecPosition;
	float fVariation = m_fVariation;

	// Variation de l'angle de la caméras en fonction de la souris
	// Regard en haut ou en bas
	m_AngleX -= pEtatSouris->lX * 0.002f;
	// Regard à droite ou à gauche
	m_AngleY -= pEtatSouris->lY * 0.002f;

	//AngleY : ]-PI/2 , PI/2[
	if(m_AngleY > 1.57f) m_AngleY = 1.57f;
	if(m_AngleY < -1.57f) m_AngleY = -1.57f;

	// Coef pour régler la vitesse de la caméra
	float coef = m_fvitesse * DeltaTemps;

	//Console<<fVariation<<endl;

	// déplacement du vecteur position en fonction du clavier
	// Reculer
	if(EtatClavier[DIK_DOWN] & 0x80 || EtatClavier[DIK_S] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lY > 0))
	{
		NextPosition.x -= cosf(m_AngleY) * cosf(m_AngleX) * coef;
		NextPosition.z -= cosf(m_AngleY) * sinf(m_AngleX) * coef;
		fVariation-=VARIATION;
		if(fVariation<0.0f)
			fVariation = - fVariation;
	}	
	// Avancer
	if(EtatClavier[DIK_UP] & 0x80 || EtatClavier[DIK_W] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lY < 0))
	{
		NextPosition.x += cosf(m_AngleY) * cosf(m_AngleX) * coef;
		NextPosition.z += cosf(m_AngleY) * sinf(m_AngleX) * coef;
		fVariation+=VARIATION;
		if(fVariation>3.14f)
			fVariation = (fVariation - 3.14f);

	}
	// Déplacement sur la gauche
	if(EtatClavier[DIK_LEFT] & 0x80 || EtatClavier[DIK_A] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lX < 0))
	{
		NextPosition.x -=  sinf(m_AngleX) * coef;
		NextPosition.z += cosf(m_AngleX) * coef;
	}
	// Déplacement sur la droite
	if(EtatClavier[DIK_RIGHT] & 0x80 || EtatClavier[DIK_D] & 0x80 || (pEtatJoystick!=NULL && pEtatJoystick->lX > 0))
	{
		NextPosition.x += sinf(m_AngleX) * coef;
		NextPosition.z -= cosf(m_AngleX) * coef;
	}

	NextPosition = CheckPosition(m_VecPosition,NextPosition);
	// Calcul du vecteur cible en fonction des angles X et Y

	if(m_VecPosition != NextPosition)
	{
		if(m_pTerrain != NULL)
		{
			NextPosition.y = m_pTerrain->GetHauteur(NextPosition.x, NextPosition.z)+ m_fHauteur + (sin(fVariation)*AMPLITUDEVARIATION);
		}
		m_fVariation = fVariation;
	}

	m_VecPosition = NextPosition;

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


D3DXVECTOR3 CCameraPremierePersonneTerrain::CheckPosition( D3DXVECTOR3 CurrentPosition,D3DXVECTOR3 NextPosition )
{
	if(m_pEnvCollision != NULL)
		return m_pEnvCollision->CheckNextPosition(CurrentPosition,NextPosition);
	else
		return NextPosition;
}