#ifndef _ENGINE_CAMERA_
#define _ENGINE_CAMERA_

#ifndef _CAMERA_
#include "Engine/Camera/Camera.h"
#endif

class ConstantBufferUpdate;
class Light;

class EngineCamera : public Camera
{
protected:
	struct sCameraShaderParam
	{
		Matrix4x4	m_mWorldViewProj;
		Matrix4x4	m_mWorld;
		Vector4		m_LightDir;
		Color		m_LightColor;
		Vector4		m_CameraDir;
	};
	ConstantBufferUpdate	* m_pCameraShaderParamBuffer;
	sCameraShaderParam		  m_sCameraShaderParam;

public:
	EngineCamera();
	virtual ~EngineCamera();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	void Set(const Matrix4x4 & _mWorld, Light * _pLight);
	ConstantBufferUpdate * GetCameraShaderParamBuffer() const { return m_pCameraShaderParamBuffer; }

	// Cam moves
	void MoveForward( const float & _fDeltaMove );
	void MoveBackward( const float & _fDeltaMove );
	void StrafeLeft( const float & _fDeltaMove );
	void StrafeRight( const float & _fDeltaMove );
	void MoveLookAt( const Vector2 & _f2DeltaMove );
	
};

#endif // _ENGINE_CAMERA_