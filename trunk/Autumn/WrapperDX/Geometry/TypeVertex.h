#ifndef _TYPE_VERTEX_
#define _TYPE_VERTEX_

class TypeVertex
{
protected:
	D3D11_INPUT_ELEMENT_DESC	* m_InputElementDesc;
	unsigned int				  m_iSize;

	void FillDescElement(	unsigned int				_iIndex, 
							const char *				_name, 
							unsigned int				_iSemanticIndex,
							DXGI_FORMAT					_eFormat, 
							unsigned int				_iInputSlot,
							unsigned int				_iAlignedByteOffset,
							D3D11_INPUT_CLASSIFICATION	_eClassification,
							unsigned int				_iInstance);
public:
	TypeVertex();
	virtual ~TypeVertex();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	inline D3D11_INPUT_ELEMENT_DESC *	GetLayout() const { return m_InputElementDesc; }
	inline unsigned int					GetLayoutSize() const { return m_iSize; }

};

#endif // _TYPE_VERTEX_
