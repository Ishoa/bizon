#include "stdafx.h"

#ifndef _ENGINE_CAMERA_
#include "Engine/Engine/EngineCamera.h"
#endif

#ifndef _DSKY_
#include "Engine/SceneGraph/Sky.h"
#endif

Sky::Sky()
: m_nIndex(0)
, m_nVertex(0)
, m_pVertexBuffer(NULL)
, m_pIndexBuffer(NULL)
, m_pVertexLayout(NULL)
, m_pVertexShader(NULL)
, m_pPixelShader(NULL)
, m_pSkyShaderParamBuffer(NULL)
{

}

Sky::~Sky()
{

}

HRESULT Sky::Create(EngineCamera * _pCamera)
{
	D_RETURN( Node::Create() );

	// Shader
	D_RETURN( CompileShaders() );

	// Geometry
	D_RETURN( BuildGeometry() );

	m_sSkyShaderParam.m_fFov = _pCamera->GetFOV();
	m_sSkyShaderParam.m_fScreenRatio = _pCamera->GetAspect();
	m_pSkyShaderParamBuffer = new ConstantBufferUpdate;
	D_RETURN( m_pSkyShaderParamBuffer->Create( sizeof( sSkyShaderParam ), 1, & m_sSkyShaderParam ) );

	return S_OK;
}

HRESULT Sky::Destroy()
{
	D_RETURN( Node::Destroy() );
	SAFE_DESTROY( m_pVertexBuffer );
	SAFE_DESTROY( m_pIndexBuffer );
	SAFE_DESTROY( m_pVertexLayout );
	SAFE_DESTROY( m_pVertexShader );
	SAFE_DESTROY( m_pPixelShader );
	SAFE_DESTROY( m_pSkyShaderParamBuffer );
	return S_OK;
}

void Sky::Render( EngineCamera * _pCamera, Light * _pLight )
{
	Node::Render(_pCamera, _pLight);

	g_pDxDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	_pCamera->Set( m_mGlobalMatrix, _pLight );
	m_pIndexBuffer->Bind();
	m_pVertexBuffer->Bind();
	m_pVertexLayout->Bind();
	m_pVertexShader->Bind();
	m_pVertexShader->SetConstantBuffer( 0, _pCamera->GetCameraShaderParamBuffer() );
	m_pPixelShader->Bind();
	m_pPixelShader->SetConstantBuffer( 0, _pCamera->GetCameraShaderParamBuffer() );
	m_pPixelShader->SetConstantBuffer( 1, m_pSkyShaderParamBuffer );
	g_pDevice->UnbindGeometryShader();
	g_pDevice->DrawIndexed( m_nIndex );
}

bool Sky::Culling()
{
	return true;
}

HRESULT Sky::CompileShaders()
{
	// Vertex Shader
	m_pVertexShader = new VertexShader("Sky.vsh");
	D_RETURN( m_pVertexShader->CompileAndCreate() );

	// Pixel Shader
	m_pPixelShader = new PixelShader("Sky.psh");
	D_RETURN( m_pPixelShader->CompileAndCreate() );

	return S_OK;
}

HRESULT Sky::BuildGeometry()
{
	// Vertex Layout
	m_pVertexLayout = new VertexLayout<SkyTypeVertex >;
	D_RETURN( m_pVertexLayout->Create( m_pVertexShader ) );

	m_nVertex = 4;
	m_nIndex = 6;

	unsigned int * IndexData = new unsigned int[ m_nIndex ];
	sVertex * VertexData = new sVertex[ m_nVertex ];

	// Index
	{
		IndexData[0] = 0;
		IndexData[1] = 1;
		IndexData[2] = 3;

		IndexData[3] = 0;
		IndexData[4] = 3;
		IndexData[5] = 2;

		m_pIndexBuffer = new IndexBuffer;
		D_RETURN( m_pIndexBuffer->Create( sizeof(unsigned int), m_nIndex, IndexData ) );
	}

	// Vertex Buffer
	{
		for( unsigned int i = 0; i < 2; ++i )
		{
			for( unsigned int j = 0; j < 2; ++j )
			{
				VertexData[ i * 2 + j ].x = i * 2.0f - 1.0f;
				VertexData[ i * 2 + j ].y = j * 2.0f - 1.0f;
			}
		}

		m_pVertexBuffer = new VertexBuffer;
		D_RETURN( m_pVertexBuffer->Create( sizeof(sVertex), m_nVertex, VertexData ) );
	}

	SAFE_DELETE_ARRAY( VertexData );
	SAFE_DELETE_ARRAY( IndexData );

	return S_OK;
}

void Sky::SetParam( float _fFov, float _fRatio )
{
	sSkyShaderParam * pShaderParam = (sSkyShaderParam *)m_pSkyShaderParamBuffer->Map();
	pShaderParam->m_fFov			= _fFov;
	pShaderParam->m_fScreenRatio	= _fRatio;
	m_pSkyShaderParamBuffer->Unmap();
}