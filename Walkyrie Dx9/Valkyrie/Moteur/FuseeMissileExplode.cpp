#include "FuseeMissileExplode.h"
#include "Camera.h"
#include "Calculsmaths.h"

CFuseeMissileExplode::CFuseeMissileExplode(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();
	m_pParticleSystemFusee = new CParticleSystem(m_pD3DDevice);

	m_vPositionDepart = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_fDistanceExplode = 100.0f;
	m_fVitessefusee = 80.0f;

	m_bGauche = false;
}


CFuseeMissileExplode::CFuseeMissileExplode()
{
	delete m_pParticleSystemFusee;
}


bool CFuseeMissileExplode::CreateAndInit(char **g_pTextNameFusee , int nTailleFusee,char **g_pTextNameExplode , int nTailleExplode)
{

	if(nTailleFusee > 1)
		m_pParticleSystemFusee->SetMultiTexture(g_pTextNameFusee,nTailleFusee);
	else
		m_pParticleSystemFusee->SetTexture(g_pTextNameFusee[0]);

	m_pParticleSystemFusee->SetMaxParticles( 1000 );
	m_pParticleSystemFusee->SetNumToRelease( 5 );
	m_pParticleSystemFusee->SetReleaseInterval( 0.005f );
	m_pParticleSystemFusee->SetLifeCycle( 0.8f );
	m_pParticleSystemFusee->SetSize( 0.3f );
	m_pParticleSystemFusee->SetColor( D3DXCOLOR( 0.3f, 0.3f, 0.3f, 1.0f ));
	m_pParticleSystemFusee->SetPosition( m_vPositionDepart );

	m_pParticleSystemFusee->SetVelocity( D3DXVECTOR3( 0.0f, -5.0f, 0.0f ) );
	m_pParticleSystemFusee->SetGravity( D3DXVECTOR3( 0.0f,0.0f, 0.0f ) );
	m_pParticleSystemFusee->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_pParticleSystemFusee->SetVelocityVar( 1.0f );

	m_pParticleSystemFusee->SetCollisionPlane( D3DXVECTOR3( 0.0f, 1.0f, 0.0f ), 
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),CR_BOUNCE );

	if(FAILED(m_pParticleSystemFusee->Init()))
		return false;

	e_EtatFuseeExplode = e_Lancement;

	return true;
}


void CFuseeMissileExplode::DestructionObjet()
{
	m_pParticleSystemFusee->Release();
}


void CFuseeMissileExplode::Rendu3D()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );

	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	switch(e_EtatFuseeExplode)
	{
		case e_Lancement:
			break;
		case e_Fusee:
			m_pParticleSystemFusee->Rendu3D();
			break;
		case e_Wait :
			break;
		case e_Explode:
			break;
		case e_Finish:
			break;
	}
}


void CFuseeMissileExplode::Animation(float fDeltaTemps, bool bTir)
{
	switch(e_EtatFuseeExplode)
	{
		case e_Lancement:
			{	
				if(bTir)
				{
					
					D3DXVECTOR3 vPosCamera = m_pScene->GetCameraCourante()->GetPosition();
					D3DXVECTOR3 vPosTarget = m_pScene->GetCameraCourante()->GetCible();
					
					m_vDir = vPosTarget - vPosCamera;
					m_vPositionDepart = vPosCamera;

					m_pParticleSystemFusee->SetPosition(m_vPositionDepart);
					m_pParticleSystemFusee->RestartParticleSystem();
					e_EtatFuseeExplode = e_Fusee;
				}
			}
			break;

		case e_Fusee:
			{
				if(CMaths::GetDistance(m_pParticleSystemFusee->GetPosition(), m_vPositionDepart) > m_fDistanceExplode)
				{
					e_EtatFuseeExplode = e_Explode;
				}
				else
				{
					D3DXVECTOR3 vNewPos = m_pParticleSystemFusee->GetPosition();
					vNewPos += m_vDir * fDeltaTemps * m_fVitessefusee ; 

					m_pParticleSystemFusee->SetPosition(vNewPos);
				}

				m_pParticleSystemFusee->Update(fDeltaTemps);
			}
			break;

		case e_Wait:
			{
		
			}
			break;

		case e_Explode:
			{
				e_EtatFuseeExplode = e_Lancement;
			}

			break;

		case e_Finish:
			{

			}
			break;
	}	
}
