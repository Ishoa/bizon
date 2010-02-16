#ifndef _INDEXBUFFER_
#define _INDEXBUFFER_

#ifndef _BUFFER_
#include "WrapperDX/Buffer/Buffer.h"
#endif

class IndexBuffer : public Buffer
{
public:
	IndexBuffer();
	virtual ~IndexBuffer();

	virtual HRESULT Create(unsigned int size, unsigned int nElts, const void * data, bool IsFlaggedStreamOutput = false);
	virtual HRESULT Destroy();
	void	Bind();
};

#endif