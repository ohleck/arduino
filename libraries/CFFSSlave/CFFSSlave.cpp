#include "CFFSSlave.h"
#include "FFSProtocole.h"
#include <Wire.h>

CFFSSlave* CFFSSlave::m_pCFFSSlave = 0;

CFFSSlave* CFFSSlave::Instance()
{
	if (!m_pCFFSSlave) m_pCFFSSlave = new CFFSSlave();
	return m_pCFFSSlave;
}

void CFFSSlave::Free()
{
   if (m_pCFFSSlave) delete m_pCFFSSlave;
   m_pCFFSSlave = 0;
}

void CFFSSlave::Init (UINT8 Slave, void (*pReceive)(CPaquet&))
{
	m_ui8TXSize = 0;
	m_user_onReceive = pReceive;
   m_ui8Slave = Slave;
   Wire.begin(m_ui8Slave);
   Wire.onRequest(CFFSSlave::RequestEvent);
	Wire.onReceive(CFFSSlave::ReceiveEvent);
}

void CFFSSlave::RequestEvent ()
{
	CFFSSlave* Slave= CFFSSlave::Instance();
	// Si aucune transmition est en cours
	if (!Slave->m_ui8TXSize)
	{
		// Si le buffer FIFO n'est pas vide
		if (Slave->m_cBufOut.size()> 0)
		{
			// On récupère le premier octet qui signifie la taille du datagramme suivant
			Slave->m_ui8TXSize=Slave->m_cBufOut.front();

			// On test si le buffer contient bien la suite du datagramme:
			// Car si nous commencons une transmission I2C, le maitre va nous demander la
			// suite aussitot.
			if (Slave->m_cBufOut.size()>= (Slave->m_ui8TXSize+1))
			{
				// Si la suite est bien la, on vide le buffer du premier Octet
				Slave->m_cBufOut.erase(Slave->m_cBufOut.begin());
			}
			// Sinon cela signifie que le buffer est en cours d'écriture,
			// On reporte donc l'envoi au prochain cycle
			else Slave->m_ui8TXSize=0;
		}
		// On informe le maitre de la quantité d'octet du datagramme
		// Qui doit suivre. 0 signifiant aucun datagramme.
		Wire.write(Slave->m_ui8TXSize);
	}
	// Le maitre a déjà été informé de taille du datagramme
	// Nous pouvons donc commencer la transmission avec la longueur convenue
	else
	{
		// Tant qu'il y a des données à écrire
		const unsigned char* ptr=Slave->m_cBufOut.begin();
		Wire.write(ptr,Slave->m_ui8TXSize);
		Slave->m_cBufOut.erase(Slave->m_cBufOut.begin(), Slave->m_cBufOut.begin()+Slave->m_ui8TXSize);
		Slave->m_ui8TXSize=0;
	}
}

void CFFSSlave::ReceiveEvent (int Taille)
{
	CFFSSlave* Slave= CFFSSlave::Instance();
	CPaquet Data;
	//Tant qu'il y a des données sur l'I2C
	while (Wire.available())
	{
		//on stock l'octet
		Data.AddByte((UINT8)Wire.read());
	}
	Slave->m_user_onReceive (Data);
}

void CFFSSlave::SendData (CPaquet& Data)
{
	UINT8 Byte;
	UINT8 Taille=(UINT8)Data.GetSize();
	if (m_cBufOut.size()>63) return;
	Data.Begin();
	m_cBufOut.push_back(Taille);
	for (int i=0; i < Taille; i++)
	{
		Data.GetByte(Byte);
		m_cBufOut.push_back(Byte);
	}
}

void CFFSSlave::SendText (const String& Message)
{
	CPaquet Data;
	Data.AddByte(m_ui8Slave);
	Data.AddByte(FFS_LOG);
	Data.AddString(Message);
	SendData (Data);
}

