#ifndef _VECTOR2_
#define _VECTOR2_

#include <D3DX10.h>

class Vector2
{
private:
	D3DXVECTOR2 m_vVector;

public:
	Vector2();
	Vector2(const Vector2 & v);
	Vector2(float _u, float _v);
	~Vector2();

	inline float X() const { return m_vVector.x; }
	inline float Y() const { return m_vVector.y; }

	inline float & X() { return  m_vVector.x; }
	inline float & Y() { return  m_vVector.y; }

	inline D3DXVECTOR2 GetVector() { return m_vVector; }

	Vector2 & operator = (const Vector2 & o);

	friend Vector2	operator - (const Vector2 & _v1, const Vector2 & _v2);
};

#endif // _VECTOR2_