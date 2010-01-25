#include "stdafx.h"

#ifndef _INPUT_DEVICE_
#include "WrapperDX/InputDevice/InputDevice.h"
#endif

#ifndef _INPUT_CONTROLLER_
#include "WrapperDx/InputDevice/InputController.h"
#endif

InputController::InputController()
: m_pInputController( NULL )
{
}

InputController::~InputController()
{
}

HRESULT InputController::Destroy()
{
	if( m_pInputController )
		m_pInputController->Unacquire();

	SAFE_RELEASE( m_pInputController );

	return S_OK;
}
