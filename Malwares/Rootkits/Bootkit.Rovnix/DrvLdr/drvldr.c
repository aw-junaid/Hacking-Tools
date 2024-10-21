//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DrvLdr project.
//	
// module: drvldr.c
// $Revision: 67 $
// $Date: 2012-05-29 22:20:56 +0400 (Вт, 29 май 2012) $
// description: 
//	Loader for kernel-mode drivers.
//	Loads a file containing a kernel-mode diver. Allocates non paged pool and creates driver's image there.
//	Applies relocations, resolves driver's import and executes driver's entry.

#include <Ntifs.h>
#include <ntddk.h>
#include <ntimage.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"
#include "bklib.h"

#define		SYS_INFO_BUFFER_MAX		PAGE_SIZE*0x10


//
//	Returns base of the specified kernel module or NULL in case of an error.
//
PVOID _stdcall GetKernelModuleBase(
	PCHAR pModuleName	// Target module name (case sensitive)
	)
{
    PVOID		ModuleBase = NULL;
    PULONG		SysInfoBuffer = NULL;
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    ULONG		SysInfoBufferSize = SYS_INFO_BUFFER_MAX;

	do 
	{
		if (!(SysInfoBuffer = MyAllocatePool(PagedPool, SysInfoBufferSize)))
			// Not enough memory
			break;

		ntStatus = ZwQuerySystemInformation(SystemModuleInformation, SysInfoBuffer, SysInfoBufferSize, &SysInfoBufferSize);

		if (!(NT_SUCCESS(ntStatus)))
		{
			MyFreePool(SysInfoBuffer);
			SysInfoBufferSize += SYS_INFO_BUFFER_MAX;
		}
	} while(!NT_SUCCESS(ntStatus));

	if (SysInfoBuffer)
	{
		PSYSTEM_MODULE pSysModule = (PSYSTEM_MODULE) &((PSYSTEM_MODULE_INFORMATION)(SysInfoBuffer))->aSM;
		ULONG NumberModules = ((PSYSTEM_MODULE_INFORMATION)(SysInfoBuffer))->uCount;

		if (*(PUSHORT)pModuleName == 'tn' && strlen(pModuleName) == 12)
		{
			// Looking for NT base
			ModuleBase = (PVOID)pSysModule->Base;
		}
		else
		{
			do 
			{
				PCHAR	Dot, pShortName = (PCHAR)&pSysModule->ImageName + pSysModule->ModuleNameOffset;

				// Compare short name first
				if (_stricmp(pShortName, pModuleName) == 0)
				{
					ModuleBase = (PVOID)pSysModule->Base;
					break;
				}

				// Compare name without extension
				if (Dot = strchr(pShortName, '.'))
				{
					 Dot[0] = 0;
					 if (_stricmp(pShortName, pModuleName) == 0)
					 {
						 ModuleBase = (PVOID)pSysModule->Base;
						 break;
					 }
				 }	// if (Dot = strchr(ShortName, '.'))

				pSysModule += 1;
			} while(NumberModules -= 1);
		}

	    MyFreePool(SysInfoBuffer);
    }	// if (SysInfoBuffer)

    return(ModuleBase);
} 


//
//	Allocates a buffer of the specified file size and loads the specified file into it.
//
NTSTATUS	LoadFileData(
	PUNICODE_STRING	FilePath,
	PVOID*			pBuffer,
	PULONG_PTR		pLength
	)
{
	HANDLE		hFile, hSection;
	NTSTATUS	ntStatus;
	IO_STATUS_BLOCK		IoStatus;
	OBJECT_ATTRIBUTES	Oa;

	PVOID	Buffer;
	LARGE_INTEGER	Offset = {0};


	InitializeObjectAttributes(&Oa, FilePath, (OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE), NULL, NULL);
	ntStatus = ZwOpenFile (&hFile, FILE_READ_DATA | SYNCHRONIZE, &Oa, &IoStatus, FILE_SHARE_READ | FILE_SHARE_DELETE, 
		FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);

	if (NT_SUCCESS(ntStatus))
	{
		FILE_STANDARD_INFORMATION	StdInfo;
		ntStatus = ZwQueryInformationFile(hFile, &IoStatus, &StdInfo, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
		if (NT_SUCCESS(ntStatus))
		{
			if (StdInfo.EndOfFile.HighPart == 0 && StdInfo.EndOfFile.LowPart > 0)
			{
				if (Buffer = MyAllocatePool(PagedPool, StdInfo.EndOfFile.LowPart))
				{
					ntStatus = ZwReadFile(hFile, 0, NULL, NULL, &IoStatus, Buffer, StdInfo.EndOfFile.LowPart, &Offset, NULL);
					if (NT_SUCCESS(ntStatus))
					{
						if (StdInfo.EndOfFile.LowPart == (ULONG)IoStatus.Information)
						{
							*pBuffer = Buffer;
							*pLength = (ULONG_PTR)StdInfo.EndOfFile.LowPart;
						}
						else
							ntStatus = STATUS_FILE_CORRUPT_ERROR;
					}	// if (NT_SUCCESS(ntStatus))

					if (!NT_SUCCESS(ntStatus))
						MyFreePool(Buffer);

				}	// if (Buffer = MyAllocatePool(PagedPool, StdInfo.EndOfFile.LowPart))
				else
					ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			}	// if (StdInfo.EndOfFile.HighPart == 0 && StdInfo.EndOfFile.LowPart > 0)
			else	
				ntStatus = STATUS_MAPPED_FILE_SIZE_ZERO;
		}	// if (NT_SUCCESS(ntStatus))
		ZwClose(hFile);
	}	// if (NT_SUCCESS(ntStatus))

	return(ntStatus);
}


//
//	Allocates memory for a PE image and creates the image from the file data within the section specified.
//
NTSTATUS	AllocateAndCopyImage(
	PVOID		SectionBase,	// Mapped file data
	ULONG_PTR	SectionSize,	// Size of the file data
	PVOID*		pImageBase,		// Receives new image base
	PULONG_PTR	pImageSize,		// Receives new image size
	PVOID*		pImagePool		// Receives address of the memory buffer containing new image
	)
{
	NTSTATUS			ntStatus = STATUS_INVALID_IMAGE_FORMAT;
	PIMAGE_NT_HEADERS	Pe;
	ULONG				i, SizeOfHeaders;
	ULONG_PTR			ImageSize;
	PVOID				ImagePool = NULL, ImageBase;
	PIMAGE_SECTION_HEADER	PeSection;

	do	// not a loop
	{
		if (SectionSize < (FIELD_OFFSET(IMAGE_DOS_HEADER, e_lfanew) + sizeof(ULONG)))
			break;

		i = ((PIMAGE_DOS_HEADER)SectionBase)->e_lfanew;
		if (SectionSize < (i + sizeof(IMAGE_NT_HEADERS)))
			break;

		Pe = (PIMAGE_NT_HEADERS)((PCHAR)SectionBase + i);

		if (Pe->Signature != IMAGE_NT_SIGNATURE)
			break;

		if (Pe->OptionalHeader.Subsystem != IMAGE_SUBSYSTEM_NATIVE)
			break;

#ifdef _WIN64
		if (Pe->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
#else
		if (Pe->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
#endif
			break;

		PeSection = IMAGE_FIRST_SECTION(Pe);

		SizeOfHeaders = (ULONG)((ULONG_PTR)PeSection - (ULONG_PTR)SectionBase + sizeof(IMAGE_SECTION_HEADER) * Pe->FileHeader.NumberOfSections);
		if (SectionSize < SizeOfHeaders)
			break;

		ImageSize = _ALIGN(Pe->OptionalHeader.SizeOfImage, (Pe->OptionalHeader.SectionAlignment > PAGE_SIZE ? Pe->OptionalHeader.SectionAlignment : PAGE_SIZE));
		if (ImageSize < SectionSize)
			break;

		if (!(ImagePool = MyAllocatePool(NonPagedPool, ImageSize + PAGE_SIZE)))
		{
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}
		
		ImageBase = (PVOID)_ALIGN((ULONG_PTR)ImagePool, PAGE_SIZE);

		memset(ImageBase, 0, ImageSize);
		
		// Copying file headers
		memcpy(ImageBase, SectionBase, SizeOfHeaders);

		// Copying sections
		for (i=0; i<Pe->FileHeader.NumberOfSections; i++)
		{
			ULONG	PhysicalSize = _ALIGN(PeSection->SizeOfRawData, Pe->OptionalHeader.FileAlignment);
			ULONG	VirtualSize = _ALIGN(PeSection->Misc.VirtualSize, (Pe->OptionalHeader.SectionAlignment > PAGE_SIZE ? Pe->OptionalHeader.SectionAlignment : PAGE_SIZE));

			if (ImageSize < (PeSection->VirtualAddress + VirtualSize))
				break;

			if (SectionSize < (PeSection->PointerToRawData + PhysicalSize))
				break;

			memcpy((PCHAR)ImageBase + PeSection->VirtualAddress, (PCHAR)SectionBase + PeSection->PointerToRawData, PhysicalSize);

			PeSection += 1;
		}	// for (i=0; i<Pe->FileHeader.NumberOfSections; i++)

		if (i < Pe->FileHeader.NumberOfSections)
			break;

		*pImagePool = ImagePool;
		*pImageBase = ImageBase;
		*pImageSize = ImageSize;

		ntStatus = STATUS_SUCCESS;

	} while(FALSE);	

	if (!(NT_SUCCESS(ntStatus)))
	{
		if (ImagePool)
			MyFreePool(ImagePool);
	}

	return(ntStatus);
}


//
//	Relocates an image file that was not loaded into memory at the preferred address.
//
NTSTATUS	RelocateImage(
	PVOID ImageBase
	)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	ULONG	i;
	PIMAGE_NT_HEADERS	Pe = (PIMAGE_NT_HEADERS)((PCHAR)ImageBase + ((PIMAGE_DOS_HEADER)ImageBase)->e_lfanew);
	PIMAGE_DATA_DIRECTORY	DataDir;
	LONG	RelocSize;

	// Processing relocs
	DataDir = &Pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	if (DataDir->VirtualAddress && (RelocSize = DataDir->Size))
	{
		ULONG_PTR	BaseDelta;
		PIMAGE_BASE_RELOCATION_EX	Reloc = (PIMAGE_BASE_RELOCATION_EX)((PCHAR)ImageBase + DataDir->VirtualAddress);

		BaseDelta = ((ULONG_PTR)ImageBase - Pe->OptionalHeader.ImageBase);

		while(RelocSize > IMAGE_SIZEOF_BASE_RELOCATION)
		{
			ULONG	NumberRelocs = (Reloc->SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / sizeof(WORD);
			PCHAR	PageVa = (PCHAR)ImageBase + Reloc->VirtualAddress;

			if (RelocSize >= (LONG)Reloc->SizeOfBlock)
			{
				for (i=0; i<NumberRelocs; i++)
				{
					USHORT	RelocType = (Reloc->TypeOffset[i] >> IMAGE_REL_BASED_SHIFT);

					switch(RelocType)
					{
					case IMAGE_REL_BASED_ABSOLUTE:
						// Do nothing. This one is used just for alingment.
						break;
					case IMAGE_REL_BASED_HIGHLOW:
						*(PULONG)(PageVa + (Reloc->TypeOffset[i] & IMAGE_REL_BASED_MASK)) += (ULONG)BaseDelta;
						break;
#ifdef _WIN64
					case IMAGE_REL_BASED_DIR64:
						*(PULONG_PTR)(PageVa + (Reloc->TypeOffset[i] & IMAGE_REL_BASED_MASK)) += BaseDelta;
						break;
#endif
					default:
						ASSERT(FALSE);
						break;
					}	// switch(RelocType)
				}	// for (i=0; i<NumberRelocs; i++)
			}	// if (RelocSize >= (LONG)Reloc->SizeOfBlock)
			RelocSize -= (LONG)Reloc->SizeOfBlock;
			Reloc = (PIMAGE_BASE_RELOCATION_EX)((PCHAR)Reloc + Reloc->SizeOfBlock);
		}	// while(RelocSize > IMAGE_SIZEOF_BASE_RELOCATION)
	}	// if (!ImageAtBase && DataDir->VirtualAddress && (RelocSize = DataDir->Size)

	return(ntStatus);
}


NTSTATUS	DrvLdrLoadDriver(
	PUNICODE_STRING	ImagePath
	)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PVOID		SectionBase, ImagePool = NULL, ImageBase;
	ULONG_PTR	SectionSize, ImageSize;
	BOOL		LoadFromVfs = TRUE;

	ntStatus = LoadFileData(ImagePath, &SectionBase, &SectionSize);

	if (NT_SUCCESS(ntStatus))
	{
		ntStatus = AllocateAndCopyImage(SectionBase, SectionSize, &ImageBase, &ImageSize, &ImagePool);
		MyFreePool(SectionBase);
	}

	if (NT_SUCCESS(ntStatus))
	{
		do 
		{
			PIMAGE_NT_HEADERS	Pe = (PIMAGE_NT_HEADERS)((PCHAR)ImageBase + ((PIMAGE_DOS_HEADER)ImageBase)->e_lfanew);
			PDRIVER_INITIALIZE	DriverEntry;

			ntStatus = RelocateImage(ImageBase);
			if (!NT_SUCCESS(ntStatus))
				break;

			ntStatus = BkResolveImageImport(ImageBase, &GetKernelModuleBase);
			if (!NT_SUCCESS(ntStatus))
				break;

			DriverEntry = (PDRIVER_INITIALIZE)((PCHAR)ImageBase + Pe->OptionalHeader.AddressOfEntryPoint);

			ntStatus = IoCreateDriver(NULL, DriverEntry);
		} while(FALSE);
	}

	if (!NT_SUCCESS(ntStatus) && (ImagePool))
		MyFreePool(ImagePool);
	
	return(ntStatus);
}