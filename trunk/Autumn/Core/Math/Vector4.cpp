#include "stdafx.h"
#include "Core/Math/Vector3.h"
#include "Vector4.h"

Vector4::Vector4()
: m_vVector(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Vector4::Vector4(const Vector4 & v)
: m_vVector( v.m_vVector )
{
}

Vector4::Vector4(const Vector3 & v)
{
	m_vVector.x = v.X();
	m_vVector.y = v.Y();
	m_vVector.z = v.Z();
	m_vVector.w = 1.0f;
}

Vector4::Vector4(float _x, float _y, float _z)
: m_vVector(_x, _y, _z, 1.0f)
{
}

Vector4::Vector4(float _x, float _y, float _z, float _w)
: m_vVector(_x, _y, _z, _w)
{
}

Vector4::~Vector4()
{
}

float Vector4::GetLength() const
{
	return D3DXVec4Length(&m_vVector);
}

Vector4 Vector4::Normalize() const
{
	return Vector4( m_vVector.x / GetLength(), m_vVector.y / GetLength(), m_vVector.z / GetLength(), m_vVector.w / GetLength() );
}