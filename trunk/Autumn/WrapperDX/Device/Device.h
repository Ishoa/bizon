#ifndef _DEVICE3D_
#define _DEVICE3D_

#include <windows.h>
#include <stdio.h>

#ifndef _SINGLETON_
#include "Core/Pattern/Singleton.h"
#endif

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

class Device3D : public Singleton< Device3D >
{	
private:
	HWND					  m_hWnd;

	// DX11
 	ID3D11Device			* m_pDevice;
 	ID3D11DeviceContext		* m_pDeviceContext;
	IDXGISwapChain			* m_pSwapChain;
	IDXGIOutput				* m_pOutput;
	D3D_FEATURE_LEVEL		m_eMaxFeatureLevel;

	unsigned int	m_iWidth;
	unsigned int	m_iHeight;
	bool			m_bFullscreen;

public:
	// Constructor
	Device3D();
	// Destructor
	~Device3D();

	// Create Device
	HRESULT Create(HWND _hWnd, unsigned int _iWidth = 800, unsigned int _iHeight = 600, bool _bFullscreen = false);
	// Destroy Device objects
	HRESULT Destroy();
	// Swap the buffers
	void EndRender();

	// Reset SwapChain
	HRESULT Reset(unsigned int _iWidth, unsigned int _iHeight, bool _bFullscreen);
	// Resize SwapChain
	HRESULT Resize(unsigned int _iWidth, unsigned int _iHeight, bool _bFullscreen);

	// retrieve max resolution -> fullscreen
	HRESULT GetMaxResolution(unsigned int & _iWidth, unsigned int & _iHeight);

	// check resolution compatible
	HRESULT CheckResolution(unsigned int _iWidth, unsigned int _iHeight);

	// Retrieve Directx 11 Device
	inline ID3D11Device			* GetDevice()			const { return m_pDevice; }
	// Retrieve Directx 11 Device Context
	inline ID3D11DeviceContext	* GetDeviceContext()	const { return m_pDeviceContext; }
	// Retrieve the Handle to the window
	inline HWND					GetHandle()				const { return m_hWnd; }
	// Retrieve SwapChain
	inline IDXGISwapChain		* GetSwapChain()		const { return m_pSwapChain; }
	// retrieve max feature level
	inline D3D_FEATURE_LEVEL	GetMaxFeatureLevel()	const { return m_eMaxFeatureLevel; }
	// retrieve device's width
	inline unsigned int			GetWidth()				const { return m_iWidth; }
	// retrieve device's height
	inline unsigned int			GetHeight()				const { return m_iHeight; }
	// Unbind VertexShader
	void UnbindVertexShader() const;
	// Unbind GeometryShader
	void UnbindGeometryShader() const;
	// Unbind PixelShader
	void UnbindPixelShader() const;
};

#define g_pDxDevice			Device3D::GetInstance()->GetDevice()
#define g_pDxDeviceContext	Device3D::GetInstance()->GetDeviceContext()
#define g_pDevice			Device3D::GetInstance()

#endif