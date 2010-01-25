#ifndef _VECTOR3_
#define _VECTOR3_

#include <D3DX10.h>


class Vector3
{
private:
	D3DXVECTOR3 m_vVector;

public:
	Vector3();
	Vector3(const Vector3 & v);
	Vector3(float _x, float _y, float _z);
	~Vector3();


	inline float X() const { return m_vVector.x; }
	inline float Y() const { return m_vVector.y; }
	inline float Z() const { return m_vVector.z; }

	inline float & X() { return m_vVector.x; }
	inline float & Y() { return m_vVector.y; }
	inline float & Z() { return m_vVector.z; }

	inline D3DXVECTOR3 GetVector() const { return m_vVector; }

	// functions
	float	GetLength() const;
	Vector3 Normalize() const;

	// operators
	Vector3	& operator += (const Vector3 & _v);
	Vector3	& operator -= (const Vector3 & _v);
	Vector3	  operator - ();
	friend Vector3	operator + (const Vector3 & _v1, const Vector3 & _v2);
	friend Vector3	operator - (const Vector3 & _v1, const Vector3 & _v2);
	friend Vector3	operator * (const Vector3 & _v, const float & _f);
	friend Vector3	operator / (const Vector3 & _v, const float & _f);
	
	// cross product
	friend Vector3	operator ^ (const Vector3 & _v1, const Vector3 & _v2);
	// dot product
	friend float	operator * (const Vector3 & _v1, const Vector3 & _v2);

};

#endif // _VECTOR3_