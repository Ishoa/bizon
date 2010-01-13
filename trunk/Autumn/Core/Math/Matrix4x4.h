#ifndef _MATRIX4X4_
#define _MATRIX4X4_

#ifndef _VECTOR4_
#include "Core/Math/Vector4.h"
#endif

#ifndef _VECTOR3_
#include "Core/Math/Vector3.h"
#endif

#include <D3DX10.h>

class Matrix4x4
{
protected:
	D3DXMATRIX m_mMatrix;
public:
	Matrix4x4();
	Matrix4x4(const Matrix4x4 & m);
	~Matrix4x4();

	void SetIdentity();

	float & operator () (unsigned int i, unsigned int j);
	float operator () (unsigned int i, unsigned int j) const;

	inline D3DXMATRIX GetMatrix() { return m_mMatrix; }

	friend Matrix4x4	operator * (const Matrix4x4 & _m1, const Matrix4x4 & _m2);
	friend Vector3		operator * (const Vector3 & _v, const Matrix4x4 & _m);
	friend Vector4		operator * (const Vector4 & _v, const Matrix4x4 & _m);

	void BuildProjectionMatrix(float _fFov, float _fAspect, float _fZNear, float _fZFar);
	void BuildProjectionMatrix(float _fLeft, float _fRight, float _fBottom, float _fTop, float _fZNear, float _fZFar);
	void BuildViewMatrix(Vector3 _Pos, Vector3 _At, Vector3 _Up);

	Matrix4x4 GetInverse() const ;

	void SetTranslation(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScaling(float x, float y, float z);

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);


};

#endif // _MATRIX4X4_