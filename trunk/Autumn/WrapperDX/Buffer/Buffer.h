#ifndef _BUFFER_
#define _BUFFER_

class Buffer
{
protected:
	ID3D11Buffer	* m_pBuffer;
	unsigned int	m_uSize;
	unsigned int	m_uElts;

public:
	Buffer();
	virtual ~Buffer();

	virtual HRESULT Create(unsigned int size, unsigned int nElts, const void * data, bool IsFlaggedStreamOutput = false) = 0;
	virtual HRESULT Destroy();

	inline ID3D11Buffer * GetBuffer() const { return m_pBuffer; }
};

#endif // _BUFFER_