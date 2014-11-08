// ADF GAUGE
#include "Arduino.h"
#include <LiquidCrystal.h>
#include "CPaquet.h"

LiquidCrystal disp(7, 8, 9, 10, 11, 12);
float fNAV1;
float fADF1;
UBYTE Slave=0x01;
UBYTE* Buffer;
CPaquet SentData;
int SelectButton;
int aSelectButton;
DWORD Step;
int Selector;


CPaquet Data;//Donnée reçu de l2fs
int DataLen;

#define FFS_LOG  (UBYTE)0x00
#define FFS_ADF  (UBYTE)0x01
#define FFS_NAV  (UBYTE)0x02
#define FFS_ALT  (UBYTE)0x03
#define FFS_HB   (UBYTE)0xFF

void SendData(CPaquet& Paquet)
{
  BYTE Len=Paquet.GetSize();
  Buffer[0]=Len;
  if (Paquet.Get(Buffer+1,Len)==PQ_OK)
  {
    Serial.write(Buffer,Len+1);
    Serial.flush();
  }
}

void SendMessage(const String& Message)
{
  SentData.Clear();
  SentData.AddByte(Slave);
  SentData.AddByte(FFS_LOG);
  SentData.AddString(Message);
  SendData(SentData);
}

void SendADF(DWORD Step)
{
  SentData.AddByte(Slave);
  SentData.AddByte(FFS_ADF);
  SentData.AddDWord(Step);
  SendData(SentData);
}

void SendNAV(DWORD Step)
{
  SentData.AddByte(Slave);
  SentData.AddByte(FFS_NAV);
  SentData.AddDWord(Step);
  SendData(SentData);
}

void setup()
{
  Buffer = new unsigned char[256];
	Serial.begin(115200);
	SendMessage("Communication ok");
  fADF1 =0;
  fNAV1 =0;
  disp.begin(16,2);
  DataLen = 0;
  
   // initialize the pushbuttons:
  pinMode(4, INPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  SelectButton=0;
  aSelectButton=0;
  Selector=0;
  
}

void Traitement (CPaquet& Commande)
{
  Commande.Begin();
  unsigned char Esclave;
  Commande.GetByte(Esclave);
  unsigned char Code;
  Commande.GetByte(Code);
  switch (Code)
  {
    case FFS_ADF:
    Commande.GetFloat(fADF1);
    break;
    case FFS_NAV:
    Commande.GetFloat(fNAV1);
    break;
    default:
    break;
  }
  Commande.Clear();
}

void loop()
{
        bool UpBtn;
        bool DnBtn;
	unsigned int td = millis();
        SentData.Clear();
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
	//!!fin!!récupération des donnéees envoyés par l2fs
	// Autre traitements cycliques
	// ...
  disp.setCursor(0,0);
  int iADF1 = int(fADF1 *10.0);
  int iNAV1 = int(fNAV1 *100.0);
  disp.printf("ADF1=%04d.%d KHz%c",iADF1/10, iADF1%10,Selector?' ':'*');
  //disp.printf("ADF1=%04.1f KHz%c",fADF1,Selector?' ':'*');
  disp.setCursor(0,1);
  disp.printf("NAV1=%03d.%02d MHz%c",iNAV1/100, iNAV1%100,Selector?'*':' ');
  //disp.printf("NAV1=%03.2f MHz%c",fNAV1,Selector?'*':' ');
  
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
    }
    aSelectButton = SelectButton;
  }
  if ((abs(Step) >20) || (abs(Step)==1))
  {
    if (Selector)
    {
      SendNAV(Step);
    }
    else SendADF(Step);
  }
  //mesure du temps écoulé pour syncroniser la prochaine boucle
  unsigned int tf = millis();
  delay (50 - (tf-td));
}


