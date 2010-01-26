#include "DXDataBase.h"

CDXDataBase::CDXDataBase()
{

	m_nIndiceDataTexture = 0;
	m_nIndiceDataMesh = 0;

	for(int i = 0;i<MAX_TEXTURE;i++)
		m_ptabTexture[i] = NULL;
	
	Console<<"constructeur CDXDataBase"<<endl;
}


CDXDataBase::~CDXDataBase()
{

}


unsigned int CDXDataBase::RechercheTextureUrl(LPDIRECT3DDEVICE9 pD3DDevice, char * pUrl)
{
	for(int i = 0;i<m_nIndiceDataTexture;i++)
	{
		if(pD3DDevice == m_ptabTexture[i]->m_pD3DDevice && strcmp(pUrl,m_ptabTexture[i]->m_pUrl) == 0)
		{
			return i;
		}
	}
	return -1;
}


unsigned int CDXDataBase::RechercheTexture(LPDIRECT3DTEXTURE9	pTexture)
{
	if(pTexture != NULL)
		for(int i = 0;i<m_nIndiceDataTexture;i++)
		{
			if(pTexture == m_ptabTexture[i]->m_pTexture)
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

LPDIRECT3DTEXTURE9  CDXDataBase::getInstanceTextureUrl(LPDIRECT3DDEVICE9   pD3DDevice, char * pUrl)
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
			if(m_nIndiceDataTexture < MAX_TEXTURE)
			{
				m_ptabTexture[m_nIndiceDataTexture] = new CDataTexture();

				if( FAILED( D3DXCreateTextureFromFile(	pD3DDevice,
														pUrl,
														&m_ptabTexture[m_nIndiceDataTexture]->m_pTexture ) ) )
				{

					delete m_ptabTexture[m_nIndiceDataTexture];
					return NULL;
				}
				m_ptabTexture[m_nIndiceDataTexture]->m_pD3DDevice = pD3DDevice;
				m_ptabTexture[m_nIndiceDataTexture]->m_pUrl =  pUrl;
				m_ptabTexture[m_nIndiceDataTexture]->m_nNbReference = 1;
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
			delete m_ptabTexture[nIndiceTexture];
			m_ptabTexture[nIndiceTexture] = NULL;
		}
	}
}


void CDXDataBase::ReleaseDataBase()
{
	bool bNull;
	for(int i = 0; i++; i<MAX_TEXTURE)
	{
		bNull = m_ptabTexture[i] != NULL;
		if(bNull)
		{
			delete m_ptabTexture[i]->m_pUrl;
			m_ptabTexture[i]->m_pTexture->Release();
			m_ptabTexture[i]->m_pTexture = NULL;
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

	return m_nIndiceDataTexture;
}


void CDXDataBase::affichageStatus() 
{
	int nbTexture = 0;
	for(int i=0;i < MAX_TEXTURE; i++)
	{
	
		if(m_ptabTexture[i] != NULL)
		{
			Console<<"texture "<<i<<" referencé "<<m_ptabTexture[i]->m_nNbReference<<" fois"<<endl;
			nbTexture++;
		}
	}
	Console<<"La Base de donnée DX comporte "<<nbTexture<<" textures"<<endl;
}