#include "stdafx.h"

#ifndef _DISPLAY_OBJECT_TYPE_VERTEX_
#include "WrapperDX/Geometry/DisplayObjectTypeVertex.h"
#endif

DisplayObjectTypeVertex::DisplayObjectTypeVertex()
: TypeVertex()
{
	m_iSize = 4;
}

DisplayObjectTypeVertex::~DisplayObjectTypeVertex()
{
}

HRESULT DisplayObjectTypeVertex::Create()
{
	D_RETURN( TypeVertex::Create() );

	FillDescElement(0, "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(1, "NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(2, "TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(3, "TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 );

	return S_OK;
}