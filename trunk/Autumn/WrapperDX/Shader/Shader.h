#ifndef _SHADER_
#define _SHADER_

class ConstantBuffer;
class Texture;
class SamplerBase;

class Shader
{
protected:
	char			m_filename[64];
	ID3DBlob		* m_pBin;
public:
	Shader();
	Shader(const char * name);
	virtual ~Shader();

	virtual const char	*	GetEntryPoint() = 0;
	virtual const char	*	GetShaderVersion() = 0;
	virtual HRESULT			Create() = 0;
	virtual HRESULT			Destroy();
	virtual void			Bind() = 0;

	virtual void SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer ) = 0;
	virtual void SetTexture( unsigned int _iSlot, Texture * _pTexture ) = 0;
	virtual void SetSampler( unsigned int _iSlot, SamplerBase * _pSampler ) = 0;

	inline const void * GetShaderCode() const { if(m_pBin) return m_pBin->GetBufferPointer(); else return NULL; };
	inline SIZE_T		GetShaderSize() const { if(m_pBin) return m_pBin->GetBufferSize(); else return 0; };

	void	SetName(const char * name);
	HRESULT	Compile();
	HRESULT	CompileAndCreate();

};

#endif