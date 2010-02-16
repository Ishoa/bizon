#ifndef _SCREEN_TEXT_
#define _SCREEN_TEXT_

#ifndef _SCREEN_TEXT_TYPE_VERTEX_
#include "WrapperDX/Geometry/ScreenTextTypeVertex.h"
#endif

#ifndef _BLEND_OPERATION_ADD_
#include "WrapperDX/Device/State/BlendOperationAdd.h"
#endif

#define MAX_CHAR	8192

class Atlas;
class VertexShader;
class PixelShader;
class VertexBufferEx;
template< class ScreenTextTypeVertex >class VertexLayout;
template< class SamplerLinear >class Sampler;
template< class T >class BlendState;

class ScreenText
{
private:
	unsigned int							m_nVertex;
	float									m_tSprite[MAX_CHAR * 8];
	VertexShader							* m_pVertexShader;
	PixelShader								* m_pPixelShader;
	VertexBufferEx							* m_pVertexBuffer;
	VertexLayout< ScreenTextTypeVertex >	* m_pVertexLayout;
	Atlas									* m_pAtlas;
	Sampler< SamplerLinear >				* m_pSampler;
	BlendState< BlendOperationAdd >			* m_pBlendStateAdd;


	void AddCharacter(char _c, unsigned int _Index, unsigned int _x, unsigned int _y, const Color & _rgba);

public:
	ScreenText();
	~ScreenText();

	HRESULT Create();
	HRESULT Destroy();

	void Begin();
	void End();
	HRESULT DrawText(const char * _str, unsigned int _x, unsigned int _y, const Color & _rgba);

};

#endif // _SCREEN_TEXT_