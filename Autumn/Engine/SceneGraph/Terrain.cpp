#include "stdafx.h"

#ifndef _ENGINE_CAMERA_
#include "Engine/Engine/EngineCamera.h"
#endif

#ifndef _TERRAIN_
#include "Engine/SceneGraph/Terrain.h"
#endif

Terrain::Terrain()
: m_nTiles( 10 )
, m_fWidth( 100.0f )
, m_fHeight( 100.0f )
, m_nIndex(0)
, m_nVertex(0)
, m_pVertexBuffer(NULL)
, m_pIndexBuffer(NULL)
, m_pVertexLayout(NULL)
, m_pVertexShader(NULL)
, m_pHullShader(NULL)
, m_pDomainShader(NULL)
, m_pPixelShader(NULL)
, m_pNormalMap(NULL)
, m_pSampler(NULL)
, m_pHeightMap( NULL )
, m_pGrassTexture( NULL )
, m_pRockTexture( NULL )
, m_pSnowTexture( NULL )
{
	SetTerrain("Default");
}

Terrain::Terrain( const char * _strTerrain )
: m_nTiles( 10 )
, m_fWidth( 100.0f )
, m_fHeight( 100.0f )
, m_nIndex(0)
, m_nVertex(0)
, m_pVertexBuffer(NULL)
, m_pIndexBuffer(NULL)
, m_pVertexLayout(NULL)
, m_pVertexShader(NULL)
, m_pHullShader(NULL)
, m_pDomainShader(NULL)
, m_pPixelShader(NULL)
, m_pNormalMap(NULL)
, m_pSampler(NULL)
, m_pHeightMap( NULL )
, m_pGrassTexture( NULL )
, m_pRockTexture( NULL )
, m_pSnowTexture( NULL )
{
	SetTerrain(_strTerrain);
}
Terrain::~Terrain()
{
}

HRESULT Terrain::Create()
{
	D_RETURN( Node::Create() );

	// Load Textures
	{
		char strTerrain[64];
		strcpy_s(strTerrain, TERRAIN_PATH);
		strcat_s(strTerrain, m_strTerrain);
		strcat_s(strTerrain, ".xml");
		TiXmlDocument doc( strTerrain );
		bool bLoadOkay = doc.LoadFile();
		if( ! bLoadOkay )
			return E_FAIL;

		TiXmlElement * pElement = doc.FirstChildElement("Size");
		if( pElement )
		{
			if( pElement->QueryFloatAttribute("width", &m_fWidth) == TIXML_NO_ATTRIBUTE )
				return E_FAIL;
			if( pElement->QueryFloatAttribute("height", &m_fHeight) == TIXML_NO_ATTRIBUTE )
				return E_FAIL;
			if( pElement->QueryIntAttribute("tile", &m_nTiles) == TIXML_NO_ATTRIBUTE )
				return E_FAIL;
		}
		else
		{
			return E_FAIL;
		}

		pElement = doc.FirstChildElement("Texture");
		if( pElement )
		{
 			char buf[32];
			std::string strNormalMap, strHeightMap, strGrass, strRock;
			if( pElement->QueryValueAttribute("NormalMap", &strNormalMap) == TIXML_NO_ATTRIBUTE )
				return E_FAIL;
			if( pElement->QueryValueAttribute("HeightMap", &strHeightMap) == TIXML_NO_ATTRIBUTE )
				return E_FAIL;
			if( pElement->QueryValueAttribute("Grass", &strGrass) == TIXML_NO_ATTRIBUTE )
				return E_FAIL;
			if( pElement->QueryValueAttribute("Rock", &strRock) == TIXML_NO_ATTRIBUTE )
				return E_FAIL;

			// Normal Map
			m_pNormalMap = new Texture2D(strNormalMap.c_str());
			D_RETURN( m_pNormalMap->Create() );

			// Height Map
			m_pHeightMap = new Texture2D(strHeightMap.c_str());
			D_RETURN( m_pHeightMap->Create() );

			// Grass Texture
			m_pGrassTexture = new Texture2D(strGrass.c_str());
			D_RETURN( m_pGrassTexture->Create() );

			// Grass Texture
			m_pRockTexture = new Texture2D(strRock.c_str());
			D_RETURN( m_pRockTexture->Create() );
		}
		else
		{
			return E_FAIL;
		}
	}

	// Shader
	D_RETURN( CompileShaders() );

	// Geometry
	D_RETURN( BuildGeometry() );

	// Sampler
	m_pSampler = new Sampler< SamplerLinear >;
	D_RETURN( m_pSampler->Create() );

	return S_OK;
}

HRESULT Terrain::Destroy()
{
	D_RETURN( Node::Destroy() );
	SAFE_DESTROY( m_pVertexBuffer );
	SAFE_DESTROY( m_pIndexBuffer );
	SAFE_DESTROY( m_pVertexLayout );
	SAFE_DESTROY( m_pVertexShader );
	SAFE_DESTROY( m_pHullShader );
	SAFE_DESTROY( m_pDomainShader );
	SAFE_DESTROY( m_pPixelShader );
	SAFE_DESTROY( m_pNormalMap );
	SAFE_DESTROY( m_pHeightMap );
	SAFE_DESTROY( m_pGrassTexture );
	SAFE_DESTROY( m_pRockTexture );
	SAFE_DESTROY( m_pSampler );
	return S_OK;
}

void Terrain::Render( EngineCamera * _pCamera, Light * _pLight )
{
	Node::Render(_pCamera, _pLight);

	// g_pDxDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST );
	g_pDxDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	_pCamera->Set( m_mGlobalMatrix, _pLight );
	m_pIndexBuffer->Bind();
	m_pVertexBuffer->Bind();
	m_pVertexLayout->Bind();
	m_pVertexShader->Bind();
	m_pVertexShader->SetConstantBuffer( 0, _pCamera->GetCameraShaderParamBuffer() );
	m_pVertexShader->SetTexture(0, m_pHeightMap);
	m_pPixelShader->Bind();
	m_pPixelShader->SetConstantBuffer( 0, _pCamera->GetCameraShaderParamBuffer() );
	m_pPixelShader->SetTexture(0, m_pNormalMap);
	m_pPixelShader->SetTexture(1, m_pHeightMap);
	m_pPixelShader->SetTexture(2, m_pGrassTexture);
	m_pPixelShader->SetTexture(3, m_pRockTexture);
	m_pPixelShader->SetSampler(0, m_pSampler);
	g_pDevice->UnbindGeometryShader();
	g_pDevice->DrawIndexed( m_nIndex );
}

bool Terrain::Culling()
{
	return true;
}

HRESULT Terrain::CompileShaders()
{
	// Vertex Shader
	m_pVertexShader = new VertexShader("Terrain.vsh");
	D_RETURN( m_pVertexShader->CompileAndCreate() );

// 	// Hull Shader
// 	m_pHullShader = new HullShader("Terrain.hsh");
// 	D_RETURN( m_pHullShader->CompileAndCreate() );
// 
// 	// Domain Shader
// 	m_pDomainShader = new DomainShader("Terrain.dsh");
// 	D_RETURN( m_pDomainShader->CompileAndCreate() );

	// Pixel Shader
	m_pPixelShader = new PixelShader("Terrain.psh");
	D_RETURN( m_pPixelShader->CompileAndCreate() );

	return S_OK;
}

HRESULT Terrain::BuildGeometry()
{
	if( m_nTiles == 0 )
		return E_FAIL;

	// Vertex Layout
	m_pVertexLayout = new VertexLayout< TerrainTypeVertex >;
	D_RETURN( m_pVertexLayout->Create( m_pVertexShader ) );

	m_nVertex = (m_nTiles+1) * (m_nTiles+1);
	unsigned int nFaces = m_nTiles * m_nTiles;
	m_nIndex = nFaces * 3 * 2;

	unsigned int * IndexData = new unsigned int[ m_nIndex ];

	sVertex * VertexData = new sVertex[ m_nVertex ];

	// Index
	{
		unsigned int uFanceInd = 0;
		for( int i = 0; i < m_nTiles; ++i )
		{
			for( int j = 0; j < m_nTiles; ++j )
			{
				IndexData[uFanceInd++] = j + i * (m_nTiles+1);
				IndexData[uFanceInd++] = j + ( i + 1) * (m_nTiles+1);
				IndexData[uFanceInd++] = (j + 1) + i * (m_nTiles+1);

				IndexData[uFanceInd++] = (j + 1) + (i + 1) * (m_nTiles+1);
				IndexData[uFanceInd++] = (j + 1) + i * (m_nTiles+1);
				IndexData[uFanceInd++] = j + ( i + 1) * (m_nTiles+1);
			}
		}

		m_pIndexBuffer = new IndexBuffer;
		D_RETURN( m_pIndexBuffer->Create( sizeof(unsigned int), m_nIndex, IndexData ) );
	}

	// Vertex Buffer
	{
		for( int i = 0; i <= m_nTiles; ++i )
		{
			for( int j = 0; j <= m_nTiles; ++j )
			{
				VertexData[ i * (m_nTiles+1) + j ].x = -m_fWidth * 0.5f + m_fWidth * i / (float)(m_nTiles+1);
				VertexData[ i * (m_nTiles+1) + j ].y = -m_fHeight * 0.5f + m_fHeight * j / (float)(m_nTiles+1);
				VertexData[ i * (m_nTiles+1) + j ].z = 0.0f;

				VertexData[ i * (m_nTiles+1) + j ].u0 = (float)i / (float)(m_nTiles+1);
				VertexData[ i * (m_nTiles+1) + j ].v0 = (float)j / (float)(m_nTiles+1);

				VertexData[ i * (m_nTiles+1) + j ].u1 = (float)i;
				VertexData[ i * (m_nTiles+1) + j ].v1 = (float)j;


				VertexData[ i * (m_nTiles+1) + j ].tx = 1.0f;
				VertexData[ i * (m_nTiles+1) + j ].ty = 0.0f;
				VertexData[ i * (m_nTiles+1) + j ].tz = 0.0f;

				VertexData[ i * (m_nTiles+1) + j ].nx = 0.0f;
				VertexData[ i * (m_nTiles+1) + j ].ny = 0.0f;
				VertexData[ i * (m_nTiles+1) + j ].nz = 1.0f;

			}

		}

		m_pVertexBuffer = new VertexBuffer;
		D_RETURN( m_pVertexBuffer->Create( sizeof(sVertex), m_nVertex, VertexData ) );
	}

	SAFE_DELETE_ARRAY( VertexData );
	SAFE_DELETE_ARRAY( IndexData );

	return S_OK;
}

void Terrain::SetTerrain( const char * _strHeightMap )
{
	strcpy_s(m_strTerrain, _strHeightMap);
}