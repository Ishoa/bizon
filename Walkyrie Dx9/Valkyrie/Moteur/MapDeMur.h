#ifndef _MAPMUR_
#define _MAPMUR_


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "CalculsCollisions.h"
#include "Mur.h"
#include "EnvCollision.h"
#include "Scene.h"

class CScene;

class CMapDeMur
{

public:



	struct MUR_FVF
	{
		float m_fWidth;
		float m_fHeight;
		LPSTR m_pTexture;
		D3DXVECTOR3 m_vPosition;
		D3DXVECTOR3 m_vDirection;
		float fRepeatTextureX;
		float fRepeatTextureY;

	};

	LPDIRECT3DDEVICE9 m_pD3DDevice;
	CScene* m_pScene;
	CLPTexture      m_pTextureDefault;

	CMur** m_pMapMur;
	int m_nNbMur;
	float m_fAvatarHeight;



	CMapDeMur(CScene* pScene);
	~CMapDeMur();
	
	bool ChargerMapDeMur(LPSTR pFileTexture,MUR_FVF* MapMur, int nNbMur);
	bool ChargerEnvCollision(CEnvCollision* pEnvCollision);
	void RenduMapDeMur();
	void Release();
};

#endif // _MAPMUR_
