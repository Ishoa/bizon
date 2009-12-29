#ifndef _TEXTURE_
#define _TEXTURE_

class Texture
{
protected:
	ID3D11ShaderResourceView	* m_pResourceShaderView;

public:
	Texture();
	virtual ~Texture();

	virtual HRESULT Create() = 0;
	virtual HRESULT Destroy();
	inline ID3D11ShaderResourceView * GetResourceView() const { return m_pResourceShaderView; }
};

#endif // _TEXTURE_