#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDx/Device/Device.h"
#endif

#ifndef _VERTEX_BUFFER_
#include "WrapperDx/Buffer/VertexBuffer.h"
#endif

VertexBuffer::VertexBuffer():Buffer()
{
}

VertexBuffer::~VertexBuffer()
{
}

HRESULT VertexBuffer::Create(unsigned int size, unsigned int nElts, const void * data)
{
	m_uSize   = size;
	m_uElts   = nElts;

	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= size * nElts;
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER /*| D3D10_BIND_SHADER_RESOURCE */ ;
	bd.CPUAccessFlags	= 0;
	bd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = data;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;


	D_RETURN( g_pDxDevice->CreateBuffer( &bd, &InitData, &m_pBuffer ) );

	return S_OK;
}

HRESULT VertexBuffer::Destroy()
{
	return Buffer::Destroy();
}


void VertexBuffer::Bind()
{
	UINT stride = m_uSize;
	UINT offset = 0;
	g_pDxDeviceContext->IASetVertexBuffers( 0, 1, &m_pBuffer, &stride, &offset );
}