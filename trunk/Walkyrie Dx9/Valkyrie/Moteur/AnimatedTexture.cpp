#include "AnimatedTexture.h"



CAnimatedTexture::CAnimatedTexture(LPDIRECT3DDEVICE9 pd3dDevice)
{

	m_pd3dDevice = pd3dDevice;
	m_nNBTextureAnimated = 0;
	m_fTimeLastChange = 0.0f;
	m_fIntervalle =  0.5f;
	m_nCurrentTexture = 0;
}

CAnimatedTexture::~CAnimatedTexture()
{
	for(int i = 0; i < m_nNBTextureAnimated;i++)
		m_pTabTexture->Release();
}

void CAnimatedTexture::UpdateTexture(float fDeltaTime)
{
	m_fTimeLastChange += fDeltaTime;
	if(m_fTimeLastChange >= m_fIntervalle)
		m_nCurrentTexture = (m_nCurrentTexture+1)%m_nNBTextureAnimated;
}


CLPTexture& CAnimatedTexture::GetTexture(int nIndiceTexture)
{
	if(nIndiceTexture >= 0 && nIndiceTexture < MAX_TEXTURE_ANIMATED)
		return m_pTabTexture[nIndiceTexture];
	else
		return m_pTabTexture[m_nCurrentTexture];
}

CLPTexture& CAnimatedTexture::GetTextureTime(float m_fCurrentTime, float fTimeDepart, float fEndTime)
{

	return m_pTabTexture[GetIndiceTextureTime(m_fCurrentTime,fTimeDepart,fEndTime)];

}

int CAnimatedTexture::GetIndiceTextureTime(float m_fCurrentTime, float fTimeDepart, float fEndTime)
{
	if(m_nNBTextureAnimated > 0)
	{
		float fTotalTime = fEndTime - fTimeDepart;
		float fStep = fTotalTime / m_nNBTextureAnimated;

		int m_nCurrentTextureTime=  (int)(m_fCurrentTime / fStep);

		return m_nCurrentTextureTime;
	}

	return m_nCurrentTexture;

}

CLPTexture& CAnimatedTexture::GetCurrentTexture()
{
	return m_pTabTexture[m_nCurrentTexture];
}

bool CAnimatedTexture::SetTexture(char** pTabNameTexture,int nNbTexture)
{

	if(m_nNBTextureAnimated > 0)
		Release();
		

	for(int i = 0;i < nNbTexture; i++)
	{
		if(!m_pTabTexture[i].chargerTexture(m_pd3dDevice,pTabNameTexture[i]))
			return false;

		m_nNBTextureAnimated++;
	}


	return true;
}


void CAnimatedTexture::Release()
{
	for(int i = 0; i < m_nNBTextureAnimated;i++)
		m_pTabTexture->Release();

	m_nCurrentTexture = 0;
	m_nNBTextureAnimated = 0;
}