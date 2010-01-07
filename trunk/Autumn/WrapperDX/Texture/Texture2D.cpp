#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDx/Device/Device.h"
#endif

#ifndef _TEXTURE2D_
#include "WrapperDx/Texture/Texture2D.h"
#endif

Texture2D::Texture2D()
: Texture()
, m_pTexture(NULL)
{
	SetName("nofile");
}

Texture2D::Texture2D(const char * _strFilename)
: Texture()
{
	SetName(_strFilename);
}

Texture2D::~Texture2D()
{
}

void Texture2D::SetName(const char * _strFilename)
{
	strcpy_s(m_strFilename, _strFilename);
}

HRESULT Texture2D::Create(bool _bUseTexturePath/* = true*/)
{
	char buf[256];
	if( _bUseTexturePath )
	{
		strcpy_s(buf, TEXTURE_PATH);
		strcat_s(buf, m_strFilename);
	}
	else
	{
		strcpy_s(buf, m_strFilename);
	}
	

	D3DX11_IMAGE_LOAD_INFO LoadInfo;
	LoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	LoadInfo.MiscFlags = 0;
	LoadInfo.FirstMipLevel = 0;
	LoadInfo.MipLevels = 1;

	ID3D11Resource * pResource = NULL;

	if( FAILED(D3DX11CreateTextureFromFile(g_pDxDevice, buf, &LoadInfo, NULL, &pResource, NULL) ) )
	{
		LOG_INFOLN("Texture introuvable : " << m_strFilename);
		return E_FAIL;
	}

 	pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void **)&m_pTexture);
 	D3D11_TEXTURE2D_DESC desc;
 	m_pTexture->GetDesc(&desc);
	m_iHeight = desc.Height;
	m_iWidth = desc.Width;
// 	m_Format = desc.Format;

	// Create the SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC vd;
	vd.Format = desc.Format;
	vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MostDetailedMip = 0;
	vd.Texture2D.MipLevels = desc.MipLevels;
	D_RETURN( g_pDxDevice->CreateShaderResourceView(m_pTexture, &vd, &m_pResourceShaderView) );

	SAFE_RELEASE(pResource);

	return S_OK;
}

HRESULT Texture2D::Destroy()
{
	D_RETURN( Texture::Destroy() );

	SAFE_RELEASE(m_pTexture);

	return S_OK;
}