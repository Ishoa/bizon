#ifndef _RENDER_TARGET_
#define _RENDER_TARGET_

#ifndef _UTILS_3D_
#include "WrapperDX/Utils/Utils.h"
#endif

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;

class RenderTarget
{
private:
	ID3D11RenderTargetView		* m_pRenderTargetView;
	ID3D11ShaderResourceView	* m_pShaderResourceView;
	ID3D11Texture2D				* m_pTexture;

	eRenderTargetFormat			m_eFormat;
	unsigned int				m_iWidth;
	unsigned int				m_iHeight;

public:
	// Constructor
	RenderTarget();
	// Destructor
	~RenderTarget();

	// Create RenderTarget
	HRESULT	Create(unsigned int _uWidth, unsigned int _uHeight, eRenderTargetFormat _eFormat);
	// Resize RenderTarget
	HRESULT Resize(unsigned int _uWidth, unsigned int _uHeight);
	// Create RenderTarget from Device's SwapChain 
	HRESULT Create();
	// Resize RenderTarget from Device's SwapChain
	HRESULT Resize();
	// Release and Destroy RenderTarget objects
	HRESULT	Destroy();
	// Retrieve RenderTargetView
	inline ID3D11RenderTargetView * GetRenderTargetView() const { return m_pRenderTargetView; }
};

#endif // _RENDER_TARGET_