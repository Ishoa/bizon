#ifndef _TERRAIN_TYPE_VERTEX_
#define _TERRAIN_TYPE_VERTEX_

#ifndef _TYPE_VERTEX_
#include "WrapperDX/Geometry/TypeVertex.h"
#endif

class TerrainTypeVertex : public TypeVertex
{
public:
	TerrainTypeVertex();
	virtual ~TerrainTypeVertex();

	virtual HRESULT Create();
};

#endif // _TERRAIN_TYPE_VERTEX_