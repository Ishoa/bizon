#include "stdafx.h"

#ifndef _ENGINE_CAMERA_
#include "Engine/Engine/EngineCamera.h"
#endif

#ifndef _DISPLAY_OBJECT_
#include "Engine/SceneGraph/DisplayObject.h"
#endif

DisplayObject::DisplayObject()
: m_nIndex(0)
, m_nVertex(0)
, m_pVertexBuffer(NULL)
, m_pIndexBuffer(NULL)
, m_pVertexLayout(NULL)
, m_pVertexShader(NULL)
, m_pPixelShader(NULL)
{

}

DisplayObject::~DisplayObject()
{

}

HRESULT DisplayObject::Create()
{
	D_RETURN( Node::Create() );

	return S_OK;
}

HRESULT DisplayObject::Destroy()
{
	D_RETURN( Node::Destroy() );
	SAFE_DESTROY( m_pVertexBuffer );
	SAFE_DESTROY( m_pIndexBuffer );
	SAFE_DESTROY( m_pVertexLayout );
	SAFE_DESTROY( m_pVertexShader );
	SAFE_DESTROY( m_pPixelShader );

	return S_OK;
}

void DisplayObject::Render( EngineCamera * _pCamera )
{
	
}

bool DisplayObject::Culling()
{
	return true;
}