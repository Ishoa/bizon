#ifndef _SPHERE_
#define _SPHERE_

#include "Vector3.h"

class Sphere
{
private:
	Vector3 m_oCenter;
	float	m_fRadius;
public:
	Sphere();
	Sphere(Vector3 _oCenter, float _fRadius);
	~Sphere();

	inline Vector3	GetCenter() const		{ return m_oCenter; }
	inline void		SetCenter(Vector3 val) { m_oCenter = val; }
	inline float	GetRadius() const		{ return m_fRadius; }
	inline void		SetRadius(float val)	{ m_fRadius = val; }

};


#endif // _SPHERE_