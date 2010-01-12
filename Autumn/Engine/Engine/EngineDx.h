#ifndef _ENGINE_DX_ 
#define _ENGINE_DX_


#ifndef _VIEW_PORT_
#include "WrapperDX/Device/ViewPort.h"
#endif

#ifndef _MATRIX4X4_
#include "Core/Math/Matrix4x4.h"
#endif

class Device3D;
class RenderTarget;
class DepthStencil;
class TimeManager;
class EngineCamera;
class ScreenText;


class Engine
{
protected:

	// Debug Display
	bool					  m_bDisplayText;

	// Fullscreen resolution, windowed resolution	
	bool					  m_bFullscreen;
	unsigned int			  m_uFullscreenWidth;
	unsigned int			  m_uFullscreenHeight;
	unsigned int			  m_uWindowedWidth;
	unsigned int			  m_uWindowedHeight;

	// Render
	Device3D				* m_pDevice;
	RenderTarget			* m_pDefaultRenderTarget;
	DepthStencil			* m_pDefaultDepthStencil;
	ViewPort				  m_oDefaultViewPort;

	// Time Manager
	TimeManager				* m_pTimeManager;

	// Camera
	EngineCamera			* m_pCamera;

	// Screen Text
	ScreenText				* m_pScreenText;

public:
	Engine();
	virtual ~Engine();

	virtual HRESULT Create(HWND _hWnd, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen = false);
	virtual HRESULT Destroy();
	virtual void	Update();

	HRESULT ToggleFullScreen();
	HRESULT Resize(unsigned int _uWidth, unsigned int _uHeight);
	unsigned int GetWidth();
	unsigned int GetHeight();

	// render
	virtual void RenderText();
	virtual void BeginRender();
	virtual void Render() = 0;
	virtual void EndRender();

	// input
	virtual void	OnKeyPressed(int _key);
	virtual void	OnKeyReleased(int _key);
	virtual void	UpdateMouse(int x, int y);
	virtual void	OnLButtonDown();
	virtual void	OnRButtonDown();
	virtual void	OnMButtonDown();
	virtual void	OnLButtonUp();
	virtual void	OnRButtonUp();
	virtual void	OnMButtonUp();

};
#endif // _ENGINE_DX_