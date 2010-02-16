#include "stdafx.h"

#ifndef _BLEND_OPERATION_NONE_
#include "WrapperDX/Device/State/BlendOperationNone.h"
#endif

BlendOperationNone::BlendOperationNone()
{

}

BlendOperationNone::~BlendOperationNone()
{

}

D3D11_BLEND_OP BlendOperationNone::GetBlendOperation()
{
	return D3D11_BLEND_OP_ADD;
}

bool BlendOperationNone::IsEnable()
{
	return false;
}