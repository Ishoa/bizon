#ifndef _DISPLAY_OBJECT_TYPE_VERTEX_
#define _DISPLAY_OBJECT_TYPE_VERTEX_

#ifndef _TYPE_VERTEX_
#include "WrapperDX/Geometry/TypeVertex.h"
#endif

class DisplayObjectTypeVertex : public TypeVertex
{
public:
	DisplayObjectTypeVertex();
	virtual ~DisplayObjectTypeVertex();

	virtual HRESULT Create();
};

#endif // _DISPLAY_OBJECT_TYPE_VERTEX_