//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vitual File System management tool
//	
// module: names.c
// $Revision: 74 $
// $Date: 2012-06-14 15:27:29 +0400 (Чт, 14 июн 2012) $
// description:
//  Name generation engine.

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

#define GUID_STR_LEN	16*2+4+2	// length of the GUID string in chars
#define tczGuidStrTempl	_T("{%08X-%04X-%04X-%04X-%08X%04X}")

#define	tczRoot			_T("\\\\.\\%s\\")


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Generates pseudo random number based on the specified seed value.
//
ULONG MyRandom(PULONG pSeed)
{
	return(*pSeed = 1664525*(*pSeed)+1013904223);
}


static ULONG GetSystemTimeStamp(VOID)
{
	LARGE_INTEGER	Frequency;
	LARGE_INTEGER	SystemTime;
	LARGE_INTEGER	TickCount;
	ULONGLONG	SecElapsed, SecStarted;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&TickCount);
	GetSystemTimeAsFileTime((LPFILETIME)&SystemTime);

	// Calculating number of seconds elapsed since the system started
	SecElapsed = (ULONGLONG)TickCount.QuadPart / (ULONGLONG)Frequency.QuadPart;

	// Calculating number of seconds elapsed since January 1, 1601 (UTC) before the system started
	SecStarted = (ULONGLONG)SystemTime.QuadPart / 10000000 - SecElapsed;

	// Calculating number of 3-day intervals elapsed since January 1, 1601 (UTC) before the system started
	SecStarted /= (3600 * 24 * 3);

	return((ULONG)(SecStarted));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Generates a GUID based on the specified seed value. The same seeds will create the same GUIDs on the same machine.
//
VOID GenGuid(GUID* pGuid, PULONG pSeed)
{
	ULONG i;
	pGuid->Data1 = MyRandom(pSeed);
	pGuid->Data2 = (USHORT)MyRandom(pSeed);
	pGuid->Data3 = (USHORT)MyRandom(pSeed);
	for (i=0; i<8; i++)
		pGuid->Data4[i] = (UCHAR)MyRandom(pSeed);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Converts the specified GUID structure into 0-terminated string.
//
LPTSTR GuidToString(GUID* pGuid)
{
	LPTSTR GuidStr = (LPTSTR)malloc((GUID_STR_LEN+1)*sizeof(_TCHAR));
	if (GuidStr)
		wsprintf(GuidStr, tczGuidStrTempl, pGuid->Data1, pGuid->Data2, pGuid->Data3, *(USHORT*)&pGuid->Data4[0], *(ULONG*)&pGuid->Data4[2],  *(USHORT*)&pGuid->Data4[6]);

	return(GuidStr);
}


LPTSTR	CreateFsDeviceName(VOID)
{
	ULONG	SysTimeStamp;
	LPTSTR	GuidName, DeviceName = NULL;
	GUID	Guid;

	SysTimeStamp = GetSystemTimeStamp();

	GenGuid(&Guid, &SysTimeStamp);
	if (GuidName = GuidToString(&Guid))
	{
		if (DeviceName = (LPTSTR)malloc((_tcslen(tczRoot) + GUID_STR_LEN + 1) * sizeof(_TCHAR)))
			wsprintf(DeviceName, tczRoot, GuidName);
		free(GuidName);		
	}

	return(DeviceName);
}

 

