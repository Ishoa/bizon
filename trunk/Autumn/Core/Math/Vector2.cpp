#include "stdafx.h"
#include "Vector2.h"

Vector2::Vector2()
: m_vVector(0,0)
{
}

Vector2::Vector2(const Vector2 & v)
: m_vVector(v.m_vVector)
{
}

Vector2::Vector2(float _x, float _y)
: m_vVector(_x, _y)
{
}

Vector2::~Vector2()
{
}

Vector2 operator-( const Vector2 & _v1, const Vector2 & _v2 )
{
	Vector2 vRes;
	vRes.m_vVector = _v1.m_vVector - _v2.m_vVector;
	return vRes;
}