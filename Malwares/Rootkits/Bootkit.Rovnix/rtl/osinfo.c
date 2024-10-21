/*************************************************************************/
/*                                                                       */
/* Copyright (c) 2000-2011 NT KERNEL RESOURCES, All Rights Reserved.     */
/* http://www.ntkernel.com                                               */
/*                                                                       */
/* Module Name:  osinfo.c                                                */
/*                                                                       */
/* Abstract: OS major,minor version and build number                     */
/*                                                                       */
/* Environment:                                                          */
/*                                                                       */
/*   Kernel mode, local network monitor driver                           */
/*                                                                       */
/* Revision History:                                                     */
/*                                                                       */
/*************************************************************************/
#include "project.h"

static ULONG OsMajorVersion = 0;
static ULONG OsMinorVersion = 0;
static ULONG OsBuildNumber  = 0;
static ULONG OsServicePack  = 0;

VOID
	OsGetVersion(
		VOID
		)
{
	NTSTATUS ntStatus;
	OBJECT_ATTRIBUTES ObjectAttributes;
	UNICODE_STRING KeyName = 
		RTL_CONSTANT_STRING(L"\\REGISTRY\\MACHINE\\SYSTEM\\CURRENTCONTROLSET\\CONTROL\\WINDOWS");
	UNICODE_STRING ValueName = RTL_CONSTANT_STRING(L"CSDVersion");
	UCHAR buffer[sizeof( KEY_VALUE_PARTIAL_INFORMATION ) + sizeof(ULONG)];
	PKEY_VALUE_PARTIAL_INFORMATION ValuePartialInfo = (PKEY_VALUE_PARTIAL_INFORMATION)buffer;
	HANDLE hKey;
	ULONG ReturnLength = 0;
	ULONG CSDVersion = 0;

	if ( OsMajorVersion != 0 ){
		return;
	}

	InitializeObjectAttributes( 
		&ObjectAttributes,
		&KeyName,
		OBJ_CASE_INSENSITIVE,
		NULL,NULL
		);

	ntStatus = 
		ZwOpenKey( 
			&hKey, 
			KEY_READ, 
			&ObjectAttributes 
			);
	if ( NT_SUCCESS( ntStatus ) ){
		ntStatus = 
			ZwQueryValueKey( 
				hKey,
				&ValueName,
				KeyValuePartialInformation,
				buffer,
				sizeof(buffer),
				&ReturnLength 
				);

		if ( NT_SUCCESS ( ntStatus ) ){
			if ( ValuePartialInfo->Type == REG_DWORD &&
				ValuePartialInfo->DataLength == sizeof(CSDVersion))
			{
				CSDVersion = *((PULONG)ValuePartialInfo->Data);
				OsServicePack = ( CSDVersion >> 8 );
			}
		}
		ZwClose( hKey );
	}

	#pragma prefast(suppress:311, "Needed for backward compatibility")
	PsGetVersion( 
		&OsMajorVersion, 
		&OsMinorVersion, 
		&OsBuildNumber,
		NULL
		);
}

ULONG
	OsGetMajorVersion(
		VOID
		)
{
	return OsMajorVersion;
}

ULONG
	OsGetMinorVersion(
		VOID
		)
{
	return OsMinorVersion;
}

ULONG
	OsGetServicePack(
		VOID
		)
{
	return OsServicePack;
}

ULONG
	OsGetBuildNumber(
		VOID
		)
{
	return OsBuildNumber;
}