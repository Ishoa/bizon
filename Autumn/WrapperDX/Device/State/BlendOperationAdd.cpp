#include "stdafx.h"

#ifndef _BLEND_OPERATION_ADD_
#include "WrapperDX/Device/State/BlendOperationAdd.h"
#endif

BlendOperationAdd::BlendOperationAdd()
{

}

BlendOperationAdd::~BlendOperationAdd()
{

}

D3D11_BLEND_OP BlendOperationAdd::GetBlendOperation()
{
	return D3D11_BLEND_OP_ADD;
}


bool BlendOperationAdd::IsEnable()
{
	return true;
}