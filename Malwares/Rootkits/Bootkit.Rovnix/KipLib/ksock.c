//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP protocol driver
//	
// module: ksock.c
// $Revision: 102 $
// $Date: 2012-08-16 13:41:17 +0400 (Чт, 16 авг 2012) $
// description: 
//  kernel sockets interface

//#define _CRT_ERRNO_DEFINED

#include "project.h"
#include "ksock.h"
#include "lwip\sockets.h"
#include "lwip\netdb.h"
#include "tsocket.h"

#define MODULE_TAG 'KOSK'

static LONG g_NumSocket = 0;

// this interface functions switches between lwip and tdi engines

LONG KSockGetEngine( VOID ){
#ifdef _TDI_SUPPORT
	if ( TsocketIsLwip() ){
		return KIP_SOCK_LWIP;
	}
	return KIP_SOCK_TDI;
#else
	return KIP_SOCK_LWIP;
#endif
}

NTSTATUS KSockSetEngine( LONG ID ){

#ifdef _TDI_SUPPORT
	if ( g_NumSocket != 0 ){
		return STATUS_TOO_MANY_OPENED_FILES;
	}
	if ( ID == KIP_SOCK_LWIP ){
		TsocketSetLwip();
	}else{
		TsocketSetTdi();
	}
	return STATUS_SUCCESS;
#else
	return STATUS_NOT_SUPPORTED;
#endif //#ifdef _TDI_SUPPORT
}

//***********************************************************************************
//
// Name: KsockFindEA
//
// Routine Description:
//
//	Parses and extended attribute list for a given target attribute.
//
// Return Value:
//
// A pointer to the requested attribute or NULL if the target wasn't found.
//
// Parameters:
//
// StartEA           - the first extended attribute in the list.
// TargetName        - the name of the target attribute.
// TargetNameLength  - the length of the name of the target attribute.
//
//***********************************************************************************
PFILE_FULL_EA_INFORMATION
	KSockFindEA(
		IN PFILE_FULL_EA_INFORMATION  StartEA,
		IN PCHAR	TargetName,
		IN USHORT	TargetNameLength
		)
{
	USHORT i;
	BOOLEAN fbFound;
	PFILE_FULL_EA_INFORMATION ea;

	PAGED_CODE();

	do {
		fbFound = TRUE;

		ea = StartEA;

		StartEA = (FILE_FULL_EA_INFORMATION *)((PUCHAR)StartEA + ea->NextEntryOffset);

		if (ea->EaNameLength != TargetNameLength) {
			continue;
		}

		for (i=0; i < ea->EaNameLength; i++) {
			if (ea->EaName[i] == TargetName[i]) {
				continue;
			}
			fbFound = FALSE;
			break;
		}

		if (fbFound) {
			return ea;
		}

	} while(ea->NextEntryOffset != 0);

	return NULL;
}
//***********************************************************************************
//
// Name: KSockCreateAddress
//
// Routine Description:
//
//	Creates new socket.
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockCreateAddress (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_CREATE_PARAMETERS Params
		)
{
	HANDLE hSocket;
	PKSOCK_OBJECT pKSocket = NULL;

	// allocate our socket
	pKSocket = 
		ExAllocatePoolWithTag( 
			NonPagedPool, 
			sizeof ( KSOCK_OBJECT ),
			MODULE_TAG
			);
	if ( pKSocket == NULL ){
		DBGLOG((LError, "ExAllocatePoolWithTag(%lu) failed\n",sizeof ( KSOCK_OBJECT )));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// create new socket
	hSocket = 
		(HANDLE)_tsocket(
			Params->Domain,
			Params->Type,
			Params->Protocol
			);
	if ( hSocket == (HANDLE)INVALID_SOCKET ){
		DBGLOG((LError, "_tsocket failed\n"));
		ExFreePoolWithTag( pKSocket, MODULE_TAG );
		return RtlErroNoToStatus(errno);
	}

	pKSocket->hSocket = hSocket;
	pKSocket->State = KSOCK_STATE_CREATED;

	FileObject->FsContext = (PVOID)KSOCK_SOCKET_OBJECT;
	FileObject->FsContext2 = (PVOID)pKSocket;
	InterlockedIncrement(&g_NumSocket);
	return STATUS_SUCCESS;
}
//***********************************************************************************
//
// Name: KSockCreateConnection
//
// Routine Description:
//
//	Creates new socket.
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockCreateConnection (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_CONNECTION_PARAMETERS Params
		)
{
	PKSOCK_OBJECT pKSocket = NULL;

	// allocate our socket
	pKSocket = 
		ExAllocatePoolWithTag( 
			NonPagedPool, 
			sizeof ( KSOCK_OBJECT ),
			MODULE_TAG
			);
	if ( pKSocket == NULL ){
		DBGLOG((LError, "ExAllocatePoolWithTag(%lu) failed\n",sizeof ( KSOCK_OBJECT )));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	pKSocket->hSocket = (HANDLE)Params->Socket;
	pKSocket->State = KSOCK_STATE_CONNECTED;

	FileObject->FsContext = (PVOID)KSOCK_SOCKET_OBJECT;
	FileObject->FsContext2 = (PVOID)pKSocket;
	InterlockedIncrement(&g_NumSocket);
	return STATUS_SUCCESS;
}
//***********************************************************************************
//
// Name: KSockCreateSocket
//
// Routine Description:
//
//	Creates new socket.
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockCreateSocket (
		IN PIRP Irp,
		IN PIO_STACK_LOCATION IoStack,
		IN PFILE_FULL_EA_INFORMATION StartEA
		)
{
	PFILE_OBJECT FileObject = IoStack->FileObject;
	PFILE_FULL_EA_INFORMATION ea;
	HANDLE hSocket;
	PKSOCK_OBJECT pKSocket = NULL;

	// request to create socket
	ea = 
		KSockFindEA(
			StartEA,
			KipSocketName,
			KIP_SOCKET_NAME_LENGTH
			);
	if ( ea != NULL ){
		PKSOCKET_CREATE_PARAMETERS Params;;
		// sanity check
		if ( ea->EaValueLength < sizeof(KSOCKET_CREATE_PARAMETERS) ){
			DBGLOG((LError, "invalid EA length (%lu) ea->EaValueLength\n", ea->EaValueLength));
			return STATUS_INVALID_PARAMETER;
		}
		Params = (PKSOCKET_CREATE_PARAMETERS)&ea->EaName[ea->EaNameLength+1];
		return
			KSockCreateAddress(
				FileObject,
				Params
				);
	}
	// request to create connection
	ea = 
		KSockFindEA(
			StartEA,
			KipSocketConnection,
			KIP_SOCKET_CONNECTION_LENGTH
			);
	if ( ea != NULL ){
		PKSOCKET_CONNECTION_PARAMETERS Params;
		// sanity check
		if ( ea->EaValueLength < sizeof(KSOCKET_CONNECTION_PARAMETERS) ){
			DBGLOG((LError, "invalid EA length (%lu) ea->EaValueLength\n", ea->EaValueLength));
			return STATUS_INVALID_PARAMETER;
		}
		Params = (PKSOCKET_CONNECTION_PARAMETERS)&ea->EaName[ea->EaNameLength+1];
		return
			KSockCreateConnection(
				FileObject,
				Params
				);
	}
	return STATUS_SUCCESS;
}
//***********************************************************************************
//
// Name: KSockCloseSocket
//
// Routine Description:
//
//	Creates new socket.
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockCloseSocket (
		IN PFILE_OBJECT FileObject
		)
{
	if ( FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT ){
		PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
		_tclosesocket((int)(ULONG_PTR)pKSocket->hSocket);
		ExFreePoolWithTag( pKSocket, MODULE_TAG );
		FileObject->FsContext = NULL;
		FileObject->FsContext2 = NULL;
		InterlockedDecrement(&g_NumSocket);
	}
	return STATUS_SUCCESS;
}
//***********************************************************************************
//
// Name: KSockBind
//
// Routine Description:
//
//	Binds socket to address
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockBind (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_BIND_PARAMETERS BindParameters,
		IN PKSOCKET_BIND_RESULT BindResult
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	struct sockaddr_in name;
	int err;

	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
	if( pKSocket->State != KSOCK_STATE_CREATED ){
		return STATUS_INVALID_PARAMETER;
	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	name.sin_family = BindParameters->Name.sin_family;
	name.sin_len = sizeof(name);
	name.sin_port = BindParameters->Name.sin_port;
	name.sin_addr.S_un.S_addr = BindParameters->Name.in_addr;

	err = _tbind(s, (struct sockaddr*)&name, sizeof(name) );
	if ( err == 0 ){
		pKSocket->State = KSOCK_STATE_BIND;
		BindResult->error_code = 0;
	}else{
		BindResult->error_code = lwip_get_last_error(s);
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockConnect
//
// Routine Description:
//
//	Connects socket to remote peer
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockConnect (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_CONNECT_PARAMETERS ConnectParameters,
		IN PKSOCKET_CONNECT_RESULT ConnectResult
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	struct sockaddr_in name;
	int err;

	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
	if( pKSocket->State != KSOCK_STATE_BIND && pKSocket->State != KSOCK_STATE_CREATED ){
		return STATUS_INVALID_PARAMETER;
	}

	s = (int)(ULONG_PTR)pKSocket->hSocket;

	name.sin_family = ConnectParameters->Name.sin_family;
	name.sin_len = sizeof(name);
	name.sin_port = ConnectParameters->Name.sin_port;
	name.sin_addr.S_un.S_addr = ConnectParameters->Name.in_addr;

	err = _tconnect(s, (struct sockaddr*)&name, sizeof(name) );
	if ( err == 0 ){
		pKSocket->State = KSOCK_STATE_CONNECTED;
		ConnectResult->error_code = 0;
	}else{
		ConnectResult->error_code = lwip_get_last_error(s);
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockShutdown
//
// Routine Description:
//
//	Shuts down socket connection
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockShutdown (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_SHUTDOWN_PARAMETERS ShutdownParameters,
		IN PKSOCKET_SHUTDOWN_RESULT ShutdownResult
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;

	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
//	if( pKSocket->State != KSOCK_STATE_CONNECTED ){
//		return STATUS_INVALID_PARAMETER;
//	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	err = _tshutdown(s, ShutdownParameters->How );
	if ( err == 0 ){
		pKSocket->State = KSOCK_STATE_SHUTDOWN;
		ShutdownResult->error_code = 0;
	}else{
		ShutdownResult->error_code = lwip_get_last_error(s);
	}
	return ntStatus;
}
//***********************************************************************************
//
// Name: KSockSend
//
// Routine Description:
//
//	Sends data portion over socket
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockSend (
		IN PFILE_OBJECT FileObject,
		IN PVOID DataPtr,
		IN ULONG Length,
		IN ULONG Flags,
		IN PKSOCKET_SEND_RESULT SendResult
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int bSent = -1;
	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
//	if( pKSocket->State != KSOCK_STATE_CONNECTED ){
//		return STATUS_CONNECTION_INVALID;
//	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	bSent = _tsend(s, (char*)DataPtr, Length, 0);

	if ( bSent == -1 ){
		SendResult->error_code = lwip_get_last_error(s);
		SendResult->BytesSent = 0;
	}else{
		SendResult->error_code = 0;
		SendResult->BytesSent = bSent;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockRecv
//
// Routine Description:
//
//	Receives data from socket
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockRecv (
		IN PFILE_OBJECT FileObject,
		IN PVOID DataPtr,
		IN ULONG Length,
		IN ULONG Flags,
		IN PKSOCKET_RECV_RESULT RecvResult
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int Result;

	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
//	if( pKSocket->State != KSOCK_STATE_CONNECTED ){
//		return STATUS_CONNECTION_INVALID;
//	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	Result = _trecv(s, (void *)DataPtr, (int)Length, (int)Flags );
	if ( Result == -1 ){
		RecvResult->error_code = lwip_get_last_error(s);
		RecvResult->BytesRecv = 0;
	}else{
		RecvResult->error_code = 0;
		RecvResult->BytesRecv = Result;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockSendTo
//
// Routine Description:
//
//	Sends UDP datagram over network
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockSendTo (
		IN PFILE_OBJECT FileObject,
		IN PVOID DataPtr,
		IN ULONG Length,
		IN ULONG Flags,
		IN PSOCKET_ADDR_IN Address,
		IN PKSOCKET_SEND_TO_RESULT SendResult
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	int Result;

	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
//	if( pKSocket->State != KSOCK_STATE_CREATED ){
//		return STATUS_INVALID_PARAMETER;
//	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	name.sin_family = Address->sin_family;
	name.sin_len = sizeof(name);
	name.sin_port = Address->sin_port;
	name.sin_addr.S_un.S_addr = Address->in_addr;

	Result = _tsendto(s, (void *)DataPtr, (int)Length, (int)Flags, (struct sockaddr*)&name, namelen );
	if ( Result == -1 ){
		SendResult->error_code = lwip_get_last_error(s);
		SendResult->BytesSent = 0;
	}else{
		SendResult->error_code = 0;
		SendResult->BytesSent = Result;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockRecvFrom
//
// Routine Description:
//
//	Receives UDP datagram from socket
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockRecvFrom (
		IN PFILE_OBJECT FileObject,
		IN PVOID DataPtr,
		IN ULONG Length,
		IN ULONG Flags,
		IN PKSOCKET_RECV_FROM_RESULT RecvResult
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int Result;
	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);

	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
//	if( pKSocket->State != KSOCK_STATE_CREATED ){
//		return STATUS_INVALID_PARAMETER;
//	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	Result = _trecvfrom(s, (void *)DataPtr, (int)Length, (int)Flags, (struct sockaddr*)&name, &namelen );
	if ( Result == -1 ){
		RecvResult->error_code = lwip_get_last_error(s);
		RecvResult->BytesRecv = 0;
	}else{
		RecvResult->error_code = 0;
		RecvResult->BytesRecv = Result;

		RecvResult->Address.sin_family = name.sin_family;
		RecvResult->Address.sin_len = name.sin_len;
		RecvResult->Address.sin_port = name.sin_port;
		RecvResult->Address.in_addr = name.sin_addr.S_un.S_addr;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockIoctl
//
// Routine Description:
//
//	socket ioctl command
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockIoctl (
		IN PFILE_OBJECT FileObject,
		IN long Command,
		IN ULONG arg_p,
		IN PKSOCKET_IOCTL_RESULT Result
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;

	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;


	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	err = _tioctlsocket(s, Command, &arg_p );
	if ( err == -1 ){
		Result->error_code = lwip_get_last_error(s);
		Result->arg_p = 0;
	}else{
		Result->error_code = 0;
		Result->arg_p = arg_p;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockGetSockOpt
//
// Routine Description:
//
//	socket getsockopt
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockGetSockOpt (
		IN PFILE_OBJECT FileObject,
		IN int level,
		IN int optname,
		IN PKSOCKET_GET_SOCK_OPT_RESULT Result,
		IN ULONG Length
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;
	int optlen = Length-FIELD_OFFSET(KSOCKET_GET_SOCK_OPT_RESULT,optval);

	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	if ( optlen < 0 ){
		return STATUS_INVALID_PARAMETER;
	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	err = _tgetsockopt(s, level, optname, Result->optval, &optlen );
	if ( err == -1 ){
		Result->error_code = lwip_get_last_error(s);
		Result->optlen = 0;
	}else{
		Result->error_code = 0;
		Result->optlen = optlen;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockSetSockOpt
//
// Routine Description:
//
//	socket setsockopt
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockSetSockOpt (
		IN PFILE_OBJECT FileObject,
		IN int level,
		IN int optname,
		OUT  char* optval,
		IN int optlen,
		IN PKSOCKET_SET_SOCK_OPT_RESULT Result
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;
	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;


	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	err = _tsetsockopt(s, level, optname, optval, optlen );
	if ( err == -1 ){
		Result->error_code = lwip_get_last_error(s);
	}else{
		Result->error_code = 0;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockGetSockName
//
// Routine Description:
//
//	socket getsockname
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockGetSockName (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_GET_SOCK_NAME_RESULT Result
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;
	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	err = _tgetsockname(s, (struct sockaddr*)&name, &namelen );
	if ( err == -1 ){
		Result->error_code = lwip_get_last_error(s);
	}else{
		Result->error_code = 0;
		Result->Address.sin_len = name.sin_len;
		Result->Address.sin_family = name.sin_family;
		Result->Address.sin_port = name.sin_port;
		Result->Address.in_addr = name.sin_addr.S_un.S_addr;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockGetPeerName
//
// Routine Description:
//
//	socket getpeername
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockGetPeerName (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_GET_PEER_NAME_RESULT Result
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;
	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;
	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	err = _tgetpeername(s, (struct sockaddr*)&name, &namelen );
	if ( err == -1 ){
		Result->error_code = lwip_get_last_error(s);
	}else{
		Result->error_code = 0;
		Result->Address.sin_len = name.sin_len;
		Result->Address.sin_family = name.sin_family;
		Result->Address.sin_port = name.sin_port;
		Result->Address.in_addr = name.sin_addr.S_un.S_addr;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockGetHostByName
//
// Routine Description:
//
//	socket gethostbyname
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockGetHostByName (
		IN char *name,
		IN int length,
		IN PKSOCKET_GET_HOST_BY_NAME_RESULT Result
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;
	ip_addr_t addr;
	int h_errnop;

	name[length-1] = 0;

#ifdef _TDI_SUPPORT
	if ( TsocketIsLwip() ){
#endif
		err = lwip_gethostbyname_r0(name,&addr,&h_errnop);
		if ( err == -1 ){
			Result->error_code = h_errnop;
		}else{
			Result->error_code = 0;
			Result->in_addr = addr.addr;
		}
#ifdef _TDI_SUPPORT
	}else{
		ntStatus = STATUS_NOT_IMPLEMENTED;
	}
#endif
	return ntStatus;
}
//***********************************************************************************
//
// Name: KSockListen
//
// Routine Description:
//
//	Starts listening on socket
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockListen (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_LISTEN_PARAMETERS ListenParameters,
		IN PKSOCKET_LISTEN_RESULT Result
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;
	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
	if( pKSocket->State != KSOCK_STATE_BIND ){
		return STATUS_ADDRESS_NOT_ASSOCIATED;
	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	err = _tlisten(s, ListenParameters->backlog );
	if ( err == -1 ){
		Result->error_code = lwip_get_last_error(s);
	}else{
		Result->error_code = 0;
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockAccept
//
// Routine Description:
//
//	accepts incoming connection
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KSockAccept (
		IN PFILE_OBJECT FileObject,
		OUT PKSOCKET_ACCEPT_RESULT Result
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;
	PKSOCK_OBJECT pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
	int s;
	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);

	ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
	if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
		return STATUS_INVALID_PARAMETER;
	}

	ASSERT(pKSocket);
	if( pKSocket->State != KSOCK_STATE_BIND ){
		return STATUS_ADDRESS_NOT_ASSOCIATED;
	}
	s = (int)(ULONG_PTR)pKSocket->hSocket;

	err = _taccept((int)(ULONG_PTR)pKSocket->hSocket, (struct sockaddr*)&name, &namelen );
	if ( err != -1 ){
		Result->error_code = 0;
		Result->Address.sin_len = name.sin_len;
		Result->Address.sin_family = name.sin_family;
		Result->Address.sin_port = name.sin_port;
		Result->Address.in_addr = name.sin_addr.S_un.S_addr;
		Result->Socket = err;
	}else{
		Result->error_code = lwip_get_last_error(s);
	}
	return ntStatus;
}

//***********************************************************************************
//
// Name: KSockSelect
//
// Routine Description:
//
//	socket select function
//
// Return Value:
//
// Operation status
//
// Parameters:
//
//***********************************************************************************
typedef struct _SOCKET_TO_HANDLE
{
	HANDLE h;
//	int s;
}SOCKET_TO_HANDLE,*PSOCKET_TO_HANDLE;

NTSTATUS
	_GetFileByHandle( 
		IN HANDLE hSocket,
		IN PFILE_OBJECT *pFileObject
		)
{
	PFILE_OBJECT FileObject = NULL;
	NTSTATUS ntStatus;
	ntStatus = 
		ObReferenceObjectByHandle(
			hSocket,
			FILE_ALL_ACCESS,
			*IoFileObjectType,
			KernelMode,
			&FileObject,
			NULL
			);
	if ( NT_SUCCESS ( ntStatus ) ){
		*pFileObject = FileObject;
	}
	return ntStatus;
}

NTSTATUS
	_FillFdSet(
		IN PFD_SET_INTERNAL fdsetint,
		IN OUT fd_set *fdset,
		IN OUT PSOCKET_TO_HANDLE h2s,
		IN OUT int *maxfd1
		)
{
	PFILE_OBJECT FileObject = NULL;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	unsigned int i;
	PKSOCK_OBJECT pKSocket;
	int s;
	int lmaxfd1 = *maxfd1;

	for ( i = 0; i < fdsetint->fd_count; ++i ){
		ntStatus = 
			_GetFileByHandle( 
				(HANDLE)fdsetint->fd_array[i], 
				&FileObject 
				);
		if ( !NT_SUCCESS ( ntStatus ) ){
			break;
		}
		ASSERT(FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT);
		if ( FileObject->FsContext != (PVOID)KSOCK_SOCKET_OBJECT ){
			ObDereferenceObject ( FileObject );
			ntStatus = STATUS_INVALID_HANDLE;
			break; 
		}
		pKSocket = (PKSOCK_OBJECT)FileObject->FsContext2;
		s = (int)(ULONG_PTR)pKSocket->hSocket;
		FD_SET(s,fdset);
		// save handle to socket 
		h2s[s].h = (HANDLE)fdsetint->fd_array[i];
		ObDereferenceObject ( FileObject );
		FileObject = NULL;
		lmaxfd1 = max(lmaxfd1,s);
	}
	*maxfd1 = lmaxfd1;
	return ntStatus;
}

unsigned int
	_UpdateFdSet(
		IN OUT PFD_SET_INTERNAL fdsetint,
		IN fd_set *fdset,
		IN PSOCKET_TO_HANDLE h2s
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	unsigned int i;
	int s;
	unsigned int nCount = 0;

	for ( i = 0; i < FD_SETSIZE; ++i ){
		if ( FD_ISSET( i, fdset )){
			fdsetint->fd_array[nCount] = h2s[i].h;
			nCount ++;
		}
	}
	fdsetint->fd_count = nCount;

	return nCount;
}

NTSTATUS 
	KSockSelect(
		IN PKSOCKET_SELECT_PARAMETERS Select,
		IN PKSOCKET_SELECT_RESULT Result
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	int err;
	fd_set readfds   = {0};
	fd_set writefds  = {0};
	fd_set exceptfds = {0};
	struct timeval timeout = {0};
	int fdr_count = 0;
	int fdw_count = 0;
	int fde_count = 0;

	SOCKET_TO_HANDLE h2sr[FD_SETSIZE];
	SOCKET_TO_HANDLE h2sw[FD_SETSIZE];
	SOCKET_TO_HANDLE h2se[FD_SETSIZE];

	fd_set *pr = NULL;
	fd_set *pw = NULL;
	fd_set *pe = NULL;
	struct timeval *pt = NULL;
	unsigned i;
	int maxfd1 = 1;


	// validate input
	if ( Select->readfds.fd_count >= FD_SETSIZE ||
		Select->writefds.fd_count >= FD_SETSIZE ||
		Select->exceptfds.fd_count >= FD_SETSIZE )
	{
		return STATUS_INVALID_PARAMETER;
	}

	if ( ( Select->ValidFields & READFDS_VALID ) == 0 &&
		( Select->ValidFields & WRITEFDS_VALID ) == 0 &&
		( Select->ValidFields & EXCEPTFDS_VALID ) == 0 )
	{
		return STATUS_INVALID_PARAMETER;
	}

	if ( ( Select->ValidFields & READFDS_VALID ) == READFDS_VALID ){
		ntStatus = _FillFdSet ( &Select->readfds, &readfds, &h2sr[0], &maxfd1 );
		if ( !NT_SUCCESS ( ntStatus ) ){
			Result->error_code = ENOTSOCK;
			return STATUS_SUCCESS;
		}
		fdr_count = Select->readfds.fd_count;
		pr = &readfds;
	}

	if ( ( Select->ValidFields & WRITEFDS_VALID ) == WRITEFDS_VALID ){
		ntStatus = _FillFdSet ( &Select->writefds, &writefds, &h2sw[0], &maxfd1 );
		if ( !NT_SUCCESS ( ntStatus ) ){
			Result->error_code = ENOTSOCK;
			return STATUS_SUCCESS;
		}
		fdw_count = Select->writefds.fd_count;
		pw = &writefds;
	}

	if ( ( Select->ValidFields & EXCEPTFDS_VALID ) == EXCEPTFDS_VALID ){
		ntStatus = _FillFdSet ( &Select->exceptfds, &exceptfds, &h2se[0], &maxfd1 );
		if ( !NT_SUCCESS ( ntStatus ) ){
			Result->error_code = ENOTSOCK;
			return STATUS_SUCCESS;
		}
		fde_count = Select->exceptfds.fd_count;
		pe = &exceptfds;
	}
	if ( ( Select->ValidFields & TIMEOUT_VALID ) == TIMEOUT_VALID ){
		timeout.tv_sec  = Select->timeout.tv_sec;
		timeout.tv_usec = Select->timeout.tv_usec;
		pt = &timeout;
	}
	// do select
	err = _tselect ( maxfd1+1, pr, pw, pe, pt );
	if ( err == -1 ){
		Result->error_code = errno;
		Result->number = 0;
		return STATUS_SUCCESS;
	}

	// process the output
	if ( pr ){
		_UpdateFdSet(&Result->readfds,pr,&h2sr[0]);
	}
	if ( pw ){
		_UpdateFdSet(&Result->writefds,pw,&h2sw[0]);
	}
	if ( pe ){
		_UpdateFdSet(&Result->exceptfds,pe,&h2se[0]);
	}

	Result->error_code = 0;
	Result->number = err;
	return STATUS_SUCCESS;
}