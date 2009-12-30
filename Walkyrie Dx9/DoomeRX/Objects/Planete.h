
#pragma once

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"
#include "Scene.h"


#define D3DFVF_VERTEX_PLANETE ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

class CPlanete
{

public :

	enum MTYPEPLANETE
	{
		SOLEIL = 0,
		TERRE,
		MARS,
		MERCURE,
		VENUS,
		JUPITER,
		SATURN,
		URANUS,
		NEPTUNE,
		PLUTON,
		LUNE
	};

	
	struct VERTEX_PLANETE {

		D3DVECTOR   P;
		D3DVECTOR   N;
		D3DCOLOR    cd;
		float       u1,v1;

	};

	//====================================================================



	MTYPEPLANETE m_eTypePlanete;

	int m_nVERTEXsz;

	// caracteristique de la sphere (ne changer que ndiv)
	int  m_ndiv;
	int  m_nvert;
	int  m_nfac;

	bool m_bBumpMappingMode;
	float m_fTaille;

	float m_fbscale,m_fdscale;

	CScene* m_pScene;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;  // index buffer
	LPDIRECT3DTEXTURE9      m_pTexture; // texture
	LPDIRECT3DTEXTURE9      m_pBumpMap; // bumpmap

	//=====================================================================

	CPlanete();
	CPlanete(CScene* pScene);
	~CPlanete();

	bool Creation(MTYPEPLANETE eTypePlanete, float nTaille,int nDiv, char* NomFichierTexture,char* NomFichierBumpMap);

	char* getDescription(int* taille);

	void augmenterdScale(float fscale);
	void augmenterbScale(float fscale);
	void diminuerdScale(float fscale);
	void diminuerbScale(float fscale);

	virtual void Release();
	virtual void RenduModel();

private :

	inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
	bool CreateBumpmapFromFile( char *fname, LPDIRECT3DTEXTURE9 *pBMP );

};

