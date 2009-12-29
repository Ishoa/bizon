#ifndef _VECTOR2_
#define _VECTOR2_

class Vector2
{
private:
	float m_fCoord[2];
public:
	Vector2();
	Vector2(const Vector2 & v);
	Vector2(float _u, float _v);
	~Vector2();

	inline float X() const { return m_fCoord[0]; }
	inline float Y() const { return m_fCoord[1]; }

	inline float & X() { return m_fCoord[0]; }
	inline float & Y() { return m_fCoord[1]; }

	inline float * GetVector() { return (float*)m_fCoord; }
};

#endif // _VECTOR2_