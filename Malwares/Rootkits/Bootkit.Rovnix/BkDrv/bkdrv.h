//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BkDrv project
//	
// module: bkdrv.h
// $Revision: 52 $
// $Date: 2012-05-15 19:24:23 +0400 (Вт, 15 май 2012) $
// description: 
//	BK additional driver-support routines.

#define	wczDeviceName	L"\\Device\\"
#define	wczLinkName		L"\\DosDevices\\"

typedef	struct _BK_DRIVER_DISPATCH_TABLE
{
	LIST_ENTRY			Entry;
	PDRIVER_DISPATCH	MajorFunction[IRP_MJ_MAXIMUM_FUNCTION + 1];
} BK_DRIVER_DISPATCH_TABLE, *PBK_DRIVER_DISPATCH_TABLE;

//
//	Initializes dispatch table for the specified driver with BK-specific dispatch callbacks.
//	Calling this routine required to use multiple dispatch tables.
//
VOID BkInitDriverDispatch(
	PVOID pDriverObject
	);


//
// Allocates and pre-initializes driver dispatch table.
//
PBK_DRIVER_DISPATCH_TABLE	BkAllocateDriverDispatchTable(VOID);

//
//	Registers dispatch table for an internal driver.
//
VOID BkRegisterDriverDispatchTable(
	PBK_DRIVER_DISPATCH_TABLE pDriverDispatchTable
	);


//
//	Generates pseudo-random GUID-style string based on a day the system started up and the specified Seed value.
//
PWCHAR	BkGenerateGuidString(
	ULONG Seed
	);


//
//	Create pseudo-random named device.
//
NTSTATUS	BkCreateRandomNamedDevice(
	PDRIVER_OBJECT		DriverObject,
	ULONG				Seed,
	ULONG				DeviceType,
	PUNICODE_STRING*	pDeviceName,
	PDEVICE_OBJECT*		pDeviceObject
	);


//
//	Create pseudo-random named symbolic link.
//
NTSTATUS BkCreateRandomNamedLink(
	PUNICODE_STRING uDeviceName, 
	ULONG Seed, 
	PUNICODE_STRING* pLinkName
	);



//
//	Generates a GUID based on the specified seed value. The same seeds will create the same GUIDs on the same machine.
//
VOID BkGuidFromSeed(
	GUID* pGuid, 
	PULONG pSeed
	);


PUNICODE_STRING	BkAllocateUnicodeString(USHORT MaximumLength);
VOID	BkFreeUnicodeString(PUNICODE_STRING	pUs);

ULONG	BkCRC32(PCHAR pMem, ULONG uLen);

// String manipulation routines:
PCHAR	strtrim(PCHAR Str, PCHAR SubChars);
ULONG	strtol(PCHAR Str, PCHAR* End, LONG Radix);

ULONG	BufferGetLine(PCHAR Buffer, ULONG Size,	PCHAR* pLine, PULONG pLineSize);
ULONG	ParseCommandLine(PCHAR CmdLine, PCHAR* pParam0, PCHAR* pParam1, PCHAR* pParam2, PCHAR* pParam3);