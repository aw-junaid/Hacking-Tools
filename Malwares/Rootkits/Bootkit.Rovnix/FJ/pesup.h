//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File Joiner utility.
//	
// module: pesup.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	PE file header support functions and types.

#pragma once
#include <winnt.h>


_inline PVOID PeSupGetImagePeHeader(PVOID DosHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)((PCHAR)DosHeader + ((PIMAGE_DOS_HEADER)DosHeader)->e_lfanew);
	return((PVOID)pe);
}


#define		PeSupGetOptionalField(PeHeader, Field)												\
			(((PIMAGE_NT_HEADERS32)PeHeader)->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 ?	\
				((PIMAGE_NT_HEADERS64)Pe)->OptionalHeader.##Field :								\
				((PIMAGE_NT_HEADERS32)Pe)->OptionalHeader.##Field)

#define		PeSupSetOptionalField(PeHeader, Field, Value)										\
			(((PIMAGE_NT_HEADERS32)PeHeader)->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 ?	\
				((PIMAGE_NT_HEADERS64)Pe)->OptionalHeader.##Field = Value :						\
				((PIMAGE_NT_HEADERS32)Pe)->OptionalHeader.##Field = Value)


_inline ULONG PeSupGetNumberOfSections(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return((ULONG)pe->FileHeader.NumberOfSections);
}

_inline PVOID PeSupGetSectionTable(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return((PVOID)((PCHAR)&pe->OptionalHeader + pe->FileHeader.SizeOfOptionalHeader));
}


_inline ULONG PeSupAlign(ULONG Size, ULONG Alignment)
{
	ULONG AlignedSize = Size & ~(Alignment-1);
				
	if (Size != AlignedSize)
		AlignedSize += Alignment;

	return(AlignedSize);
}




