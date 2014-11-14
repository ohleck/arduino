#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <iterator>
#include <vector>
#include <string>
#include <Wire.h>
#include "CPaquet.h"

using namespace std;

UINT8 NBSlave=0;
UINT8 CurSlave=0;
UINT8* Buffer;
CPaquet SentData;
vector<CPaquet> Pile;
CPaquet Data;
int DataLen;

// Master Code
#define FFS_LOG  (UINT8)0x00
#define FFS_NSL  (UINT8)0x01 //Nombre d'esclave à gerer
#define FFS_SERVICE (UINT8)0xFF

void setup()
{
  Pile.reserve(5);
  Buffer = new unsigned char[64];
  Serial.begin(115200);
  SendMessage("Master ok");
  DataLen = 0;
  Wire.begin();
  
}

void loop()
{
  unsigned int td = millis();
  ListenSerial(); //Ecoute le port série et traite les messages si besoin est
  //mesure du temps écoulé pour syncroniser la prochaine boucle
  TraitementEsclave();
  unsigned int tf = millis();
  delay (10 - (tf-td));
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

void SendMessage(const string& Message)
{
  SentData.Clear();
  SentData.AddByte((UINT8)0); // Master = esclave 0
  SentData.AddByte(FFS_LOG);
  SentData.AddString(Message);
  SerialSend(SentData);
}

void Traitement (CPaquet& Commande)
{
  string Message;
  Commande.Begin();
  UINT8 Esclave;
  Commande.GetByte(Esclave);
  if (!Esclave)
  {
    unsigned char Code;
    Commande.GetByte(Code);
    switch (Code)
    {
      case FFS_NSL:
        Commande.GetByte(NBSlave);
        CurSlave = 1;
        Message = "No d'esclave=";
        //Message += itoa(NBSlave);
        SendMessage(Message);
        break;
      default:
        break;
    }
  }
  else
  {
    if (Pile.size() < 5 ) Pile.push_back(Commande); // Si autre esclave alors on met le paquet dans la pile
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

void TraitementEsclave ()
{
  UINT8 Byte=0;
  //Traitement de la comm descendante
  if (Pile.size() > 0)
  {
    vector<CPaquet>::iterator i = Pile.begin();
    UINT8 Esclave;
    i->Begin();
    UINT8 Len=i->GetSize();
    i->GetByte(Esclave);
    if (i->Get(Buffer,Len)==PQ_OK)
    {
      Wire.beginTransmission(Esclave);
      Wire.write(Buffer,Len);
      Wire.endTransmission();
    }
    Pile.erase(i);
  }
  if (NBSlave>0)
  {
    SentData.Clear();
    Wire.beginTransmission(CurSlave);
    Wire.requestFrom((int)CurSlave,1);
    while(Wire.available())
    {
      Byte = Wire.read();
    }
    if (Byte>0)
    {
      Wire.requestFrom((int)CurSlave,(int)Byte);
      while(Wire.available())
      {
        Byte = Wire.read();
        SentData.AddByte(Byte);
      }
      SerialSend(SentData);
    }
    Wire.endTransmission();
    CurSlave++;
    if (CurSlave>NBSlave) CurSlave=1;
  }
}


