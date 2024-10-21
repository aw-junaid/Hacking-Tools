//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: ndisif.h
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//	ndis interface functions declaration


#ifndef __NDIS_IF_
#define __NDIS_IF_

// types definition
typedef
PNET_BUFFER_LIST
	(NTAPI *PNDIS_ALLOCATE_NET_BUFFER_LIST)(
		IN NDIS_HANDLE  PoolHandle,
		IN USHORT  ContextSize,
		IN USHORT  ContextBackFill
		); 

typedef
NDIS_HANDLE
	(NTAPI *PNDIS_ALLOCATE_NET_BUFFER_LIST_POOL)(
		IN NDIS_HANDLE  NdisHandle,
		IN PNET_BUFFER_LIST_POOL_PARAMETERS Parameters
		);

typedef
VOID
	(NTAPI *PNDIS_FREE_NET_BUFFER_LIST)(
		IN PNET_BUFFER_LIST  NetBufferList
		); 

typedef
VOID
	(NTAPI *PNDIS_FREE_NET_BUFFER_LIST_POOL)(
		IN NDIS_HANDLE  PoolHandle
		); 

typedef
VOID
	(NTAPI *PNDIS_M_INDICATE_RECEIVE_NET_BUFFER_LISTS)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNET_BUFFER_LIST  NetBufferLists,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN ULONG  NumberOfNetBufferLists,
		IN ULONG  ReceiveFlags
		); 

extern PNDIS_ALLOCATE_NET_BUFFER_LIST NdisAllocateNetBufferListPtr;
extern PNDIS_ALLOCATE_NET_BUFFER_LIST_POOL NdisAllocateNetBufferListPoolPtr;
extern PNDIS_FREE_NET_BUFFER_LIST NdisFreeNetBufferListPtr;
extern PNDIS_FREE_NET_BUFFER_LIST_POOL NdisFreeNetBufferListPoolPtr;
extern PNDIS_M_INDICATE_RECEIVE_NET_BUFFER_LISTS NdisMIndicateReceiveNetBufferListsPtr;

NTSTATUS
	NDIS_InitializeInterface(
		VOID
		);

VOID
	NDIS_ReleaseInterface(
		VOID
		);

//////////////////////////////////////////////////////////////////////////
// ADAPTER LIST MANAGMENT FUNCTIONS

PKIP_NDIS_ADAPTER 
	NDIS_AllocateAdapter ( 
		IN PKIP_NDIS_MINIPORT pMiniport 
		);

VOID 
	NDIS_FreeAdapter ( 
		IN PKIP_NDIS_ADAPTER pAdapter 
		);

PKIP_NDIS_ADAPTER 
	NDIS_FindAdapterByHandle (
		IN NDIS_HANDLE MiniportHandle
		);

PKIP_NDIS_ADAPTER 
	NDIS_FindAdapterByAdapterContext (
		IN NDIS_HANDLE MiniportAdapterContext
		);

PKIP_NDIS_ADAPTER 
	NDIS_FindAdapterByDeviceObject (
		IN PKIP_NDIS_MINIPORT pMiniport,
		IN PDEVICE_OBJECT DeviceObject
		);

// ras link
PKIP_RAS_LINK 
	NDIS_AllocateRasLink ( 
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WAN_LINE_UP pWanLineUp
		);
VOID 
	NDIS_FreeRasLink ( 
		IN PKIP_NDIS_ADAPTER pAdapter, 
		IN PKIP_RAS_LINK pLink 
		);

PKIP_RAS_LINK 
	NDIS_FindRasLinkByHandle ( 
		IN PKIP_NDIS_ADAPTER pAdapter, 
		IN NDIS_HANDLE LinkHandle 
		);

PKIP_RAS_LINK 
	NDIS_FindRasLinkByMac ( 
		IN PKIP_NDIS_ADAPTER pAdapter, 
		IN char *MacAddr 
		);

// ndis driver functions
PKIP_NDIS_MINIPORT 
	NDIS_AllocateMiniport ( 
		VOID 
		);

VOID 
	NDIS_FreeMiniport ( 
		IN PKIP_NDIS_MINIPORT pMiniport 
		);

PKIP_NDIS_MINIPORT 
	NDIS_FindMiniportByWrapperHandle (
		IN NDIS_HANDLE NdisWrapperHandle
		);

PKIP_NDIS_MINIPORT 
	NDIS_FindMiniportByDriverHandle (
		IN NDIS_HANDLE NdisDriverHandle
		);

PKIP_NDIS_MINIPORT
	NDIS_FindMiniportByMiniportDriverContext (
		IN NDIS_HANDLE  MiniportAdapterContext
		);

PKIP_NDIS_MINIPORT
	NDIS_FindMiniportByDriverObject (
		IN PDRIVER_OBJECT DriverObject
		);

//////////////////////////////////////////////////////////////////////////
// NDIS INTERFACE FUNCTIONS

NDIS_STATUS
	NDIS_MQueryInformation(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_OID  Oid,
		IN PVOID  InformationBuffer,
		IN ULONG  InformationBufferLength,
		OUT PULONG  BytesWritten,
		OUT PULONG  BytesNeeded
		);

NDIS_STATUS
	NDIS_MQueryOid(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_OID  Oid,
		IN PVOID  InformationBuffer,
		IN ULONG  InformationBufferLength
		);

NDIS_STATUS
	NDIS_MSetInformation(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_OID  Oid,
		IN PVOID  InformationBuffer,
		IN ULONG  InformationBufferLength,
		OUT PULONG  BytesRead,
		OUT PULONG  BytesNeeded
		);

NDIS_STATUS
	NDIS_TransferData(
		IN PKIP_NDIS_ADAPTER pAdapter,
		OUT PNDIS_PACKET            Packet,
		OUT PUINT                   BytesTransferred,
		IN  NDIS_HANDLE             MiniportReceiveContext,
		IN  UINT                    ByteOffset,
		IN  UINT                    BytesToTransfer
		);

VOID
	NDIS_ReturnPacket(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_PACKET  Packets
		);

VOID
	NDIS_ReturnNetBufferList(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNET_BUFFER_LIST  NetBufferList,
		IN BOOLEAN bDispatch
		);

BOOLEAN 
	NDIS_CopyPacketData(
		IN PNDIS_PACKET	Packet,
		IN OUT PUCHAR		lpBuffer,
		IN ULONG			nNumberOfBytesToRead,
		IN ULONG			nOffset,                
		IN PULONG			lpNumberOfBytesRead
		);

VOID 
	NDIS_FreePacket( 
		IN PKIP_NDIS_PROTOCOL pProtocol, 
		IN PNDIS_PACKET Packet 
		);

BOOLEAN
	NDIS_CopyNetBufferToBuffer(
		 IN PNET_BUFFER		pNetBuffer,
		 IN OUT PUCHAR		lpBuffer,
		 IN ULONG			nNumberOfBytesToRead,
		 IN ULONG			nOffset,                
		 OUT PULONG			lpNumberOfBytesRead
		 );

VOID
	NDIS_GetFirstBufferFromNetBuffer(
		IN PNET_BUFFER pNetBuffer,
		OUT PVOID *FirstBufferVA,
		OUT PULONG FirstBufferLength
		);

#define NDIS_IndicatePacket(_A,_P) \
	NdisMIndicateReceivePacket((_A)->NdisMiniportHandle,(PNDIS_PACKET*)&(_P),1)

NDIS_HANDLE
	NDIS_GetPoolFromPacket(
		IN PNDIS_PACKET            Packet
		);

UNICODE_STRING
	NDIS_GetMiniportName(
		IN NDIS_HANDLE Miniport
		);

PDEVICE_OBJECT
	NDIS_GetMiniportDeviceObject(
		IN NDIS_HANDLE Miniport
		);

PNPAGED_LOOKASIDE_LIST
	NDIS_GetSGListLookasideList(
		IN NDIS_HANDLE Miniport
		);

ULONG
	NDIS_GetScatterGatherListSize(
		IN NDIS_HANDLE Miniport
		);

PDMA_ADAPTER
	NDIS_GetSystemAdapterObject(
		IN NDIS_HANDLE Miniport
		);


#endif //__NDIS_IF_