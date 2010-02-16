#ifndef _GEOMETRY_SHADER_
#define _GEOMETRY_SHADER_

#define GEOMETRY_SHADER_ENTRY	"GSMain"

#ifndef _SHADER_
#include "WrapperDx/Shader/Shader.h"
#endif

class GeometryShader : public Shader
{
private:
	ID3D11GeometryShader	* m_pGeometryShader;

public:
	GeometryShader();
	GeometryShader(const char * name);
	virtual ~GeometryShader();

	virtual const char *	GetEntryPoint();
	virtual const char *	GetShaderVersion();
	virtual HRESULT			Create();
	virtual HRESULT			Destroy();
	virtual void			Bind();
	virtual void			SetConstantBuffer( unsigned int _iSlot, ConstantBuffer * _pBuffer );
	virtual void			SetTexture( unsigned int _iSlot, Texture * _pTexture );
	virtual void			SetSampler( unsigned int _iSlot, SamplerBase * _pSampler );

	inline ID3D11GeometryShader * GetGeometryShader() { return m_pGeometryShader; };

// 	virtual const char * GetTypeShader();
// 
// 	// TODO : remplacer la structure D3D10_SO_DECLARATION_ENTRY
// 	virtual HRESULT Create();
// 	virtual HRESULT CreateWithSO(D3D10_SO_DECLARATION_ENTRY * SOLayout, int nElts);
// 	virtual HRESULT Destroy();
// 	virtual HRESULT CompileAndCreate();
// 	virtual HRESULT CompileAndCreateWithSO(D3D10_SO_DECLARATION_ENTRY * SOLayout, int nElts);
// 	virtual HRESULT CreateSOBuffers(UINT flags, UINT size, int nElts, int num);
// 	virtual void	Bind();
// 
// 	inline ID3D10GeometryShader * GetGeometryShader() const { return m_pGeometryShader; };
// 	inline ID3D10Buffer * GetSOBuffer(int num) const { return m_pSOBuff[num]; };
// 	inline void SetNumSOBuffer(int num) { m_NumBuffers = num; };
// 	inline int GetNumSOBuffer() const { return m_NumBuffers; };
};

#endif // _GEOMETRY_SHADER_