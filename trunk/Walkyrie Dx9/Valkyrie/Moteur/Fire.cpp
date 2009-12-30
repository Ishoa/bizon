#include "Fire.h"


HRESULT UpdatePariculeCustomFire(CParticleSystem* pParticleSystem,float fDeltatime)
{
	Particle  *pParticle;
	Particle **ppParticle;
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

			if(pParticle->m_fPourcentColor < 0.7f)
				pParticle->m_eEtatParticule = EEtat_Secondaire;

/*
			if(pParticleSystem->m_fCoeffRotationParticule != 0.0f)
			{
				pParticle->m_fAngle += pParticleSystem->m_fCoeffRotationParticule;
				if(pParticle->m_fAngle > 360.0f)
					pParticle->m_fAngle = 0.0f;
			}*/


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

CFire::CFire(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pFire = new CParticleSystem(m_pD3DDevice);

}

CFire::~CFire()
{	
	delete m_pFire;
}

bool CFire::CreateAndInit(char **g_pTextNameFire , int nTailleFire,char **g_pTextNameSmoke , int nTailleSmoke)
{

	if(nTailleFire > 1)
		m_pFire->SetMultiTexture(g_pTextNameFire,nTailleFire);
	else
		m_pFire->SetTexture(g_pTextNameFire[0]);

	if(nTailleSmoke > 1)
		m_pFire->SetMultiTextureSecondaire(g_pTextNameSmoke,nTailleSmoke);
	else
		m_pFire->SetTextureSecondaire(g_pTextNameSmoke[0]);

	m_pFire->SetMaxParticles( 1000 );
	m_pFire->SetNumToRelease( 5 );
	m_pFire->SetReleaseInterval( 0.01f );
	m_pFire->SetLifeCycle( 2.0f );
	m_pFire->SetSize( 0.6f );
	m_pFire->SetColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
	m_pFire->SetPosition( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
	m_pFire->SetVelocity( D3DXVECTOR3( 0.0f, 5.0f, 0.0f) );
	m_pFire->SetGravity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pFire->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pFire->SetVelocityVar( 3.0f );
	//m_pFire->SetCoeffRotationParticule(0.1f);

	m_pFire->SetFonctionUpdate(&UpdatePariculeCustomFire);

	m_pFire->Init();

	return true;
}

void CFire::DestructionObjet()
{

	m_pFire->Release();
}

void CFire::Rendu3D()
{
	m_pFire->Rendu3D();
}

void CFire::Animation(float fDeltaTemps)
{
	m_pFire->Update(fDeltaTemps);

}

