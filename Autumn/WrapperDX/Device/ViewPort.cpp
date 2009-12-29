#include "stdafx.h"

#ifndef _DEVICE3D_
#include "Device.h"
#endif

#ifndef _VIEW_PORT_
#include "ViewPort.h"
#endif

ViewPort::ViewPort()
{
}

ViewPort::~ViewPort()
{
}

HRESULT ViewPort::Create(float x, float y, float width, float height, float zMin, float zMax)
{
	m_oViewPort.TopLeftX = x;
	m_oViewPort.TopLeftY = y;
	m_oViewPort.Width    = width;
	m_oViewPort.Height   = height;
	m_oViewPort.MinDepth = zMin;
	m_oViewPort.MaxDepth = zMax;

	return S_OK;
}

void ViewPort::Resize(float _uWidth, float _uHeight)
{
	m_oViewPort.Width    = _uWidth;
	m_oViewPort.Height   = _uHeight;
}

HRESULT ViewPort::Destroy()
{
	return S_OK;
}