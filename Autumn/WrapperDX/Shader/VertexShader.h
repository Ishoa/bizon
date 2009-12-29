#ifndef _VERTEX_SHADER_
#define _VERTEX_SHADER_

#define VERTEX_SHADER_ENTRY	"VSMain"

#ifndef _SHADER_
#include "WrapperDX/Shader/Shader.h"
#endif

class VertexShader : public Shader
{
private:
	ID3D11VertexShader * m_pVertexShader;
public:
	VertexShader();
	VertexShader(const char * name);
	~VertexShader();

	virtual const char *	GetEntryPoint();
	virtual const char *	GetShaderVersion();
	virtual HRESULT			Create();
	virtual HRESULT			Destroy();
	virtual void			Bind();
	virtual void			SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer );
	virtual void			SetTexture( unsigned int _iSlot, Texture * _pTexture );


	inline ID3D11VertexShader * GetVertexShader() { return m_pVertexShader; };
};

#endif // _VERTEX_SHADER_