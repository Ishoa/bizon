#ifndef _INPUT_DEVICE_
#define _INPUT_DEVICE_

#ifndef _SINGLETON_
#include "Core/Pattern/Singleton.h"
#endif

struct IDirectInput8;

class InputDevice : public Singleton< InputDevice >
{
protected:
	IDirectInput8 * m_pDIN;
	HWND			m_hWnd;
	HINSTANCE		m_hInstance;

public:
	InputDevice();
	~InputDevice();

	HRESULT Create(HWND _hWnd, HINSTANCE _hInstance);
	HRESULT Destroy();

	inline IDirectInput8 *	GetDevice() { return m_pDIN; }
	inline HWND				GetHandle() { return m_hWnd; }
	inline HINSTANCE		GetHandleInstance() { return m_hInstance; }
};

#define g_pInputDevice		InputDevice::GetInstance()
#define g_pDxInputDevice	InputDevice::GetInstance()->GetDevice()

#endif // _INPUT_DEVICE_