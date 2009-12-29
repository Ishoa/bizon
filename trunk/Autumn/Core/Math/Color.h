#ifndef _COLORD_
#define _COLORD_

#include <D3DX10math.h>

class Color
{
private:
	D3DXCOLOR m_cColor;
public:
	Color();
	Color(float _fRed, float _fGreen, float _fBlue);
	Color(float _fRed, float _fGreen, float _fBlue, float _fAlpha);
	Color(const Color & c);
	~Color();

	inline float & Red()	{ return m_cColor.r; }
	inline float & Green()	{ return m_cColor.g; }
	inline float & Blue()	{ return m_cColor.b; }
	inline float & Alpha()	{ return m_cColor.a; }

	inline float Red()	const { return m_cColor.r; }
	inline float Green()const { return m_cColor.g; }
	inline float Blue()	const { return m_cColor.b; }
	inline float Alpha()const { return m_cColor.a; }

	inline D3DXCOLOR GetColor() const { return m_cColor; }

	void Set(float _fRed, float _fGreen, float _fBlue, float _fAlpha);

	friend Color operator + (const Color & _c1, const Color & _c2);
	friend Color operator - (const Color & _c1, const Color & _c2);
	friend Color operator * (const Color & _c1, const Color & _c2);
	friend Color operator * (const Color & _c, const float & _f);

};


#endif // _COLOR_