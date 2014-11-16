#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <iterator>
#include <vector>
#include <CTimer.h>
#include <Wire.h>
#include "CPaquet.h"

using namespace std;

UINT8* Buffer;
vector<int> Esclaves;
vector<int>::iterator CurSlave;
UINT8 address;
UINT8 SlaveRecept;
CTimer* T1;
CPaquet Data;
int DataLen;

// Master Code
#define FFS_LOG  (UINT8)0x00
#define FFS_NSL  (UINT8)0x01 //Nombre d'esclave à gerer
#define FFS_SERVICE (UINT8)0xFF

void setup()
{
  Buffer = new unsigned char[64];
  Serial.begin(115200);
  SendMessage("Master ok");
  DataLen = 0;
  T1=CCtrlTimer::Instance()->AddTimer(100,ScanSlave);
  T1->Start(100);
  address=1;
  SlaveRecept=0;
  CurSlave = Esclaves.end();
  Wire.begin();
  
}

void loop()
{
  unsigned int td = millis();
  ListenSerial(); //Ecoute le port série et traite les messages si besoin est
  //mesure du temps écoulé pour syncroniser la prochaine boucle
  
  Slave();
  unsigned int tf = millis();
  delay (10 - (tf-td));
  CCtrlTimer::Instance()->Update();
}

void ScanSlave(CTimer* me)
{
  Wire.beginTransmission(address);
  
  if (Wire.endTransmission() == 0)
  {
    Esclaves.push_back(address);
  }
  address++;
  if (address > 127) me->Stop();
}

void SerialSend(CPaquet& Paquet)
{
  Buffer[0]=Paquet.GetSize();
  if (Paquet.Get(Buffer+1,Buffer[0])==PQ_OK)
  {
    Serial.write(Buffer,Buffer[0]+1);
    Serial.flush();
  }
}

void SendMessage(const String& Message)
{
  CPaquet Data;
  Data.AddByte((UINT8)0); // Master = esclave 0
  Data.AddByte(FFS_LOG);
  Data.AddString(Message);
  SerialSend(Data);
}

void Traitement (CPaquet& Commande)
{
  String Message;
  Commande.Begin();
  UINT8 Esclave;
  UINT8 Len;
  Commande.GetByte(Esclave);
  if (!Esclave)
  {
    unsigned char Code;
    Commande.GetByte(Code);
    switch (Code)
    {
      default:
        break;
    }
  }
  else
  {
    //if (Pile.size() < 4 ) Pile.push_back(Commande); // Si autre esclave alors on met le paquet dans la pile
    Len=Commande.GetSize();
    Commande.Get(Buffer,Len);
    Wire.beginTransmission(Esclave);
    Wire.write(Buffer,Len);
    Wire.endTransmission();
  }
  Commande.Clear();
}

void ListenSerial()
{
  char Bloc; //Donnée extraite
  while (Serial.available()) //Tant qu'il y a des données sur le port série
  {
    Bloc=Serial.read(); //on stock le caractére
    if ( DataLen < 1)
    {
      //Début d'une nouvelle trame
      // Début on récupère la longueur
      DataLen = Bloc;
    }
    else // Une trame est en cours de transmission
    {
      Data.AddByte(Bloc);
      DataLen --;
      if (DataLen<1) Traitement (Data);
    }
  }
}

void Slave()
{
  UINT8 Esclave;
  CPaquet Buffer;
  /*if (!Pile.empty())
  {
    vector<CPaquet>::iterator it=Pile.begin();
    it->Begin();
    it->GetByte(Esclave);
    Len=it->GetSize();
    it->Get(Buffer,Len);
    Pile.erase(it);
    Wire.beginTransmission(Esclave);
    Wire.write(Buffer,Len);
    Wire.endTransmission();
  }*/
  
  // Request Slave information
  if (Esclaves.empty()) return;
  if (CurSlave==Esclaves.end()) CurSlave=Esclaves.begin();
  if (!SlaveRecept)
  {
    Wire.requestFrom(*CurSlave,1);
    while (Wire.available()) SlaveRecept = Wire.read();
  }
  else
  {  
    Wire.requestFrom(*CurSlave,(int)SlaveRecept);
    while(SlaveRecept>0)
    {
      UINT8 Octet = (UINT8)Wire.read();
      Buffer.AddByte(Octet);
      SlaveRecept--;
    }
    SerialSend(Buffer);
  }
  if (!SlaveRecept) ++CurSlave;
}


