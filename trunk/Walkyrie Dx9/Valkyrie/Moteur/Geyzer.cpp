#include "Geyzer.h"



CGeyzer::CGeyzer(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pGeyzer = new CParticleSystem(m_pD3DDevice);

}


CGeyzer::~CGeyzer()
{	
	delete m_pGeyzer;
}

bool CGeyzer::CreateAndInit(char **g_pTextNameJetDeau , int nTailleJetDeau)
{

	if(nTailleJetDeau > 1)
		m_pGeyzer->SetMultiTexture(g_pTextNameJetDeau,nTailleJetDeau);
	else
		m_pGeyzer->SetTexture(g_pTextNameJetDeau[0]);


	m_pGeyzer->SetMaxParticles( 1000 );
	m_pGeyzer->SetNumToRelease( 5 );
	m_pGeyzer->SetReleaseInterval( 0.005f );
	m_pGeyzer->SetLifeCycle( 1.0f );
	m_pGeyzer->SetSize( 0.5f );
	m_pGeyzer->SetColor( D3DXCOLOR( 1.0f, 1.0f, 0.3f, 1.0f ));
	m_pGeyzer->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pGeyzer->SetVelocity( D3DXVECTOR3( 0.0f, 25.0f, 0.0f) );
	m_pGeyzer->SetGravity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pGeyzer->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	m_pGeyzer->SetVelocityVar( 1.0f );

	m_pGeyzer->Init();

	return true;
}

void CGeyzer::DestructionObjet()
{

	m_pGeyzer->Release();
}

void CGeyzer::Rendu3D()
{
	m_pGeyzer->Rendu3D();
}

void CGeyzer::Animation(float fDeltaTemps)
{
	m_pGeyzer->Update(fDeltaTemps);

}

