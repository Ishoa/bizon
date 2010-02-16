#ifndef _VIEWER_
#define _VIEWER_

#ifndef _ENGINE_DX_
#include "Engine/Engine/EngineDx.h"
#endif

class Node;
class Plan;
class PLYObject;
class Terrain;

class Viewer : public Engine
{
protected:
	Node		* m_pRoot;
	Plan		* m_pPlan;
	PLYObject	* m_pTeapot;
	Terrain		* m_pTerrain;

public:
	Viewer();
	virtual ~Viewer();

	virtual HRESULT Create(HWND _hWnd, HINSTANCE _hInstance, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen = false);
	virtual HRESULT Destroy();
	virtual void	Update();
	virtual void	Render();
};

#endif // _VIEWER_