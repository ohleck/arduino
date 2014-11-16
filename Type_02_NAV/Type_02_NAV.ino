// NAV GAUGE
#include "Arduino.h"
#include <StandardCplusplus.h>
#include <CPaquet.h>
#include <Wire.h>
#include <CFFSSlave.h>
#include <FFSProtocole.h>

#include <LiquidCrystal.h>

LiquidCrystal disp(7, 8, 9, 10, 11, 12);
float fNAV1;
float fNAV2;
UINT8 SlaveAddress = 0x01;
CFFSSlave* Slave;
int SelectButton;
int aSelectButton;
INT32 Step;
int Selector;

void setup()
{
  fNAV1 =0;
  fNAV2 =0;
  disp.begin(16,2);
   // initialize the pushbuttons:
  pinMode(4, INPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  SelectButton=0;
  aSelectButton=0;
  Selector=0;
  Slave = CFFSSlave::Instance();
  Slave->Init(SlaveAddress,Traitement);
  Slave->SendText("Esclave OK");
}

void loop()
{
  bool UpBtn;
  bool DnBtn;
  unsigned int td = millis();
      	
  disp.setCursor(0,0);
  int iNAV1 = int(fNAV1 *100.0);
  int iNAV2 = int(fNAV2 *100.0);
  disp.printf("NAV1=%03d.%02d MHz",iNAV1/100, iNAV1%100);
  disp.setCursor(0,1);
  disp.printf("NAV2=%03d.%02d MHz",iNAV2/100, iNAV2%100);
  
  //Traitement des entrées
  SelectButton = digitalRead(4);
  UpBtn=digitalRead(3);
  DnBtn=digitalRead(2);
  if (UpBtn && !DnBtn) Step ++;
  else if  (DnBtn && !UpBtn) Step --;
  else Step=0;

  if (SelectButton != aSelectButton)
  {
    if (SelectButton)
    {
      Selector = !Selector;
      Step=0;
      SendNAV(Step);
    }
    aSelectButton = SelectButton;
  }
  if ((abs(Step) >20) || (abs(Step)==1))
  {
      SendNAV(Step);
  }
  //mesure du temps écoulé pour syncroniser la prochaine boucle
  unsigned int tf = millis();
  delay (20 - (tf-td));
}

void SendNAV(INT32 Step)
{
  CPaquet Data;
  Data.AddByte(SlaveAddress);
  Data.AddByte(FFS_NAV);
  Data.AddDWord(Step);
  Slave->SendData(Data);
}

void Traitement (CPaquet& Data)
{
  Data.Begin();
  UINT8 Esclave;
  Data.GetByte(Esclave);
  if (Esclave!=SlaveAddress) return; // Pas le bon esclave
  UINT8 Code;
  Data.GetByte(Code);
  switch (Code)
  {
    case FFS_NAV:
    Data.GetFloat(fNAV1);
    Data.GetFloat(fNAV2);
    break;
    default:
    break;
  }
}


