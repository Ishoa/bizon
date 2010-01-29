#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDX/Device/Device.h"
#endif

#ifndef _RENDER_TARGET_
#include "RenderTarget.h"
#endif

RenderTarget::RenderTarget()
: m_pRenderTargetView(NULL)
, m_pShaderResourceView(NULL)
, m_pTexture(NULL)
{
	
}

RenderTarget::~RenderTarget()
{

}

HRESULT RenderTarget::Create()
{
	// recuperation de la rendertarget à partir des buffers de la swapchain
	D_RETURN( g_pDevice->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pTexture) );

	// creation de la rendertargetview
	D_RETURN( g_pDxDevice->CreateRenderTargetView(m_pTexture, NULL, &m_pRenderTargetView) );
	D3D11_TEXTURE2D_DESC TexDesc;
	m_pTexture->GetDesc( &TexDesc );
	m_iWidth = TexDesc.Width;
	m_iHeight = TexDesc.Height;

	// Create the SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC ResourceViewDesc;
	ResourceViewDesc.Format = TexDesc.Format;
	ResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ResourceViewDesc.Texture2D.MipLevels = TexDesc.MipLevels;
	D_RETURN( g_pDxDevice->CreateShaderResourceView(m_pTexture, &ResourceViewDesc, &m_pShaderResourceView) );

	return S_OK;
}

HRESULT RenderTarget::Create( unsigned int _iWidth, unsigned int _iHeight, eRenderTargetFormat _eFormat )
{
 	m_iWidth = _iWidth;
 	m_iHeight = _iHeight;
	m_eFormat = _eFormat;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	if( m_eFormat == eRenderTargetFormat_RGBA )
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	if( m_eFormat == eRenderTargetFormat_R )
		desc.Format = DXGI_FORMAT_R32_FLOAT;
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE  | D3D11_BIND_RENDER_TARGET;
	desc.Height = m_iHeight;
	desc.Width = m_iWidth;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MiscFlags = 0;
	D_RETURN( g_pDxDevice->CreateTexture2D(&desc, NULL, &m_pTexture));

	// Create the RTV
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	if( m_eFormat == eRenderTargetFormat_RGBA )
		RTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	if( m_eFormat == eRenderTargetFormat_R )
		RTVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;
	D_RETURN( g_pDxDevice->CreateRenderTargetView(m_pTexture, &RTVDesc, &m_pRenderTargetView) );

	// Create the SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	if( m_eFormat == eRenderTargetFormat_RGBA )
		SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	if( m_eFormat == eRenderTargetFormat_R )
		SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	D_RETURN( g_pDevice->GetDevice()->CreateShaderResourceView(m_pTexture, &SRVDesc, &m_pShaderResourceView) );

	return S_OK;
}

HRESULT RenderTarget::Resize()
{
	D_RETURN( Destroy() );
	D_RETURN( Create() );
	return S_OK;
}

HRESULT RenderTarget::Resize( unsigned int _uWidth, unsigned int _uHeight )
{
	D_RETURN( Destroy() );
	return Create(_uWidth, _uHeight, m_eFormat);
}

HRESULT RenderTarget::Destroy()
{
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pShaderResourceView);
	SAFE_RELEASE(m_pTexture);

	return S_OK;
}