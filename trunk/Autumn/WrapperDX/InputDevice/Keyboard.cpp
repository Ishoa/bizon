#include "stdafx.h"

#ifndef _INPUT_DEVICE_
#include "WrapperDX/InputDevice/InputDevice.h"
#endif

#ifndef _KEYBOARD_
#include "WrapperDx/InputDevice/Keyboard.h"
#endif

Keyboard::Keyboard()
{

}

Keyboard::~Keyboard()
{

}

HRESULT Keyboard::Create()
{
	D_RETURN( g_pDxInputDevice->CreateDevice(GUID_SysKeyboard, &m_pInputController, NULL) );
	D_RETURN( m_pInputController->SetDataFormat(&c_dfDIKeyboard) );
	D_RETURN( m_pInputController->SetCooperativeLevel( g_pInputDevice->GetHandle(), KEYBOARD_COOPLEVEL) );

	return S_OK;
}

HRESULT Keyboard::Poll()
{
	if( ! m_pInputController )
		return E_FAIL;

	ZeroMemory(&m_Data, sizeof(char) * m_SizeBuffer);

	if ( FAILED( m_pInputController->GetDeviceState(sizeof(char) * m_SizeBuffer, &m_Data) ) )
	{
		if( m_pInputController->Acquire() == DIERR_INPUTLOST )
		{
			while( m_pInputController->Acquire() == DIERR_INPUTLOST );

			m_pInputController->GetDeviceState(sizeof(char) * m_SizeBuffer, &m_Data);
		}
		else
		{
			return E_FAIL;
		}
	}
	
	return S_OK;
}

int Keyboard::KeyPressed(int _iTouche) const
{
	return (m_Data[_iTouche] & 0x80);
}