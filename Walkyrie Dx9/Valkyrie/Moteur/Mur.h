#ifndef _CMUR_
#define _CMUR_


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Sprites3D.h"
#include "ObjetDeCollision.h"


class CMur :public CObjetDeCollision, public CSprite3D
{

	
public :
	CMur(CScene* pScene);
	~CMur();

	D3DXMATRIXA16 GetWorldMatrix();

	bool ChargerPolygonList();
	bool ChargerPolygonListLowDetail(){m_pPolygonListLowDetail = m_pPolygonList;return true;}
	virtual bool  ChargerSprites3D(LPSTR pFileTexture, float fWidth = 1.0f, float fHeight = 1.0f, float fRepeatTextureX = 1.0f,float fRepeatTextureY = 1.0f);
	virtual void Release();

};

#endif // _CMUR_
