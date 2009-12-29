#ifndef _TIME_MANAGER_
#define _TIME_MANAGER_

#ifndef _MANAGER_
#include "Engine/Manager/Manager.h"
#endif

struct TimeInfo
{
	float m_fDeltaTime;
	float m_fCurrentTime;
};

class TimeManager : public Manager
{
private:
	__int64		m_dwFrequency;
	__int64		m_dwT0;
	__int64		m_dwT1;
	__int64		m_dwStart;
	TimeInfo	m_TimeInfo;
public:

	TimeManager();
	virtual ~TimeManager();

	virtual HRESULT Create();
	virtual HRESULT Update();
	virtual HRESULT Destroy();

	const TimeInfo & GetInfo() { return m_TimeInfo; };

	inline float GetDeltaTime() const { return m_TimeInfo.m_fDeltaTime; }
	inline float GetCurrentTime() const { return m_TimeInfo.m_fCurrentTime; }
	
};

#endif // _TIME_MANAGER_