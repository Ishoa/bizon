#ifndef _LIGHT_
#define _LIGHT_

#ifndef _COLORD_
#include "Core/Math/Color.h"
#endif

#ifndef _VECTOR4_
#include "Core/Math/Vector4.h"
#endif

class Light
{
protected:
	// Light Color
	Color		m_oColor;
	// Direction / Position
	Vector4		m_Direction;
public:
	Light(const Color & _Color = Color(1,1,1,1), const Vector4 & _Direction = Vector4(0,0,0,1));
	~Light();

	HRESULT Create();
	HRESULT Destroy();

	inline bool		IsDirectional()	const { return m_Direction.W() == 0.0f; }
	inline bool		IsPositional()	const { return m_Direction.W() == 1.0f; }
	inline Color	GetColor()		const { return m_oColor; }
	inline Vector3	GetDirection()	const { return Vector3(m_Direction.X(), m_Direction.Y(), m_Direction.Z()); }

	inline void SetColor(Color val)		  { m_oColor = val; }
	inline void SetDirection(Vector4 val) { m_Direction = val; }

	


};

#endif // _LIGHT_