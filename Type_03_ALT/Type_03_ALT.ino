// NAV GAUGE
#include "Arduino.h"
#include <StandardCplusplus.h>
#include <CPaquet.h>
#include <Wire.h>
#include <CFFSSlave.h>
#include <FFSProtocole.h>
#include <TFT.h>
#include <SPI.h>

#define cs   10
#define dc   9
#define rst  8  

TFT TFTscreen = TFT(cs, dc, rst);

UINT32 AAltitude;
UINT32 Altitude;
UINT8 SlaveAddress = 0x02;
CFFSSlave* Slave;

void setup()
{
  Altitude=0;
  AAltitude=0;
  TFTscreen.begin();
  Slave = CFFSSlave::Instance();
  Slave->Init(SlaveAddress,Traitement);
  Slave->SendText("Esclave OK");
  TFTscreen.background(0, 127, 255);
  TFTscreen.stroke(255,255,255);
  TFTscreen.setTextSize(2);
  TFTscreen.text("Altitude : ",0,0);
  TFTscreen.setTextSize(3);
}

void loop()
{
  unsigned int td = millis();
  TFTscreen.stroke(0, 127, 255);
  TFTscreen.setCursor(0,20);
  TFTscreen.printf("%05dFT",AAltitude);
  TFTscreen.stroke(255,255,255);
  TFTscreen.setCursor(0,20);
  TFTscreen.printf("%05dFT",Altitude);
  AAltitude=Altitude;
  
  //mesure du temps écoulé pour syncroniser la prochaine boucle
  unsigned int tf = millis();
  delay (250 - (tf-td));
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
    case FFS_ALT:
      Data.GetDWord(Altitude);
      break;
    default:
      break;
  }
}


