#include "stdafx.h"

#ifndef _TYPE_VERTEX_
#include "WrapperDX/Geometry/TypeVertex.h"
#endif

TypeVertex::TypeVertex()
: m_InputElementDesc(NULL)
, m_iSize( 0 )
{
}

TypeVertex::~TypeVertex()
{
}

HRESULT TypeVertex::Create()
{
	if( m_iSize > 0 )
	{
		m_InputElementDesc = new D3D11_INPUT_ELEMENT_DESC[m_iSize];

		return S_OK;
	}

	return E_FAIL;
}

HRESULT TypeVertex::Destroy()
{
	SAFE_DELETE_ARRAY( m_InputElementDesc );
	return S_OK;
}

void TypeVertex::FillDescElement( unsigned int _iIndex, const char * _name, unsigned int _iSemanticIndex, DXGI_FORMAT _eFormat, unsigned int _iInputSlot, unsigned int _iAlignedByteOffset, D3D11_INPUT_CLASSIFICATION _eClassification, unsigned int _iInstance )
{
	m_InputElementDesc[ _iIndex ].SemanticName			= _name;
	m_InputElementDesc[ _iIndex ].SemanticIndex			= _iSemanticIndex;
	m_InputElementDesc[ _iIndex ].Format				= _eFormat;
	m_InputElementDesc[ _iIndex ].InputSlot				= _iInputSlot;
	m_InputElementDesc[ _iIndex ].AlignedByteOffset		= _iAlignedByteOffset;
	m_InputElementDesc[ _iIndex ].InputSlotClass		= _eClassification;
	m_InputElementDesc[ _iIndex ].InstanceDataStepRate	= _iInstance;
}