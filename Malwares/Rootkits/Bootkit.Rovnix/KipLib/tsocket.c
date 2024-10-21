//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Module: tsocket.c
//	Author: Zufyxe, September 2010
//	Description:
//	 Universal socket interface.
//

#include "project.h"
#include "tdisup.h"
#include "tsocket.h"
#include "..\bkdrv\handle.h"

#include "lwip\sockets.h"
#include "lwip\netdb.h"

#ifdef _TDI_SUPPORT

typedef struct	_TDGRAM
{
	TA_IP_ADDRESS	IpAddress;
	CHAR			Data[0];
} TDGRAM, *PTDGRAM;


HANDLE		TdiTSocket(LONG Domain, LONG Type, LONG Protocol);
TDISTATUS	TdiTClose(HANDLE	Socket);
TDISTATUS	TdiTIoctl(HANDLE	Socket, LONG Cmd, PVOID Argp);
TDISTATUS	TdiTSendto(HANDLE Socket, PVOID Data, LONG Size, ULONG Flags, struct sockaddr * To, ULONG Tolen);
TDISTATUS	TdiTRecvFrom(HANDLE Socket, PVOID Data, ULONG Size, ULONG Flags,  struct sockaddr * From, PULONG Fromlen);
TDISTATUS	TdiTGetSockOpt(HANDLE Socket, LONG Level, LONG Optname, PVOID Optval, PULONG Optlen);
TDISTATUS	TdiTSelect(ULONG Maxfdp1, PVOID Readset, PVOID Writeset, PVOID Exceptset, PVOID Timeout);
TDISTATUS	TdiTConnect(HANDLE Socket, struct sockaddr_in* Name, ULONG Namelen);
TDISTATUS	TdiTSetSockOpt(HANDLE Socket, LONG Level, LONG Optname, PVOID Optval, ULONG Optlen);
TDISTATUS	TdiTBind(HANDLE Socket, struct sockaddr *Name, ULONG Namelen);
TDISTATUS	TdiTShutdown(HANDLE Socket, LONG How);
TDISTATUS	TdiTSend(HANDLE Socket, PVOID Data, LONG Size, ULONG Flags);
TDISTATUS	TdiTGetSockName(HANDLE Socket, struct sockaddr *Name, PULONG	Namelen);
TDISTATUS	TdiTRecv(HANDLE Socket, PVOID Mem, LONG Len, ULONG Flags);
TDISTATUS	TdiTGetPeerName(HANDLE Socket, struct sockaddr *Name, PULONG	Namelen);
struct hostent* TdiTGetHostByName(const char *name);
int TdiTGetHostByNameR(const char *name, struct hostent *ret, char *buf,
	size_t buflen, struct hostent **result, int *h_errnop);

TDISTATUS	TdiTListen(HANDLE Socket, int backlog);
TDISTATUS	TdiTAccept(HANDLE Socket, struct sockaddr *Name, PULONG	Namelen);

#define	FD_READ		1
#define	FD_WRITE	2
#define	FD_ERROR	3



PHANDLE_TABLE	g_Handles = NULL;

#endif

TSOCKET_TABLE	g_TsocketLwipTable =
{
	(FUNC_SOCKET)&lwip_socket,
	(FUNC_CLOSE)&lwip_close,
	(FUNC_IOCTL)&lwip_ioctl,
	(FUNC_SENDTO)&lwip_sendto,
	(FUNC_RECVFROM)&lwip_recvfrom,
	(FUNC_GETSOCKOPT)&lwip_getsockopt,
	(FUNC_SELECT)&lwip_select,
	(FUNC_CONNECT)&lwip_connect,
	(FUNC_SETSOCKOPT)&lwip_setsockopt,
	(FUNC_BIND)&lwip_bind,
	(FUNC_SHUTDOWN)&lwip_shutdown,
	(FUNC_SEND)&lwip_send,
	(FUNC_GETSOCKNAME)&lwip_getsockname,
	(FUNC_RECV)&lwip_recv,
	(FUNC_GETPEERNAME)&lwip_getpeername,

	(FUNC_GETHOSTBYNAME)&lwip_gethostbyname,
	(FUNC_GETHOSTBYNAMER)&lwip_gethostbyname_r,

	(FUNC_LISTEN)&lwip_listen,
	(FUNC_ACCEPT)&lwip_accept
};

#ifdef _TDI_SUPPORT

TSOCKET_TABLE	g_TsocketTdiTable =
{
	(FUNC_SOCKET)&TdiTSocket,
	(FUNC_CLOSE)&TdiTClose,
	(FUNC_IOCTL)&TdiTIoctl,
	(FUNC_SENDTO)&TdiTSendto,
	(FUNC_RECVFROM)&TdiTRecvFrom,
	(FUNC_GETSOCKOPT)&TdiTGetSockOpt,
	(FUNC_SELECT)&TdiTSelect,
	(FUNC_CONNECT)&TdiTConnect,
	(FUNC_SETSOCKOPT)&TdiTSetSockOpt,
	(FUNC_BIND)&TdiTBind,
	(FUNC_SHUTDOWN)&TdiTShutdown,
	(FUNC_SEND)&TdiTSend,
	(FUNC_GETSOCKNAME)&TdiTGetSockName,
	(FUNC_RECV)&TdiTRecv,
	(FUNC_GETPEERNAME)&TdiTGetPeerName,
	(FUNC_GETHOSTBYNAME)&TdiTGetHostByName,
	(FUNC_GETHOSTBYNAMER)&TdiTGetHostByNameR,
	(FUNC_LISTEN)&TdiTListen,
	(FUNC_ACCEPT)&TdiTAccept
};

#endif //#ifdef _TDI_SUPPORT

PTSOCKET_TABLE g_CurrentTsocketTable = &g_TsocketLwipTable;

#ifdef _TDI_SUPPORT

// ---- routines switches socket tables -----------------------------------------------------------------------------------------
void TsocketSetTdi( void )
{
	g_CurrentTsocketTable = &g_TsocketTdiTable;
}

void TsocketSetLwip( void )
{
	g_CurrentTsocketTable = &g_TsocketLwipTable;
}

BOOLEAN TsocketIsLwip( void )
{
	return ( g_CurrentTsocketTable == &g_TsocketLwipTable );
}

// ---- Context manipulation routines -----------------------------------------------------------------------------------------

_inline	PTSOCKET_CONTEXT CreateContext(HANDLE Handle)
{
	PTSOCKET_CONTEXT Ctx;
	ASSERT(g_Handles);

	if (!HandleCreate(g_Handles, Handle, &Ctx))
		Ctx = NULL;

	return(Ctx);
}


_inline BOOLEAN DeleteContext(HANDLE Handle)
{
	return(HandleClose(g_Handles, Handle, NULL));
}


_inline PTSOCKET_CONTEXT GetContext(HANDLE Handle)
{
	PTSOCKET_CONTEXT Ctx;
	if (!HandleOpen(g_Handles, Handle, &Ctx))
		Ctx = NULL;

	return(Ctx);
}

_inline	BOOLEAN ReleaseContext(PTSOCKET_CONTEXT Ctx)
{
	PHANDLE_RECORD	HRec = CONTAINING_RECORD(Ctx, HANDLE_RECORD, Context);
	return(HandleClose(NULL, 0, HRec));
}

// ---- Help routines ---------------------------------------------------------------------------------------------------------

_inline	VOID TdiCopyAddress(struct sockaddr* Dest, PTDI_IP_ADDRESS Src)
{
	PTDI_ADDRESS_IP	DestIp = (PTDI_ADDRESS_IP)&Dest->sa_data;
	Dest->sa_len = (UCHAR)Src->AddressLength;
	Dest->sa_family = (UCHAR)Src->AddressType;
	DestIp->in_addr = Src->Address.in_addr;
	DestIp->sin_port = Src->Address.sin_port;
}

_inline PIO_STATUS_BLOCK TdiInitAsyncStatus(PIO_STATUS_BLOCK pIoStatus, PKEVENT	pEvent)
{
	KeResetEvent(pEvent);
	pIoStatus->Status = STATUS_PENDING;
	pIoStatus->Information = (ULONG_PTR)pEvent;
	return(pIoStatus);
}

// ---- Event handlers ---------------------------------------------------------------------------------------------------------


NTSTATUS
TsocketEventError(
		HANDLE		Socket, 
		NTSTATUS	ntStatus
		)
{
    PTSOCKET_CONTEXT	Context;
	if (Context = GetContext(Socket))
	{
		Context->IoStatus.Status = ntStatus;
		ReleaseContext(Context);
	}
    return(TDI_SUCCESS);
}


NTSTATUS
TsocketEventReceive(
			HANDLE  Socket, 
			CONNECTION_CONTEXT  ConnectionContext,  
			ULONG	Flags, 
			ULONG	Indicated, 
			ULONG	Available, 
			PULONG	Taken, 
			PCHAR	Data, 
			PVOID*	EventRcvBuffer 
		)
{
	
	NTSTATUS	ntStatus = TDI_NOT_ACCEPTED;
	PTSOCKET_CONTEXT	Context;

	if (Context = GetContext(Socket))
	{
		if (KStreamWrite(Context->Datastream, Data, Indicated))
		{
			KeSetEvent(&Context->ReadReady, 0, FALSE);
			ntStatus = TDI_SUCCESS;
			ASSERT(Indicated == Available);

			*Taken = Indicated;
			*EventRcvBuffer = NULL;
		}

		ReleaseContext(Context);
	}
	else
		ASSERT(FALSE);
    return(ntStatus);
}



NTSTATUS 
TsocketEventReceiveDatagram(
				HANDLE		Socket,
				ULONG		AddressLength,  
				TRANSPORT_ADDRESS  *Address, 
				ULONG		OptionsLength, 
				PVOID		Options,  
				ULONG		Flags, 
				ULONG		Indicated, 
				ULONG		Available,  
				PULONG		Taken, 
				PUCHAR		Data, 
				PVOID*		Buffer    
				) 
{
	PTSOCKET_CONTEXT	Context;
	NTSTATUS	ntStatus = TDI_NOT_ACCEPTED;


	if (Context = GetContext(Socket))
	{
		PLBUFFER	lBuffer = LBufferAllocate(Indicated);
		if (lBuffer)
		{
			TdiCopyAddress((struct sockaddr*)&lBuffer->Sign, (PTDI_IP_ADDRESS)&Address->Address[0]);
			memcpy(&lBuffer->Data, Data, Indicated);
			DStreamPushBuffer(Context->Datagrams, lBuffer);
			ntStatus = TDI_SUCCESS;
			*Taken = Indicated; 
			*Buffer = NULL;
		}
		ReleaseContext(Context);
	}

    return(ntStatus);
}


// ---- API -----------------------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Socket cleanup callback routine.
//
static BOOLEAN _stdcall SocketCleanup(HANDLE Socket, PTSOCKET_CONTEXT Context)
{
	if (Context->ConnectionObject)
		ObDereferenceObject(Context->ConnectionObject);
	if (Context->ConnectionHandle)
		ZwClose(Context->ConnectionHandle);
	if (Context->AddressObject)
	{
		TdiSetEventHandler(Context->AddressObject, TDI_EVENT_ERROR, NULL, NULL);
		TdiSetEventHandler(Context->AddressObject, TDI_EVENT_RECEIVE, NULL, NULL);
		TdiSetEventHandler(Context->AddressObject, TDI_EVENT_RECEIVE_DATAGRAM, NULL, NULL);

		ObDereferenceObject(Context->AddressObject);
	}
	if (Context->AddressHandle)
		ZwClose(Context->AddressHandle);

	if (Context->Datagrams)
		DStreamRelease(Context->Datagrams);
	if (Context->Datastream)
		KStreamRelease(Context->Datastream);

	return(TRUE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Creates and initializes new socket.
//
HANDLE	TdiTSocket(LONG Domain, LONG Type, LONG Protocol)
{
	PFILE_OBJECT	Address;
	HANDLE		hAddress;
	NTSTATUS	ntStatus;
	PTSOCKET_CONTEXT	Context = NULL;
	HANDLE		Socket = (HANDLE)INVALID_SOCKET;
	BOOLEAN	Ret = FALSE;

	do	// not a loop
	{
		if (!(Socket = HandleCreate(g_Handles, 0, &Context)))
			break;

		Context->Type = Type;
		Context->Flags = TS_FLAG_BLOCKING;

		KeInitializeEvent(&Context->ReadComplete, NotificationEvent, FALSE);
		KeInitializeEvent(&Context->WriteComplete, NotificationEvent, FALSE);
		KeInitializeEvent(&Context->ReadReady, SynchronizationEvent, FALSE);
		
		ntStatus = TdiCreateAddress(&Context->AddressHandle, &Context->AddressObject, Type, 0, 0);
		if (!NT_SUCCESS(ntStatus))
			break;
		
		TdiSetEventHandler(Context->AddressObject, TDI_EVENT_ERROR, TsocketEventError, Socket);
			
		
		if (Type == SOCK_STREAM)
		{
			// Creating stream type socket
			ASSERT(Type == SOCK_STREAM);
			ntStatus = TdiCreateConnection(&Context->ConnectionHandle, &Context->ConnectionObject);
			if (!NT_SUCCESS(ntStatus))
				break;

			ntStatus = TdiBind(Context->ConnectionObject, Context->AddressHandle);
			if (!NT_SUCCESS(ntStatus))
				break;

			if (!(Context->Datastream = KStreamAllocate()))
				break;

			TdiSetEventHandler(Context->AddressObject, TDI_EVENT_RECEIVE, TsocketEventReceive, Socket);

		}	// if (Type == SOCK_STREAM)
		else
		{
			ASSERT(Type == SOCK_DGRAM);
			if (!(Context->Datagrams = DStreamAllocate()))
				break;

			TdiSetEventHandler(Context->AddressObject, TDI_EVENT_RECEIVE_DATAGRAM, TsocketEventReceiveDatagram, Socket);
		}

       	Context->IoStatus.Status = ntStatus;
		return(Socket);

		ASSERT(FALSE);
	} while(FALSE);

	if (Socket)
	{
		ASSERT(Context);
		ReleaseContext(Context);
	}

	return((HANDLE)INVALID_SOCKET);

	UNREFERENCED_PARAMETER(Domain);
	UNREFERENCED_PARAMETER(Protocol);
}


TDISTATUS	TdiTShutdown(HANDLE Socket, LONG How)
{
	TDISTATUS	Status = INVALID_SOCKET;
	PTSOCKET_CONTEXT	Context;

	if (Context = GetContext(Socket))
	{
		if (Context->ConnectionObject)
			TdiDisconnect(Context->ConnectionObject);
		if (Context->Datastream)
			KStreamClear(Context->Datastream);
		if (Context->Datagrams)
			DStreamClear(Context->Datagrams);

		KeResetEvent(&Context->ReadComplete);
		KeResetEvent(&Context->ReadReady);
		KeResetEvent(&Context->WriteComplete);

		Status = STATUS_SUCCESS;
		Context->IoStatus.Status = Status;
		
		ReleaseContext(Context);
	}	// if (Context = GetContext(Socket))
	return(Status);
}


TDISTATUS	TdiTClose(HANDLE	Socket)
{
	TdiTShutdown(Socket, 0);
	DeleteContext(Socket);
	return(0);
}


TDISTATUS	TdiTIoctl(HANDLE Socket, LONG Cmd, PULONG Argp)
{
	PTSOCKET_CONTEXT	Context;
	
	if (Context = GetContext(Socket))
	{
		switch(Cmd)
		{
		case FIONBIO:
			if (*Argp)
				Context->Flags &= (~TS_FLAG_BLOCKING);
			else
				Context->Flags |= TS_FLAG_BLOCKING;
			break;
		case FIONREAD:
			if (Context->Datastream)
				*Argp = KStreamGetLength(Context->Datastream);
			else
				ASSERT(FALSE);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		ReleaseContext(Context);
	}
	UNREFERENCED_PARAMETER(Socket);
	return(0);
}


TDISTATUS	TdiTSendto(HANDLE Socket, PVOID Data, LONG Size, ULONG Flags, struct sockaddr *To, ULONG Tolen)
{
	TDISTATUS	Status = INVALID_SOCKET;
	NTSTATUS	ntStatus;
	PTSOCKET_CONTEXT	Context;
	struct sockaddr_in *Address = (struct sockaddr_in *)To;

	if (Context = GetContext(Socket))
	{	
		PKEVENT	pEvent = NULL;

		if (!(Context->Flags & TS_FLAG_BLOCKING))
		{
			pEvent = &Context->WriteComplete;
			KeResetEvent(pEvent);
		}

		ntStatus = TdiSendto(Context->AddressObject, Data, Size, *(PULONG)&Address->sin_addr, Address->sin_port, pEvent);
		if (NT_SUCCESS(ntStatus))
			Status = Size;

		ReleaseContext(Context);
	}

	return(Status);
}


ULONG StreamGetDatagram(PDSTREAM pStream, PVOID	Data, ULONG	Size,  struct sockaddr* From, PULONG Fromlen)
{
	ULONG		dSize = 0;
	PLBUFFER	lBuffer, lFound = NULL;
	PLIST_ENTRY	pEntry;

	DStreamLock(pStream);
	pEntry = pStream->BufferListHead.Flink;
	while (pEntry != &pStream->BufferListHead)
	{
		lBuffer = CONTAINING_RECORD(pEntry, LBUFFER, Entry);
		pEntry = pEntry->Flink;

		if (lBuffer->Size <= Size)
		{
			if (From && Fromlen)
			{				
				struct sockaddr* RecvdAddr = (struct sockaddr*)&lBuffer->Sign;
				PTDI_ADDRESS_IP UdpAddr = (PTDI_ADDRESS_IP)&RecvdAddr->sa_data;
				PTDI_ADDRESS_IP	FromAddr = (PTDI_ADDRESS_IP)&From->sa_data;

				if (From->sa_len && From->sa_family == AF_INET)
				{
					if ((FromAddr->in_addr != 0) && (FromAddr->in_addr != UdpAddr->in_addr))
						continue;
					if ((FromAddr->sin_port != 0) && (FromAddr->sin_port != UdpAddr->sin_port))
						continue;
				}

				memcpy(From, RecvdAddr, sizeof(struct sockaddr));
				*Fromlen = sizeof(TDI_IP_ADDRESS);
			}	// if (From && Fromlen)

			
			lFound = lBuffer;
			RemoveEntryList(&lFound->Entry);
			pStream->Count -= 1;
			break;
		}	// if (lBuffer->Size <= Size)
	}	// while (pEntry != &pStream->BufferListHead)
	DStreamUnlock(pStream);

	if (lFound)
	{
		memcpy((PCHAR)Data, (PCHAR)&lFound->Data, lFound->Size);
		dSize = lFound->Size;
		LBufferFree(lFound, NULL);
	}

	return(dSize);
}



TDISTATUS	TdiTRecvFrom(HANDLE Socket, PVOID Data, ULONG Size, ULONG Flags,  struct sockaddr* From, PULONG Fromlen)
{
	TDISTATUS	Status = INVALID_SOCKET;
	PTSOCKET_CONTEXT	Context;
	NTSTATUS	ntStatus;
	struct sockaddr_in *Address = (struct sockaddr_in *)From;
	
	if (Context = GetContext(Socket))
	{
		PLBUFFER	lBuffer;	
		PTDGRAM		Dgram;
	
		// Trying to empty Datagrams stream first
		if ((Context->Datagrams) && !(Status = StreamGetDatagram(Context->Datagrams, Data, Size, From, Fromlen)))
		{
			ULONG	bRead = 0;

			if (Context->Flags & TS_FLAG_BLOCKING)
				ntStatus = TdiRecvfrom(Context->AddressObject, Data, Size, *(PULONG)&Address->sin_addr, Address->sin_port, &bRead, NULL);
			else
				ntStatus = STATUS_PENDING;

			Context->IoStatus.Status = ntStatus;

			if (ntStatus == STATUS_SUCCESS)
				Status = bRead;
			else
				Status = INVALID_SOCKET;
	
		}	// if ((Context->Datagrams) && !(Status = StreamGetDatagram(Context->Datagrams, Data, Size, From, Fromlen)))

		ReleaseContext(Context);
	}	// if (Context = GetContext(Socket))

	return(Status);


}



TDISTATUS	TdiTGetSockOpt(HANDLE Socket, LONG Level, LONG Optname, PVOID Optval, PULONG Optlen)
{
	TDISTATUS	Status = INVALID_SOCKET;
	PTSOCKET_CONTEXT	Context;

	if (Context = GetContext(Socket))
	{
		if (Optval && Optlen)
		{
			Status = STATUS_SUCCESS;
			switch(Optname)
			{
			case SO_ERROR:
				ASSERT(*Optlen == sizeof(ULONG));
				switch(Context->IoStatus.Status)
				{
					case STATUS_PENDING:
						*(PULONG)Optval = EWOULDBLOCK;
						break;
					case STATUS_SUCCESS:
						*(PULONG)Optval = STATUS_SUCCESS;
						break;
					case STATUS_IO_TIMEOUT:
					case STATUS_TIMEOUT:
						*(PULONG)Optval = ETIMEDOUT;
						break;
					default:
						Status = INVALID_SOCKET;
						break;
				}	// switch(Context->IoStatus.Status)
				Context->IoStatus.Status = 0;
				break;
			default:
				Status = INVALID_SOCKET;
				break;
			}	// switch(Optname)
		}	// if (Optval && Optlen)
		ReleaseContext(Context);
	}	// if (Context = GetContext(Socket))
	return(Status);
}


TDISTATUS	TdiTSetSockOpt(HANDLE Socket, LONG Level, LONG Optname, PVOID Optval, ULONG Optlen)
{
	TDISTATUS	Status = INVALID_SOCKET;
	PTSOCKET_CONTEXT	Context;

	if (Context = GetContext(Socket))
	{
		if (Optval && Optlen)
		{
			Status = STATUS_SUCCESS;
			switch(Optname)
			{
			case SO_RCVTIMEO:
				ASSERT(Optlen == sizeof(ULONG));
				Context->RecvTimeout = *(PULONG)Optval;
				Status = STATUS_SUCCESS;
				break;
			case SO_SNDTIMEO:
				ASSERT(Optlen == sizeof(ULONG));
				Context->SendTimeout = *(PULONG)Optval;
				Status = STATUS_SUCCESS;
				break;
			default:
				ASSERT(FALSE);
				Status = INVALID_SOCKET;
			break;
			}
		}	// SO_SNDTIMEO
		ReleaseContext(Context);
	}	// if (Context = GetContext(Socket))
	return(Status);
}



ULONG	FdTest(fd_set* Fd, ULONG Flags, PLARGE_INTEGER	DstTime)
{
	ULONG	i = 0, Count = 0;
	struct fd_set NewSet = {0};


	if (Fd)
	{
		do
		{
			if (!Fd->fd_bits[i >> 3])
			{
				i += sizeof(UCHAR);
				continue;
			}

			if (FD_ISSET(i, Fd))
			{
				PTSOCKET_CONTEXT	Context;
				HANDLE	Socket = (HANDLE)i;
				BOOLEAN	Ready = FALSE;

				if (Context = GetContext(Socket))
				{
					switch(Flags)
					{
					case FD_READ:
						if ((Context->Datagrams) && DStreamGetLength(Context->Datagrams))
							Ready = TRUE;
						if (Context->Datastream)
						{
							NTSTATUS ntStatus = KeWaitForSingleObject(&Context->Datastream->DataReadyEvent, Executive, KernelMode, FALSE, DstTime);
							if (ntStatus == STATUS_SUCCESS && KStreamGetLength(Context->Datastream))
								Ready = TRUE;
						}
						break;
					case FD_WRITE:
						if (KeWaitForSingleObject(&Context->WriteComplete, Executive, KernelMode, FALSE, DstTime) == STATUS_SUCCESS)
							Ready = TRUE;
						break;
					case FD_ERROR:
						if (!NT_SUCCESS(Context->IoStatus.Status))
							Ready = TRUE;
						break;
					default:
						ASSERT(FALSE);
						break;
					}	// switch(Flags)
					ReleaseContext(Context);

					if (Ready)
					{
						FD_SET(i, &NewSet);
						Count += 1;
					}
				}	// if (Context = GetContext(Socket))
			}	// if (FD_ISSET(i, Fd))

			i += 1;
		}	while(i<FD_SETSIZE);

		memcpy(Fd, &NewSet, sizeof(struct fd_set));
	}	// if (Fd)
	return(Count);
}



TDISTATUS	TdiTSelect(ULONG Maxfdp1, fd_set* Readset, fd_set* Writeset, fd_set* Exceptset, struct timeval *Timeout)
{
	ULONG	Count = 0;
	LARGE_INTEGER	DstTime;

	KeQuerySystemTime(&DstTime);	
	DstTime.QuadPart += (SECONDS(Timeout->tv_sec) + MILLISECONDS(Timeout->tv_usec));

	Count += FdTest(Readset, FD_READ, &DstTime);
	Count += FdTest(Writeset, FD_WRITE, &DstTime);
	Count += FdTest(Exceptset, FD_ERROR, &DstTime);

	return(Count);
}



TDISTATUS	TdiTConnect(HANDLE Socket, struct sockaddr_in* Name, ULONG Namelen)
{
	TDISTATUS	Status = INVALID_SOCKET;
	PTSOCKET_CONTEXT	Context;

	if (Context = GetContext(Socket))
	{
		PKEVENT		pEvent = NULL;
		NTSTATUS	ntStatus;
		PIO_STATUS_BLOCK	pAsyncStatus = NULL;	

		if (!(Context->Flags & TS_FLAG_BLOCKING))
			pAsyncStatus = TdiInitAsyncStatus(&Context->IoStatus, &Context->WriteComplete);
		

		ntStatus = TdiConnect(Context->ConnectionObject, *(PULONG)&Name->sin_addr, Name->sin_port, pAsyncStatus);

		if (ntStatus == STATUS_SUCCESS)
			Status = STATUS_SUCCESS;
		
		ReleaseContext(Context);
	}
	return(Status);
}




TDISTATUS	TdiTBind(HANDLE Socket, struct sockaddr *Name, ULONG Namelen)
{
	TDISTATUS	Status = INVALID_SOCKET;
	NTSTATUS	ntStatus;
	PTSOCKET_CONTEXT	Context;
	PTDI_ADDRESS_IP		IpAddr = (PTDI_ADDRESS_IP)&Name->sa_data;

	if (Context = GetContext(Socket))
	{
		Status = STATUS_SUCCESS;
		if (Context->AddressObject)
		{
			ASSERT(Context->AddressHandle);
			ObDereferenceObject(Context->AddressObject);
			ZwClose(Context->AddressHandle);

			Context->AddressHandle = 0;
			Context->AddressObject = NULL;
		}	// if (Context->AddressObject)
	
		ASSERT(Name->sa_len != 0 && Name->sa_family == AF_INET);
		ntStatus = TdiCreateAddress(&Context->AddressHandle, &Context->AddressObject, Context->Type, IpAddr->in_addr, IpAddr->sin_port);
		if (NT_SUCCESS(ntStatus))
			ntStatus = TdiBind(Context->ConnectionObject, Context->AddressHandle);

		if (NT_SUCCESS(ntStatus))
			Status = STATUS_SUCCESS;
			
		ReleaseContext(Context);
	}	// if (Context = GetContext(Socket))
	return(Status);
}



TDISTATUS	TdiTSend(HANDLE Socket, PVOID Data, LONG Size, ULONG Flags)
{
	TDISTATUS	Status = INVALID_SOCKET;
	NTSTATUS	ntStatus;
	PTSOCKET_CONTEXT	Context;

	if (Context = GetContext(Socket))
	{
		ntStatus = TdiSend(Context->ConnectionObject, Data, Size);
		if (NT_SUCCESS(ntStatus))
		{
			Context->IoStatus.Status = STATUS_SUCCESS;
			Status = Size;
		}
	
		ReleaseContext(Context);
	}
	return(Status);

}


TDISTATUS	TdiTGetSockName(HANDLE Socket, struct sockaddr *Name, PULONG	Namelen)
{
	CHAR	InfoBuffer[0x80] = {0};
	ULONG	bLen = 0x80;
	TDISTATUS	Status = INVALID_SOCKET;
	PTSOCKET_CONTEXT	Context;
	PTDI_ADDRESS_INFO	AddressInfo = (PTDI_ADDRESS_INFO)&InfoBuffer;	

	ASSERT(Name != NULL && Namelen != NULL);
	ASSERT(*Namelen >= sizeof(struct sockaddr));

	if (Context = GetContext(Socket))
	{
		if (NT_SUCCESS(TdiQueryAddress(Context->AddressObject, AddressInfo, &bLen)))
		{
			TdiCopyAddress(Name, (PTDI_IP_ADDRESS)&AddressInfo->Address.Address);
			*Namelen = sizeof(struct sockaddr);
			Status = STATUS_SUCCESS;
		}
		ReleaseContext(Context);
	}
	return(Status);
}

/*
///////////////////////////////////////////////////////////////////////////////
//  getpeername
//      Get socket peer address
///////////////////////////////////////////////////////////////////////////////
*/
#if 0
LONG getpeername(
				 IN PTDI_SOCKET Socket,
				 IN PTDI_ADDRESS_IP LocalTdiAddressIp,
				 OUT PTDI_ADDRESS_IP RemoteTdiAddressIp
				 )
{
	LONG RetCode;
	NTSTATUS Status;
	ULONG Iter;
	ULONG Count;
	PFILE_OBJECT FileObject;
	TCP_REQUEST_QUERY_INFORMATION_EX Request;
	PTDI_TCPROW Row;
	char Buffer[1024];

	RetCode = STATUS_UNSUCCESSFUL;
	__try
	{
		if ( LocalTdiAddressIp == NULL || RemoteTdiAddressIp == NULL )
			__leave;

		if ( Socket->Type != SOCKET_TYPE_TCP )
			__leave;

		FileObject = Socket->ConnectionFileObject;
		if ( FileObject == NULL )
			__leave;

		if ( ( ULONG )FileObject->FsContext2 != TDI_CONNECTION_FILE )
			__leave;

		Request.ID.Entity.Entity = CO_TL_ENTITY;
		Request.ID.Entity.Instance = 0;
		Request.ID.Class = INFO_CLASS_PROTOCOL;
		Request.ID.Type = INFO_TYPE_PROVIDER;
		Request.ID.ID = IP_MIB_ADDRTABLE_ENTRY_ID;
		RtlZeroMemory( Request.Context, sizeof( Request.Context ) );
		Status = TdiDeviceIoControl( Socket,
			IOCTL_TCP_QUERY_INFORMATION_EX,
			&Request,
			sizeof( TCP_REQUEST_QUERY_INFORMATION_EX ),
			Buffer,
			sizeof( Buffer )
			);
		if ( Status != STATUS_SUCCESS )
			__leave;

		Row = ( PTDI_TCPROW )Buffer;
		for ( Iter = 0, Count = Socket->IoStatus.Information / sizeof( TDI_TCPROW ); Iter < Count; Iter++, Row++ )
		{
			if ( LocalTdiAddressIp->sin_port == Row->LocalPort )
			{
				RemoteTdiAddressIp->in_addr = Row->RemoteAddress;
				RemoteTdiAddressIp->sin_port = ( USHORT )Row->RemotePort;
				RetCode = STATUS_SUCCESS;
				break;
			}
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
	}

	return RetCode;
}
#endif

TDISTATUS	TdiTGetPeerName(HANDLE Socket, struct sockaddr *Name, PULONG	Namelen)
{
	//TDOD:
	return STATUS_NOT_IMPLEMENTED;
}

struct hostent*
	TdiTGetHostByName(const char *name)
{
	//TDOD:
	return NULL;
}

int
	TdiTGetHostByNameR(const char *name, struct hostent *ret, char *buf,
		size_t buflen, struct hostent **result, int *h_errnop)
{
	//TDOD:
	return STATUS_NOT_IMPLEMENTED;
}

TDISTATUS	TdiTRecv(HANDLE Socket, PVOID Mem, LONG Len, ULONG Flags)
{
	TDISTATUS	Status = INVALID_SOCKET;
	PTSOCKET_CONTEXT	Context;

	if (Context = GetContext(Socket))
	{
		if (!(Status = KStreamRead(Context->Datastream, Mem, Len)))
		{
			Status = INVALID_SOCKET;

			if (Context->Flags |= TS_FLAG_BLOCKING)
			{
				LARGE_INTEGER	Timeout = {0};
				PLARGE_INTEGER	pTimeout = &Timeout;
				NTSTATUS ntStatus;

				if (Context->RecvTimeout)
					Timeout.QuadPart = RELATIVE(MILLISECONDS(Context->RecvTimeout));
				else
					pTimeout = NULL;
					
				ntStatus = KeWaitForSingleObject(&Context->Datastream->DataReadyEvent, Executive, KernelMode, TRUE, pTimeout);
				if (NT_SUCCESS(ntStatus))
					Status = KStreamRead(Context->Datastream, Mem, Len);
			
				Context->IoStatus.Status = ntStatus;
			}
			else
				Context->IoStatus.Status = STATUS_PENDING;

		}	// if (!(Status = KStreamRead(Context->Datastream, Mem, Len)))
		ReleaseContext(Context);
	}	// if (Context = GetContext(Socket))
	return(Status);
}

TDISTATUS	TdiTListen(HANDLE Socket, int backlog)
{
	//TDOD:
	return STATUS_NOT_IMPLEMENTED;
}

TDISTATUS	TdiTAccept(HANDLE Socket, struct sockaddr *Name, PULONG	Namelen)
{
	//TDOD:
	return STATUS_NOT_IMPLEMENTED;
}

// ---- Init and cleanup routines -------------------------------------------------------------------------------------------------

NTSTATUS	TsocketInit(VOID)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;
	ntStatus = HandleAllocateTable(&g_Handles, sizeof(TSOCKET_CONTEXT), NULL, (HANDLE_CLEANUP_ROUTINE)&SocketCleanup);
	return(ntStatus);
}

VOID	TsocketCleanup(VOID)
{
	HandleReleaseTable(g_Handles);
}
#endif


LONG __stdcall _tlisten( LONG s, int backlog )
{
	return((g_CurrentTsocketTable->listen)(s, backlog));
}

LONG __stdcall _taccept( LONG s, struct sockaddr* addr, int* addrlen )
{
	return((g_CurrentTsocketTable->accept)(s, addr, addrlen));
}

LONG __stdcall _tsocket( int domain, int type, int protocol )
{
	return((g_CurrentTsocketTable->socket)(domain, type, protocol));
}

LONG __stdcall _tclosesocket( LONG Socket )
{
	return((g_CurrentTsocketTable->close)(Socket));
}

LONG __stdcall _tioctlsocket( LONG s, long cmd, ULONG* argp )
{
	return((g_CurrentTsocketTable->ioctl)(s, cmd, argp));
}

LONG __stdcall _tsendto( LONG s, char* buf, int len, int flags, struct sockaddr* to, int tolen )
{
	return((g_CurrentTsocketTable->sendto)(s, buf, len, flags, to, tolen));
}

LONG __stdcall _trecvfrom( LONG s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen )
{
	return((g_CurrentTsocketTable->recvfrom)(s, buf, len, flags, from, fromlen));
}

LONG __stdcall _tgetsockopt( LONG s, int level, int optname, char* optval, int* optlen )
{
	return((g_CurrentTsocketTable->getsockopt)(s, level, optname, optval, optlen));
}

LONG __stdcall _tselect( int maxfdp1, struct fd_set *readset, struct fd_set *writeset, struct fd_set *exceptset, struct timeval *timeout )
{
	return((g_CurrentTsocketTable->select)(maxfdp1, readset, writeset, exceptset, timeout));
}

LONG __stdcall _tconnect( LONG s, struct sockaddr* name, int namelen )
{
	return((g_CurrentTsocketTable->connect)(s, name, namelen));
}

LONG __stdcall _tsetsockopt( LONG s, int level, int optname, char* optval, int optlen )
{
	return((g_CurrentTsocketTable->setsockopt)(s, level, optname, optval, optlen));
}

LONG __stdcall _tbind( LONG s, struct sockaddr* name, int namelen )
{
	return((g_CurrentTsocketTable->bind)(s, name, namelen));
}

LONG __stdcall _tshutdown( LONG s, int how )
{
	return((g_CurrentTsocketTable->shutdown)(s, how));
}

// we need to workaround lwip problem
// it can't send data more then TCP_SND_BUF/2 bytes
#define SEND_MAX_SIZE TCP_SND_BUF/2

LONG __stdcall _tsend( LONG s, char* buf, int len, int flags )
{
	int bSent = -1;
	int bSize;
	int bTotal=0;
	FUNC_SEND pSend = g_CurrentTsocketTable->send;
	FUNC_SELECT pSelect = g_CurrentTsocketTable->select;

	do 
	{
		fd_set w;

		FD_ZERO(&w);
		FD_SET(s,&w);

		if ( pSelect( s+1,NULL,&w, NULL, NULL ) == -1 ){
			break;
		}

		// we can send only TCP_SND_BUF/2 blocks
		bSize = min(SEND_MAX_SIZE,len - bTotal);
		bSent = pSend(s, (char*)buf+bTotal, bSize, 0);
		if ( bSent == -1 ){
			break;
		}
		bTotal += bSent;
	} while (bTotal < len);

	return bTotal;
}

LONG __stdcall _trecv( LONG s, char* buf, int len, int flags )
{
	return((g_CurrentTsocketTable->recv)(s, buf, len, flags));
}

LONG __stdcall _tgetsockname( LONG s, struct sockaddr* name, int* namelen )
{
	return((g_CurrentTsocketTable->getsockname)(s, name, namelen));
}

LONG __stdcall _tgetpeername( LONG s, struct sockaddr* name, int* namelen )
{
	return((g_CurrentTsocketTable->getpeername)(s, name, namelen));
}

struct hostent* __stdcall _tgethostbyname( char *name )
{
	return((g_CurrentTsocketTable->gethostbyname)(name));
}