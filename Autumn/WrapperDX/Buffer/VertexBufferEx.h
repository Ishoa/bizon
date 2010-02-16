#ifndef _VERTEX_BUFFER_EX_
#define _VERTEX_BUFFER_EX_

#ifndef _VERTEX_BUFFER_
#include "WrapperDx/Buffer/VertexBuffer.h"
#endif

class VertexBufferEx : public VertexBuffer
{
public:
	VertexBufferEx();
	virtual ~VertexBufferEx();

	virtual HRESULT Create(unsigned int size, unsigned int nElts, const void * data, bool IsFlaggedStreamOutput = false);
	virtual HRESULT Destroy();

	void	Bind();

	void *	Map();
	void	Unmap();
};

#endif // _VERTEX_BUFFER_EX_