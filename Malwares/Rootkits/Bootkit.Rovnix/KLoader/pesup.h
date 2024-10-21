//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KLoader project Version 2.9.2
//	
// module: pesup.h
// $Revision: 45 $
// $Date: 2012-05-09 13:18:54 +0400 (Ср, 09 май 2012) $
// description:
//	PE file header support functions and types.

#pragma once

typedef union _SECTION_NAME
{
	UCHAR	Byte[8];
	ULONG	Dword[2];
} SECTION_NAME, *PSECTION_NAME;

typedef struct _LINKED_BUFFER  LINKED_BUFFER, *PLINKED_BUFFER;

typedef struct _LINKED_BUFFER
{
	PLINKED_BUFFER	Next;		// Next buffer in list, 0 if current is the last one
	PCHAR			Buffer;		// Pointer to memory buffer
	ULONG			Size;		// Size of the memory buffer in bytes
} LINKED_BUFFER, *PLINKED_BUFFER;

typedef struct _IMAGE_DELAY_IMPORT_DESCRIPTOR
{
	DWORD		Flags;
	DWORD		Name;
	DWORD		Module;
	DWORD		FirstThunk;
	DWORD		OriginalFirstThunk;
	DWORD		BoundIAT;
	DWORD		UnloadIAT;
	DWORD		TimeDateStamp;
} IMAGE_DELAY_IMPORT_DESCRIPTOR, *PIMAGE_DELAY_IMPORT_DESCRIPTOR;


typedef ULONG_PTR	IAT_ENTRY;
typedef IAT_ENTRY*	PIAT_ENTRY;

typedef ULONG			EXPORT_ENTRY;
typedef EXPORT_ENTRY*	PEXPORT_ENTRY;


_inline PVOID PeSupGetImagePeHeader(PVOID DosHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)((PCHAR)DosHeader + ((PIMAGE_DOS_HEADER)DosHeader)->e_lfanew);
	return((PVOID)pe);
}


#define		PeSupGetOptionalField(PeHeader, Field)												\
				(FIELD_OFFSET(IMAGE_OPTIONAL_HEADER32, ##Field) != FIELD_OFFSET(IMAGE_OPTIONAL_HEADER64, ##Field) && \
				((PIMAGE_NT_HEADERS32)PeHeader)->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 ?	\
				((PIMAGE_NT_HEADERS64)PeHeader)->OptionalHeader.##Field :								\
				((PIMAGE_NT_HEADERS32)PeHeader)->OptionalHeader.##Field)

#define		PeSupSetOptionalField(PeHeader, Field, Value)										\
				(FIELD_OFFSET(IMAGE_OPTIONAL_HEADER32, ##Field) != FIELD_OFFSET(IMAGE_OPTIONAL_HEADER64, ##Field) && \
				((PIMAGE_NT_HEADERS32)PeHeader)->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 ?	\
				((PIMAGE_NT_HEADERS64)PeHeader)->OptionalHeader.##Field = Value :						\
				((PIMAGE_NT_HEADERS32)PeHeader)->OptionalHeader.##Field = Value)

#define		PeSupPtrOptionalField(PeHeader, Field)										\
				(FIELD_OFFSET(IMAGE_OPTIONAL_HEADER32, ##Field) != FIELD_OFFSET(IMAGE_OPTIONAL_HEADER64, ##Field) && \
				((PIMAGE_NT_HEADERS32)PeHeader)->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 ?	\
				(PVOID)&((PIMAGE_NT_HEADERS64)PeHeader)->OptionalHeader.##Field:						\
				(PVOID)&((PIMAGE_NT_HEADERS32)PeHeader)->OptionalHeader.##Field)


#define		PeSupGetDirectoryEntryPtr(PeHeader, Entry)												\
				(((PIMAGE_NT_HEADERS32)PeHeader)->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 ?		\
				&((PIMAGE_NT_HEADERS64)PeHeader)->OptionalHeader.DataDirectory[##Entry] :					\
				&((PIMAGE_NT_HEADERS32)PeHeader)->OptionalHeader.DataDirectory[##Entry])


_inline ULONG_PTR PeSupGetImageBase(PCHAR DosHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)(DosHeader + ((PIMAGE_DOS_HEADER)DosHeader)->e_lfanew);
	return((ULONG_PTR)pe->OptionalHeader.ImageBase);

}

_inline ULONG_PTR* PeSupGetImageBaseOfs(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return(&pe->OptionalHeader.ImageBase);

}


_inline ULONG PeSupGetImageSize(PCHAR DosHeader)
{
	PIMAGE_NT_HEADERS32		PEHeader32	= (PIMAGE_NT_HEADERS32)PeSupGetImagePeHeader(DosHeader);
	PIMAGE_NT_HEADERS64		PEHeader64	= (PIMAGE_NT_HEADERS64)PEHeader32;
	ULONG	ImageSize;

	if (PEHeader32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
		ImageSize = PEHeader64->OptionalHeader.SizeOfImage;
	else
		ImageSize = PEHeader32->OptionalHeader.SizeOfImage;

	return(ImageSize);
}



_inline ULONG_PTR PeSupGetPeImageBase(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return((ULONG_PTR)pe->OptionalHeader.ImageBase);

}

_inline ULONG PeSupGetSectionRva(PVOID Section)
{
	PIMAGE_SECTION_HEADER sections = (PIMAGE_SECTION_HEADER)Section;
	return(sections->VirtualAddress);
}


_inline ULONG PeSupGetSectionVSize(PVOID Section)
{
	PIMAGE_SECTION_HEADER sections = (PIMAGE_SECTION_HEADER)Section;
	return(sections->Misc.VirtualSize);
}

_inline ULONG PeSupGetSectionRSize(PVOID Section)
{
	PIMAGE_SECTION_HEADER sections = (PIMAGE_SECTION_HEADER)Section;
	return(sections->SizeOfRawData);
}

_inline PVOID PeSupGetFirstWritableSection(PCHAR DosHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)(DosHeader + ((PIMAGE_DOS_HEADER)DosHeader)->e_lfanew);
	PIMAGE_SECTION_HEADER sections = (PIMAGE_SECTION_HEADER)((PCHAR)&pe->OptionalHeader + pe->FileHeader.SizeOfOptionalHeader);
	ULONG NumberOfSections = pe->FileHeader.NumberOfSections;

	do 
	{
		if (sections->Characteristics & IMAGE_SCN_MEM_WRITE)
			return((PVOID)sections);

		sections += 1;
		NumberOfSections -= 1;
	} while (NumberOfSections);

	return(NULL);
}	

_inline ULONG PeSupGetNumberOfSections(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return((ULONG)pe->FileHeader.NumberOfSections);
}

_inline ULONG PeSupGetImageIatVa(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return((ULONG)pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
}

_inline ULONG PeSupGetImageIatSize(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return((ULONG)pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);
}


_inline PVOID PeSupGetSectionTable(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return((PVOID)((PCHAR)&pe->OptionalHeader + pe->FileHeader.SizeOfOptionalHeader));
}

_inline ULONG PeSupGetSectionTableSize(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return(pe->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));
}


_inline PVOID PeSupGetNextSection(PVOID Section)
{
	return((PVOID)((ULONG_PTR)Section + sizeof(IMAGE_SECTION_HEADER)));
}


_inline ULONG PeSubGetSectionFlags(PVOID Section)
{
	return(((PIMAGE_SECTION_HEADER)Section)->Characteristics);
}

_inline ULONG PeSupGetSectionOffset(PVOID Section)
{
	return(((PIMAGE_SECTION_HEADER)Section)->PointerToRawData);
}

_inline ULONG PeSupGetSectionAlignment(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	return(pe->OptionalHeader.SectionAlignment);
}


_inline ULONG PeSupAlign(ULONG Size, ULONG Alignment)
{
	ULONG AlignedSize = Size & ~(Alignment-1);
				
	if (Size != AlignedSize)
		AlignedSize += Alignment;

	return(AlignedSize);
}

_inline PVOID PeSupFindSectionByName(PCHAR DosHeader, PSECTION_NAME SecName)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)(DosHeader + ((PIMAGE_DOS_HEADER)DosHeader)->e_lfanew);
	ULONG NumberOfSections = pe->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)((PCHAR)&pe->OptionalHeader + pe->FileHeader.SizeOfOptionalHeader);
	PVOID pFound = NULL;

	do
	{
		PSECTION_NAME pName = (PSECTION_NAME)&pSection->Name;
		if (pName->Dword[0] == SecName->Dword[0] && pName->Dword[1] == SecName->Dword[1])
			pFound = (PVOID)pSection;

		pSection += 1;
		NumberOfSections -=1;
	} while((NumberOfSections) && (pFound == NULL));
	
	return(pFound);
}

_inline LONG PeSupIsPe64(PVOID PeHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)PeHeader;
	if (pe->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
		return(TRUE);
	else
		return(FALSE);
}

_inline PVOID PeSupRvaToVa(ULONG Rva, PCHAR ImageBase)
{
	return((PVOID)((ULONG_PTR)Rva + (ULONG_PTR)ImageBase));

}


_inline ULONG	PeSupGetFileAlign(PVOID	PeHeader)
{
	PIMAGE_NT_HEADERS32	Pe = (PIMAGE_NT_HEADERS32)PeHeader;
	if (Pe->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
		return(((PIMAGE_NT_HEADERS64)Pe)->OptionalHeader.FileAlignment);
	else
		return(Pe->OptionalHeader.FileAlignment);
}







#define PeSupGetFreeCodeSpace(x)	PeSupGetSectionFreeBuffers(x, IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_DISCARDABLE)