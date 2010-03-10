#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDX/Device/Device.h" 
#endif

#ifndef _RASTERIZER_STATE_
#include "WrapperDX/Device/State/RasterizerState.h"
#endif


RasterizerState::RasterizerState()
: m_pRasterizerState( NULL )
{
}

RasterizerState::~RasterizerState()
{
}

HRESULT RasterizerState::Create( bool _bWireFrame /*= false*/, bool _bAntiAliasingEnable /*= false*/ )
{
	D3D11_RASTERIZER_DESC RasterizerDesc;
	RasterizerDesc.FillMode = (_bWireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID);
	RasterizerDesc.CullMode = D3D11_CULL_BACK;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.0f;
	RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.ScissorEnable = false;
	RasterizerDesc.MultisampleEnable = _bAntiAliasingEnable;
	RasterizerDesc.AntialiasedLineEnable = false;

	D_RETURN( g_pDxDevice->CreateRasterizerState(&RasterizerDesc, &m_pRasterizerState) );
	return S_OK;
}

HRESULT RasterizerState::Destroy()
{
	SAFE_RELEASE( m_pRasterizerState );
	return S_OK;
}

void RasterizerState::Bind()
{
	g_pDxDeviceContext->RSSetState( m_pRasterizerState );
}