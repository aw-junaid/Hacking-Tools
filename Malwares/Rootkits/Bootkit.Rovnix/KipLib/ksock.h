//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP protocol driver
//	
// module: ksock.h
// $Revision: 102 $
// $Date: 2012-08-16 13:41:17 +0400 (Чт, 16 авг 2012) $
// description: 
//  kernel sockets interface

#ifndef __KSOCK_H_
#define __KSOCK_H_

#define KSOCK_CONTROL_OBJECT 0
#define KSOCK_SOCKET_OBJECT  1

// socket states
#define KSOCK_STATE_CREATED  0
#define KSOCK_STATE_BIND     1
#define KSOCK_STATE_CONNECTED     2
#define KSOCK_STATE_SHUTDOWN 3
#define KSOCK_STATE_LISTEN   4

typedef struct _KSOCK_OBJECT
{
	HANDLE hSocket;
	ULONG State;
}KSOCK_OBJECT,*PKSOCK_OBJECT;

LONG KSockGetEngine( VOID );
NTSTATUS KSockSetEngine( LONG ID );

NTSTATUS 
	KSockCreateSocket (
		IN PIRP Irp,
		IN PIO_STACK_LOCATION IoStack,
		IN PFILE_FULL_EA_INFORMATION StartEA
		);

NTSTATUS 
	KSockCloseSocket (
		IN PFILE_OBJECT FileObject
		);

NTSTATUS 
	KSockBind (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_BIND_PARAMETERS BindParameters,
		IN PKSOCKET_BIND_RESULT BindResult
		);

NTSTATUS 
	KSockConnect (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_CONNECT_PARAMETERS ConnectParameters,
		IN PKSOCKET_CONNECT_RESULT ConnectResult
		);

NTSTATUS 
	KSockShutdown (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_SHUTDOWN_PARAMETERS ShutdownParameters,
		IN PKSOCKET_SHUTDOWN_RESULT ShutdownResult
		);

NTSTATUS 
	KSockSend (
		IN PFILE_OBJECT FileObject,
		IN PVOID DataPtr,
		IN ULONG Length,
		IN ULONG Flags,
		IN PKSOCKET_SEND_RESULT SendResult
		);

NTSTATUS 
	KSockRecv (
		IN PFILE_OBJECT FileObject,
		IN PVOID DataPtr,
		IN ULONG Length,
		IN ULONG Flags,
		IN PKSOCKET_RECV_RESULT RecvResult
		);

NTSTATUS 
	KSockSendTo (
		IN PFILE_OBJECT FileObject,
		IN PVOID DataPtr,
		IN ULONG Length,
		IN ULONG Flags,
		IN PSOCKET_ADDR_IN Address,
		IN PKSOCKET_SEND_TO_RESULT SendResult
		);

NTSTATUS 
	KSockRecvFrom (
		IN PFILE_OBJECT FileObject,
		IN PVOID DataPtr,
		IN ULONG Length,
		IN ULONG Flags,
		IN PKSOCKET_RECV_FROM_RESULT RecvResult
		);

NTSTATUS 
	KSockIoctl (
		IN PFILE_OBJECT FileObject,
		IN long Command,
		IN ULONG arg_p,
		IN PKSOCKET_IOCTL_RESULT Result
		);

NTSTATUS 
	KSockGetSockOpt (
		IN PFILE_OBJECT FileObject,
		IN int level,
		IN int optname,
		IN PKSOCKET_GET_SOCK_OPT_RESULT Result,
		IN ULONG Length
		);

NTSTATUS 
	KSockSetSockOpt (
		IN PFILE_OBJECT FileObject,
		IN int level,
		IN int optname,
		OUT  char* optval,
		IN int optlen,
		IN PKSOCKET_SET_SOCK_OPT_RESULT Result
		);

NTSTATUS 
	KSockGetSockName (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_GET_SOCK_NAME_RESULT Result
		);

NTSTATUS 
	KSockGetPeerName (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_GET_PEER_NAME_RESULT Result
		);

NTSTATUS 
	KSockGetHostByName (
		IN char *name,
		IN int length,
		IN PKSOCKET_GET_HOST_BY_NAME_RESULT Result
		);

NTSTATUS 
	KSockListen (
		IN PFILE_OBJECT FileObject,
		IN PKSOCKET_LISTEN_PARAMETERS ListenParameters,
		IN PKSOCKET_LISTEN_RESULT Result
		);

NTSTATUS 
	KSockAccept (
		IN PFILE_OBJECT FileObject,
		OUT PKSOCKET_ACCEPT_RESULT Result
		);

NTSTATUS 
	KSockSelect(
		IN PKSOCKET_SELECT_PARAMETERS Select,
		IN PKSOCKET_SELECT_RESULT Result
		);

#endif //__KSOCK_H_