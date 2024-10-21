#ifndef __MINIPORT_H_
#define __MINIPORT_H_

#include "protocol.h"

//
//	All mac options require the reserved bit to be set in
//	the miniports mac options.
//
#define	NDIS_MAC_OPTION_NDISWAN		0x00000001

typedef struct _NDIS_WRAPPER_HANDLE
{
	PDRIVER_OBJECT DriverObject;
	UNICODE_STRING ServiceRegPath;
}NDIS_WRAPPER_HANDLE, *PNDIS_WRAPPER_HANDLE;


//
// Definitions for NDIS_MINIPORT_BLOCK GeneralFlags.
//
#define	fADAPTER_NORMAL_INTERRUPTS				0x00000001
#define	fADAPTER_IN_INITIALIZE					0x00000002
#define	fADAPTER_ARCNET_BROADCAST_SET			0x00000004
#define	fADAPTER_BUS_MASTER					0x00000008
#define	fADAPTER_DMA_32_BIT_ADDRESSES			0x00000010
#define	fADAPTER_BEING_REMOVED					0x00000020
#define	fADAPTER_SG_LIST						0x00000040
#define	fADAPTER_SUPPORTS_MEDIA_QUERY			0x00000080
#define	fADAPTER_REQUEST_TIMEOUT				0x00000100
#define	fADAPTER_INDICATES_PACKETS				0x00000200
#define	fADAPTER_PROCESSING_REQUEST			0x00000400
#define	fADAPTER_IGNORE_PACKET_QUEUE			0x00000800
#define	fADAPTER_IGNORE_REQUEST_QUEUE			0x00001000
#define	fADAPTER_IGNORE_TOKEN_RING_ERRORS		0x00002000
#define	fADAPTER_IS_IN_ALL_LOCAL				0x00004000
#define	fADAPTER_INTERMEDIATE_DRIVER			0x00008000
#define	fADAPTER_IS_NDIS_5_0					0x00010000
#define	fADAPTER_IS_CO							0x00020000
#define	fADAPTER_DESERIALIZE					0x00040000
#define	fADAPTER_SHUT_DOWN						0x00080000
#define	fADAPTER_RESET_REQUESTED				0x00100000
#define	fADAPTER_RESET_IN_PROGRESS				0x00200000
#define	fADAPTER_RESOURCES_AVAILABLE			0x00400000
#define	fADAPTER_ORPHANED						0x00800000
#define	fADAPTER_RESTORING_FILTERS				0x01000000
#define	fADAPTER_REQUIRES_MEDIA_POLLING		0x02000000
#define	fADAPTER_SUPPORTS_MEDIA_SENSE			0x04000000
#define	fADAPTER_SHUTTING_DOWN					0x08000000
#define	fADAPTER_SECONDARY						0x10000000
#define	fADAPTER_MEDIA_CONNECTED				0x20000000
#define	fADAPTER_INACTIVE						0x40000000
#define	fADAPTER_HALTING						0x80000000

//
//  Macros for setting, clearing, and testing bits in the Miniport Flags.
//
#define KIP_MINIPORT_SET_FLAG(_M, _F)	 		((_M)->MiniportFlags |= (_F))
#define KIP_MINIPORT_CLEAR_FLAG(_M, _F)			((_M)->MiniportFlags &= ~(_F))
#define KIP_MINIPORT_TEST_FLAG(_M, _F)			(((_M)->MiniportFlags & (_F)) != 0)
#define KIP_MINIPORT_TEST_FLAGS(_M, _F)			(((_M)->MiniportFlags & (_F)) == (_F))

//
// adapter support macro
#define KIP_MINIPORT_SET_INACTIVE(_M) KIP_MINIPORT_SET_FLAG(_M, fADAPTER_INACTIVE)
#define KIP_MINIPORT_SET_ACTIVE(_M)   KIP_MINIPORT_CLEAR_FLAG(_M, fADAPTER_INACTIVE)
#define KIP_MINIPORT_IS_ACTIVE(_M)    (KIP_MINIPORT_TEST_FLAG(_M, fADAPTER_INACTIVE)==FALSE)

// kip adapter flags
#define KIP_ADAPTER_INITIALIZED 0x00000001
#define KIP_ADAPTER_PAUSED      0x00000004
#define KIP_ADAPTER_QUERY_PENDING 0x00000010 // query information pending
#define KIP_ADAPTER_SET_PENDING   0x00000020 // set information pending

#define KIP_IS_ADAPTER_INITIALIZED(_A) (((_A)->Flags & (KIP_ADAPTER_INITIALIZED)) != 0)
#define KIP_ADAPTER_GET_QUERY_PENDING(_A)    (((_A)->Flags & (KIP_ADAPTER_QUERY_PENDING)) != 0)
#define KIP_ADAPTER_SET_QUERY_PENDING(_A)    (((_A)->Flags & (KIP_ADAPTER_SET_PENDING)) != 0)


#define MAX_VENDOR_DESCRIPTION_LENGTH		128

//////////////////////////////////////////////////////////////////////////
// forward declaration
typedef struct _KIP_NDIS_MINIPORT KIP_NDIS_MINIPORT,*PKIP_NDIS_MINIPORT;
typedef struct _KIP_NDIS_ADAPTER KIP_NDIS_ADAPTER,*PKIP_NDIS_ADAPTER;
typedef struct _KIP_NDIS_PROTOCOL KIP_NDIS_PROTOCOL,*PKIP_NDIS_PROTOCOL;

//
// NDIS request 
//
typedef struct _NDIS_QUERY_INFORMATION
{
	BOOLEAN		Internal; //TRUE, if internal request
	NDIS_OID    Oid;
	PVOID       InformationBuffer;
	UINT        InformationBufferLength;
	PUINT        BytesWritten;
	PUINT        BytesNeeded;
	NDIS_STATUS Status;
	NDIS_EVENT  Event; // completion event
} NDIS_QUERY_INFORMATION,*PNDIS_QUERY_INFORMATION;

typedef struct _NDIS_SET_INFORMATION
{
	BOOLEAN		Internal; //TRUE, if internal request
	NDIS_OID    Oid;
	PVOID       InformationBuffer;
	UINT        InformationBufferLength;
	PUINT        BytesRead;
	PUINT        BytesNeeded;
	NDIS_STATUS Status;
	NDIS_EVENT		Event; // completion event
} NDIS_SET_INFORMATION,*PNDIS_SET_INFORMATION;

//
//  NDIS Request context structure for ndis6
//
typedef struct _NDISPROT_REQUEST
{
	union
	{
		NDIS_OID_REQUEST         OidRequest;
		NDIS_REQUEST NdisRequest;
	};
	NDIS_EVENT               ReqEvent;
	ULONG                    Status;

} NDISPROT_REQUEST, *PNDISPROT_REQUEST;

//
// miniport 
//
typedef struct _KIP_NDIS_MINIPORT
{
	// NIC driver list
	LIST_ENTRY									Link;

	/* Lock for this structure */
	NDIS_SPIN_LOCK								Lock;

	// ndis version
	// ndis version
	UCHAR										MajorNdisVersion;
	UCHAR                                       MinorNdisVersion;

	//kip nic driver flags
	ULONG										Flags;

	// list of adapters
	LIST_ENTRY									AdapterList;

	// original driver object
	PDRIVER_OBJECT								DriverObject;

	//original add device function
	PDRIVER_DISPATCH							PnPDispatch;

	// ndis parameters
	union
	{
		struct  
		{

			NDIS_MINIPORT_DRIVER_CHARACTERISTICS	MiniportCharacteristics;

			// adapter general attributes
			// A miniport driver passes a pointer to an NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES 
			// structure in the MiniportAttributes parameter of the NdisMSetMiniportAttributes function.
			// A miniport driver calls NdisMSetMiniportAttributes from its MiniportInitializeEx 
			// function during initialization.
			NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES Attributes;

			// A handle to a driver-allocated context area where the driver maintains 
			// state and configuration information.
			// The miniport driver passes this context area to the NdisMRegisterMiniportDriver function. 
			NDIS_HANDLE  NdisMiniportAdapterContext;

			// variable that uniquely identifies this driver. The driver must save this handle for use in subsequent NdisXxx function calls.
			NDIS_HANDLE NdisMiniportDriverHandle;

			DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) CHAR STUB_MiniportInitializeEx[MAX_STUB_SIZE];

		}Ndis6;

		struct  
		{
			//Specifies the handle returned by NdisMInitializeWrapper. 
			NDIS_HANDLE		NdisWrapperHandle;

			NDIS51_MINIPORT_CHARACTERISTICS		MiniportCharacteristics;

			// miniport stubs
			DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) CHAR STUB_MiniportInitialize[MAX_STUB_SIZE];
		}Ndis5;
	};

}KIP_NDIS_MINIPORT,*PKIP_NDIS_MINIPORT;

// NIC

typedef enum _KIP_INTERFACE_TYPE
{
	IF_TYPE_NIC = 0,
	IF_TYPE_WAN
}KIP_INTERFACE_TYPE,*PKIP_INTERFACE_TYPE;

typedef struct _KIP_NDIS_INTERFACE
{
	KIP_INTERFACE_TYPE Type;

	// object's list link
	LIST_ENTRY									Link;

	/* Lock for this structure */
	NDIS_SPIN_LOCK								Lock;

	// backward pointer to driver
	PKIP_NDIS_MINIPORT							Miniport;

	/* Maximum Transfer Unit */
	USHORT MTU;

	/* Link speed */
	ULONG Speed;

	// miniport medium
	NDIS_MEDIUM MediaType;
	NDIS_PHYSICAL_MEDIUM PhysicalMediumType;

	// common part
	UCHAR CurrentMacAddress[ETHARP_HWADDR_LEN];  /* Local HW address */
	ULONG HWAddressLength;                   /* Length of HW address */

	// query/set information requests
	NDIS_SET_INFORMATION SetInformationRequest; //ndis5
	NDIS_QUERY_INFORMATION QueryInformationRequest; //ndis5
	PNDISPROT_REQUEST CurrentRequest; // ndis6 and ndiswan

	//some booleans for optimization
	ULONG bNdisTransferDataNeverCalled;
	ULONG bLookaheadDataMayBeUsed;

	// number of media connects
	LONG MediaDisconnectCount;
	// number of media connects
	LONG MediaConnectCount;

	// protocol section
	KIP_NDIS_PROTOCOL Protocol; 

}KIP_NDIS_INTERFACE,*PKIP_NDIS_INTERFACE;

typedef struct _KIP_NDIS_ADAPTER
{
	KIP_NDIS_INTERFACE Interface;

	// adapter queue in miniport
	LIST_ENTRY									MiniportLink;

	// wan linked list
	LIST_ENTRY									RasLinkList;
	LONG										RasCount; //link counter

	// miniport uses this handle to pass to Ndisxxx functions
	NDIS_HANDLE									NdisMiniportHandle;

	//resident context area allocated by MiniportInitialize/MiniportInitializeEx
	NDIS_HANDLE									NdisMiniportAdapterContext;

	//kip miniport flags
	ULONG										MiniportFlags;

	//kip flags
	ULONG										Flags;

	// Miniport's Device State 
	NDIS_DEVICE_POWER_STATE						MPDeviceState;
	// True - When the miniport is transitioning from a D0 to Standby (>D0) State
	BOOLEAN										StandingBy;

	CHAR szVendorDescription[MAX_VENDOR_DESCRIPTION_LENGTH+1];
	
	UCHAR HeaderSize;                       /* Size of link-level header */
	ULONG MinFrameSize;                      /* Minimum frame size in bytes */
	ULONG MaxFrameSize;                      /* Maximum frame size in bytes */
	ULONG MaxSendPackets;                    /* Maximum number of packets per send */
	ULONG MacOptions;                        /* MAC options for NIC driver/adapter */
	ULONG PacketFilter;                      /* Packet filter for this adapter */

	/* Lookahead for adapter */
	UINT CurrentLookahead;
	UINT MaximumLookahead;

	//miniport block
	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER    SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER   SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER   ResetCompleteHandler;

	W_SEND_PACKETS_HANDLER      SendPacketsHandler;
	NDIS_M_START_SENDS          DeferredSendHandler;

	ETH_RCV_INDICATE_HANDLER    EthRxIndicateHandler;
	TR_RCV_INDICATE_HANDLER     TrRxIndicateHandler;

	ETH_RCV_COMPLETE_HANDLER    EthRxCompleteHandler;
	TR_RCV_COMPLETE_HANDLER     TrRxCompleteHandler;

	NDIS_M_STATUS_HANDLER       StatusHandler;          // EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER StatusCompleteHandler;  // EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;      // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER QueryCompleteHandler;   // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER SetCompleteHandler;     // EXPOSED via macros. Do not move

	NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;
	WAN_RCV_HANDLER             WanRcvHandler;
	WAN_RCV_COMPLETE_HANDLER    WanRcvCompleteHandler;

	// common stubs
	CHAR STUB_EthRxIndicateHandler[MAX_STUB_SIZE];
	CHAR STUB_EthRxCompleteHandler[MAX_STUB_SIZE];

}KIP_NDIS_ADAPTER,*PKIP_NDIS_ADAPTER;

#define KIP_LOCK_ADAPTER(_A) \
	NdisAcquireSpinLock(&(_A)->Interface.Lock)

#define KIP_UNLOCK_ADAPTER(_A) \
	NdisReleaseSpinLock(&(_A)->Interface.Lock)

#define GetProtocol(_I) &(((PKIP_NDIS_INTERFACE)(_I))->Protocol)

// wan  link state
typedef struct _KIP_RAS_LINK
{
	KIP_NDIS_INTERFACE Interface;
	PKIP_NDIS_ADAPTER pAdapter; // back pointer to ndis adatpter

	// ndis link context
	NDIS_HANDLE NdisLinkHandle;

	// wan settings
	ip_addr_t IpAddr;
	ip_addr_t IpMask;
	ip_addr_t RemoteIpAddr;

#define RAS_LINK_BUFFER_LENGTH 1024

	UCHAR  RemoteAddress [ETHARP_HWADDR_LEN];	// Represents the address of the remote node on the link in Ethernet-style format. NDISWAN supplies this value.

	ULONG  ProtocolBufferLength;// Specifies the number of bytes in the buffer at ProtocolBuffer
	UCHAR  ProtocolBuffer [RAS_LINK_BUFFER_LENGTH]; // Containing protocol-specific information supplied by a higher-level component that makes connections through NDISWAN
	// to the appropriate protocol(s). Maximum size is 600 bytes (on Windows Vista)

} KIP_RAS_LINK, *PKIP_RAS_LINK;

NDIS_STATUS
	MpInitialize(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_MEDIUM MediaType
		);

VOID
	MpHalt(
		IN PKIP_NDIS_ADAPTER pAdapter
		);

NDIS_STATUS
	MpSendPacketHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_PACKET Packet
		);

BOOLEAN
	MpSendPacketCompleteHandler(
		IN  PKIP_NDIS_ADAPTER		pAdapter,
		IN  PNDIS_PACKET            Packet,
		IN  NDIS_STATUS             Status
		);

INT
	MpIndicatePacketsHandler(
		IN  PKIP_NDIS_ADAPTER		pAdapter,
		IN  PPNDIS_PACKET           *ppPacketArray,
		IN  PUINT                   pNumberOfPackets
		);

VOID
	MpIndicateEthReceiveHandler(
		IN PKIP_NDIS_ADAPTER       pAdapter,
		IN NDIS_HANDLE             MacReceiveContext,
		IN PVOID                   HeaderBuffer,
		IN UINT                    HeaderBufferSize,
		IN PVOID                   LookaheadBuffer,
		IN UINT                    LookaheadBufferSize,
		IN UINT                    PacketSize,
		OUT PBOOLEAN               CallOrig
		);

BOOLEAN 
	MpTransferDataComplete(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_PACKET  Packet,
		IN NDIS_STATUS  Status,
		IN UINT  BytesTransferred,
		OUT PBOOLEAN CallOrig
		);

VOID
	MpReturnPacketHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_PACKET *pPacket,
		IN PBOOLEAN CallOrig
		);

VOID
	MpIndicateStatusHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_STATUS  GeneralStatus,
		IN PVOID  StatusBuffer,
		IN UINT  StatusBufferSize,
		IN BOOLEAN BeforeMiniport
		);

NDIS_STATUS
	MpSetInformationHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_SET_INFORMATION Request,
		OUT PBOOLEAN         CallOrig
		);

VOID
	MpSetInformationCompleteHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_SET_INFORMATION Request,
		IN NDIS_STATUS       Status,
		OUT PBOOLEAN         CallOrig
		);

NDIS_STATUS
	MpQueryInformationHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_QUERY_INFORMATION Request,
		OUT PBOOLEAN         CallOrig
		);

VOID
	MpQueryInformationCompleteHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_QUERY_INFORMATION Request,
		IN NDIS_STATUS       Status,
		OUT PBOOLEAN         CallOrig
		);

VOID
	MpOidRequestComplete(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_OID_REQUEST  OidRequest,
		IN NDIS_STATUS       Status,
		OUT PBOOLEAN         CallOrig
		);

NDIS_STATUS
	MpSetMiniportAttributes (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_MINIPORT_ADAPTER_ATTRIBUTES  MiniportAttributes
		);

VOID
	MpSendSendNetBufferLists(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNET_BUFFER_LIST  NetBufferLists
		);

BOOLEAN
	MpSendNetBufferListsComplete(
		IN  PKIP_NDIS_ADAPTER pAdapter,
		IN  PNET_BUFFER_LIST  *NetBufferLists,
		IN  ULONG  SendCompleteFlags
		);

VOID
	MpIndicateReceiveNetBufferLists(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNET_BUFFER_LIST  *NetBufferLists,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN PULONG  NumberOfNetBufferLists,
		IN ULONG  ReceiveFlags
		);

NDIS_STATUS
	MpIndicateDataBufferToNdis(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNET_BUFFER NetBuffer,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN ULONG  ReceiveFlags
		);

#endif //__MINIPORT_H_