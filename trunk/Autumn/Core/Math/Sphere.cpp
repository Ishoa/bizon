#include "stdafx.h"
#include "Sphere.h"


Sphere::Sphere()
: m_oCenter(0,0,0)
, m_fRadius(0)
{
}

Sphere::Sphere(Vector3 _oCenter, float _fRadius)
: m_oCenter(_oCenter)
, m_fRadius(_fRadius)
{
}

Sphere::~Sphere()
{

}