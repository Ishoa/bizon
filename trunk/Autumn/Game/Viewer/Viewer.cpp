#include "stdafx.h"

#ifndef _VIEWER_
#include "Game/Viewer/Viewer.h"
#endif

Viewer::Viewer()
: m_pRoot(NULL)
, m_pTerrain(NULL)
, m_pSky(NULL)
{

}

Viewer::~Viewer()
{
}

HRESULT Viewer::Create(HWND _hWnd, HINSTANCE _hInstance, unsigned int _uWidth, unsigned int _uHeight, bool _bFullscreen)
{
	LOG_INFOLN("* Viewer");
	D_RETURN(Engine::Create(_hWnd, _hInstance, _uWidth, _uHeight, _bFullscreen));

	m_pRoot = new Node;
	D_RETURN( m_pRoot->Create() );

// 	m_pTeapot = new PLYObject("sphere.ply");
// 	m_pTeapot->SetTexture( "Earth" );
// 	D_RETURN( m_pTeapot->Create() );
// 	m_pRoot->AddChild( m_pTeapot );

	m_pTerrain = new Terrain("Terrain01");
	D_RETURN( m_pTerrain->Create() );
	m_pRoot->AddChild( m_pTerrain );

// 	m_pSky = new Sky;
// 	D_RETURN( m_pSky->Create(m_pCamera) );
// 	m_pRoot->AddChild( m_pSky );

	return S_OK;
}

HRESULT Viewer::Destroy()
{
	D_RETURN( Engine::Destroy() );

	// delete all sub-nodes
	SAFE_DESTROY( m_pRoot );

	return S_OK;
}

void Viewer::Update()
{
	UpdateManager();

// 	Matrix4x4 mMatrix;.
// 	mMatrix.SetTranslation(0,0,3);
// 	float fAngle = GetTimeManager()->GetCurrentTime() * (float)M_PI / 1000.0f;
// 	mMatrix.Rotate(0,0, fAngle );
// 	m_pTeapot->SetLocalMatrix(mMatrix);

	Render();
}


void Viewer::Render()
{
	BeginRender();

	m_pRoot->Render( m_pCamera, m_pLight );

	EndRender();
}

HRESULT Viewer::Resize( unsigned int _uWidth, unsigned int _uHeight )
{
	D_RETURN( Engine::Resize(_uWidth, _uHeight) );

	if( m_pSky && m_pCamera )
		m_pSky->SetParam( m_pCamera->GetFOV(), m_pCamera->GetAspect() );

	return S_OK;
}