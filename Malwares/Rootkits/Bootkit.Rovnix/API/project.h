//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP user-mode API library
//	
// module: project.h
// $Revision: 77 $
// $Date: 2012-01-17 22:20:34 +0400 (Вт, 17 янв 2012) $
// description:
//  Common includes.

#include <windows.h>
#include <tchar.h>
#include <malloc.h>
#include <winioctl.h>   // Compiling Win32 Applications Or DLL's

#pragma warning(push)
#pragma warning(disable:4005) // macro redefinition
#include "..\inc\ntdll.h"
#include <ntstatus.h>
#pragma warning(pop)

#include "kipioctl.h"


// Predefined from names.c
PWSTR	CreateRandomDeviceName(PWSTR Template, ULONG Seed);
VOID	FreeRandomDeviceName(PWSTR Name);
