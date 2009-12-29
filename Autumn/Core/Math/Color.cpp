#include "stdafx.h"
#include "Color.h"

Color::Color()
: m_cColor(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Color::Color(float _fRed, float _fGreen, float _fBlue)
: m_cColor( _fRed, _fGreen, _fBlue, 1.0f)
{
}

Color::Color(float _fRed, float _fGreen, float _fBlue, float _fAlpha)
: m_cColor( _fRed, _fGreen, _fBlue, _fAlpha)
{
}

Color::Color(const Color & c)
: m_cColor( c.m_cColor )
{
}

Color::~Color()
{

}

void Color::Set(float _fRed, float _fGreen, float _fBlue, float _fAlpha)
{
	m_cColor.r = _fRed;
	m_cColor.g = _fGreen;
	m_cColor.b = _fBlue;
	m_cColor.a = _fAlpha;
}

Color operator + (const Color & _c1, const Color & _c2)
{
	Color cRes;
	cRes.m_cColor = _c1.m_cColor + _c2.m_cColor;
	return cRes;
}

Color operator - (const Color & _c1, const Color & _c2)
{
	Color cRes;
	cRes.m_cColor = _c1.m_cColor - _c2.m_cColor;
	return cRes;
}

Color operator * (const Color & _c1, const Color & _c2)
{
	return Color(_c1.m_cColor.r * _c2.m_cColor.r, _c1.m_cColor.g * _c2.m_cColor.g, _c1.m_cColor.b * _c2.m_cColor.b, _c1.m_cColor.a * _c2.m_cColor.a);
}

Color operator * (const Color & _c, const float & _f)
{
	Color cRes;
	cRes.m_cColor = _c.m_cColor * _f;
	return cRes;
}