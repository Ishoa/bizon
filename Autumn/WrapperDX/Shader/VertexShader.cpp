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

#ifndef _VERTEX_SHADER_
#include "VertexShader.h"
#endif


VertexShader::VertexShader()
: Shader()
, m_pVertexShader( NULL )
{
}

VertexShader::VertexShader(const char * name)
: Shader(name)
, m_pVertexShader( NULL )
{
}

VertexShader::~VertexShader()
{
}

const char * VertexShader::GetEntryPoint()
{
	return VERTEX_SHADER_ENTRY;
}

const char * VertexShader::GetShaderVersion()
{
	switch ( g_pDevice->GetMaxFeatureLevel() )
	{
	case D3D_FEATURE_LEVEL_11_0 : return "vs_5_0"; break;
	case D3D_FEATURE_LEVEL_10_1 : return "vs_4_1"; break;
	case D3D_FEATURE_LEVEL_10_0 : return "vs_4_0"; break;
	case D3D_FEATURE_LEVEL_9_3 : return "vs_4_0_level_9_3"; break;
	case D3D_FEATURE_LEVEL_9_2 : return "vs_4_0_level_9_1"; break;
	case D3D_FEATURE_LEVEL_9_1 : return "vs_4_0_level_9_1"; break;
	}

	return NULL;
}

HRESULT VertexShader::Create()
{
	if( ! m_pBin )
		return E_FAIL;

	D_RETURN( g_pDxDevice->CreateVertexShader( (DWORD*)(m_pBin->GetBufferPointer()), m_pBin->GetBufferSize(), NULL, &m_pVertexShader ) );

	return S_OK;
}
 
HRESULT VertexShader::Destroy()
{
	D_RETURN( Shader::Destroy() );

	SAFE_RELEASE(m_pVertexShader);

	return S_OK;
}

void VertexShader::Bind()
{
	g_pDxDeviceContext->VSSetShader( m_pVertexShader , NULL, 0);
}

void VertexShader::SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer )
{
	ID3D11Buffer * pBuffer = _pBuffer->GetBuffer();
	g_pDxDeviceContext->VSSetConstantBuffers(_iSlot, 1, &pBuffer );
}

void VertexShader::SetTexture( unsigned int _iSlot, Texture * _pTexture )
{
	ID3D11ShaderResourceView * pSRV = _pTexture->GetResourceView();
	g_pDxDeviceContext->VSSetShaderResources(_iSlot, 1, &pSRV );
}