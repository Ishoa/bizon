#ifndef _VERTEX_BUFFER_
#define _VERTEX_BUFFER_

#ifndef _BUFFER_
#include "WrapperDX/Buffer/Buffer.h"
#endif

class VertexBuffer : public Buffer
{
public:
	VertexBuffer();
	virtual ~VertexBuffer();

	virtual HRESULT Create(unsigned int size, unsigned int nElts, const void * data);
	virtual HRESULT Destroy();
	
	void	Bind();
};

#endif