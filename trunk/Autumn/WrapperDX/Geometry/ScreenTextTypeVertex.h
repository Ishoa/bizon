#ifndef _SCREEN_TEXT_TYPE_VERTEX_
#define _SCREEN_TEXT_TYPE_VERTEX_

#ifndef _TYPE_VERTEX_
#include "WrapperDX/Geometry/TypeVertex.h"
#endif

class ScreenTextTypeVertex : public TypeVertex
{
public:
	ScreenTextTypeVertex();
	virtual ~ScreenTextTypeVertex();

	virtual HRESULT Create();
};

#endif // _SCREEN_TEXT_TYPE_VERTEX_