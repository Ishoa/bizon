#ifndef _CMUR_
#define _CMUR_


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Sprites3D.h"
#include "ObjetDeCollision.h"


class CMur :public CObjetDeCollision, public CSprite3D
{


public :
	CMur(LPDIRECT3DDEVICE9 pD3DDevice= NULL);
	~CMur();

	D3DXMATRIXA16 GetWorldMatrix();

	bool ChargerPolygonList();
	virtual bool  ChargerSprites3D(LPSTR pFileTexture, float fWidth = 1.0f, float fHeight = 1.0f, float fRepeatTextureX = 1.0f,float fRepeatTextureY = 1.0f);
	virtual void Release();

};

#endif // _CMUR_
