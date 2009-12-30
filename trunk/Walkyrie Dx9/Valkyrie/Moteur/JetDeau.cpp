#include "JetDeau.h"


HRESULT UpdatePariculeCustom(CParticleSystem* pParticleSystem,float fDeltatime)
{
	Particle  *pParticle;
	Particle **ppParticle;
	Plane     *pPlane;
	Plane    **ppPlane;
	D3DXVECTOR3 vOldPosition;

	pParticleSystem->m_fDeltatime = fDeltatime;     // Update our particle system timer...
	ppParticle = &pParticleSystem->m_pActiveList; // Start at the head of the active list

	//m_pTexParticle->UpdateTexture(m_fDeltatime);

	while( *ppParticle )
	{
		pParticle = *ppParticle; // Set a pointer to the head
		pParticle->m_fTimePassed += pParticleSystem->m_fDeltatime;
		pParticleSystem->m_nCompteurParticule++;

		if( pParticle->m_fTimePassed >= pParticleSystem->m_fLifeCycle )
		{
			// Time is up, put the particle back on the free list...
			*ppParticle = pParticle->m_pNext;
			pParticle->m_pNext = pParticleSystem->m_pFreeList;
			pParticleSystem->m_pFreeList = pParticle;

			--pParticleSystem->m_dwActiveCount;
		}
		else
		{
			// Update particle position and velocity

			// Update velocity with respect to Gravity (Constant Acceleration)
			pParticle->m_vCurVel += pParticleSystem->m_vGravity * pParticleSystem->m_fDeltatime;

			// Update velocity with respect to Wind (Acceleration based on 
			// difference of vectors)
			if( pParticleSystem->m_bAirResistence == true )
				pParticle->m_vCurVel += (pParticleSystem->m_vWind - pParticle->m_vCurVel) * pParticleSystem->m_fDeltatime;

			// Finally, update position with respect to velocity
			vOldPosition = pParticle->m_vCurPos;
			D3DXVECTOR3 vecTmp = pParticle->m_vCurVel * pParticleSystem->m_fDeltatime;
			pParticle->m_vCurPos += vecTmp;

			pParticle->m_fPourcentColor = (pParticleSystem->m_fLifeCycle-pParticle->m_fTimePassed)/pParticleSystem->m_fLifeCycle;
			pParticle->m_nIndiceTextureCurrentParticule = pParticleSystem->m_pTexPrincipale->GetIndiceTextureTime(pParticle->m_fTimePassed,0.0f,pParticleSystem->m_fLifeCycle);

			pParticle->m_eEtatParticule = EEtat_Principale;
			if(pParticleSystem->m_fCoeffRotationParticule != 0.0f)
			{
				pParticle->m_fAngle += pParticleSystem->m_fCoeffRotationParticule;
				if(pParticle->m_fAngle > 360.0f)
					pParticle->m_fAngle = 0.0f;
			}

			//-----------------------------------------------------------------
			// BEGIN Checking the particle against each plane that was set up

			ppPlane = &pParticleSystem->m_pPlanes; // Set a pointer to the head

			while( *ppPlane )
			{
				pPlane = *ppPlane;
				int result = CParticleSystem::classifyPoint( &pParticle->m_vCurPos, pPlane );

				if( result == CP_BACK /*|| result == CP_ONPLANE */ )
				{
					if( pPlane->m_nCollisionResult == CR_BOUNCE )
					{
						pParticle->m_vCurPos = vOldPosition;

						float Kr = pPlane->m_fBounceFactor;

						D3DXVECTOR3 Vn = D3DXVec3Dot( &pPlane->m_vNormal, 
							&pParticle->m_vCurVel ) * 
							pPlane->m_vNormal;
						D3DXVECTOR3 Vt = pParticle->m_vCurVel - Vn;
						D3DXVECTOR3 Vp = Vt - Kr * Vn;

						pParticle->m_vCurVel = Vp;

					}
					else if( pPlane->m_nCollisionResult == CR_RECYCLE )
					{
						pParticle->m_fTimePassed = 0.0f;
					}

					else if( pPlane->m_nCollisionResult == CR_STICK )
					{
						pParticle->m_vCurPos = vOldPosition;
						pParticle->m_vCurVel = D3DXVECTOR3(0.0f,0.0f,0.0f);
					}
				}

				ppPlane = &pPlane->m_pNext;
			}

			// END Plane Checking
			//-----------------------------------------------------------------

			ppParticle = &pParticle->m_pNext;
		}
	}

	pParticleSystem->m_fLastUpdate += pParticleSystem->m_fDeltatime;
	if( pParticleSystem->m_fLastUpdate > pParticleSystem->m_fReleaseInterval )
	{
		// Reset update timing...
		pParticleSystem->m_fLastUpdate = 0.0f;

		// Emit new particles at specified flow rate...
		for( DWORD i = 0; i < pParticleSystem->m_dwNumToRelease; ++i )
		{
			// Do we have any free particles to put back to work?
			if( pParticleSystem->m_pFreeList )
			{
				// If so, hand over the first free one to be reused.
				pParticle = pParticleSystem->m_pFreeList;
				// Then make the next free particle in the list next to go!
				pParticleSystem->m_pFreeList = pParticle->m_pNext;
			}
			else
			{
				// There are no free particles to recycle...
				// We'll have to create a new one from scratch!
				if( pParticleSystem->m_dwActiveCount < pParticleSystem->m_dwMaxParticles )
				{
					if( NULL == ( pParticle = new Particle ) )
						return E_OUTOFMEMORY;
				}
			}

			if( pParticleSystem->m_dwActiveCount < pParticleSystem->m_dwMaxParticles )
			{
				pParticle->m_pNext = pParticleSystem->m_pActiveList; // Make it the new head...
				pParticleSystem->m_pActiveList = pParticle;

				// Set the attributes for our new particle...
				pParticle->m_vCurVel = pParticleSystem->m_vVelocity;

				if( pParticleSystem->m_fVelocityVar != 0.0f )
				{
					D3DXVECTOR3 vRandomVec = CParticleSystem::getRandomVector();
					pParticle->m_vCurVel += vRandomVec * pParticleSystem->m_fVelocityVar;
				}
				pParticle->m_eEtatParticule = EEtat_Principale;
				pParticle->m_fTimePassed  = 0.0f;
				pParticle->m_vCurPos    = pParticleSystem->m_vPosition;
			
				++pParticleSystem->m_dwActiveCount;
			}
		}
	}

	return S_OK;

}

CJetDeau::CJetDeau(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pJetDeau = new CParticleSystem(m_pD3DDevice);

}

CJetDeau::~CJetDeau()
{	
	delete m_pJetDeau;
}

bool CJetDeau::CreateAndInit(char **g_pTextNameJetDeau , int nTailleJetDeau)
{

	if(nTailleJetDeau > 1)
		m_pJetDeau->SetMultiTexture(g_pTextNameJetDeau,nTailleJetDeau);
	else
		m_pJetDeau->SetTexture(g_pTextNameJetDeau[0]);
	

	m_pJetDeau->SetMaxParticles( 3000 );
	m_pJetDeau->SetNumToRelease( 20 );
	m_pJetDeau->SetReleaseInterval( 0.02f );
	m_pJetDeau->SetLifeCycle( 2.5f );
	m_pJetDeau->SetSize( 0.2f );
	m_pJetDeau->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.7f, 0.7f ));
	m_pJetDeau->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pJetDeau->SetVelocity( D3DXVECTOR3( 0.0f, 15.0f, 0.0f) );
	m_pJetDeau->SetGravity( D3DXVECTOR3( 0.0f, -9.8f, 0.0f) );
	m_pJetDeau->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pJetDeau->SetVelocityVar( 1.0f );

	m_pJetDeau->SetCollisionPlane( D3DXVECTOR3( 0.0f, 1.0f, 0.0f ), 
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	//m_pJetDeau->SetFonctionUpdate(&UpdatePariculeCustom);

	m_pJetDeau->Init();

	return true;
}

void CJetDeau::DestructionObjet()
{

	m_pJetDeau->Release();
}

void CJetDeau::Rendu3D()
{
	m_pJetDeau->Rendu3D();
}

void CJetDeau::Animation(float fDeltaTemps)
{
	m_pJetDeau->Update(fDeltaTemps);

}

