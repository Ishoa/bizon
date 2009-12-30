#include "CameraPremierePersonneLaby.h"

CCameraPremierePersonneLaby::CCameraPremierePersonneLaby():CCameraPremierePersonne()
{
	m_pEnvCollision = NULL;
}


CCameraPremierePersonneLaby::CCameraPremierePersonneLaby(CScene* pScene,CEnvCollision* pEnvCollision, float fHauteur):CCameraPremierePersonne(pScene,fHauteur)
{
	m_pEnvCollision = pEnvCollision;
}

CCameraPremierePersonneLaby::~CCameraPremierePersonneLaby()
{

}

D3DXVECTOR3 CCameraPremierePersonneLaby::CheckPosition( D3DXVECTOR3 CurrentPosition,D3DXVECTOR3 NextPosition )
{
	if(m_pEnvCollision != NULL)
		return m_pEnvCollision->CheckNextPosition(CurrentPosition,NextPosition);
	else
		return NextPosition;
}