#ifndef _TERRAIN_
#define _TERRAIN_

#ifndef _NODE_
#include "Engine/SceneGraph/Node.h"
#endif

#ifndef _TERRAIN_TYPE_VERTEX_
#include "WrapperDX/Geometry/TerrainTypeVertex.h"
#endif

class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
template< class Vertextype >class VertexLayout;
class EngineCamera;
class Texture2D;
template< class Samplertype >class Sampler;

class Terrain : public Node
{
protected:
	struct sVertex
	{
		float x, y, z;
		float nx, ny, nz;
		float tx, ty, tz;
		float u0, v0;
		float u1, v1;
	};

	unsigned int							m_nTiles;
	float									m_fSize;
	char									m_strTerrain[32];
	unsigned int							m_nIndex;
	unsigned int							m_nVertex;
	VertexBuffer							* m_pVertexBuffer;
	IndexBuffer								* m_pIndexBuffer;
	VertexShader							* m_pVertexShader;
	PixelShader								* m_pPixelShader;
	VertexLayout<TerrainTypeVertex>			* m_pVertexLayout;
	Texture2D								* m_pNormalMap;
	Texture2D								* m_pGrassTexture;
	Texture2D								* m_pRockTexture;
	Texture2D								* m_pSnowTexture;
	Texture2D								* m_pHeightMap;
	Sampler<SamplerLinear>					* m_pSampler;


	virtual HRESULT	CompileShaders();
	virtual HRESULT	BuildGeometry();

public:
	Terrain();
	Terrain(const char * _strTerrain);
	virtual ~Terrain();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual void Render( EngineCamera * _pCamera, Light * _pLight );
	virtual bool Culling();

	virtual void SetTerrain(const char * _strTerrain);
};

#endif // _TERRAIN_