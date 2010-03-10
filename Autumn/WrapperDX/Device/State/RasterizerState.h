#ifndef _RASTERIZER_STATE_
#define _RASTERIZER_STATE_

class RasterizerState
{
private:
	ID3D11RasterizerState * m_pRasterizerState;
public:
	RasterizerState();
	~RasterizerState();

	HRESULT Create( bool _bWireFrame = false, bool _bAntiAliasingEnable = false );
	HRESULT Destroy();

	void Bind();
};

#endif