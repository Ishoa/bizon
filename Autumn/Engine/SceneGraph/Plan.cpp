#include "stdafx.h"

#ifndef _ENGINE_CAMERA_
#include "Engine/Engine/EngineCamera.h"
#endif

#ifndef _PLAN_
#include "Engine/SceneGraph/Plan.h"
#endif

Plan::Plan()
: DisplayObject()
{
	Set( 10.0f );
}

Plan::~Plan()
{

}

HRESULT Plan::Create()
{
	D_RETURN( Node::Create() );

	// Shader
	D_RETURN( CompileShaders() );

	// Geometry
	D_RETURN( BuildGeometry() );

	return S_OK;

}

HRESULT Plan::Destroy()
{
	D_RETURN( DisplayObject::Destroy() );

	return S_OK;
}

void Plan::Render( EngineCamera * _pCamera )
{
	g_pDxDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	_pCamera->SetWorld( m_mGlobalMatrix );
	m_pIndexBuffer->Bind();
	m_pVertexBuffer->Bind();
	m_pVertexLayout->Bind();
	m_pVertexShader->Bind();
	m_pVertexShader->SetConstantBuffer( 0, _pCamera->GetCameraShaderParamBuffer() );
	m_pPixelShader->Bind();
	g_pDevice->UnbindGeometryShader();
	g_pDxDeviceContext->DrawIndexed( m_nIndex, 0, 0 );
}

bool Plan::Culling()
{
	return true;
}

HRESULT Plan::CompileShaders()
{
	// Vertex Shader
	m_pVertexShader = new VertexShader("DisplayObject.vsh");
	D_RETURN( m_pVertexShader->CompileAndCreate() );

	// Pixel Shader
	m_pPixelShader = new PixelShader("DisplayObject.psh");
	D_RETURN( m_pPixelShader->CompileAndCreate() );

	return S_OK;
}

HRESULT Plan::BuildGeometry()
{
	// Vertex Layout
	m_pVertexLayout = new VertexLayout< DisplayObjectTypeVertex >;
	D_RETURN( m_pVertexLayout->Create( m_pVertexShader ) );

	m_nVertex = 4;
	m_nIndex = 12;

	// VertexBuffer
	{
		float * VertexData = new float[ m_nVertex * 3 ];

		float TabCoeff[4][3];
		TabCoeff[0][0] = -m_fSize;	TabCoeff[0][1] = -m_fSize;	TabCoeff[0][2] = 0.0f;
		TabCoeff[1][0] =  m_fSize;	TabCoeff[1][1] = -m_fSize;	TabCoeff[1][2] = 0.0f;
		TabCoeff[2][0] =  m_fSize;	TabCoeff[2][1] =  m_fSize;	TabCoeff[2][2] = 0.0f; 
		TabCoeff[3][0] = -m_fSize;	TabCoeff[3][1] =  m_fSize;	TabCoeff[3][2] = 0.0f;

		unsigned int uIndex = 0;

		for(unsigned int i = 0; i < m_nVertex; i++)
		{
			// Position
			for(unsigned int j = 0; j < 3; j++)
				VertexData[uIndex++] = TabCoeff[i][j];
		}

		m_pVertexBuffer = new VertexBuffer;
		D_RETURN( m_pVertexBuffer->Create( sizeof(float) * 3, m_nVertex, VertexData ) );
		SAFE_DELETE_ARRAY( VertexData );
	}

	// IndexBuffer
	{
		unsigned int * IndexData = new unsigned int[m_nIndex];

		IndexData[0] = 0;
		IndexData[1] = 2;
		IndexData[2] = 1;
		IndexData[3] = 0;
		IndexData[4] = 3;
		IndexData[5] = 2;
		IndexData[6] = 0;
		IndexData[7] = 1;
		IndexData[8] = 2;
		IndexData[9] = 0;
		IndexData[10] = 2;
		IndexData[11] = 3;

		m_pIndexBuffer = new IndexBuffer;
		D_RETURN( m_pIndexBuffer->Create( sizeof(unsigned int), m_nIndex, IndexData ) );
	
		SAFE_DELETE_ARRAY( IndexData );
	}

	return S_OK;
}

void Plan::Set( const float & _fSize )
{
	m_fSize = _fSize;
}