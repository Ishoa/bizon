#ifndef _VIEWER_
#define _VIEWER_

#ifndef _ENGINE_DX_
#include "Engine/Engine/EngineDx.h"
#endif

class Plan;

class Viewer : public Engine
{
protected:
	Plan	* m_pPlan;

public:
	Viewer();
	virtual ~Viewer();

	virtual HRESULT Create(HWND _hWnd, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen = false);
	virtual HRESULT Destroy();
	virtual void	Update();
	virtual void	Render();
};

#endif // _VIEWER_