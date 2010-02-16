#include "stdafx.h"

#ifndef _TERRAIN_TYPE_VERTEX_
#include "WrapperDX/Geometry/TerrainTypeVertex.h"
#endif

TerrainTypeVertex::TerrainTypeVertex()
: TypeVertex()
{
	m_iSize = 5;
}

TerrainTypeVertex::~TerrainTypeVertex()
{
}

HRESULT TerrainTypeVertex::Create()
{
	D_RETURN( TypeVertex::Create() );

	FillDescElement(0, "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(1, "NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(2, "TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(3, "TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(4, "TEXCOORD"	, 1, DXGI_FORMAT_R32G32_FLOAT		, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 );

	return S_OK;
}