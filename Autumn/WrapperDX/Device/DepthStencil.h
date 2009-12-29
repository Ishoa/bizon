#ifndef _DEPTH_STENCIL_
#define _DEPTH_STENCIL_

class DepthStencil
{
private:
	ID3D11DepthStencilState * m_pDepthStencilState;
	ID3D11DepthStencilView	* m_pDepthStencilView;
	ID3D11Texture2D			* m_pDepthStencil;
	ID3D11ShaderResourceView* m_pShaderResourceView;

public:
	DepthStencil();
	~DepthStencil();

	HRESULT	Create(unsigned int _uWidth, unsigned int _uHeight);
	HRESULT Destroy();
	HRESULT Resize(unsigned int _uWidth, unsigned int _uHeight);

	inline ID3D11Texture2D			*	GetRenderTargetTexture()	{ return m_pDepthStencil; }
	inline ID3D11DepthStencilView	*	GetDepthStencilView()		{ return m_pDepthStencilView; }
	inline ID3D11DepthStencilState	*	GetDepthStencilState()		{ return m_pDepthStencilState; }
	inline ID3D11ShaderResourceView	*	GetShaderResourceView()		{ return m_pShaderResourceView; }
};

#endif // _DEPTH_STENCIL_