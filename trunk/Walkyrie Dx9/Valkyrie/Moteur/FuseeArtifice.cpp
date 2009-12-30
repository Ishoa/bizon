#include "FuseeArtifice.h"
#include "Calculsmaths.h"


CFuseeArtifice::CFuseeArtifice(LPDIRECT3DDEVICE9 pd3dDevice)
{

	m_pD3DDevice = pd3dDevice;
	m_pParticleSystemFusee = new CParticleSystem(m_pD3DDevice);
	m_pParticleSystemExplode = new CParticleSystem(m_pD3DDevice);

	m_vPositionDepart = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_fDistanceExplode = 30.0f;
	m_vDir = D3DXVECTOR3(0.0f,1.0f,0.0f);
	m_fVitessefusee = 25.0f;
	m_fTimeAfterExplode = 0.1f;
	m_bBoucle = true;
	m_fCurrentTime = 0.0f;
}

CFuseeArtifice::~CFuseeArtifice()
{

	delete m_pParticleSystemFusee;
	delete m_pParticleSystemExplode;
}

bool CFuseeArtifice::CreateAndInit(char **g_pTextNameFusee , int nTailleFusee,char **g_pTextNameExplode , int nTailleExplode)
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
	m_pParticleSystemFusee->SetColor( D3DXCOLOR( 1.0f, 1.0f, 0.3f, 1.0f ));
	m_pParticleSystemFusee->SetPosition( m_vPositionDepart );

	m_pParticleSystemFusee->SetVelocity( D3DXVECTOR3( 0.0f, -5.0f, 0.0f ) );
	m_pParticleSystemFusee->SetGravity( D3DXVECTOR3( 0.0f,-9.8f, 0.0f ) );
	m_pParticleSystemFusee->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_pParticleSystemFusee->SetVelocityVar( 1.0f );

	if(FAILED(m_pParticleSystemFusee->Init()))
		return false;


	if(nTailleExplode > 1)
		m_pParticleSystemExplode->SetMultiTexture(g_pTextNameExplode,nTailleExplode);
	else
		m_pParticleSystemExplode->SetTexture(g_pTextNameExplode[0]);

	m_pParticleSystemExplode->SetMaxParticles( 100 );
	m_pParticleSystemExplode->SetNumToRelease( 100 );
	m_pParticleSystemExplode->SetReleaseInterval( 0.05f );
	m_pParticleSystemExplode->SetLifeCycle( 4.0f );
	m_pParticleSystemExplode->SetSize( 1.0f );
	m_pParticleSystemExplode->SetColor( D3DXCOLOR( 1.0f, 1.0f, 0.3f, 1.0f ));

	D3DXVECTOR3 vExplode = m_pParticleSystemFusee->GetPosition();
	m_pParticleSystemExplode->SetPosition( D3DXVECTOR3( vExplode.x, vExplode.y + m_fDistanceExplode, vExplode.z) );

	m_pParticleSystemExplode->SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pParticleSystemExplode->SetGravity( D3DXVECTOR3( 0.0f, -3.0f, 0.0f) );
	m_pParticleSystemExplode->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pParticleSystemExplode->SetVelocityVar( 10.0f );

	if(FAILED(m_pParticleSystemExplode->Init()))
		return false;

	m_pParticleSystemFusee->RestartParticleSystem();
	m_pParticleSystemExplode->RestartParticleSystem();

	m_nSigne = 1;
	m_fCurrentTime = 0.0f;
	e_EtatFuseeArtifice = e_Lancement;

	return true;
}

void CFuseeArtifice::DestructionObjet()
{
	m_pParticleSystemFusee->Release();
	m_pParticleSystemExplode->Release();
	
}


void CFuseeArtifice::Rendu3D()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );

	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	switch(e_EtatFuseeArtifice)
	{
		case e_Lancement:
			break;
		case e_Fusee:
				m_pParticleSystemFusee->Rendu3D();
			break;
		case e_Wait :
			break;
		case e_Explode:
				m_pParticleSystemExplode->Rendu3D();
			break;
		case e_Finish:
			break;

	}

}


void CFuseeArtifice::Animation(float fDeltaTemps)
{
	switch(e_EtatFuseeArtifice)
	{
		case e_Lancement:
			{
				m_fCurrentTime+=fDeltaTemps;
					if(m_fCurrentTime >= m_fTimeAvantLancement)
					{
						m_fCurrentTime = 0.0f;
						e_EtatFuseeArtifice = e_Fusee;
					}
			}
			break;

		case e_Fusee:

			if(CMaths::GetDistance(m_pParticleSystemFusee->GetPosition(), m_vPositionDepart) > m_fDistanceExplode)
			{
				e_EtatFuseeArtifice = e_Wait;
				m_fCurrentTime = 0.0f;
			}
			else
			{
				D3DXVECTOR3 vNewPos = m_pParticleSystemFusee->GetPosition();
				vNewPos += m_vDir * fDeltaTemps * m_fVitessefusee ; 

				m_pParticleSystemFusee->SetPosition(vNewPos);
			}

			m_pParticleSystemFusee->Update(fDeltaTemps);

			break;


		case e_Wait:
			{
				m_fCurrentTime+= fDeltaTemps;

				D3DXVECTOR3 vNewPos = m_pParticleSystemFusee->GetPosition();
				vNewPos += m_vDir * fDeltaTemps * m_fVitessefusee ; 
				m_pParticleSystemFusee->SetPosition(vNewPos);


				if(m_fCurrentTime >= m_fTimeAfterExplode )
				{
					e_EtatFuseeArtifice = e_Explode;
					m_pParticleSystemExplode->SetPosition(m_pParticleSystemFusee->GetPosition());
					m_pParticleSystemExplode->RestartParticleSystem();
					m_fTimeToExplode = 0.0f;
				}
			}
			break;

		case e_Explode:
			{
				m_fTimeToExplode+=fDeltaTemps;
				if(m_fTimeToExplode > 4.0f)
				{
					switch(m_nSigne)
					{
					case -1:
						m_nSigne = 0;
						m_pParticleSystemFusee->SetColor(D3DXCOLOR( 1.0f, 0.2f, 0.2f, 1.0f ));
						m_pParticleSystemExplode->SetColor(D3DXCOLOR( 1.0f, 0.2f, 0.2f, 1.0f ));
						break;

					case 0:
						m_nSigne = 1;
						m_pParticleSystemFusee->SetColor(D3DXCOLOR( 1.0f, 1.0f, 0.3f, 1.0f ));
						m_pParticleSystemExplode->SetColor(D3DXCOLOR( 1.0f, 1.0f, 0.3f, 1.0f ));
						break;

					case 1:
						m_nSigne =-1;
						m_pParticleSystemFusee->SetColor(D3DXCOLOR( 0.2f, 1.0f, 0.2f, 1.0f ));
						m_pParticleSystemExplode->SetColor(D3DXCOLOR( 0.2f, 1.0f, 0.2f, 1.0f ));
						break;
					}
					e_EtatFuseeArtifice = e_Finish;
				}

				m_pParticleSystemExplode->Update(fDeltaTemps);

			}
			
			break;

		case e_Finish:
			{
				if (m_bBoucle)
				{
					m_pParticleSystemFusee->SetPosition(m_vPositionDepart);
					m_pParticleSystemFusee->RestartParticleSystem();
					e_EtatFuseeArtifice = e_Fusee;
				}
			}
			break;
		}	

}