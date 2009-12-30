#ifndef _COBJETDECOLLISION_
#define _COBJETDECOLLISION_

#include "Polygonlist.h"
#include "Boundingbox.h"
#include "CalculsCollisions.h"

class CObjetDeCollision
{

protected:
	CPolygonList* m_pPolygonList;
	CPolygonList* m_pPolygonListLowDetail;

public:
	CObjetDeCollision();
	~CObjetDeCollision();

	virtual bool ChargerPolygonList()=0;
	virtual bool ChargerPolygonListLowDetail()=0;
	virtual D3DXMATRIXA16 GetWorldMatrix()=0;

	CPolygonList*   GetPolygonList   ();
	CPolygonList*   GetPolygonListLowDetail();
	CBoundingBox    GetBoundingBox   ();
	CBoundingSphere GetBoundingSphere();

	D3DXVECTOR3 CheckNextPosition( D3DXVECTOR3 CurrentPosition, D3DXVECTOR3 NextPosition,float fAvatarHeight, CCalculsCollisions::eCollisionType p_CollisionType = CCalculsCollisions::COLLISION_AND_SLIDING);

	virtual void Release();

};
#endif //_COBJETDECOLLISION_