#include "stdafx.h"
#include "Vector2.h"

Vector2::Vector2()
{
	m_fCoord[0] = 0.0f;
	m_fCoord[1] = 0.0f;
}

Vector2::Vector2(const Vector2 & v)
{
	m_fCoord[0] = v.m_fCoord[0];
	m_fCoord[1] = v.m_fCoord[1];
}

Vector2::Vector2(float _x, float _y)
{

	m_fCoord[0] = _x;
	m_fCoord[1] = _y;

}

Vector2::~Vector2()
{
}