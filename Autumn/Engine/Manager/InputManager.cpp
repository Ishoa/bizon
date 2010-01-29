#include "stdafx.h"

#ifndef _INPUT_MANAGER_
#include "Engine/Manager/InputManager.h"
#endif


InputManager::InputManager()
: Manager()
, m_hWnd( NULL )
, m_hInstance( NULL )
, m_pKeyBoard( NULL )
, m_pMouse( NULL )
{

}

InputManager::InputManager( HWND _hWnd, HINSTANCE _hInstance )
:Manager()
, m_hWnd( _hWnd )
, m_hInstance( _hInstance )
, m_pKeyBoard( NULL )
, m_pMouse( NULL )
{

}

InputManager::~InputManager()
{

}

HRESULT InputManager::Create()
{
	m_pInputDevice = InputDevice::CreateInstance();
	D_RETURN( m_pInputDevice->Create( m_hWnd, m_hInstance ) );

	m_pKeyBoard = new Keyboard;
	D_RETURN( m_pKeyBoard->Create() );

	m_pMouse = new Mouse;
	D_RETURN( m_pMouse->Create() );

	return S_OK;
}

HRESULT InputManager::Update()
{
	m_pKeyBoard->Poll();
	m_pMouse->Poll();

	return S_OK;	
}

HRESULT InputManager::Destroy()
{
	SAFE_DESTROY( m_pKeyBoard );
	SAFE_DESTROY( m_pMouse );
	SAFE_DESTROY( m_pInputDevice );
	return S_OK;
}

int InputManager::KeyPressed( int _iTouche ) const
{
	return m_pKeyBoard->KeyPressed( _iTouche );
}

int InputManager::LeftButtonPressed() const
{
	return m_pMouse->LeftClick();
}

int InputManager::RightButtonPressed() const
{
	return m_pMouse->RightClick();
}

const Vector2 & InputManager::GetMousePosition() const
{
	return m_pMouse->GetPosition();
}

const Vector2 & InputManager::GetMouseDeltaPosition() const
{
	return m_pMouse->GetDeltaPosition();
}

void InputManager::SetMousePosition( int _x, int _y )
{
	if( m_pMouse )
		m_pMouse->SetPosition(_x, _y);
}