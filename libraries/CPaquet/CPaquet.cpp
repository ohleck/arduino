/**
OpenWorlds Copyright (C) 2008 Neophile

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/


// ****************************************************************************
// *                                                                          *
// *                 CPAQUET.H : Classe de formatage des paquets tcp          *
// *                                                                          *
// *                      Neophile (c) 2006 : 09/02/2007                      *
// *      Rev B : 18/03/07                                                    *
// *                                                                          *
// ****************************************************************************
// * Rev A:Neophile:27/03/2007;Description :                                  *
// *                                Ajout du type APaquet pour la gestion     *
// *                                De la pile d'envoi des paquets            *
// ****************************************************************************

#include "CPaquet.h"

//-----------------------------------------------------------------------------
// Constructeur vierge

CPaquet::CPaquet (bool pLEndian)
{
	Pointeur=0;
	Fin=0;
	LEndian = pLEndian;
}

//-----------------------------------------------------------------------------
// Constructeur avec initialisation

CPaquet::CPaquet (unsigned char* Buffer, unsigned char Taille, bool pLEndian)
{
	LEndian = pLEndian;
	Pointeur=0;
	Fin=0;
	Set (Buffer,Taille);
}

//-----------------------------------------------------------------------------
// Destructeur

CPaquet::~CPaquet ()
{
}

//-----------------------------------------------------------------------------
// Retourne des donn®¶es de tous type en fonction du type endian de la plateforme

PQ_RESULT CPaquet::GetVoid(unsigned char* Data, unsigned char Taille)
{
	if (!Taille) return PQ_BAD_LENGTH;
	unsigned char MemPtr;
	if (LEndian) MemPtr=Taille-1;
	else MemPtr=0;
	for (unsigned char i = 0; i < Taille; i++)
	{
		if (Pointeur >= Fin) return PQ_EOF;
		Data[MemPtr] = Tampon[Pointeur];
		Pointeur ++;
		if (LEndian) --MemPtr;
		else ++MemPtr;
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Incr®¶mente l'iterateur pour une donn®¶e type

PQ_RESULT CPaquet::GetVoid(unsigned char Taille)
{
	if (!Taille) return PQ_BAD_LENGTH;
	if (Pointeur>(Fin-Taille)) return PQ_EOF;
	Pointeur+= Taille;
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajoute  une donn®¶es de tout type en fonction du type endian de la plateforme

PQ_RESULT	CPaquet::AddVoid(const unsigned char* Data, unsigned char Taille)
{
	unsigned char PtrMem;
	if (Pointeur>(256 - Taille)) return PQ_BAD_LENGTH;
	if (LEndian) PtrMem = Taille -1;
	else PtrMem = 0;
	for (unsigned char i = 0; i < Taille; i++)
	{
		Tampon[Pointeur]=Data[PtrMem];
		if (LEndian) --PtrMem;
		else ++PtrMem;
		Pointeur++;
	}
	if (Fin<Pointeur) Fin=Pointeur;
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajoute des donn®¶es nulle pour tout type

PQ_RESULT	CPaquet::AddVoid(unsigned char Taille)
{
	if (Pointeur>(256 - Taille)) return PQ_BAD_LENGTH;
	for (size_t i = 0; i < Taille; i++)
	{
		Tampon[Pointeur]=0;
		Pointeur++;
	}
	if (Fin<Pointeur) Fin=Pointeur;
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Remplie le buffer avec un autre buffer simple

PQ_RESULT CPaquet::Set(unsigned char* Buffer, unsigned char Taille)
{
	if (Taille > 256) return PQ_BAD_LENGTH;
	Pointeur=0;
	for (int i=0; i <= Taille; i++)
	{
		Tampon[i]=Buffer[i];
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Renvoie le buffer complet avec sa taille

PQ_RESULT CPaquet::Get(unsigned char* Buffer, unsigned char Taille)
{
	if (!Taille) return PQ_BAD_LENGTH;
	if (Taille > 256) return PQ_BAD_LENGTH;
	for (int i=0; i < Taille; i++)
	{
		Buffer[i]=Tampon[i];
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Renvoie la taille du buffer

unsigned char CPaquet::GetSize ()
{
	return Fin;
}

//-----------------------------------------------------------------------------
// Renvoi un octet sign√©

PQ_RESULT CPaquet::GetByte (BYTE& Byte)
{
	return GetVoid((unsigned char*)(&Byte),sizeof(unsigned char));
}

//-----------------------------------------------------------------------------
// Renvoi un octet non sign√©

PQ_RESULT CPaquet::GetByte (UBYTE& Byte)
{
	return GetVoid((unsigned char*)(&Byte),sizeof(unsigned char));
}

//-----------------------------------------------------------------------------
// Saute un Octet

PQ_RESULT CPaquet::GetByte()
{
	return GetVoid(sizeof(unsigned char));
}

//-----------------------------------------------------------------------------
// Renvoi un mot sign√© (2 octets)

PQ_RESULT	CPaquet::GetWord(WORD& Word)
{
	return GetVoid((unsigned char*)(&Word), sizeof(WORD));
}

//-----------------------------------------------------------------------------
// Renvoi un mot non sign√© (2 octets)

PQ_RESULT	CPaquet::GetWord(UWORD& Word)
{
	return GetVoid((unsigned char*)(&Word), sizeof(WORD));
}

//-----------------------------------------------------------------------------
// Saute un Mot

PQ_RESULT CPaquet::GetWord()
{
	return GetVoid(sizeof(WORD));
}

//-----------------------------------------------------------------------------
// Renvoie un mot double sign√© (4 octets)

PQ_RESULT CPaquet::GetDWord(DWORD& DWord)
{
	return GetVoid((unsigned char*)(&DWord), sizeof(DWORD));
}

//-----------------------------------------------------------------------------
// Renvoie un mot double non sign√© (4 octets)

PQ_RESULT CPaquet::GetDWord(UDWORD& DWord)
{
	return GetVoid((unsigned char*)(&DWord), sizeof(DWORD));
}


//-----------------------------------------------------------------------------
// Saute un Mot Double

PQ_RESULT CPaquet::GetDWord()
{
	return GetVoid(sizeof(DWORD));
}

//-----------------------------------------------------------------------------
// Renvoie un nombre flottant

PQ_RESULT	CPaquet::GetFloat(float& Real)
{
	return GetVoid((unsigned char*)(&Real), sizeof(float));
}

//-----------------------------------------------------------------------------
// Saute un Flottant

PQ_RESULT CPaquet::GetFloat()
{
	return GetVoid(sizeof(float));
}

//-----------------------------------------------------------------------------
// Renvoie une chaine de charact√®re

PQ_RESULT CPaquet::GetString(String& Str)
{
	unsigned char Taille;
	Str="";
	if (GetByte(Taille)) return PQ_EOF;
	if (Pointeur >(256-Taille)) return PQ_BAD_LENGTH;
	for (size_t i=0; i<Taille; i++)
	{
		Str += Tampon[Pointeur];
		Pointeur++;
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Saute une chaine de charact√®res

PQ_RESULT CPaquet::GetString()
{
	unsigned char Taille;
	if (GetByte(Taille)) return PQ_EOF;
	return GetStream(Taille);
}

//-----------------------------------------------------------------------------
// Renvoie une chaine de charactere de grande longueur

PQ_RESULT CPaquet::GetLongString(String& Str)
{
	WORD Taille;
	Str="";
	if (GetWord (Taille)) return PQ_EOF;
	for (WORD i=0; i<Taille; ++i)
	{
		if (Pointeur>Fin) return PQ_EOF;
		Str += Tampon[Pointeur];
		Pointeur ++;
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Saute une chaine de charactere de grande longueur

PQ_RESULT CPaquet::GetLongString()
{
	WORD Taille;
	if (GetWord (Taille)) return PQ_EOF;
	return GetStream(Taille);
}

//-----------------------------------------------------------------------------
// Renvoie une suite d'octets

PQ_RESULT CPaquet::GetStream(unsigned char* Buffer, unsigned char Taille)
{
	for (unsigned char i=0; i<Taille ; ++i)
	{
		if (Pointeur>Fin) return PQ_EOF;
		Buffer[i]=Tampon[Pointeur];
		Pointeur++;
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Saute une suite d'octets

PQ_RESULT CPaquet::GetStream(unsigned char Taille)
{
	for (int i=0; i<Taille ; ++i)
	{
		if (Pointeur>Fin) return PQ_EOF;
		Pointeur++;
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Efface et r√©initialise le buffer

PQ_RESULT CPaquet::Clear()
{
	for (int i=0;i<256;i++)
	{
		Tampon[i]=0;
	}
	Pointeur=0;
	Fin=0;
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Donne la position du pointeur dans le buffer

unsigned char CPaquet::GetPos ()
{
	return Pointeur;
}

//-----------------------------------------------------------------------------
// Encapsulation d'un autre paquet

PQ_RESULT CPaquet::Add(CPaquet& Paquet)
{
	if ((Paquet.Fin + Pointeur) >= 255 ) return PQ_BAD_LENGTH;
	for (int i=0; i <= Paquet.Fin; i++)
	{
		Tampon[Pointeur] = Paquet.Tampon[i];
	}
	if (Fin<Pointeur) Fin=Pointeur;
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'un Octet sign√©

PQ_RESULT CPaquet::AddByte(BYTE Byte)
{
	return AddVoid((const unsigned char*)(&Byte), sizeof(unsigned char));
}

//-----------------------------------------------------------------------------
// Ajout d'un Octet non sign√©

PQ_RESULT CPaquet::AddByte(UBYTE Byte)
{
	return AddVoid((const unsigned char*)(&Byte), sizeof(unsigned char));
}

//-----------------------------------------------------------------------------
// Ajout d'un Octet appell√© aussi char

PQ_RESULT CPaquet::AddByte()
{
	return AddVoid (sizeof(unsigned char));
}

//-----------------------------------------------------------------------------
// Ajout d'un mot sign√©(2 octets)

PQ_RESULT CPaquet::AddWord(WORD Word)
{
	return AddVoid((const unsigned char*)(&Word), sizeof (WORD));
}

//-----------------------------------------------------------------------------
// Ajout d'un mot non sign√©(2 octets)

PQ_RESULT CPaquet::AddWord(UWORD Word)
{
	return AddVoid((const unsigned char*)(&Word), sizeof (WORD));
}

//-----------------------------------------------------------------------------
// Saute un mot (2 octets)

PQ_RESULT CPaquet::AddWord()
{
	return AddVoid (sizeof(WORD));
}

//-----------------------------------------------------------------------------
// Ajout d'un double mot sign√© (4 octets)

PQ_RESULT CPaquet::AddDWord(DWORD Dword)
{
	return AddVoid((const unsigned char*)(&Dword), sizeof(DWORD));
}

//-----------------------------------------------------------------------------
// Ajout d'un double mot non sign√© (4 octets)

PQ_RESULT CPaquet::AddDWord(UDWORD Dword)
{
	return AddVoid((const unsigned char*)(&Dword), sizeof(UDWORD));
}


//-----------------------------------------------------------------------------
// Ajout d'un double mot vide

PQ_RESULT	CPaquet::AddDWord()
{
	return AddVoid (sizeof(DWORD));
}

//-----------------------------------------------------------------------------
// Ajout d'un nombre flotant

PQ_RESULT	CPaquet::AddFloat(float Real)
{
	return AddVoid((const unsigned char*)(&Real), sizeof (float));
}


//-----------------------------------------------------------------------------
// Ajout d'un flottant vide

PQ_RESULT	CPaquet::AddFloat()
{
	return AddVoid (sizeof(float));
}

//-----------------------------------------------------------------------------
// Ajout d'une chaine de characteres au format PROTO AW

PQ_RESULT CPaquet::AddString(const String& Str)
{
	unsigned char Taille=Str.length();
	if (Taille >255) return PQ_TOO_LONG;
	if ((Pointeur + Taille + 1) > 256) return PQ_BAD_LENGTH;
	AddByte (Taille);
	for (unsigned char i=0; i<Taille; i++)
	{
		Tampon[Pointeur]=Str.charAt(i);
		Pointeur++;
	}
	if (Fin<Pointeur) Fin=Pointeur;
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'une chaine de characteres au format PROTO AW

PQ_RESULT CPaquet::AddString()
{
	AddByte ();
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'une chaine longue de charactere

PQ_RESULT CPaquet::AddLongString(const String& Str)
{
	WORD Taille = Str.length ();
	if (Taille > 200) Taille=200;
	if ((Pointeur + Taille + 2) > 256) return PQ_BAD_LENGTH;
	AddWord (Taille);
	for (WORD i=0; i<Taille; i++)
	{
		Tampon[Pointeur] = Str[i];
		Pointeur++;
	}
	if (Fin<Pointeur) Fin=Pointeur;
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Saut d'une chaine longue de charactere

PQ_RESULT CPaquet::AddLongString()
{
	WORD Word=0;
	return AddWord(Word);;
}

//-----------------------------------------------------------------------------
// Ajout d'une suite d'octets

PQ_RESULT CPaquet::AddStream(unsigned char* Buffer, unsigned char Taille)
{
	if ((Pointeur + Taille ) > 256) return PQ_BAD_LENGTH;
	for (unsigned char i=0; i < Taille; i++)
	{
		AddByte(Buffer[i]);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'une suite d'octets

PQ_RESULT CPaquet::AddStream(String& Str)
{
	if ((Pointeur + Str.length() ) > 256) return PQ_BAD_LENGTH;
	for (unsigned char i=0; i < Str.length(); ++i)
	{
		AddByte(Str[i]);
	}
	return PQ_OK;
}
