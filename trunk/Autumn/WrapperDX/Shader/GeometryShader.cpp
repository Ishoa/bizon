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

#ifndef _GEOMETRY_SHADER_
#include "WrapperDx/Shader/GeometryShader.h"
#endif

GeometryShader::GeometryShader()
: Shader()
, m_pGeometryShader( NULL )
{
}

GeometryShader::GeometryShader( const char * name )
: Shader( name )
, m_pGeometryShader( NULL )
{
}

GeometryShader::~GeometryShader()
{
}

const char * GeometryShader::GetEntryPoint()
{
	return GEOMETRY_SHADER_ENTRY;
}

const char * GeometryShader::GetShaderVersion()
{
	switch ( g_pDevice->GetMaxFeatureLevel() )
	{
	case D3D_FEATURE_LEVEL_11_0 : return "gs_5_0"; break;
	case D3D_FEATURE_LEVEL_10_1 : return "gs_4_1"; break;
	case D3D_FEATURE_LEVEL_10_0 : return "gs_4_0"; break;
	case D3D_FEATURE_LEVEL_9_3 : return NULL; break;
	case D3D_FEATURE_LEVEL_9_2 : return NULL; break;
	case D3D_FEATURE_LEVEL_9_1 : return NULL; break;
	}

	return NULL;
}

HRESULT GeometryShader::Create()
{
	if( GetShaderVersion() == NULL )
	{
		LOG_INFOLN("Geometry Shader unavailable with this version of Graphic Card");
		return E_FAIL;
	}

	if( ! m_pBin )
		return E_FAIL;

	D_RETURN( g_pDxDevice->CreateGeometryShader( (DWORD*)(m_pBin->GetBufferPointer()), m_pBin->GetBufferSize(), NULL, &m_pGeometryShader ) );

	return S_OK;
}

HRESULT GeometryShader::Destroy()
{
	D_RETURN( Shader::Destroy() );

	SAFE_RELEASE( m_pGeometryShader );

	return S_OK;
}

void GeometryShader::Bind()
{
	g_pDxDeviceContext->GSSetShader( m_pGeometryShader , NULL, 0);
}

void GeometryShader::SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer )
{
	ID3D11Buffer * pBuffer = _pBuffer->GetBuffer();
	g_pDxDeviceContext->GSSetConstantBuffers(_iSlot, 1, &pBuffer );
}

void GeometryShader::SetTexture( unsigned int _iSlot, Texture * _pTexture )
{
	ID3D11ShaderResourceView * pSRV = _pTexture->GetResourceView();
	g_pDxDeviceContext->GSSetShaderResources(_iSlot, 1, &pSRV );
}

void GeometryShader::SetSampler( unsigned int _iSlot, SamplerBase * _pSampler )
{
	ID3D11SamplerState * pST = _pSampler->GetSampler();
	g_pDxDeviceContext->GSSetSamplers(_iSlot, 1, &pST );
}