#include "stdafx.h"

#ifndef _TIME_MANAGER_
#include "Engine/Manager/TimeManager.h"
#endif

#ifndef _SCREEN_TEXT_
#include "Engine/Engine/ScreenText.h"
#endif

#ifndef _CAMERA_
#include "Engine/Engine/EngineCamera.h"
#endif

#ifndef _ENGINE_DX_
#include "Engine/Engine/EngineDx.h"
#endif

Engine::Engine()
: m_pDevice(NULL)
, m_pDefaultRenderTarget(NULL)
, m_pDefaultDepthStencil(NULL)
, m_pTimeManager(NULL)
, m_pCamera(NULL)
{

}

Engine::~Engine()
{
}

HRESULT Engine::Create(HWND _hWnd, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen)
{
	// TODO recup taille fullscreen
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


	// Time Manager
	m_pTimeManager = new TimeManager;
	E_RETURN( m_pTimeManager->Create(), "Create TimeManager : " );

	// Camera
	m_pCamera = new EngineCamera;
	D_RETURN( m_pCamera->Create() );
	m_pCamera->SetView(Vector3(-25.0f, 0.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	m_pCamera->BuildViewMatrix();
	float fAspect = (float)uWidth / (float)uHeight;
	m_pCamera->BuildProjectionMatrix((float)M_PI * 0.25f, fAspect, 0.1f, 100.0f);

	// Screen Text
	m_pScreenText = new ScreenText;
	E_RETURN( m_pScreenText->Create(), "Create ScreenText : " );

	
	return S_OK;
}

HRESULT Engine::Destroy()
{
	SAFE_DESTROY( m_pDevice );
	SAFE_DESTROY( m_pDefaultRenderTarget );
	SAFE_DESTROY( m_pDefaultDepthStencil );
	SAFE_DESTROY( m_pTimeManager );
	SAFE_DESTROY( m_pCamera );
	SAFE_DESTROY( m_pScreenText );
	
	return S_OK;
}

void Engine::Update()
{
	m_pTimeManager->Update();
	
	BeginRender();
	
	static float fDeltaTime = 0.0f;
	static float fTime = 0.0f;
	fDeltaTime += m_pTimeManager->GetDeltaTime();
	if( fDeltaTime > 1000.0f )
	{
		fTime = m_pTimeManager->GetDeltaTime();
		fDeltaTime = 0.0f;
	}
	char buf[32];
	sprintf_s(buf, "FPS : %.2f", 1000.0f / fTime );
	m_pScreenText->DrawText(buf, 0, 0, Color(1.0f, 0.0f, 0.0f, 1.0f));

	Render();
	EndRender();
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

void Engine::BeginRender()
{
	SetRenderTargets(&m_pDefaultRenderTarget, 1, m_pDefaultDepthStencil);
	ClearRenderTarget(m_pDefaultRenderTarget);
	ClearDepthStencil(m_pDefaultDepthStencil);

	m_pScreenText->Begin();
}

void Engine::EndRender()
{
	m_pScreenText->End();

	g_pDevice->EndRender();
}

//////////////////////////////////////////////////////////////////////////
// input
void Engine::OnKeyPressed(int _key)
{
	switch( _key )
	{
	case VK_F2		: ToggleFullScreen(); break;
// 	case VK_LEFT	: m_pCamera->Move(DIRECTION_LEFT); break;
// 	case VK_RIGHT	: m_pCamera->Move(DIRECTION_RIGHT); break;
// 	case VK_DOWN	: m_pCamera->Move(DIRECTION_DOWN); break;
// 	case VK_UP		: m_pCamera->Move(DIRECTION_UP); break;
	case VK_ESCAPE	: if( m_bFullscreen ) ToggleFullScreen(); PostQuitMessage(0); break;
	default : break;
	}
}

void Engine::OnKeyReleased(int _key)
{
	switch( _key )
	{
	default : break;
	}
}

void Engine::UpdateMouse(int x, int y)
{

}

void Engine::OnLButtonDown()
{
}

void Engine::OnRButtonDown()
{
}

void Engine::OnMButtonDown()
{
}

void Engine::OnLButtonUp()
{
}

void Engine::OnRButtonUp()
{
}

void Engine::OnMButtonUp()
{
}