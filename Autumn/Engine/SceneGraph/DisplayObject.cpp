#include "stdafx.h"

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

	// Vertex Shader
	m_pVertexShader = new VertexShader("DisplayObject.vsh");
	D_RETURN( m_pVertexShader->CompileAndCreate() );

	// Pixel Shader
	m_pPixelShader = new PixelShader("DisplayObject.psh");
	D_RETURN( m_pPixelShader->CompileAndCreate() );

	// Vertex Layout
	m_pVertexLayout = new VertexLayout< DisplayObjectTypeVertex >;
	D_RETURN( m_pVertexLayout->Create( m_pVertexShader ) );

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

void DisplayObject::Render()
{
	
}

bool DisplayObject::Culling()
{
	return true;
}