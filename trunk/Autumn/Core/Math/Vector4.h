#ifndef _VECTOR4_
#define _VECTOR4_

class Vector4
{
private:
	D3DXVECTOR4 m_vVector;
public:
	Vector4();
	Vector4(const Vector4 & v);
	Vector4(const Vector3 & v);
	Vector4(float _x, float _y, float _z);
	Vector4(float _x, float _y, float _z, float _w);
	~Vector4();

	inline float X() const { return m_vVector.x; }
	inline float Y() const { return m_vVector.y; }
	inline float Z() const { return m_vVector.z; }
	inline float W() const { return m_vVector.w; }

	inline float & X() { return m_vVector.x; }
	inline float & Y() { return m_vVector.y; }
	inline float & Z() { return m_vVector.z; }
	inline float & W() { return m_vVector.w; }

	inline D3DXVECTOR4 GetVector() const { return m_vVector; }

	float	GetLength() const;
	Vector4 Normalize() const;
	
};

#endif // _VECTOR4_