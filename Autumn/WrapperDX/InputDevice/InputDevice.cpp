#include "stdafx.h"

#ifndef _INPUT_DEVICE_
#include "WrapperDx/InputDevice/InputDevice.h"
#endif

InputDevice::InputDevice()
: m_pDIN( NULL )
{

}

InputDevice::~InputDevice()
{
}

HRESULT InputDevice::Create(HWND _hWnd, HINSTANCE _hInstance)
{
	m_hWnd = _hWnd;
	m_hInstance = _hInstance;
	D_RETURN( DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIN, NULL) );	

	return S_OK;	
}

HRESULT InputDevice::Destroy()
{
	SAFE_RELEASE(m_pDIN);

	return S_OK;
}