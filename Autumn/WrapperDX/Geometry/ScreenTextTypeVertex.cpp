#include "stdafx.h"

#ifndef _SCREEN_TEXT_TYPE_VERTEX_
#include "WrapperDX/Geometry/ScreenTextTypeVertex.h"
#endif

ScreenTextTypeVertex::ScreenTextTypeVertex()
: TypeVertex()
{
	m_iSize = 3;
}

ScreenTextTypeVertex::~ScreenTextTypeVertex()
{
}

HRESULT ScreenTextTypeVertex::Create()
{
	D_RETURN( TypeVertex::Create() );

	FillDescElement(0, "POSITION"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(1, "COLOR"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0,  8, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	FillDescElement(2, "TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 );

	return S_OK;
}