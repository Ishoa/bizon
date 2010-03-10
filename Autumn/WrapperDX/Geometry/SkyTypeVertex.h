#ifndef _SKY_TYPE_VERTEX_
#define _SKY_TYPE_VERTEX_

#ifndef _TYPE_VERTEX_
#include "WrapperDX/Geometry/TypeVertex.h"
#endif

class SkyTypeVertex : public TypeVertex
{
public:
	SkyTypeVertex();
	virtual ~SkyTypeVertex();

	virtual HRESULT Create();
};

#endif // _SKY_TYPE_VERTEX_