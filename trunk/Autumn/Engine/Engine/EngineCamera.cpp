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
	Vector3 CameraDir( m_vAt - m_vPos );

	sCameraShaderParam * pShaderParam = (sCameraShaderParam *)m_pCameraShaderParamBuffer->Map();
	pShaderParam->m_mWorldViewProj	= _mWorld * GetViewProj();
	pShaderParam->m_mWorld			= _mWorld;
	pShaderParam->m_LightDir		= (_pLight->GetDirection() * _mWorld.GetInverse()).Normalize();
	pShaderParam->m_LightColor		= _pLight->GetColor();
	pShaderParam->m_CameraDir		= (CameraDir * _mWorld.GetInverse()).Normalize();
	m_pCameraShaderParamBuffer->Unmap();
}

void EngineCamera::MoveForward( const float & _fDeltaMove )
{
	Translate(  GetLooktAtDirection() * _fDeltaMove );
}

void EngineCamera::MoveBackward( const float & _fDeltaMove )
{
	Translate(  - GetLooktAtDirection() * _fDeltaMove );
}

void EngineCamera::StrafeLeft( const float & _fDeltaMove )
{
	Translate(  GetLateralDirection() * _fDeltaMove );
}

void EngineCamera::StrafeRight( const float & _fDeltaMove )
{
	Translate(  - GetLateralDirection() * _fDeltaMove );
}

void EngineCamera::MoveLookAt( const Vector2 & _f2DeltaMove )
{
	m_vAt -= GetLateralDirection() * _f2DeltaMove.X() * 40.0f;
	m_vAt += m_vUp * _f2DeltaMove.Y() * 40.0f;
	BuildViewMatrix();
}