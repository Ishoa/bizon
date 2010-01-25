#include "stdafx.h"

#ifndef _TIME_MANAGER_
#include "Engine/Manager/TimeManager.h"
#endif

#ifndef _INPUT_MANAGER_
#include "Engine/Manager/InputManager.h"
#endif

#ifndef _SCREEN_TEXT_
#include "Engine/Engine/ScreenText.h"
#endif

#ifndef _CAMERA_
#include "Engine/Engine/EngineCamera.h"
#endif

#ifndef _LIGHT_
#include "Engine/Light/Light.h"
#endif

#ifndef _ENGINE_DX_
#include "Engine/Engine/EngineDx.h"
#endif

Engine::Engine()
: m_pDevice(NULL)
, m_pDefaultRenderTarget(NULL)
, m_pDefaultDepthStencil(NULL)
, m_pCamera(NULL)
, m_pScreenText(NULL)
, m_pLight(NULL)
, m_bDisplayText(false)
, m_fSpeedCam( 0.05f )
{

}

Engine::~Engine()
{
}

HRESULT Engine::Create(HWND _hWnd, HINSTANCE _hInstance, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen)
{
	// TODO recup taille fullscreen
	m_hWnd = _hWnd;
	m_hInstance = _hInstance;
	unsigned int uWidth, uHeight;
	m_bFullscreen = _bFullscreen;
	m_uFullscreenWidth = 1280;
	m_uFullscreenHeight = 800;
	m_uWindowedWidth = _uWidth;
	m_uWindowedHeight = _uHeight;

	if( m_bFullscreen )
	{
		uWidth = m_uFullscreenWidth;
		uHeight = m_uFullscreenHeight;
	}
	else
	{
		uWidth = m_uWindowedWidth;
		uHeight = m_uWindowedHeight;
	}
	
	// device
	m_pDevice = Device3D::CreateInstance();
	E_RETURN( m_pDevice->Create(_hWnd), "Create Device : " );

	// rendertarget
	m_pDefaultRenderTarget = new RenderTarget;
	E_RETURN( m_pDefaultRenderTarget->Create(), "Create RenderTarget : " );

	// depthstencil
	m_pDefaultDepthStencil = new DepthStencil;
	E_RETURN( m_pDefaultDepthStencil->Create(GetWidth(), GetHeight()), "Create DepthStencil : " );

	// association RenderTarget with the Device
	SetRenderTargets(&m_pDefaultRenderTarget, 1, m_pDefaultDepthStencil);
	ClearRenderTarget(m_pDefaultRenderTarget);
	ClearDepthStencil(m_pDefaultDepthStencil);

	// Viewport
	E_RETURN( m_oDefaultViewPort.Create( 0, 0, (float)GetWidth(), (float)GetHeight(), 0.0f, 1.0f), "Create ViewPort : " );

	SetViewPorts(&m_oDefaultViewPort);


	// Manager
	m_pManager[Manager::eManager_TIME] = new TimeManager;
	E_RETURN( m_pManager[Manager::eManager_TIME]->Create(), "Create TimeManager : " );
	m_pManager[Manager::eManager_INPUT] = new InputManager(_hWnd, _hInstance);
	E_RETURN( m_pManager[Manager::eManager_INPUT]->Create(), "Create InputManager : " );

	// Camera
	m_pCamera = new EngineCamera;
	D_RETURN( m_pCamera->Create() );
	m_pCamera->SetView(Vector3(-25.0f, 10.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	m_pCamera->BuildViewMatrix();
	float fAspect = (float)uWidth / (float)uHeight;
	m_pCamera->BuildProjectionMatrix((float)M_PI * 0.25f, fAspect, 0.1f, 100.0f);

	// Screen Text
	m_pScreenText = new ScreenText;
	E_RETURN( m_pScreenText->Create(), "Create ScreenText : " );

	// Light
	m_pLight = new Light( Color(1,1,1,1), Vector4(-1,1,1,0) );
	E_RETURN( m_pLight->Create(), "Create Light : " );

	
	return S_OK;
}

HRESULT Engine::Destroy()
{
	for(unsigned int i = 0; i < Manager::eManager_COUNT; ++i)
		SAFE_DESTROY( m_pManager[i] );

	SAFE_DESTROY( m_pDevice );
	SAFE_DESTROY( m_pDefaultRenderTarget );
	SAFE_DESTROY( m_pDefaultDepthStencil );
	SAFE_DESTROY( m_pCamera );
	SAFE_DESTROY( m_pScreenText );
	SAFE_DESTROY( m_pLight );
	
	return S_OK;
}

HRESULT Engine::ToggleFullScreen()
{
	m_bFullscreen = ! m_bFullscreen;
	if( m_bFullscreen )
	{
		LOG_INFOLN("- Set fullscreen mode");
		D_RETURN( g_pDevice->Reset(m_uFullscreenWidth, m_uFullscreenHeight, m_bFullscreen) );
	}
	else
	{
		LOG_INFOLN("- Set windowed mode");
		D_RETURN( g_pDevice->Reset(m_uWindowedWidth, m_uWindowedHeight, m_bFullscreen) );
	}

	SetRenderTargets(&m_pDefaultRenderTarget, 1);

	return S_OK;
}

HRESULT Engine::Resize(unsigned int _uWidth, unsigned int _uHeight)
{
	// prevent from reducing window
	if( _uWidth == 0 || _uHeight == 0 )
		return S_OK;

	if( m_bFullscreen )
	{
		m_uFullscreenWidth = _uWidth;
		m_uFullscreenHeight = _uHeight;
	}
	else
	{
		m_uWindowedWidth = _uWidth;
		m_uWindowedHeight = _uHeight;
	}

	SetRenderTargets(NULL, 0, NULL);

	return S_OK;

}

unsigned int Engine::GetWidth()
{
	return ( m_bFullscreen ? m_uFullscreenWidth : m_uWindowedWidth );
}

unsigned int Engine::GetHeight()
{
	return ( m_bFullscreen ? m_uFullscreenHeight : m_uWindowedHeight );
}


//////////////////////////////////////////////////////////////////////////
// render

void Engine::RenderText()
{
	static float fDeltaTime = 0.0f;
	static float fCurrentFrames = 0.0f;
	static unsigned int nFrames = 0;
	nFrames++;
	fDeltaTime += GetTimeManager()->GetDeltaTime();
	if( fDeltaTime > 1000.0f )
	{
		fCurrentFrames = (float)nFrames / fDeltaTime * 1000.0f;
		fDeltaTime = 0.0f;
		nFrames = 0;
	}
	char buf[32];
	sprintf_s(buf, "FPS : %.2f", fCurrentFrames );
	m_pScreenText->DrawText(buf, 0, 0, Color(1.0f, 1.0f, 1.0f, 1.0f));
}

void Engine::BeginRender()
{
	SetRenderTargets(&m_pDefaultRenderTarget, 1, m_pDefaultDepthStencil);
	ClearRenderTarget(m_pDefaultRenderTarget, Color(0,0,0,1));
	ClearDepthStencil(m_pDefaultDepthStencil);

	if( m_bDisplayText )
		m_pScreenText->Begin();
}

void Engine::EndRender()
{
	if( m_bDisplayText )
		m_pScreenText->End();

	g_pDevice->EndRender();
}

//////////////////////////////////////////////////////////////////////////
// input
void Engine::UpdateKeyboard()
{
	// Escape : Quit
	if( GetInputManager()->KeyPressed( DIK_ESCAPE ) )
	{
		if( m_bFullscreen ) 
			ToggleFullScreen();
		PostQuitMessage(0);
	}
	// F1 / Shift + F1 : Show / Hide text
	if( GetInputManager()->KeyPressed( DIK_F1 ) )
	{
		if( GetInputManager()->KeyPressed( DIK_LSHIFT ) )
			m_bDisplayText = false;
		else
			m_bDisplayText = true;
	}
	if( GetInputManager()->KeyPressed( DIK_RCONTROL ) )
	{
		// UP : Rotate Camera
		if( GetInputManager()->KeyPressed( DIK_UPARROW ) )
			m_pCamera->MoveLookAt( Vector2( 0, m_fSpeedCam * GetTimeManager()->GetDeltaTime() ) );
		// DOWN : Rotate Camera
		if( GetInputManager()->KeyPressed( DIK_DOWNARROW ) )
			m_pCamera->MoveLookAt( Vector2( 0, - m_fSpeedCam * GetTimeManager()->GetDeltaTime() ) );
		// LEFT : Rotate Camera
		if( GetInputManager()->KeyPressed( DIK_LEFTARROW ) )
			m_pCamera->MoveLookAt( Vector2( m_fSpeedCam * GetTimeManager()->GetDeltaTime(), 0 ) );
		// RIGHT : Rotate Camera
		if( GetInputManager()->KeyPressed( DIK_RIGHTARROW ) )
			m_pCamera->MoveLookAt( Vector2( - m_fSpeedCam * GetTimeManager()->GetDeltaTime(), 0 ) );
	}
	else
	{
		// UP : Move Camera
		if( GetInputManager()->KeyPressed( DIK_UPARROW ) )
			m_pCamera->MoveForward( m_fSpeedCam * GetTimeManager()->GetDeltaTime() );
		// DOWN : Move Camera
		if( GetInputManager()->KeyPressed( DIK_DOWNARROW ) )
			m_pCamera->MoveBackward( m_fSpeedCam * GetTimeManager()->GetDeltaTime() );
		// LEFT : Move Camera
		if( GetInputManager()->KeyPressed( DIK_LEFTARROW ) )
			m_pCamera->StrafeLeft( m_fSpeedCam * GetTimeManager()->GetDeltaTime() );
		// RIGHT : Move Camera
		if( GetInputManager()->KeyPressed( DIK_RIGHTARROW ) )
			m_pCamera->StrafeRight( m_fSpeedCam * GetTimeManager()->GetDeltaTime() );
	}
}

void Engine::UpdateMouse()
{
	// Left Click
// 	if( GetInputManager()->LeftButtonPressed() )
// 		m_pCamera->MoveLookAt( GetInputManager()->GetMouseDeltaPosition() );
}

//////////////////////////////////////////////////////////////////////////
// Manager Getters

void Engine::UpdateManager()
{
	for(unsigned int i = 0; i < Manager::eManager_COUNT; ++i)
		m_pManager[i]->Update();

	UpdateKeyboard();
	UpdateMouse();
}

TimeManager * Engine::GetTimeManager() const
{
	return (TimeManager *)m_pManager[Manager::eManager_TIME];
}

InputManager * Engine::GetInputManager() const
{
	return (InputManager *)m_pManager[Manager::eManager_INPUT];
}
