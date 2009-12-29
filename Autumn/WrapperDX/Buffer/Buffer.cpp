#include "stdafx.h"

#ifndef _BUFFER_
#include "WrapperDX/Buffer/Buffer.h"
#endif


Buffer::Buffer()
: m_pBuffer( NULL )
, m_uSize( 0 )
, m_uElts( 0 )
{

}

HRESULT Buffer::Destroy()
{
	SAFE_RELEASE(m_pBuffer);

	return S_OK;
}

Buffer::~Buffer()
{
}
