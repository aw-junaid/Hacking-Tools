#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

//////////////////////////////////////////////////////////////////////////
// forward declaration
typedef struct _KIP_NDIS_MINIPORT KIP_NDIS_MINIPORT,*PKIP_NDIS_MINIPORT;
typedef struct _KIP_NDIS_INTERFACE KIP_NDIS_INTERFACE,*PKIP_NDIS_INTERFACE;
typedef struct _KIP_NDIS_ADAPTER KIP_NDIS_ADAPTER,*PKIP_NDIS_ADAPTER;
typedef struct _KIP_NDIS_PROTOCOL KIP_NDIS_PROTOCOL,*PKIP_NDIS_PROTOCOL;
typedef struct _KIP_RAS_LINK KIP_RAS_LINK,*PKIP_RAS_LINK;

//////////////////////////////////////////////////////////////////////////
// NDIS PROTOCOL

typedef
BOOLEAN
	(NTAPI *PSEND_BUFFER)(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN struct pbuf *p
		);

#define TRANSMIT_PACKET_GROW_AMOUNT 128
#define MAX_RECV_PACKET_POOL_SIZE 0x0000FFFF

#define	  MODULE_TAG1				'DpiK'


// protocol section
typedef struct _KIP_NDIS_PROTOCOL
{
	ULONG					EnableDHCP;
	USHORT					PppoeSessId;

	PKIP_NDIS_INTERFACE pInterface;

	// lwip if
	struct netif IIF;

	// protocol buffer send function
	PSEND_BUFFER			PSendBuffer;

	// number of outstanding sends
	LONG OutstandingSends;
	LONG PendingPackets; // number of packet descriptors

	union
	{
		struct {
			// for ndis 5
			NDIS_HANDLE					PacketPoolHandle;
			NDIS_HANDLE					BufferPoolHandle;

			NDIS_SPIN_LOCK				PacketPoolLock;

			LIST_ENTRY					PendingTransfertData; 
			NDIS_SPIN_LOCK				PendingTransfertLock;
		};
		HANDLE NdisNetBufferListPool; // for ndis6
	};

}KIP_NDIS_PROTOCOL,*PKIP_NDIS_PROTOCOL;

#define KIP_LOCK_PACKET_POOL(_P) \
	if (OsGetMajorVersion() == 4) \
		NdisAcquireSpinLock( &(_P)->PacketPoolLock )

#define KIP_UNLOCK_PACKET_POOL(_P) \
	if (OsGetMajorVersion() == 4) \
		NdisReleaseSpinLock ( &(_P)->PacketPoolLock );

//per packet protocol context
typedef struct _KIP_PACKET_CONTEXT
{
	PKIP_NDIS_ADAPTER pAdapter;
	PKIP_NDIS_PROTOCOL pProtocol;
}KIP_PACKET_CONTEXT,*PKIP_PACKET_CONTEXT;

typedef struct DECLSPEC_ALIGN(16) _NDIS_PACKET_RESERVED
{
	LIST_ENTRY		qLink;

	NDIS_STATUS		Status;
	UINT			BytesTransferred;
	ULONG			FilterResult;

	PKIP_NDIS_ADAPTER Adapter;

	ULONG			DataLength;	//data length
	UCHAR			DataBuffer[MAX_ETHER_SIZE]; //protocol extra data

} NDIS_PACKET_RESERVED, *PNDIS_PACKET_RESERVED;

typedef struct _NDIS_PACKET_EX
{
	NDIS_PACKET				Packet;
	NDIS_PACKET_RESERVED	Reserved;

} NDIS_PACKET_EX, *PNDIS_PACKET_EX;

#define NDIS_PER_PACKET_CONTEXT(_P) \
	(PKIP_PACKET_CONTEXT)(((PNDIS_PACKET)(_P))->ProtocolReserved)

#define NDIS_PER_NBLIST_CONTEXT(_P) \
	(PKIP_PACKET_CONTEXT)(((PNET_BUFFER_LIST)(_P))->ProtocolReserved)

//////////////////////////////////////////////////////////////////////////
// ndis packet descriptor

#define PACKET_EMBEDED_BUFFER 0x00000001 // we need to free buffer while releasing packet

typedef struct _KIP_PACKET_DESCRIPTOR
{
	LONG			RefCount;		// Packet reference counter ()
	ULONG			__padding__;	// padding to align following  pointers to 64-bit boundary

	LIST_ENTRY		ListEntry;		// linked list entry for debugging

	ULONG			Flags; // packet flags

	PVOID			pOrgPacket; //pointer to original Ndis_Packet or pointer to original Net_Buffer_List
	PVOID			pNB;			// Pointer to NET_BUFFER described by this PACKET_DESCRIPTOR

	ULONG			Length;			// overall packet length

	// original adapter
	PKIP_NDIS_PROTOCOL pProtocol;

	//////////////////////////////////////////////////////////////////////////
	// reserver place for pbuf
	struct pbuf_custom	LwipPacket;

	//////////////////////////////////////////////////////////////////////////

	DECLSPEC_CACHEALIGN VOID* DataBuffer;	// data buffer (if any)
}KIP_PACKET_DESCRIPTOR,*PKIP_PACKET_DESCRIPTOR;

NDIS_STATUS
	ProtocolInitializeSubsystem(
		VOID
		);

VOID 
	ProtocolReleaseSubsystem(
		VOID
		);

VOID
	ProtocolRegisterInterface(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN ULONG IpAddress,
		IN ULONG IpMask,
		IN ULONG IpGateWay,
		IN BOOLEAN EnableDHCP
		);

NDIS_STATUS
	ProtocolBindAdapter(
		IN PKIP_NDIS_INTERFACE pInterface,
		IN ULONG IpAddress,
		IN ULONG IpMask,
		IN ULONG IpGateWay,
		IN BOOLEAN WanLink
		);

NDIS_STATUS
	ProtocolUnBindAdapter(
		IN PKIP_NDIS_MINIPORT pMiniport,
		IN PKIP_NDIS_INTERFACE pInterface,
		IN PKIP_NDIS_PROTOCOL pProtocol
		);

BOOLEAN
	ProtocolSendPacketComplete(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET      Packet,
		IN NDIS_STATUS      Status
		);

BOOLEAN
	ProtocolSendPBuffer(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN struct pbuf *p
		);

NDIS_STATUS
	ProtocolReceiveInternal(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET_EX MyPacket
		);

NDIS_STATUS
	ProtocolReceivePacket(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET Packet
		);

NDIS_STATUS
	ProtocolWanReceive(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET Packet
		);

NDIS_STATUS
	ProtocolReceive(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN NDIS_HANDLE             MacReceiveContext,
		IN PVOID                   HeaderBuffer,
		IN UINT                    HeaderBufferSize,
		IN PVOID                   LookaheadBuffer,
		IN UINT                    LookaheadBufferSize,
		IN UINT                    ResidualSize,
		IN UINT                    PacketSize
		);

PNDIS_PACKET
	ProtocolReturnPacketHandler(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET  Packet
		);

BOOLEAN
	ProtocolSendNetBufferListComplete(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN  PNET_BUFFER_LIST NetBufferList
		);

NDIS_STATUS
	ProtocolIndicateNetBufferList(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNET_BUFFER_LIST NetBufferList,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN ULONG  ReceiveFlags
		) ;


#endif //__PROTOCOL_H_