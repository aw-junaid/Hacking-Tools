//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP user-mode API library
//	
// module: create.c
// $Revision: 93 $
// $Date: 2012-06-06 17:17:26 +0400 (Ср, 06 июн 2012) $
// description:
//  Creation of KIP-specific sockets: name socket and connection socket.

#include "project.h"

typedef
NTSTATUS
(NTAPI *PZW_CREATE_FILE)(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer,
    IN ULONG EaLength
    );

typedef
NTSTATUS
(NTAPI *PRTL_INIT_UNICODE_STRING)(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );

#define	wczDosDeviceTempl	L"\\??\\%s\\"


HANDLE 
	CreateSocketNative( 
		IN PFILE_FULL_EA_INFORMATION Ea,
		IN ULONG EaLength
		)
{
	HMODULE hModule;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	NTSTATUS ntStatus;
	OBJECT_ATTRIBUTES ObjectAttributes; 
	UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(KIP_DEVICE_NAME);
	IO_STATUS_BLOCK IoStatus;
	PZW_CREATE_FILE ZwCreateFilePtr;
	PRTL_INIT_UNICODE_STRING RtlInitUnicodeStringPtr;
	PWSTR	wDeviceName;
	hModule = LoadLibrary( TEXT("ntdll.dll") );
	if ( hModule == NULL ){
		return INVALID_HANDLE_VALUE;
	}
	wDeviceName = CreateRandomDeviceName(wczDosDeviceTempl, KIP_DEVICE_NAME_SEED);
	if (wDeviceName == NULL ){
		FreeLibrary( hModule );
		return INVALID_HANDLE_VALUE;
	}

	ZwCreateFilePtr = (PZW_CREATE_FILE)GetProcAddress(hModule,"ZwCreateFile");
	RtlInitUnicodeStringPtr = (PRTL_INIT_UNICODE_STRING)GetProcAddress(hModule,"RtlInitUnicodeString");
	if ( ZwCreateFilePtr ){
		RtlInitUnicodeStringPtr(&DeviceName, wDeviceName);
		InitializeObjectAttributes(
			&ObjectAttributes, 
			&DeviceName, 
			OBJ_CASE_INSENSITIVE, 
			0, 0
			);

		ntStatus = 
			ZwCreateFilePtr(
				&hFile,
				READ_CONTROL,
				&ObjectAttributes,
				&IoStatus,
				NULL,
				FILE_ATTRIBUTE_NORMAL,
				0,
				FILE_OPEN_IF,0, 
				Ea, EaLength
				);
		if ( ntStatus != STATUS_SUCCESS ){
			hFile = INVALID_HANDLE_VALUE;
		}
	}
	FreeRandomDeviceName(wDeviceName);
	FreeLibrary( hModule );
	return hFile;
}

HANDLE CreateSocket( int domain, int type, int protocol )
{
	CHAR Buffer[sizeof(FILE_FULL_EA_INFORMATION) + KIP_SOCKET_NAME_LENGTH + sizeof(KSOCKET_CREATE_PARAMETERS)] = {0};
	PFILE_FULL_EA_INFORMATION Ea = (PFILE_FULL_EA_INFORMATION)&Buffer;
	PKSOCKET_CREATE_PARAMETERS CreateParams;

	Ea->NextEntryOffset = 0;
	Ea->Flags = 0;
	Ea->EaNameLength = KIP_SOCKET_NAME_LENGTH;
	RtlCopyMemory(Ea->EaName, KipSocketName, KIP_SOCKET_NAME_LENGTH);
	Ea->EaValueLength = sizeof(KSOCKET_CREATE_PARAMETERS);	

	CreateParams = (PKSOCKET_CREATE_PARAMETERS)&Ea->EaName[Ea->EaNameLength+1];
	CreateParams->Domain = domain; 
	CreateParams->Type = type; 
	CreateParams->Protocol = protocol; 

	return CreateSocketNative(Ea,sizeof(Buffer) );
}

HANDLE CreateConnectionSocket( int Socket )
{
	CHAR Buffer[sizeof(FILE_FULL_EA_INFORMATION) + KIP_SOCKET_CONNECTION_LENGTH + sizeof(KSOCKET_CONNECTION_PARAMETERS)] = {0};
	PFILE_FULL_EA_INFORMATION Ea = (PFILE_FULL_EA_INFORMATION)&Buffer;
	PKSOCKET_CONNECTION_PARAMETERS CreateParams;

	Ea->NextEntryOffset = 0;
	Ea->Flags = 0;
	Ea->EaNameLength = KIP_SOCKET_CONNECTION_LENGTH;
	RtlCopyMemory(Ea->EaName, KipSocketConnection, KIP_SOCKET_CONNECTION_LENGTH);
	Ea->EaValueLength = sizeof(KSOCKET_CONNECTION_PARAMETERS);	

	CreateParams = (PKSOCKET_CONNECTION_PARAMETERS)&Ea->EaName[Ea->EaNameLength+1];
	CreateParams->Socket = Socket;

	return CreateSocketNative(Ea,sizeof(Buffer));
}