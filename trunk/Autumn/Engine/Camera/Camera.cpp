#include "stdafx.h"
#include "Camera.h"

#include <math.h>

Camera::Camera()
{
	BuildProjectionMatrix((float)D3DX_PI * 0.25f, 1.0f, 1.0f, 100.0f);
	SetView( Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) );
	BuildViewMatrix();
}

Camera::~Camera()
{
}


HRESULT Camera::Create()
{
	return S_OK;
}

HRESULT Camera::Destroy()
{
	return S_OK;
}

void Camera::SetAspect(float _fAspect)
{
	BuildProjectionMatrix(m_fFov, _fAspect, m_fZNear, m_fZFar);
}

void Camera::BuildProjectionMatrix(float _fFov, float _fAspect, float _fZNear, float _fZFar)
{
	m_fFov = _fFov;
	m_fAspect = _fAspect;
	m_fZNear = _fZNear;
	m_fZFar = _fZFar;
	m_mProjection.BuildProjectionMatrix(m_fFov, m_fAspect, m_fZNear, m_fZFar);
	m_bUsePerspective = true;
}

void Camera::BuildProjectionMatrix(float _fLeft, float _fRight, float _fBottom, float _fTop, float _fZNear, float _fZFar)
{
	m_fZFar = _fZFar;
	m_fZNear = _fZNear;
	m_fLeft = _fLeft;
	m_fRight = _fRight;
	m_fBottom = _fBottom;
	m_fTop = _fTop;
	m_mProjection.BuildProjectionMatrix(m_fLeft, m_fRight, m_fBottom, m_fTop, m_fZNear, m_fZFar);
	m_bUsePerspective = false;
}

void Camera::SetView(Vector3 _Pos, Vector3 _At, Vector3 _Up)
{
	m_vPos	= _Pos;
	m_vAt	= _At;
	m_vUp	= _Up;	
}

void Camera::BuildViewMatrix()
{
	m_mView.BuildViewMatrix(m_vPos, m_vAt, m_vUp);
}

Matrix4x4 Camera::GetViewProj() const
{
	return m_mView * m_mProjection;
}

void Camera::Translate( const Vector3 & _v )
{
	m_vPos += _v;
	m_vAt += _v;

	BuildViewMatrix();
}

void Camera::Update( const TimeInfo & _sTimeInfo )
{
}

Vector3 Camera::GetLooktAtDirection() const
{
	Vector3 vLookat = m_vAt - m_vPos;
	return vLookat.Normalize();
}

Vector3 Camera::GetLateralDirection() const
{
	Vector3 vLat = ( m_vAt - m_vPos ) ^ m_vUp;
	return vLat.Normalize();
}