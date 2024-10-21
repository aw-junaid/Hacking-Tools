//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: pesup.h
// $Revision: 84 $
// $Date: 2012-07-20 17:16:28 +0400 (Пт, 20 июл 2012) $
// description: 
//	BK helper library. PE-structure support routines.

#pragma once
#include <winnt.h>


typedef ULONG_PTR	IAT_ENTRY;
typedef IAT_ENTRY*	PIAT_ENTRY;

typedef ULONG			EXPORT_ENTRY;
typedef EXPORT_ENTRY*	PEXPORT_ENTRY;

typedef	PVOID (_stdcall* PPE_GET_MODULE_BASE)		(PCHAR	ModuleName);	


_inline PVOID PeSupGetImagePeHeader(PCHAR DosHeader)
{
	PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)(DosHeader + ((PIMAGE_DOS_HEADER)DosHeader)->e_lfanew);
	return((PVOID)pe);
}


#define		PeSupGetOptionalField(PeHeader, Field)												\
				(FIELD_OFFSET(IMAGE_OPTIONAL_HEADER32, ##Field) != FIELD_OFFSET(IMAGE_OPTIONAL_HEADER64, ##Field) && \
				((PIMAGE_NT_HEADERS32)PeHeader)->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 ?	\
				((PIMAGE_NT_HEADERS64)PeHeader)->OptionalHeader.##Field :								\
				((PIMAGE_NT_HEADERS32)PeHeader)->OptionalHeader.##Field)

#define		PeSupGetDirectoryEntryPtr(PeHeader, Entry)												\
				(((PIMAGE_NT_HEADERS32)PeHeader)->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 ?		\
				&((PIMAGE_NT_HEADERS64)PeHeader)->OptionalHeader.DataDirectory[##Entry] :					\
				&((PIMAGE_NT_HEADERS32)PeHeader)->OptionalHeader.DataDirectory[##Entry])


_inline PVOID PeSupRvaToVa(ULONG Rva, PCHAR ImageBase)
{
	return((PVOID)((ULONG_PTR)Rva + (ULONG_PTR)ImageBase));

}


#define	PeSupAlign(Size, Alingment)		((Size + (Alingment - 1)) & (~(Alingment - 1 )))

//	The list of loded modules. Used by PeSupGetModuleBase function. Must be initialized by the caller.
PLIST_ENTRY	PeSupLoadedModuleList;

//	Searches for Export table entry of a specified function within a specified module.
PEXPORT_ENTRY	PeSupGetExportEntry( 
	PVOID	 TargetModule,			// Image base of the target module (where exported function located)
	PCHAR    ExportedFunction		// exported function name
	);

// Retrieves a module base for the specified module from the specified list of loaded modules.
PVOID	PeSupGetModuleBase(
	PCHAR	ModuleName
	);

// Retrieves the address of an exported function or variable from the specified module.
PVOID	PeSupGetFunctionAddress(
	PVOID	ImageBase,			// target module base
	PCHAR	FunctionName		// target function name
	);

//	Resolves specified image import based on the specified list of loaded modules.
NTSTATUS	PeSupResolveImageImport(
	PVOID	ImageBase,
	PPE_GET_MODULE_BASE	GetModuleBase
	);