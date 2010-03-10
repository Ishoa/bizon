#ifndef _HULL_SHADER_
#define _HULL_SHADER_

#define HULL_SHADER_ENTRY	"HSMain"

#ifndef _SHADER_
#include "WrapperDX/Shader/Shader.h"
#endif

class HullShader : public Shader
{
private:
	ID3D11HullShader * m_pHullShader;
public:
	HullShader();
	HullShader(const char * name);
	virtual ~HullShader();

	virtual const char *	GetEntryPoint();
	virtual const char *	GetShaderVersion();
	virtual HRESULT			Create();
	virtual HRESULT			Destroy();
	virtual void			Bind();
	virtual void			SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer );
	virtual void			SetTexture( unsigned int _iSlot, Texture * _pTexture );
	virtual void			SetSampler( unsigned int _iSlot, SamplerBase * _pSampler );


	inline ID3D11HullShader * GetVertexShader() { return m_pHullShader; };
};

#endif // _HULL_SHADER_