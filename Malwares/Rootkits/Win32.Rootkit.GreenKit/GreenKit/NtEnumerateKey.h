#ifndef NT_OPENFILE_H
# define NT_OPENFILE_H
/*
# include "windefs.h"

typedef NTSTATUS(WINAPI *TD_NtEnumerateKey)(HANDLE, ULONG, KEY_INFORMATION_CLASS, PVOID, ULONG, PULONG);

NTSTATUS NTAPI NewNtEnumerateKey(HANDLE, ULONG, KEY_INFORMATION_CLASS, PVOID, ULONG, PULONG);

typedef NTSTATUS (NTAPI *TD_NtOpenKey)(
OUT PHANDLE  KeyHandle,
IN ACCESS_MASK  DesiredAccess,
IN POBJECT_ATTRIBUTES  ObjectAttributes
);

TD_NtEnumerateKey oldNtEnumerateKey;
TD_NtEnumerateKey hookNtEnumerateKey;
*/
#endif