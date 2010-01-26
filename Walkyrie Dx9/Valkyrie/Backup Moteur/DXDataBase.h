#ifndef _CDXDATABASE_
#define _CDXDATABASE_

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <dinput.h>

#include "VariableExtern.h"

#define MAX_TEXTURE			50
#define MAX_MESH			10
#define MAX_VERTEXBUFFER	10
#define MAX_SURFACES		10

class CDXDataBase
{

public:

	struct CDataTexture
	{
		LPDIRECT3DDEVICE9   m_pD3DDevice;
		LPDIRECT3DTEXTURE9	m_pTexture;
		int					m_nNbReference;
		char*				m_pUrl;

	};

	struct CDataMesh
	{
		LPDIRECT3DDEVICE9	m_pD3DDevice;
		LPD3DXMESH			m_pTexture;
		int					m_nNbReference;
		char*				m_pUrl;

	};

private:

	int m_nIndiceDataTexture;
	int m_nIndiceDataMesh;

	CDataTexture*			m_ptabTexture[MAX_TEXTURE];
	CDataMesh*				m_ptabMesh[MAX_MESH];


public:

	CDXDataBase();
	~CDXDataBase();


	/*####################### DATA TEXTURE ############################*/


	LPDIRECT3DTEXTURE9  getInstanceTextureUrl(LPDIRECT3DDEVICE9   pD3DDevice,char * pUrl);
	LPDIRECT3DTEXTURE9  getInstanceTexture(LPDIRECT3DTEXTURE9 pTexture);


	void ReleaseTexture(LPDIRECT3DTEXTURE9	pTexture);




	unsigned int RechercheTextureUrl(LPDIRECT3DDEVICE9   pD3DDevice, char * pUrl);
	unsigned int RechercheTexture(LPDIRECT3DTEXTURE9	pTexture);	

	int getNbTextures();
	int getNbInstanceTexture(LPDIRECT3DTEXTURE9	pTexture);

	void ReleaseDataBase();

	void affichageStatus(); 
};


#endif //_CDXDATABASE_