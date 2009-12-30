#ifndef _PLAN_
#define _PLAN_

#ifndef _DISPLAY_OBJECT_
#include "Engine/SceneGraph/DisplayObject.h"
#endif

class EngineCamera;

class Plan : public DisplayObject
{
protected:

	float	m_fSize;

	virtual HRESULT	CompileShaders();
	virtual HRESULT	BuildGeometry();

public:
	Plan();
	virtual ~Plan();

	void Set(const float & _fSize);

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual void Render( EngineCamera * _pCamera );
	//virtual void RenderShadow();
	virtual bool Culling();

};

#endif // _PLAN_