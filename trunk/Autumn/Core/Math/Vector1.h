#ifndef _VECTOR1_
#define _VECTOR1_

class Vector1
{
public:
	float x;
public:
	Vector1();
	Vector1(float _x);
	Vector1(const Vector1 & v);
	virtual ~Vector1();

	inline virtual float & X() { return x; }

};


#endif // _VECTOR1_