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


bool CLPTexture::chargerTextureFromFileEx(LPDIRECT3DDEVICE9 pD3DDevice,
										  char* pSrcFile,
										  UINT Width,
										  UINT Height,
										  UINT MipLevels,
										  DWORD Usage,
										  D3DFORMAT Format,
										  D3DPOOL Pool,
										  DWORD Filter,
										  DWORD MipFilter,
										  D3DCOLOR ColorKey,
										  D3DXIMAGE_INFO * pSrcInfo,
										  PALETTEENTRY * pPalette)
{

	m_pTexture = DxDataBase.getInstanceTextureUrlEx(pD3DDevice,
													pSrcFile,
													Width,
													Height,
													MipLevels,
													Usage,
													Format,
													Pool,
													Filter,
													MipFilter,
													ColorKey,
													pSrcInfo,
													pPalette);

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
	if(m_pTexture !=NULL )
		DxDataBase.ReleaseTexture(m_pTexture);
	m_pTexture = NULL;
}