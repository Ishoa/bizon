#include "stdafx.h"

#ifndef _SKY_TYPE_VERTEX_
#include "WrapperDX/Geometry/SkyTypeVertex.h"
#endif

SkyTypeVertex::SkyTypeVertex()
: TypeVertex()
{
	m_iSize = 1;
}

SkyTypeVertex::~SkyTypeVertex()
{
}

HRESULT SkyTypeVertex::Create()
{
	D_RETURN( TypeVertex::Create() );

	FillDescElement(0, "POSITION"	, 0, DXGI_FORMAT_R32G32_FLOAT	, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 );

	return S_OK;
}