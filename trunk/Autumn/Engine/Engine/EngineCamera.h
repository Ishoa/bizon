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
	
};

#endif // _ENGINE_CAMERA_