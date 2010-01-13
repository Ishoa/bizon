#include "stdafx.h"

#ifndef _LIGHT_
#include "Engine/Light/Light.h"
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

void EngineCamera::Set(const Matrix4x4 & _mWorld, Light * _pLight)
{
	Vector4 CameraDir( m_vAt - m_vPos );
	sCameraShaderParam * pShaderParam = (sCameraShaderParam *)m_pCameraShaderParamBuffer->Map();
	pShaderParam->m_mWorldViewProj	= _mWorld * GetViewProj();
	pShaderParam->m_LightDir		= _pLight->GetDirection() * _mWorld.GetInverse();
	pShaderParam->m_LightColor		= _pLight->GetColor();
	pShaderParam->m_CameraDir		= CameraDir * _mWorld.GetInverse();
	m_pCameraShaderParamBuffer->Unmap();
}