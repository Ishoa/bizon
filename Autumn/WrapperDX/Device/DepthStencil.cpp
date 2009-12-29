#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDX/Device/Device.h" 
#endif

#ifndef _DEPTH_STENCIL_
#include "WrapperDX/Device/DepthStencil.h"
#endif

DepthStencil::DepthStencil()
: m_pDepthStencilState( NULL )
, m_pDepthStencilView( NULL )
, m_pDepthStencil( NULL )
, m_pShaderResourceView( NULL )
{
}
DepthStencil::~DepthStencil()
{
}

HRESULT	DepthStencil::Create(unsigned int _uWidth, unsigned int _uHeight)
{
	// création du depthstencil
	D3D11_TEXTURE2D_DESC DepthStencilDesc;
	DepthStencilDesc.Width = _uWidth;
	DepthStencilDesc.Height = _uHeight;
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.ArraySize = 1;
	DepthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	DepthStencilDesc.SampleDesc.Count = 1;
	DepthStencilDesc.SampleDesc.Quality = 0;
	DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	DepthStencilDesc.CPUAccessFlags = 0;
	DepthStencilDesc.MiscFlags = 0;

	D_RETURN( g_pDxDevice->CreateTexture2D(&DepthStencilDesc, NULL, &m_pDepthStencil) );

	// creation de la SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	D_RETURN( g_pDevice->GetDevice()->CreateShaderResourceView( m_pDepthStencil, &SRVDesc, &m_pShaderResourceView ) );

	// creation du depthstencilview
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;
	DepthStencilViewDesc.Flags = 0;

	D_RETURN( g_pDxDevice->CreateDepthStencilView(m_pDepthStencil, &DepthStencilViewDesc, &m_pDepthStencilView) );

	// creation du depthstencilstate
	D3D11_DEPTH_STENCIL_DESC DPdesc;
	ZeroMemory(&DPdesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DPdesc.DepthEnable = true;
	DPdesc.DepthFunc = D3D11_COMPARISON_LESS;
	DPdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DPdesc.StencilEnable = false;
	DPdesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	DPdesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	D_RETURN( g_pDxDevice->CreateDepthStencilState(&DPdesc, &m_pDepthStencilState) );


	return S_OK;
}

HRESULT DepthStencil::Resize(unsigned int _uWidth, unsigned int _uHeight)
{
	D_RETURN( Destroy() );
	return Create(_uWidth, _uHeight);
}

HRESULT DepthStencil::Destroy()
{
	SAFE_RELEASE( m_pDepthStencilState );
	SAFE_RELEASE( m_pDepthStencilView );
	SAFE_RELEASE( m_pDepthStencil );
	SAFE_RELEASE( m_pShaderResourceView );

	return S_OK;
}
