#ifndef _BLEND_OPERATION_SUB_
#define _BLEND_OPERATION_SUB_

#include <d3d11.h>

class BlendOperationSub
{
public:
	BlendOperationSub();
	~BlendOperationSub();

	static D3D11_BLEND_OP GetBlendOperation();
	static bool IsEnable();
};

#endif // _BLEND_OPERATION_SUB_