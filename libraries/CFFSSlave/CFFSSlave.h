#ifndef CFFSSLAVE_H
#define CFFSSLAVE_H

/*!
* \file CFFSSlave.h
* \brief FFS Slave Manager
* \author Gianni.P
* \version 0.1
* \date 15/11/2014
*
* FFS Slave Manager for arduino
*
*/

#include <arduino.h>

#include <StandardCplusplus.h>
#include <vector>

#include <CPaquet.h>

using namespace std;

class CFFSSlave
{
	friend class CTimer;
	friend class Wire;
private:
	CFFSSlave (){}
	~CFFSSlave (){}
	static CFFSSlave* m_pCFFSSlave;
	void (*m_user_onReceive)(CPaquet&);
	UINT8 m_ui8Slave;
	UINT8 m_ui8TXSize; /*> Drapeau pour mémoriser l'envoi en cours d'un datagrame*/
protected:
	vector<UINT8> m_cBufOut; /*> Tampon FIFO pour l'envoi des données */
	static void RequestEvent ();
	static void ReceiveEvent (int);
public:
	static CFFSSlave* Instance();
	static void Free();
	void Init (UINT8 Slave, void (*)(CPaquet&));
	void SendData (CPaquet&);
	void SendText (const String&);
};

#endif
