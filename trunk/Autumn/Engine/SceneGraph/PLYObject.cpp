#include "stdafx.h"

#include "ReadPLY/readPLY.h"

#ifndef _ENGINE_CAMERA_
#include "Engine/Engine/EngineCamera.h"
#endif

#ifndef _PLY_OBJECT_
#include "Engine/SceneGraph/PLYObject.h"
#endif

PLYObject::PLYObject(const char * _strName /*= "no file"*/)
: DisplayObject()
{
	SetName( _strName );
}

PLYObject::~PLYObject()
{

}

HRESULT PLYObject::Create()
{
	D_RETURN( DisplayObject::Create() );

	// Shader
	D_RETURN( CompileShaders() );

	// Geometry
	D_RETURN( BuildGeometry() );

	return S_OK;

}

HRESULT PLYObject::Destroy()
{
	D_RETURN( DisplayObject::Destroy() );

	return S_OK;
}

void PLYObject::Render( EngineCamera * _pCamera, Light * _pLight )
{
	g_pDxDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	_pCamera->Set( m_mGlobalMatrix, _pLight );
	m_pIndexBuffer->Bind();
	m_pVertexBuffer->Bind();
	m_pVertexLayout->Bind();
	m_pVertexShader->Bind();
	m_pVertexShader->SetConstantBuffer( 0, _pCamera->GetCameraShaderParamBuffer() );
	m_pPixelShader->Bind();
	g_pDevice->UnbindGeometryShader();
	g_pDxDeviceContext->DrawIndexed( m_nIndex, 0, 0 );
}

bool PLYObject::Culling()
{
	return true;
}

HRESULT PLYObject::CompileShaders()
{
	// Vertex Shader
	m_pVertexShader = new VertexShader("DisplayObject.vsh");
	D_RETURN( m_pVertexShader->CompileAndCreate() );

	// Pixel Shader
	m_pPixelShader = new PixelShader("DisplayObject.psh");
	D_RETURN( m_pPixelShader->CompileAndCreate() );

	return S_OK;
}

HRESULT PLYObject::BuildGeometry()
{
// 	POINT_PLY * VertexBuffer;
// 	FACE_PLY * IndexBuffer;
// 
// 	char strFile[64];
// 	strcpy_s(strFile, SHADER_PATH);
// 	strcat_s(strFile, m_strFilename);
// 
// 	// Index
// 	int k = 0;
// 	for(int i=0;i<m_nIndex;i++)
// 	{
// 		for(int j=0;j<IndexBuffer[i].npts;j++)
// 		{
// 			m_Index[k] = IndexBuffer[i].ind[j];
// 			k++;
// 		}
// 	}
// 
// 	ReadPly(strFile, &m_nVertex, &VertexBuffer, &m_nIndex, &IndexBuffer);
// 
// 	// Vertex Layout
// 	m_pVertexLayout = new VertexLayout< DisplayObjectTypeVertex >;
// 	D_RETURN( m_pVertexLayout->Create( m_pVertexShader ) );
// 
// 	m_nVertex = 4;
// 	m_nIndex = 12;
// 
// 	// VertexBuffer
// 	{
// 		float * VertexData = new float[ m_nVertex * 3 ];
// 
// 		float TabCoeff[4][3];
// 		TabCoeff[0][0] = -m_fSize;	TabCoeff[0][1] = -m_fSize;	TabCoeff[0][2] = 0.0f;
// 		TabCoeff[1][0] =  m_fSize;	TabCoeff[1][1] = -m_fSize;	TabCoeff[1][2] = 0.0f;
// 		TabCoeff[2][0] =  m_fSize;	TabCoeff[2][1] =  m_fSize;	TabCoeff[2][2] = 0.0f; 
// 		TabCoeff[3][0] = -m_fSize;	TabCoeff[3][1] =  m_fSize;	TabCoeff[3][2] = 0.0f;
// 
// 		unsigned int uIndex = 0;
// 
// 		for(unsigned int i = 0; i < m_nVertex; i++)
// 		{
// 			// Position
// 			for(unsigned int j = 0; j < 3; j++)
// 				VertexData[uIndex++] = TabCoeff[i][j];
// 		}
// 
// 		m_pVertexBuffer = new VertexBuffer;
// 		D_RETURN( m_pVertexBuffer->Create( sizeof(float) * 3, m_nVertex, VertexData ) );
// 		SAFE_DELETE_ARRAY( VertexData );
// 	}
// 
// 	// IndexBuffer
// 	{
// 		unsigned int * IndexData = new unsigned int[m_nIndex];
// 
// 		IndexData[0] = 0;
// 		IndexData[1] = 2;
// 		IndexData[2] = 1;
// 		IndexData[3] = 0;
// 		IndexData[4] = 3;
// 		IndexData[5] = 2;
// 		IndexData[6] = 0;
// 		IndexData[7] = 1;
// 		IndexData[8] = 2;
// 		IndexData[9] = 0;
// 		IndexData[10] = 2;
// 		IndexData[11] = 3;
// 
// 		m_pIndexBuffer = new IndexBuffer;
// 		D_RETURN( m_pIndexBuffer->Create( sizeof(unsigned int), m_nIndex, IndexData ) );
// 	
// 		SAFE_DELETE_ARRAY( IndexData );
// 	}

	return S_OK;
}


void PLYObject::SetName(const char * _name)
{
	strcpy_s(m_strFilename, _name);
}