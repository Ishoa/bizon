#ifndef _PLAN_
#define _PLAN_

#ifndef _DISPLAY_OBJECT_
#include "Engine/SceneGraph/DisplayObject.h"
#endif

class Plan : public DisplayObject
{
protected:

	Vector3 m_vUp;
	float	m_fSize;

	virtual HRESULT	CompileShaders();
	virtual HRESULT	BuildGeometry();

public:
	Plan();
	virtual ~Plan();

	void Set(const Vector3 & _vUp, const float & _fSize);

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual void Render();
	//virtual void RenderShadow();
	virtual bool Culling();

};

#endif // _PLAN_