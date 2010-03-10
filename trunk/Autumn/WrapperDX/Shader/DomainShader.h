#ifndef _DOMAIN_SHADER_
#define _DOMAIN_SHADER_

#define DOMAIN_SHADER_ENTRY	"DSMain"

#ifndef _SHADER_
#include "WrapperDX/Shader/Shader.h"
#endif

class DomainShader : public Shader
{
private:
	ID3D11DomainShader * m_pDomainShader;
public:
	DomainShader();
	DomainShader(const char * name);	
	virtual ~DomainShader();

	virtual const char *	GetEntryPoint();
	virtual const char *	GetShaderVersion();
	virtual HRESULT			Create();
	virtual HRESULT			Destroy();
	virtual void			Bind();
	virtual void			SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer );
	virtual void			SetTexture( unsigned int _iSlot, Texture * _pTexture );
	virtual void			SetSampler( unsigned int _iSlot, SamplerBase * _pSampler );


	inline ID3D11DomainShader * GetVertexShader() { return m_pDomainShader; };
};

#endif // _DOMAIN_SHADER_