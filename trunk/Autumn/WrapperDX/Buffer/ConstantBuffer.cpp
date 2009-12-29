#include "stdafx.h"
#include "ConstantBuffer.h"
#include "WrapperDx/Device/Device.h"


ConstantBuffer::ConstantBuffer() : Buffer()
{

}

ConstantBuffer::~ConstantBuffer()
{
}

HRESULT ConstantBuffer::Create(unsigned int size, unsigned int nElts, const void * data)
{
	UNREF_PARAM( nElts );
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = size;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = data;

	D_RETURN( g_pDevice->GetDevice()->CreateBuffer(&cbDesc, &InitData, &m_pBuffer) );

	return S_OK;
}

HRESULT ConstantBuffer::Destroy()
{
	return Buffer::Destroy();
}