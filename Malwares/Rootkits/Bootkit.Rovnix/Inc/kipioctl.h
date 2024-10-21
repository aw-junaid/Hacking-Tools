//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP user-mode API library
//	
// module: kipioctl.h
// $Revision: 94 $
// $Date: 2012-06-06 18:00:17 +0400 (Ср, 06 июн 2012) $
// description:
//  User to kernel IOCTL codes and structures.

#ifndef _KIP_IOCTL_H_
#define _KIP_IOCTL_H_

#ifdef _WINDOWS
	//#include <winioctl.h>   // Compiling Win32 Applications Or DLL's
#else

#endif

#pragma warning( disable : 4200 )

#define	KIP_DEVICE_NAME_SEED	0xe9c33c9e

#define KIP_DRIVER_NAMEW L"KIP"
#define KIP_DRIVER_NAMEA "KIP"

#define KIP_DEVICE_NAME     L"\\Device\\" KIP_DRIVER_NAMEW
#define KIP_DOS_DEVICE_NAME L"\\DosDevices\\" KIP_DRIVER_NAMEW

//debug parameters
#define VALUE_DEBUG_LEVEL L"DebugLevel"

#define KipSocketName "KipSocketName"
#define KIP_SOCKET_NAME_LENGTH (sizeof (KipSocketName) - 1)

#define KipSocketConnection "KipSocketConnection"
#define KIP_SOCKET_CONNECTION_LENGTH (sizeof (KipSocketConnection) - 1)

#define KIP_SOCK_LWIP 0 // default
#define KIP_SOCK_TDI  1 // default

// socket table for kernel clients

#ifndef INVALID_SOCKET
#define	SOCKET				UINT_PTR
#define	INVALID_SOCKET		(UINT_PTR)-1
#endif

typedef LONG (_stdcall* FUNC_SOCKET)		(LONG Domain, LONG Type, LONG Protocol);
typedef LONG (_stdcall* FUNC_CLOSE)			(SOCKET Socket);
typedef LONG (_stdcall* FUNC_IOCTL)			(SOCKET Socket, LONG Cmd, PVOID Argp);
typedef LONG (_stdcall* FUNC_SENDTO)		(SOCKET Socket, PVOID Dataptr, LONG Size, ULONG Flags, PVOID To, ULONG Tolen);
typedef LONG (_stdcall* FUNC_RECVFROM)		(SOCKET Socket, PVOID Mem, LONG Len, ULONG Flags, PVOID From, PULONG Fromlen);
typedef LONG (_stdcall* FUNC_GETSOCKOPT)	(SOCKET Socket, LONG Level, LONG Optname, PVOID Optval, PULONG Optlen);
typedef LONG (_stdcall* FUNC_SETSOCKOPT)	(SOCKET Socket, LONG Level, LONG Optname, PVOID Optval, ULONG Optlen);
typedef LONG (_stdcall* FUNC_SELECT)		(LONG Maxfdp1, PVOID Readset, PVOID Writeset, PVOID Exceptset, PVOID Timeout);
typedef LONG (_stdcall* FUNC_CONNECT)		(SOCKET Socket, struct sockaddr *Name, ULONG Namelen);
typedef	LONG (_stdcall* FUNC_BIND)			(SOCKET Socket, struct sockaddr *Name, ULONG Namelen);
typedef LONG (_stdcall* FUNC_SHUTDOWN)		(SOCKET Socket, LONG How);
typedef LONG (_stdcall* FUNC_SEND)			(SOCKET Socket, PVOID Dataptr, LONG Size, ULONG Flags);
typedef LONG (_stdcall* FUNC_GETSOCKNAME)	(SOCKET Socket, struct sockaddr *Name, PULONG	Namelen);
typedef LONG (_stdcall* FUNC_RECV)			(SOCKET Socket, PVOID Mem, LONG Len, ULONG Flags);
typedef LONG (_stdcall* FUNC_GETPEERNAME)	(SOCKET Socket, struct sockaddr *Name, PULONG	Namelen);

typedef struct hostent* (_stdcall* FUNC_GETHOSTBYNAME)(const char *name);
typedef int (_stdcall* FUNC_GETHOSTBYNAMER)(const char *name, struct hostent *ret, char *buf,
	size_t buflen, struct hostent **result, int *h_errnop);

typedef LONG (_stdcall* FUNC_LISTEN)	   (SOCKET Socket, int backlog );
typedef LONG (_stdcall* FUNC_ACCEPT)	   (SOCKET Socket, struct sockaddr *addr, PULONG	addrlen);

//typedef LONG (_stdcall* FUNC_SELECT)(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,struct timeval *timeout);

typedef struct _TSOCKET_TABLE
{
	FUNC_SOCKET			socket;
	FUNC_CLOSE			close;
	FUNC_IOCTL			ioctl;
	FUNC_SENDTO			sendto;
	FUNC_RECVFROM		recvfrom;
	FUNC_GETSOCKOPT		getsockopt;
	FUNC_SELECT			select;
	FUNC_CONNECT		connect;
	FUNC_SETSOCKOPT		setsockopt;
	FUNC_BIND			bind;
	FUNC_SHUTDOWN		shutdown;
	FUNC_SEND			send;
	FUNC_GETSOCKNAME	getsockname;
	FUNC_RECV			recv;
	FUNC_GETPEERNAME	getpeername;
	FUNC_GETHOSTBYNAME  gethostbyname;
	FUNC_GETHOSTBYNAMER gethostbynamer;
	FUNC_LISTEN         listen;
	FUNC_ACCEPT         accept;

} TSOCKET_TABLE, *PTSOCKET_TABLE;

typedef struct _FD_SET_INTERNAL {
	unsigned int   fd_count;               /* how many are SET? */
	HANDLE  fd_array[64/*FD_SETSIZE*/];   /* an array of SOCKETs */
} FD_SET_INTERNAL,*PFD_SET_INTERNAL;

typedef struct _TIMEVAL_INTERNAL {
	long    tv_sec;         /* seconds */
	long    tv_usec;        /* and microseconds */
}TIMEVAL_INTERNAL,*PTIMEVAL_INTERNAL;

// for KSOCK_SWITCH
typedef struct _KSOCKET_ENGINE
{
	LONG Engine;
}KSOCKET_ENGINE,*PKSOCKET_ENGINE;

typedef struct _KSOCKET_FUNC_TABLE
{
	PTSOCKET_TABLE SockTable;
}KSOCKET_FUNC_TABLE,*PKSOCKET_FUNC_TABLE;

typedef struct _SOCKET_ADDR_IN {
	unsigned char sin_len;
	unsigned char sin_family;
	unsigned short sin_port;
	unsigned long  in_addr;
	char sin_zero[8];
}SOCKET_ADDR_IN,*PSOCKET_ADDR_IN;

typedef struct _KSOCKET_RESULT
{
	int error_code;
}KSOCKET_RESULT,*PKSOCKET_RESULT;

// for socket creation
typedef struct _KSOCKET_CREATE_PARAMETERS
{
	int Domain; 
	int Type; 
	int Protocol;
}KSOCKET_CREATE_PARAMETERS,*PKSOCKET_CREATE_PARAMETERS;

// for socket creation
typedef struct _KSOCKET_CONNECTION_PARAMETERS
{
	int Socket;
}KSOCKET_CONNECTION_PARAMETERS,*PKSOCKET_CONNECTION_PARAMETERS;

// [in] for socket binding
typedef struct _KSOCKET_BIND_PARAMETERS
{
	SOCKET_ADDR_IN Name;
}KSOCKET_BIND_PARAMETERS,*PKSOCKET_BIND_PARAMETERS;

typedef struct _KSOCKET_RESULT KSOCKET_BIND_RESULT,*PKSOCKET_BIND_RESULT;

// [in] for socket connection
typedef struct _KSOCKET_CONNECT_PARAMETERS
{
	SOCKET_ADDR_IN Name;
}KSOCKET_CONNECT_PARAMETERS,*PKSOCKET_CONNECT_PARAMETERS;

typedef struct _KSOCKET_RESULT KSOCKET_CONNECT_RESULT,*PKSOCKET_CONNECT_RESULT;

// [in] for socket shutdown
typedef struct _KSOCKET_SHUTDOWN_PARAMETERS
{
	int How;
}KSOCKET_SHUTDOWN_PARAMETERS,*PKSOCKET_SHUTDOWN_PARAMETERS;

typedef struct _KSOCKET_RESULT KSOCKET_SHUTDOWN_RESULT,*PKSOCKET_SHUTDOWN_RESULT;

// [in] for socket send
typedef struct _KSOCKET_SEND_PARAMETERS
{
	union
	{
		PVOID DataPtr;
		// Following value used when passing a pointer from a WOW64 process to a 64-bit driver,
		//  in this case high part of Alignment must be zero.
		ULONGLONG	Alignment;
	};
	ULONG Size;

//#define MSG_PEEK       0x01    /* Peeks at an incoming message */
//#define MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
//#define MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
//#define MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
//#define MSG_MORE       0x10    /* Sender will send more */

	ULONG Flags;
}KSOCKET_SEND_PARAMETERS,*PKSOCKET_SEND_PARAMETERS;

// [out] for socket send
typedef struct _KSOCKET_SEND_RESULT
{
	int error_code;
	ULONG BytesSent;
}KSOCKET_SEND_RESULT,*PKSOCKET_SEND_RESULT;

// [in] for socket recv
typedef struct _KSOCKET_RECV_PARAMETERS
{
	union
	{
		PVOID DataPtr;
		// Following value used when passing a pointer from a WOW64 process to a 64-bit driver,
		//  in this case high part of Alignment must be zero.
		ULONGLONG	Alignment;
	};
	ULONG Size;
	ULONG Flags;
}KSOCKET_RECV_PARAMETERS,*PKSOCKET_RECV_PARAMETERS;

// [out] for socket recv
typedef struct _KSOCKET_RECV_RESULT
{
	int error_code;
	ULONG BytesRecv;
}KSOCKET_RECV_RESULT,*PKSOCKET_RECV_RESULT;

// [in] for socket send to
typedef struct _KSOCKET_SEND_TO_PARAMETERS
{
	union
	{
		PVOID DataPtr;
		// Following value used when passing a pointer from a WOW64 process to a 64-bit driver,
		//  in this case high part of Alignment must be zero.
		ULONGLONG	Alignment;
	};
	ULONG Size;

	//#define MSG_PEEK       0x01    /* Peeks at an incoming message */
	//#define MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
	//#define MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
	//#define MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
	//#define MSG_MORE       0x10    /* Sender will send more */

	ULONG Flags;
	SOCKET_ADDR_IN Address;
}KSOCKET_SEND_TO_PARAMETERS,*PKSOCKET_SEND_TO_PARAMETERS;

// [out] for socket send to
typedef struct _KSOCKET_SEND_TO_RESULT
{
	int error_code;
	ULONG BytesSent;
}KSOCKET_SEND_TO_RESULT,*PKSOCKET_SEND_TO_RESULT;

// [in] for socket recv from
typedef struct _KSOCKET_RECV_FROM_PARAMETERS
{
	union
	{
		PVOID DataPtr;
		// Following value used when passing a pointer from a WOW64 process to a 64-bit driver,
		//  in this case high part of Alignment must be zero.
		ULONGLONG	Alignment;
	};
	ULONG Size;
	ULONG Flags;
}KSOCKET_RECV_FROM_PARAMETERS,*PKSOCKET_RECV_FROM_PARAMETERS;

// [out] for socket recv from
typedef struct _KSOCKET_RECV_FROM_RESULT
{
	int error_code;
	SOCKET_ADDR_IN Address;
	ULONG BytesRecv;
}KSOCKET_RECV_FROM_RESULT,*PKSOCKET_RECV_FROM_RESULT;

// [in out] for socket ioctl
typedef struct _KSOCKET_IOCTL_PARAMETERS
{
	long Cmd;
	ULONG arg_p;
}KSOCKET_IOCTL_PARAMETERS,*PKSOCKET_IOCTL_PARAMETERS;

typedef struct _KSOCKET_IOCTL_RESULT
{
	int error_code;
	ULONG arg_p;
}KSOCKET_IOCTL_RESULT,*PKSOCKET_IOCTL_RESULT;

// [in] for getsockopt
typedef struct _KSOCKET_GET_SOCK_OPT_PARAMETERS
{
	int level;
	int optname;
}KSOCKET_GET_SOCK_OPT_PARAMETERS,*PKSOCKET_GET_SOCK_OPT_PARAMETERS;

typedef struct _KSOCKET_GET_SOCK_OPT_RESULT
{
	int error_code;
	int optlen;
	char optval[0];
}KSOCKET_GET_SOCK_OPT_RESULT,*PKSOCKET_GET_SOCK_OPT_RESULT;

// on output SystemBuffer contains optval

// [in] for setsockopt
typedef struct _KSOCKET_SET_SOCK_OPT_PARAMETERS
{
	int level;
	int optname;
	int optlen;
	char optval[0];
}KSOCKET_SET_SOCK_OPT_PARAMETERS,*PKSOCKET_SET_SOCK_OPT_PARAMETERS;

typedef struct _KSOCKET_RESULT KSOCKET_SET_SOCK_OPT_RESULT,*PKSOCKET_SET_SOCK_OPT_RESULT;

// [out] for getsockname
typedef struct _KSOCKET_GET_SOCK_NAME_RESULT
{
	int error_code;
	SOCKET_ADDR_IN Address;
}KSOCKET_GET_SOCK_NAME_RESULT,*PKSOCKET_GET_SOCK_NAME_RESULT;

// [out] for getsockname
typedef struct _KSOCKET_GET_PEER_NAME_RESULT
{
	int error_code;
	SOCKET_ADDR_IN Address;
}KSOCKET_GET_PEER_NAME_RESULT,*PKSOCKET_GET_PEER_NAME_RESULT;

// [out] for gethostbyname
typedef struct _KSOCKET_GET_HOST_BY_NAME_PARAMETERS
{
	char hostname[0];
}KSOCKET_GET_HOST_BY_NAME_PARAMETERS,*PKSOCKET_GET_HOST_BY_NAME_PARAMETERS;

typedef struct _KSOCKET_GET_HOST_BY_NAME_RESULT
{
	int error_code;
	unsigned long  in_addr;
}KSOCKET_GET_HOST_BY_NAME_RESULT,*PKSOCKET_GET_HOST_BY_NAME_RESULT;

// [in] for listen
typedef struct _KSOCKET_LISTEN_PARAMETERS
{
	int backlog;
}KSOCKET_LISTEN_PARAMETERS,*PKSOCKET_LISTEN_PARAMETERS;

typedef struct _KSOCKET_RESULT KSOCKET_LISTEN_RESULT,*PKSOCKET_LISTEN_RESULT;

// [out] for accept
typedef struct _KSOCKET_ACCEPT_RESULT
{
	int error_code;
	int Socket;
	SOCKET_ADDR_IN Address;
}KSOCKET_ACCEPT_RESULT,*PKSOCKET_ACCEPT_RESULT;

//[in out] for select
#define READFDS_VALID		0x1
#define WRITEFDS_VALID		0x2
#define EXCEPTFDS_VALID		0x4
#define TIMEOUT_VALID		0x8

typedef struct _KSOCKET_SELECT_PARAMETERS
{
	CHAR ValidFields;
	int nfds;

	FD_SET_INTERNAL readfds;
	FD_SET_INTERNAL writefds;
	FD_SET_INTERNAL exceptfds;

	TIMEVAL_INTERNAL timeout;

}KSOCKET_SELECT_PARAMETERS,*PKSOCKET_SELECT_PARAMETERS;

typedef struct _KSOCKET_SELECT_RESULT
{
	int error_code;
	int number;

	FD_SET_INTERNAL readfds;
	FD_SET_INTERNAL writefds;
	FD_SET_INTERNAL exceptfds;

}KSOCKET_SELECT_RESULT,*PKSOCKET_SELECT_RESULT;

#define FILE_DEVICE_KIP		0x00008400
#define KIP_IOCTL_INDEX		0x840

#define _KIP_IOCTL_BUFFERED(_CODE_) \
	CTL_CODE(FILE_DEVICE_KIP, KIP_IOCTL_INDEX+(_CODE_), METHOD_BUFFERED, FILE_ANY_ACCESS)

#define _KIP_IOCTL_DIRECT(_CODE_) \
	CTL_CODE(FILE_DEVICE_KIP, KIP_IOCTL_INDEX+(_CODE_), METHOD_NEITHER, FILE_ANY_ACCESS)

#define KSOCK_BIND			1
#define KSOCK_CONNECT		2
#define KSOCK_SEND			3
#define KSOCK_RECV			4
#define KSOCK_SENDTO		5
#define KSOCK_RECVFROM		6
#define KSOCK_SHUTDOWN		7
#define KSOCK_IOCTL			8
#define KSOCK_GETSOCKOPT	9
#define KSOCK_SETSOCKOPT	10
#define KSOCK_GETSOCKNAME	11
#define KSOCK_GETPEERNAME	12
#define KSOCK_GETHOSTBYNAME	13
#define KSOCK_LISTEN		14
#define KSOCK_ACCEPT		15
#define KSOCK_SELECT		16

#define KSOCK_GET_ENGINE    50
#define KSOCK_SET_ENGINE    51
#define KSOCK_GET_SOCK_TABLE    100

#define IOCTL_KSOCK_BIND			_KIP_IOCTL_BUFFERED(KSOCK_BIND)
#define IOCTL_KSOCK_CONNECT			_KIP_IOCTL_BUFFERED(KSOCK_CONNECT)
#define IOCTL_KSOCK_SEND			_KIP_IOCTL_BUFFERED(KSOCK_SEND)
#define IOCTL_KSOCK_RECV			_KIP_IOCTL_BUFFERED(KSOCK_RECV)
#define IOCTL_KSOCK_SENDTO			_KIP_IOCTL_BUFFERED(KSOCK_SENDTO)
#define IOCTL_KSOCK_RECVFROM		_KIP_IOCTL_BUFFERED(KSOCK_RECVFROM)
#define IOCTL_KSOCK_SHUTDOWN		_KIP_IOCTL_BUFFERED(KSOCK_SHUTDOWN)
#define IOCTL_KSOCK_IOCTL			_KIP_IOCTL_BUFFERED(KSOCK_IOCTL)
#define IOCTL_KSOCK_GETSOCKOPT		_KIP_IOCTL_BUFFERED(KSOCK_GETSOCKOPT)
#define IOCTL_KSOCK_SETSOCKOPT		_KIP_IOCTL_BUFFERED(KSOCK_SETSOCKOPT)
#define IOCTL_KSOCK_GETSOCKNAME		_KIP_IOCTL_BUFFERED(KSOCK_GETSOCKNAME)
#define IOCTL_KSOCK_GETPEERNAME		_KIP_IOCTL_BUFFERED(KSOCK_GETPEERNAME)

#define IOCTL_KSOCK_GETHOSTBYNAME		_KIP_IOCTL_BUFFERED(KSOCK_GETHOSTBYNAME)

#define IOCTL_KSOCK_LISTEN		    _KIP_IOCTL_BUFFERED(KSOCK_LISTEN)
#define IOCTL_KSOCK_ACCEPT		    _KIP_IOCTL_BUFFERED(KSOCK_ACCEPT)
#define IOCTL_KSOCK_SELECT		    _KIP_IOCTL_BUFFERED(KSOCK_SELECT)

#define IOCTL_KSOCK_GET_ENGINE	    _KIP_IOCTL_BUFFERED(KSOCK_GET_ENGINE)
#define IOCTL_KSOCK_SET_ENGINE	    _KIP_IOCTL_BUFFERED(KSOCK_SET_ENGINE)

// input KSOCKET_ENGINE
// output KSOCKET_ENGINE
#define IOCTL_INTERNAL_GET_SOCK_TABLE	    _KIP_IOCTL_BUFFERED(KSOCK_GET_SOCK_TABLE)

#endif //_KIP_IOCTL_H_