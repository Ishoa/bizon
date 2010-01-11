#ifndef _PIXEL_SHADER_
#define _PIXEL_SHADER_

#define PIXEL_SHADER_ENTRY	"PSMain"

#ifndef _SHADER_
#include "WrapperDX/Shader/Shader.h"
#endif

class PixelShader : public Shader
{
private:
	ID3D11PixelShader * m_pPixelShader;
public:
	PixelShader();
	PixelShader(const char * name);
	~PixelShader();

	virtual const char *	GetEntryPoint();
	virtual const char *	GetShaderVersion();
	virtual HRESULT			Create();
	virtual HRESULT			Destroy();
	virtual void			Bind();
	virtual void			SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer );
	virtual void			SetTexture( unsigned int _iSlot, Texture * _pTexture );
	virtual void			SetSampler( unsigned int _iSlot, SamplerBase * _pSampler );

	inline ID3D11PixelShader * GetPixelShader() { return m_pPixelShader; };
};

#endif // _PIXEL_SHADER_