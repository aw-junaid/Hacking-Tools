//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Module: tsocket.h
//	Author: Zufyxe, September 2010
//	Description:
//	 Universal socket interface.
//

#include "kstream.h"

#define	TSOCKET		ULONG_PTR
#define	TDISTATUS	LONG

#define	SOCKET_TO_OBJECT(x)	(PFILE_OBJECT)(x | 0x80000000)
#define	OBJECT_TO_SOCKET(x)	((ULONG_PTR)x & 0x7fffffff)

extern	PTSOCKET_TABLE g_CurrentTsocketTable;
extern	TSOCKET_TABLE	g_TsocketLwipTable;
#ifdef _TDI_SUPPORT
extern	TSOCKET_TABLE	g_TsocketTdiTable;
#endif

LONG __stdcall _tsocket( int domain, int type, int protocol );
LONG __stdcall _tclosesocket( LONG Socket );
LONG __stdcall _tioctlsocket( LONG s, long cmd, ULONG* argp );
LONG __stdcall _tsendto( LONG s, char* buf, int len, int flags, struct sockaddr* to, int tolen );
LONG __stdcall _trecvfrom( LONG s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen );
LONG __stdcall _tgetsockopt( LONG s, int level, int optname, char* optval, int* optlen );
LONG __stdcall _tselect( int maxfdp1, struct fd_set *readset, struct fd_set *writeset, struct fd_set *exceptset, struct timeval *timeout );
LONG __stdcall _tconnect( LONG s, struct sockaddr* name, int namelen );
LONG __stdcall _tsetsockopt( LONG s, int level, int optname, char* optval, int optlen );
LONG __stdcall _tbind( LONG s, struct sockaddr* name, int namelen );
LONG __stdcall _tshutdown( LONG s, int how );
LONG __stdcall _tsend( LONG s, char* buf, int len, int flags );
LONG __stdcall _trecv( LONG s, char* buf, int len, int flags );
LONG __stdcall _tgetsockname( LONG s, struct sockaddr* name, int* namelen );
LONG __stdcall _tgetpeername( LONG s, struct sockaddr* name, int* namelen );
LONG __stdcall _tlisten( LONG s, int backlog );
LONG __stdcall _taccept( LONG s, struct sockaddr* addr, int* addrlen );
struct hostent* __stdcall _tgethostbyname( char *name );

//#define	_tsocket(x, y, z)			(g_CurrentTsocketTable->socket)(x, y, z)
//#define	_tclosesocket(x)			(g_CurrentTsocketTable->close)(x)
//#define	_tioctlsocket(x, y, z)		(g_CurrentTsocketTable->ioctl)(x, y, z)
//#define	_tsendto(a, b, c, d, e, f)	(g_CurrentTsocketTable->sendto)(a, b, c, d, e, f)
//#define	_trecvfrom(a, b, c, d, e, f)(g_CurrentTsocketTable->recvfrom)(a, b, c, d, e, f)
//#define	_tgetsockopt(a, b, c, d, e)	(g_CurrentTsocketTable->getsockopt)(a, b, c, d, e)
//#define	_tselect(a, b, c, d, e)		(g_CurrentTsocketTable->select)(a, b, c, d, e)
//#define	_tconnect(a, b, c)			(g_CurrentTsocketTable->connect)(a, b, c)
//#define	_tsetsockopt(a, b, c, d, e)	(g_CurrentTsocketTable->setsockopt)(a, b, c, d, e)
//#define	_tbind(a, b, c)				(g_CurrentTsocketTable->bind)(a, b, c)
//#define	_tshutdown(a, b)			(g_CurrentTsocketTable->shutdown)(a, b)
//#define	_tsend(a, b, c, d)			(g_CurrentTsocketTable->send)(a, b, c, d)
//#define	_tgetsockname(a, b, c)		(g_CurrentTsocketTable->getsockname)(a, b, c)
//#define	_tgethostbyname(a)			(g_CurrentTsocketTable->gethostbyname)(a)
#define	_tgethostbynamer(a,b,c,d,e,f)	(g_CurrentTsocketTable->gethostbynamer)(a,b,c,d,e,f)
//#define	_trecv(a, b, c, d)			(g_CurrentTsocketTable->recv)(a, b, c, d)
//#define	_tgetpeername(a, b, c)		(g_CurrentTsocketTable->getpeername)(a, b, c)
//#define	_tlisten(a, b )				(g_CurrentTsocketTable->listen)(a, b )
//#define	_taccept(a, b, c)			(g_CurrentTsocketTable->accept)(a, b, c)

void TsocketSetTdi( void );
void TsocketSetLwip( void );
BOOLEAN TsocketIsLwip( void );


typedef struct _TSOCKET_CONTEXT
{
	PFILE_OBJECT	AddressObject;
	HANDLE			AddressHandle;
	PFILE_OBJECT	ConnectionObject;
	HANDLE			ConnectionHandle;
	ULONG			Flags;
	ULONG			Type;
	ULONG			SendTimeout;	// milliseconds
	ULONG			RecvTimeout;	// milliseconds
	KEVENT			ReadComplete;
	KEVENT			WriteComplete;
	KEVENT			ReadReady;
	NTSTATUS		LastStatus;		
	PDSTREAM		Datagrams;
	PKSTREAM		Datastream;
	IO_STATUS_BLOCK IoStatus;
}TSOCKET_CONTEXT, *PTSOCKET_CONTEXT;


NTSTATUS	TsocketInit(VOID);

#define	TS_FLAG_BLOCKING	1






