#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDx/Device/Device.h"
#endif

#ifndef _CONSTANT_BUFFER_UPDATE_
#include "ConstantBufferUpdate.h"
#endif


ConstantBufferUpdate::ConstantBufferUpdate()
: ConstantBuffer()
{

}

ConstantBufferUpdate::~ConstantBufferUpdate()
{
}

HRESULT ConstantBufferUpdate::Create(unsigned int size, unsigned int nElts, const void * data)
{
	UNREF_PARAM( nElts );

	m_iSize = size;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = size;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
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

void ConstantBufferUpdate::Update(void * _pData)
{
	D3D11_MAPPED_SUBRESOURCE Map;
	g_pDxDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map );

	CopyMemory(&Map.pData, _pData, m_iSize);

	g_pDxDeviceContext->Unmap(m_pBuffer, 0);
}
