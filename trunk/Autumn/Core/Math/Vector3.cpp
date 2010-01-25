#include "stdafx.h"
#include "Vector3.h"

#include <math.h>

Vector3::Vector3()
: m_vVector(0.0f,0.0f,0.0f)
{
}

Vector3::Vector3(const Vector3 & v)
: m_vVector( v.m_vVector )
{
}

Vector3::Vector3(float _x, float _y, float _z)
: m_vVector(_x, _y, _z)
{
}

Vector3::~Vector3()
{
}

float Vector3::GetLength() const
{
	return D3DXVec3Length(&m_vVector);
}

Vector3 Vector3::Normalize() const
{
	return (*this) / GetLength();
}

Vector3	& Vector3::operator += (const Vector3 & _v)
{
	(*this) = (*this) + _v;
	return (*this);
}

Vector3	& Vector3::operator -= (const Vector3 & _v)
{
	(*this) = (*this) - _v;
	return (*this);
}

Vector3 Vector3::operator-()
{
	return Vector3( - m_vVector.x, - m_vVector.y, - m_vVector.z );
}

Vector3 operator + (const Vector3 & _v1, const Vector3 & _v2)
{
	Vector3 vRes;
	vRes.m_vVector = _v1.m_vVector + _v2.m_vVector;
	return vRes;
}

Vector3 operator - (const Vector3 & _v1, const Vector3 & _v2)
{
	Vector3 vRes;
	vRes.m_vVector = _v1.m_vVector - _v2.m_vVector;
	return vRes;
}

Vector3 operator * (const Vector3 & _v, const float & _f)
{
	Vector3 vRes;
	vRes.m_vVector = _v.m_vVector * _f;
	return vRes;
}

Vector3 operator / (const Vector3 & _v, const float & _f)
{
	Vector3 vRes;
	vRes.m_vVector = _v.m_vVector / _f;
	return vRes;
}

// cross product
Vector3 operator ^ (const Vector3 & _v1, const Vector3 & _v2)
{
	Vector3 vRes;
	D3DXVec3Cross(&vRes.m_vVector, &_v1.m_vVector, &_v2.m_vVector);
	return vRes;
}

// dot product
float operator * (const Vector3 & _v1, const Vector3 & _v2)
{
	return D3DXVec3Dot(&_v1.m_vVector, &_v2.m_vVector);
}