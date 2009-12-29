#include "stdafx.h"
#include "Vector1.h"

Vector1::Vector1()
: x(0.0f)
{
}

Vector1::Vector1(const Vector1 & v)
: x(v.x)
{
}

Vector1::Vector1(float _x)
: x(_x)
{
}

Vector1::~Vector1()
{
}