#include "DXDataBase.h"

CDXDataBase::CDXDataBase()
{


	for(int i = 0;i<MAX_TEXTURE;i++)
		m_ptabTexture[i] = NULL;

	Console<<"constructeur CDXDataBase"<<endl;
}


CDXDataBase::~CDXDataBase()
{

}

/*#######################################################################*/

unsigned int CDXDataBase::RechercheTextureUrl(LPDIRECT3DDEVICE9   pD3DDevice,
								 char* pUrl,
								 COption option,
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

		if(option == SANS_OPTION)
		{
			for(int i = 0;i<MAX_TEXTURE;i++)
			{
				if(m_ptabTexture[i]!= NULL && pD3DDevice == m_ptabTexture[i]->m_pD3DDevice &&strcmp(pUrl,m_ptabTexture[i]->m_pUrl) == 0)
				{
					return i;
				}
			}

		}
		else
		{
			for(int i = 0;i<MAX_TEXTURE;i++)
			{
				if(m_ptabTexture[i] != NULL &&
				   pD3DDevice == m_ptabTexture[i]->m_pD3DDevice		&&
				   strcmp(pUrl,m_ptabTexture[i]->m_pUrl) == 0		&&
				   Width		== m_ptabTexture[i]->m_Width		&&
				   Height		== m_ptabTexture[i]->m_Height		&&
				   MipLevels	== m_ptabTexture[i]->m_MipLevels	&&
				   Usage		== m_ptabTexture[i]->m_Usage		&&
				   Format		== m_ptabTexture[i]->m_Format		&&
				   Pool			== m_ptabTexture[i]->m_Pool			&&
				   Filter		== m_ptabTexture[i]->m_Filter		&&
				   MipFilter	== m_ptabTexture[i]->m_MipFilter	&&
				   ColorKey		== m_ptabTexture[i]->m_ColorKey		&&
				   pSrcInfo		== m_ptabTexture[i]->m_pSrcInfo		&&
				   pPalette		== m_ptabTexture[i]->m_pPalette		
				   )
				{
					return i;
				}
			}
		}
	return -1;
}


unsigned int CDXDataBase::RechercheTexture(LPDIRECT3DTEXTURE9	pTexture)
{
	if(pTexture != NULL)
		for(int i = 0;i<MAX_TEXTURE;i++)
		{
			if(m_ptabTexture[i]!= NULL && pTexture == m_ptabTexture[i]->m_pTexture)
			{
				return i;
			}
		}
	return -1;
}



LPDIRECT3DTEXTURE9  CDXDataBase::getInstanceTexture(LPDIRECT3DTEXTURE9 pTexture)
{

	if(pTexture != NULL)
	{
		unsigned int nIndiceTexture = RechercheTexture(pTexture);
		if(nIndiceTexture != -1)
		{
			m_ptabTexture[nIndiceTexture]->m_nNbReference++;
			return m_ptabTexture[nIndiceTexture]->m_pTexture;
		}	
	}
	return NULL;

}

LPDIRECT3DTEXTURE9  CDXDataBase::getInstanceTextureUrl(LPDIRECT3DDEVICE9   pD3DDevice, char* pUrl)
{
	if(pUrl != NULL && pD3DDevice!= NULL)
	{
		unsigned int nIndiceTexture = RechercheTextureUrl(pD3DDevice, pUrl);
		if(nIndiceTexture != -1)
		{
			m_ptabTexture[nIndiceTexture]->m_nNbReference++;
			return m_ptabTexture[nIndiceTexture]->m_pTexture;

		}
		else
		{
			int m_nIndiceDataTexture = 0;
			while(m_ptabTexture[m_nIndiceDataTexture] != NULL && m_nIndiceDataTexture<=MAX_TEXTURE)
			{
				m_nIndiceDataTexture++;
			}

			if(m_nIndiceDataTexture < MAX_TEXTURE)
			{
				m_ptabTexture[m_nIndiceDataTexture] = new CDataTexture();

				if( FAILED( D3DXCreateTextureFromFile(	pD3DDevice,
														pUrl,
														&m_ptabTexture[m_nIndiceDataTexture]->m_pTexture ) ) )
				{

					delete m_ptabTexture[m_nIndiceDataTexture];
					m_ptabTexture[m_nIndiceDataTexture] = NULL;
					return NULL;
				}
				m_ptabTexture[m_nIndiceDataTexture]->m_Option = SANS_OPTION;
				m_ptabTexture[m_nIndiceDataTexture]->m_pD3DDevice = pD3DDevice;
				m_ptabTexture[m_nIndiceDataTexture]->m_pUrl = new char[strlen(pUrl)];
				strcpy(m_ptabTexture[m_nIndiceDataTexture]->m_pUrl,pUrl);
				m_ptabTexture[m_nIndiceDataTexture]->m_nNbReference = 1;
				
				m_ptabTexture[m_nIndiceDataTexture]->m_Width =	D3DX_DEFAULT;
				m_ptabTexture[m_nIndiceDataTexture]->m_Height =	D3DX_DEFAULT;
				m_ptabTexture[m_nIndiceDataTexture]->m_MipLevels =	D3DX_DEFAULT;
				m_ptabTexture[m_nIndiceDataTexture]->m_Usage =	NULL;
				m_ptabTexture[m_nIndiceDataTexture]->m_Format =	D3DFMT_A8R8G8B8;
				m_ptabTexture[m_nIndiceDataTexture]->m_Pool =	D3DPOOL_MANAGED;	
				m_ptabTexture[m_nIndiceDataTexture]->m_Filter =	D3DX_DEFAULT;
				m_ptabTexture[m_nIndiceDataTexture]->m_MipFilter = D3DX_DEFAULT;
				m_ptabTexture[m_nIndiceDataTexture]->m_ColorKey	= D3DCOLOR_XRGB(255, 255, 255);
				m_ptabTexture[m_nIndiceDataTexture]->m_pSrcInfo	= NULL;
				m_ptabTexture[m_nIndiceDataTexture]->m_pPalette	= NULL;


				m_nIndiceDataTexture++;
				return m_ptabTexture[m_nIndiceDataTexture-1]->m_pTexture;
			}
			else
				return NULL;
		}
	}
	return NULL;
}

LPDIRECT3DTEXTURE9  CDXDataBase::getInstanceTextureUrlEx(LPDIRECT3DDEVICE9 pD3DDevice,
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

	if(pSrcFile != NULL && pD3DDevice!= NULL)
	{
		unsigned int nIndiceTexture = RechercheTextureUrl(pD3DDevice, pSrcFile,OPTION,
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
		if(nIndiceTexture != -1)
		{
			m_ptabTexture[nIndiceTexture]->m_nNbReference++;
			return m_ptabTexture[nIndiceTexture]->m_pTexture;

		}
		else
		{
			int m_nIndiceDataTexture = 0;
			while(m_ptabTexture[m_nIndiceDataTexture] != NULL && m_nIndiceDataTexture<=MAX_TEXTURE)
			{
				m_nIndiceDataTexture++;
			}

			if(m_nIndiceDataTexture < MAX_TEXTURE)
			{

				m_ptabTexture[m_nIndiceDataTexture] = new CDataTexture();
		
				if( FAILED( D3DXCreateTextureFromFileEx(pD3DDevice,
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
														pPalette,
														&m_ptabTexture[m_nIndiceDataTexture]->m_pTexture) ) )
				{

					delete m_ptabTexture[m_nIndiceDataTexture];
					return NULL;
				}
				m_ptabTexture[m_nIndiceDataTexture]->m_Option = OPTION;
				m_ptabTexture[m_nIndiceDataTexture]->m_pD3DDevice = pD3DDevice;
				m_ptabTexture[m_nIndiceDataTexture]->m_pUrl =  pSrcFile;
				m_ptabTexture[m_nIndiceDataTexture]->m_nNbReference = 1;

				m_ptabTexture[m_nIndiceDataTexture]->m_Width =	Width;
				m_ptabTexture[m_nIndiceDataTexture]->m_Height =	Height;
				m_ptabTexture[m_nIndiceDataTexture]->m_MipLevels =	MipLevels;
				m_ptabTexture[m_nIndiceDataTexture]->m_Usage =	Usage;
				m_ptabTexture[m_nIndiceDataTexture]->m_Format =	Format;
				m_ptabTexture[m_nIndiceDataTexture]->m_Pool =	Pool;	
				m_ptabTexture[m_nIndiceDataTexture]->m_Filter =	Filter;
				m_ptabTexture[m_nIndiceDataTexture]->m_MipFilter = MipFilter;
				m_ptabTexture[m_nIndiceDataTexture]->m_ColorKey	= ColorKey;
				m_ptabTexture[m_nIndiceDataTexture]->m_pSrcInfo	= pSrcInfo;
				m_ptabTexture[m_nIndiceDataTexture]->m_pPalette	= pPalette;

				m_nIndiceDataTexture++;
				return m_ptabTexture[m_nIndiceDataTexture-1]->m_pTexture;
			}
			else
				return NULL;
		}
	}
	return NULL;

}


void CDXDataBase::ReleaseTexture(LPDIRECT3DTEXTURE9	pTexture)
{
	unsigned int nIndiceTexture = RechercheTexture(pTexture);
	if( nIndiceTexture != -1)
	{
		int nbref = m_ptabTexture[nIndiceTexture]->m_nNbReference--;
		if(nbref == 0)
		{
			m_ptabTexture[nIndiceTexture]->m_pTexture->Release();
			delete m_ptabTexture[nIndiceTexture]->m_pUrl;
			m_ptabTexture[nIndiceTexture]->m_pD3DDevice = NULL;
			SAFE_DELETE(m_ptabTexture[nIndiceTexture]->m_pSrcInfo);
			SAFE_DELETE(m_ptabTexture[nIndiceTexture]->m_pPalette);
			delete m_ptabTexture[nIndiceTexture];
			m_ptabTexture[nIndiceTexture] = NULL;
		}
	}
}


void CDXDataBase::ReleaseDataBase()
{
	bool bNull;
	for(int i = 0; i<MAX_TEXTURE;i++)
	{
		bNull = m_ptabTexture[i] != NULL;
		if(bNull)
		{
			m_ptabTexture[i]->m_pTexture->Release();
			m_ptabTexture[i]->m_pUrl = NULL;
			m_ptabTexture[i]->m_pD3DDevice = NULL;
			SAFE_DELETE(m_ptabTexture[i]->m_pSrcInfo);
			SAFE_DELETE(m_ptabTexture[i]->m_pPalette);
			delete m_ptabTexture[i];
			m_ptabTexture[i] = NULL;
		}
	}


}


int CDXDataBase::getNbInstanceTexture(LPDIRECT3DTEXTURE9	pTexture)
{
	if(pTexture != NULL)
	{
		unsigned int nIndiceTexture = RechercheTexture(pTexture);
		if( nIndiceTexture != -1)
		{
			return m_ptabTexture[nIndiceTexture]->m_nNbReference;
		}
	}
	return -1;
}


int CDXDataBase::getNbTextures()
{
	int compteur = 0;
	for(int i = 0; i<MAX_TEXTURE; i++)
		if(m_ptabTexture[i] != NULL)
			compteur++;
	return compteur;
}

void CDXDataBase::affichageStatus() 
{
	int nbTexture = 0;
	int nbMesh = 0;

	Console<<endl<<"############# Base de données DX #################"<<endl<<endl;
	Console<<"******************** Textures ***************************"<<endl<<endl;
	for(int i=0;i < MAX_TEXTURE; i++)
	{
	
		if(m_ptabTexture[i] != NULL)
		{
			Console<<"Texture "<<i<<" : \""<<m_ptabTexture[i]->m_pUrl<<"\" reference "<<m_ptabTexture[i]->m_nNbReference<<" fois"<<endl;
			nbTexture++;
		}
	}

	Console<<endl<<"--------------------------------------------------"<<endl<<endl;
	Console<<"La Base de donnée DX comporte "<<nbTexture<<" Textures"<<endl<<endl;
	Console<<"##################################################"<<endl<<endl;
}


