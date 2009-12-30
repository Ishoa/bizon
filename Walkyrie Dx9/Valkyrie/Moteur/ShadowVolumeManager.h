
#ifndef _CDSHADOWVOLUMEMANAGER_
#define _CDSHADOWVOLUMEMANAGER_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"
#include "Scene.h"
#include "Mesh.h"
#include "ShadowVolume.h"

#define MAX_VOLUME 50
#define MAX_LIGHT 3

class CShadowVolume;


class CShadowVolumeManager
{

private:

	LPDIRECT3DDEVICE9   m_pD3DDevice;
	CScene* m_pScene;

	int m_nIndiceShadowVolumes;
	CShadowVolume m_pShadowVolume[MAX_VOLUME];

	D3DXVECTOR3 m_pvlight;

	LPDIRECT3DVERTEXBUFFER9  m_pBigSquareVB;

	bool m_bTwoSidedStencilsAvailable;

public:

	CShadowVolumeManager(CScene* pScene);
	~CShadowVolumeManager();

	void InitShadowVolumeManager(D3DCOLOR color);
	void InitLight(D3DXVECTOR3 vLight );

	HRESULT AddAndBuildShadowVolumes( CMesh* pObject);
	void ReBuildShadowVolume();

	void renderShadowToStencilBuffer( void );
	void renderShadowToScene( void );

	void setTwoSidedStencilsAvailable(bool bTwoSidedStencilsAvailable){m_bTwoSidedStencilsAvailable = bTwoSidedStencilsAvailable;}
	void clearShadowVolumes();
	
	void Reset();
	void RenduShadow();
	void RenduShadowVolume();
	void Release();
};

#endif //__CDSHADOWVOLUMEMANAGER__