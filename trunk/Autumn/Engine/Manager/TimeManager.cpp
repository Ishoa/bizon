#include "stdafx.h"

#ifndef _TIME_MANAGER_
#include "Engine/Manager/TimeManager.h"
#endif


TimeManager::TimeManager()
:Manager()
{

}

TimeManager::~TimeManager()
{

}

HRESULT TimeManager::Create()
{
	if( ! QueryPerformanceFrequency( (LARGE_INTEGER *)&m_dwFrequency ) )
		return E_FAIL;

	QueryPerformanceCounter( (LARGE_INTEGER *)&m_dwT0 );

	m_dwStart = m_dwT0;

	return S_OK;
}

HRESULT TimeManager::Update()
{
	QueryPerformanceCounter( (LARGE_INTEGER *)&m_dwT1 );

	// en nanosecondes
	unsigned int uDeltaTime = (unsigned int)(( m_dwT1 - m_dwT0 ) * 1000000 / m_dwFrequency);
	unsigned int uCurrentTime = (unsigned int)(( m_dwT1 - m_dwStart ) * 1000000 / m_dwFrequency);

	// en millisecondes
	m_TimeInfo.m_fDeltaTime = (float)( uDeltaTime ) / 1000.0f;
	m_TimeInfo.m_fCurrentTime = (float)( uCurrentTime ) / 1000.0f;
	
	m_dwT0 = m_dwT1;

	return S_OK;	
}

HRESULT TimeManager::Destroy()
{
	return S_OK;
}