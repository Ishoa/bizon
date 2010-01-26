#include "MapDeMur.h"


CMapDeMur::CMapDeMur(LPDIRECT3DDEVICE9 pD3DDevice )
{
	m_pD3DDevice = pD3DDevice;
	m_pTextureDefault = NULL;
	m_pMapMur = NULL;
	m_nNbMur = 0;
	m_fAvatarHeight =  1.2f;


}


bool CMapDeMur::ChargerMapDeMur(LPSTR pFileTexture,MUR_FVF* MapMur, int nNbMur)
{
	m_pMapMur = new CMur[nNbMur];

	m_nNbMur = nNbMur;

	// Si c'est le cas, on tente de charger celui-ci.
	if( FAILED( D3DXCreateTextureFromFile( m_pD3DDevice,
		pFileTexture,
		&m_pTextureDefault ) ) )
	{
		// Si l'opération échoue, on retourne une erreur.
		return false;
	}

	if(m_pMapMur == NULL || MapMur== NULL || m_nNbMur==0)
		return false;

	for(int i=0; i<m_nNbMur; i++)
	{
		m_pMapMur[i] = CMur(m_pD3DDevice);
		if(MapMur[i].fRepeatTextureX == 0)
			MapMur[i].fRepeatTextureX=1.0f;
		if(MapMur[i].fRepeatTextureY == 0)
			MapMur[i].fRepeatTextureY=1.0f;

		m_pMapMur[i].ChargerSprites3D(MapMur[i].m_pTexture,MapMur[i].m_fWidth,MapMur[i].m_fHeight,MapMur[i].fRepeatTextureX,MapMur[i].fRepeatTextureY);
		m_pMapMur[i].SetTranslationXYZ(MapMur[i].m_vPosition.x,MapMur[i].m_vPosition.y,MapMur[i].m_vPosition.z);
		m_pMapMur[i].SetRotationXYZ(MapMur[i].m_vDirection.x,MapMur[i].m_vDirection.y,MapMur[i].m_vDirection.z);
		if(MapMur[i].m_pTexture == NULL) 
			m_pMapMur[i].SetTexture(m_pTextureDefault);
	}
	return true;
}

bool CMapDeMur::ChargerEnvCollision(CEnvCollision* pEnvCollision)
{
	if(pEnvCollision!=NULL)
	{
		for(int i=0; i<m_nNbMur; i++)
		{
			if(!pEnvCollision->AddObjetDeCollision(&m_pMapMur[i],CEnvCollision::LIST_POLYGON,1.2f,CCalculsCollisions::COLLISION_AND_SLIDING))
				return false;
		}
		return true;
	}
	else
		return false;

}

CMapDeMur::~CMapDeMur()
{
	if(m_pMapMur != NULL)
	{
		delete []m_pMapMur;
		m_pMapMur = NULL;
	}
	
}


void CMapDeMur::RenduMapDeMur()
{

	if(m_pMapMur != NULL)
		for(int i=0; i<m_nNbMur; i++)
			m_pMapMur[i].RenderSprite3D(false); 
}


void CMapDeMur::Release()
{
	if(m_pMapMur != NULL)
		for(int i=0; i<m_nNbMur; i++)
			m_pMapMur[i].Release();

}
