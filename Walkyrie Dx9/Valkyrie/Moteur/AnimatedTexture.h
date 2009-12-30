

#ifndef _ANIMATEDTEXTURE_H_
#define _ANIMATEDTEXTURE_H_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Texture.h"

#define MAX_TEXTURE_ANIMATED 20


class CAnimatedTexture
{
private:

	CLPTexture m_pTabTexture[MAX_TEXTURE_ANIMATED];
	int m_nNBTextureAnimated;
	int m_nCurrentTexture;
	float m_fTimeLastChange;
	float m_fIntervalle;

	LPDIRECT3DDEVICE9 m_pd3dDevice;

public:

	CAnimatedTexture(LPDIRECT3DDEVICE9 pd3dDevice);

	~CAnimatedTexture();

	bool SetTexture(char** pTabNameTexture,int nNbTexture);

	void SetIntervalBetweenText(float fIntervalle){m_fIntervalle = fIntervalle;}

	void UpdateTexture(float fDeltaTime);

	CLPTexture& GetCurrentTexture();
	CLPTexture& GetTexture(int nIndiceTexture);
	CLPTexture& GetTextureTime(float m_fCurrentTime, float fTimeDepart = 0.0f, float fEndTime = 1.0f);
	int GetIndiceTextureTime(float m_fCurrentTime, float fTimeDepart = 0.0f, float fEndTime = 1.0f);
	void Release();
};


#endif