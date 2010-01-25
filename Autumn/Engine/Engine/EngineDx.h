#ifndef _ENGINE_DX_ 
#define _ENGINE_DX_

#ifndef _MANAGER_
#include "Engine/Manager/Manager.h"
#endif

#ifndef _VIEW_PORT_
#include "WrapperDX/Device/ViewPort.h"
#endif

#ifndef _MATRIX4X4_
#include "Core/Math/Matrix4x4.h"
#endif

class Device3D;
class RenderTarget;
class DepthStencil;
class Manager;
class TimeManager;
class InputManager;
class EngineCamera;
class ScreenText;
class Light;


class Engine
{
protected:

	// Debug Display
	bool					  m_bDisplayText;

	// Fullscreen resolution, windowed resolution
	HWND					  m_hWnd;
	HINSTANCE				  m_hInstance;
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

	// Manager
	Manager					* m_pManager[Manager::eManager_COUNT];

	// Camera
	EngineCamera			* m_pCamera;
	float					m_fSpeedCam;

	// Screen Text
	ScreenText				* m_pScreenText;

	// Light
	Light					* m_pLight;

public:
	Engine();
	virtual ~Engine();

	virtual HRESULT Create(HWND _hWnd, HINSTANCE _hInstance, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen = false);
	virtual HRESULT Destroy();
	virtual void	Update() = 0;

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
	virtual void	UpdateKeyboard();
	virtual void	UpdateMouse();

	// Manager
	void			UpdateManager();
	TimeManager *	GetTimeManager() const;
	InputManager *	GetInputManager() const;

};
#endif // _ENGINE_DX_