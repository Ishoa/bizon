#ifndef _BLEND_OPERATION_NONE_
#define _BLEND_OPERATION_NONE_

#include <d3d11.h>

class BlendOperationNone
{
public:
	BlendOperationNone();
	~BlendOperationNone();

	static D3D11_BLEND_OP GetBlendOperation();
	static bool IsEnable();
};

#endif // _BLEND_OPERATION_NONE_