#include "stdafx.h"

#ifndef _VIEWER_
#include "Game/Viewer/Viewer.h"
#endif

Viewer::Viewer()
: m_pRoot(NULL)
{

}

Viewer::~Viewer()
{
}

HRESULT Viewer::Create(HWND _hWnd, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen)
{
	LOG_INFOLN("* Viewer");
	D_RETURN(Engine::Create(_hWnd, _uWidth, _uHeight, _bFullscreen));

	m_pRoot = new Node;
	D_RETURN( m_pRoot->Create() );

	m_pPlan = new Plan;
	m_pPlan->Set( 10.0f );
	m_pPlan->SetTexture( "Four" );
	D_RETURN( m_pPlan->Create() );
	m_pRoot->AddChild( m_pPlan );

	m_pTeapot = new PLYObject("sphere.ply");
	m_pTeapot->SetTexture( "Earth" );
	D_RETURN( m_pTeapot->Create() );
	m_pRoot->AddChild( m_pTeapot );

	return S_OK;
}

HRESULT Viewer::Destroy()
{
	D_RETURN( Engine::Destroy() );

	SAFE_DESTROY( m_pRoot );

	return S_OK;
}

void Viewer::Update()
{
	Engine::Update();

	Matrix4x4 mMatrix;
	mMatrix.SetTranslation(0,0,2);
	float fAngle = m_pTimeManager->GetCurrentTime() * (float)M_PI / 1000.0f;
	mMatrix.Rotate(0,0, fAngle );
	m_pTeapot->SetLocalMatrix(mMatrix);
}


void Viewer::Render()
{
	m_pRoot->Render( m_pCamera, m_pLight );
}