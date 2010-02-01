#ifndef _DISPLAY_OBJECT_
#define _DISPLAY_OBJECT_

#ifndef _SAMPLER_POINT_
#include "WrapperDX/Texture/Sampler/SamplerPoint.h"
#endif

#ifndef _SAMPLER_LINEAR_
#include "WrapperDX/Texture/Sampler/SamplerLinear.h"
#endif

#ifndef _DISPLAY_OBJECT_TYPE_VERTEX_
#include "WrapperDX/Geometry/DisplayObjectTypeVertex.h"
#endif

#ifndef _NODE_
#include "Engine/SceneGraph/Node.h"
#endif

class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
template< class Vertextype >class VertexLayout;
class EngineCamera;
class Texture2D;
template< class Samplertype >class Sampler;

class DisplayObject : public Node
{
protected:

	unsigned int							m_nIndex;
	unsigned int							m_nVertex;
	char									m_strTextureName[32];
	VertexBuffer							* m_pVertexBuffer;
	IndexBuffer								* m_pIndexBuffer;
	VertexShader							* m_pVertexShader;
	PixelShader								* m_pPixelShader;
	VertexLayout<DisplayObjectTypeVertex>	* m_pVertexLayout;
	Texture2D								* m_pNormalMap;
	Texture2D								* m_pTexture;
	Sampler<SamplerPoint>					* m_pSampler;

 	virtual HRESULT	CompileShaders() = 0;
	virtual HRESULT	BuildGeometry() = 0;


public:
	DisplayObject();
	virtual ~DisplayObject();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual void Render( EngineCamera * _pCamera, Light * _pLight );
	virtual bool Culling();

	virtual void SetTexture(const char * _strTextureName);

};

#endif // _DISPLAY_OBJECT_