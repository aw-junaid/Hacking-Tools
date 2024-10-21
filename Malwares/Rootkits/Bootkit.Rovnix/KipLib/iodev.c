#include "project.h"
#include "bkdrv.h"
#include "ksock.h"
#include "iodev.h"
#include "util.h"
//#include "tsocket.h"

static	PDEVICE_OBJECT				g_KipDeviceObject	= NULL;
static	PBK_DRIVER_DISPATCH_TABLE	g_KipDispatchTable	= NULL;
static	PUNICODE_STRING				g_KipDeviceName		= NULL;
static	PUNICODE_STRING				g_KipSymbolicLink	= NULL;


NTSTATUS 
	KipDriverEntry ( 
		IN PDRIVER_OBJECT DriverObject,
		IN PUNICODE_STRING RegistryPath
		);

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, KipDriverEntry)
#endif

//***********************************************************************************
//
// Name: KipDispatchCreate
//
// Routine Description:
//
//	IRP_MJ_CREATE dispatcher. 
//
// Return Value:
//
// A pointer to the requested attribute or NULL if the target wasn't found.
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	KipDispatchCreate (
		IN PIRP Irp,
		IN PIO_STACK_LOCATION IoStack
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PFILE_FULL_EA_INFORMATION StartEA, ea;

	StartEA = (PFILE_FULL_EA_INFORMATION)Irp->AssociatedIrp.SystemBuffer;
	if ( StartEA != NULL )
		ntStatus = KSockCreateSocket(Irp, IoStack, StartEA);
	else
	{
		PFILE_OBJECT FileObject = IoStack->FileObject;
		FileObject->FsContext = (PVOID)KSOCK_CONTROL_OBJECT;
	}

	Irp -> IoStatus.Status = ntStatus;
	Irp -> IoStatus.Information = 0;

	IoCompleteRequest ( Irp, IO_NO_INCREMENT );

	return STATUS_SUCCESS;
}

NTSTATUS 
	KipDispatchCleanup (
		IN PIRP Irp,
		IN PIO_STACK_LOCATION IoStack
		)
{
	Irp -> IoStatus.Status = STATUS_SUCCESS;
	Irp -> IoStatus.Information = 0;

	IoCompleteRequest ( Irp, IO_NO_INCREMENT );

	UNREFERENCED_PARAMETER(IoStack);

	return STATUS_SUCCESS;
}

NTSTATUS 
	KipDispatchClose (
		IN PIRP Irp,
		IN PIO_STACK_LOCATION IoStack
		)
{
	PFILE_OBJECT FileObject = IoStack->FileObject;

	if ( FileObject->FsContext == (PVOID)KSOCK_SOCKET_OBJECT ){
		KSockCloseSocket( FileObject );
	}
	
	Irp -> IoStatus.Status = STATUS_SUCCESS;
	Irp -> IoStatus.Information = 0;

	IoCompleteRequest ( Irp, IO_NO_INCREMENT );

	return STATUS_SUCCESS;
}

NTSTATUS 
	KipDispatchShutdown (
		IN PIRP Irp,
		IN PIO_STACK_LOCATION IoStack
		)
{
	Irp -> IoStatus.Status = STATUS_SUCCESS;
	Irp -> IoStatus.Information = 0;

	IoCompleteRequest ( Irp, IO_NO_INCREMENT );

	UNREFERENCED_PARAMETER(IoStack);

	return STATUS_SUCCESS;
}

NTSTATUS 
	KipDispatchInternelIoControl (
		IN PIRP Irp,
		IN PIO_STACK_LOCATION IoStack
		)
{
	NTSTATUS ntStatus = STATUS_NOT_IMPLEMENTED;
	ULONG ReturnedBytes = 0;
/*
	NTSTATUS ntStatus = STATUS_INVALID_PARAMETER;
	

	switch( IoStack->Parameters.DeviceIoControl.IoControlCode )
	{
	case IOCTL_INTERNAL_GET_SOCK_TABLE:
		{
			PKSOCKET_ENGINE Parameters = (PKSOCKET_ENGINE)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_FUNC_TABLE Table = (PKSOCKET_FUNC_TABLE)Irp->AssociatedIrp.SystemBuffer;
			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_ENGINE) )
			{
				DBGLOG((LError, "IOCTL_INTERNAL_GET_SOCK_TABLE, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_FUNC_TABLE) )
			{
				DBGLOG((LError, "IOCTL_INTERNAL_GET_SOCK_TABLE, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
#ifdef _TDI_SUPPORT
			if ( Parameters->Engine == KIP_SOCK_LWIP ){
				Table->SockTable = &g_TsocketLwipTable;
			}else{
				Table->SockTable = &g_TsocketTdiTable;
			}
#else
			if ( Parameters->Engine == KIP_SOCK_LWIP ){
				Table->SockTable = &g_TsocketLwipTable;
			}else{
				ntStatus = STATUS_NOT_SUPPORTED;
			}
#endif //_TDI_SUPPORT
			ReturnedBytes = sizeof(KSOCKET_ENGINE);
		}
		break;
	}
*/	
	Irp -> IoStatus.Status = ntStatus;
	Irp -> IoStatus.Information = ReturnedBytes;

	IoCompleteRequest ( Irp, IO_NO_INCREMENT );

	return ntStatus;
}

NTSTATUS 
	KipDispatchIoControl (
		IN PIRP Irp,
		IN PIO_STACK_LOCATION IoStack
		)
{
	NTSTATUS ntStatus = STATUS_INVALID_PARAMETER;
	ULONG ReturnedBytes = 0;

	switch( IoStack->Parameters.DeviceIoControl.IoControlCode )
	{
	case IOCTL_KSOCK_GET_ENGINE:
		{
			PKSOCKET_ENGINE Parameters = (PKSOCKET_ENGINE)Irp->AssociatedIrp.SystemBuffer;
			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_ENGINE) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_GET_ENGINE, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			Parameters->Engine = KSockGetEngine();
			ReturnedBytes = sizeof(KSOCKET_ENGINE);
		}
		break;
	case IOCTL_KSOCK_SET_ENGINE:
		{
			PKSOCKET_ENGINE Parameters = (PKSOCKET_ENGINE)Irp->AssociatedIrp.SystemBuffer;
			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_ENGINE) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SET_ENGINE, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			ntStatus = KSockSetEngine(Parameters->Engine);
		}
		break;
	case IOCTL_KSOCK_BIND:
		{
			PKSOCKET_BIND_PARAMETERS Parameters = (PKSOCKET_BIND_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_BIND_RESULT Result = (PKSOCKET_BIND_RESULT)Irp->AssociatedIrp.SystemBuffer;
			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_BIND_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_BIND, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_BIND_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_BIND, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			ntStatus = 
				KSockBind(
					IoStack->FileObject,
					Parameters,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_BIND_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_CONNECT:
		{
			PKSOCKET_CONNECT_PARAMETERS Parameters = (PKSOCKET_CONNECT_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_CONNECT_RESULT Result = (PKSOCKET_CONNECT_RESULT)Irp->AssociatedIrp.SystemBuffer;
			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_CONNECT_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_CONNECT, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_CONNECT_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_CONNECT, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			ntStatus = 
				KSockConnect(
					IoStack->FileObject,
					Parameters,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_CONNECT_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_SEND:
		{
			PKSOCKET_SEND_PARAMETERS Parameters = (PKSOCKET_SEND_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_SEND_RESULT Result = (PKSOCKET_SEND_RESULT)Irp->AssociatedIrp.SystemBuffer;
			PMDL Mdl = NULL;
			PVOID SystemBuffer = NULL;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_SEND_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SEND, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_SEND_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SEND, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			// map and lock user buffer
			ntStatus = 
				UF_LockUserBuffer( 
					Parameters->DataPtr, 
					Parameters->Size, 
					IoReadAccess,
					&Mdl, 
					&SystemBuffer 
					);
			if ( !NT_SUCCESS ( ntStatus ) ){
				DBGLOG((LError, "IOCTL_KSOCK_SEND, %08X=UF_LockUserBuffer(%p,%lu) failed\n",
					ntStatus,Parameters->DataPtr,Parameters->Size));
				break;
			}
			ntStatus = 
				KSockSend(
					IoStack->FileObject,
					SystemBuffer,
					Parameters->Size,
					Parameters->Flags,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_SEND_RESULT);
			}
			// unlock data buffer
			UF_UnlockUserBuffer( Mdl, SystemBuffer );
		}
		break;
	case IOCTL_KSOCK_RECV:
		{
			PKSOCKET_RECV_PARAMETERS Parameters = (PKSOCKET_RECV_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_RECV_RESULT Result = (PKSOCKET_RECV_RESULT)Irp->AssociatedIrp.SystemBuffer;
			PMDL Mdl = NULL;
			PVOID SystemBuffer = NULL;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_RECV_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SEND, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_RECV_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SEND, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			// map and lock user buffer
			ntStatus = 
				UF_LockUserBuffer( 
					Parameters->DataPtr, 
					Parameters->Size, 
					IoModifyAccess,
					&Mdl, 
					&SystemBuffer 
					);
			if ( !NT_SUCCESS ( ntStatus ) ){
				DBGLOG((LError, "IOCTL_KSOCK_RECV, %08X=UF_LockUserBuffer(%p,%lu) failed\n",
					ntStatus,Parameters->DataPtr,Parameters->Size));
				break;
			}
			ntStatus = 
				KSockRecv(
					IoStack->FileObject,
					SystemBuffer,
					Parameters->Size,
					Parameters->Flags,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_SEND_RESULT);
			}
			// unlock data buffer
			UF_UnlockUserBuffer( Mdl, SystemBuffer );
		}
		break;
	case IOCTL_KSOCK_SENDTO:
		{
			PKSOCKET_SEND_TO_PARAMETERS Parameters = (PKSOCKET_SEND_TO_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_SEND_TO_RESULT Result = (PKSOCKET_SEND_TO_RESULT)Irp->AssociatedIrp.SystemBuffer;
			PMDL Mdl = NULL;
			PVOID SystemBuffer = NULL;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_SEND_TO_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SENDTO, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_SEND_TO_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SENDTO, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			// map and lock user buffer
			ntStatus = 
				UF_LockUserBuffer( 
					Parameters->DataPtr, 
					Parameters->Size, 
					IoReadAccess,
					&Mdl, 
					&SystemBuffer 
					);
			if ( !NT_SUCCESS ( ntStatus ) ){
				DBGLOG((LError, "IOCTL_KSOCK_SENDTO, %08X=UF_LockUserBuffer(%p,%lu) failed\n",
					ntStatus,Parameters->DataPtr,Parameters->Size));
				break;
			}
			ntStatus = 
				KSockSendTo(
					IoStack->FileObject,
					SystemBuffer,
					Parameters->Size,
					Parameters->Flags,
					&Parameters->Address,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_SEND_TO_RESULT);
			}
			// unlock data buffer
			UF_UnlockUserBuffer( Mdl, SystemBuffer );
		}
		break;
	case IOCTL_KSOCK_RECVFROM:
		{
			PKSOCKET_RECV_FROM_PARAMETERS Parameters = (PKSOCKET_RECV_FROM_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_RECV_FROM_RESULT Result = (PKSOCKET_RECV_FROM_RESULT)Irp->AssociatedIrp.SystemBuffer;
			PMDL Mdl = NULL;
			PVOID SystemBuffer = NULL;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_RECV_FROM_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_RECVFROM, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_RECV_FROM_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_RECVFROM, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			// map and lock user buffer
			ntStatus = 
				UF_LockUserBuffer( 
					Parameters->DataPtr, 
					Parameters->Size,
					IoModifyAccess,
					&Mdl, 
					&SystemBuffer 
					);
			if ( !NT_SUCCESS ( ntStatus ) ){
				DBGLOG((LError, "IOCTL_KSOCK_RECVFROM, %08X=UF_LockUserBuffer(%p,%lu) failed\n",
					ntStatus,Parameters->DataPtr,Parameters->Size));
				break;
			}
			ntStatus = 
				KSockRecvFrom(
					IoStack->FileObject,
					SystemBuffer,
					Parameters->Size,
					Parameters->Flags,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_RECV_FROM_RESULT);
			}
			// unlock data buffer
			UF_UnlockUserBuffer( Mdl, SystemBuffer );
		}
		break;
	case IOCTL_KSOCK_SHUTDOWN:
		{
			PKSOCKET_SHUTDOWN_PARAMETERS Parameters = (PKSOCKET_SHUTDOWN_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_SHUTDOWN_RESULT Result = (PKSOCKET_SHUTDOWN_RESULT)Irp->AssociatedIrp.SystemBuffer;
			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_SHUTDOWN_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SHUTDOWN, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_SHUTDOWN_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SHUTDOWN, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}
			ntStatus = 
				KSockShutdown(
					IoStack->FileObject,
					Parameters,
					Result
					);
		}
		break;
	case IOCTL_KSOCK_IOCTL:
		{
			PKSOCKET_IOCTL_PARAMETERS Parameters = (PKSOCKET_IOCTL_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_IOCTL_RESULT Result = (PKSOCKET_IOCTL_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_IOCTL_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_IOCTL, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_IOCTL_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_IOCTL, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockIoctl(
					IoStack->FileObject,
					Parameters->Cmd,
					Parameters->arg_p,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_IOCTL_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_GETSOCKOPT:
		{
			PKSOCKET_GET_SOCK_OPT_PARAMETERS Parameters = (PKSOCKET_GET_SOCK_OPT_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_GET_SOCK_OPT_RESULT Result = (PKSOCKET_GET_SOCK_OPT_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_GET_SOCK_OPT_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_GETSOCKOPT, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_GET_SOCK_OPT_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_GETSOCKOPT, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockGetSockOpt(
					IoStack->FileObject,
					Parameters->level,
					Parameters->optname,
					Result,
					IoStack->Parameters.DeviceIoControl.OutputBufferLength
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = Result->optlen + sizeof(KSOCKET_GET_SOCK_OPT_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_SETSOCKOPT:
		{
			PKSOCKET_SET_SOCK_OPT_PARAMETERS Parameters = (PKSOCKET_SET_SOCK_OPT_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_SET_SOCK_OPT_RESULT Result = (PKSOCKET_SET_SOCK_OPT_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_SET_SOCK_OPT_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SETSOCKOPT, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( sizeof(KSOCKET_SET_SOCK_OPT_PARAMETERS) + Parameters->optlen > IoStack->Parameters.DeviceIoControl.InputBufferLength )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SETSOCKOPT, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_SET_SOCK_OPT_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SETSOCKOPT, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockSetSockOpt(
					IoStack->FileObject,
					Parameters->level,
					Parameters->optname,
					(char*)Parameters->optval,
					Parameters->optlen,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_SET_SOCK_OPT_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_GETSOCKNAME:
		{
			PKSOCKET_GET_SOCK_NAME_RESULT Result = (PKSOCKET_GET_SOCK_NAME_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_GET_SOCK_NAME_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_GETSOCKOPT, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockGetSockName(
					IoStack->FileObject,		
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_GET_SOCK_NAME_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_GETPEERNAME:
		{
			PKSOCKET_GET_PEER_NAME_RESULT Result = (PKSOCKET_GET_PEER_NAME_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_GET_PEER_NAME_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_GETSOCKOPT, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockGetPeerName(
					IoStack->FileObject,			
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_GET_PEER_NAME_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_GETHOSTBYNAME:
		{
			PKSOCKET_GET_HOST_BY_NAME_PARAMETERS Parameters = (PKSOCKET_GET_HOST_BY_NAME_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_GET_HOST_BY_NAME_RESULT Result = (PKSOCKET_GET_HOST_BY_NAME_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_GET_HOST_BY_NAME_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_GETHOSTBYNAME, invalid in parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_GET_HOST_BY_NAME_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_GETHOSTBYNAME, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockGetHostByName(
					Parameters->hostname,
					IoStack->Parameters.DeviceIoControl.InputBufferLength,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_GET_HOST_BY_NAME_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_LISTEN:
		{
			PKSOCKET_LISTEN_PARAMETERS Parameters = (PKSOCKET_LISTEN_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_LISTEN_RESULT Result = (PKSOCKET_LISTEN_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_LISTEN_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_LISTEN, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockListen(
					IoStack->FileObject,					
					Parameters,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_LISTEN_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_ACCEPT:
		{
			PKSOCKET_ACCEPT_RESULT Result = (PKSOCKET_ACCEPT_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_ACCEPT_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_GETSOCKOPT, invalid parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockAccept(
					IoStack->FileObject,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_ACCEPT_RESULT);
			}
		}
		break;
	case IOCTL_KSOCK_SELECT:
		{
			PKSOCKET_SELECT_PARAMETERS Parameters = (PKSOCKET_SELECT_PARAMETERS)Irp->AssociatedIrp.SystemBuffer;
			PKSOCKET_SELECT_RESULT Result = (PKSOCKET_SELECT_RESULT)Irp->AssociatedIrp.SystemBuffer;

			if ( IoStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(KSOCKET_SELECT_PARAMETERS) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SELECT, invalid in parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.InputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			if ( IoStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(KSOCKET_SELECT_RESULT) )
			{
				DBGLOG((LError, "IOCTL_KSOCK_SELECT, invalid out parameter length, %lu\n",
					IoStack->Parameters.DeviceIoControl.OutputBufferLength));
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = 
				KSockSelect(
					Parameters,
					Result
					);
			if ( NT_SUCCESS ( ntStatus ) ){
				ReturnedBytes = sizeof(KSOCKET_SELECT_RESULT);
			}
		}
		break;
	}	
	Irp -> IoStatus.Status = ntStatus;
	Irp -> IoStatus.Information = ReturnedBytes;

	IoCompleteRequest ( Irp, IO_NO_INCREMENT );

	return ntStatus;
}


NTSTATUS
	KipCommonDispatchRoutine(
		IN PDEVICE_OBJECT DeviceObject,
		IN PIRP	Irp
		)
{
	NTSTATUS	ntStatus = STATUS_NOT_IMPLEMENTED;

	if (DeviceObject == g_KipDeviceObject)
	{
		PIO_STACK_LOCATION IoStack = IoGetCurrentIrpStackLocation(Irp);

		switch(IoStack->MajorFunction)
		{
		case IRP_MJ_CREATE:
			ntStatus = KipDispatchCreate(Irp, IoStack);	
			break;
		case IRP_MJ_CLOSE:
			ntStatus = KipDispatchClose(Irp, IoStack);
			break;
		case IRP_MJ_CLEANUP:
			ntStatus = KipDispatchCleanup(Irp, IoStack);
			break;
		case IRP_MJ_INTERNAL_DEVICE_CONTROL:
			ntStatus = KipDispatchInternelIoControl(Irp, IoStack);
			break;
		case IRP_MJ_DEVICE_CONTROL:
			ntStatus = KipDispatchIoControl(Irp, IoStack);
			break;
		default:
			break;
		}	// switch(IoStack->MajorFunction)
	}
	else
		// Do not complete IRP, it will be completed by BkDrv.lib
		ntStatus = STATUS_NO_SUCH_DEVICE;

	return(ntStatus);
}


NTSTATUS 
	KipDriverEntry( 
		IN PDRIVER_OBJECT DriverObject,
		IN PUNICODE_STRING RegistryPath
		)
{
	UNICODE_STRING	DeviceName;
	UNICODE_STRING	SymbolicLinkName;
	int				Counter = 0;
	NTSTATUS		Status = STATUS_SUCCESS;

	// Initialize device name string
	RtlInitUnicodeString ( &DeviceName, KIP_DEVICE_NAME );

	Status = BkCreateRandomNamedDevice(DriverObject, KIP_DEVICE_NAME_SEED, FILE_DEVICE_NETWORK, 
		&g_KipDeviceName, &g_KipDeviceObject);

	if ( Status == STATUS_SUCCESS )
	{
		DBGLOG(( LTrace, "Created named device %wZ\n", g_KipDeviceName ));

		if (g_KipDispatchTable = BkAllocateDriverDispatchTable())
		{
			// Create symbolic link for device
			Status = BkCreateRandomNamedLink(g_KipDeviceName, KIP_DEVICE_NAME_SEED, &g_KipSymbolicLink);

			g_KipDeviceObject->Flags = DO_DIRECT_IO;

			// Initialize dispatcher handler
			g_KipDispatchTable->MajorFunction [ IRP_MJ_DEVICE_CONTROL ]          = KipCommonDispatchRoutine;
			g_KipDispatchTable->MajorFunction [ IRP_MJ_INTERNAL_DEVICE_CONTROL ] = KipCommonDispatchRoutine;
			g_KipDispatchTable->MajorFunction [ IRP_MJ_CREATE ]   = KipCommonDispatchRoutine;
			g_KipDispatchTable->MajorFunction [ IRP_MJ_CLOSE ]    = KipCommonDispatchRoutine;
			g_KipDispatchTable->MajorFunction [ IRP_MJ_CLEANUP ]  = KipCommonDispatchRoutine;
			//DriverObject->MajorFunction [ IRP_MJ_SHUTDOWN ] = DriverDispatchShutdown;

			BkRegisterDriverDispatchTable(g_KipDispatchTable);
		}
		else
			Status = STATUS_INSUFFICIENT_RESOURCES;

		DriverObject->DriverUnload = NULL;
	}else{
		DBGLOG(( LCriticalError, "Failed to create device %wZ, error=%08X\n", g_KipDeviceName, Status ));
	}

	return Status;
}
