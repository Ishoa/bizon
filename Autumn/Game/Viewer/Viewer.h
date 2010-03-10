#ifndef _VIEWER_
#define _VIEWER_

#ifndef _ENGINE_DX_
#include "Engine/Engine/EngineDx.h"
#endif

class Node;
class PLYObject;
class Terrain;
class Sky;

class Viewer : public Engine
{
protected:
	Node		* m_pRoot;
	PLYObject	* m_pTeapot;
	Terrain		* m_pTerrain;
	Sky			* m_pSky;

public:
	Viewer();
	virtual ~Viewer();

	virtual HRESULT Create(HWND _hWnd, HINSTANCE _hInstance, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen = false);
	virtual HRESULT Destroy();
	virtual void	Update();
	virtual void	Render();

	virtual HRESULT Resize(unsigned int _uWidth, unsigned int _uHeight);
};

#endif // _VIEWER_