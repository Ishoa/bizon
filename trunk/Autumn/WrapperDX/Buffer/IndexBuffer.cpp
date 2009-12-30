#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDx/Device/Device.h"
#endif

#ifndef _INDEXBUFFER_
#include "WrapperDX/Buffer/IndexBuffer.h"
#endif


IndexBuffer::IndexBuffer():Buffer()
{
}

IndexBuffer::~IndexBuffer()
{
}

HRESULT IndexBuffer::Create(unsigned int size, unsigned int nElts, const void * data)
{
	m_uSize	  = size;
	m_uElts	  = nElts;

	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= m_uSize * m_uElts;
	bd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags	= 0;
	bd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = data;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	
	D_RETURN( g_pDxDevice->CreateBuffer( &bd, &InitData, &m_pBuffer ) );

	return S_OK;
}

HRESULT IndexBuffer::Destroy()
{
	return Buffer::Destroy();
}

void IndexBuffer::Bind()
{
	g_pDxDeviceContext->IASetIndexBuffer( m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
}
