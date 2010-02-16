#ifndef _CONSTANT_BUFFER_UPDATE_
#define _CONSTANT_BUFFER_UPDATE_

#ifndef _BUFFER_
#include "WrapperDX/Buffer/ConstantBuffer.h"
#endif

class ConstantBufferUpdate : public ConstantBuffer
{
protected:
	unsigned int m_iSize;

public:
	ConstantBufferUpdate();
	virtual ~ConstantBufferUpdate();

	virtual HRESULT Create(unsigned int size, unsigned int nElts, const void * data, bool IsFlaggedStreamOutput = false);
	virtual HRESULT Destroy();
	void *	Map();
	void	Unmap();
	void	Update(void * _pData, unsigned int size);
};

#endif // _CONSTANT_BUFFER_UPDATE_