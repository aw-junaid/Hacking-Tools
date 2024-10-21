//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP user-mode API library
//	
// module: kipapi.c
// $Revision: 94 $
// $Date: 2012-06-06 18:00:17 +0400 (Ср, 06 июн 2012) $
// description:
//  User-mode API implementation.

#include "project.h"
#include "err.h"

#ifndef FIELD_SIZE
#define FIELD_SIZE(type, field) (sizeof(((type *)0)->field))
#endif

extern HANDLE	CreateSocket( int domain, int type, int protocol );
extern HANDLE	CreateConnectionSocket( int Socket );
extern DWORD	ErrNoToWSAERROR( int err );
extern HANDLE 	CreateSocketNative(PVOID Ea, IN ULONG EaLength);

#define	OpenKipDriver()	CreateSocketNative(NULL, 0)

///////////////////////////////////////////////////////////////////////////////////////
SOCKET __stdcall _tsocket( int domain, int type, int protocol )
{
	return (SOCKET)CreateSocket(domain, type, protocol);
}

///////////////////////////////////////////////////////////////////////////////////////
int __stdcall _tclosesocket( SOCKET Socket )
{
	CloseHandle((HANDLE)Socket);
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tbind( SOCKET s, const struct sockaddr* name, int namelen )
{
	KSOCKET_BIND_PARAMETERS BindParams;
	KSOCKET_BIND_RESULT Result = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	if ( namelen != sizeof(struct sockaddr_in) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	BindParams.Name.sin_len = sizeof(struct sockaddr_in);
	BindParams.Name.sin_family = (unsigned char)((struct sockaddr_in*)name)->sin_family;
	BindParams.Name.sin_port = ((struct sockaddr_in*)name)->sin_port;
	BindParams.Name.in_addr = ((struct sockaddr_in*)name)->sin_addr.s_addr;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_BIND,
			&BindParams,
			sizeof(BindParams),
			&Result,
			sizeof(Result),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int __stdcall _tconnect( SOCKET s, const struct sockaddr* name, int namelen )
{
	KSOCKET_CONNECT_PARAMETERS ConnectParams = {0};
	KSOCKET_CONNECT_RESULT Result = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	if ( namelen != sizeof(struct sockaddr_in) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	ConnectParams.Name.sin_len = sizeof(struct sockaddr_in);
	ConnectParams.Name.sin_family = (unsigned char)((struct sockaddr_in*)name)->sin_family;
	ConnectParams.Name.sin_port = ((struct sockaddr_in*)name)->sin_port;
	ConnectParams.Name.in_addr = ((struct sockaddr_in*)name)->sin_addr.s_addr;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_CONNECT,
			&ConnectParams,
			sizeof(ConnectParams),
			&Result,sizeof(Result),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( Result.error_code != 0 ){
		if ( Result.error_code == ECONNABORTED || Result.error_code == ECONNRESET ){
			Result.error_code = ECONNREFUSED;
		}
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tsend( SOCKET s,  const char* buf, int len, int flags )
{
	KSOCKET_SEND_PARAMETERS SendParams = {0};
	KSOCKET_SEND_RESULT SendResult = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	SendParams.DataPtr = (PVOID)buf;
	SendParams.Size = len;
	SendParams.Flags = flags;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_SEND,
			&SendParams,
			sizeof(SendParams),
			&SendResult,sizeof(SendResult),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(SendResult) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( SendResult.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (SendResult.error_code) );
		return SOCKET_ERROR;
	}
	return SendResult.BytesSent;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _trecv( SOCKET s,  const char* buf, int len, int flags )
{
	KSOCKET_RECV_PARAMETERS RecvParams = {0};
	KSOCKET_RECV_RESULT RecvResult = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	RecvParams.DataPtr = (PVOID)buf;
	RecvParams.Size = len;
	RecvParams.Flags = flags;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_RECV,
			&RecvParams,
			sizeof(RecvParams),
			&RecvResult,sizeof(RecvResult),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(RecvResult) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( RecvResult.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (RecvResult.error_code) );
		return SOCKET_ERROR;
	}
	return RecvResult.BytesRecv;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tsendto( SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen )
{
	KSOCKET_SEND_TO_PARAMETERS SendParams = {0};
	KSOCKET_SEND_TO_RESULT SendResult = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	SendParams.DataPtr = (PVOID)buf;
	SendParams.Size = len;
	SendParams.Flags = flags;

	if ( tolen != sizeof(struct sockaddr_in) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	SendParams.Address.sin_len = sizeof(struct sockaddr_in);
	SendParams.Address.sin_family = (unsigned char)((struct sockaddr_in*)to)->sin_family;
	SendParams.Address.sin_port = ((struct sockaddr_in*)to)->sin_port;
	SendParams.Address.in_addr = ((struct sockaddr_in*)to)->sin_addr.s_addr;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_SENDTO,
			&SendParams,
			sizeof(SendParams),
			&SendResult,sizeof(SendResult),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(SendResult) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( SendResult.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (SendResult.error_code) );
		return SOCKET_ERROR;
	}
	return SendResult.BytesSent;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _trecvfrom( SOCKET s,  const char* buf, int len, int flags,struct sockaddr* from, int* fromlen )
{
	KSOCKET_RECV_FROM_PARAMETERS RecvParams = {0};
	KSOCKET_RECV_FROM_RESULT RecvResult = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	if ( *fromlen < sizeof(struct sockaddr_in) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	RecvParams.DataPtr = (PVOID)buf;
	RecvParams.Size = len;
	RecvParams.Flags = flags;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_RECVFROM,
			&RecvParams,
			sizeof(RecvParams),
			&RecvResult,sizeof(RecvResult),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(RecvResult) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( RecvResult.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (RecvResult.error_code) );
		return SOCKET_ERROR;
	}
	((struct sockaddr_in*)from)->sin_family = RecvResult.Address.sin_family;
	((struct sockaddr_in*)from)->sin_port = RecvResult.Address.sin_port;
	((struct sockaddr_in*)from)->sin_addr.s_addr = RecvResult.Address.in_addr;
	*fromlen = sizeof(struct sockaddr_in);

	return RecvResult.BytesRecv;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tshutdown( SOCKET s, int how )
{
	KSOCKET_SHUTDOWN_PARAMETERS ShudownParams = {0};
	KSOCKET_SHUTDOWN_RESULT Result = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	if ( how < 0 || how > 2 ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	ShudownParams.How = how;
	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_SHUTDOWN,
			&ShudownParams,
			sizeof(ShudownParams),
			&Result,sizeof(Result),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tioctlsocket( SOCKET s, long cmd, u_long* argp )
{
	KSOCKET_IOCTL_PARAMETERS IoctlParams = {0};
	KSOCKET_IOCTL_RESULT Result = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	IoctlParams.Cmd = cmd;
	IoctlParams.arg_p = *argp;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_IOCTL,
			&IoctlParams,
			sizeof(IoctlParams),
			&Result,sizeof(Result),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}
	*argp = Result.arg_p;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tgetsockopt( SOCKET s, int level, int optname, char* optval, int* optlen )
{
	KSOCKET_GET_SOCK_OPT_PARAMETERS SockOptParams = {0};
	PKSOCKET_GET_SOCK_OPT_RESULT Result = {0};
	CHAR Buffer[100];
	int optlenlocal = *optlen;
	BOOL fbResult;
	ULONG BytesReturned = 0;

	if ( optlenlocal < sizeof(DWORD) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	SockOptParams.level = level;
	SockOptParams.optname = optname;

	Result = (PKSOCKET_GET_SOCK_OPT_RESULT)Buffer;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_GETSOCKOPT,
			&SockOptParams,
			sizeof(SockOptParams),
			Result,sizeof(Result),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned < sizeof(KSOCKET_GET_SOCK_OPT_RESULT) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	if ( BytesReturned < sizeof(KSOCKET_GET_SOCK_OPT_RESULT) + Result->optlen ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	if ( Result->error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result->error_code) );
		return SOCKET_ERROR;
	}

	if ( optlenlocal < Result->optlen ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	*optlen = Result->optlen;
	memcpy(optval,Result->optval,Result->optlen);
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tsetsockopt( SOCKET s, int level, int optname, const char* optval, int optlen )
{
	PKSOCKET_SET_SOCK_OPT_PARAMETERS SockOptParams = {0};
	KSOCKET_SET_SOCK_OPT_RESULT Result = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	SockOptParams = (PKSOCKET_SET_SOCK_OPT_PARAMETERS)LocalAlloc(LMEM_FIXED, sizeof(KSOCKET_SET_SOCK_OPT_PARAMETERS)+optlen);
	if ( !SockOptParams ){
		SetLastError(ERROR_OUTOFMEMORY);
		return SOCKET_ERROR;
	}
	SockOptParams->level = level;
	SockOptParams->optname = optname;
	SockOptParams->optlen = optlen;
	memcpy(SockOptParams->optval,optval,optlen);

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_SETSOCKOPT,
			SockOptParams,
			sizeof(KSOCKET_SET_SOCK_OPT_PARAMETERS)+optlen,
			&Result,sizeof(Result),
			&BytesReturned,
			NULL
			);
	LocalFree ( SockOptParams );
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned < sizeof(PKSOCKET_SET_SOCK_OPT_RESULT) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tgetsockname( SOCKET s, struct sockaddr* name, int* namelen )
{
	KSOCKET_GET_SOCK_NAME_RESULT Result = {0};
	int namelenlocal = *namelen;
	BOOL fbResult;
	ULONG BytesReturned = 0;

	if ( *namelen < sizeof(struct sockaddr_in) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_GETSOCKNAME,
			NULL, 0,
			&Result,
			sizeof(Result),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}

	((struct sockaddr_in*)name)->sin_family = Result.Address.sin_family;
	((struct sockaddr_in*)name)->sin_port = Result.Address.sin_port;
	((struct sockaddr_in*)name)->sin_addr.s_addr = Result.Address.in_addr;
	*namelen = sizeof(struct sockaddr_in);
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tgetpeername( SOCKET s, struct sockaddr* name, int* namelen )
{
	KSOCKET_GET_PEER_NAME_RESULT Result = {0};
	int namelenlocal = *namelen;
	BOOL fbResult;
	ULONG BytesReturned = 0;

	if ( *namelen < sizeof(struct sockaddr_in) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_GETPEERNAME,
			NULL, 0,
			&Result,
			sizeof(Result),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}

	((struct sockaddr_in*)name)->sin_family = Result.Address.sin_family;
	((struct sockaddr_in*)name)->sin_port = Result.Address.sin_port;
	((struct sockaddr_in*)name)->sin_addr.s_addr = Result.Address.in_addr;
	*namelen = sizeof(struct sockaddr_in);

	return 0;
}
//////////////////////////////////////////////////////////////////////////
SOCKET __stdcall _taccept( SOCKET s, struct sockaddr* addr, int* addrlen )
{
	KSOCKET_ACCEPT_RESULT Result = {0};
	SOCKET as;
	int namelenlocal = *addrlen;
	BOOL fbResult;
	ULONG BytesReturned = 0;

	if ( *addrlen < sizeof(struct sockaddr_in) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_ACCEPT,
			NULL, 0,
			&Result,
			sizeof(Result),
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}

	// create new connection socket
	as = (SOCKET)CreateConnectionSocket( Result.Socket );
	if ( as != SOCKET_ERROR ){

		((struct sockaddr_in*)addr)->sin_family = Result.Address.sin_family;
		((struct sockaddr_in*)addr)->sin_port = Result.Address.sin_port;
		((struct sockaddr_in*)addr)->sin_addr.s_addr = Result.Address.in_addr;
		*addrlen = sizeof(struct sockaddr_in);
	}

	return as;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall _tlisten( SOCKET s, int backlog )
{
	KSOCKET_LISTEN_PARAMETERS ListenParams = {0};
	KSOCKET_LISTEN_RESULT Result = {0};
	BOOL fbResult;
	ULONG BytesReturned = 0;

	ListenParams.backlog = backlog;

	fbResult =
		DeviceIoControl(
			(HANDLE)s,
			IOCTL_KSOCK_LISTEN,
			&ListenParams,sizeof(ListenParams),
			&Result, sizeof(Result),	
			&BytesReturned,
			NULL
			);
	if ( fbResult == FALSE ){
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}

	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
typedef struct _HOSTENT_EX
{
	struct hostent	s_hostent;
	PCHAR			s_aliases;
	ULONG			s_hostent_addr;
	PULONG			s_phostent_addr[2];
} HOSTENT_EX, *PHOSTENT_EX;

static	LONG volatile HostentTlsIndex = TLS_OUT_OF_INDEXES;

struct hostent* __stdcall _tgethostbyname( const char *name )
{
	PKSOCKET_GET_HOST_BY_NAME_PARAMETERS Parameters = {0};
	KSOCKET_GET_HOST_BY_NAME_RESULT Result = {0};
	ULONG BytesReturned = 0;
	HANDLE hFile;
	BOOL fbResult;
	int namelen;
	struct hostent *h = NULL;

	if ( name == NULL ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return NULL;
	}

	namelen = lstrlen( name );
	
	// open protocol driver
	hFile = OpenKipDriver();

	if ( hFile != INVALID_HANDLE_VALUE )
	{
		fbResult =
			DeviceIoControl(
				hFile,
				IOCTL_KSOCK_GETHOSTBYNAME,
				(char*)name,namelen+1,
				&Result,sizeof(Result),			
				&BytesReturned,
				NULL
				);

		if ( fbResult == TRUE )
		{
			if ( BytesReturned >= sizeof(Result) )
			{
				if ( Result.error_code == 0 )
				{
					if (HostentTlsIndex == TLS_OUT_OF_INDEXES)
					{
						// No TLS index, allocating
						DWORD	NewIndex = TlsAlloc();
						if (InterlockedCompareExchange(&HostentTlsIndex, NewIndex, TLS_OUT_OF_INDEXES) != TLS_OUT_OF_INDEXES)
							// TLS index already exists
							TlsFree(NewIndex);
					}

					if (HostentTlsIndex != TLS_OUT_OF_INDEXES)
					{
						PHOSTENT_EX	pHostentEx;

						if (!(pHostentEx = (PHOSTENT_EX)TlsGetValue(HostentTlsIndex)))
						{
							// Allocating new HOSTENT_EX structure
							if (pHostentEx = VirtualAlloc(NULL, sizeof(HOSTENT_EX), MEM_COMMIT, PAGE_READWRITE))
								TlsSetValue(HostentTlsIndex, pHostentEx);
						}

						if (pHostentEx)
						{
							/* fill hostent */
							pHostentEx->s_hostent_addr = Result.in_addr;
							pHostentEx->s_phostent_addr[0] = &pHostentEx->s_hostent_addr;
							pHostentEx->s_phostent_addr[1] = NULL;
							pHostentEx->s_hostent.h_name = (char*)name;
							pHostentEx->s_hostent.h_aliases = &pHostentEx->s_aliases;
							pHostentEx->s_hostent.h_addrtype = AF_INET;
							pHostentEx->s_hostent.h_length = sizeof(unsigned long);
							pHostentEx->s_hostent.h_addr_list = (char**)&pHostentEx->s_phostent_addr;
							h = &pHostentEx->s_hostent;
						}	// if (pHostentEx || (pHostentEx == VirtualAlloc(NULL, sizeof(HOSTENT_EX), MEM_COMMIT, PAGE_READWRITE)))
					}	// if (HostentTlsIndex != TLS_OUT_OF_INDEXES)
					
					if (!h){
						SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					}
				} else { //Result.error_code == 0
					SetLastError( ErrNoToWSAERROR (Result.error_code) );
				}				
			}else {
				SetLastError(ERROR_INVALID_PARAMETER);
			}
		}	// if ( fbResult == TRUE )
		CloseHandle( hFile );
	}	// if ( hFile != INVALID_HANDLE_VALUE )
	return h;
}

//////////////////////////////////////////////////////////////////////////
int __stdcall _tselect( int maxfdp1, fd_set *readset, fd_set *writeset, 
					   fd_set *exceptset, const struct timeval *timeout )
{
	KSOCKET_SELECT_PARAMETERS Select = {0};
	KSOCKET_SELECT_RESULT Result = {0};
	HANDLE hFile;
	BOOL fbResult;
	ULONG BytesReturned = 0;
	ULONG Error;
	int fds = 0;
	unsigned int i;

	// sanity checks
	if ( readset == NULL && writeset == NULL && exceptset == NULL ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( readset ){
		if ( readset->fd_count >= FD_SETSIZE ){
			SetLastError(ERROR_INVALID_PARAMETER);
			return SOCKET_ERROR;
		}
		if ( IsBadReadPtr( readset, FIELD_SIZE(fd_set,fd_count) + sizeof(SOCKET) * readset->fd_count ) ){
			SetLastError(ERROR_INVALID_PARAMETER);
			return SOCKET_ERROR;
		}
	}

	if ( writeset ){
		if ( writeset->fd_count >= FD_SETSIZE ){
			SetLastError(ERROR_INVALID_PARAMETER);
			return SOCKET_ERROR;
		}
		if ( IsBadReadPtr( writeset, FIELD_SIZE(fd_set,fd_count) + sizeof(SOCKET) * writeset->fd_count ) ){
			SetLastError(ERROR_INVALID_PARAMETER);
			return SOCKET_ERROR;
		}
	}

	if ( exceptset ){
		if ( exceptset->fd_count >= FD_SETSIZE ){
			SetLastError(ERROR_INVALID_PARAMETER);
			return SOCKET_ERROR;
		}
		if ( IsBadReadPtr( exceptset, FIELD_SIZE(fd_set,fd_count) + sizeof(SOCKET) * exceptset->fd_count ) ){
			SetLastError(ERROR_INVALID_PARAMETER);
			return SOCKET_ERROR;
		}
	}

	if ( timeout ){
		if ( IsBadReadPtr( timeout, sizeof(struct timeval)) ){
			SetLastError(ERROR_INVALID_PARAMETER);
			return SOCKET_ERROR;
		}
	}
	// initialize call
	Select.nfds = maxfdp1;
	if ( readset ){
		Select.ValidFields |= READFDS_VALID;
		Select.readfds.fd_count = readset->fd_count;
		for ( i = 0; i < readset->fd_count; ++i ){
			Select.readfds.fd_array[i] = (HANDLE)readset->fd_array[i];
		}
	}
	if ( writeset ){
		Select.ValidFields |= WRITEFDS_VALID;
		Select.writefds.fd_count = writeset->fd_count;
		for ( i = 0; i < writeset->fd_count; ++i ){
			Select.writefds.fd_array[i] = (HANDLE)writeset->fd_array[i];
		}
	}
	if ( exceptset ){
		Select.ValidFields |= EXCEPTFDS_VALID;
		Select.exceptfds.fd_count = exceptset->fd_count;
		for ( i = 0; i < exceptset->fd_count; ++i ){
			Select.exceptfds.fd_array[i] = (HANDLE)exceptset->fd_array[i];
		}
	}
	if ( timeout ){
		Select.ValidFields |= TIMEOUT_VALID;
		Select.timeout.tv_sec = timeout->tv_sec;
		Select.timeout.tv_usec = timeout->tv_usec;
	}

	// open protocol driver
	hFile = OpenKipDriver();
	if ( hFile == INVALID_HANDLE_VALUE )
		return SOCKET_ERROR;

	fbResult =
		DeviceIoControl(
			hFile,
			IOCTL_KSOCK_SELECT,
			&Select,sizeof(Select),
			&Result, sizeof(Result),	
			&BytesReturned,
			NULL
			);

	Error = GetLastError();

	// close driver
	CloseHandle(hFile);

	if ( fbResult == FALSE ){
		SetLastError(Error);
		return SOCKET_ERROR;
	}
	if ( BytesReturned != sizeof(Result) ){
		SetLastError(ERROR_INVALID_PARAMETER);
		return SOCKET_ERROR;
	}
	if ( Result.error_code != 0 ){
		SetLastError( ErrNoToWSAERROR (Result.error_code) );
		return SOCKET_ERROR;
	}

	// handle the results
	if ( readset ){
		readset->fd_count = Result.readfds.fd_count;
		for ( i = 0; i < Result.readfds.fd_count; ++i ){
			readset->fd_array[i] = (SOCKET)Result.readfds.fd_array[i];
			fds ++;
		}
	}
	if ( writeset ){
		writeset->fd_count = Result.writefds.fd_count;
		for ( i = 0; i < Result.writefds.fd_count; ++i ){
			writeset->fd_array[i] = (SOCKET)Result.writefds.fd_array[i];
			fds ++;
		}
	}

	if ( exceptset ){
		exceptset->fd_count = Result.exceptfds.fd_count;
		for ( i = 0; i < Result.exceptfds.fd_count; ++i ){
			exceptset->fd_array[i] = (SOCKET)Result.exceptfds.fd_array[i];
			fds ++;
		}
	}

	return fds;
}