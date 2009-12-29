#ifndef _CONSTANT_BUFFER_
#define _CONSTANT_BUFFER_

#ifndef _BUFFER_
#include "WrapperDX/Buffer/Buffer.h"
#endif

class ConstantBuffer : public Buffer
{
private:

public:
	ConstantBuffer();
	virtual ~ConstantBuffer();

	virtual HRESULT Create(unsigned int size, unsigned int nElts, const void * data);
	virtual HRESULT Destroy();
};

#endif // _CONSTANT_BUFFER_