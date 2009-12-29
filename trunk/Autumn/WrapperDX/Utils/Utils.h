#ifndef _UTILS_3D_
#define _UTILS_3D_

#ifndef _COLORD_
#include "Core/Math/Color.h"
#endif

class DepthStencil;
class RenderTarget;
class ViewPort;

//////////////////////////////////////////////////////////////////////////

#define MAX_RENDERTARGETS	8
#define MAX_VIEWPORTS		16

//////////////////////////////////////////////////////////////////////////

enum eRenderTargetFormat
{
	eRenderTargetFormat_R,
	eRenderTargetFormat_RGBA
};

//////////////////////////////////////////////////////////////////////////

// Set ViewPort
void SetViewPorts( ViewPort * pViewPort, unsigned int _iNumVP = 1 );

// Set RenderTarget
void SetRenderTargets( RenderTarget ** _ppRenderTarget, unsigned int _iNumRT = 1, DepthStencil * _pDepthStencil = NULL );
// clear RenderTarget
void ClearRenderTarget( RenderTarget * _pRenderTarget, const Color & _oColor = Color(0.0f, 0.0f, 0.0f, 1.0f) );
// clear depth buffer
void ClearDepthStencil(DepthStencil * _pDS, float _fClearDepth = 1.0f);
// clear depth & stencil buffers
void ClearDepthStencil(DepthStencil * _pDS, float _fClearDepth, unsigned char _uClearStencil);

#endif
