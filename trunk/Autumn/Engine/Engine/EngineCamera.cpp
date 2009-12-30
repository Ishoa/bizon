#include "stdafx.h"

#ifndef _CONSTANT_BUFFER_UPDATE_
#include "WrapperDX/Buffer/ConstantBufferUpdate.h"
#endif

#ifndef _ENGINE_CAMERA_
#include "Engine/Engine/EngineCamera.h"
#endif

EngineCamera::EngineCamera()
: m_pCameraShaderParamBuffer(NULL)
{

}

EngineCamera::~EngineCamera()
{

}

HRESULT EngineCamera::Create()
{
	D_RETURN( Camera::Create() );

	// Camera Shader Param
	m_sCameraShaderParam.m_mWorldViewProj = GetViewProj();
	m_pCameraShaderParamBuffer = new ConstantBufferUpdate;
	D_RETURN( m_pCameraShaderParamBuffer->Create( sizeof( sCameraShaderParam ), 1, & m_sCameraShaderParam ) );

	return S_OK;
}

HRESULT EngineCamera::Destroy()
{
	D_RETURN( Camera::Destroy() );
	SAFE_DESTROY( m_pCameraShaderParamBuffer );

	return S_OK;
}

void EngineCamera::SetWorld( const Matrix4x4 & _mWorld )
{
	sCameraShaderParam * pShaderParam = (sCameraShaderParam *)m_pCameraShaderParamBuffer->Map();
	pShaderParam->m_mWorldViewProj = _mWorld * GetViewProj();
	m_pCameraShaderParamBuffer->Unmap();
}