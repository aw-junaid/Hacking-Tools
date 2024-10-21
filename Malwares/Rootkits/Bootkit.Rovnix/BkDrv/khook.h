//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BkDrv project
//	
// module: khook.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Kernel-mode hoking engine implementation.

#pragma once

// Hook flags
#define HOOK_SET			0x100
#define HOOK_CAN_FAIL		0x200

// Hook types
#define HOOK_TYPE_IAT		1
#define HOOK_TYPE_EXPORT	2
#define	HOOK_TYPE_POINTER	HOOK_TYPE_IAT
#define HOOK_TYPE_MASK		0xff

typedef ULONG_PTR	IAT_ENTRY;
typedef IAT_ENTRY*	PIAT_ENTRY;

typedef ULONG			EXPORT_ENTRY;
typedef EXPORT_ENTRY*	PEXPORT_ENTRY;

#define MAX_STUB_SIZE	100

typedef struct _HOOK_FUNCTION
{
	PCHAR		HokedModule;	// Name of a module where HookedFunction located (exported)
	PCHAR		HookedFunction;	// Name of a hooked function
	PVOID		Address;		// Hook function
	PVOID		Stub;			// Address of hook stub if used.
	PVOID		Original;		// Address of the original function
} HOOK_FUNCTION, *PHOOK_FUNCTION;

// Hook structures
typedef struct _HOOK
{
	LIST_ENTRY		Entry;			// Hook list entry
	PHOOK_FUNCTION	HookFn;			// Current hook function
	PVOID			OriginalEntry;	// Original function entry (for IAT or Export hooks)
	ULONG_PTR		OriginalValue;	// Original value for IDT and Export hooks
	ULONG			Flags;			// Hook flags
} HOOK, *PHOOK;

// Defination of a HOOK structure
#define DEFINE_HOOK(pHookFn, HookingType)	\
{{0, 0}, pHookFn, NULL, 0, HookingType}
	


#define OP_JMP_NEAR			0xe9
#define OP_JMP_DWORD_PTR	0x25ff

// temporary macros for call stub
#define OP_POP_EAX 0x58
#define OP_PUSH_DWORD 0x68
#define OP_PUSH_EAX 0x50

#pragma pack(push)
#pragma pack(1)
typedef struct _JMP_STUB32
{
	// JMP NEAR XXXX instruction
	UCHAR	Opcode;		// must be 0xe9
	ULONG	Offset;		// jump offset
} JMP_STUB32, *PJMP_STUB32;

typedef struct _JMP_STUB64
{
	// JMP QWORD PTR [$+6]/DQ XXXXXXXX instructions
	USHORT		Opcode;		// must be 0x25ff
	ULONG		Offset;		// must be 0
	ULONG_PTR	Address;	// jump address
} JMP_STUB64, *PJMP_STUB64;

typedef struct _CALL_STUB
{
	UCHAR	OpPopEax;		// must be 0x58
	UCHAR	OpPushDword;		// must be 0x68
	ULONG	Ptr;
	UCHAR	OpPushEax;		// must be 0x50
	// JMP NEAR XXXX instruction
	UCHAR	OpJmp;		// must be 0xe9
	ULONG	Offset;		// jump offset
}CALL_STUB,*PCALL_STUB;

//todo define 64 stub


#ifdef _WIN64
	typedef 	JMP_STUB64			JMP_STUB;
	typedef 	PJMP_STUB64			PJMP_STUB;
	#define		JMP_STUB_OPCODE		OP_JMP_DWORD_PTR

#else
	typedef		JMP_STUB32			JMP_STUB;
	typedef		PJMP_STUB32			PJMP_STUB;
	#define		JMP_STUB_OPCODE		OP_JMP_NEAR
#endif
#pragma pack(pop)

// Functions

NTSTATUS	HookInit(VOID);
NTSTATUS	SetMultipleHooks(PHOOK	pHook, LONG NumberHooks, PVOID ModuleBase);
ULONG		RemoveAllHooks(PHOOK_FUNCTION pHookFn);
VOID		SetHook(PHOOK pHook, PVOID pHookedEntry, PVOID ModuleBase);
BOOL		RestoreHook(PHOOK pHook);
VOID		HookDump(VOID);

PVOID		GetModuleBase(PCHAR pModuleName);
PVOID		MakeCallStub(PVOID CallStub, PVOID Handler, PVOID Context, ULONG nArgs);
