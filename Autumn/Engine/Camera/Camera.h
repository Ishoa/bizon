#ifndef _CAMERA_
#define _CAMERA_

#ifndef _VECTOR3_
#include "Core/Math/Vector3.h"
#endif

#ifndef _MATRIX4X4_
#include "Core/Math/Matrix4x4.h"
#endif

#ifndef _TIME_MANAGER_
#include "Engine/Manager/TimeManager.h"
#endif

class Camera
{
protected:
	bool		m_bUsePerspective;
	float		m_fAspect;
	float		m_fZNear;
	float		m_fZFar;
	float		m_fFov;
	float		m_fLeft;
	float		m_fRight;
	float		m_fBottom;
	float		m_fTop;

	Vector3		m_vPos;
	Vector3		m_vAt;
	Vector3		m_vUp;
	Matrix4x4	m_mProjection;
	Matrix4x4	m_mView;

public:
	Camera();
	virtual ~Camera();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	inline Vector3	GetPos() const { return m_vPos; }
	inline void		SetPos(Vector3 val) { m_vPos = val; }

	inline Vector3	GetLookAt() const { return m_vAt; }
	inline void		SetLookAt(Vector3 val) { m_vAt = val; }

	inline Vector3	GetUp() const { return m_vUp; }
	inline void		SetUp(Vector3 val) { m_vUp = val; }

	inline bool		IsPerspectiveProj() const { return m_bUsePerspective; }

	void SetAspect(float _fAspect);
	void BuildProjectionMatrix(float _fFov, float _fAspect, float _fZNear, float _fZFar);
	void BuildProjectionMatrix(float _fLeft, float _fRight, float _fBottom, float _fTop, float _fZNear, float _fZFar);
	void SetView(Vector3 _Pos, Vector3 _At, Vector3 _Up);
	void BuildViewMatrix();

	void Update( const TimeInfo & _sTimeInfo );

	Matrix4x4 GetViewProj() const;
	inline Matrix4x4 GetView()		const { return m_mView; }
	inline Matrix4x4 GetProj()		const { return m_mProjection; }
};

#endif // _CAMERA_