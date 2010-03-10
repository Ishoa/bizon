#ifndef _DSKY_
#define _DSKY_

#ifndef _NODE_
#include "Engine/SceneGraph/Node.h"
#endif

#ifndef _SKY_TYPE_VERTEX_
#include "WrapperDX/Geometry/SkyTypeVertex.h"
#endif

class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
template< class Vertextype >class VertexLayout;
class EngineCamera;
class ConstantBufferUpdate;

class Sky : public Node
{
protected:
	struct sVertex
	{
		float x, y;
	};

	struct sSkyShaderParam
	{
		float m_fFov;
		float m_fScreenRatio;
		float fUseless1;
		float fUseless2;
	};
	ConstantBufferUpdate	* m_pSkyShaderParamBuffer;
	sSkyShaderParam			m_sSkyShaderParam;

	unsigned int							m_nIndex;
	unsigned int							m_nVertex;
	VertexBuffer							* m_pVertexBuffer;
	IndexBuffer								* m_pIndexBuffer;
	VertexShader							* m_pVertexShader;
	PixelShader								* m_pPixelShader;
	VertexLayout<SkyTypeVertex>				* m_pVertexLayout;


	virtual HRESULT	CompileShaders();
	virtual HRESULT	BuildGeometry();

public:
	Sky();
	virtual ~Sky();

	virtual HRESULT Create(EngineCamera * _pCamera);
	virtual HRESULT Destroy();

	virtual void Render( EngineCamera * _pCamera, Light * _pLight );
	virtual bool Culling();

	void SetParam(float _fFov, float _fRatio);

};

#endif // _DSKY_