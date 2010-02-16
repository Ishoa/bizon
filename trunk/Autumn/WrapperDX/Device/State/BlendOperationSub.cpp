#include "stdafx.h"

#ifndef _BLEND_OPERATION_SUB_
#include "WrapperDX/Device/State/BlendOperationSub.h"
#endif

BlendOperationSub::BlendOperationSub()
{

}

BlendOperationSub::~BlendOperationSub()
{

}

D3D11_BLEND_OP BlendOperationSub::GetBlendOperation()
{
	return D3D11_BLEND_OP_SUBTRACT;
}

bool BlendOperationSub::IsEnable()
{
	return true;
}