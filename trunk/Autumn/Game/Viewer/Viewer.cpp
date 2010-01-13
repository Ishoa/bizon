#include "stdafx.h"
#include "Viewer.h"

Viewer::Viewer()
: m_pPlan(NULL)
{

}

Viewer::~Viewer()
{
}

HRESULT Viewer::Create(HWND _hWnd, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen)
{
	LOG_INFOLN("* Viewer");
	D_RETURN(Engine::Create(_hWnd, _uWidth, _uHeight, _bFullscreen));

	m_pPlan = new Plan;
	m_pPlan->Set( 10.0f );
	m_pPlan->SetTexture( "Four" );
	D_RETURN( m_pPlan->Create() );

	return S_OK;
}

HRESULT Viewer::Destroy()
{
	D_RETURN( Engine::Destroy() );

	SAFE_DESTROY( m_pPlan );

	return S_OK;
}

void Viewer::Update()
{
	Engine::Update();
}


void Viewer::Render()
{
	m_pPlan->Render( m_pCamera, m_pLight );
}