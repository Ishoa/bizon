#ifndef _CENVCOLLISION_
#define _CENVCOLLISION_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "ObjetDeCollision.h"

#define MAXOBJET 50


class CEnvCollision
{
public :
	enum CTypeDeCheckCollision
	{
		LIST_POLYGON = 0,
		LIST_POLYGON_LOW_DETAIL,
		BOUNDING_BOX,
		BOUNDING_SPHERE

	};

	struct SObjetDeCollision
	{
		CObjetDeCollision* m_pObjetDeCollision;
		int m_nIdObjet;
		float m_fAvatarHeight;
		CTypeDeCheckCollision m_eTypeCheckDeCollision;
		CCalculsCollisions::eCollisionType m_eCollisionType;

	};

private:

	SObjetDeCollision m_pTabCollision[MAXOBJET];
	int m_nNbCollision;

public:

	CEnvCollision();
	~CEnvCollision();

	bool AddObjetDeCollision(CObjetDeCollision* ObjetDeCollision,CTypeDeCheckCollision eTypeCollision = LIST_POLYGON,float fAvatarHeight = 1.0f,CCalculsCollisions::eCollisionType eCollisionType = CCalculsCollisions::COLLISION_ONLY);
	void Release();

	D3DXVECTOR3 CheckNextPosition( D3DXVECTOR3 CurrentPosition,D3DXVECTOR3 NextPosition);



};
#endif //_CENVCOLLISION_