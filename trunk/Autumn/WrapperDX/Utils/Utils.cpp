#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDX/Device/Device.h"
#endif

#ifndef _DEPTH_STENCIL_
#include "WrapperDX/Device/DepthStencil.h"
#endif

#ifndef _RENDER_TARGET_
#include "WrapperDX/Device/RenderTarget.h"
#endif

#ifndef _VIEW_PORT_
#include "WrapperDX/Device/ViewPort.h"
#endif

#ifndef _UTILS_3D_
#include "WrapperDX/Utils/Utils.h"
#endif

void SetViewPorts( ViewPort * pViewPort, unsigned int _iNumVP/* = 1*/)
{
	D3D11_VIEWPORT tVP[MAX_VIEWPORTS];
	for(unsigned int i = 0; i < _iNumVP; ++i)
		tVP[i] = pViewPort[i].GetViewPort();
	g_pDxDeviceContext->RSSetViewports(_iNumVP, tVP);
}

void SetRenderTargets( RenderTarget ** _ppRenderTarget, unsigned int _iNumRT , DepthStencil * _pDepthStencil /*= NULL*/ )
{
	ID3D11RenderTargetView * pRTV[MAX_RENDERTARGETS];
	for(unsigned int i = 0; i < _iNumRT; ++i)
		pRTV[i] = _ppRenderTarget[i]->GetRenderTargetView();

	ID3D11DepthStencilView * pDSV = ( _pDepthStencil != NULL ? _pDepthStencil->GetDepthStencilView() : NULL );

	g_pDxDeviceContext->OMSetRenderTargets(_iNumRT, pRTV, pDSV);

	if( _pDepthStencil )
		g_pDxDeviceContext->OMSetDepthStencilState(_pDepthStencil->GetDepthStencilState(), 0);

}

void ClearRenderTarget( RenderTarget * _pRenderTarget, const Color & _oColor /*= Color(0.0f, 0.0f, 0.0f, 1.0f) */ )
{
	g_pDxDeviceContext->ClearRenderTargetView(_pRenderTarget->GetRenderTargetView(), _oColor.GetColor() );
}

void ClearDepthStencil(DepthStencil * _pDS, float _fClearDepth)
{
	g_pDxDeviceContext->ClearDepthStencilView(_pDS->GetDepthStencilView(), D3D10_CLEAR_DEPTH, _fClearDepth, 0);
}

void ClearDepthStencil(DepthStencil * _pDS, float _fClearDepth, unsigned char _uClearStencil)
{
	g_pDxDeviceContext->ClearDepthStencilView(_pDS->GetDepthStencilView(), D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, _fClearDepth, _uClearStencil);
}
