#ifndef CTIMER_H
#define CTIMER_H

/*!
* \file CTimer.h
* \brief Timer Manager
* \author Gianni.P
* \version 0.1
* \date 15/11/2014
*
* Timer + Controller Class for arduino 
*
*/

#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <iterator>
#include <vector>

using namespace std;

class CTimer;

typedef void (*TimerCallback)(CTimer* pTimer); /*!> Callback prototype for Timer event function*/


/*! \class CTimer
 *  \brief Timer representation
 *
 *  Timer Class for arduino board
 */
 
class CTimer
{
	public:
		
		/* \brief CTimer constructor
		 *
		 *  int Duration : Timer duration in msec
		 * TimerCallback CallBack : address of callback timer function in format:
		 * 	void myEventTimerFunction (CTimer* Timer) 
		 */
		CTimer (int Duration, TimerCallback CallBack);
		
		/* \brief CTimer destructor
		 */
		~CTimer(){}
		
		/* \brief CTimer Updater
		 *
		 * This method update the timer and call the callback function if time duration elapsed
		 * if OneShoot is false, it start again the timer for the same duration
		 * need to be placed in loop
		 */
		void	Update();
		
		
		void	Start(int Duration=0,bool OneShoot=false );
		void	Stop();
		bool	IsRunning();
	private:
		int m_iDuration;
		bool	m_bOneShoot;
		unsigned int m_uiStartTime;
		TimerCallback m_pCallback;
};

class CCtrlTimer
{
	private:
	CCtrlTimer(){}
	~CCtrlTimer(){}
	vector<CTimer*> Timers;
	static CCtrlTimer* m_pInstance;
	public:
	static CCtrlTimer* Instance();
	CTimer* AddTimer (int Duration,  TimerCallback CallBack);
	void DelTimer (CTimer* pTimer);
	void Update();
};

#endif