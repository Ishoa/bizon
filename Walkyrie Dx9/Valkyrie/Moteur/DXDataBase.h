#ifndef _CDXDATABASE_
#define _CDXDATABASE_

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include "VariableGlobale.h"

#define MAX_TEXTURE			100
#define MAX_VERTEXBUFFER	10
#define MAX_SURFACES		10

class CDXDataBase
{

public:
	enum COption
	{
		SANS_OPTION,
		OPTION
	};

	struct CDataTexture
	{
		LPDIRECT3DDEVICE9   m_pD3DDevice;
		LPDIRECT3DTEXTURE9	m_pTexture;
		int					m_nNbReference;
		char*				m_pUrl;
		COption				m_Option;
		UINT				m_Width;
		UINT				m_Height;
		UINT				m_MipLevels;
		DWORD				m_Usage;
		D3DFORMAT			m_Format;
		D3DPOOL				m_Pool;
		DWORD				m_Filter;
		DWORD				m_MipFilter;
		D3DCOLOR			m_ColorKey;
		D3DXIMAGE_INFO *	m_pSrcInfo;
		PALETTEENTRY *		m_pPalette;
	};


private:


	CDataTexture*			m_ptabTexture[MAX_TEXTURE];


public:

	CDXDataBase();
	~CDXDataBase();


	/*####################### DATA TEXTURE ############################*/


	LPDIRECT3DTEXTURE9  getInstanceTextureUrl(LPDIRECT3DDEVICE9   pD3DDevice,char* pUrl);

	LPDIRECT3DTEXTURE9  getInstanceTextureUrlEx(LPDIRECT3DDEVICE9 pD3DDevice,
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
												PALETTEENTRY * pPalette);

	LPDIRECT3DTEXTURE9  getInstanceTexture(LPDIRECT3DTEXTURE9 pTexture);


	void ReleaseTexture(LPDIRECT3DTEXTURE9	pTexture);



	unsigned int RechercheTextureUrl(LPDIRECT3DDEVICE9   pD3DDevice,
									 char* pUrl,
									 COption option = SANS_OPTION,
									 UINT Width = D3DX_DEFAULT,
									 UINT Height = D3DX_DEFAULT,
									 UINT MipLevels = D3DX_DEFAULT,
									 DWORD Usage = NULL,
									 D3DFORMAT Format = D3DFMT_A8R8G8B8,
									 D3DPOOL Pool = D3DPOOL_MANAGED,
									 DWORD Filter = D3DX_DEFAULT,
									 DWORD MipFilter = D3DX_DEFAULT,
									 D3DCOLOR ColorKey = D3DCOLOR_XRGB(255, 255, 255),
									 D3DXIMAGE_INFO * pSrcInfo = NULL,
									 PALETTEENTRY * pPalette = NULL);



	unsigned int RechercheTexture(LPDIRECT3DTEXTURE9	pTexture);	

	int getNbTextures();
	int getNbInstanceTexture(LPDIRECT3DTEXTURE9	pTexture);

	/*####################### GENERAL ############################*/

	void ReleaseDataBase();

	void affichageStatus(); 
};


#endif //_CDXDATABASE_