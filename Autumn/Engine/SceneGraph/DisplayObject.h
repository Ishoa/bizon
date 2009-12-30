#ifndef _DISPLAY_OBJECT_
#define _DISPLAY_OBJECT_

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

class DisplayObject : public Node
{
protected:

	unsigned int							m_nIndex;
	unsigned int							m_nVertex;
	VertexBuffer							* m_pVertexBuffer;
	IndexBuffer								* m_pIndexBuffer;
	VertexShader							* m_pVertexShader;
	PixelShader								* m_pPixelShader;
	VertexLayout<DisplayObjectTypeVertex>	* m_pVertexLayout;

 	virtual HRESULT	CompileShaders() = 0;
	virtual HRESULT	BuildGeometry() = 0;


public:
	DisplayObject();
	virtual ~DisplayObject();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual void Render( EngineCamera * _pCamera );
	virtual bool Culling();

};

#endif // _DISPLAY_OBJECT_