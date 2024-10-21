#ifndef __NDIS_X_H_
#define __NDIS_X_H_

#define	NUMBER_OF_WORK_ITEM_TYPES	NdisMaxWorkItems
#define	NUMBER_OF_SINGLE_WORK_ITEMS	6

#define NDIS_M_MAX_MULTI_LIST 32

typedef struct _NDIS_BIND_PATHS
{
	UINT						Number;
	NDIS_STRING					Paths[1];
} NDIS_BIND_PATHS, *PNDIS_BIND_PATHS;

//
//  Defines the type of work item.
//
typedef enum _NDIS_WORK_ITEM_TYPE
{
	NdisWorkItemRequest,
	NdisWorkItemSend,
	NdisWorkItemReturnPackets,
	NdisWorkItemResetRequested,
	NdisWorkItemResetInProgress,
	NdisWorkItemHalt,
#if !(NDIS_NT)
	NdisWorkItemSendLoopback,
#endif
	NdisWorkItemMiniportCallback,
	NdisMaxWorkItems
} NDIS_WORK_ITEM_TYPE, *PNDIS_WORK_ITEM_TYPE;

typedef enum _NDIS_PNP_DEVICE_STATE
{
	NdisPnPDeviceAdded,
	NdisPnPDeviceStarted,
	NdisPnPDeviceQueryStopped,
	NdisPnPDeviceStopped,
	NdisPnPDeviceQueryRemoved,
	NdisPnPDeviceRemoved,
	NdisPnPDeviceSurpriseRemoved
} NDIS_PNP_DEVICE_STATE;


typedef struct _FDDI_FILTER *PFDDI_FILTER;
typedef struct _NDIS_MAC_BLOCK		NDIS_MAC_BLOCK, *PNDIS_MAC_BLOCK;
typedef struct _OID_LIST    OID_LIST, *POID_LIST;
typedef struct _NDIS_M_OPEN_BLOCK		NDIS_M_OPEN_BLOCK,*PNDIS_M_OPEN_BLOCK;
typedef struct _ARC_FILTER ARC_FILTER,*PARC_FILTER;

//
// Arcnet specific stuff
//
#define ARC_SEND_BUFFERS			8
#define ARC_HEADER_SIZE				4

typedef struct _ARC_BUFFER_LIST
{
	PVOID					Buffer;
	UINT					Size;
	UINT					BytesLeft;
	struct _ARC_BUFFER_LIST *Next;
} ARC_BUFFER_LIST, *PARC_BUFFER_LIST;

typedef struct _NDIS_ARC_BUF
{
	NDIS_HANDLE					ArcnetBufferPool;
	PUCHAR						ArcnetLookaheadBuffer;
	UINT						NumFree;
	ARC_BUFFER_LIST				ArcnetBuffers[ARC_SEND_BUFFERS];
} NDIS_ARC_BUF, *PNDIS_ARC_BUF;

typedef struct _NDIS_LOG
{
	PNDIS_MINIPORT_BLOCK		Miniport;	// The owning miniport block
	KSPIN_LOCK					LogLock;	// For serialization
	PIRP						Irp;		// Pending Irp to consume this log
	UINT						TotalSize;	// Size of the log buffer
	UINT						CurrentSize;// Size of the log buffer
	UINT						InPtr;		// IN part of the circular buffer
	UINT						OutPtr;		// OUT part of the circular buffer
	UCHAR						LogBuf[1];	// The circular buffer
} NDIS_LOG, *PNDIS_LOG;

typedef struct _NDIS_MINIPORT_WORK_ITEM
{
	//
	//	Link for the list of work items of this type.
	//
	SINGLE_LIST_ENTRY 	Link;

	//
	//	type of work item and context information.
	//
	NDIS_WORK_ITEM_TYPE WorkItemType;
	PVOID 				WorkItemContext;
} NDIS_MINIPORT_WORK_ITEM, *PNDIS_MINIPORT_WORK_ITEM;

//
// block used for references using a ULONG
//
typedef struct _ULONG_REFERENCE
{
	KSPIN_LOCK                  SpinLock;
	ULONG                       ReferenceCount;
	BOOLEAN                     Closing;
} ULONG_REFERENCE, *PULONG_REFERENCE;

typedef struct _NDIS_SG_DMA_BLOCK
{
	NDIS_OBJECT_HEADER Header;
	PNDIS_MINIPORT_BLOCK Miniport;
	PVOID MiniportAdapterContext;
	PDMA_ADAPTER DmaAdapterObject;
	PVOID ProcessSGListHandler; 
	PVOID SharedMemAllocateCompleteHandler;
	ULONG Flags;
	ULONG MaximumPhysicalMapping;
	ULONG ScatterGatherListSize;
	PNPAGED_LOOKASIDE_LIST SGListLookasideList;
	LONG DmaAdapterRefCount;
	PKEVENT DmaResourcesReleasedEvent;
	PVOID SharedMemoryPage[2];
	ULONG SharedMemoryLeft[2];
	LARGE_INTEGER SharedMemoryAddress[2];
	PDMA_ADAPTER SavedDmaAdapterObject;
	PMAP_REGISTER_ENTRY MapRegisters;
	PKEVENT AllocationEvent;
	USHORT CurrentMapRegister;
	USHORT BaseMapRegistersNeeded;
	USHORT SGMapRegistersNeeded;
} NDIS_SG_DMA_BLOCK, *PNDIS_SG_DMA_BLOCK;

typedef struct _NDIS_STATS
{
	LARGE_INTEGER StartTicks;
	ULONGLONG DirectedPacketsOut;
	ULONGLONG DirectedPacketsIn;
}NDIS_STATS,*PNDIS_STATS;



typedef
VOID
(*FDDI_RCV_INDICATE_HANDLER)(
	IN	PFDDI_FILTER			Filter,
	IN	NDIS_HANDLE				MacReceiveContext,
	IN	PCHAR					Address,
	IN	UINT					AddressLength,
	IN	PVOID					HeaderBuffer,
	IN	UINT					HeaderBufferSize,
	IN	PVOID					LookaheadBuffer,
	IN	UINT					LookaheadBufferSize,
	IN	UINT					PacketSize
	);

typedef
VOID
(*FDDI_RCV_COMPLETE_HANDLER)(
	IN	PFDDI_FILTER			Filter
	);

typedef
VOID
(FASTCALL *NDIS_M_PROCESS_DEFERRED)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport
	);

typedef
NDIS_STATUS
(FASTCALL *NDIS_M_QUEUE_WORK_ITEM)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport,
	IN	NDIS_WORK_ITEM_TYPE		WorkItemType,
	IN	 PVOID					WorkItemContext1,
	IN	PVOID					WorkItemContext2
	);

typedef
NDIS_STATUS
(FASTCALL *NDIS_M_QUEUE_NEW_WORK_ITEM)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport,
	IN	NDIS_WORK_ITEM_TYPE 	WorkItemType,
	IN	 PVOID					WorkItemContext1,
	IN	PVOID					WorkItemContext2
	);

typedef
VOID
(FASTCALL *NDIS_M_DEQUEUE_WORK_ITEM)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport,
	IN	NDIS_WORK_ITEM_TYPE		WorkItemType,
	OUT PVOID	*				WorkItemContext1,
	OUT	PVOID	*				WorkItemContext2
	);

//
// Miniport extensions for NDIS 5.1
//
typedef VOID
(*W_CANCEL_SEND_PACKETS_HANDLER)(
    __in  NDIS_HANDLE             MiniportAdapterContext,
    __in  PVOID                   CancelId
    );

typedef VOID
(*W_PNP_EVENT_NOTIFY_HANDLER)(
    __in  NDIS_HANDLE             MiniportAdapterContext,
    __in  NDIS_DEVICE_PNP_EVENT   DevicePnPEvent,
    __in  PVOID                   InformationBuffer,
    __in  ULONG                   InformationBufferLength
    );

typedef VOID
(*W_MINIPORT_SHUTDOWN_HANDLER) (
    __in  NDIS_HANDLE                     MiniportAdapterContext
    );

typedef struct _X_BINDING_INFO X_BINDING_INFO,*PX_BINDING_INFO;

typedef struct _X_FILTER
{
	PX_BINDING_INFO OpenList;
	NDIS_RW_LOCK BindListLock;
	PNDIS_MINIPORT_BLOCK Miniport;
	ULONG CombinedPacketFilter;
	ULONG OldCombinedPacketFilter;
	ULONG NumOpens;
	PX_BINDING_INFO MCastSet;
	PX_BINDING_INFO SingleActiveOpen;
	UCHAR AdapterAddress[6];
	union{
		struct{
			UCHAR (*MCastAddressBuf)[6];
			UCHAR (*OldMCastAddressBuf)[6];
			ULONG MaxMulticastAddresses;
			ULONG NumAddresses;
			ULONG OldNumAddresses;
		};
		struct{
			UCHAR AdapterShortAddress[2];
			UCHAR (*MCastLongAddressBuf)[6];
			UCHAR (*MCastShortAddressBuf)[2];
			UCHAR (*OldMCastLongAddressBuf)[6];
			UCHAR (*OldMCastShortAddressBuf)[2];
			ULONG MaxMulticastLongAddresses;
			ULONG MaxMulticastShortAddresses;
			ULONG NumLongAddresses;
			ULONG NumShortAddresses;
			ULONG OldNumLongAddresses;
			ULONG OldNumShortAddresses;
			UCHAR SupportsShortAddresses;
		};
		struct{
			ULONG CombinedFunctionalAddress;
			ULONG GroupAddress;
			ULONG GroupReferences;
			ULONG OldCombinedFunctionalAddress;
			ULONG OldGroupAddress;
			ULONG OldGroupReferences;
		};
	};
}X_FILTER,*PX_FILTER;

typedef struct _DMA_OPERATIONS_V1 {
	ULONG Size;
	PPUT_DMA_ADAPTER PutDmaAdapter;
	PALLOCATE_COMMON_BUFFER AllocateCommonBuffer;
	PFREE_COMMON_BUFFER FreeCommonBuffer;
	PALLOCATE_ADAPTER_CHANNEL AllocateAdapterChannel;
	PFLUSH_ADAPTER_BUFFERS FlushAdapterBuffers;
	PFREE_ADAPTER_CHANNEL FreeAdapterChannel;
	PFREE_MAP_REGISTERS FreeMapRegisters;
	PMAP_TRANSFER MapTransfer;
	PGET_DMA_ALIGNMENT GetDmaAlignment;
	PREAD_DMA_COUNTER ReadDmaCounter;
	PGET_SCATTER_GATHER_LIST GetScatterGatherList;
	PPUT_SCATTER_GATHER_LIST PutScatterGatherList;
} DMA_OPERATIONS_V1,*PDMA_OPERATIONS_V1;

typedef struct _DMA_OPERATIONS_V2 {
	DMA_OPERATIONS_V1;
	PCALCULATE_SCATTER_GATHER_LIST_SIZE CalculateScatterGatherList;
	PBUILD_SCATTER_GATHER_LIST BuildScatterGatherList;
	PBUILD_MDL_FROM_SCATTER_GATHER_LIST BuildMdlFromScatterGatherList;
} DMA_OPERATIONS_V2,*PDMA_OPERATIONS_V2;

typedef struct _DMA_OPERATIONS_V3 {
	DMA_OPERATIONS_V2;
	PVOID               GetDmaAdapterInfo;
	PVOID              GetDmaTransferInfo;
	PVOID    InitializeDmaTransferContext;
	PVOID          AllocateCommonBufferEx;
	PVOID        AllocateAdapterChannelEx;
	PVOID          ConfigureAdapterChannel;
	PVOID             CancelAdapterChannel;
	PVOID                    MapTransferEx;
	PVOID         GetScatterGatherListEx;
	PVOID       BuildScatterGatherListEx;
	PVOID           FlushAdapterBuffersEx;
	PVOID                FreeAdapterObject;
	PVOID             CancelMappedTransfer;
} DMA_OPERATIONS_V3,*PDMA_OPERATIONS_V3;

//
// one of these per mini-port registered on a Driver
//
struct _NDIS_MINIPORT_BLOCK_NT
{
	ULONG						NullValue;		// used to distinquish between MACs and mini-ports
	PDEVICE_OBJECT				DeviceObject;	// created by the wrapper
	PNDIS_M_DRIVER_BLOCK		DriverHandle;	// pointer to our Driver block
	NDIS_HANDLE					MiniportAdapterContext; // context when calling mini-port functions
	NDIS_STRING					MiniportName;	// how mini-port refers to us
	PNDIS_M_OPEN_BLOCK			OpenQueue;		// queue of opens for this mini-port
	PNDIS_MINIPORT_BLOCK		NextMiniport;	// used by driver's MiniportQueue
	REFERENCE					Ref;			// contains spinlock for OpenQueue

	BOOLEAN						padding1;		// normal ints:	DO NOT REMOVE OR NDIS WILL BREAK!!!
	BOOLEAN						padding2;		// processing def: DO NOT REMOVE OR NDIS WILL BREAK!!!


	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the
	// same time. The difficultly is if DPC A releases the spin lock
	// and DPC B tries to run, we want to defer B until after A has
	// exited.
	//
	BOOLEAN						LockAcquired;	// EXPOSED via macros. Do not move

	UCHAR						PmodeOpens;		// Count of opens which turned on pmode/all_local

	NDIS_SPIN_LOCK				Lock;

	PNDIS_MINIPORT_INTERRUPT	Interrupt;

	ULONG						Flags;			// Flags to keep track of the
	// miniport's state.
	//
	//Work that the miniport needs to do.
	//
	KSPIN_LOCK					WorkLock;
	SINGLE_LIST_ENTRY			WorkQueue[NUMBER_OF_WORK_ITEM_TYPES];
	SINGLE_LIST_ENTRY			WorkItemFreeQueue;

	//
	// Stuff that got deferred.
	//
	KDPC						Dpc;
	NDIS_TIMER					WakeUpDpcTimer;

	//
	// Holds media specific information.
	//
	PETH_FILTER					EthDB;		// EXPOSED via macros. Do not move
	PTR_FILTER					TrDB;		// EXPOSED via macros. Do not move
	PFDDI_FILTER				FddiDB;		// EXPOSED via macros. Do not move
	PARC_FILTER					ArcDB;		// EXPOSED via macros. Do not move

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER	SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER	SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER	ResetCompleteHandler;

	PVOID						WrapperContext;
	NDIS_MEDIUM					MediaType;

	//
	// contains mini-port information
	//
	ULONG						BusNumber;
	NDIS_INTERFACE_TYPE			BusType;
	NDIS_INTERFACE_TYPE			AdapterType;

	//
	// Holds the map registers for this mini-port.
	//
	ULONG						PhysicalMapRegistersNeeded;
	ULONG						MaximumPhysicalMapping;
	PMAP_REGISTER_ENTRY			MapRegisters;	// EXPOSED via macros. Do not move

	//
	//	WorkItem routines that can change depending on whether we
	//	are fullduplex or not.
	//
	NDIS_M_PROCESS_DEFERRED 	ProcessDeferredHandler;
	NDIS_M_QUEUE_WORK_ITEM		QueueWorkItemHandler;
	NDIS_M_QUEUE_NEW_WORK_ITEM	QueueNewWorkItemHandler;
	NDIS_M_DEQUEUE_WORK_ITEM	DeQueueWorkItemHandler;

	PNDIS_TIMER					DeferredTimer;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST			Resources;

	//
	//	This pointer is reserved. Used for debugging
	//
	PVOID						Reserved;

	PADAPTER_OBJECT				SystemAdapterObject;

	SINGLE_LIST_ENTRY			SingleWorkItems[NUMBER_OF_SINGLE_WORK_ITEMS];

	//
	//	For efficiency
	//
	W_HANDLE_INTERRUPT_HANDLER	HandleInterruptHandler;
	W_DISABLE_INTERRUPT_HANDLER	DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER	EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER		SendPacketsHandler;
	NDIS_M_START_SENDS			DeferredSendHandler;

	//
	// The following cannot be unionized.
	//
	ETH_RCV_INDICATE_HANDLER	EthRxIndicateHandler;	// EXPOSED via macros. Do not move
	TR_RCV_INDICATE_HANDLER		TrRxIndicateHandler;	// EXPOSED via macros. Do not move
	FDDI_RCV_INDICATE_HANDLER	FddiRxIndicateHandler;	// EXPOSED via macros. Do not move

	ETH_RCV_COMPLETE_HANDLER	EthRxCompleteHandler;	// EXPOSED via macros. Do not move
	TR_RCV_COMPLETE_HANDLER		TrRxCompleteHandler;	// EXPOSED via macros. Do not move
	FDDI_RCV_COMPLETE_HANDLER	FddiRxCompleteHandler;	// EXPOSED via macros. Do not move

	NDIS_M_STATUS_HANDLER		StatusHandler;			// EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER	StatusCompleteHandler;	// EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER	TDCompleteHandler;		// EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER	QueryCompleteHandler;	// EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER	SetCompleteHandler;		// EXPOSED via macros. Do not move

	/********************************************************************************************/
	/****************                                                                  **********/
	/**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
	/**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
	/****************                                                                  **********/
	/********************************************************************************************/

	PNDIS_MAC_BLOCK				FakeMac;

	ULONG						InterruptLevel;
	ULONG						InterruptVector;
	NDIS_INTERRUPT_MODE			InterruptMode;

	UCHAR						TrResetRing;
	UCHAR						ArcnetAddress;

	//
	//	This is the processor number that the miniport's
	//	interrupt DPC and timers are running on.
	//
	UCHAR						AssignedProcessor;

	NDIS_HANDLE					ArcnetBufferPool;
	PARC_BUFFER_LIST			ArcnetFreeBufferList;
	PARC_BUFFER_LIST			ArcnetUsedBufferList;
	PUCHAR						ArcnetLookaheadBuffer;
	UINT						CheckForHangTimeout;

	//
	// These two are used temporarily while allocating the map registers.
	//
	KEVENT						AllocationEvent;
	UINT						CurrentMapRegister;

	//
	// Send information
	//
	NDIS_SPIN_LOCK				SendLock;
	ULONG						SendFlags;			// Flags for send path.
	PNDIS_PACKET				FirstPacket;		// This pointer serves two purposes;
	// it is the head of the queue of ALL
	// packets that have been sent to
	// the miniport, it is also the head
	// of the packets that have been sent
	// down to the miniport by the wrapper.
	PNDIS_PACKET				LastPacket;			// This is tail pointer for the global
	// packet queue and this is the tail
	// pointer to the queue of packets
	// waiting to be sent to the miniport.
	PNDIS_PACKET				FirstPendingPacket; // This is head of the queue of packets
	// waiting to be sent to miniport.
	PNDIS_PACKET				LastMiniportPacket; // This is the tail pointer of the
	// queue of packets that have been
	// sent to the miniport by the wrapper.

	PNDIS_PACKET				LoopbackHead;		// Head of loopback queue.
	PNDIS_PACKET				LoopbackTail;		// Tail of loopback queue.

	ULONG						SendResourcesAvailable;
	PPNDIS_PACKET				PacketArray;
	UINT						MaximumSendPackets;

	//
	// Transfer data information
	//
	PNDIS_PACKET				FirstTDPacket;
	PNDIS_PACKET				LastTDPacket;
	PNDIS_PACKET				LoopbackPacket;

	//
	// Reset information
	//
	NDIS_STATUS					ResetStatus;

	//
	// RequestInformation
	//
	PNDIS_REQUEST				PendingRequest;
	PNDIS_REQUEST				MiniportRequest;
	NDIS_STATUS					RequestStatus;
	UINT						MaximumLongAddresses;
	UINT						MaximumShortAddresses;
	UINT						CurrentLookahead;
	UINT						MaximumLookahead;
	UINT						MacOptions;

	KEVENT						RequestEvent;

	UCHAR						MulticastBuffer[NDIS_M_MAX_MULTI_LIST][6];

	//
	// Temp stuff for using the old NDIS functions
	//
	ULONG						ChannelNumber;

	UINT						NumberOfAllocatedWorkItems;

	PNDIS_LOG					Log;

	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST				CallMgrAfList;

	//
	// Store information here to track adapters
	//
	ULONG                       BusId;
	ULONG                       SlotNumber;

	//
	//	Pointer to the current thread. Used in layered miniport code.
	//
	LONG						MiniportThread;
	LONG						SendThread;

	//
	//	To handle packets returned during RcvIndication
	//
	WORK_QUEUE_ITEM				WorkItem;
	PNDIS_PACKET				ReturnPacketsQueue;

	//
	// Context associated with the intermediate driver
	//
	NDIS_HANDLE					DeviceContext;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	UNICODE_STRING				BaseName;
}NDIS_MINIPORT_BLOCK_NT,*PNDIS_MINIPORT_BLOCK_NT;

//
// one of these per mini-port registered on a Driver
//
typedef struct _NDIS_MINIPORT_BLOCK_W2K
{
	ULONG						NullValue;			// used to distinquish between MACs and mini-ports
	PNDIS_MINIPORT_BLOCK		NextMiniport;		// used by driver's MiniportQueue
	PNDIS_M_DRIVER_BLOCK		DriverHandle;		// pointer to our Driver block
	NDIS_HANDLE					MiniportAdapterContext; // context when calling mini-port functions
	UNICODE_STRING				MiniportName;		// how mini-port refers to us
	PNDIS_BIND_PATHS			BindPaths;
	NDIS_HANDLE					OpenQueue;			// queue of opens for this mini-port
	REFERENCE					ShortRef;		    // contains spinlock for OpenQueue

	NDIS_HANDLE					DeviceContext;		// Context associated with the intermediate driver

	UCHAR						Padding1;			// DO NOT REMOVE OR NDIS WILL BREAK!!!

	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the same time.
	//
	UCHAR						LockAcquired;		// EXPOSED via macros. Do not move

	UCHAR						PmodeOpens;			// Count of opens which turned on pmode/all_local

	//
	//	This is the processor number that the miniport's
	//	interrupt DPC and timers are running on.
	//
	UCHAR						AssignedProcessor;

	KSPIN_LOCK					Lock;

	PNDIS_REQUEST				MediaRequest;

	PNDIS_MINIPORT_INTERRUPT	Interrupt;

	ULONG						Flags;				// Flags to keep track of the
	// miniport's state.
	ULONG						PnPFlags;

	//
	// Send information
	//
	LIST_ENTRY					PacketList;
	PNDIS_PACKET				FirstPendingPacket; // This is head of the queue of packets
	// waiting to be sent to miniport.
	PNDIS_PACKET				ReturnPacketsQueue;

	//
	// Space used for temp. use during request processing
	//
	ULONG						RequestBuffer;
	PVOID						SetMCastBuffer;

	PNDIS_MINIPORT_BLOCK		PrimaryMiniport;
	PVOID						WrapperContext;

	//
	// context to pass to bus driver when reading or writing config space
	//
	PVOID						BusDataContext;
	//
	// flag to specify PnP capabilities of the device. we need this to fail query_stop
	// query_remove or suspend request if the device can not handle it
	//
	ULONG						PnPCapabilities;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST			Resources;

	//
	// Watch-dog timer
	//
	NDIS_TIMER					WakeUpDpcTimer;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	// Note:
	// the following two fields should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	UNICODE_STRING				BaseName;
	UNICODE_STRING				SymbolicLinkName;

	//
	// Check for hang stuff
	//
	ULONG						CheckForHangSeconds;
	USHORT						CFHangTicks;
	USHORT						CFHangCurrentTick;

	//
	// Reset information
	//
	NDIS_STATUS					ResetStatus;
	NDIS_HANDLE					ResetOpen;

	//
	// Holds media specific information.
	//
#ifdef __cplusplus
	FILTERDBS					FilterDbs;			// EXPOSED via macros. Do not move
#else
	FILTERDBS;										// EXPOSED via macros. Do not move
#endif

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER	SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER	SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER	ResetCompleteHandler;

	NDIS_MEDIUM					MediaType;

	//
	// contains mini-port information
	//
	ULONG						BusNumber;
	NDIS_INTERFACE_TYPE			BusType;
	NDIS_INTERFACE_TYPE			AdapterType;

	PDEVICE_OBJECT				DeviceObject;
	PDEVICE_OBJECT				PhysicalDeviceObject;
	PDEVICE_OBJECT				NextDeviceObject;

	//
	// Holds the map registers for this mini-port.
	//
	PMAP_REGISTER_ENTRY			MapRegisters;	// EXPOSED via macros. Do not move

	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST				CallMgrAfList;

	PVOID						MiniportThread;
	PVOID						SetInfoBuf;
	USHORT						SetInfoBufLen;
	USHORT						MaxSendPackets;

	//
	//	Status code that is returned from the fake handlers.
	//
	NDIS_STATUS					FakeStatus;

	PVOID						LockHandler;		// For the filter lock

	//
	// the following field should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	PUNICODE_STRING				pAdapterInstanceName;	//	Instance specific name for the adapter.

	PADAPTER_OBJECT				SystemAdapterObject;

	UINT						MacOptions;

	//
	// RequestInformation
	//
	PNDIS_REQUEST				PendingRequest;
	UINT						MaximumLongAddresses;
	UINT						MaximumShortAddresses;
	UINT						CurrentLookahead;
	UINT						MaximumLookahead;

	//
	//	For efficiency
	//
	W_HANDLE_INTERRUPT_HANDLER	HandleInterruptHandler;
	W_DISABLE_INTERRUPT_HANDLER	DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER	EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER		SendPacketsHandler;
	NDIS_M_START_SENDS			DeferredSendHandler;

	//
	// The following cannot be unionized.
	//
	ETH_RCV_INDICATE_HANDLER	EthRxIndicateHandler;	// EXPOSED via macros. Do not move
	TR_RCV_INDICATE_HANDLER		TrRxIndicateHandler;	// EXPOSED via macros. Do not move
	FDDI_RCV_INDICATE_HANDLER	FddiRxIndicateHandler;	// EXPOSED via macros. Do not move

	ETH_RCV_COMPLETE_HANDLER	EthRxCompleteHandler;	// EXPOSED via macros. Do not move
	TR_RCV_COMPLETE_HANDLER		TrRxCompleteHandler;	// EXPOSED via macros. Do not move
	FDDI_RCV_COMPLETE_HANDLER	FddiRxCompleteHandler;	// EXPOSED via macros. Do not move

	NDIS_M_STATUS_HANDLER		StatusHandler;			// EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER	StatusCompleteHandler;	// EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER	TDCompleteHandler;		// EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER	QueryCompleteHandler;	// EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER	SetCompleteHandler;		// EXPOSED via macros. Do not move

	NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;// EXPOSED via macros. Do not move
	WAN_RCV_HANDLER				WanRcvHandler;			// EXPOSED via macros. Do not move
	WAN_RCV_COMPLETE_HANDLER	WanRcvCompleteHandler;	// EXPOSED via macros. Do not move

	/********************************************************************************************/
	/****************                                                                  **********/
	/**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
	/**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
	/****************                                                                  **********/
	/********************************************************************************************/

	//
	// Work that the miniport needs to do.
	//
	SINGLE_LIST_ENTRY			WorkQueue[NUMBER_OF_WORK_ITEM_TYPES];
	SINGLE_LIST_ENTRY			SingleWorkItems[NUMBER_OF_SINGLE_WORK_ITEMS];

	PNDIS_MAC_BLOCK				FakeMac;

	UCHAR						SendFlags;
	UCHAR						TrResetRing;
	UCHAR						ArcnetAddress;

	union
	{
		PNDIS_ARC_BUF			ArcBuf;
		//
		// the following fiels has a different use under NT and Memphis
		//
#if NDIS_NT
		PVOID					BusInterface;
#else
		PVOID					PhysicalAddressArray;
#endif
	};

	//
	// Temp stuff for using the old NDIS functions
	//
	ULONG						ChannelNumber;

	PNDIS_LOG					Log;

	//
	// Store information here to track adapters
	//
	ULONG						BusId;
	ULONG						SlotNumber;

	PCM_RESOURCE_LIST			AllocatedResources;
	PCM_RESOURCE_LIST			AllocatedResourcesTranslated;

	//
	//	Contains a list of the packet patterns that have been added to the
	//	adapter.
	//
	SINGLE_LIST_ENTRY			PatternList;

	//
	//	The driver's power management capabilities.
	//
	NDIS_PNP_CAPABILITIES		PMCapabilities;

	//
	// DeviceCapabilites as received from bus driver
	//
	DEVICE_CAPABILITIES			DeviceCaps;

	//
	//	Contains the wake-up events that are enabled for the miniport.
	//
	ULONG						WakeUpEnable;

	//
	//	The current device state that the adapter is in.
	//
	DEVICE_POWER_STATE			CurrentDeviceState;

	//
	//	The following IRP is created in response to a cable disconnect
	//	from the device.  We keep a pointer around in case we need to cancel
	//	it.
	//
	PIRP						pIrpWaitWake;

	SYSTEM_POWER_STATE			WaitWakeSystemState;

	//
	//	The following is a pointer to a dynamically allocated array
	//	of GUID structs. This is used to map GUIDs to OIDs
	//	for custom GUIDs provided by the miniport.
	//

	LARGE_INTEGER				VcIndex;				//	Index used to identify a VC.
	KSPIN_LOCK					VcCountLock;			//	Lock used to protect VC instance count.
	LIST_ENTRY					WmiEnabledVcs;			//	List of WMI enabled VCs
	PNDIS_GUID					pNdisGuidMap;			// This is a list of all the GUIDs
	//  and OIDs supported including any
	//	customg GUIDs.
	PNDIS_GUID					pCustomGuidMap;			// This is a pointer into
	//	the pGuidToOidMap to the
	//	first custom GUID.
	USHORT						VcCount;				//	Number of VC's that have instance names.

	USHORT						cNdisGuidMap;			// This is the number of std. GUIDs
	USHORT						cCustomGuidMap;			// This is the number of custom GUIDs

	//
	// These two are used temporarily while allocating the map registers.
	//
	USHORT						CurrentMapRegister;
	PKEVENT						AllocationEvent;

	USHORT						PhysicalMapRegistersNeeded;
	USHORT                      SGMapRegistersNeeded;
	ULONG						MaximumPhysicalMapping;

	//
	// This timer is used for media disconnect timouts.
	//
	NDIS_TIMER					MediaDisconnectTimer;

	//
	// The timeout value for media disconnect timer to fire
	// default is 20 seconds
	//
	USHORT						MediaDisconnectTimeOut;

	//
	// Used for WMI support
	//
	USHORT						InstanceNumber;

	//
	// this event will be set at the end of adapter initialization
	//
	NDIS_EVENT					OpenReadyEvent;

	//
	// current PnP state of the device, ex. started, stopped, query_removed, etc.
	//
	NDIS_PNP_DEVICE_STATE		PnPDeviceState;

	//
	// previous device state. to be used when we get a cancel_remove or a cancel_stop
	//
	NDIS_PNP_DEVICE_STATE		OldPnPDeviceState;

	//
	// Handlers to Write/Read Bus data
	//
	PGET_SET_DEVICE_DATA 		SetBusData;
	PGET_SET_DEVICE_DATA 		GetBusData;

	POID_LIST					OidList;

	KDPC						DeferredDpc;

	//
	// Some NDIS gathered stats
	//
	NDIS_STATS					NdisStats;

	//
	// Valid during Packet Indication
	//
	PNDIS_PACKET				IndicatedPacket[MAXIMUM_PROCESSORS];

	//
	// this event is for protecting against returning from REMOVE IRP
	// too early and while we still have pending workitems
	//
	PKEVENT						RemoveReadyEvent;

	//
	// this event gets signaled when all opens on the miniport are closed
	//
	PKEVENT						AllOpensClosedEvent;

	//
	// this event gets signaled when all requests on the miniport are gone
	//
	PKEVENT						AllRequestsCompletedEvent;

	//
	// Init time for the miniport in milliseconds
	//
	ULONG						InitTimeMs;

	NDIS_MINIPORT_WORK_ITEM		WorkItemBuffer[NUMBER_OF_SINGLE_WORK_ITEMS];
	PNDIS_MINIPORT_TIMER		TimerQueue;

	//
	// flags to fail certain NDIS APIs to make sure the driver does the right things
	//
	ULONG						DriverVerifyFlags;

	//
	// used to queue miniport on global miniport queue
	//
	PNDIS_MINIPORT_BLOCK		NextGlobalMiniport;

	//
	// InternalResetCount:	The # of times NDIS decided a miniport was hung
	// MiniportResetCount	The # of times miniport decided it was hung
	//
	USHORT						InternalResetCount;
	USHORT						MiniportResetCount;

	USHORT						MediaSenseConnectCount;
	USHORT						MediaSenseDisconnectCount;

	PNDIS_PACKET	*			xPackets;

	//
	// track the user mode requests
	//
	ULONG						UserModeOpenReferences;

#if	LOCK_DBG
	ULONG						LockDbg;
	ULONG						LockDbgX;
	PVOID						LockThread;
#endif

	PSECURITY_DESCRIPTOR        SecurityDescriptor;
	//
	// both these variables are protected by Ref->SpinLock
	//
	ULONG                       NumUserOpens;   // number of non-admin open handles
	ULONG                       NumAdminOpens;  // number of admin open handles
	ULONG_REFERENCE             Ref;

#if !(NDIS_NT)
	PNDIS_PACKET				LoopbackHead;
	PNDIS_PACKET				LoopbackTail;
#endif
}NDIS_MINIPORT_BLOCK_W2K,*PNDIS_MINIPORT_BLOCK_W2K;

typedef struct _NDIS_MINIPORT_BLOCK_WXP
{
	PVOID                       Signature;          // MINIPORT_DEVICE_MAGIC_VALUE
	PNDIS_MINIPORT_BLOCK        NextMiniport;       // used by driver's MiniportQueue
	PNDIS_M_DRIVER_BLOCK        DriverHandle;       // pointer to our Driver block
	NDIS_HANDLE                 MiniportAdapterContext; // context when calling mini-port functions
	UNICODE_STRING              MiniportName;       // how mini-port refers to us
	PNDIS_BIND_PATHS            BindPaths;
	NDIS_HANDLE                 OpenQueue;          // queue of opens for this mini-port
	REFERENCE                   ShortRef;           // contains spinlock for OpenQueue

	NDIS_HANDLE                 DeviceContext;      // Context associated with the intermediate driver

	UCHAR                       Padding1;           // DO NOT REMOVE OR NDIS WILL BREAK!!!

	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the same time.
	//
	UCHAR                       LockAcquired;       // EXPOSED via macros. Do not move

	UCHAR                       PmodeOpens;         // Count of opens which turned on pmode/all_local

	//
	//  This is the processor number that the miniport's
	//  interrupt DPC and timers are running on.
	//
	UCHAR                       AssignedProcessor;

	KSPIN_LOCK                  Lock;

	PNDIS_REQUEST               MediaRequest;

	PNDIS_MINIPORT_INTERRUPT    Interrupt;

	ULONG                       Flags;              // Flags to keep track of the
	// miniport's state.
	ULONG                       PnPFlags;

	//
	// Send information
	//
	LIST_ENTRY                  PacketList;
	PNDIS_PACKET                FirstPendingPacket; // This is head of the queue of packets
	// waiting to be sent to miniport.
	PNDIS_PACKET                ReturnPacketsQueue;

	//
	// Space used for temp. use during request processing
	//
	ULONG                       RequestBuffer;
	PVOID                       SetMCastBuffer;

	PNDIS_MINIPORT_BLOCK        PrimaryMiniport;
	PVOID                       WrapperContext;

	//
	// context to pass to bus driver when reading or writing config space
	//
	PVOID                       BusDataContext;
	//
	// flag to specify PnP capabilities of the device. we need this to fail query_stop
	// query_remove or suspend request if the device can not handle it
	//
	ULONG                       PnPCapabilities;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST           Resources;

	//
	// Watch-dog timer
	//
	NDIS_TIMER                  WakeUpDpcTimer;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	// Note:
	// the following two fields should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	UNICODE_STRING              BaseName;
	UNICODE_STRING              SymbolicLinkName;

	//
	// Check for hang stuff
	//
	ULONG                       CheckForHangSeconds;
	USHORT                      CFHangTicks;
	USHORT                      CFHangCurrentTick;

	//
	// Reset information
	//
	NDIS_STATUS                 ResetStatus;
	NDIS_HANDLE                 ResetOpen;

	//
	// Holds media specific information.
	//
#ifdef __cplusplus
	FILTERDBS                   FilterDbs;          // EXPOSED via macros. Do not move
#else
	FILTERDBS;                                      // EXPOSED via macros. Do not move
#endif

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER    SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER   SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER   ResetCompleteHandler;

	NDIS_MEDIUM                 MediaType;

	//
	// contains mini-port information
	//
	ULONG                       BusNumber;
	NDIS_INTERFACE_TYPE         BusType;
	NDIS_INTERFACE_TYPE         AdapterType;

	PDEVICE_OBJECT              DeviceObject;
	PDEVICE_OBJECT              PhysicalDeviceObject;
	PDEVICE_OBJECT              NextDeviceObject;

	//
	// Holds the map registers for this mini-port.
	//
	PMAP_REGISTER_ENTRY         MapRegisters;   // EXPOSED via macros. Do not move

	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST               CallMgrAfList;

	PVOID                       MiniportThread;
	PVOID                       SetInfoBuf;
	USHORT                      SetInfoBufLen;
	USHORT                      MaxSendPackets;

	//
	//  Status code that is returned from the fake handlers.
	//
	NDIS_STATUS                 FakeStatus;

	PVOID                       LockHandler;        // For the filter lock

	//
	// the following field should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	PUNICODE_STRING             pAdapterInstanceName;   //  Instance specific name for the adapter.

	PNDIS_MINIPORT_TIMER        TimerQueue;

	UINT                        MacOptions;

	//
	// RequestInformation
	//
	PNDIS_REQUEST               PendingRequest;
	UINT                        MaximumLongAddresses;
	UINT                        MaximumShortAddresses;
	UINT                        CurrentLookahead;
	UINT                        MaximumLookahead;

	//
	//  For efficiency
	//
	W_HANDLE_INTERRUPT_HANDLER  HandleInterruptHandler;
	W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER      SendPacketsHandler;
	NDIS_M_START_SENDS          DeferredSendHandler;

	//
	// The following cannot be unionized.
	//
	ETH_RCV_INDICATE_HANDLER    EthRxIndicateHandler;   // EXPOSED via macros. Do not move
	TR_RCV_INDICATE_HANDLER     TrRxIndicateHandler;    // EXPOSED via macros. Do not move
	FDDI_RCV_INDICATE_HANDLER   FddiRxIndicateHandler;  // EXPOSED via macros. Do not move

	ETH_RCV_COMPLETE_HANDLER    EthRxCompleteHandler;   // EXPOSED via macros. Do not move
	TR_RCV_COMPLETE_HANDLER     TrRxCompleteHandler;    // EXPOSED via macros. Do not move
	FDDI_RCV_COMPLETE_HANDLER   FddiRxCompleteHandler;  // EXPOSED via macros. Do not move

	NDIS_M_STATUS_HANDLER       StatusHandler;          // EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER StatusCompleteHandler;  // EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;      // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER QueryCompleteHandler;   // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER SetCompleteHandler;     // EXPOSED via macros. Do not move

	NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;// EXPOSED via macros. Do not move
	WAN_RCV_HANDLER             WanRcvHandler;          // EXPOSED via macros. Do not move
	WAN_RCV_COMPLETE_HANDLER    WanRcvCompleteHandler;  // EXPOSED via macros. Do not move

	/********************************************************************************************/
	/****************                                                                  **********/
	/**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
	/**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
	/****************                                                                  **********/
	/********************************************************************************************/

	PNDIS_MINIPORT_BLOCK        NextGlobalMiniport;     // used to queue miniport on global miniport queue

	//
	// Work that the miniport needs to do.
	//
	SINGLE_LIST_ENTRY           WorkQueue[NUMBER_OF_WORK_ITEM_TYPES];
	SINGLE_LIST_ENTRY           SingleWorkItems[NUMBER_OF_SINGLE_WORK_ITEMS];

	UCHAR                       SendFlags;
	UCHAR                       TrResetRing;
	UCHAR                       ArcnetAddress;
	UCHAR                       XState;     // flag to indicate why we have set the fake handlers

	union
	{
#if ARCNET
		PNDIS_ARC_BUF           ArcBuf;
#endif
		//
		// the following field has a different use under NT and Memphis
		//
		PVOID                   BusInterface;
	};

	PNDIS_LOG                   Log;

	//
	// Store information here to track adapters
	//
	ULONG                       SlotNumber;

	PCM_RESOURCE_LIST           AllocatedResources;
	PCM_RESOURCE_LIST           AllocatedResourcesTranslated;

	//
	//  Contains a list of the packet patterns that have been added to the
	//  adapter.
	//
	SINGLE_LIST_ENTRY           PatternList;

	//
	//  The driver's power management capabilities.
	//
	NDIS_PNP_CAPABILITIES       PMCapabilities;

	//
	// DeviceCapabilites as received from bus driver
	//
	DEVICE_CAPABILITIES         DeviceCaps;

	//
	//  Contains the wake-up events that are enabled for the miniport.
	//
	ULONG                       WakeUpEnable;

	//
	//  The current device state that the adapter is in.
	//
	DEVICE_POWER_STATE          CurrentDevicePowerState;

	//
	//  The following IRP is created in response to a cable disconnect
	//  from the device.  We keep a pointer around in case we need to cancel
	//  it.
	//
	PIRP                        pIrpWaitWake;

	SYSTEM_POWER_STATE          WaitWakeSystemState;

	//
	//  The following is a pointer to a dynamically allocated array
	//  of GUID structs. This is used to map GUIDs to OIDs
	//  for custom GUIDs provided by the miniport.
	//

	LARGE_INTEGER               VcIndex;                //  Index used to identify a VC.
	KSPIN_LOCK                  VcCountLock;            //  Lock used to protect VC instance count.
	LIST_ENTRY                  WmiEnabledVcs;          //  List of WMI enabled VCs
	PNDIS_GUID                  pNdisGuidMap;           // This is a list of all the GUIDs
	//  and OIDs supported including any
	//  customg GUIDs.
	PNDIS_GUID                  pCustomGuidMap;         // This is a pointer into
	//  the pGuidToOidMap to the
	//  first custom GUID.
	USHORT                      VcCount;                //  Number of VC's that have instance names.

	USHORT                      cNdisGuidMap;           // This is the number of std. GUIDs
	USHORT                      cCustomGuidMap;         // This is the number of custom GUIDs

	//
	// These two are used temporarily while allocating the map registers.
	//
	USHORT                      CurrentMapRegister;
	PKEVENT                     AllocationEvent;

	USHORT                      BaseMapRegistersNeeded;
	USHORT                      SGMapRegistersNeeded;
	ULONG                       MaximumPhysicalMapping;

	//
	// This timer is used for media disconnect timouts.
	//
	NDIS_TIMER                  MediaDisconnectTimer;

	//
	// The timeout value for media disconnect timer to fire
	// default is 20 seconds
	//
	USHORT                      MediaDisconnectTimeOut;

	//
	// Used for WMI support
	//
	USHORT                      InstanceNumber;

	//
	// this event will be set at the end of adapter initialization
	//
	NDIS_EVENT                  OpenReadyEvent;

	//
	// current PnP state of the device, ex. started, stopped, query_removed, etc.
	//
	NDIS_PNP_DEVICE_STATE       PnPDeviceState;

	//
	// previous device state. to be used when we get a cancel_remove or a cancel_stop
	//
	NDIS_PNP_DEVICE_STATE       OldPnPDeviceState;

	//
	// Handlers to Write/Read Bus data
	//
	PGET_SET_DEVICE_DATA        SetBusData;
	PGET_SET_DEVICE_DATA        GetBusData;

	KDPC                        DeferredDpc;

	//
	// Some NDIS gathered stats
	//
	NDIS_STATS                  NdisStats;

	//
	// Valid during Packet Indication
	//
	PNDIS_PACKET                IndicatedPacket[MAXIMUM_PROCESSORS];

	//
	// this event is for protecting against returning from REMOVE IRP
	// too early and while we still have pending workitems
	//
	PKEVENT                     RemoveReadyEvent;

	//
	// this event gets signaled when all opens on the miniport are closed
	//
	PKEVENT                     AllOpensClosedEvent;

	//
	// this event gets signaled when all requests on the miniport are gone
	//
	PKEVENT                     AllRequestsCompletedEvent;

	//
	// Init time for the miniport in milliseconds
	//
	ULONG                       InitTimeMs;

	NDIS_MINIPORT_WORK_ITEM     WorkItemBuffer[NUMBER_OF_SINGLE_WORK_ITEMS];

	PDMA_ADAPTER                SystemAdapterObject;

	//
	// flags to fail certain NDIS APIs to make sure the driver does the right things
	//
	ULONG                       DriverVerifyFlags;

	POID_LIST                   OidList;
	//
	// InternalResetCount:  The # of times NDIS decided a miniport was hung
	// MiniportResetCount   The # of times miniport decided it was hung
	//
	USHORT                      InternalResetCount;
	USHORT                      MiniportResetCount;

	USHORT                      MediaSenseConnectCount;
	USHORT                      MediaSenseDisconnectCount;

	PNDIS_PACKET    *           xPackets;

	//
	// track the user mode requests
	//
	ULONG                       UserModeOpenReferences;

	//
	// Saved handlers. The handlers in the NDIS_OPEN_BLOCK are saved here
	// by ndisMSwapOpenHandlers and restored by ndisMRestoreOpenHandlers.
	//
	union
	{
		PVOID                   SavedSendHandler;
		PVOID                   SavedWanSendHandler;
	};
	PVOID                       SavedSendPacketsHandler;
	PVOID                       SavedCancelSendPacketsHandler;

	//
	// real SendPacketsHandler
	//
	W_SEND_PACKETS_HANDLER      WSendPacketsHandler;                

	ULONG                       MiniportAttributes;

	PDMA_ADAPTER                SavedSystemAdapterObject;
	USHORT                      NumOpens;
	//
	// extra check for hang ticks allowed for OID_GEN_MEDIA_CONNECT_STATUS and OID_GEN_LINK_SPEED
	//
	USHORT                      CFHangXTicks; 
	ULONG                       RequestCount;
	ULONG                       IndicatedPacketsCount;
	ULONG                       PhysicalMediumType;
	//
	// last request. needed for debugging purpose
	//
	PNDIS_REQUEST               LastRequest;
	LONG                        DmaAdapterRefCount;
	PVOID                       FakeMac;
	ULONG                       LockDbg;
	ULONG                       LockDbgX;
	PVOID                       LockThread;
	ULONG                       InfoFlags;
	KSPIN_LOCK                  TimerQueueLock;
	PKEVENT                     ResetCompletedEvent;
	PKEVENT                     QueuedBindingCompletedEvent;
	PKEVENT                     DmaResourcesReleasedEvent;
	FILTER_PACKET_INDICATION_HANDLER SavedPacketIndicateHandler;
	ULONG                       RegisteredInterrupts;

	PNPAGED_LOOKASIDE_LIST      SGListLookasideList;
	ULONG                       ScatterGatherListSize;

	PKEVENT                     WakeUpTimerEvent;
	PSECURITY_DESCRIPTOR        SecurityDescriptor;
	//
	// both these variables are protected by Ref->SpinLock
	//
	ULONG                       NumUserOpens;   // number of non-admin open handles
	ULONG                       NumAdminOpens;  // number of admin open handles
	ULONG_REFERENCE             Ref;
}NDIS_MINIPORT_BLOCK_WXP,*PNDIS_MINIPORT_BLOCK_WXP;

typedef struct _NDIS_MINIPORT_BLOCK_WNET
{
	PVOID                       Signature;          // MINIPORT_DEVICE_MAGIC_VALUE
	PNDIS_MINIPORT_BLOCK        NextMiniport;       // used by driver's MiniportQueue
	PNDIS_M_DRIVER_BLOCK        DriverHandle;       // pointer to our Driver block
	NDIS_HANDLE                 MiniportAdapterContext; // context when calling mini-port functions
	UNICODE_STRING              MiniportName;       // how mini-port refers to us
	PNDIS_BIND_PATHS            BindPaths;
	NDIS_HANDLE                 OpenQueue;          // queue of opens for this mini-port
	REFERENCE                   ShortRef;           // contains spinlock for OpenQueue

	NDIS_HANDLE                 DeviceContext;      // Context associated with the intermediate driver

	UCHAR                       Padding1;           // DO NOT REMOVE OR NDIS WILL BREAK!!!

	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the same time.
	//
	UCHAR                       LockAcquired;       // EXPOSED via macros. Do not move

	UCHAR                       PmodeOpens;         // Count of opens which turned on pmode/all_local

	//
	//  This is the processor number that the miniport's
	//  interrupt DPC and timers are running on.
	//
	UCHAR                       AssignedProcessor;

	KSPIN_LOCK                  Lock;

	PNDIS_REQUEST               MediaRequest;

	PNDIS_MINIPORT_INTERRUPT    Interrupt;

	ULONG                       Flags;              // Flags to keep track of the
	// miniport's state.
	ULONG                       PnPFlags;

	//
	// Send information
	//
	LIST_ENTRY                  PacketList;
	PNDIS_PACKET                FirstPendingPacket; // This is head of the queue of packets
	// waiting to be sent to miniport.
	PNDIS_PACKET                ReturnPacketsQueue;

	//
	// Space used for temp. use during request processing
	//
	ULONG                       RequestBuffer;
	PVOID                       SetMCastBuffer;

	PNDIS_MINIPORT_BLOCK        PrimaryMiniport;
	PVOID                       WrapperContext;

	//
	// context to pass to bus driver when reading or writing config space
	//
	PVOID                       BusDataContext;
	//
	// flag to specify PnP capabilities of the device. we need this to fail query_stop
	// query_remove or suspend request if the device can not handle it
	//
	ULONG                       PnPCapabilities;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST           Resources;

	//
	// Watch-dog timer
	//
	NDIS_TIMER                  WakeUpDpcTimer;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	// Note:
	// the following two fields should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	UNICODE_STRING              BaseName;
	UNICODE_STRING              SymbolicLinkName;

	//
	// Check for hang stuff
	//
	ULONG                       CheckForHangSeconds;
	USHORT                      CFHangTicks;
	USHORT                      CFHangCurrentTick;

	//
	// Reset information
	//
	NDIS_STATUS                 ResetStatus;
	NDIS_HANDLE                 ResetOpen;

	//
	// Holds media specific information.
	//
#ifdef __cplusplus
	FILTERDBS                   FilterDbs;          // EXPOSED via macros. Do not move
#else
	FILTERDBS;                                      // EXPOSED via macros. Do not move
#endif

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER    SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER   SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER   ResetCompleteHandler;

	NDIS_MEDIUM                 MediaType;

	//
	// contains mini-port information
	//
	ULONG                       BusNumber;
	NDIS_INTERFACE_TYPE         BusType;
	NDIS_INTERFACE_TYPE         AdapterType;

	PDEVICE_OBJECT              DeviceObject;
	PDEVICE_OBJECT              PhysicalDeviceObject;
	PDEVICE_OBJECT              NextDeviceObject;

	//
	// Holds the map registers for this mini-port.
	//
	struct _MAP_REGISTER_ENTRY *MapRegisters;   // EXPOSED via macros. Do not move

	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST               CallMgrAfList;

	PVOID                       MiniportThread;
	PVOID                       SetInfoBuf;
	USHORT                      SetInfoBufLen;
	USHORT                      MaxSendPackets;

	//
	//  Status code that is returned from the fake handlers.
	//
	NDIS_STATUS                 FakeStatus;

	PVOID                       LockHandler;        // For the filter lock

	//
	// the following field should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	PUNICODE_STRING             pAdapterInstanceName;   //  Instance specific name for the adapter.

	PNDIS_MINIPORT_TIMER        TimerQueue;

	UINT                        MacOptions;

	//
	// RequestInformation
	//
	PNDIS_REQUEST               PendingRequest;
	UINT                        MaximumLongAddresses;
	UINT                        MaximumShortAddresses;
	UINT                        CurrentLookahead;
	UINT                        MaximumLookahead;

	//
	//  For efficiency
	//
	W_HANDLE_INTERRUPT_HANDLER  HandleInterruptHandler;
	W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER      SendPacketsHandler;
	NDIS_M_START_SENDS          DeferredSendHandler;

	//
	// The following cannot be unionized.
	//
	ETH_RCV_INDICATE_HANDLER    EthRxIndicateHandler;   // EXPOSED via macros. Do not move
	TR_RCV_INDICATE_HANDLER     TrRxIndicateHandler;    // EXPOSED via macros. Do not move
	FDDI_RCV_INDICATE_HANDLER   FddiRxIndicateHandler;  // EXPOSED via macros. Do not move

	ETH_RCV_COMPLETE_HANDLER    EthRxCompleteHandler;   // EXPOSED via macros. Do not move
	TR_RCV_COMPLETE_HANDLER     TrRxCompleteHandler;    // EXPOSED via macros. Do not move
	FDDI_RCV_COMPLETE_HANDLER   FddiRxCompleteHandler;  // EXPOSED via macros. Do not move

	NDIS_M_STATUS_HANDLER       StatusHandler;          // EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER StatusCompleteHandler;  // EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;      // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER QueryCompleteHandler;   // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER SetCompleteHandler;     // EXPOSED via macros. Do not move

	NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;// EXPOSED via macros. Do not move
	WAN_RCV_HANDLER             WanRcvHandler;          // EXPOSED via macros. Do not move
	WAN_RCV_COMPLETE_HANDLER    WanRcvCompleteHandler;  // EXPOSED via macros. Do not move

	/********************************************************************************************/
	/****************                                                                  **********/
	/**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
	/**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
	/****************                                                                  **********/
	/********************************************************************************************/
	PNDIS_SG_DMA_BLOCK          MiniportSGDmaBlock;
	PNDIS_MINIPORT_BLOCK        NextGlobalMiniport;     // used to queue miniport on global miniport queue

	//
	// Work that the miniport needs to do.
	//
	SINGLE_LIST_ENTRY           WorkQueue[NUMBER_OF_WORK_ITEM_TYPES];
	SINGLE_LIST_ENTRY           SingleWorkItems[NUMBER_OF_SINGLE_WORK_ITEMS];

	UCHAR                       SendFlags;
	UCHAR                       TrResetRing;
	UCHAR                       ArcnetAddress;
	UCHAR                       XState;     // flag to indicate why we have set the fake handlers

	union
	{
#if ARCNET
		PNDIS_ARC_BUF           ArcBuf;
#endif
		//
		// the following field has a different use under NT and Memphis
		//
		PVOID                   BusInterface;
	};

	PNDIS_LOG                   Log;

	//
	// Store information here to track adapters
	//
	ULONG                       SlotNumber;

	PCM_RESOURCE_LIST           AllocatedResources;
	PCM_RESOURCE_LIST           AllocatedResourcesTranslated;

	//
	//  Contains a list of the packet patterns that have been added to the
	//  adapter.
	//
	SINGLE_LIST_ENTRY           PatternList;

	//
	//  The driver's power management capabilities.
	//
	NDIS_PNP_CAPABILITIES       PMCapabilities;

	//
	// DeviceCapabilites as received from bus driver
	//
	DEVICE_CAPABILITIES         DeviceCaps;

	//
	//  Contains the wake-up events that are enabled for the miniport.
	//
	ULONG                       WakeUpEnable;

	//
	//  The current device state that the adapter is in.
	//
	DEVICE_POWER_STATE          CurrentDevicePowerState;

	//
	//  The following IRP is created in response to a cable disconnect
	//  from the device.  We keep a pointer around in case we need to cancel
	//  it.
	//
	PIRP                        pIrpWaitWake;

	SYSTEM_POWER_STATE          WaitWakeSystemState;

	//
	//  The following is a pointer to a dynamically allocated array
	//  of GUID structs. This is used to map GUIDs to OIDs
	//  for custom GUIDs provided by the miniport.
	//

	LARGE_INTEGER               VcIndex;                //  Index used to identify a VC.
	KSPIN_LOCK                  VcCountLock;            //  Lock used to protect VC instance count.
	LIST_ENTRY                  WmiEnabledVcs;          //  List of WMI enabled VCs
	PNDIS_GUID                  pNdisGuidMap;           // This is a list of all the GUIDs
	//  and OIDs supported including any
	//  customg GUIDs.
	PNDIS_GUID                  pCustomGuidMap;         // This is a pointer into
	//  the pGuidToOidMap to the
	//  first custom GUID.
	USHORT                      VcCount;                //  Number of VC's that have instance names.

	USHORT                      cNdisGuidMap;           // This is the number of std. GUIDs
	USHORT                      cCustomGuidMap;         // This is the number of custom GUIDs

	//
	// These two are used temporarily while allocating the map registers.
	//
	USHORT                      CurrentMapRegister;
	PKEVENT                     AllocationEvent;

	USHORT                      BaseMapRegistersNeeded;
	USHORT                      SGMapRegistersNeeded;
	ULONG                       MaximumPhysicalMapping;

	//
	// This timer is used for media disconnect timouts.
	//
	NDIS_TIMER                  MediaDisconnectTimer;

	//
	// The timeout value for media disconnect timer to fire
	// default is 20 seconds
	//
	USHORT                      MediaDisconnectTimeOut;

	//
	// Used for WMI support
	//
	USHORT                      InstanceNumber;

	//
	// this event will be set at the end of adapter initialization
	//
	NDIS_EVENT                  OpenReadyEvent;

	//
	// current PnP state of the device, ex. started, stopped, query_removed, etc.
	//
	NDIS_PNP_DEVICE_STATE       PnPDeviceState;

	//
	// previous device state. to be used when we get a cancel_remove or a cancel_stop
	//
	NDIS_PNP_DEVICE_STATE       OldPnPDeviceState;

	//
	// Handlers to Write/Read Bus data
	//
	PGET_SET_DEVICE_DATA        SetBusData;
	PGET_SET_DEVICE_DATA        GetBusData;

	KDPC                        DeferredDpc;

	//
	// Some NDIS gathered stats
	//
	NDIS_STATS                  NdisStats;

	//
	// Valid during Packet Indication
	//
	PNDIS_PACKET                IndicatedPacket[MAXIMUM_PROCESSORS];

	//
	// this event is for protecting against returning from REMOVE IRP
	// too early and while we still have pending workitems
	//
	PKEVENT                     RemoveReadyEvent;

	//
	// this event gets signaled when all opens on the miniport are closed
	//
	PKEVENT                     AllOpensClosedEvent;

	//
	// this event gets signaled when all requests on the miniport are gone
	//
	PKEVENT                     AllRequestsCompletedEvent;

	//
	// Init time for the miniport in milliseconds
	//
	ULONG                       InitTimeMs;

	NDIS_MINIPORT_WORK_ITEM     WorkItemBuffer[NUMBER_OF_SINGLE_WORK_ITEMS];

	PDMA_ADAPTER                SystemAdapterObject;

	//
	// flags to fail certain NDIS APIs to make sure the driver does the right things
	//
	ULONG                       DriverVerifyFlags;

	POID_LIST                   OidList;
	//
	// InternalResetCount:  The # of times NDIS decided a miniport was hung
	// MiniportResetCount   The # of times miniport decided it was hung
	//
	USHORT                      InternalResetCount;
	USHORT                      MiniportResetCount;

	USHORT                      MediaSenseConnectCount;
	USHORT                      MediaSenseDisconnectCount;

	PNDIS_PACKET    *           xPackets;

	//
	// track the user mode requests
	//
	ULONG                       UserModeOpenReferences;

	//
	// Saved handlers. The handlers in the NDIS_OPEN_BLOCK are saved here
	// by ndisMSwapOpenHandlers and restored by ndisMRestoreOpenHandlers.
	//
	union
	{
		PVOID                   SavedSendHandler;
		PVOID                   SavedWanSendHandler;
	};
	W_SEND_PACKETS_HANDLER      SavedSendPacketsHandler;
	W_CANCEL_SEND_PACKETS_HANDLER SavedCancelSendPacketsHandler;

	//
	// real SendPacketsHandler
	//
	W_SEND_PACKETS_HANDLER      WSendPacketsHandler;                

	ULONG                       MiniportAttributes;

	PDMA_ADAPTER                SavedSystemAdapterObject;
	USHORT                      NumOpens;
	//
	// extra check for hang ticks allowed for OID_GEN_MEDIA_CONNECT_STATUS and OID_GEN_LINK_SPEED
	//
	USHORT                      CFHangXTicks; 
	ULONG                       RequestCount;
	ULONG                       IndicatedPacketsCount;
	ULONG                       PhysicalMediumType;
	//
	// last request. needed for debugging purpose
	//
	PNDIS_REQUEST               LastRequest;
	LONG                        DmaAdapterRefCount;
	PVOID                       FakeMac;
	ULONG                       LockDbg;
	ULONG                       LockDbgX;
	PVOID                       LockThread;
	ULONG                       InfoFlags;
	KSPIN_LOCK                  TimerQueueLock;
	PKEVENT                     ResetCompletedEvent;
	PKEVENT                     QueuedBindingCompletedEvent;
	PKEVENT                     DmaResourcesReleasedEvent;
	FILTER_PACKET_INDICATION_HANDLER SavedPacketIndicateHandler;
	ULONG                       RegisteredInterrupts;

	PNPAGED_LOOKASIDE_LIST      SGListLookasideList;
	ULONG                       ScatterGatherListSize;
	PKEVENT                     WakeUpTimerEvent;

	PSECURITY_DESCRIPTOR        SecurityDescriptor;
	//
	// both these variables are protected by Ref->SpinLock
	//
	ULONG                       NumUserOpens;   // number of non-admin open handles
	ULONG                       NumAdminOpens;  // number of admin open handles
	ULONG_REFERENCE             Ref;

	NDIS_RECEIVE_SCALE_CAPABILITIES RecvScaleCapabilities;
	LONG PagingPathCount;
	UNICODE_STRING DevinterfaceNetSymbolicLinkName;

}NDIS_MINIPORT_BLOCK_WNET,*PNDIS_MINIPORT_BLOCK_WNET;

typedef enum _NDIS_MINIPORT_STATE
{
	NdisMiniportUndefindState = 0x0,
	NdisMiniportHalted = 0x1,
	NdisMiniportInitializing = 0x2,
	NdisMiniportRestarting = 0x3,
	NdisMiniportRunning = 0x4,
	NdisMiniportPausing = 0x5,
	NdisMiniportPaused = 0x6
}NDIS_MINIPORT_STATE,*PNDIS_MINIPORT_STATE;

typedef enum _NDIS_FILTER_STATE
{
	NdisFilterDetached = 0x0,
	NdisFilterAttaching = 0x1,
	NdisFilterPaused = 0x2,
	NdisFilterRestarting = 0x3,
	NdisFilterRunning = 0x4,
	NdisFilterPausing = 0x5,
	NdisFilterDetaching = 0x6
}NDIS_FILTER_STATE,*PNDIS_FILTER_STATE;

//
// NDIS_PM_PARAMETERS structure is used in OID_PM_PARAMETERS for quering and 
// updating currently enabled power management hardware capabilities.
//
#define NDIS_PM_PARAMETERS_REVISION_1              1

typedef struct _NDIS_PM_PARAMETERS
{
	NDIS_OBJECT_HEADER      Header;
	ULONG                   EnabledWoLPacketPatterns; // NDIS_PM_WOL_XXX flags
	ULONG                   EnabledProtocolOffloads;  // NDIS_PM_PROTOCOL_OFFLOAD_XXX flags    
	ULONG                   WakeUpFlags;              // NDIS_PM_WAKE_XXX flags

} NDIS_PM_PARAMETERS, *PNDIS_PM_PARAMETERS;

#define NDIS_SIZEOF_NDIS_PM_PARAMETERS_REVISION_1     \
	RTL_SIZEOF_THROUGH_FIELD(NDIS_PM_PARAMETERS, WakeUpFlags)

typedef struct _NDIS_PM_PARAMETERS_WIN8
{
	//
	// Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	// Header.Revision = NDIS_PM_PARAMETERS_REVISION_2;
	// Header.Size = NDIS_SIZEOF_NDIS_PM_PARAMETERS_REVISION_2;
	//
	NDIS_OBJECT_HEADER      Header;

	ULONG                   EnabledWoLPacketPatterns; // NDIS_PM_WOL_XXX_ENABLED flags
	ULONG                   EnabledProtocolOffloads;  // NDIS_PM_PROTOCOL_OFFLOAD_XXX_ENABLED flags
	ULONG                   WakeUpFlags;              // NDIS_PM_WAKE_ON_XXX_ENABLED flags

	ULONG                   MediaSpecificWakeUpEvents; // NDIS_{WLAN|WWAN}_WAKE_ON_XXX_ENABLED flags

} NDIS_PM_PARAMETERS_WIN8, *PNDIS_PM_PARAMETERS_WIN8;

#define NDIS_SIZEOF_NDIS_PM_PARAMETERS_REVISION_2     \
	RTL_SIZEOF_THROUGH_FIELD(NDIS_PM_PARAMETERS, MediaSpecificWakeUpEvents)

//
// NDIS_PM_CAPABILITIES structure is used in PowerManagementCapabilitiesEx field of 
// NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES and NDIS_BIND_PARAMETERS
//
#define NDIS_PM_CAPABILITIES_REVISION_1              1

typedef struct _NDIS_PM_CAPABILITIES
{
	NDIS_OBJECT_HEADER      Header;
	ULONG                   Flags;
	ULONG                   SupportedWoLPacketPatterns;
	ULONG                   NumTotalWoLPatterns;
	ULONG                   MaxWoLPatternSize;              // maximum bytes that can be compared against a pattern
	ULONG                   MaxWoLPatternOffset;            // strting from MAC header, how many bytes in the packet can be examined
	ULONG                   MaxWoLPacketSaveBuffer;         // how many bytes of WOL packet can be saved to a buffer and indicated up
	ULONG                   SupportedProtocolOffloads;
	ULONG                   NumArpOffloadIPv4Addresses;
	ULONG                   NumNSOffloadIPv6Addresses;
	NDIS_DEVICE_POWER_STATE MinMagicPacketWakeUp;
	NDIS_DEVICE_POWER_STATE MinPatternWakeUp;
	NDIS_DEVICE_POWER_STATE MinLinkChangeWakeUp;
}NDIS_PM_CAPABILITIES, *PNDIS_PM_CAPABILITIES;

#define NDIS_SIZEOF_NDIS_PM_CAPABILITIES_REVISION_1     \
	RTL_SIZEOF_THROUGH_FIELD(NDIS_PM_CAPABILITIES, MinLinkChangeWakeUp)

typedef struct _NDIS_PM_CAPABILITIES_WIN8
{
	NDIS_OBJECT_HEADER      Header;
	ULONG                   Flags;
	ULONG                   SupportedWoLPacketPatterns;
	ULONG                   NumTotalWoLPatterns;
	ULONG                   MaxWoLPatternSize;              // maximum bytes that can be compared against a pattern
	ULONG                   MaxWoLPatternOffset;            // strting from MAC header, how many bytes in the packet can be examined
	ULONG                   MaxWoLPacketSaveBuffer;         // how many bytes of WOL packet can be saved to a buffer and indicated up
	ULONG                   SupportedProtocolOffloads;
	ULONG                   NumArpOffloadIPv4Addresses;
	ULONG                   NumNSOffloadIPv6Addresses;
	NDIS_DEVICE_POWER_STATE MinMagicPacketWakeUp;
	NDIS_DEVICE_POWER_STATE MinPatternWakeUp;
	NDIS_DEVICE_POWER_STATE MinLinkChangeWakeUp;

	ULONG                   SupportedWakeUpEvents;          // NDIS_PM_WAKE_ON_XXX_SUPPORTED flags
	ULONG                   MediaSpecificWakeUpEvents;      // NDIS_{WLAN|WWAN}_WAKE_ON_XXX_SUPPORTED flags

}NDIS_PM_CAPABILITIES_WIN8, *PNDIS_PM_CAPABILITIES_WIN8;

#define NDIS_SIZEOF_NDIS_PM_CAPABILITIES_REVISION_2     \
	RTL_SIZEOF_THROUGH_FIELD(NDIS_PM_CAPABILITIES, MediaSpecificWakeUpEvents)

//
// Data structures for advertising generic filtering capabilities
// in ReceiveFilterCapabilities field of NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES 
// structure, OID_RECEIVE_FILTER_HARDWARE_CAPABILITIES  and
// OID_RECEIVE_FILTER_CURRENT_CAPABILITIES query OIDs
//
#define NDIS_RECEIVE_FILTER_CAPABILITIES_REVISION_1     1
typedef struct _NDIS_RECEIVE_FILTER_CAPABILITIES
{
	__in  NDIS_OBJECT_HEADER          Header;
	__in  ULONG                       Flags;
	__in  ULONG                       EnabledFilterTypes;
	__in  ULONG                       EnabledQueueTypes;
	__in  ULONG                       NumQueues;
	__in  ULONG                       SupportedQueueProperties;
	__in  ULONG                       SupportedFilterTests;
	__in  ULONG                       SupportedHeaders;
	__in  ULONG                       SupportedMacHeaderFields;
	__in  ULONG                       MaxMacHeaderFilters;
	__in  ULONG                       MaxQueueGroups;
	__in  ULONG                       MaxQueuesPerQueueGroup;
	__in  ULONG                       MinLookaheadSplitSize;
	__in  ULONG                       MaxLookaheadSplitSize;
} NDIS_RECEIVE_FILTER_CAPABILITIES, *PNDIS_RECEIVE_FILTER_CAPABILITIES;

#define NDIS_SIZEOF_RECEIVE_FILTER_CAPABILITIES_REVISION_1     \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_RECEIVE_FILTER_CAPABILITIES, MaxLookaheadSplitSize)

//
// Data structure for advertising the NIC VMQ capabilities
// Used in HardwareNicSwitchCapabilities and CurrentNicSwitchCapabilities
// fields of NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES,
// OID_NIC_SWITCH_HARDWARE_CAPABILITIES and OID_NIC_SWITCH_CURRENT_CAPABILITIES
//
#define NDIS_NIC_SWITCH_CAPABILITIES_REVISION_1        1
typedef struct _NDIS_NIC_SWITCH_CAPABILITIES
{
	__in  NDIS_OBJECT_HEADER          Header;
	__in  ULONG                       Flags;
	__in  ULONG                       NdisReserved1;
	__in  ULONG                       NumTotalMacAddresses;
	__in  ULONG                       NumMacAddressesPerPort;
	__in  ULONG                       NumVlansPerPort;
	__in  ULONG                       NdisReserved2;
	__in  ULONG                       NdisReserved3;
}NDIS_NIC_SWITCH_CAPABILITIES, *PNDIS_NIC_SWITCH_CAPABILITIES;

#define NDIS_SIZEOF_NIC_SWITCH_CAPABILITIES_REVISION_1     \
	RTL_SIZEOF_THROUGH_FIELD(NDIS_NIC_SWITCH_CAPABILITIES, NdisReserved3)

typedef struct _NDIS_FILTER_BLOCK NDIS_FILTER_BLOCK,*PNDIS_FILTER_BLOCK;
typedef struct _NDIS_FILTER_BLOCK
{
	NDIS_OBJECT_HEADER Header;
	PNDIS_FILTER_BLOCK NextFilter;
	PVOID FilterDriver;
	PVOID FilterModuleContext;
	PNDIS_MINIPORT_BLOCK Miniport;
	PUNICODE_STRING FilterInstanceName;
	PUNICODE_STRING FilterFriendlyName;
	ULONG Flags;
	NDIS_FILTER_STATE State;
	REFERENCE Ref;
	LONG FakeStatus;
	PNDIS_FILTER_BLOCK NextGlobalFilter;
	PNDIS_FILTER_BLOCK LowerFilter;
	PNDIS_FILTER_BLOCK HigherFilter;
	PVOID AsyncOpContext;
	LONG NumOfPauseRestartRequests;
	ULONG64 Lock;
	PVOID LockThread;
	ULONG LockDbg;
	LIST_ENTRY OidRequestList;
	PNDIS_OID_REQUEST PendingOidRequest;
	REFERENCE PnPRef;
	NDIS_MEDIUM MediaType;
	NDIS_PHYSICAL_MEDIUM PhysicalMediaType;
	NET_IF_MEDIA_CONNECT_STATE MediaConnectState;
	NET_IF_MEDIA_DUPLEX_STATE MediaDuplexState;
	ULONG64 XmitLinkSpeed;
	ULONG64 RcvLinkSpeed;
	NDIS_SUPPORTED_PAUSE_FUNCTIONS PauseFunctions;
	ULONG AutoNegotiationFlags;
	UCHAR XState;
	UCHAR Reserved1;
	UCHAR Reserved2;
	UCHAR Reserved3;
	NET_IF_MEDIA_CONNECT_STATE MediaConnectStateIndicateUp;
	NET_IF_MEDIA_DUPLEX_STATE MediaDuplexStateIndicateUp;
	ULONG64 XmitLinkSpeedIndicateUp;
	ULONG64 RcvLinkSpeedIndicateUp;
	NDIS_SUPPORTED_PAUSE_FUNCTIONS PauseFunctionsIndicateUp;
	ULONG AutoNegotiationFlagsIndicateUp;
	PVOID NextRequestHandle;
	PVOID NextSendNetBufferListsHandler;
	PVOID NextSendNetBufferListsContext;
	PVOID NextSendNetBufferListsObject;
	PVOID NextSendNetBufferListsCompleteHandler;
	PVOID NextSendNetBufferListsCompleteContext;
	PVOID NextSendNetBufferListsCompleteObject;
	PVOID NextIndicateReceiveNetBufferListsHandler;
	PVOID NextIndicateReceiveNetBufferListsContext;
	PVOID NextIndicateReceiveNetBufferListsObject;
	PVOID NextReturnNetBufferListsHandler;
	PVOID NextReturnNetBufferListsContext;
	PVOID NextReturnNetBufferListsObject;
	PVOID NextIndicateStatusHandle;
	PVOID NextDevicePnPEventNotifyHandle;
	PVOID NextNetPnPEventHandle;
	PVOID NextCancelSendNetBufferListsHandler;
	PVOID NextCancelSendNetBufferListsContext;
	PVOID SetFilterModuleOptionalHandlers;
	NDIS_FILTER_PARTIAL_CHARACTERISTICS Characteristics;
	NDIS_FILTER_PARTIAL_CHARACTERISTICS SavedCharacteristics;
	PVOID OidRequestHandler;
	PVOID OidRequestCompleteHandler;
	PVOID CancelRequestHandler;
	PVOID DevicePnPEventNotifyHandler;
	PVOID NetPnPEventHandler;
	PVOID StatusHandler;
	PVOID FilterSendNetBufferListsHandler;
	PVOID FilterIndicateReceiveNetBufferListsHandler;
	PVOID FilterCancelSendNetBufferListsHandler;
	PVOID InitiateOffloadCompleteHandler;
	PVOID TerminateOffloadCompleteHandler;
	PVOID UpdateOffloadCompleteHandler;
	PVOID InvalidateOffloadCompleteHandler;
	PVOID QueryOffloadCompleteHandler;
	PVOID IndicateOffloadEventHandler;
	PVOID TcpOffloadSendCompleteHandler;
	PVOID TcpOffloadReceiveCompleteHandler;
	PVOID TcpOffloadDisconnectCompleteHandler;
	PVOID TcpOffloadForwardCompleteHandler;
	PVOID TcpOffloadEventHandler;
	PVOID TcpOffloadReceiveIndicateHandler;
	PVOID InitiateOffloadHandler;
	PVOID TerminateOffloadHandler;
	PVOID UpdateOffloadHandler;
	PVOID InvalidateOffloadHandler;
	PVOID QueryOffloadHandler;
	PVOID TcpOffloadSendHandler;
	PVOID TcpOffloadReceiveHandler;
	PVOID TcpOffloadDisconnectHandler;
	PVOID TcpOffloadForwardHandler;
	PVOID TcpOffloadReceiveReturnHandler;
	PVOID Offload;
	GUID InterfaceGuid;
	ULONG IfIndex;
	PVOID IfBlock;
	PVOID DirectOidRequestHandler;
	PVOID DirectOidRequestCompleteHandler;
	PVOID NextDirectRequestHandle;
	ULONG DirectOidRequestCount;
	PVOID CancelDirectOidRequestHandler;
	SINGLE_LIST_ENTRY WOLPatternList;
	SINGLE_LIST_ENTRY PMProtocolOffloadList;
	NDIS_PM_PARAMETERS PMCurrentParameters;
}NDIS_FILTER_BLOCK,*PNDIS_FILTER_BLOCK;

typedef struct _NDIS_MINIPORT_HANDLERS_WIN6 {

	PVOID RequestContext;
	PVOID CancelSendContext;
	PVOID IndicateNetBufferListsContext;
	PVOID SaveIndicateNetBufferListsContext;
	PVOID ReturnNetBufferListsContext;
	PVOID SendNetBufferListsContext;
	PVOID SendNetBufferListsObject;
	PVOID SendNetBufferListsCompleteContext;
	PVOID RequestHandle;
	PVOID StatusHandle;
	PVOID DevicePnPEventNotifyHandle;
	PVOID NetPnPEventHandle;
	union{
		PVOID CancelSendHandler;
		PVOID CancelSendPacketsHandler;
	};
	PVOID SendNetBufferListsCompleteHandler;
	PVOID IndicateNetBufferListsHandler;
	PVOID SaveIndicateNetBufferListsHandler;
	PVOID ReturnNetBufferListsHandler;
	PVOID SendNetBufferListsHandler;

}NDIS_MINIPORT_HANDLERS_WIN6,*PNDIS_MINIPORT_HANDLERS_WIN6;

typedef struct _NDIS_MINIPORT_HANDLERS_WIN6_SP1 {

	PVOID RequestContext;
	PVOID CancelSendContext;
	PVOID IndicateNetBufferListsContext;
	PVOID SaveIndicateNetBufferListsContext;
	PVOID ReturnNetBufferListsContext;
	PVOID SendNetBufferListsContext;
	PVOID SendNetBufferListsObject;
	PVOID SendNetBufferListsCompleteContext;
	PVOID RequestHandle;
	PVOID StatusHandle;
	PVOID DevicePnPEventNotifyHandle;
	PVOID NetPnPEventHandle;
	union{
		PVOID CancelSendHandler;
		PVOID CancelSendPacketsHandler;
	};
	PVOID SendNetBufferListsCompleteHandler;
	PVOID IndicateNetBufferListsHandler;
	PVOID SaveIndicateNetBufferListsHandler;
	PVOID ReturnNetBufferListsHandler;
	PVOID SendNetBufferListsHandler;
	PVOID DirectRequestHandle;

}NDIS_MINIPORT_HANDLERS_WIN6_SP1,*PNDIS_MINIPORT_HANDLERS_WIN6_SP1;

typedef struct _NDIS_MINIPORT_HANDLERS_WIN7
{
	PVOID RequestContext;
	PVOID CancelSendContext;
	PVOID IndicateNetBufferListsContext;
	PVOID IndicateNetBufferListsObject;
	PVOID SaveIndicateNetBufferListsContext;
	PVOID ReturnNetBufferListsContext;
	PVOID ReturnNetBufferListsObject;
	PVOID SendNetBufferListsContext;
	PVOID SendNetBufferListsObject;
	PVOID SendNetBufferListsCompleteContext;
	PVOID SendNetBufferListsCompleteObject;
	PVOID RequestHandle;
	PVOID StatusHandle;
	PVOID DevicePnPEventNotifyHandle;
	PVOID NetPnPEventHandle;
	union{
		PVOID CancelSendHandler;
		PVOID CancelSendPacketsHandler;
	};
	PVOID SendNetBufferListsCompleteHandler;
	PVOID IndicateNetBufferListsHandler;
	PVOID SaveIndicateNetBufferListsHandler;
	PVOID ReturnNetBufferListsHandler;
	PVOID SendNetBufferListsHandler;
	PVOID DirectRequestHandle;
}NDIS_MINIPORT_HANDLERS_WIN7,*PNDIS_MINIPORT_HANDLERS_WIN7;


typedef struct _NDIS_MINIPORT_HANDLERS_WIN8
{
	PVOID RequestContext;
	PVOID CancelSendContext;
	PVOID IndicateNetBufferListsContext;
	PVOID IndicateNetBufferListsTracker;
	PVOID SaveIndicateNetBufferListsContext;
	PVOID ReturnNetBufferListsContext;
	PVOID ReturnNetBufferListsTracker;
	PVOID SendNetBufferListsContext;
	PVOID SendNetBufferListsTracker;
	PVOID SendNetBufferListsObject;
	PVOID SendNetBufferListsCompleteContext;
	PVOID SendNetBufferListsCompleteTracker;
	PVOID RequestHandle;

	union{
		PVOID CancelSendHandler;
		PVOID CancelSendPacketsHandler;
	};
	PVOID SendNetBufferListsCompleteHandler;
	PVOID IndicateNetBufferListsHandler;
	PVOID SaveIndicateNetBufferListsHandler;
	PVOID ReturnNetBufferListsHandler;
	PVOID SendNetBufferListsHandler;
	PVOID DirectRequestHandle;
}NDIS_MINIPORT_HANDLERS_WIN8,*PNDIS_MINIPORT_HANDLERS_WIN8;

typedef enum _NDIS_M_PERIODIC_RECEIVE_STATE
{
	PeriodicReceivesOff = 0x0,
	PeriodicReceivesOn = 0x1,
	PeriodicReceivesBlocked = 0x2
}NDIS_M_PERIODIC_RECEIVE_STATE,*PNDIS_M_PERIODIC_RECEIVE_STATE;

typedef struct _NDIS_M_PERIODIC_RECEIVES_WIN6 {

	PNDIS_MINIPORT_BLOCK NextMiniportBlock;
	NDIS_M_PERIODIC_RECEIVE_STATE State;
	KSPIN_LOCK SpinLock;
	PVOID LockThread;
	ULONG LockDbg;
	LONG NumMQueuedNbls;
	LONG NumNblsDequeued;
	PNET_BUFFER_LIST QueuedHead;
	PNET_BUFFER_LIST QueuedTail;
	PNET_BUFFER_LIST IndicateHead;
	PNET_BUFFER_LIST IndicateTail;
	ULONG BoundToIP;
	UCHAR Paused;
	LONG NblsAllocated;
	ULONG TrackingQueued;
	ULONG TrackingDequeued;
	ULONG TrackingResources;
	ULONG TrackingPending;
	ULONG TrackingIndicated;
	ULONG TrackingEmptied;
}NDIS_M_PERIODIC_RECEIVES_WIN6,*PNDIS_M_PERIODIC_RECEIVES_WIN6;


typedef struct _NDIS_M_PERIODIC_RECEIVES_WIN7
{
	PNDIS_MINIPORT_BLOCK NextMiniportBlock;
	NDIS_M_PERIODIC_RECEIVE_STATE State;
	KSPIN_LOCK SpinLock;
	PVOID LockThread;
	ULONG LockDbg;
	LONG NumMQueuedNbls;
	LONG NumNblsDequeued;
	PNET_BUFFER_LIST QueuedHead;
	PNET_BUFFER_LIST QueuedTail;
	ULONG BoundToIP;
	UCHAR Paused;
	LONG NblsAllocated;
	ULONG TrackingQueued;
	ULONG TrackingDequeued;
	ULONG TrackingResources;
	ULONG TrackingPending;
	ULONG TrackingIndicated;
	ULONG TrackingEmptied;
	WORK_QUEUE_ITEM WorkItem;
	LONG WorkItemQueued;
}NDIS_M_PERIODIC_RECEIVES_WIN7,*PNDIS_M_PERIODIC_RECEIVES_WIN7;

typedef struct _NDIS_M_PERIODIC_RECEIVES_WIN7 NDIS_M_PERIODIC_RECEIVES_WIN8,*PNDIS_M_PERIODIC_RECEIVES_WIN8;

typedef struct {
	PCI_CAPABILITIES_HEADER         Header;
	struct _PCI_MSI_MESSAGE_CONTROL {
		USHORT MSIEnable  :1;
		USHORT MultipleMessageCapable  :3;
		USHORT MultipleMessageEnable  :3;
		USHORT CapableOf64Bits  :1;
		USHORT PerVectorMaskCapable  :1;
		USHORT Reserved  :7;
	} MessageControl;
	union {
		struct _PCI_MSI_MESSAGE_ADDRESS {
			ULONG Reserved  :2;
			ULONG Address  :30;
		} Register;
		ULONG                           Raw;
	} MessageAddressLower;
	union {
		struct {
			USHORT MessageData;
		} Option32Bit;
		struct {
			ULONG  MessageAddressUpper;
			USHORT MessageData;
			USHORT Reserved;
			ULONG  MaskBits;
			ULONG  PendingBits;
		} Option64Bit;
	};
} PCI_MSI_CAPABILITY, *PPCI_MSI_CAPABILITY;

typedef struct {
	union {
		struct {
			ULONG BaseIndexRegister  :3;
			ULONG Reserved  :29;
		};
		ULONG  TableOffset;
	};
} PCIX_TABLE_POINTER, *PPCIX_TABLE_POINTER;

typedef struct {
	PCI_CAPABILITIES_HEADER Header;
	struct {
		USHORT TableSize  :11;
		USHORT Reserved  :3;
		USHORT FunctionMask  :1;
		USHORT MSIXEnable  :1;
	} MessageControl;
	PCIX_TABLE_POINTER      MessageTable;
	PCIX_TABLE_POINTER      PBATable;
} PCI_MSIX_CAPABILITY, *PPCI_MSIX_CAPABILITY;

typedef union _NDIS_MINIPORT_OFFLOAD_REG
{
	struct{
		ULONG IPv4XSumTxDisabled : 1;
		ULONG IPv4XSumRxDisabled : 1;
		ULONG TCPv4XSumTxDisabled : 1;
		ULONG TCPv4XSumRxDisabled : 1;
		ULONG UDPv4XSumTxDisabled : 1;
		ULONG UDPv4XSumRxDisabled : 1;
		ULONG TCPv6XSumTxDisabled : 1;
		ULONG TCPv6XSumRxDisabled : 1;
		ULONG UDPv6XSumTxDisabled : 1;
		ULONG UDPv6XSumRxDisabled : 1;
		ULONG LsoV1IPv4Disabled : 1;
		ULONG LsoV2IPv4Disabled : 1;
		ULONG LsoV2IPv6Disabled : 1;
		ULONG IPsecAHDisabled  : 1;
		ULONG IPsecESPDisabled : 1;
		ULONG TCPv4ConnectionOffloadDisabled : 1;
		ULONG TCPv6ConnectionOffloadDisabled : 1;
		ULONG ReadRegistry     : 1;
		ULONG IPsecV2AHDisabled : 1;
		ULONG IPsecV2ESPDisabled : 1;
		ULONG IPsecV2IPv4AHDisabled : 1;
		ULONG IPsecV2IPv4ESPDisabled : 1;
		ULONG IPSecV2IPv6Disabled : 1;
	};
	ULONG Value;
}NDIS_MINIPORT_OFFLOAD_REG,*PNDIS_MINIPORT_OFFLOAD_REG;

typedef union _NDIS_PM_ADMIN_GONFIG
{
	struct{
		ULONG WakeOnPattern : 2;
		ULONG WakeOnMagicPacket : 2;
		ULONG DeviceSleepOnDisconnect : 2;
		ULONG PMARPOffload : 2;
		ULONG PMNSOffload : 2;
		ULONG PMWiFiRekeyOffload : 2;
	};
	ULONG Value;
}NDIS_PM_ADMIN_GONFIG,*PNDIS_PM_ADMIN_GONFIG;

typedef struct _IO_REMOVE_LOCK_NODBG {
	IO_REMOVE_LOCK_COMMON_BLOCK Common;
} IO_REMOVE_LOCK_NODBG, *PIO_REMOVE_LOCK_NODBG;

typedef struct _NDIS_INTERRUPT_BLOCK NDIS_INTERRUPT_BLOCK,*PNDIS_INTERRUPT_BLOCK;
typedef VOID (NTAPI *FUNC)(void);

typedef struct _NDIS_PCI_DEVICE_CUSTOM_PROPERTIES_WIN6
{
	NDIS_OBJECT_HEADER              Header;
	UINT32                          DeviceType;             // conventional, PCI-X, PCI-E, etc.
	UINT32                          CurrentSpeedAndMode;    // PCI_DEVICE_CONVENTIONAL_xx or PCIX_MODExxx. valid only for conventional and PCI-X devices
	UINT32                          CurrentPayloadSize;     // PCI_EXPRESS_MAX_PAYLOAD_SIZE, valid only for PCI-E devices
	UINT32                          MaxPayloadSize;         // PCI_EXPRESS_MAX_PAYLOAD_SIZE, valid only for PCI-E devices
	UINT32                          MaxReadRequestSize;     // PCI_EXPRESS_MAX_PAYLOAD_SIZE, valid only for PCI-E devices
	UINT32                          CurrentLinkSpeed;       // PCI_EXPRESS_LINK_SPEED_xxx. valid only for PCI-E devices 
	UINT32                          CurrentLinkWidth;       // PCI_EXPRESS_LINK_WIDTH_xx. valid only for PCI-E devices
	UINT32                          MaxLinkSpeed;           // PCI_EXPRESS_LINK_SPEED_xxx. valid only for PCI-E devices
	UINT32                          MaxLinkWidth;           // PCI_EXPRESS_LINK_WIDTH_xx. valid only for PCI-E devices
} NDIS_PCI_DEVICE_CUSTOM_PROPERTIES_WIN6, *PNDIS_PCI_DEVICE_CUSTOM_PROPERTIES_WIN6;

// windows vista

typedef struct _NDIS_MINIPORT_BLOCK_WIN6
{
	NDIS_OBJECT_HEADER          Header;
	PNDIS_MINIPORT_BLOCK        NextMiniport;       // used by driver's MiniportQueue
	PNDIS_MINIPORT_BLOCK        BaseMiniport;
	NDIS_HANDLE                 MiniportAdapterContext; // context when calling mini-port functions
	union{
		UNICODE_STRING              Reserved4;
		struct
		{
			UCHAR MajorNdisVersion;
			UCHAR MinorNdisVersion;
			PVOID PerProcCounters;
		};		
	};
	union
	{
		ULONG EnabledPerformanceCounters;
		PVOID pvoidEnabledPerformanceCounters;
	};
	NDIS_HANDLE                 OpenQueue;          // queue of opens for this mini-port
	REFERENCE                   ShortRef;           // contains spinlock for OpenQueue

	union
	{
		ULONG EnabledPerformanceCountersEx;
		PVOID pvoidEnabledPerformanceCountersEx;
	};

	UCHAR                       LinkStateIndicationFlags;           // DO NOT REMOVE OR NDIS WILL BREAK!!!

	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the same time.
	//
	UCHAR                       LockAcquired;       // EXPOSED via macros. Do not move

	UCHAR                       PmodeOpens;         // Count of opens which turned on pmode/all_local

	//
	//  This is the processor number that the miniport's
	//  interrupt DPC and timers are running on.
	//
	//  Note: This field is no longer used
	//
	UCHAR                       Reserved23;

	KSPIN_LOCK                  Lock;

	PNDIS_REQUEST               MediaRequest;

	PNDIS_MINIPORT_INTERRUPT    Interrupt;

	ULONG                       Flags;              // Flags to keep track of the miniport's state.
	ULONG                       PnPFlags;

	//
	// Send information
	//
	LIST_ENTRY                  PacketList;
	PNDIS_PACKET                FirstPendingPacket; // This is head of the queue of packets
	// waiting to be sent to miniport.
	PNDIS_PACKET                ReturnPacketsQueue;

	//
	// Space used for temp. use during request processing
	//
	ULONG                       RequestBuffer;
	PVOID                       SetMCastBuffer;

	PNDIS_MINIPORT_BLOCK        PrimaryMiniport;
	FUNC                        NextCancelSendNetBufferListsHandler;

	//
	// context to pass to bus driver when reading or writing config space
	//
	PVOID                       OidContext;
	ULONG                       SupportedOidListLength;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST           Resources;

	//
	// Watch-dog timer
	//
	NDIS_TIMER                  WakeUpDpcTimer;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	// Note:
	// the following two fields should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	union{
		UNICODE_STRING              Reserved20;
		struct
		{
			NET_IF_MEDIA_CONNECT_STATE MiniportMediaConnectState;
			NET_IF_MEDIA_DUPLEX_STATE MiniportMediaDuplexState;
		};		
	};
	UNICODE_STRING              SymbolicLinkName;

	//
	// Check for hang stuff
	//
	ULONG                       CheckForHangSeconds;
	USHORT                      CFHangTicks;
	USHORT                      CFHangCurrentTick;

	//
	// Reset information
	//
	NDIS_STATUS                 ResetStatus;
	NDIS_HANDLE                 ResetOpen;

	//
	// Holds media specific information.
	//
#ifdef __cplusplus
	FILTERDBS                   FilterDbs;          // EXPOSED via macros. Do not move
#else
	FILTERDBS;                                      // EXPOSED via macros. Do not move
#endif

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER    SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER   SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER   ResetCompleteHandler;

	NDIS_MEDIUM                 MediaType;

	//
	// contains mini-port information
	//
	ULONG                       AutoNegotiationFlags;
	NDIS_INTERFACE_TYPE         Reserved5;
	NDIS_INTERFACE_TYPE         AdapterType;

	union{
		PDEVICE_OBJECT              Reserved6;
		NET_IF_MEDIA_CONNECT_STATE	MediaConnectState;
	};
	union{
		PDEVICE_OBJECT              Reserved7;
		NET_IF_MEDIA_DUPLEX_STATE	MediaDuplexState;
	};
	PULONG                      SupportedOidList;


	PNDIS_SG_DMA_BLOCK          MiniportSGDmaBlock;

	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST               CallMgrAfList;

	PVOID                       MiniportThread;
	PVOID                       SetInfoBuf;
	USHORT                      SetInfoBufLen;
	USHORT                      MaxSendPackets;

	//
	//  Status code that is returned from the fake handlers.
	//
	NDIS_STATUS                 FakeStatus;

	PVOID                       Reserved24;        // For the filter lock

	PNDIS_RECEIVE_SCALE_PARAMETERS CombinedNdisRSSParameters;
	union{
		PNDIS_MINIPORT_TIMER TimerQueue;
		PVOID TimerObjectQueue;
	};
	UINT                        MacOptions;

	//
	// RequestInformation
	//
	PNDIS_REQUEST               PendingRequest;
	UINT                        MaximumLongAddresses;
	UINT                        MaximumShortAddresses;
	UINT                        MiniportCurrentLookahead;
	UINT                        MiniportMaximumLookahead;

	//
	//  For efficiency
	//
	union{
		ULONG_PTR                   Reserved1;
		PX_FILTER                   NullMediaFilter;
	};
	W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER      SendPacketsHandler;
	NDIS_M_START_SENDS          DeferredSendHandler;

	//
	// The following cannot be unionized.
	//
	ETH_RCV_INDICATE_HANDLER    EthRxIndicateHandler;   // EXPOSED via macros. Do not move
	TR_RCV_INDICATE_HANDLER     TrRxIndicateHandler;    // EXPOSED via macros. Do not move
	FUNC                        NextSendNetBufferListsHandler;

	ETH_RCV_COMPLETE_HANDLER    EthRxCompleteHandler;   // EXPOSED via macros. Do not move
	TR_RCV_COMPLETE_HANDLER     TrRxCompleteHandler;    // EXPOSED via macros. Do not move
	FUNC                        SavedNextSendNetBufferListsHandler;

	NDIS_M_STATUS_HANDLER       StatusHandler;          // EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER StatusCompleteHandler;  // EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;      // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER QueryCompleteHandler;   // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER SetCompleteHandler;     // EXPOSED via macros. Do not move

	NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;// EXPOSED via macros. Do not move
	WAN_RCV_HANDLER             WanRcvHandler;          // EXPOSED via macros. Do not move
	WAN_RCV_COMPLETE_HANDLER    WanRcvCompleteHandler;  // EXPOSED via macros. Do not move

	/********************************************************************************************/
	/****************                                                                  **********/
	/**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
	/**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
	/****************                                                                  **********/
	/********************************************************************************************/

	FUNC  IndicateNetBufferListsHandler;
	PVOID IndicateNetBufferListsContext;
	FUNC  SendNetBufferListsCompleteHandler;
	FUNC  NextReturnNetBufferLists;
	PVOID NextReturnNetBufferListsContext;

	UINT64 XmitLinkSpeed;
	UINT64 RcvLinkSpeed;
	NDIS_SUPPORTED_PAUSE_FUNCTIONS PauseFunctions;

	NDIS_SUPPORTED_PAUSE_FUNCTIONS MiniportPauseFunctions;
	ULONG MiniportAutoNegotiationFlags;
	SINGLE_LIST_ENTRY WorkQueue[7];
	SINGLE_LIST_ENTRY SingleWorkItems[6];
	UCHAR SendFlags;
	UCHAR TrResetRing;
	UCHAR MP6SupportPM;
	UCHAR XState;
	PNDIS_LOG Log;
	PCM_RESOURCE_LIST AllocatedResources;
	PCM_RESOURCE_LIST AllocatedResourcesTranslated;
	SINGLE_LIST_ENTRY PatternList;

	NDIS_PNP_CAPABILITIES PMCapabilities61;
	DEVICE_CAPABILITIES DeviceCaps;

	ULONG CombinedWakeUpEnable;
	ULONG WakeUpEnable;
	PIRP pIrpWaitWake;
	SYSTEM_POWER_STATE WaitWakeSystemState;
	LARGE_INTEGER VcIndex;
	ULONG VcCountLock;
	LIST_ENTRY	WmiEnabledVcs;
	PNDIS_GUID pNdisGuidMap;
	PNDIS_GUID pCustomGuidMap;
	USHORT VcCount;
	USHORT cNdisGuidMap;
	USHORT cCustomGuidMap;
	NDIS_TIMER MediaDisconnectTimer;
	NDIS_PNP_DEVICE_STATE PnPDeviceState;
	NDIS_PNP_DEVICE_STATE OldPnPDeviceState;
	KDPC DeferredDpc;
	LARGE_INTEGER StartTicks;
#ifdef _X86_
	PNDIS_PACKET IndicatedPacket[32];
#else
	PNDIS_PACKET IndicatedPacket[64];
#endif
	PKEVENT RemoveReadyEvent;
	PKEVENT AllOpensClosedEvent;
	PKEVENT AllRequestsCompletedEvent;
	ULONG InitTimeMs;
	NDIS_MINIPORT_WORK_ITEM WorkItemBuffer[6];
	POID_LIST OidList;
	USHORT InternalResetCount;
	USHORT MiniportResetCount;
	USHORT MediaSenseConnectCount;
	USHORT MediaSenseDisconnectCount;
	PNDIS_PACKET *xPackets;
	ULONG UserModeOpenReferences;
	FUNC  SavedIndicateNetBufferListsHandler;
	PVOID SavedIndicateNetBufferListsContext;
	FUNC  WSendPacketsHandler;
	ULONG MiniportAttributes;
	USHORT NumOpens;
	USHORT CFHangXTicks;
	ULONG RequestCount;
	ULONG IndicatedPacketsCount;
	ULONG PhysicalMediumType;
	NDIS_MEDIUM MiniportMediaType;
	PNDIS_REQUEST LastRequest;
	PVOID FakeMac;
	ULONG LockDbg;
	ULONG LockDbgX;
	PVOID LockThread;
	ULONG InfoFlags;
	KSPIN_LOCK TimerQueueLock;
	PKEVENT ResetCompletedEvent;
	PKEVENT QueuedBindingCompletedEvent;
	FUNC  SavedPacketIndicateHandler;
	ULONG RegisteredInterrupts;
	ULONG SetOid;
	PKEVENT WakeUpTimerEvent;
	PVOID DeviceContext;
	NDIS_RECEIVE_SCALE_CAPABILITIES RcvScaleCapabilities;

	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR CombinedRSSParametersBuf[196];
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR RSSParametersBuf[196];

	UCHAR Miniport5InNdis6Mode;
	UCHAR Miniport5HasNdis6Component;
	UCHAR RestoreStackNeeded;
	CHAR MediaChangeFilters;
	UCHAR FilterPnPLockAcquired;
	UCHAR LWFilterAllLoaded;
	UCHAR CheckPacketFilters;
	UCHAR ReceiveFilters;
	PVOID FilterPnPLockThread;
	ULONG FilterPnPLockDbgX;
	KSPIN_LOCK RecvLock;
	NDIS_MINIPORT_STATE RecvState;
	ULONG OutstandingReceives;
	PVOID MiniportRecvLockThread;
	ULONG RecvLockDbg;
	FUNC  NextSendPacketsHandler;
	FUNC  FinalSendPacketsHandler;
	PVOID LWFilterMutexOwnerThread;
	ULONG LWFilterMutexOwner;
	ULONG LWFilterMutexOwnerCount;
	PNDIS_FILTER_BLOCK LowestFilter;
	PNDIS_FILTER_BLOCK HighestFilter;
	PVOID ShutdownContext;
	union{
		FUNC  ShutdownHandler;
		FUNC  ShutdownHandlerEx;
	};
	KBUGCHECK_CALLBACK_RECORD BugcheckCallbackRecord;
	FUNC  TopIndicateNetBufferListsHandler;
	FUNC  TopIndicateLoopbackNetBufferListsHandler;
	FUNC  Ndis5PacketIndicateHandler;
	FUNC  MiniportReturnPacketHandler;
	PVOID MiniportReturnPacketContext;
	FUNC  SynchronousReturnPacketHandler;
	PVOID SynchronousReturnPacketContext;
	FUNC  NextRequestHandler;
	PVOID NextRequestContext;
	LIST_ENTRY OidRequestList;
	PNDIS_OID_REQUEST PendingOidRequest;
	PVOID NextCoOidRequestHandle;
	ULONG Ndis6ProtocolsBound;
	ULONG PmodeOpen6;
	NDIS_MINIPORT_STATE State;
	PKEVENT AsyncOpCompletionEvent;
	LONG AsyncOpCompletionStatus;
	ULONG_REFERENCE Ref;
	ULONG64 MaxXmitLinkSpeed;
	ULONG64 MaxRcvLinkSpeed;
	ULONG SupportedPacketFilters;
	NDIS_MINIPORT_HANDLERS_WIN6 NoFilter;
	NDIS_MINIPORT_HANDLERS_WIN6 Next;
	LONG NumOfPauseRestartRequests;
	ULONG FilterPnPFlags;
	ULONG SupportedStatistics;
	ULONG cDpcSendCompletes;
	ULONG cDpcRcvIndications;
	ULONG cDpcRcvIndicationCalls;
	ULONG cDpcNbSendCompletes;
	ULONG cDpcNblSendCompletes;
	ULONG cDpcNblIndications;
	ULONG cDpcMaxPacketsIndicated;
	ULONG cDpcTotalDpcCount;
	NDIS_RECEIVE_SCALE_CAPABILITIES RecvScaleCapabilities;
	PNDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES GeneralAttributes;
	LIST_ENTRY PortList;
	PUCHAR AllocatedPortIndices;
	ULONG AllocatedPortIndicesLength;
	ULONG NumberOfPorts;
	ULONG NumberOfActivePorts;
	ULONG64 MiniportXmitLinkSpeed;
	ULONG64 MiniportRcvLinkSpeed;
	KEVENT RestoreStackTimerEvent;
	NDIS_TIMER RestoreStackTimer;
	NDIS_WORK_ITEM RestoreStackWorkItem;
	ULONG RestoreStackBindRefCount;
	ULONG ProtocolsToBind;
	FUNC  DpcTrackers;
	FUNC  CurrentDpcTracker;
	UCHAR DpcTrackerIndex;
	UCHAR SupportedRss;
	ULONG64 DpcWatchDogCycleCount;
	ULONG DataBackFillSize;
	ULONG ContextBackFillSize;
	ULONG SupportedPauseFunctions;
	NDIS_RESTART_GENERAL_ATTRIBUTES TopFilterRestartAttributes;
	NDIS_RECEIVE_SCALE_CAPABILITIES TopRecvScaleCapabilities;
	LONG NumOfOpenPauseRestartRequests;
	LONG MiniportStackPauseCount;
	ULONG NsiOpenReferences;
	PKEVENT NsiRequestsCompletedEvent;
	DEVICE_POWER_STATE QueryPowerDeviceState;
	UCHAR MinimumNdisMajorVersion;
	UCHAR MinimumNdisMinorVersion;
	USHORT NumOfMinimumVersionDrivers;
	PVOID BottomIfStats;
	NDIS_M_PERIODIC_RECEIVES_WIN6 PeriodicReceiveQueue;
	NDIS_PORT_CONTROLL_STATE DefaultSendControlState;
	NDIS_PORT_CONTROLL_STATE DefaultRcvControlState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultSendAuthorizationState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultRcvAuthorizationState;
	NDIS_PORT_CONTROLL_STATE DefaultPortSendControlState;
	NDIS_PORT_CONTROLL_STATE DefaultPortRcvControlState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultPortSendAuthorizationState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultPortRcvAuthorizationState;
	NDIS_PCI_DEVICE_CUSTOM_PROPERTIES_WIN6 PciDeviceCustomProperties;

	FUNC TopNdis5PacketIndicateHandler;
	PULONG ndisSupportedOidList;
	ULONG ndisSupportedOidListLength;

	WORK_QUEUE_ITEM MiniportDpcWorkItem;
	ULONG64 InvalidFrames;

	union{
		UCHAR BeginSavedFields;
		ULONG NumUserOpens;
	};
	NDIS_MINIPORT_OFFLOAD_REG OffloadRegistry;
	USHORT MediaDisconnectTimeOut;
	USHORT SGMapRegistersNeeded;
	ULONG DriverVerifyFlags;
	PGET_SET_DEVICE_DATA  SetBusData;
	PGET_SET_DEVICE_DATA  GetBusData;
	PVOID BusDataContext;
	NDIS_INTERFACE_TYPE BusType;
	ULONG BusNumber;
	ULONG SlotNumber;
	NDIS_EVENT OpenReadyEvent;
	ULONG NumAdminOpens;
	PVOID DriverHandle;
	PNDIS_BIND_PATHS BindPaths;
	PNDIS_BIND_PATHS LWFilterList;
	PVOID ProtocolsFailToBind;
	ULONG MiniportPhysicalMediumType;
	PVOID LWFilterAttachList;
	ULONG NumFilters;
	ULONG NetLuidIndex;
	KMUTANT LWFilterMutex;
	PVOID SecurityDescriptor;
	UNICODE_STRING BaseName;
	UNICODE_STRING MiniportName;
	PDEVICE_OBJECT DeviceObject;
	PDEVICE_OBJECT PhysicalDeviceObject;
	PDEVICE_OBJECT NextDeviceObject;
	PNDIS_MINIPORT_BLOCK NextGlobalMiniport;
	PUNICODE_STRING pAdapterInstanceName;
	ULONG PnPCapabilities;
	DEVICE_POWER_STATE CurrentDevicePowerState;

	PVOID BusInterface;
	USHORT InstanceNumber;
	PVOID ConfigurationHandle;
	GUID InterfaceGuid;
	PVOID IfBlock;
	ULONG IfIndex;
	NET_IF_ADMIN_STATUS AdminStatus;
	NET_IF_OPER_STATUS OperStatus;
	ULONG OperStatusFlags;
	union{
		FUNC  SendHandler;
		FUNC  WanSendHandler;
	};
	PVOID Offload;
	PVOID AddDeviceContext;
	IO_REMOVE_LOCK_NODBG RemoveLock;
	UNICODE_STRING DevinterfaceNetSymbolicLinkName;
	UCHAR RssEnable;
	UCHAR ChimneyEnable;
	ULONG PhysicalMediumInInf;
	UNICODE_STRING ExportName;
	UNICODE_STRING FilterClass;
	PVOID StatusProcessingThread;
	ULONG StatusProcessingDbgX;
	UNICODE_STRING FdoName;
	PNDIS_MINIPORT_BLOCK NextOrphanedMiniport;

}NDIS_MINIPORT_BLOCK_WIN6,*PNDIS_MINIPORT_BLOCK_WIN6;

// vista sp1
typedef struct _NDIS_MINIPORT_BLOCK_WIN6_SP1
{
	NDIS_OBJECT_HEADER          Header;
	PNDIS_MINIPORT_BLOCK        NextMiniport;       // used by driver's MiniportQueue
	PNDIS_MINIPORT_BLOCK        BaseMiniport;
	NDIS_HANDLE                 MiniportAdapterContext; // context when calling mini-port functions
	union{
		UNICODE_STRING              Reserved4;
		struct
		{
			UCHAR MajorNdisVersion;
			UCHAR MinorNdisVersion;
			PIRP PendingQueryPowerIrp;
		};		
	};
	PKEVENT PnPEventLockEvent;

	NDIS_HANDLE                 OpenQueue;          // queue of opens for this mini-port
	REFERENCE                   ShortRef;           // contains spinlock for OpenQueue

	PNDIS_OBJECT_HEADER MediaSpecificAttributes;

	UCHAR                       LinkStateIndicationFlags;           // DO NOT REMOVE OR NDIS WILL BREAK!!!

	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the same time.
	//
	UCHAR                       LockAcquired;       // EXPOSED via macros. Do not move

	UCHAR                       PmodeOpens;         // Count of opens which turned on pmode/all_local

	//
	//  This is the processor number that the miniport's
	//  interrupt DPC and timers are running on.
	//
	//  Note: This field is no longer used
	//
	UCHAR                       AssignedProcessor;

	KSPIN_LOCK                  Lock;

	PNDIS_REQUEST               MediaRequest;

	union{
		PNDIS_MINIPORT_INTERRUPT    Interrupt;
		PVOID InterruptEx;
	};

	ULONG                       Flags;              // Flags to keep track of the miniport's state.
	ULONG                       PnPFlags;

	//
	// Send information
	//
	LIST_ENTRY                  PacketList;
	PNDIS_PACKET                FirstPendingPacket; // This is head of the queue of packets
	// waiting to be sent to miniport.
	PNDIS_PACKET                ReturnPacketsQueue;

	//
	// Space used for temp. use during request processing
	//
	ULONG                       RequestBuffer;
	PVOID                       SetMCastBuffer;

	PNDIS_MINIPORT_BLOCK        PrimaryMiniport;
	FUNC                        NextCancelSendNetBufferListsHandler;

	//
	// context to pass to bus driver when reading or writing config space
	//
	PVOID                       OidContext;
	ULONG                       SupportedOidListLength;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST           Resources;

	//
	// Watch-dog timer
	//
	NDIS_TIMER                  WakeUpDpcTimer;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	// Note:
	// the following two fields should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	union{
		UNICODE_STRING              Reserved20;
		struct
		{
			NET_IF_MEDIA_CONNECT_STATE MiniportMediaConnectState;
			NET_IF_MEDIA_DUPLEX_STATE MiniportMediaDuplexState;
		};		
	};
	UNICODE_STRING              SymbolicLinkName;

	//
	// Check for hang stuff
	//
	ULONG                       CheckForHangSeconds;
	USHORT                      CFHangTicks;
	USHORT                      CFHangCurrentTick;

	//
	// Reset information
	//
	NDIS_STATUS                 ResetStatus;
	NDIS_HANDLE                 ResetOpen;

	//
	// Holds media specific information.
	//
#ifdef __cplusplus
	FILTERDBS                   FilterDbs;          // EXPOSED via macros. Do not move
#else
	FILTERDBS;                                      // EXPOSED via macros. Do not move
#endif

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER    SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER   SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER   ResetCompleteHandler;

	NDIS_MEDIUM                 MediaType;

	//
	// contains mini-port information
	//
	ULONG                       AutoNegotiationFlags;
	NDIS_INTERFACE_TYPE         Reserved5;
	NDIS_INTERFACE_TYPE         AdapterType;

	union{
		PDEVICE_OBJECT              Reserved6;
		NET_IF_MEDIA_CONNECT_STATE	MediaConnectState;
	};
	union{
		PDEVICE_OBJECT              Reserved7;
		NET_IF_MEDIA_DUPLEX_STATE	MediaDuplexState;
	};
	PULONG                      SupportedOidList;


	PNDIS_SG_DMA_BLOCK          MiniportSGDmaBlock;

	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST               CallMgrAfList;

	PVOID                       MiniportThread;
	PVOID                       SetInfoBuf;
	USHORT                      SetInfoBufLen;
	USHORT                      MaxSendPackets;

	//
	//  Status code that is returned from the fake handlers.
	//
	NDIS_STATUS                 FakeStatus;

	PVOID                       LockHandler;        // For the filter lock

	PNDIS_RECEIVE_SCALE_PARAMETERS CombinedNdisRSSParameters;
	union{
		PNDIS_MINIPORT_TIMER TimerQueue;
		PVOID TimerObjectQueue;
	};
	UINT                        MacOptions;

	//
	// RequestInformation
	//
	PNDIS_REQUEST               PendingRequest;
	UINT                        MaximumLongAddresses;
	UINT                        MaximumShortAddresses;
	UINT                        MiniportCurrentLookahead;
	UINT                        MiniportMaximumLookahead;

	//
	//  For efficiency
	//
	union{
		ULONG_PTR                   Reserved1;
		PX_FILTER                   NullMediaFilter;
	};
	W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER      SendPacketsHandler;
	NDIS_M_START_SENDS          DeferredSendHandler;

	//
	// The following cannot be unionized.
	//
	ETH_RCV_INDICATE_HANDLER    EthRxIndicateHandler;   // EXPOSED via macros. Do not move
	TR_RCV_INDICATE_HANDLER     TrRxIndicateHandler;    // EXPOSED via macros. Do not move
	FUNC                        NextSendNetBufferListsHandler;

	ETH_RCV_COMPLETE_HANDLER    EthRxCompleteHandler;   // EXPOSED via macros. Do not move
	TR_RCV_COMPLETE_HANDLER     TrRxCompleteHandler;    // EXPOSED via macros. Do not move
	FUNC                        SavedNextSendNetBufferListsHandler;

	NDIS_M_STATUS_HANDLER       StatusHandler;          // EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER StatusCompleteHandler;  // EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;      // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER QueryCompleteHandler;   // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER SetCompleteHandler;     // EXPOSED via macros. Do not move

	NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;// EXPOSED via macros. Do not move
	WAN_RCV_HANDLER             WanRcvHandler;          // EXPOSED via macros. Do not move
	WAN_RCV_COMPLETE_HANDLER    WanRcvCompleteHandler;  // EXPOSED via macros. Do not move

	/********************************************************************************************/
	/****************                                                                  **********/
	/**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
	/**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
	/****************                                                                  **********/
	/********************************************************************************************/

	FUNC  IndicateNetBufferListsHandler;
	PVOID IndicateNetBufferListsContext;
	FUNC  SendNetBufferListsCompleteHandler;
	FUNC  NextReturnNetBufferLists;
	PVOID NextReturnNetBufferListsContext;

	UINT64 XmitLinkSpeed;
	UINT64 RcvLinkSpeed;
	NDIS_SUPPORTED_PAUSE_FUNCTIONS PauseFunctions;

	NDIS_SUPPORTED_PAUSE_FUNCTIONS MiniportPauseFunctions;
	ULONG MiniportAutoNegotiationFlags;
	SINGLE_LIST_ENTRY WorkQueue[7];
	SINGLE_LIST_ENTRY SingleWorkItems[6];
	UCHAR SendFlags;
	UCHAR TrResetRing;
	UCHAR MP6SupportPM;
	UCHAR XState;
	PNDIS_LOG Log;
	PCM_RESOURCE_LIST AllocatedResources;
	PCM_RESOURCE_LIST AllocatedResourcesTranslated;
	SINGLE_LIST_ENTRY PatternList;

	NDIS_PNP_CAPABILITIES PMCapabilities61;
	DEVICE_CAPABILITIES DeviceCaps;

	ULONG CombinedWakeUpEnable;
	ULONG WakeUpEnable;
	PIRP pIrpWaitWake;
	SYSTEM_POWER_STATE WaitWakeSystemState;
	LARGE_INTEGER VcIndex;
	ULONG VcCountLock;
	LIST_ENTRY	WmiEnabledVcs;
	PNDIS_GUID pNdisGuidMap;
	PNDIS_GUID pCustomGuidMap;
	USHORT VcCount;
	USHORT cNdisGuidMap;
	USHORT cCustomGuidMap;
	NDIS_TIMER MediaDisconnectTimer;
	NDIS_PNP_DEVICE_STATE PnPDeviceState;
	NDIS_PNP_DEVICE_STATE OldPnPDeviceState;
	KDPC DeferredDpc;
	LARGE_INTEGER StartTicks;
#ifdef _X86_
	PNDIS_PACKET IndicatedPacket[32];
#else
	PNDIS_PACKET IndicatedPacket[64];
#endif
	PKEVENT RemoveReadyEvent;
	PKEVENT AllOpensClosedEvent;
	PKEVENT AllRequestsCompletedEvent;
	ULONG InitTimeMs;
	NDIS_MINIPORT_WORK_ITEM WorkItemBuffer[6];
	POID_LIST OidList;
	USHORT InternalResetCount;
	USHORT MiniportResetCount;
	USHORT MediaSenseConnectCount;
	USHORT MediaSenseDisconnectCount;
	PNDIS_PACKET *xPackets;
	ULONG UserModeOpenReferences;
	FUNC  SavedIndicateNetBufferListsHandler;
	PVOID SavedIndicateNetBufferListsContext;
	FUNC  WSendPacketsHandler;
	ULONG MiniportAttributes;
	USHORT NumOpens;
	USHORT CFHangXTicks;
	ULONG RequestCount;
	ULONG IndicatedPacketsCount;
	ULONG PhysicalMediumType;
	NDIS_MEDIUM MiniportMediaType;
	PNDIS_REQUEST LastRequest;
	PVOID FakeMac;
	ULONG LockDbg;
	ULONG LockDbgX;
	PVOID LockThread;
	ULONG InfoFlags;
	KSPIN_LOCK TimerQueueLock;
	PKEVENT ResetCompletedEvent;
	PKEVENT QueuedBindingCompletedEvent;
	FUNC  SavedPacketIndicateHandler;
	ULONG RegisteredInterrupts;
	ULONG SetOid;
	PKEVENT WakeUpTimerEvent;
	PVOID DeviceContext;
	NDIS_RECEIVE_SCALE_CAPABILITIES RcvScaleCapabilities;

	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR CombinedRSSParametersBuf[196];
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR RSSParametersBuf[196];

	UCHAR Miniport5InNdis6Mode;
	UCHAR Miniport5HasNdis6Component;
	UCHAR RestoreStackNeeded;
	CHAR MediaChangeFilters;
	UCHAR FilterPnPLockAcquired;
	UCHAR LWFilterAllLoaded;
	UCHAR CheckPacketFilters;
	UCHAR ReceiveFilters;
	PVOID FilterPnPLockThread;
	ULONG FilterPnPLockDbgX;
	KSPIN_LOCK RecvLock;
	NDIS_MINIPORT_STATE RecvState;
	ULONG OutstandingReceives;
	PVOID MiniportRecvLockThread;
	ULONG RecvLockDbg;
	FUNC  NextSendPacketsHandler;
	FUNC  FinalSendPacketsHandler;
	PVOID LWFilterMutexOwnerThread;
	ULONG LWFilterMutexOwner;
	ULONG LWFilterMutexOwnerCount;
	PNDIS_FILTER_BLOCK LowestFilter;
	PNDIS_FILTER_BLOCK HighestFilter;
	PVOID ShutdownContext;
	union{
		FUNC  ShutdownHandler;
		FUNC  ShutdownHandlerEx;
	};
	KBUGCHECK_CALLBACK_RECORD BugcheckCallbackRecord;
	FUNC  TopIndicateNetBufferListsHandler;
	FUNC  TopIndicateLoopbackNetBufferListsHandler;
	FUNC  Ndis5PacketIndicateHandler;
	FUNC  MiniportReturnPacketHandler;
	PVOID MiniportReturnPacketContext;
	FUNC  SynchronousReturnPacketHandler;
	PVOID SynchronousReturnPacketContext;
	FUNC  NextRequestHandler;
	PVOID NextRequestContext;
	LIST_ENTRY OidRequestList;
	PNDIS_OID_REQUEST PendingOidRequest;
	PVOID NextCoOidRequestHandle;
	ULONG Ndis6ProtocolsBound;
	ULONG PmodeOpen6;
	NDIS_MINIPORT_STATE State;
	PKEVENT AsyncOpCompletionEvent;
	LONG AsyncOpCompletionStatus;
	ULONG_REFERENCE Ref;
	ULONG64 MaxXmitLinkSpeed;
	ULONG64 MaxRcvLinkSpeed;
	ULONG SupportedPacketFilters;
	NDIS_MINIPORT_HANDLERS_WIN6_SP1 NoFilter;
	NDIS_MINIPORT_HANDLERS_WIN6_SP1 Next;
	LONG NumOfPauseRestartRequests;
	ULONG FilterPnPFlags;
	ULONG SupportedStatistics;
	ULONG cDpcSendCompletes;
	ULONG cDpcRcvIndications;
	ULONG cDpcRcvIndicationCalls;
	ULONG cDpcNbSendCompletes;
	ULONG cDpcNblSendCompletes;
	ULONG cDpcNblIndications;
	ULONG cDpcMaxPacketsIndicated;
	ULONG cDpcTotalDpcCount;
	NDIS_RECEIVE_SCALE_CAPABILITIES RecvScaleCapabilities;
	PNDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES GeneralAttributes;
	LIST_ENTRY PortList;
	PUCHAR AllocatedPortIndices;
	ULONG AllocatedPortIndicesLength;
	ULONG NumberOfPorts;
	ULONG NumberOfActivePorts;
	ULONG64 MiniportXmitLinkSpeed;
	ULONG64 MiniportRcvLinkSpeed;
	KEVENT RestoreStackTimerEvent;
	NDIS_TIMER RestoreStackTimer;
	NDIS_WORK_ITEM RestoreStackWorkItem;
	ULONG RestoreStackBindRefCount;
	ULONG ProtocolsToBind;
	FUNC  DpcTrackers;
	FUNC  CurrentDpcTracker;
	UCHAR DpcTrackerIndex;
	UCHAR SupportedRss;
	ULONG64 DpcWatchDogCycleCount;
	ULONG DataBackFillSize;
	ULONG ContextBackFillSize;
	ULONG SupportedPauseFunctions;
	NDIS_RESTART_GENERAL_ATTRIBUTES TopFilterRestartAttributes;
	NDIS_RECEIVE_SCALE_CAPABILITIES TopRecvScaleCapabilities;
	LONG NumOfOpenPauseRestartRequests;
	LONG MiniportStackPauseCount;
	ULONG NsiOpenReferences;
	PKEVENT NsiRequestsCompletedEvent;
	DEVICE_POWER_STATE QueryPowerDeviceState;
	UCHAR MinimumNdisMajorVersion;
	UCHAR MinimumNdisMinorVersion;
	USHORT NumOfMinimumVersionDrivers;
	PVOID BottomIfStats;
	NDIS_M_PERIODIC_RECEIVES_WIN6 PeriodicReceiveQueue;
	NDIS_PORT_CONTROLL_STATE DefaultSendControlState;
	NDIS_PORT_CONTROLL_STATE DefaultRcvControlState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultSendAuthorizationState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultRcvAuthorizationState;
	NDIS_PORT_CONTROLL_STATE DefaultPortSendControlState;
	NDIS_PORT_CONTROLL_STATE DefaultPortRcvControlState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultPortSendAuthorizationState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultPortRcvAuthorizationState;
	NDIS_PCI_DEVICE_CUSTOM_PROPERTIES_WIN6 PciDeviceCustomProperties;

	FUNC TopNdis5PacketIndicateHandler;
	PULONG ndisSupportedOidList;
	ULONG ndisSupportedOidListLength;

	ULONG MsiIntCount;
	WORK_QUEUE_ITEM MiniportDpcWorkItem;
	ULONG64 InvalidFrames;

	LONG PagingPathCount;

	union{
		UCHAR BeginSavedFields;
		ULONG NumUserOpens;
	};
	NDIS_MINIPORT_OFFLOAD_REG OffloadRegistry;
	USHORT MediaDisconnectTimeOut;
	USHORT SGMapRegistersNeeded;
	ULONG DriverVerifyFlags;
	PGET_SET_DEVICE_DATA  SetBusData;
	PGET_SET_DEVICE_DATA  GetBusData;
	PVOID BusDataContext;
	NDIS_INTERFACE_TYPE BusType;
	ULONG BusNumber;
	ULONG SlotNumber;
	NDIS_EVENT OpenReadyEvent;
	ULONG NumAdminOpens;
	PVOID DriverHandle;
	PNDIS_BIND_PATHS BindPaths;
	PNDIS_BIND_PATHS LWFilterList;
	PVOID ProtocolsFailToBind;
	ULONG MiniportPhysicalMediumType;
	PVOID LWFilterAttachList;
	ULONG NumFilters;
	ULONG NetLuidIndex;
	KMUTANT LWFilterMutex;
	PVOID SecurityDescriptor;
	UNICODE_STRING BaseName;
	UNICODE_STRING MiniportName;
	PDEVICE_OBJECT DeviceObject;
	PDEVICE_OBJECT PhysicalDeviceObject;
	PDEVICE_OBJECT NextDeviceObject;
	PNDIS_MINIPORT_BLOCK NextGlobalMiniport;
	PUNICODE_STRING pAdapterInstanceName;
	ULONG PnPCapabilities;
	DEVICE_POWER_STATE CurrentDevicePowerState;

	PVOID BusInterface;
	USHORT InstanceNumber;
	PVOID ConfigurationHandle;
	GUID InterfaceGuid;
	PVOID IfBlock;
	ULONG IfIndex;
	NET_IF_ADMIN_STATUS AdminStatus;
	NET_IF_OPER_STATUS OperStatus;
	ULONG OperStatusFlags;
	union{
		FUNC  SendHandler;
		FUNC  WanSendHandler;
	};
	PVOID Offload;
	PVOID AddDeviceContext;
	IO_REMOVE_LOCK_NODBG RemoveLock;
	UNICODE_STRING DevinterfaceNetSymbolicLinkName;
	UCHAR RssEnable;
	UCHAR ChimneyEnable;
	ULONG PhysicalMediumInInf;
	UNICODE_STRING ExportName;
	UNICODE_STRING FilterClass;
	PVOID StatusProcessingThread;
	ULONG StatusProcessingDbgX;
	UNICODE_STRING FdoName;
	PNDIS_MINIPORT_BLOCK NextOrphanedMiniport;

}NDIS_MINIPORT_BLOCK_WIN6_SP1,*PNDIS_MINIPORT_BLOCK_WIN6_SP1;

// windows 7
typedef struct _NDIS_MINIPORT_BLOCK_WIN7
{
	NDIS_OBJECT_HEADER          Header;
	PNDIS_MINIPORT_BLOCK        NextMiniport;       // used by driver's MiniportQueue
	PNDIS_MINIPORT_BLOCK        BaseMiniport;
	NDIS_HANDLE                 MiniportAdapterContext; // context when calling mini-port functions
	union{
		UNICODE_STRING              Reserved4;
		struct
		{
			UCHAR MajorNdisVersion;
			UCHAR MinorNdisVersion;
			PVOID PerProcCounters;
		};		
	};
	union
	{
		ULONG EnabledPerformanceCounters;
		PVOID pvoidEnabledPerformanceCounters;
	};
	NDIS_HANDLE                 OpenQueue;          // queue of opens for this mini-port
	REFERENCE                   ShortRef;           // contains spinlock for OpenQueue

	union
	{
		ULONG EnabledPerformanceCountersEx;
		PVOID pvoidEnabledPerformanceCountersEx;
	};

	UCHAR                       LinkStateIndicationFlags;           // DO NOT REMOVE OR NDIS WILL BREAK!!!

	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the same time.
	//
	UCHAR                       LockAcquired;       // EXPOSED via macros. Do not move

	UCHAR                       PmodeOpens;         // Count of opens which turned on pmode/all_local

	//
	//  This is the processor number that the miniport's
	//  interrupt DPC and timers are running on.
	//
	//  Note: This field is no longer used
	//
	UCHAR                       Reserved23;

	KSPIN_LOCK                  Lock;

	PNDIS_REQUEST               MediaRequest;

	PNDIS_MINIPORT_INTERRUPT    Interrupt;

	ULONG                       Flags;              // Flags to keep track of the miniport's state.
	ULONG                       PnPFlags;

	//
	// Send information
	//
	LIST_ENTRY                  PacketList;
	PNDIS_PACKET                FirstPendingPacket; // This is head of the queue of packets
	// waiting to be sent to miniport.
	PNDIS_PACKET                ReturnPacketsQueue;

	//
	// Space used for temp. use during request processing
	//
	ULONG                       RequestBuffer;
	PVOID                       SetMCastBuffer;

	PNDIS_MINIPORT_BLOCK        PrimaryMiniport;
	FUNC                        NextCancelSendNetBufferListsHandler;

	//
	// context to pass to bus driver when reading or writing config space
	//
	PVOID                       OidContext;
	ULONG                       SupportedOidListLength;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST           Resources;

	//
	// Watch-dog timer
	//
	NDIS_TIMER                  WakeUpDpcTimer;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	// Note:
	// the following two fields should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	union{
		UNICODE_STRING              Reserved20;
		struct
		{
			NET_IF_MEDIA_CONNECT_STATE MiniportMediaConnectState;
			NET_IF_MEDIA_DUPLEX_STATE MiniportMediaDuplexState;
		};		
	};
	UNICODE_STRING              SymbolicLinkName;

	//
	// Check for hang stuff
	//
	ULONG                       CheckForHangSeconds;
	USHORT                      CFHangTicks;
	USHORT                      CFHangCurrentTick;

	//
	// Reset information
	//
	NDIS_STATUS                 ResetStatus;
	NDIS_HANDLE                 ResetOpen;

	//
	// Holds media specific information.
	//
#ifdef __cplusplus
	FILTERDBS                   FilterDbs;          // EXPOSED via macros. Do not move
#else
	FILTERDBS;                                      // EXPOSED via macros. Do not move
#endif

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER    SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER   SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER   ResetCompleteHandler;

	NDIS_MEDIUM                 MediaType;

	//
	// contains mini-port information
	//
	ULONG                       AutoNegotiationFlags;
	NDIS_INTERFACE_TYPE         Reserved5;
	NDIS_INTERFACE_TYPE         AdapterType;

	union{
		PDEVICE_OBJECT              Reserved6;
		NET_IF_MEDIA_CONNECT_STATE	MediaConnectState;
	};
	union{
		PDEVICE_OBJECT              Reserved7;
		NET_IF_MEDIA_DUPLEX_STATE	MediaDuplexState;
	};
	PULONG                      SupportedOidList;


	PNDIS_SG_DMA_BLOCK          MiniportSGDmaBlock;

	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST               CallMgrAfList;

	PVOID                       MiniportThread;
	PVOID                       SetInfoBuf;
	USHORT                      SetInfoBufLen;
	USHORT                      MaxSendPackets;

	//
	//  Status code that is returned from the fake handlers.
	//
	NDIS_STATUS                 FakeStatus;

	PVOID                       Reserved24;        // For the filter lock

	PNDIS_RECEIVE_SCALE_PARAMETERS CombinedNdisRSSParameters;
	union{
		PNDIS_MINIPORT_TIMER TimerQueue;
		PVOID TimerObjectQueue;
	};
	UINT                        MacOptions;

	//
	// RequestInformation
	//
	PNDIS_REQUEST               PendingRequest;
	UINT                        MaximumLongAddresses;
	UINT                        MaximumShortAddresses;
	UINT                        MiniportCurrentLookahead;
	UINT                        MiniportMaximumLookahead;

	//
	//  For efficiency
	//
	union{
		ULONG_PTR                   Reserved1;
		PX_FILTER                   NullMediaFilter;
	};
	W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER      SendPacketsHandler;
	NDIS_M_START_SENDS          DeferredSendHandler;

	//
	// The following cannot be unionized.
	//
	ETH_RCV_INDICATE_HANDLER    EthRxIndicateHandler;   // EXPOSED via macros. Do not move
	TR_RCV_INDICATE_HANDLER     TrRxIndicateHandler;    // EXPOSED via macros. Do not move
	FUNC                        NextSendNetBufferListsHandler;

	ETH_RCV_COMPLETE_HANDLER    EthRxCompleteHandler;   // EXPOSED via macros. Do not move
	TR_RCV_COMPLETE_HANDLER     TrRxCompleteHandler;    // EXPOSED via macros. Do not move
	FUNC                        SavedNextSendNetBufferListsHandler;

	NDIS_M_STATUS_HANDLER       StatusHandler;          // EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER StatusCompleteHandler;  // EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;      // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER QueryCompleteHandler;   // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER SetCompleteHandler;     // EXPOSED via macros. Do not move

	NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;// EXPOSED via macros. Do not move
	WAN_RCV_HANDLER             WanRcvHandler;          // EXPOSED via macros. Do not move
	WAN_RCV_COMPLETE_HANDLER    WanRcvCompleteHandler;  // EXPOSED via macros. Do not move

	/********************************************************************************************/
	/****************                                                                  **********/
	/**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
	/**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
	/****************                                                                  **********/
	/********************************************************************************************/

	FUNC  IndicateNetBufferListsHandler;
	PVOID IndicateNetBufferListsContext;
	FUNC  SendNetBufferListsCompleteHandler;
	FUNC  NextReturnNetBufferLists;
	PVOID NextReturnNetBufferListsContext;
	PKEVENT PnPEventLockEvent;
	PNDIS_OBJECT_HEADER MediaSpecificAttributes;
	PIRP PendingQueryPowerIrp;
	PVOID UnalignedPerProcCounters;
	PNDIS_INTERRUPT_BLOCK InterruptEx;
	UINT64 XmitLinkSpeed;
	UINT64 RcvLinkSpeed;
	NDIS_SUPPORTED_PAUSE_FUNCTIONS PauseFunctions;
	PROCESSOR_NUMBER AssignedProcessor;
	NDIS_SUPPORTED_PAUSE_FUNCTIONS MiniportPauseFunctions;
	ULONG MiniportAutoNegotiationFlags;
	SINGLE_LIST_ENTRY WorkQueue[7];
	SINGLE_LIST_ENTRY SingleWorkItems[6];
	UCHAR SendFlags;
	UCHAR TrResetRing;
	UCHAR MP6SupportPM;
	UCHAR XState;
	PNDIS_LOG Log;
	PCM_RESOURCE_LIST AllocatedResources;
	PCM_RESOURCE_LIST AllocatedResourcesTranslated;
	SINGLE_LIST_ENTRY PatternList;
	SINGLE_LIST_ENTRY WOLPatternList;
	SINGLE_LIST_ENTRY PMProtocolOffloadList;
	NDIS_PNP_CAPABILITIES PMCapabilities61;
	NDIS_PM_CAPABILITIES PMHardwareCapabilities;
	NDIS_PM_CAPABILITIES PMAdvertisedCapabilities;
	NDIS_PM_PARAMETERS PMCurrentParameters;
	DEVICE_CAPABILITIES DeviceCaps;
	UCHAR S0WakeupSupported;
	ULONG CombinedWakeUpEnable;
	ULONG WakeUpEnable;
	PIRP pIrpWaitWake;
	SYSTEM_POWER_STATE WaitWakeSystemState;
	LARGE_INTEGER VcIndex;
	ULONG VcCountLock;
	LIST_ENTRY	WmiEnabledVcs;
	PNDIS_GUID pNdisGuidMap;
	PNDIS_GUID pCustomGuidMap;
	USHORT VcCount;
	USHORT cNdisGuidMap;
	USHORT cCustomGuidMap;
	NDIS_TIMER MediaDisconnectTimer;
	NDIS_PNP_DEVICE_STATE PnPDeviceState;
	NDIS_PNP_DEVICE_STATE OldPnPDeviceState;
	KDPC DeferredDpc;
	LARGE_INTEGER StartTicks;
	PNDIS_PACKET IndicatedPacket;
	PKEVENT RemoveReadyEvent;
	PKEVENT AllOpensClosedEvent;
	PKEVENT AllRequestsCompletedEvent;
	ULONG InitTimeMs;
	NDIS_MINIPORT_WORK_ITEM WorkItemBuffer[6];
	POID_LIST OidList;
	USHORT InternalResetCount;
	USHORT MiniportResetCount;
	USHORT MediaSenseConnectCount;
	USHORT MediaSenseDisconnectCount;
	PNDIS_PACKET *xPackets;
	ULONG UserModeOpenReferences;
	FUNC  SavedIndicateNetBufferListsHandler;
	PVOID SavedIndicateNetBufferListsContext;
	FUNC  WSendPacketsHandler;
	ULONG MiniportAttributes;
	USHORT NumOpens;
	USHORT CFHangXTicks;
	ULONG RequestCount;
	ULONG IndicatedPacketsCount;
	ULONG PhysicalMediumType;
	NDIS_MEDIUM MiniportMediaType;
	PNDIS_REQUEST LastRequest;
	PVOID FakeMac;
	ULONG LockDbg;
	ULONG LockDbgX;
	PVOID LockThread;
	ULONG InfoFlags;
	KSPIN_LOCK TimerQueueLock;
	PKEVENT ResetCompletedEvent;
	PKEVENT QueuedBindingCompletedEvent;
	FUNC  SavedPacketIndicateHandler;
	ULONG RegisteredInterrupts;
	ULONG SetOid;
	PKEVENT WakeUpTimerEvent;
	PVOID DeviceContext;
#ifdef _X86_
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR CombinedRSSParametersBuf[604];
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR RSSParametersBuf[604];
#else
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR CombinedRSSParametersBuf[656];
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR RSSParametersBuf[656];
#endif
	UCHAR UsingMSIX;
	UCHAR Miniport5InNdis6Mode;
	UCHAR Miniport5HasNdis6Component;
	UCHAR RestoreStackNeeded;
	CHAR MediaChangeFilters;
	UCHAR FilterPnPLockAcquired;
	UCHAR LWFilterAllLoaded;
	UCHAR CheckPacketFilters;
	UCHAR ReceiveFilters;
	PVOID FilterPnPLockThread;
	ULONG FilterPnPLockDbgX;
	KSPIN_LOCK RecvLock;
	NDIS_MINIPORT_STATE RecvState;
	ULONG OutstandingReceives;
	PVOID MiniportRecvLockThread;
	ULONG RecvLockDbg;
	FUNC  NextSendPacketsHandler;
	FUNC  FinalSendPacketsHandler;
	PVOID LWFilterMutexOwnerThread;
	ULONG LWFilterMutexOwner;
	ULONG LWFilterMutexOwnerCount;
	PNDIS_FILTER_BLOCK LowestFilter;
	PNDIS_FILTER_BLOCK HighestFilter;
	PVOID ShutdownContext;
	union{
		FUNC  ShutdownHandler;
		FUNC  ShutdownHandlerEx;
	};
	KBUGCHECK_CALLBACK_RECORD BugcheckCallbackRecord;
	FUNC  TopIndicateNetBufferListsHandler;
	FUNC  TopIndicateLoopbackNetBufferListsHandler;
	FUNC  Ndis5PacketIndicateHandler;
	FUNC  MiniportReturnPacketHandler;
	PVOID MiniportReturnPacketContext;
	FUNC  SynchronousReturnPacketHandler;
	PVOID SynchronousReturnPacketContext;
	FUNC  NextRequestHandler;
	PVOID NextRequestContext;
	LIST_ENTRY OidRequestList;
	PNDIS_OID_REQUEST PendingOidRequest;
	PVOID NextCoOidRequestHandle;
	ULONG Ndis6ProtocolsBound;
	ULONG PmodeOpen6;
	NDIS_MINIPORT_STATE State;
	PKEVENT AsyncOpCompletionEvent;
	LONG AsyncOpCompletionStatus;
	ULONG_REFERENCE Ref;
	ULONG64 MaxXmitLinkSpeed;
	ULONG64 MaxRcvLinkSpeed;
	ULONG SupportedPacketFilters;
	NDIS_MINIPORT_HANDLERS_WIN7 NoFilter;
	NDIS_MINIPORT_HANDLERS_WIN7 Next;
	LONG NumOfPauseRestartRequests;
	ULONG FilterPnPFlags;
	ULONG SupportedStatistics;
	ULONG cDpcSendCompletes;
	ULONG cDpcRcvIndications;
	ULONG cDpcRcvIndicationCalls;
	ULONG cDpcNbSendCompletes;
	ULONG cDpcNblSendCompletes;
	ULONG cDpcNblIndications;
	ULONG cDpcMaxPacketsIndicated;
	ULONG cDpcTotalDpcCount;
	NDIS_RECEIVE_SCALE_CAPABILITIES RecvScaleCapabilities;
	PNDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES GeneralAttributes;
	LIST_ENTRY PortList;
	PUCHAR AllocatedPortIndices;
	ULONG AllocatedPortIndicesLength;
	ULONG NumberOfPorts;
	ULONG NumberOfActivePorts;
	ULONG64 MiniportXmitLinkSpeed;
	ULONG64 MiniportRcvLinkSpeed;
	KEVENT RestoreStackTimerEvent;
	NDIS_TIMER RestoreStackTimer;
	NDIS_WORK_ITEM RestoreStackWorkItem;
	ULONG RestoreStackBindRefCount;
	ULONG ProtocolsToBind;
	FUNC  DpcTrackers;
	FUNC  CurrentDpcTracker;
	UCHAR DpcTrackerIndex;
	UCHAR SupportedRss;
	ULONG64 DpcWatchDogCycleCount;
	ULONG DataBackFillSize;
	ULONG ContextBackFillSize;
	ULONG SupportedPauseFunctions;
	NDIS_RESTART_GENERAL_ATTRIBUTES TopFilterRestartAttributes;
	NDIS_RECEIVE_SCALE_CAPABILITIES TopRecvScaleCapabilities;
	LONG NumOfOpenPauseRestartRequests;
	LONG MiniportStackPauseCount;
	ULONG NsiOpenReferences;
	PKEVENT NsiRequestsCompletedEvent;
	DEVICE_POWER_STATE QueryPowerDeviceState;
	UCHAR MinimumNdisMajorVersion;
	UCHAR MinimumNdisMinorVersion;
	USHORT NumOfMinimumVersionDrivers;
	PVOID BottomIfStats;
	NDIS_M_PERIODIC_RECEIVES_WIN7 PeriodicReceiveQueue;
	NDIS_PORT_CONTROLL_STATE DefaultSendControlState;
	NDIS_PORT_CONTROLL_STATE DefaultRcvControlState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultSendAuthorizationState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultRcvAuthorizationState;
	NDIS_PORT_CONTROLL_STATE DefaultPortSendControlState;
	NDIS_PORT_CONTROLL_STATE DefaultPortRcvControlState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultPortSendAuthorizationState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultPortRcvAuthorizationState;
	NDIS_PCI_DEVICE_CUSTOM_PROPERTIES PciDeviceCustomProperties;
	FUNC TopNdis5PacketIndicateHandler;
	PULONG ndisSupportedOidList;
	ULONG ndisSupportedOidListLength;
	ULONG MsiIntCount;
	WORK_QUEUE_ITEM MiniportDpcWorkItem;
	ULONG64 InvalidFrames;
	LONG PagingPathCount;
	LIST_ENTRY ReceiveQueueList;
	PUCHAR AllocatedQueueIndices;
	ULONG AllocatedQueueIndicesLength;
	ULONG NumReceiveQueues;
	LIST_ENTRY ReceiveFilterList;
	PUCHAR AllocatedReceiveFilterIndices;
	ULONG AllocatedReceiveFilterIndicesLength;
	ULONG NumReceiveFilters;
	ULONG EnabledReceiveFilterTypes;
	ULONG EnabledReceiveQueueTypes;
	PNDIS_RECEIVE_FILTER_CAPABILITIES ReceiveFilterHwCapabilities;
	PNDIS_RECEIVE_FILTER_CAPABILITIES ReceiveFilterCurrentCapabilities;
	PNDIS_NIC_SWITCH_CAPABILITIES NicSwitchHwCapabilities;
	PNDIS_NIC_SWITCH_CAPABILITIES NicSwitchCurrentCapabilities;
	LIST_ENTRY SharedMemoryBlockList;
	FUNC  AllocateSharedMemoryHandler;
	FUNC  FreeSharedMemoryHandler;
	PVOID AllocateSharedMemoryContext;
	ULONG MsiXTableEntries;
	PCI_MSI_CAPABILITY MsiCaps;
	PCI_MSIX_CAPABILITY MsiXCaps;
	ULONG64 NumberOfIndirectionTableChanges;
	union{
		UCHAR BeginSavedFields;
		ULONG NumUserOpens;
	};
	NDIS_MINIPORT_OFFLOAD_REG OffloadRegistry;
	USHORT MediaDisconnectTimeOut;
	USHORT SGMapRegistersNeeded;
	ULONG DriverVerifyFlags;
	PGET_SET_DEVICE_DATA  SetBusData;
	PGET_SET_DEVICE_DATA  GetBusData;
	PVOID BusDataContext;
	NDIS_INTERFACE_TYPE BusType;
	ULONG BusNumber;
	ULONG SlotNumber;
	NDIS_EVENT OpenReadyEvent;
	ULONG NumAdminOpens;
	PVOID DriverHandle;
	PNDIS_BIND_PATHS BindPaths;
	PNDIS_BIND_PATHS LWFilterList;
	PVOID ProtocolsFailToBind;
	ULONG MiniportPhysicalMediumType;
	PVOID LWFilterAttachList;
	ULONG NumFilters;
	ULONG NetLuidIndex;
	KMUTANT LWFilterMutex;
	PVOID SecurityDescriptor;
	UNICODE_STRING BaseName;
	UNICODE_STRING MiniportName;
	PDEVICE_OBJECT DeviceObject;
	PDEVICE_OBJECT PhysicalDeviceObject;
	PDEVICE_OBJECT NextDeviceObject;
	PNDIS_MINIPORT_BLOCK NextGlobalMiniport;
	PUNICODE_STRING pAdapterInstanceName;
	ULONG PnPCapabilities;
	DEVICE_POWER_STATE CurrentDevicePowerState;
	DEVICE_POWER_STATE DriverPowerState;
	NDIS_PM_ADMIN_GONFIG PMAdminConfig;
	KSEMAPHORE PMPatternSemaphore;
	KSEMAPHORE PMOffloadSemaphore;
	PVOID BusInterface;
	USHORT InstanceNumber;
	PVOID ConfigurationHandle;
	GUID InterfaceGuid;
	PVOID IfBlock;
	ULONG IfIndex;
	NET_IF_ADMIN_STATUS AdminStatus;
	NET_IF_OPER_STATUS OperStatus;
	ULONG OperStatusFlags;
	union{
		FUNC  SendHandler;
		FUNC  WanSendHandler;
	};
	PVOID Offload;
	PVOID AddDeviceContext;
	IO_REMOVE_LOCK_NODBG RemoveLock;
	UNICODE_STRING DevinterfaceNetSymbolicLinkName;
	UCHAR RssEnable;
	UCHAR ChimneyEnable;
	ULONG PhysicalMediumInInf;
	UNICODE_STRING ExportName;
	UNICODE_STRING FilterClass;
	PVOID StatusProcessingThread;
	ULONG StatusProcessingDbgX;
	UNICODE_STRING FdoName;
	PNDIS_MINIPORT_BLOCK NextOrphanedMiniport;
	KEVENT PowerEvent;
	PKEVENT pPowerEvent;
	ULONG DirectOidRequestCount;
	PKEVENT AllDirectRequestsCompletedEvent;
	PVOID HDSplitCurrentConfig;
	PVOID MSIXConfigContext;
	PVOID SetMSIXTableEntry;
	PVOID MaskMSIXTableEntry;
	PVOID UnmaskMSIXTableEntry;
	WORK_QUEUE_ITEM DevicePowerStateWorkItem;
	WORK_QUEUE_ITEM SystemPowerStateWorkItem;
	PVOID DefaultReceiveQueue;
	ULONG MaxNumRssProcessors;
	PROCESSOR_NUMBER RssBaseProcessor;
	USHORT NumaNodeId;
	PUSHORT NumaDistances;
	PUNICODE_STRING pModifiedInstanceName;
	WORK_QUEUE_ITEM DeleteMiniportWorkItem;
	PVOID ProcessingOpen;
}NDIS_MINIPORT_BLOCK_WIN7,*PNDIS_MINIPORT_BLOCK_WIN7;

typedef enum _NDIS_MINIPORT_EVENT
{
	NdisMEvent_Unknown,
	NdisMEventDx_SystemSleep,
	NdisMEventDx_NicQuiet,
	NdisMEventDx_SSIdle,
	NdisMEventDx_D3DTimeout,
	NdisMEventD0_SystemResume,
	NdisMEventD0_NicActive,
	NdisMEventD0_AoAcWake,
	NdisMEventD0_SSResume,
	NdisMEventD0_D3DCancel,
	NdisMEventD0_D3DWake,
	NdisMEventD0_D3DResume,
	NdisMEventD0_Complete,
	NdisMEventD0_SSComplete,
	NdisMEventDx_Complete,
	NdisMEventDx_SSComplete,
	NdisMEvent_DeviceAdded,
	NdisMEvent_DeviceStart,
	NdisMEvent_DeviceQueryRemove,
	NdisMEvent_DeviceCancelRemove,
	NdisMEvent_DeviceRemove,
	NdisMEvent_DeviceSurpriseRemoval,
	NdisMEvent_DeviceQueryStop,
	NdisMEvent_DeviceCancelStop,
	NdisMEvent_DeviceStop,
	NdisMEvent_MiniportInitialized,
	NdisMEvent_MiniportHalted,
	NdisMEvent_MiniportPaused,
	NdisMEvent_MiniportRestarted,
	NdisMEvent_MiniportPmInitialized
}NDIS_MINIPORT_EVENT,*PNDIS_MINIPORT_EVENT;

typedef struct NDIS_NBL_TRACKER_HANDLE__
{
	LONG unused;
}NDIS_NBL_TRACKER_HANDLE,*PNDIS_NBL_TRACKER_HANDLE;


// for win8 

typedef struct _NDIS_RESTART_GENERAL_ATTRIBUTES_WIN8
{
	NDIS_OBJECT_HEADER              Header;
	ULONG                           MtuSize;
	ULONG64                         MaxXmitLinkSpeed;
	ULONG64                         MaxRcvLinkSpeed;
	ULONG                           LookaheadSize;
	ULONG                           MacOptions;
	ULONG                           SupportedPacketFilters;
	ULONG                           MaxMulticastListSize;
	PNDIS_RECEIVE_SCALE_CAPABILITIES RecvScaleCapabilities;
	NET_IF_ACCESS_TYPE              AccessType;
	ULONG                           Flags;       
	NET_IF_CONNECTION_TYPE          ConnectionType;
	ULONG                           SupportedStatistics;
	ULONG                           DataBackFillSize;
	ULONG                           ContextBackFillSize;
	PNDIS_OID                       SupportedOidList;
	ULONG                           SupportedOidListLength;
	ULONG                           MaxLookaheadSizeAccessed;
}NDIS_RESTART_GENERAL_ATTRIBUTES_WIN8, *PNDIS_RESTART_GENERAL_ATTRIBUTES_WIN8;

typedef struct _NDIS_RECEIVE_SCALE_CAPABILITIES_WIN8
{
	NDIS_OBJECT_HEADER  Header;
	ULONG   CapabilitiesFlags;
	ULONG   NumberOfInterruptMessages;
	ULONG   NumberOfReceiveQueues;
	USHORT  NumberOfIndirectionTableEntries;
} NDIS_RECEIVE_SCALE_CAPABILITIES_WIN8, *PNDIS_RECEIVE_SCALE_CAPABILITIES_WIN8;

typedef struct _NDIS_MINIPORT_BLOCK_WIN8
{
	NDIS_OBJECT_HEADER          Header;
	PNDIS_MINIPORT_BLOCK        NextMiniport;       // used by driver's MiniportQueue
	PNDIS_MINIPORT_BLOCK        BaseMiniport;
	NDIS_HANDLE                 MiniportAdapterContext; // context when calling mini-port functions
	union{
		UNICODE_STRING              Reserved4;
		struct
		{
			UCHAR MajorNdisVersion;
			UCHAR MinorNdisVersion;
			PVOID PcwDataBlock;
		};		
	};
	union
	{
		LONG PcwDatapathEventMask;
		PVOID Reserved28;
	};
	NDIS_HANDLE                 OpenQueue;          // queue of opens for this mini-port
	REFERENCE                   ShortRef;           // contains spinlock for OpenQueue

	union
	{
		LONG PcwDatapathCycleMask;
		PVOID Reserved29;
	};

	UCHAR                       LinkStateIndicationFlags;           // DO NOT REMOVE OR NDIS WILL BREAK!!!

	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the same time.
	//
	UCHAR                       LockAcquired;       // EXPOSED via macros. Do not move

	UCHAR                       PmodeOpens;         // Count of opens which turned on pmode/all_local

	//
	//  This is the processor number that the miniport's
	//  interrupt DPC and timers are running on.
	//
	//  Note: This field is no longer used
	//
	UCHAR                       Reserved23;

	KSPIN_LOCK                  Lock;

	PNDIS_REQUEST               Reserved25;

	PNDIS_MINIPORT_INTERRUPT    Interrupt;

	ULONG                       Flags;              // Flags to keep track of the miniport's state.
	ULONG                       PnPFlags;

	//
	// Send information
	//
	LIST_ENTRY                  PacketList;
	PNDIS_PACKET                FirstPendingPacket; // This is head of the queue of packets
	// waiting to be sent to miniport.
	PNDIS_PACKET                ReturnPacketsQueue;

	//
	// Space used for temp. use during request processing
	//
	ULONG                       RequestBuffer;
	PVOID                       Reserved26;

	PNDIS_MINIPORT_BLOCK        PrimaryMiniport;
	FUNC                        NextCancelSendNetBufferListsHandler;

	//
	// context to pass to bus driver when reading or writing config space
	//
	PVOID                       OidContext;
	ULONG                       SupportedOidListLength;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST           Resources;

	//
	// Watch-dog timer
	//
	NDIS_TIMER                  WakeUpDpcTimer;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	// Note:
	// the following two fields should be explicitly UNICODE_STRING because
	// under Win9x the NDIS_STRING is an ANSI_STRING
	//
	union{
		UNICODE_STRING              Reserved20;
		struct
		{
			NET_IF_MEDIA_CONNECT_STATE MiniportMediaConnectState;
			NET_IF_MEDIA_DUPLEX_STATE MiniportMediaDuplexState;
		};		
	};
	UNICODE_STRING              SymbolicLinkName;

	//
	// Check for hang stuff
	//
	ULONG                       CheckForHangSeconds;
	USHORT                      CFHangTicks;
	USHORT                      CFHangCurrentTick;

	//
	// Reset information
	//
	NDIS_STATUS                 ResetStatus;
	NDIS_HANDLE                 ResetOpen;

	//
	// Holds media specific information.
	//
#ifdef __cplusplus
	FILTERDBS                   FilterDbs;          // EXPOSED via macros. Do not move
#else
	FILTERDBS;                                      // EXPOSED via macros. Do not move
#endif

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER    SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER   SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER   ResetCompleteHandler;

	NDIS_MEDIUM                 MediaType;

	//
	// contains mini-port information
	//
	ULONG                       AutoNegotiationFlags;
	NDIS_INTERFACE_TYPE         Reserved5;
	NDIS_INTERFACE_TYPE         AdapterType;

	union{
		PDEVICE_OBJECT              Reserved6;
		NET_IF_MEDIA_CONNECT_STATE	MediaConnectState;
	};
	union{
		PDEVICE_OBJECT              Reserved7;
		NET_IF_MEDIA_DUPLEX_STATE	MediaDuplexState;
	};
	PULONG                      SupportedOidList;


	PNDIS_SG_DMA_BLOCK          MiniportSGDmaBlock;

	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST               CallMgrAfList;

	PVOID                       MiniportThread;
	PVOID                       SetInfoBuf;
	USHORT                      SetInfoBufLen;
	USHORT                      MaxSendPackets;

	//
	//  Status code that is returned from the fake handlers.
	//
	NDIS_STATUS                 FakeStatus;

	PVOID                       Reserved24;        // For the filter lock

	PNDIS_RECEIVE_SCALE_PARAMETERS CombinedNdisRSSParameters;
	union{
		PNDIS_MINIPORT_TIMER TimerQueue;
		PVOID TimerObjectQueue;
	};
	UINT                        MacOptions;

	//
	// RequestInformation
	//
	PNDIS_REQUEST               PendingRequest;
	UINT                        MaximumLongAddresses;
	UINT                        Reserved27;
	UINT                        MiniportCurrentLookahead;
	UINT                        MiniportMaximumLookahead;

	//
	//  For efficiency
	//
	union{
		ULONG_PTR                   Reserved1;
		PX_FILTER                   NullMediaFilter;
	};
	W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER      SendPacketsHandler;
	NDIS_M_START_SENDS          DeferredSendHandler;

	//
	// The following cannot be unionized.
	//
	ETH_RCV_INDICATE_HANDLER    EthRxIndicateHandler;   // EXPOSED via macros. Do not move
	PVOID     Reserved30;    // EXPOSED via macros. Do not move
	FUNC                        NextSendNetBufferListsHandler;

	ETH_RCV_COMPLETE_HANDLER    EthRxCompleteHandler;   // EXPOSED via macros. Do not move
	PVOID     Reserved31;    // EXPOSED via macros. Do not move
	FUNC                        SavedNextSendNetBufferListsHandler;

	NDIS_M_STATUS_HANDLER       StatusHandler;          // EXPOSED via macros. Do not move
	NDIS_M_STS_COMPLETE_HANDLER StatusCompleteHandler;  // EXPOSED via macros. Do not move
	NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;      // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER QueryCompleteHandler;   // EXPOSED via macros. Do not move
	NDIS_M_REQ_COMPLETE_HANDLER SetCompleteHandler;     // EXPOSED via macros. Do not move

	NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;// EXPOSED via macros. Do not move
	WAN_RCV_HANDLER             WanRcvHandler;          // EXPOSED via macros. Do not move
	WAN_RCV_COMPLETE_HANDLER    WanRcvCompleteHandler;  // EXPOSED via macros. Do not move

	/********************************************************************************************/
	/****************                                                                  **********/
	/**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
	/**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
	/****************                                                                  **********/
	/********************************************************************************************/

	FUNC  IndicateNetBufferListsHandler;
	FUNC  IndicateNetBufferListsContext;
	PVOID endNetBufferListsCompleteHandler;
	PKEVENT PnPEventLockEvent;
	PNDIS_OBJECT_HEADER MediaSpecificAttributes;
	PIRP PendingQueryPowerIrp;

	PNDIS_INTERRUPT_BLOCK InterruptEx;
	UINT64 XmitLinkSpeed;
	UINT64 RcvLinkSpeed;
	NDIS_SUPPORTED_PAUSE_FUNCTIONS PauseFunctions;

	NDIS_SUPPORTED_PAUSE_FUNCTIONS MiniportPauseFunctions;
	ULONG MiniportAutoNegotiationFlags;
	SINGLE_LIST_ENTRY WorkQueue[7];
	SINGLE_LIST_ENTRY SingleWorkItems[6];
	UCHAR SendFlags;

	UCHAR MP6SupportPM;
	UCHAR XState;
	PNDIS_LOG Log;
	PCM_RESOURCE_LIST AllocatedResources;
	PCM_RESOURCE_LIST AllocatedResourcesTranslated;
	SINGLE_LIST_ENTRY PatternList;
	SINGLE_LIST_ENTRY WOLPatternList;
	SINGLE_LIST_ENTRY PMProtocolOffloadList;
	NDIS_PNP_CAPABILITIES PMCapabilities61;
	NDIS_PM_CAPABILITIES_WIN8 PMHardwareCapabilities;
	NDIS_PM_CAPABILITIES_WIN8 PMAdvertisedCapabilities;
	NDIS_PM_PARAMETERS_WIN8 PMCurrentParameters;

	NDIS_PM_PARAMETERS_WIN8 PMWmiParameters;
	DEVICE_CAPABILITIES OriginalDeviceCaps;

	DEVICE_CAPABILITIES DeviceCaps;
	UCHAR S0WakeupSupported;

	ULONG WakeUpEnable;
	PIRP pIrpWaitWake;
	SYSTEM_POWER_STATE WaitWakeSystemState;
	LARGE_INTEGER VcIndex;
	ULONG VcCountLock;
	LIST_ENTRY	WmiEnabledVcs;
	PNDIS_GUID pNdisGuidMap;
	PNDIS_GUID pCustomGuidMap;
	USHORT VcCount;
	USHORT cNdisGuidMap;
	USHORT cCustomGuidMap;
	NDIS_TIMER MediaDisconnectTimer;
	NDIS_PNP_DEVICE_STATE PnPDeviceState;
	NDIS_PNP_DEVICE_STATE OldPnPDeviceState;
	KDPC DeferredDpc;
	LARGE_INTEGER StartTicks;
	PNDIS_PACKET IndicatedPacket;
	PKEVENT RemoveReadyEvent;
	PKEVENT AllOpensClosedEvent;
	PKEVENT AllRequestsCompletedEvent;
	ULONG InitTimeMs;
	NDIS_MINIPORT_WORK_ITEM WorkItemBuffer[6];
	POID_LIST OidList;
	USHORT InternalResetCount;
	USHORT MiniportResetCount;
	USHORT MediaSenseConnectCount;
	USHORT MediaSenseDisconnectCount;
	PNDIS_PACKET *xPackets;
	ULONG UserModeOpenReferences;
	FUNC  SavedIndicateNetBufferListsHandler;
	PVOID SavedIndicateNetBufferListsContext;
	FUNC  WSendPacketsHandler;
	ULONG MiniportAttributes;
	USHORT NumOpens;
	USHORT CFHangXTicks;
	ULONG RequestCount;
	ULONG IndicatedPacketsCount;
	ULONG PhysicalMediumType;
	NDIS_MEDIUM MiniportMediaType;
	PNDIS_REQUEST LastRequest;
	PVOID FakeMac;
	ULONG LockDbg;
	ULONG LockDbgX;
	PVOID LockThread;
	ULONG InfoFlags;
	KSPIN_LOCK TimerQueueLock;
	PKEVENT ResetCompletedEvent;
	PKEVENT QueuedBindingCompletedEvent;
	FUNC  SavedPacketIndicateHandler;
	ULONG RegisteredInterrupts;
	ULONG SetOid;
	KEVENT CFHCompletedEvent;
	PVOID DeviceContext;
#ifdef _X86_
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR CombinedRSSParametersBuf[604];
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR RSSParametersBuf[604];
#else
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR CombinedRSSParametersBuf[720];
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR RSSParametersBuf[720];
#endif
	UCHAR UsingMSIX;
	UCHAR Miniport5InNdis6Mode;
	UCHAR Miniport5HasNdis6Component;
	UCHAR RestoreStackNeeded;
	CHAR MediaChangeFilters;
	UCHAR FilterPnPLockAcquired;
	UCHAR LWFilterAllLoaded;
	UCHAR CheckPacketFilters;
	UCHAR ReceiveFilters;
	PVOID FilterPnPLockThread;
	ULONG FilterPnPLockDbgX;
	KSPIN_LOCK RecvLock;
	NDIS_MINIPORT_STATE RecvState;
	ULONG OutstandingReceives;
	PVOID MiniportRecvLockThread;
	ULONG RecvLockDbg;
	FUNC  NextSendPacketsHandler;
	FUNC  FinalSendPacketsHandler;

	PNDIS_FILTER_BLOCK LowestFilter;
	PNDIS_FILTER_BLOCK HighestFilter;
	PVOID ShutdownContext;
	FUNC  ShutdownHandler;

	KBUGCHECK_CALLBACK_RECORD BugcheckCallbackRecord;
	FUNC  TopIndicateNetBufferListsHandler;
	FUNC  TopIndicateLoopbackNetBufferListsHandler;
	FUNC  Ndis5PacketIndicateHandler;
	FUNC  MiniportReturnPacketHandler;
	PVOID MiniportReturnPacketContext;
	FUNC  SynchronousReturnPacketHandler;
	PVOID SynchronousReturnPacketContext;

	LIST_ENTRY OidRequestList;
	PNDIS_OID_REQUEST PendingOidRequest;
	PNDIS_OID_REQUEST ProcessedOidRequest;

	PVOID NextCoOidRequestHandle;
	ULONG Ndis6ProtocolsBound;
	ULONG PmodeOpen6;
	NDIS_MINIPORT_STATE State;
	PKEVENT AsyncOpCompletionEvent;
	LONG AsyncOpCompletionStatus;
	ULONG_REFERENCE Ref;
	ULONG64 MaxXmitLinkSpeed;
	ULONG64 MaxRcvLinkSpeed;
	ULONG SupportedPacketFilters;
	NDIS_MINIPORT_HANDLERS_WIN8 NoFilter;
	NDIS_MINIPORT_HANDLERS_WIN8 Next;
	LONG NumOfPauseRestartRequests;
	ULONG FilterPnPFlags;
	ULONG SupportedStatistics;
	ULONG cDpcSendCompletes;
	ULONG cDpcRcvIndications;
	ULONG cDpcRcvIndicationCalls;
	ULONG cDpcNbSendCompletes;
	ULONG cDpcNblSendCompletes;
	ULONG cDpcNblIndications;
	ULONG cDpcMaxPacketsIndicated;
	ULONG cDpcTotalDpcCount;
	NDIS_RECEIVE_SCALE_CAPABILITIES_WIN8 RecvScaleCapabilities;
	PNDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES GeneralAttributes;
	LIST_ENTRY PortList;
	PUCHAR AllocatedPortIndices;
	ULONG AllocatedPortIndicesLength;
	ULONG NumberOfPorts;
	ULONG NumberOfActivePorts;
	ULONG64 MiniportXmitLinkSpeed;
	ULONG64 MiniportRcvLinkSpeed;
	KEVENT RestoreStackTimerEvent;
	NDIS_TIMER RestoreStackTimer;
	NDIS_WORK_ITEM RestoreStackWorkItem;
	ULONG RestoreStackBindRefCount;
	ULONG ProtocolsToBind;
	FUNC  DpcTrackers;
	FUNC  CurrentDpcTracker;
	UCHAR DpcTrackerIndex;
	UCHAR SupportedRss;

	BOOLEAN RestoreStackTimerWasSet;
	ULONG64 DpcWatchDogCycleCount;
	ULONG DataBackFillSize;
	ULONG ContextBackFillSize;

	NDIS_RESTART_GENERAL_ATTRIBUTES_WIN8 TopFilterRestartAttributes;
	NDIS_RECEIVE_SCALE_CAPABILITIES_WIN8 TopRecvScaleCapabilities;
	LONG NumOfOpenPauseRestartRequests;
	LONG MiniportStackPauseCount;
	ULONG NsiOpenReferences;

	PVOID ProcessingOpen;
	PKEVENT NsiRequestsCompletedEvent;
	DEVICE_POWER_STATE QueryPowerDeviceState;
	UCHAR MinimumNdisMajorVersion;
	UCHAR MinimumNdisMinorVersion;

	PVOID BottomIfStats;
	NDIS_M_PERIODIC_RECEIVES_WIN8 PeriodicReceiveQueue;
	NDIS_PORT_CONTROLL_STATE DefaultSendControlState;
	NDIS_PORT_CONTROLL_STATE DefaultRcvControlState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultSendAuthorizationState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultRcvAuthorizationState;
	NDIS_PORT_CONTROLL_STATE DefaultPortSendControlState;
	NDIS_PORT_CONTROLL_STATE DefaultPortRcvControlState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultPortSendAuthorizationState;
	NDIS_PORT_AUTHORIZATION_STATE DefaultPortRcvAuthorizationState;
	NDIS_PCI_DEVICE_CUSTOM_PROPERTIES PciDeviceCustomProperties;
	FUNC TopNdis5PacketIndicateHandler;
	PULONG ndisSupportedOidList;
	ULONG ndisSupportedOidListLength;
	ULONG MsiIntCount;
	WORK_QUEUE_ITEM MiniportDpcWorkItem;
	ULONG64 InvalidFrames;
	LONG PagingPathCount;
	LIST_ENTRY ReceiveQueueList;
	PUCHAR AllocatedQueueIndices;
	ULONG AllocatedQueueIndicesLength;
	ULONG NumReceiveQueues;
	LIST_ENTRY ReceiveFilterList;
	PUCHAR AllocatedReceiveFilterIndices;
	ULONG AllocatedReceiveFilterIndicesLength;
	ULONG NumReceiveFilters;
	ULONG EnabledReceiveFilterTypes;
	ULONG EnabledReceiveQueueTypes;
	PNDIS_RECEIVE_FILTER_CAPABILITIES ReceiveFilterHwCapabilities;
	PNDIS_RECEIVE_FILTER_CAPABILITIES ReceiveFilterCurrentCapabilities;

	PNDIS_RECEIVE_FILTER_CAPABILITIES TopReceiveFilterCurrentCapabilities;

	PNDIS_NIC_SWITCH_CAPABILITIES NicSwitchHwCapabilities;
	PNDIS_NIC_SWITCH_CAPABILITIES NicSwitchCurrentCapabilities;

	PNDIS_NIC_SWITCH_CAPABILITIES TopNicSwitchCurrentCapabilities;
	BOOLEAN QueueAllocationAllowed;

	LIST_ENTRY SharedMemoryBlockList;
	FUNC  AllocateSharedMemoryHandler;
	FUNC  FreeSharedMemoryHandler;
	PVOID AllocateSharedMemoryContext;

	PCI_MSI_CAPABILITY MsiCaps;
	PCI_MSIX_CAPABILITY MsiXCaps;
	ULONG64 NumberOfIndirectionTableChanges;
	ULONG PciDeviceSriovSupport;
	union{
		UCHAR BeginSavedFields;
		ULONG NumUserOpens;
	};
	NDIS_MINIPORT_OFFLOAD_REG OffloadRegistry;
	USHORT MediaDisconnectTimeOut;
	USHORT SGMapRegistersNeeded;
	ULONG DriverVerifyFlags;
	PGET_SET_DEVICE_DATA  SetBusData;
	PGET_SET_DEVICE_DATA  GetBusData;
	PVOID BusDataContext;
	NDIS_INTERFACE_TYPE BusType;
	ULONG BusNumber;
	ULONG SlotNumber;
	NDIS_EVENT OpenReadyEvent;
	ULONG NumAdminOpens;
	PVOID DriverHandle;
	PNDIS_BIND_PATHS BindPaths;
	PNDIS_BIND_PATHS LWFilterList;
	PVOID ProtocolsFailToBind;
	ULONG MiniportPhysicalMediumType;
	PVOID LWFilterAttachList;
	ULONG NumFilters;

	PVOID SecurityDescriptor;
	UNICODE_STRING BaseName;
	UNICODE_STRING MiniportName;
	PDEVICE_OBJECT DeviceObject;
	PDEVICE_OBJECT PhysicalDeviceObject;
	PDEVICE_OBJECT NextDeviceObject;
	PNDIS_MINIPORT_BLOCK NextGlobalMiniport;
	PUNICODE_STRING pAdapterInstanceName;
	ULONG PnPCapabilities;
	DEVICE_POWER_STATE CurrentDevicePowerState;

	KSPIN_LOCK PowerStateLock;
	DEVICE_POWER_STATE DriverPowerState;

	PNET_BUFFER_LIST LowPowerRcvReturnNbls;
	ULONG PendingReturnNBLCount;
	PNDIS_NBL_TRACKER_HANDLE LowPowerRcvReturnNblTracker;

	NDIS_PM_ADMIN_GONFIG PMAdminConfig;
	KSEMAPHORE PMPatternSemaphore;
	KSEMAPHORE PMOffloadSemaphore;
	PVOID BusInterface;
	USHORT InstanceNumber;
	PVOID ConfigurationHandle;
	GUID InterfaceGuid;

	NET_LUID_LH NetLuid;
	BOOLEAN IfBlockAvailable;
	PVOID IfBlock;
	PKEVENT IfBlockPointerRefZeroEvent;
	ULONG IfIndex;
	NET_IF_ADMIN_STATUS AdminStatus;
	NET_IF_OPER_STATUS OperStatus;
	ULONG OperStatusFlags;
	union{
		FUNC  SendHandler;
		FUNC  WanSendHandler;
	};

	NDIS_NBL_TRACKER_HANDLE NblTracker;
	PVOID Offload;
	PVOID NDKBlock;
	PVOID AddDeviceContext;
	IO_REMOVE_LOCK_NODBG RemoveLock;
	UNICODE_STRING DevinterfaceNetSymbolicLinkName;
	UCHAR RssEnable;
	UCHAR ChimneyEnable;
	ULONG PhysicalMediumInInf;
	UNICODE_STRING ExportName;
	UNICODE_STRING FilterClass;
	PVOID StatusProcessingThread;
	ULONG StatusProcessingDbgX;
	UNICODE_STRING FdoName;

	KEVENT PowerD0CompleteEvent;
	LONG LastD0CompleteStatus;
	NDIS_MINIPORT_EVENT LastD0Reason;

	ULONG DirectOidRequestCount;
	PKEVENT AllDirectRequestsCompletedEvent;
	PVOID HDSplitCurrentConfig;
	PVOID MSIXConfigContext;
	PVOID SetMSIXTableEntry;
	PVOID MaskMSIXTableEntry;
	PVOID UnmaskMSIXTableEntry;
	WORK_QUEUE_ITEM DevicePowerStateWorkItem;
	WORK_QUEUE_ITEM SystemPowerStateWorkItem;
	PVOID DefaultReceiveQueue;
	ULONG MaxNumRssProcessors;
	PROCESSOR_NUMBER RssBaseProcessor;

	PROCESSOR_NUMBER RssMaxProcessor;
	ULONG RssProfile;

	USHORT NumaNodeId;
	PUSHORT NumaDistances;
	PUNICODE_STRING pModifiedInstanceName;
	WORK_QUEUE_ITEM DeleteMiniportWorkItem;

	ULONG SyncFlags;

}NDIS_MINIPORT_BLOCK_WIN8,*PNDIS_MINIPORT_BLOCK_WIN8;

typedef struct _NDIS51_MINIPORT_CHARACTERISTICS
{
	NDIS50_MINIPORT_CHARACTERISTICS;
	//
	// Extensions for NDIS 5.1
	//
	W_CANCEL_SEND_PACKETS_HANDLER   CancelSendPacketsHandler;
	W_PNP_EVENT_NOTIFY_HANDLER      PnPEventNotifyHandler;
	W_MINIPORT_SHUTDOWN_HANDLER     AdapterShutdownHandler;
	PVOID                           Reserved1;
	PVOID                           Reserved2;
	PVOID                           Reserved3;
	PVOID                           Reserved4;
} NDIS51_MINIPORT_CHARACTERISTICS;

typedef struct _WAN_IP_INFO_W2K
{
	ULONG AdapterType; // defined below
	ULONG SeverData;
	ULONG LocalIpAddress;
	ULONG IpSubnetMask;
	ULONG RemoteIpAddress;
	ULONG Unused[2];
} WAN_IP_INFO_W2K, *PWAN_IP_INFO_W2K;


typedef struct _WAN_IP_INFO_NT
{ // LITTLE_HACK: it is undocumented contents of protocol buffer for IP adapter type, standard NT 4.0 RAS 
	ULONG AdapterType; // 1 - client, 0 - server 
	ULONG IpAddress; // client IP address DWORD Zero; // ??? 
} WAN_IP_INFO_NT, *PWAN_IP_INFO_NT; 

typedef struct _WAN_IP_INFO_VISTA
{
	CHAR Unknown[584];
	ULONG LocalIpAddress;
	ULONG IpSubnetMask;
	ULONG RemoteIpAddress;

}WAN_IP_INFO_VISTA,*PWAN_IP_INFO_VISTA;

#endif //__NDIS_X_H_