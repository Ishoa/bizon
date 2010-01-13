#include "stdafx.h"

#ifndef _LIGHT_
#include "Engine/Light/Light.h"
#endif

Light::Light( const Color & _Color /*= Color(1,1,1,1)*/, const Vector4 & _Direction /*= Vector4(0,0,0,1)*/ )
: m_oColor(_Color)
, m_Direction(_Direction)
{

}

Light::~Light()
{
}

HRESULT Light::Create()
{
	return S_OK;
}

HRESULT Light::Destroy()
{
	return S_OK;
}