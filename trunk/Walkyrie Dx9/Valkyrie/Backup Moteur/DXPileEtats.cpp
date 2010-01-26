#include "DXPileEtats.h"

CDXPileEtats::CDXPileEtats(LPDIRECT3DDEVICE9 pD3DDevice)
{

	m_pD3DDevice = pD3DDevice;
}


CDXPileEtats::~CDXPileEtats()
{

}
// Cette fonction sauvegarde l'état du Device Direct3D avant utilisation.
bool CDXPileEtats::PushDeviceProperties()
{
	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &m_Light);
		m_pD3DDevice->GetRenderState(D3DRS_ZENABLE, &m_ZEnable);
		m_pD3DDevice->GetSamplerState( 0, D3DSAMP_ADDRESSU,   &m_AdressU );
		m_pD3DDevice->GetSamplerState( 0, D3DSAMP_ADDRESSV,   &m_AdressV );
		m_pD3DDevice->GetRenderState ( D3DRS_ZWRITEENABLE,    &m_ZWriteEnable );
		m_pD3DDevice->GetRenderState ( D3DRS_SRCBLEND,        &m_SrcBlend );
		m_pD3DDevice->GetRenderState ( D3DRS_DESTBLEND,       &m_DestBlend );
		m_pD3DDevice->GetRenderState ( D3DRS_ALPHATESTENABLE, &m_AlphaTestEnable );
		m_pD3DDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE,&m_AlphaBlendEnable );
		return true;
	}
	else
	
		return false;


}


// Cette fonction restaure l'état du Device Direct3D après utilisation.
bool CDXPileEtats::PopDeviceProperties()
{

	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, m_ZEnable);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, m_Light);
		m_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,    m_AdressU );
		m_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,    m_AdressV );
		m_pD3DDevice->SetRenderState ( D3DRS_ZWRITEENABLE,     m_ZWriteEnable );
		m_pD3DDevice->SetRenderState ( D3DRS_SRCBLEND,         m_SrcBlend );
		m_pD3DDevice->SetRenderState ( D3DRS_DESTBLEND,        m_DestBlend );
		m_pD3DDevice->SetRenderState ( D3DRS_ALPHATESTENABLE,  m_AlphaTestEnable );
		m_pD3DDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, m_AlphaBlendEnable );
		return true;
	}
	else
		return false;
}

