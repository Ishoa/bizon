#include "stdafx.h"

#ifndef _DISPLAY_OBJECT_TYPE_VERTEX_
#include "WrapperDX/Geometry/DisplayObjectTypeVertex.h"
#endif

DisplayObjectTypeVertex::DisplayObjectTypeVertex()
: TypeVertex()
{
	m_iSize = 1;
}

DisplayObjectTypeVertex::~DisplayObjectTypeVertex()
{
}

HRESULT DisplayObjectTypeVertex::Create()
{
	D_RETURN( TypeVertex::Create() );

	FillDescElement(0, "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 );

	return S_OK;
}