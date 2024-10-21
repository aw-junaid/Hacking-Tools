#ifndef WINDEFS_H
# define WINDEFS_H

# define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)

# include <winternl.h>
#include "resource.h"
#include "process.h"
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <iostream>

typedef struct _MY_SYSTEM_PROCESS_INFORMATION
{
    ULONG                   NextEntryOffset;
    ULONG                   NumberOfThreads;
    LARGE_INTEGER           Reserved[3];
    LARGE_INTEGER           CreateTime;
    LARGE_INTEGER           UserTime;
    LARGE_INTEGER           KernelTime;
    UNICODE_STRING          ImageName;
    ULONG                   BasePriority;
    HANDLE                  ProcessId;
    HANDLE                  InheritedFromProcessId;
} MY_SYSTEM_PROCESS_INFORMATION, *PMY_SYSTEM_PROCESS_INFORMATION;


typedef NTSTATUS(WINAPI *PNT_OPEN_FILE)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, ULONG, ULONG);

typedef NTSTATUS (WINAPI *PNT_CREATE_FILE)(PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength);

typedef enum _KEY_INFORMATION_CLASS {
	KeyBasicInformation = 0,
	KeyNodeInformation = 1,
	KeyFullInformation = 2,
	KeyNameInformation = 3,
	KeyCachedInformation = 4,
	KeyFlagsInformation = 5,
	KeyVirtualizationInformation = 6,
	KeyHandleTagsInformation = 7,
	MaxKeyInfoClass = 8
} KEY_INFORMATION_CLASS;

typedef struct _KEY_BASIC_INFORMATION {
	LARGE_INTEGER LastWriteTime;
	ULONG         TitleIndex;
	ULONG         NameLength;
	WCHAR         Name[1];
} KEY_BASIC_INFORMATION, *PKEY_BASIC_INFORMATION;

typedef struct _KEY_NODE_INFORMATION
{
	LARGE_INTEGER LastWriteTime;
	ULONG         TitleIndex;
	ULONG         ClassOffset;
	ULONG         ClassLength;
	ULONG         NameLength;
	WCHAR         Name[1];
	/* Class[1]; */
} KEY_NODE_INFORMATION, *PKEY_NODE_INFORMATION;

#endif
