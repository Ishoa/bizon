#ifndef _BLEND_OPERATION_MAX_
#define _BLEND_OPERATION_MAX_

#include <d3d11.h>

class BlendOperationMax
{
public:
	BlendOperationMax();
	~BlendOperationMax();

	static D3D11_BLEND_OP GetBlendOperation();
	static bool IsEnable();
};

#endif // _BLEND_OPERATION_MAX_