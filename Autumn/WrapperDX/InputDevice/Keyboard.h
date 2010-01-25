#ifndef _KEYBOARD_
#define _KEYBOARD_

#include <dinput.h>

#ifndef _INPUT_CONTROLLER_
#include "WrapperDx/InputDevice/InputController.h"
#endif

class Keyboard : public InputController
{
protected:
	static const unsigned int m_SizeBuffer = 256;
	char m_Data[m_SizeBuffer];
public:
	Keyboard();
	~Keyboard();
	virtual HRESULT Create();
	virtual	HRESULT Poll();
	
	int KeyPressed(int) const;
};

#endif