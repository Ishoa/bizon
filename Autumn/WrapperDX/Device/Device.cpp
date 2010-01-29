#include "stdafx.h"
#include "Device.h"

Device3D::Device3D()
: m_pDevice(NULL)
, m_pDeviceContext(NULL)
, m_pSwapChain(NULL)
, m_pOutput(NULL)
, m_hWnd(NULL)
{
}

Device3D::~Device3D()
{
}

HRESULT Device3D::Create(HWND _hWnd, unsigned int _iWidth, unsigned int _iHeight, bool _bFullscreen)
{
	m_hWnd			= _hWnd;
	m_iWidth		= _iWidth;
	m_iHeight		= _iHeight;
	m_bFullscreen	= _bFullscreen;

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory( &SwapChainDesc, sizeof(SwapChainDesc) );
	// DXGI_MODE_DESC
	SwapChainDesc.BufferDesc.Width = m_iWidth;
	SwapChainDesc.BufferDesc.Height = m_iHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// DXGI_SAMPLE_DESC
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	SwapChainDesc.OutputWindow = m_hWnd;
	SwapChainDesc.Windowed = ! m_bFullscreen;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;


#ifdef _DEBUG
	D_RETURN( D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, 0,D3D11_SDK_VERSION, &SwapChainDesc, &m_pSwapChain, &m_pDevice, &m_eMaxFeatureLevel, &m_pDeviceContext ) );	
#else
	D_RETURN( D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,D3D11_SDK_VERSION, &SwapChainDesc, &m_pSwapChain, &m_pDevice, &m_eMaxFeatureLevel, &m_pDeviceContext ) );
#endif

	m_pSwapChain->GetContainingOutput(&m_pOutput);

	return S_OK;
}

HRESULT Device3D::Destroy()
{
	SAFE_RELEASE( m_pDevice );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pDeviceContext );
	SAFE_RELEASE( m_pOutput );

	return S_OK;
}

void Device3D::EndRender()
{
	m_pSwapChain->Present(0,0);
}

HRESULT Device3D::Reset(unsigned int _iWidth, unsigned int _iHeight, bool _bFullscreen)
{
	m_iWidth		= _iWidth;
	m_iHeight		= _iHeight;
	m_bFullscreen	= _bFullscreen;

	m_pSwapChain->SetFullscreenState(_bFullscreen, NULL);

	DXGI_MODE_DESC ModeDesc;
	ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ModeDesc.Height = _iHeight;
	ModeDesc.Width = _iWidth;
	ModeDesc.RefreshRate.Numerator = 60;
	ModeDesc.RefreshRate.Denominator = 1;
	ModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	ModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	return m_pSwapChain->ResizeTarget(&ModeDesc);
}

void Device3D::UnbindVertexShader() const
{
	m_pDeviceContext->VSSetShader( NULL, NULL, 0 );
}

void Device3D::UnbindGeometryShader() const
{
	m_pDeviceContext->GSSetShader( NULL, NULL, 0 );
}

void Device3D::UnbindPixelShader() const
{
	m_pDeviceContext->PSSetShader( NULL, NULL, 0 );
}

HRESULT Device3D::GetMaxResolution( unsigned int & _iWidth, unsigned int & _iHeight )
{
	_iWidth = 0;
	_iHeight = 0;

	if( ! m_pOutput )
		return E_FAIL;
		
	unsigned int num   = 0;
	unsigned int flags = DXGI_ENUM_MODES_SCALING;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	D_RETURN( m_pOutput->GetDisplayModeList( format, flags, &num, 0) );

	if( num == 0 )
		return E_FAIL;
	
	DXGI_MODE_DESC * pDescs = new DXGI_MODE_DESC[num];	
	D_RETURN( m_pOutput->GetDisplayModeList( format, flags, &num, pDescs) );

	unsigned int SizeMax = 0;

	for(unsigned int i = 0; i < num; ++i)
	{
		if( pDescs[i].Width * pDescs[i].Height > SizeMax )
		{
			_iWidth = pDescs[i].Width;
			_iHeight = pDescs[i].Height;
		}
	}

	delete []pDescs;

	return S_OK;
}

HRESULT Device3D::CheckResolution( unsigned int _iWidth, unsigned int _iHeight )
{
	if( ! m_pOutput )
		return E_FAIL;

	unsigned int num   = 0;
	unsigned int flags = DXGI_ENUM_MODES_SCALING;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	D_RETURN( m_pOutput->GetDisplayModeList( format, flags, &num, 0) );

	if( num == 0 )
		return E_FAIL;

	DXGI_MODE_DESC * pDescs = new DXGI_MODE_DESC[num];	
	D_RETURN( m_pOutput->GetDisplayModeList( format, flags, &num, pDescs) );
	
	for(unsigned int i = 0; i < num; ++i)
	{
		if( pDescs[i].Width == _iWidth && pDescs[i].Height == _iHeight )
		{
			delete []pDescs;
			return S_OK;
		}
	}

	delete []pDescs;

	return E_FAIL;
}

HRESULT Device3D::Resize( unsigned int _iWidth, unsigned int _iHeight, bool _bFullscreen )
{
	unsigned int flags = 0;
#ifndef _DEBUG
	if( _bFullscreen )
		flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	m_iWidth		= _iWidth;
	m_iHeight		= _iHeight;
	D_RETURN( m_pSwapChain->ResizeBuffers(1, m_iWidth, m_iHeight, DXGI_FORMAT_R8G8B8A8_UNORM, flags) );

	return S_OK;
}