#ifndef _CJOYSTICK_H
#define _CJOYSTICK_H

// include the basic windows header files and the DirectX header files


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Peripherique.h"


class CPeripherique;

class CJoystick
{
public:

	CPeripherique* m_pPeripherique;			// pointeur sur le gestionnaire de phériphérique
	LPDIRECTINPUTDEVICE8 pJoystick;		// pointeur sur l'objet Souris
	DIJOYSTATE2 m_EtatJoyStick;// Structure contenant l'état du joystick

	CJoystick();
	CJoystick(CPeripherique* pPeripherique);
	~CJoystick();

	bool Creation(LPDIRECTINPUT8 lpDI);
	void Destruction();
	
	bool Acquisition(){pJoystick->Acquire(); return true;}
	bool DeAcquisition(){pJoystick->Unacquire(); return true;}
	CPeripherique* GetPeripherique(){return m_pPeripherique;}

	bool GetEtatJoystick(bool bActiveAquisition);
	DIJOYSTATE2* GetStructEtatJoystick(){return &m_EtatJoyStick;}

	
};

#endif _CJOYSTICK_H