#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDx/Device/Device.h"
#endif

#ifndef _VERTEX_BUFFER_EX_
#include "WrapperDx/Buffer/VertexBufferEx.h"
#endif

VertexBufferEx::VertexBufferEx():VertexBuffer()
{
}

VertexBufferEx::~VertexBufferEx()
{
}

HRESULT VertexBufferEx::Create(unsigned int size, unsigned int nElts, const void * data, bool IsFlaggedStreamOutput)
{
	m_uSize   = size;
	m_uElts   = nElts;

	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DYNAMIC;
	bd.ByteWidth		= size * nElts;
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER | ( IsFlaggedStreamOutput ? D3D11_BIND_STREAM_OUTPUT : NULL );
	bd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags		= 0;

	D_RETURN( g_pDxDevice->CreateBuffer( &bd, NULL, &m_pBuffer ) );

	return S_OK;
}

HRESULT VertexBufferEx::Destroy()
{
	return VertexBuffer::Destroy();
}

void * VertexBufferEx::Map()
{
	D3D11_MAPPED_SUBRESOURCE oMap;
	g_pDxDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &oMap );

	return oMap.pData;
}

void VertexBufferEx::Unmap()
{
	g_pDxDeviceContext->Unmap(m_pBuffer, 0);
}

void VertexBufferEx::Bind()
{
	UINT stride = m_uSize;
	UINT offset = 0;
	g_pDxDeviceContext->IASetVertexBuffers( 0, 1, &m_pBuffer, &stride, &offset );
}