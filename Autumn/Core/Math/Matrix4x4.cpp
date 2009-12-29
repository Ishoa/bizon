#include "stdafx.h"
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{

}

Matrix4x4::Matrix4x4(const Matrix4x4 & m)
: m_mMatrix(m.m_mMatrix)
{
}

Matrix4x4::~Matrix4x4()
{

}

void Matrix4x4::SetIdentity()
{
	D3DXMatrixIdentity(&m_mMatrix);
}


float & Matrix4x4::operator () (unsigned int i, unsigned int j)
{
	return m_mMatrix(i, j);
}

float Matrix4x4::operator () (unsigned int i, unsigned int j) const
{
	return m_mMatrix(i, j);
}

Matrix4x4 operator * (const Matrix4x4 & _m1, const Matrix4x4 & _m2)
{
	Matrix4x4 mRes;
	mRes.m_mMatrix = _m1.m_mMatrix * _m2.m_mMatrix;
	return mRes;
}

Vector3 operator*( const Vector3 & _v, const Matrix4x4 & _m )
{
	D3DXVECTOR4 vRes;
	D3DXVec3Transform(&vRes, &_v.GetVector(), &_m.m_mMatrix);
	vRes /= vRes.w;
	return Vector3(vRes.x, vRes.y, vRes.z);
}
void Matrix4x4::BuildProjectionMatrix(float _fFov, float _fAspect, float _fZNear, float _fZFar)
{
	D3DXMatrixPerspectiveFovLH(&m_mMatrix, _fFov, _fAspect, _fZNear, _fZFar);
}

void Matrix4x4::BuildProjectionMatrix(float _fLeft, float _fRight, float _fBottom, float _fTop, float _fZNear, float _fZFar)
{
	D3DXMatrixOrthoOffCenterLH(&m_mMatrix, _fLeft, _fRight, _fBottom, _fTop, _fZNear, _fZFar);
}

void Matrix4x4::BuildViewMatrix(Vector3 _Pos, Vector3 _At, Vector3 _Up)
{
	D3DXMatrixLookAtLH(&m_mMatrix, &_Pos.GetVector(), &_At.GetVector(), &_Up.GetVector());
}

Matrix4x4 Matrix4x4::GetInverse() const
{
	Matrix4x4 mRes;
	D3DXMatrixInverse(&mRes.m_mMatrix, NULL, &m_mMatrix);
	return mRes;
}

void Matrix4x4::SetTranslation( float x, float y, float z )
{
	D3DXMatrixTranslation(&m_mMatrix, x, y, z);
}

void Matrix4x4::SetRotation( float x, float y, float z )
{
	D3DXMatrixRotationYawPitchRoll(&m_mMatrix, x, y, z);
}

void Matrix4x4::SetScaling( float x, float y, float z )
{
	D3DXMatrixScaling(&m_mMatrix, x, y, z);
}

void Matrix4x4::Translate( float x, float y, float z )
{
	D3DXMATRIX mTranslate;
	D3DXMatrixTranslation(&mTranslate, x, y, z);
	m_mMatrix = m_mMatrix * mTranslate;
}

void Matrix4x4::Rotate( float x, float y, float z )
{
	D3DXMATRIX mRotate;
	D3DXMatrixRotationYawPitchRoll(&mRotate, x, y, z);
	m_mMatrix = m_mMatrix * mRotate;
}

void Matrix4x4::Scale( float x, float y, float z )
{
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, x, y, z);
	m_mMatrix = m_mMatrix * mScale;
}