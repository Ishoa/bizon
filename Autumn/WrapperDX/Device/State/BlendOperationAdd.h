#ifndef _BLEND_OPERATION_ADD_
#define _BLEND_OPERATION_ADD_

#include <d3d11.h>

class BlendOperationAdd
{
public:
	BlendOperationAdd();
	~BlendOperationAdd();

	static D3D11_BLEND_OP GetBlendOperation();
	static bool IsEnable();
};

#endif // _BLEND_OPERATION_ADD_