#ifndef _BLEND_OPERATION_MIN_
#define _BLEND_OPERATION_MIN_

#include <d3d11.h>

class BlendOperationMin
{
public:
	BlendOperationMin();
	~BlendOperationMin();

	static D3D11_BLEND_OP GetBlendOperation();
	static bool IsEnable();
};

#endif // _BLEND_OPERATION_MIN_