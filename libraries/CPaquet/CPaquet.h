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
// *              CPAQUET.HPP : Classe de formatage des paquets tcp           *
// *                                                                          *
// *                      Neophile (c) 2006 : 09/02/2007                      *
// *      Rev B : 18/03/07                                                    *
// *                                                                          *
// ****************************************************************************
// * Rev A:Neophile:27/03/2007;Description :                                  *
// *                                Ajout du type APaquet pour la gestion     *
// *                                De la pile d'envoi des paquets            *
// ****************************************************************************

#ifndef CPAQUET_HPP
#define CPAQUET_HPP

#include "Arduino.h"

enum PQ_RESULT
{
	PQ_OK=0,
	PQ_EOF,
	PQ_BAD_LENGTH,
	PQ_TOO_LONG
};
typedef char BYTE; // Signed Byte (8bits)
typedef unsigned char UBYTE; // Unsigned Byte (8bits)
typedef short (WORD); // Signed Word (16 Bits) 
typedef unsigned short UWORD; // Unsigned Word (16 Bits)
typedef long DWORD; // Signed Double Word (32Bit)
typedef unsigned long UDWORD; // Unsigned Double Word (32 Bits)

class CPaquet
{
	private:
		BYTE  Tampon[256];
		BYTE	Pointeur;
		BYTE	Fin;

		bool			LEndian;
		PQ_RESULT		GetVoid			(unsigned char* Data, unsigned char Taille);
		PQ_RESULT		GetVoid			(unsigned char Taille);
		PQ_RESULT		AddVoid			(const unsigned char* Data, unsigned char Taille);
		PQ_RESULT		AddVoid			(unsigned char Taille);

	public:
						CPaquet			(bool Endian = true);
						CPaquet			(unsigned char* Buffer, unsigned char Taille,bool Endian = true);
						~CPaquet		();
		PQ_RESULT		Set				(unsigned char* Buffer, unsigned char Taille);
		PQ_RESULT		Get				(unsigned char* Buffer, unsigned char Taille);
		unsigned char	GetSize			();
		PQ_RESULT		GetByte			(BYTE& Byte);
		PQ_RESULT		GetByte			(UBYTE& Byte);
		PQ_RESULT		GetByte			();
		PQ_RESULT		GetWord			(WORD& Word);
		PQ_RESULT		GetWord			(UWORD& Word);
		PQ_RESULT		GetWord			();
		PQ_RESULT		GetDWord		(DWORD& DWord);
		PQ_RESULT		GetDWord		(UDWORD& DWord);
		PQ_RESULT		GetDWord		();
		PQ_RESULT		GetFloat		(float& Real);
		PQ_RESULT		GetFloat		();
		PQ_RESULT		GetString		(String& Str);
		PQ_RESULT		GetString		();
		PQ_RESULT		GetLongString	(String& Str);
		PQ_RESULT		GetLongString	();
		PQ_RESULT		GetStream		(unsigned char* Buffer, unsigned char Taille);
		PQ_RESULT		GetStream		(unsigned char Taille);
		PQ_RESULT		Clear				();
		void				Begin				(){Pointeur=0;}
		unsigned char	GetPos			();
		PQ_RESULT		Add				(CPaquet& Paquet);
		PQ_RESULT		AddByte			(BYTE Byte);
		PQ_RESULT		AddByte			(UBYTE Byte);
		PQ_RESULT		AddByte			();
		PQ_RESULT		AddWord			(WORD Word);
		PQ_RESULT		AddWord			(UWORD Word);
		PQ_RESULT		AddWord			();
		PQ_RESULT		AddDWord		(DWORD DWord);
		PQ_RESULT		AddDWord		(UDWORD DWord);
		PQ_RESULT		AddDWord		();
		PQ_RESULT		AddFloat		(float Real);
		PQ_RESULT		AddFloat		();
		PQ_RESULT		AddString		(const String& Str);
		PQ_RESULT		AddString		();
		PQ_RESULT		AddLongString	(const String& Str);
		PQ_RESULT		AddLongString	();
		PQ_RESULT		AddStream		(unsigned char* Buffer, unsigned char Taille);
		PQ_RESULT		AddStream		(String& Str);
};

#endif
