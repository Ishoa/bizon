#ifndef _PLY_OBJECT_
#define _PLY_OBJECT_

#ifndef _DISPLAY_OBJECT_
#include "Engine/SceneGraph/DisplayObject.h"
#endif

class EngineCamera;

class PLYObject : public DisplayObject
{
protected:

	char m_strFilename[32];

	virtual HRESULT	CompileShaders();
	virtual HRESULT	BuildGeometry();

public:
	PLYObject(const char * _strName = "no file");
	virtual ~PLYObject();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual void Render( EngineCamera * _pCamera, Light * _pLight );
	//virtual void RenderShadow();
	virtual bool Culling();

	void SetName(const char * _name);

};

#endif // _PLY_OBJECT_