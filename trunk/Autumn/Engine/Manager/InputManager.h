#ifndef _INPUT_MANAGER_
#define _INPUT_MANAGER_

#ifndef _MANAGER_
#include "Engine/Manager/Manager.h"
#endif

class InputDevice;
class InputController;

class InputManager : public Manager
{
private:
	HWND			m_hWnd;
	HINSTANCE		m_hInstance;
	InputDevice		* m_pInputDevice;
	Keyboard		* m_pKeyBoard;
	Mouse			* m_pMouse;

public:

	InputManager();
	InputManager(HWND _hWnd, HINSTANCE _hInstance);
	virtual ~InputManager();

	virtual HRESULT Create();
	virtual HRESULT Update();
	virtual HRESULT Destroy();

	int				KeyPressed(int _iTouche) const;
	int				LeftButtonPressed() const;
	int				RightButtonPressed() const;
	const Vector2 & GetMousePosition() const;
	const Vector2 & GetMouseDeltaPosition() const;
};

#endif // _INPUT_MANAGER_