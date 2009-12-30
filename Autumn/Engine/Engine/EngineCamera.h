#ifndef _ENGINE_CAMERA_
#define _ENGINE_CAMERA_

#ifndef _CAMERA_
#include "Engine/Camera/Camera.h"
#endif

class ConstantBufferUpdate;

class EngineCamera : public Camera
{
protected:
	struct sCameraShaderParam
	{
		Matrix4x4	m_mWorldViewProj;
	};
	ConstantBufferUpdate	* m_pCameraShaderParamBuffer;
	sCameraShaderParam		  m_sCameraShaderParam;

public:
	EngineCamera();
	virtual ~EngineCamera();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	void SetWorld(const Matrix4x4 & _mWorld);
	ConstantBufferUpdate * GetCameraShaderParamBuffer() const { return m_pCameraShaderParamBuffer; }
	
};

#endif // _ENGINE_CAMERA_