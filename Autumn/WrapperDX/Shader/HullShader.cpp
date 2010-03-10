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

#ifndef _HULL_SHADER_
#include "WrapperDX/Shader/HullShader.h"
#endif


HullShader::HullShader()
: Shader()
, m_pHullShader( NULL )
{
}

HullShader::HullShader(const char * name)
: Shader(name)
, m_pHullShader( NULL )
{
}

HullShader::~HullShader()
{
}

const char * HullShader::GetEntryPoint()
{
	return HULL_SHADER_ENTRY;
}

const char * HullShader::GetShaderVersion()
{
	if( g_pDevice->GetMaxFeatureLevel() == D3D_FEATURE_LEVEL_11_0 )
		return "hs_5_0";
	else
		return NULL;
}

HRESULT HullShader::Create()
{
	if( ! m_pBin )
		return E_FAIL;

	D_RETURN( g_pDxDevice->CreateHullShader( (DWORD*)(m_pBin->GetBufferPointer()), m_pBin->GetBufferSize(), NULL, &m_pHullShader ) );

	return S_OK;
}
 
HRESULT HullShader::Destroy()
{
	D_RETURN( Shader::Destroy() );

	SAFE_RELEASE(m_pHullShader);

	return S_OK;
}

void HullShader::Bind()
{
	g_pDxDeviceContext->HSSetShader( m_pHullShader , NULL, 0);
}

void HullShader::SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer )
{
	ID3D11Buffer * pBuffer = _pBuffer->GetBuffer();
	g_pDxDeviceContext->HSSetConstantBuffers(_iSlot, 1, &pBuffer );
}

void HullShader::SetTexture( unsigned int _iSlot, Texture * _pTexture )
{
	ID3D11ShaderResourceView * pSRV = _pTexture->GetResourceView();
	g_pDxDeviceContext->HSSetShaderResources(_iSlot, 1, &pSRV );
}

void HullShader::SetSampler( unsigned int _iSlot, SamplerBase * _pSampler )
{
	ID3D11SamplerState * pST = _pSampler->GetSampler();
	g_pDxDeviceContext->HSSetSamplers(_iSlot, 1, &pST );
}