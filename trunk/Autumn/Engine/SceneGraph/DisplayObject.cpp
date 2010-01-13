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
, m_pNormalMap(NULL)
, m_pTexture(NULL)
, m_pSampler(NULL)
{

}

DisplayObject::~DisplayObject()
{

}

HRESULT DisplayObject::Create()
{
	D_RETURN( Node::Create() );

	m_pSampler = new Sampler< SamplerLinear >;
	D_RETURN( m_pSampler->Create() );

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
	SAFE_DESTROY( m_pTexture );
	SAFE_DESTROY( m_pNormalMap );
	SAFE_DESTROY( m_pSampler );

	return S_OK;
}

void DisplayObject::Render( EngineCamera * _pCamera, Light * _pLight )
{
	
}

bool DisplayObject::Culling()
{
	return true;
}

void DisplayObject::SetTexture( const char * _strTextureName )
{
	strcpy_s(m_strTextureName, _strTextureName);
}