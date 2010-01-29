#include "stdafx.h"

#ifndef _DEVICE3D_
#include "WrapperDX/Device/Device.h"
#endif

#ifndef _INPUT_DEVICE_
#include "WrapperDX/InputDevice/InputDevice.h"
#endif

#ifndef _MOUSE_
#include "WrapperDx/InputDevice/Mouse.h"
#endif

Mouse::Mouse()
: InputController()
, m_vScreenPosition(0,0)
{

}

Mouse::~Mouse()
{

}

HRESULT Mouse::Create()
{

	D_RETURN( g_pDxInputDevice->CreateDevice(GUID_SysMouse, &m_pInputController, NULL) );
	D_RETURN( m_pInputController->SetDataFormat(&c_dfDIMouse) );
	D_RETURN( m_pInputController->SetCooperativeLevel( g_pInputDevice->GetHandle(), MOUSE_COOPLEVEL) );

	return S_OK;
}

HRESULT Mouse::Poll()
{
	if( m_bDirty )
		m_vDeltaScreenPosition = Vector2(0,0);

	if( ! m_pInputController )
		return E_FAIL;

	ZeroMemory(&m_Data, sizeof(DIMOUSESTATE));

	if ( FAILED( m_pInputController->GetDeviceState(sizeof(DIMOUSESTATE) , &m_Data) ) )
	{
		if( m_pInputController->Acquire() == DIERR_INPUTLOST )
		{
			while( m_pInputController->Acquire() == DIERR_INPUTLOST );

			m_pInputController->GetDeviceState(sizeof(DIMOUSESTATE), &m_Data);
		}
		else
		{
			return E_FAIL;
		}
	}

	m_bDirty = true;

	return S_OK;
}

int Mouse::LeftClick() const
{
	return (m_Data.rgbButtons[0] & 0x80);
}

int Mouse::RightClick() const
{
	return (m_Data.rgbButtons[1] & 0x80);
}

const Vector2 & Mouse::GetPosition() const
{
	return m_vScreenPosition;
}

long Mouse::GetWheel() const
{
	return m_Data.lZ;
}

const Vector2 &  Mouse::GetDeltaPosition() const
{
	return m_vDeltaScreenPosition;
}

long Mouse::GetDeltaX() const
{
	return m_Data.lX;
}

long Mouse::GetDeltaY() const
{
	return m_Data.lY;
}

void Mouse::SetPosition( int _x, int _y )
{
	Vector2 ScreenPos(  2.0f * ( (float)_x - (float)g_pDevice->GetWidth()  * 0.5f ) / (float)g_pDevice->GetWidth(),
						2.0f * ( (float)( g_pDevice->GetHeight() - _y ) - (float)g_pDevice->GetHeight() * 0.5f ) / (float)g_pDevice->GetHeight() );
	m_vDeltaScreenPosition = ScreenPos - m_vScreenPosition;
	m_vScreenPosition = ScreenPos;
	m_bDirty = false;
}