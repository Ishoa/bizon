#include "Texture.h"

CLPTexture::CLPTexture()
{
	m_pTexture = NULL;
}

bool CLPTexture::chargerTexture(LPDIRECT3DDEVICE9 pD3DDevice, char* pUrl)
{

	m_pTexture = DxDataBase.getInstanceTextureUrl(pD3DDevice,pUrl);

	if(m_pTexture == NULL)
		return false;

	return true;
}



CLPTexture::~CLPTexture()
{

}

int CLPTexture::getNbInstanceTexture()
{
	return DxDataBase.getNbInstanceTexture(m_pTexture);
}


CLPTexture& CLPTexture::operator= (const CLPTexture& pText)
{
	if(getDXTexture() != pText.getDXTexture())
	{
		Release();
		m_pTexture = DxDataBase.getInstanceTexture(pText.getDXTexture());
	}
	return *this;
}

void CLPTexture::Release()
{
	DxDataBase.ReleaseTexture(m_pTexture);
}