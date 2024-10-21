//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP user-mode API library
//	
// module: kipapi.h
// $Revision: 88 $
// $Date: 2012-05-04 17:37:22 +0400 (Пт, 04 май 2012) $
// description:
//  Main definition file.

#ifndef __KIPAPI_H_
#define __KIPAPI_H_

#define KIP_API

//#ifdef KIPAPI_EXPORTS
//#define KIP_API __declspec(dllexport)
//#else
//#define KIP_API __declspec(dllimport)
//#endif

// Additional block because we using DEF file for C exports

#include "kipioctl.h"


#if !defined(SOCKET_ERROR)

typedef UINT_PTR        SOCKET;

#define	SOCKET_ERROR	(SOCKET)-1
#define FD_SETSIZE      64

typedef struct fd_set {
        ULONG   fd_count;               /* how many are SET? */
        SOCKET  fd_array[FD_SETSIZE];   /* an array of SOCKETs */
} fd_set;

/*
 * Structure used in select() call, taken from the BSD file sys/time.h.
 */
struct timeval {
        LONG    tv_sec;         /* seconds */
        LONG    tv_usec;        /* and microseconds */
};

/*
 * Commands for ioctlsocket(),  taken from the BSD file fcntl.h.
 *
 *
 * Ioctl's have the command encoded in the lower word,
 * and the size of any in or out parameters in the upper
 * word.  The high 2 bits of the upper word are used
 * to encode the in/out status of the parameter; for now
 * we restrict parameters to at most 128 bytes.
 */
#define IOCPARM_MASK    0x7f            /* parameters must be < 128 bytes */
#define IOC_VOID        0x20000000      /* no parameters */
#define IOC_OUT         0x40000000      /* copy out parameters */
#define IOC_IN          0x80000000      /* copy in parameters */
#define IOC_INOUT       (IOC_IN|IOC_OUT)
                                        /* 0x20000000 distinguishes new &
                                           old ioctl's */
#define _IO(x,y)        (IOC_VOID|((x)<<8)|(y))

#define _IOR(x,y,t)     (IOC_OUT|(((LONG)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define _IOW(x,y,t)     (IOC_IN|(((LONG)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define FIONREAD    _IOR('f', 127, ULONG) /* get # bytes to read */
#define FIONBIO     _IOW('f', 126, ULONG) /* set/clear non-blocking i/o */
#define FIOASYNC    _IOW('f', 125, ULONG) /* set/clear async i/o */

#define AF_INET         2               /* internetwork: UDP, TCP, etc. */

#define SOCK_STREAM     1               /* stream socket */
#define SOCK_DGRAM      2               /* datagram socket */

#define IPPROTO_TCP     6				/* tcp */
#define IPPROTO_UDP     17			    /* user datagram protocol */

#define SOMAXCONN       0x7fffffff

/*
 * WinSock 2 extension -- manifest constants for shutdown()
 */
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define SOL_SOCKET      0xffff          /* options for socket level */

/*
 * Additional options.
 */
#define SO_SNDBUF       0x1001          /* send buffer size */
#define SO_RCVBUF       0x1002          /* receive buffer size */
#define SO_SNDLOWAT     0x1003          /* send low-water mark */
#define SO_RCVLOWAT     0x1004          /* receive low-water mark */
#define SO_SNDTIMEO     0x1005          /* send timeout */
#define SO_RCVTIMEO     0x1006          /* receive timeout */
#define SO_ERROR        0x1007          /* get error status and clear */
#define SO_TYPE         0x1008          /* get socket type */


/*
 * Socket address, internet style.
 */

#ifndef s_addr
/*
 * Internet address (old style... should be updated)
 */
struct in_addr {
        union {
                struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { USHORT s_w1,s_w2; } S_un_w;
                ULONG S_addr;
        } S_un;
#define s_addr  S_un.S_addr
                                /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2
                                /* host on imp */
#define s_net   S_un.S_un_b.s_b1
                                /* network */
#define s_imp   S_un.S_un_w.s_w2
                                /* imp */
#define s_impno S_un.S_un_b.s_b4
                                /* imp # */
#define s_lh    S_un.S_un_b.s_b3
                                /* logical host */
};
#endif


struct sockaddr_in {
        SHORT	sin_family;
        USHORT	sin_port;
        struct  in_addr sin_addr;
        CHAR    sin_zero[8];
};

struct  hostent {
        CHAR    FAR * h_name;           /* official name of host */
        CHAR    FAR * FAR * h_aliases;  /* alias list */
        SHORT   h_addrtype;             /* host address type */
        SHORT   h_length;               /* length of address */
        CHAR    FAR * FAR * h_addr_list; /* list of addresses */
#define h_addr  h_addr_list[0]          /* address, for backward compat */
};

/*
 * Structure used by kernel to store most
 * addresses.
 */
struct sockaddr {
        USHORT	sa_family;              /* address family */
        CHAR    sa_data[14];            /* up to 14 bytes of direct address */
};

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr *PSOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr_in *PSOCKADDR_IN;

#define htons(A) ((((USHORT)(A) & 0xff00) >> 8) | (((USHORT)(A) & 0x00ff) << 8))
#define htonl(A) ((((ULONG)(A) & 0xff000000) >> 24) | (((ULONG)(A) & 0x00ff0000) >> 8) | (((ULONG)(A) & 0x0000ff00) << 8) | (((ULONG)(A) & 0x000000ff) << 24))
#endif	// SOCKET_ERROR

enum
{
	FILE_NAME_SIZE = 1000
};

#define SYS_FILE _T(".SYS")

#ifdef __cplusplus
extern "C" {
#endif
	KIP_API int __stdcall _tsocket( int domain, int type, int protocol );
	KIP_API int __stdcall _tclosesocket( SOCKET Socket );
	KIP_API int __stdcall _tbind( SOCKET s, const struct sockaddr* name, int namelen );
	KIP_API int __stdcall _tconnect( SOCKET s, const struct sockaddr* name, int namelen );
	KIP_API int __stdcall _tsend( SOCKET s,  const char* buf, int len, int flags );
	KIP_API int __stdcall _trecv( SOCKET s,  const char* buf, int len, int flags );

	KIP_API int __stdcall _tsendto( SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen );
	KIP_API int __stdcall _trecvfrom( SOCKET s,  const char* buf, int len, int flags,struct sockaddr* from, int* fromlen );
	KIP_API int __stdcall _tshutdown( SOCKET s, int how );

	KIP_API int __stdcall _tioctlsocket( SOCKET s, long cmd, ULONG* argp );
	KIP_API int __stdcall _tgetsockopt( SOCKET s, int level, int optname, char* optval, int* optlen );
	KIP_API int __stdcall _tsetsockopt( SOCKET s, int level, int optname, const char* optval, int optlen );

	KIP_API int __stdcall _tgetsockname( SOCKET s, struct sockaddr* name, int* namelen );
	KIP_API int __stdcall _tgetpeername( SOCKET s, struct sockaddr* name, int* namelen );
	KIP_API struct hostent* __stdcall _tgethostbyname( const char *name );

	KIP_API SOCKET __stdcall _taccept( SOCKET s, struct sockaddr* addr, int* addrlen );
	KIP_API int __stdcall _tlisten( SOCKET s, int backlog );

	KIP_API int __stdcall _tselect( int maxfdp1, struct fd_set *readset, struct fd_set *writeset, 
		struct fd_set *exceptset, const struct timeval *timeout );

	KIP_API struct hostent* __stdcall _tgethostbyname( const char *name );
#ifdef __cplusplus
}
#endif

#endif //#ifndef __NTTDIAPI_H_