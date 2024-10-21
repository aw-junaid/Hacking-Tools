//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Module: tdisup.h
//	Author: Zufyxe, July 2010
//	Description:
//	 TDI based socket interface.
//


typedef struct _TDI_IP_ADDRESS {
  USHORT	AddressLength;
  USHORT	AddressType;
  TDI_ADDRESS_IP Address;
} TDI_IP_ADDRESS, *PTDI_IP_ADDRESS;




// Tdi status codes
	#define TDI_SUCCESS             STATUS_SUCCESS
    #define TDI_NO_RESOURCES        STATUS_INSUFFICIENT_RESOURCES
    #define TDI_ADDR_IN_USE         STATUS_ADDRESS_ALREADY_EXISTS
    #define TDI_BAD_ADDR            STATUS_INVALID_ADDRESS_COMPONENT
    #define TDI_NO_FREE_ADDR        STATUS_TOO_MANY_ADDRESSES
    #define TDI_ADDR_INVALID        STATUS_INVALID_ADDRESS
    #define TDI_ADDR_DELETED        STATUS_ADDRESS_CLOSED
    #define TDI_BUFFER_OVERFLOW     STATUS_BUFFER_OVERFLOW
    #define TDI_BAD_EVENT_TYPE      STATUS_INVALID_PARAMETER
    #define TDI_BAD_OPTION          STATUS_INVALID_PARAMETER
    #define TDI_CONN_REFUSED        STATUS_CONNECTION_REFUSED
    #define TDI_INVALID_CONNECTION  STATUS_CONNECTION_INVALID
    #define TDI_ALREADY_ASSOCIATED  STATUS_ADDRESS_ALREADY_ASSOCIATED
    #define TDI_NOT_ASSOCIATED      STATUS_ADDRESS_NOT_ASSOCIATED
    #define TDI_CONNECTION_ACTIVE   STATUS_CONNECTION_ACTIVE
    #define TDI_CONNECTION_ABORTED  STATUS_CONNECTION_ABORTED
    #define TDI_CONNECTION_RESET    STATUS_CONNECTION_RESET
    #define TDI_TIMED_OUT           STATUS_IO_TIMEOUT
    #define TDI_GRACEFUL_DISC       STATUS_GRACEFUL_DISCONNECT
    #define TDI_NOT_ACCEPTED        STATUS_DATA_NOT_ACCEPTED
    #define TDI_MORE_PROCESSING     STATUS_MORE_PROCESSING_REQUIRED
    #define TDI_INVALID_STATE       STATUS_INVALID_DEVICE_STATE
    #define TDI_INVALID_PARAMETER   STATUS_INVALID_PARAMETER
    #define TDI_DEST_NET_UNREACH    STATUS_NETWORK_UNREACHABLE
    #define TDI_DEST_HOST_UNREACH   STATUS_HOST_UNREACHABLE
    #define TDI_DEST_UNREACHABLE    TDI_DEST_HOST_UNREACH
    #define TDI_DEST_PROT_UNREACH   STATUS_PROTOCOL_UNREACHABLE
    #define TDI_DEST_PORT_UNREACH   STATUS_PORT_UNREACHABLE
    #define TDI_INVALID_QUERY       STATUS_INVALID_DEVICE_REQUEST
    #define TDI_REQ_ABORTED         STATUS_REQUEST_ABORTED
    #define TDI_BUFFER_TOO_SMALL    STATUS_BUFFER_TOO_SMALL
    #define TDI_CANCELLED           STATUS_CANCELLED
    #define TDI_BUFFER_TOO_BIG      STATUS_INVALID_BUFFER_SIZE
    #define TDI_INVALID_REQUEST     STATUS_INVALID_DEVICE_REQUEST
    #define TDI_PENDING             STATUS_PENDING
    #define TDI_ITEM_NOT_FOUND      STATUS_OBJECT_NAME_NOT_FOUND



NTSTATUS	
TdiCreateConnection(
		PHANDLE			Handle, 
		PFILE_OBJECT	*ConnectionObject
		);

NTSTATUS
TdiCreateAddress(
		PHANDLE			Handle, 
		PFILE_OBJECT	*FileObject, 
		ULONG			Type,
		ULONG			Address,
		USHORT			Port
		);

NTSTATUS
TdiSetEventHandler(
		PFILE_OBJECT	FileObject,
		LONG			EventType, 
		PVOID			EventHandler,
		PVOID			EventContext
		);

NTSTATUS
TdiBind(
		PFILE_OBJECT	FileObject, 
		HANDLE			Address
		);

NTSTATUS
TdiConnect(
		PFILE_OBJECT		FileObject, 
		ULONG				Addr, 
		USHORT				Port,
		PIO_STATUS_BLOCK	pIoStatus
		);

NTSTATUS
TdiDisconnect(
		PFILE_OBJECT FileObject
		);

NTSTATUS
TdiSend(
		PFILE_OBJECT	FileObject, 
		PVOID			Data, 
		ULONG			Length
		);

NTSTATUS
TdiRecv(
		PFILE_OBJECT	FileObject, 
		PVOID			Data, 
		ULONG			Length
		);

NTSTATUS
TdiEventDisconnect(
		PVOID	EventContext, 
		CONNECTION_CONTEXT cContext, 
		LONG	Unused0, 
		PVOID	Unused1, 
		LONG	Unused2, 
		PVOID	Unused3, 
		ULONG	Flags
		);

NTSTATUS
TdiEventError(
		PVOID		Unused0, 
		NTSTATUS	Status
		);

NTSTATUS
TdiEventReceive(
		PVOID	Unused0, 
		CONNECTION_CONTEXT Unused1, 
		ULONG	Flags, 
		ULONG	Indicated, 
		ULONG	Available, 
		PULONG	Taken, 
		PVOID	Unused2, 
		PIRP	*Irp
		);

NTSTATUS
TdiSendto(
		PFILE_OBJECT FileObject, 
		PVOID		Data, 
		ULONG		Length,
		ULONG		Addr,
		USHORT		Port,
		PKEVENT		WriteEvent
		);

NTSTATUS
TdiRecvfrom(
	PFILE_OBJECT	FileObject, 
	PVOID			Data, 
	ULONG			Length, 
	ULONG			Addr,
	USHORT			Port,
	PULONG			pRead,
	PKEVENT			ReadEvent
	);

NTSTATUS
TdiQueryAddress(
	PFILE_OBJECT		AddressObject, 
	PTDI_ADDRESS_INFO	AddressInfo,
	PULONG				pInfoLength
	);