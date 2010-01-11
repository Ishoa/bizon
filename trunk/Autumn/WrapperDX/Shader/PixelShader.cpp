#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDx/Device/Device.h"
#endif

#ifndef _CONSTANT_BUFFER_
#include "WrapperDX/Buffer/ConstantBuffer.h"
#endif

#ifndef _SAMPLER_
#include "WrapperDX/Texture/Sampler/Sampler.h"
#endif

#ifndef _TEXTURE_
#include "WrapperDX/Texture/Texture.h"
#endif

#ifndef _PIXEL_SHADER_
#include "WrapperDx/Shader/PixelShader.h"
#endif


PixelShader::PixelShader()
: Shader()
, m_pPixelShader( NULL )
{
}

PixelShader::PixelShader(const char * name)
: Shader(name)
, m_pPixelShader( NULL )
{
}

PixelShader::~PixelShader()
{
}

const char * PixelShader::GetEntryPoint()
{
	return PIXEL_SHADER_ENTRY;
}

const char * PixelShader::GetShaderVersion()
{
	switch ( g_pDevice->GetMaxFeatureLevel() )
	{
	case D3D_FEATURE_LEVEL_11_0 : return "ps_5_0"; break;
	case D3D_FEATURE_LEVEL_10_1 : return "ps_4_1"; break;
	case D3D_FEATURE_LEVEL_10_0 : return "ps_4_0"; break;
	case D3D_FEATURE_LEVEL_9_3 : return "ps_4_0_level_9_3"; break;
	case D3D_FEATURE_LEVEL_9_2 : return "ps_4_0_level_9_1"; break;
	case D3D_FEATURE_LEVEL_9_1 : return "ps_4_0_level_9_1"; break;
	}

	return NULL;
}

HRESULT PixelShader::Create()
{
	if( ! m_pBin )
		return E_FAIL;

	D_RETURN( g_pDxDevice->CreatePixelShader( (DWORD*)(m_pBin->GetBufferPointer()), m_pBin->GetBufferSize(), NULL, &m_pPixelShader ) );

	return S_OK;
}

HRESULT PixelShader::Destroy()
{
	D_RETURN( Shader::Destroy() );

	SAFE_RELEASE( m_pPixelShader );

	return S_OK;
}

void PixelShader::Bind()
{
	g_pDxDeviceContext->PSSetShader( m_pPixelShader , NULL, 0);
}

void PixelShader::SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer )
{
	ID3D11Buffer * pBuffer = _pBuffer->GetBuffer();
	g_pDxDeviceContext->PSSetConstantBuffers(_iSlot, 1, &pBuffer );
}

void PixelShader::SetTexture( unsigned int _iSlot, Texture * _pTexture )
{
	ID3D11ShaderResourceView * pSRV = _pTexture->GetResourceView();
	g_pDxDeviceContext->PSSetShaderResources(_iSlot, 1, &pSRV );
}

void PixelShader::SetSampler( unsigned int _iSlot, SamplerBase * _pSampler )
{
	ID3D11SamplerState * pST = _pSampler->GetSampler();
	g_pDxDeviceContext->PSSetSamplers(_iSlot, 1, &pST );
}