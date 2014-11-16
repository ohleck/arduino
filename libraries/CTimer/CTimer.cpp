/*!
* \file CTimer.cpp
* \brief Timer Manager
* \author Gianni.P
* \version 0.1
* \date 15/11/2014
*
* Timer + Controller Class for arduino 
*
*/

#include "CTimer.h"
#include <arduino.h>

CTimer::CTimer (int Duration, TimerCallback CallBack)
{
	if (Duration <10) Duration=10;
	m_iDuration=Duration;
	m_pCallback = CallBack;
	m_uiStartTime=0;
	m_bOneShoot=false;
}

void CTimer::Start (int Duration, bool OneShoot)
{
	if (m_uiStartTime) return;
	if (Duration > 10) m_iDuration=Duration;
	m_uiStartTime=millis();
	m_bOneShoot=OneShoot;
}

void CTimer::Stop ()
{
	if(m_uiStartTime) m_uiStartTime=0;
}

void CTimer::Update ()
{
	if(m_uiStartTime)
	{
		if (millis()>=(m_uiStartTime+m_iDuration))
		{
			if (!m_bOneShoot) m_uiStartTime=millis();
			else m_uiStartTime=0;
			m_pCallback(this);
		}
	}
}

bool CTimer::IsRunning()
{
	return (m_uiStartTime>0);
}

CCtrlTimer* CCtrlTimer::m_pInstance=0;

CCtrlTimer* CCtrlTimer::Instance()
{
	if (!m_pInstance) m_pInstance=new CCtrlTimer;
	return m_pInstance;
}

CTimer* CCtrlTimer::AddTimer(int Duration, TimerCallback CallBack)
{
	CTimer* Timer=new CTimer(Duration,CallBack);
	Timers.push_back(Timer);
	return Timer;
}

void CCtrlTimer::DelTimer(CTimer* pTimer)
{
	for (vector<CTimer*>::iterator it=Timers.begin(); it != Timers.end();++it)
	{
		if (pTimer == (*it))
		{
			delete (*it);
			Timers.erase(it);
		}
	}
}

void CCtrlTimer::Update ()
{
	for (vector<CTimer*>::iterator it=Timers.begin(); it != Timers.end();++it)
	{
		(*it)->Update();
	}
}