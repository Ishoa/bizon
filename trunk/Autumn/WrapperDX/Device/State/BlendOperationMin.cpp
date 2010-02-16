#include "stdafx.h"

#ifndef _BLEND_OPERATION_MIN_
#include "WrapperDX/Device/State/BlendOperationMin.h"
#endif

BlendOperationMin::BlendOperationMin()
{

}

BlendOperationMin::~BlendOperationMin()
{

}

D3D11_BLEND_OP BlendOperationMin::GetBlendOperation()
{
	return D3D11_BLEND_OP_MIN;
}

bool BlendOperationMin::IsEnable()
{
	return true;
}
