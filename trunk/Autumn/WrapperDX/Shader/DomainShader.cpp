#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDx/Device/Device.h"
#endif

#ifndef _CONSTANT_BUFFER_
#include "WrapperDX/Buffer/ConstantBuffer.h"
#endif

#ifndef _TEXTURE_
#include "WrapperDX/Texture/Texture.h"
#endif

#ifndef _SAMPLER_
#include "WrapperDX/Texture/Sampler/Sampler.h"
#endif

#ifndef _DOMAIN_SHADER_
#include "WrapperDX/Shader/DomainShader.h"
#endif


DomainShader::DomainShader()
: Shader()
, m_pDomainShader( NULL )
{
}

DomainShader::DomainShader(const char * name)
: Shader(name)
, m_pDomainShader( NULL )
{
}

DomainShader::~DomainShader()
{
}

const char * DomainShader::GetEntryPoint()
{
	return DOMAIN_SHADER_ENTRY;
}

const char * DomainShader::GetShaderVersion()
{
	if ( g_pDevice->GetMaxFeatureLevel() == D3D_FEATURE_LEVEL_11_0 )
		return "ds_5_0";
	else
		return NULL;
}

HRESULT DomainShader::Create()
{
	if( ! m_pBin )
		return E_FAIL;

	D_RETURN( g_pDxDevice->CreateDomainShader( (DWORD*)(m_pBin->GetBufferPointer()), m_pBin->GetBufferSize(), NULL, &m_pDomainShader ) );

	return S_OK;
}
 
HRESULT DomainShader::Destroy()
{
	D_RETURN( Shader::Destroy() );

	SAFE_RELEASE(m_pDomainShader);

	return S_OK;
}

void DomainShader::Bind()
{
	g_pDxDeviceContext->DSSetShader( m_pDomainShader , NULL, 0);
}

void DomainShader::SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer )
{
	ID3D11Buffer * pBuffer = _pBuffer->GetBuffer();
	g_pDxDeviceContext->DSSetConstantBuffers(_iSlot, 1, &pBuffer );
}

void DomainShader::SetTexture( unsigned int _iSlot, Texture * _pTexture )
{
	ID3D11ShaderResourceView * pSRV = _pTexture->GetResourceView();
	g_pDxDeviceContext->DSSetShaderResources(_iSlot, 1, &pSRV );
}

void DomainShader::SetSampler( unsigned int _iSlot, SamplerBase * _pSampler )
{
	ID3D11SamplerState * pST = _pSampler->GetSampler();
	g_pDxDeviceContext->DSSetSamplers(_iSlot, 1, &pST );
}