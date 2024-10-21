//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK initial loader builder. Version 2.9.2
//	
// module: main.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description:
//	Common includes, constants and predifinitions.

#pragma once

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

#pragma warning(push)
#pragma warning(disable:4005) // macro redefinition
#include "..\inc\ntdll.h"
#include <ntstatus.h>
#pragma warning(pop)

#define	PAGE_SIZE	0x1000


// DbgPrint() and checked heap allocations
#include "..\inc\dbg.h"

// Lists support 
#include "listsup.h"

// Macros
#define MAX_PATH_BYTES (MAX_PATH*sizeof(_TCHAR))

// constant string length
#define	cstrlenA(str)	(sizeof(str)-1)
#define	cstrlenW(str)	(sizeof(str)-sizeof(WCHAR))/sizeof(WCHAR)

#if _UNICODE
	#define cstrlen(str)	cstrlenW(str)
#else
	#define cstrlen(str)	cstrlenA(str)
#endif

#define APP_VERSION	1


_inline ULONG rand32(VOID)
{
	return(((rand()%0x100) << 24) + ((rand()%0x100) << 16) + ((rand()%0x100) << 8) + (rand()%0x100));
}


#ifdef __cplusplus
extern "C" {
#endif

// Application defined memory allocation\freeing routines used by different static libraries
PVOID __stdcall	AppAlloc(ULONG Size);
VOID __stdcall	AppFree(PVOID pMem);

// Application defined rand() routine used by different static libraries
ULONG __stdcall AppRand(VOID);

#ifdef __cplusplus
}
#endif