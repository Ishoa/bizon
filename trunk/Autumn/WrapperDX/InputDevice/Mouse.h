#ifndef _MOUSE_
#define _MOUSE_

#ifndef _VECTOR2_
#include "Core/Math/Vector2.h"
#endif

#ifndef _INPUT_CONTROLLER_
#include "WrapperDx/InputDevice/InputController.h"
#endif

class Mouse : public InputController
{
protected:
	DIMOUSESTATE m_Data;
	Vector2 m_vScreenPosition;
	Vector2 m_vDeltaScreenPosition;
	bool m_bDirty;

public:
	Mouse();
	~Mouse();
	virtual HRESULT Create();
	virtual	HRESULT Poll();

	int		LeftClick() const;
	int		RightClick() const;
	long	GetWheel() const;
	long	GetDeltaX() const;
	long	GetDeltaY() const;
	void	SetPosition(int _x, int _y);
	const Vector2 & GetPosition() const;
	const Vector2 & GetDeltaPosition() const;

};

#endif