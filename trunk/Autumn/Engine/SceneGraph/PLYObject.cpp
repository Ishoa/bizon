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

	// Texture
	char strFile[64];
	strcpy_s(strFile, m_strTextureName);
	strcat_s(strFile, "NormalMap.png");
	m_pNormalMap = new Texture2D(strFile);
	D_RETURN( m_pNormalMap->Create() );

	strcpy_s(strFile, m_strTextureName);
	strcat_s(strFile, "Tex.png");
	m_pTexture = new Texture2D(strFile);
	D_RETURN( m_pTexture->Create() );

	return S_OK;

}

HRESULT PLYObject::Destroy()
{
	D_RETURN( DisplayObject::Destroy() );

	return S_OK;
}

void PLYObject::Render( EngineCamera * _pCamera, Light * _pLight )
{
	DisplayObject::Render(_pCamera, _pLight);

	g_pDxDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	_pCamera->Set( m_mGlobalMatrix, _pLight );
	m_pIndexBuffer->Bind();
	m_pVertexBuffer->Bind();
	m_pVertexLayout->Bind();
	m_pVertexShader->Bind();
	m_pVertexShader->SetConstantBuffer( 0, _pCamera->GetCameraShaderParamBuffer() );
	m_pPixelShader->Bind();
	m_pPixelShader->SetConstantBuffer( 0, _pCamera->GetCameraShaderParamBuffer() );
	m_pPixelShader->SetTexture(0, m_pNormalMap);
	m_pPixelShader->SetTexture(1, m_pTexture);
	m_pPixelShader->SetSampler(0, m_pSampler);
	g_pDevice->UnbindGeometryShader();
	g_pDxDeviceContext->DrawIndexed( m_nIndex, 0, 0 );
}

bool PLYObject::Culling()
{
	return DisplayObject::Culling();
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
	POINT_PLY * tVertexBuffer;
	FACE_PLY * tIndexBuffer;

	char strFile[64];
	strcpy_s(strFile, OBJECT_PLY_PATH);
	strcat_s(strFile, m_strFilename);
	int nVertex, nIndex;
	ReadPly(strFile, &nVertex, &tVertexBuffer, &nIndex, &tIndexBuffer);

	m_nVertex = nVertex;
	m_nIndex = nIndex;
	unsigned int nFaces = m_nIndex;
	m_nIndex *= 3;
	unsigned int * IndexData = new unsigned int[ m_nIndex ];

	sVertex * VertexData = new sVertex[ m_nVertex ];

	// Index
	{
		unsigned int uFanceInd = 0;
		for(unsigned int i = 0; i < nFaces; ++i)
		{
			IndexData[uFanceInd++] = tIndexBuffer[i].ind[0];
			IndexData[uFanceInd++] = tIndexBuffer[i].ind[1];
			IndexData[uFanceInd++] = tIndexBuffer[i].ind[2];
			SAFE_DELETE_ARRAY( tIndexBuffer[i].ind );
		}
		m_pIndexBuffer = new IndexBuffer;
		D_RETURN( m_pIndexBuffer->Create( sizeof(unsigned int), m_nIndex, IndexData ) );
	}

	// Vertex Buffer
	{
		Vector3 MaxPoint(FLT_MIN, FLT_MIN, FLT_MIN), MinPoint(FLT_MAX, FLT_MAX, FLT_MAX), oCenter;
		for(unsigned int i = 0; i < m_nVertex; ++i)
		{
			// Position
			VertexData[i].x = tVertexBuffer[i].x;
			VertexData[i].y = tVertexBuffer[i].y;
			VertexData[i].z = tVertexBuffer[i].z;
			// used to calculate center point to generate texcoord
			MaxPoint.X() = max( tVertexBuffer[i].x, MaxPoint.X() );
			MaxPoint.Y() = max( tVertexBuffer[i].y, MaxPoint.Y() );
			MaxPoint.Z() = max( tVertexBuffer[i].z, MaxPoint.Z() );
			MinPoint.X() = min( tVertexBuffer[i].x, MinPoint.X() );
			MinPoint.Y() = min( tVertexBuffer[i].y, MinPoint.Y() );
			MinPoint.Z() = min( tVertexBuffer[i].z, MinPoint.Z() );

			// Normal
			VertexData[i].nx = tVertexBuffer[i].nx;
			VertexData[i].ny = tVertexBuffer[i].ny;
			VertexData[i].nz = tVertexBuffer[i].nz;

			// Tangent
			VertexData[i].tx = 0.0f;
			VertexData[i].ty = 0.0f;
			VertexData[i].tz = 0.0f;
		}

		oCenter = ( MaxPoint + MinPoint ) * 0.5f;

		// TexCoord
		Vector3 Normal;
		float angle1, angle2( 0.0f );

		for(unsigned int i = 0 ; i < m_nVertex; ++i)
		{
			Normal = Vector3(tVertexBuffer[i].x, tVertexBuffer[i].y, tVertexBuffer[i].z) - oCenter;
			Normal = Normal.Normalize();
			angle1 = acosf( Normal.Z() );
			if(cosf( angle1 ) != 0.0f)
				angle2 = atan2f( Normal.Y(), Normal.X() );
			else
				angle2 = 0.0f;

			VertexData[i].u = 0.5f + ( 0.5f * angle2/(float)M_PI);
			VertexData[i].v = angle1 / (float)M_PI;
		}

		// generate tangents
		for( unsigned int i = 0; i < m_nIndex; i+=3 )
		{
			const sVertex & v0 = VertexData[IndexData[i]];
			const sVertex & v1 = VertexData[IndexData[i+1]];
			const sVertex & v2 = VertexData[IndexData[i+2]];

			Vector3 v1v0( v1.x - v0.x, v1.y - v0.y, v1.z - v0.z );
			Vector3 v2v0( v2.x - v0.x, v2.y - v0.y, v2.z - v0.z );

			float v1v0_u = v1.u - v0.u;
			float v2v0_u = v2.u - v0.u;
			float v1v0_v = v1.v - v0.v;
			float v2v0_v = v2.v - v0.v;

			float fDet = v1v0_u * v2v0_v - v2v0_u * v1v0_v;

			Vector3 tangente(1,0,0);

			if( abs(fDet) > 0.0001f )
			{
				float fScale = 1.0f / fDet;

				tangente = Vector3( ( v2v0_v * v1v0.X() - v1v0_v * v2v0.X() ) * fScale,
									( v2v0_v * v1v0.Y() - v1v0_v * v2v0.Y() ) * fScale,
									( v2v0_v * v1v0.Z() - v1v0_v * v2v0.Z() ) * fScale );
			}


			tangente = tangente.Normalize();

			VertexData[ IndexData[i] ].tx += tangente.X();
			VertexData[ IndexData[i] ].ty += tangente.Y();
			VertexData[ IndexData[i] ].tz += tangente.Z();
			VertexData[ IndexData[i+1] ].tx += tangente.X();
			VertexData[ IndexData[i+1] ].ty += tangente.Y();
			VertexData[ IndexData[i+1] ].tz += tangente.Z();
			VertexData[ IndexData[i+2] ].tx += tangente.X();
			VertexData[ IndexData[i+2] ].ty += tangente.Y();
			VertexData[ IndexData[i+2] ].tz += tangente.Z();
		}
		for( unsigned int i = 0 ; i < m_nVertex; ++i )
		{
			Vector3 tangente( VertexData[i].tx, VertexData[i].ty, VertexData[i].tz );
			tangente = tangente.Normalize();
			VertexData[i].tx = tangente.X();
			VertexData[i].ty = tangente.Y();
			VertexData[i].tz = tangente.Z();
		}

		m_pVertexBuffer = new VertexBuffer;
		D_RETURN( m_pVertexBuffer->Create( sizeof(sVertex), m_nVertex, VertexData ) );
	}

	// Vertex Layout
	{
		m_pVertexLayout = new VertexLayout< DisplayObjectTypeVertex >;
		D_RETURN( m_pVertexLayout->Create( m_pVertexShader ) );
	}

	SAFE_DELETE_ARRAY( VertexData );
	SAFE_DELETE_ARRAY( IndexData );
	SAFE_DELETE_ARRAY( tVertexBuffer );
	SAFE_DELETE_ARRAY( tIndexBuffer );

	return S_OK;
}


void PLYObject::SetName(const char * _name)
{
	strcpy_s(m_strFilename, _name);
}