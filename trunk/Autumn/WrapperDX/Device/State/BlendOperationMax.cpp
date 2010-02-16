#include "stdafx.h"

#ifndef _BLEND_OPERATION_MAX_
#include "WrapperDX/Device/State/BlendOperationMax.h"
#endif

BlendOperationMax::BlendOperationMax()
{

}

BlendOperationMax::~BlendOperationMax()
{

}

D3D11_BLEND_OP BlendOperationMax::GetBlendOperation()
{
	return D3D11_BLEND_OP_MAX;
}