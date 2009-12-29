#include "stdafx.h"

#ifndef _TEXTURE_
#include "WrapperDx/Texture/Texture.h"
#endif

Texture::Texture()
: m_pResourceShaderView(NULL)
{

}

Texture::~Texture()
{
}

HRESULT Texture::Destroy()
{
	SAFE_RELEASE(m_pResourceShaderView);

	return S_OK;
}