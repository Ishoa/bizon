#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDx/Device/Device.h"
#endif

#ifndef _CONSTANT_BUFFER_UPDATE_
#include "WrapperDx/Buffer/ConstantBufferUpdate.h"
#endif


ConstantBufferUpdate::ConstantBufferUpdate()
: ConstantBuffer()
{

}

ConstantBufferUpdate::~ConstantBufferUpdate()
{
}

HRESULT ConstantBufferUpdate::Create(unsigned int size, unsigned int nElts, const void * data, bool IsFlaggedStreamOutput)
{
	UNREF_PARAM( nElts );

	m_iSize = size;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = size;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER | ( IsFlaggedStreamOutput ? D3D11_BIND_STREAM_OUTPUT : NULL );
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = data;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	D_RETURN( g_pDxDevice->CreateBuffer(&cbDesc, &InitData, &m_pBuffer) );

	return S_OK;
}

HRESULT ConstantBufferUpdate::Destroy()
{
	return Buffer::Destroy();
}

void * ConstantBufferUpdate::Map()
{
	D3D11_MAPPED_SUBRESOURCE oMap;
	g_pDxDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &oMap );

	return oMap.pData;
}

void ConstantBufferUpdate::Unmap()
{
	g_pDxDeviceContext->Unmap(m_pBuffer, 0);
}

void ConstantBufferUpdate::Update(void * _pData, unsigned int size)
{
	D3D11_MAPPED_SUBRESOURCE oMap;
	g_pDxDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &oMap );

	ZeroMemory(&oMap, sizeof(D3D11_MAPPED_SUBRESOURCE));
	CopyMemory(&oMap.pData, _pData, m_iSize);

	g_pDxDeviceContext->Unmap(m_pBuffer, 0);
}
