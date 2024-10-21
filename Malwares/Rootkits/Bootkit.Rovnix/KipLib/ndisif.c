//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: ndisif.c
// $Revision: 81 $
// $Date: 2012-01-18 19:58:07 +0400 (Ср, 18 янв 2012) $
// description: 
//	ndis interface functions


#include "project.h"
#include "ndisif.h"
#include "ndishook.h"
#include "protocol.h"

//disable warning about deprecated functions
#pragma warning(disable : 4996)

#define MODULE_TAG 'FINK'

//list of loaded drivers
static LIST_ENTRY g_MiniportList;
static LIST_ENTRY g_AdapterList;
static NDIS_SPIN_LOCK g_NdisGlobalLock;
static LONG g_RequestID = 0; // request id for NdisOidRequest
#define NPROT_GET_NEXT_CANCEL_ID() (PVOID)InterlockedIncrement(&g_RequestID)

PNDIS_ALLOCATE_NET_BUFFER_LIST NdisAllocateNetBufferListPtr = NULL;
PNDIS_ALLOCATE_NET_BUFFER_LIST_POOL NdisAllocateNetBufferListPoolPtr = NULL;
PNDIS_FREE_NET_BUFFER_LIST NdisFreeNetBufferListPtr = NULL;
PNDIS_FREE_NET_BUFFER_LIST_POOL NdisFreeNetBufferListPoolPtr = NULL;
PNDIS_M_INDICATE_RECEIVE_NET_BUFFER_LISTS NdisMIndicateReceiveNetBufferListsPtr = NULL;

NTSTATUS
	NDIS_InitializeInterface(
		VOID
		);

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, NDIS_InitializeInterface)
#endif

//***********************************************************************************
// Name: NDIS_InitializeInterface
//
// Routine Description: 
//	Initializes NDIS interface support
//
// Return Value: 
//		None.
//
// Parameters:
//	None.
//***********************************************************************************

NTSTATUS
	NDIS_InitializeInterface(
		VOID
		)
{
	NTSTATUS nStatus;
	// initialize ndis interface
	InitializeListHead ( &g_MiniportList );
	InitializeListHead ( &g_AdapterList );

	NdisAllocateSpinLock ( &g_NdisGlobalLock );

	OsGetVersion();

	// load ndis6 functions
	//if ( OsGetMajorVersion() >= 6 ){
	//	PVOID NdisBase = GetModuleBase ( "ndis.sys" );
	//	if ( NdisBase ){
	//		NdisAllocateNetBufferListPtr     = (PNDIS_ALLOCATE_NET_BUFFER_LIST)BkGetFunctionAddress(NdisBase, "NdisAllocateNetBufferList");
	//		NdisAllocateNetBufferListPoolPtr = (PNDIS_ALLOCATE_NET_BUFFER_LIST_POOL)BkGetFunctionAddress(NdisBase, "NdisAllocateNetBufferListPool");
	//		NdisFreeNetBufferListPtr         = (PNDIS_FREE_NET_BUFFER_LIST)BkGetFunctionAddress(NdisBase, "NdisFreeNetBufferList");
	//		NdisFreeNetBufferListPoolPtr     = (PNDIS_FREE_NET_BUFFER_LIST_POOL)BkGetFunctionAddress(NdisBase, "NdisFreeNetBufferListPool");
	//		NdisMIndicateReceiveNetBufferListsPtr = (PNDIS_M_INDICATE_RECEIVE_NET_BUFFER_LISTS)BkGetFunctionAddress(NdisBase, "NdisMIndicateReceiveNetBufferLists");
	//	}
	//}

	nStatus = ProtocolInitializeSubsystem();
	if ( nStatus != STATUS_SUCCESS ){
		DBGLOG(( LCriticalError, "%08X = ProtocolInitializeSubsystem failed\n", nStatus));
		NdisFreeSpinLock ( &g_NdisGlobalLock );
		return nStatus;
	}
	return nStatus;
}

VOID
	NDIS_ReleaseInterface(
		VOID
		)
{
	ProtocolReleaseSubsystem();
	NdisFreeSpinLock ( &g_NdisGlobalLock );
}

//////////////////////////////////////////////////////////////////////////
// NIC MINIPORT LIST MANAGMENT FUNCTIONS

//***********************************************************************************
// Name: NDIS_AllocateAdapter
//
// Routine Description: 
//	Allocates and initializes new adapter entry.
//
// Return Value: 
//		Pointer to adapter structure
//
// Parameters:
//	None.
//***********************************************************************************
PKIP_NDIS_ADAPTER NDIS_AllocateAdapter ( IN PKIP_NDIS_MINIPORT pMiniport )
{
	PKIP_NDIS_ADAPTER		pAdapter = NULL;
	NDIS_STATUS				nStatus;
	NDIS_PHYSICAL_ADDRESS	noMaxAddr = NDIS_PHYSICAL_ADDRESS_CONST(-1,-1);

	// Allocate new ADAPTER_ENTRY from non-paged memory
	pAdapter = 
		( PKIP_NDIS_ADAPTER )
			ExAllocatePoolWithTag( 
				NonPagedPool, 
				sizeof ( KIP_NDIS_ADAPTER ), 
				MODULE_TAG 
				);

	if ( pAdapter == NULL ){
		DBGLOG(( LCriticalError, "ExAllocatePoolWithTag(%lu) failed\n", sizeof ( KIP_NDIS_ADAPTER ) ));
		return NULL;
	}

	NdisZeroMemory (pAdapter, sizeof ( KIP_NDIS_ADAPTER ));
	NdisAllocateSpinLock( &pAdapter->Interface.Lock );
	pAdapter->Interface.Miniport = pMiniport;
	pAdapter->Interface.Type = IF_TYPE_NIC;
	pAdapter->Interface.MTU = MAX_802_3_LENGTH;
	pAdapter->Interface.Speed = 0;
	pAdapter->Interface.HWAddressLength = ETHARP_HWADDR_LEN;
	pAdapter->Interface.Protocol.pInterface = (PKIP_NDIS_INTERFACE)pAdapter;

	NdisInitializeListHead( &pAdapter->RasLinkList );

	// initially set to disconnected state
	KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);

	// set initial power state to D0
	pAdapter->MPDeviceState = NdisDeviceStateD0;

	pAdapter->MaxFrameSize = MAX_ETHER_SIZE;
	pAdapter->MinFrameSize = MIN_ETHER_SIZE;
	pAdapter->MaxSendPackets = 1;

	// ndis requests
	if ( pMiniport->MajorNdisVersion >= 6 ){
		//NdisInitializeEvent( &pAdapter->SetInformationRequest.Event   );
	}else{
		NdisInitializeEvent( &pAdapter->Interface.SetInformationRequest.Event   );
		NdisInitializeEvent( &pAdapter->Interface.QueryInformationRequest.Event );
	}

	// put it to global list

	//save adapter in global list
	NdisAcquireSpinLock ( &g_NdisGlobalLock );
	InsertHeadList( &pMiniport->AdapterList, &pAdapter->MiniportLink );
	InsertHeadList( &g_AdapterList, &pAdapter->Interface.Link );
	NdisReleaseSpinLock ( &g_NdisGlobalLock );


	return pAdapter;
}

//***********************************************************************************
//  Name: NDIS_FreeAdapter
// 
//	Routine Description: 
//		This routine deallocates KIP_NDIS_ADAPTER structure 
//
//	Return Value: None
//
//	Parameters: 
//		pMiniport - pointer to allocated KIP_NDIS_ADAPTER structure
//	
//  NOTE: None	
//***********************************************************************************

VOID NDIS_FreeAdapter ( IN PKIP_NDIS_ADAPTER pAdapter )
{
	if ( pAdapter ){
		// remove it from global lists
		NdisAcquireSpinLock ( &g_NdisGlobalLock );
		RemoveEntryList( &pAdapter->MiniportLink );
		RemoveEntryList( &pAdapter->Interface.Link );
		NdisReleaseSpinLock ( &g_NdisGlobalLock );

		// Free adapter entry structure
		ExFreePoolWithTag( pAdapter, MODULE_TAG );
	}
}
//***********************************************************************************
// Name: NDIS_FindAdapterByHandle
//
// Description: 
//		Routine finds adapter by adapter handle
//
// Return value: 
//		pointer to adapter if found, NULL otherwise
//
// Parameters: 
//	MiniportHandle	- adapter handle
//
// NOTE: None
// **********************************************************************************

PKIP_NDIS_ADAPTER 
	NDIS_FindAdapterByHandle (
		IN NDIS_HANDLE MiniportHandle
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_NDIS_ADAPTER	pAdapter;
	
	NdisAcquireSpinLock( &g_NdisGlobalLock );

	// Walk the list of adapters
	for ( ListEntry = g_AdapterList.Flink; ListEntry != &g_AdapterList; ListEntry = ListEntry->Flink )
	{
		pAdapter = CONTAINING_RECORD ( ListEntry, KIP_NDIS_ADAPTER, Interface.Link );
		if ( pAdapter->NdisMiniportHandle == MiniportHandle ){
			// If adapter found return it
			break;
		}
		pAdapter = NULL;
	}

	NdisReleaseSpinLock( &g_NdisGlobalLock );
	return pAdapter;
}
//***********************************************************************************
// Name: NDIS_FindAdapterByAdapterContext
//
// Description: 
//		Routine finds adapter by adapter context
//
// Return value: 
//		pointer to adapter if found, NULL otherwise
//
// Parameters: 
//	MiniportHandle	- adapter handle
//
// NOTE: None
// **********************************************************************************

PKIP_NDIS_ADAPTER 
	NDIS_FindAdapterByAdapterContext (
		IN NDIS_HANDLE MiniportAdapterContext
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_NDIS_ADAPTER	pAdapter;
	
	NdisAcquireSpinLock( &g_NdisGlobalLock );

	// Walk the list of adapters
	for ( ListEntry = g_AdapterList.Flink; ListEntry != &g_AdapterList; ListEntry = ListEntry->Flink )
	{
		pAdapter = CONTAINING_RECORD ( ListEntry, KIP_NDIS_ADAPTER, Interface.Link );
		if ( pAdapter->NdisMiniportAdapterContext == MiniportAdapterContext ){
			// If adapter found return it
			break;
		}
		pAdapter = NULL;
	}

	NdisReleaseSpinLock( &g_NdisGlobalLock );
	return pAdapter;
}

//***********************************************************************************
// Name: NDIS_FindAdapterByPhysicalDevice
//
// Description: 
//		Routine finds adapter by physical device
//
// Return value: 
//		pointer to adapter if found, NULL otherwise
//
// Parameters: 
//	MiniportHandle	- adapter handle
//
// NOTE: None
// **********************************************************************************

PKIP_NDIS_ADAPTER 
	NDIS_FindAdapterByDeviceObject (
		IN PKIP_NDIS_MINIPORT pMiniport,
		IN PDEVICE_OBJECT DeviceObject
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_NDIS_ADAPTER	pAdapter = NULL;
	
	NdisAcquireSpinLock( &g_NdisGlobalLock );

	// Walk the list of adapters
	for ( ListEntry = pMiniport->AdapterList.Flink; ListEntry != &pMiniport->AdapterList; ListEntry = ListEntry->Flink )
	{
		PDEVICE_OBJECT Devobj = NULL;
		pAdapter = CONTAINING_RECORD ( ListEntry, KIP_NDIS_ADAPTER, MiniportLink );
		Devobj = NDIS_GetMiniportDeviceObject( pAdapter->NdisMiniportHandle );
		if ( Devobj && DeviceObject == Devobj ){
			// If adapter found return it
			break;
		}
		pAdapter = NULL;
	}

	NdisReleaseSpinLock( &g_NdisGlobalLock );
	return pAdapter;
}
//***********************************************************************************
// Name: NDIS_AllocateRasLink
//
// Routine Description: 
//	Allocates and initializes new WAN link entry.
//
// Return Value: 
//		Pointer to ras link structure
//
// Parameters:
//	None.
//***********************************************************************************
PKIP_RAS_LINK 
	NDIS_AllocateRasLink ( 
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WAN_LINE_UP pWanLineUp
		)
{
	PKIP_RAS_LINK			pRasLink = NULL;
	NDIS_STATUS				nStatus;
	NDIS_PHYSICAL_ADDRESS	noMaxAddr = NDIS_PHYSICAL_ADDRESS_CONST(-1,-1);

	// Allocate new ADAPTER_ENTRY from non-paged memory
	nStatus = 
		NdisAllocateMemory (
			&pRasLink,
			sizeof ( KIP_RAS_LINK ),
			0,
			noMaxAddr
			);

	if ( nStatus != NDIS_STATUS_SUCCESS || !pRasLink ){
		DBGLOG(( LCriticalError, "%08X = NdisAllocateMemory(%lu) failed\n", nStatus, sizeof ( KIP_RAS_LINK ) ));
		return NULL;
	}

	NdisZeroMemory (pRasLink, sizeof ( KIP_RAS_LINK ));

	NdisAllocateSpinLock( &pRasLink->Interface.Lock );
	pRasLink->Interface.Miniport = pAdapter->Interface.Miniport;
	pRasLink->Interface.Type  = IF_TYPE_WAN;
	pRasLink->Interface.MediaType = NdisMedium802_3;
	pRasLink->Interface.MTU   = (USHORT)pWanLineUp->MaximumTotalSize;
	pRasLink->Interface.Speed = pWanLineUp->LinkSpeed;
	pRasLink->Interface.HWAddressLength = ETHARP_HWADDR_LEN;
	pRasLink->Interface.Protocol.pInterface = (PKIP_NDIS_INTERFACE)pRasLink;
	NdisMoveMemory(pRasLink->Interface.CurrentMacAddress, pWanLineUp->LocalAddress, ETHARP_HWADDR_LEN);

	pRasLink->pAdapter = pAdapter;
	NdisMoveMemory(pRasLink->RemoteAddress, pWanLineUp->RemoteAddress, ETHARP_HWADDR_LEN);
	pRasLink->ProtocolBufferLength = pWanLineUp->ProtocolBufferLength;
	if (pWanLineUp->ProtocolBufferLength > RAS_LINK_BUFFER_LENGTH){
		NdisMoveMemory(pRasLink->ProtocolBuffer, pWanLineUp->ProtocolBuffer, RAS_LINK_BUFFER_LENGTH);
	}else{
		NdisMoveMemory(pRasLink->ProtocolBuffer, pWanLineUp->ProtocolBuffer, pWanLineUp->ProtocolBufferLength);
	}
	pRasLink->NdisLinkHandle = (NDIS_HANDLE)InterlockedIncrement(&pAdapter->RasCount);

	//link it to adapter
	KIP_LOCK_ADAPTER ( pAdapter );
	InsertHeadList(&pAdapter->RasLinkList,&pRasLink->Interface.Link);
	KIP_UNLOCK_ADAPTER ( pAdapter );
	return pRasLink;
}
//***********************************************************************************
//  Name: NDIS_FreeRasLink
// 
//	Routine Description: 
//		This routine deallocates KIP_RAS_LINK structure 
//
//	Return Value: None
//
//	Parameters: 
//		pAdapter - pointer to allocated KIP_NDIS_ADAPTER structure
//	
//  NOTE: None	
//***********************************************************************************
VOID NDIS_FreeRasLink ( IN PKIP_NDIS_ADAPTER pAdapter, IN PKIP_RAS_LINK pLink )
{
	// remove link from list
	KIP_LOCK_ADAPTER ( pAdapter );
	RemoveEntryList( &pLink->Interface.Link );
	KIP_UNLOCK_ADAPTER ( pAdapter );

	// Free link structure
	NdisFreeMemory ( pLink, sizeof (KIP_RAS_LINK), 0 );
}
//***********************************************************************************
//  Name: NDIS_FindRasLinkByHandle
// 
//	Routine Description: 
//		Finds ras link object by ndis ras link handle 
//
//	Return Value: None
//
//	Parameters: 
//		pAdapter - pointer to allocated KIP_NDIS_ADAPTER structure
//		LinkHandle - link handle
//	
//  NOTE: None	
//***********************************************************************************
PKIP_RAS_LINK 
	NDIS_FindRasLinkByHandle ( 
		IN PKIP_NDIS_ADAPTER pAdapter, 
		IN NDIS_HANDLE LinkHandle 
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_RAS_LINK pLink = NULL;

	KIP_LOCK_ADAPTER( pAdapter );

	// Walk the list of adapters
	for ( ListEntry = pAdapter->RasLinkList.Flink; 
		ListEntry != &pAdapter->RasLinkList; 
		ListEntry = ListEntry->Flink )
	{
		pLink = CONTAINING_RECORD ( ListEntry, KIP_RAS_LINK, Interface.Link );
		if ( pLink->NdisLinkHandle == LinkHandle ){
			// If adapter found return it
			break;
		}
		pLink = NULL;
	}

	KIP_UNLOCK_ADAPTER( pAdapter );
	return pLink;
}

//***********************************************************************************
//  Name: NDIS_FindRasLinkByMac
// 
//	Routine Description: 
//		Finds ras link object by local mc address 
//
//	Return Value: None
//
//	Parameters: 
//		pAdapter - pointer to allocated KIP_NDIS_ADAPTER structure
//		MacAddr - local mac
//	
//  NOTE: None	
//***********************************************************************************
PKIP_RAS_LINK 
	NDIS_FindRasLinkByMac ( 
		IN PKIP_NDIS_ADAPTER pAdapter, 
		IN char *MacAddr 
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_RAS_LINK pLink = NULL;

	KIP_LOCK_ADAPTER( pAdapter );

	// Walk the list of adapters
	for ( ListEntry = pAdapter->RasLinkList.Flink; 
		ListEntry != &pAdapter->RasLinkList; 
		ListEntry = ListEntry->Flink )
	{
		pLink = CONTAINING_RECORD ( ListEntry, KIP_RAS_LINK, Interface.Link );
		if ( (RtlCompareMemory(&pLink->Interface.CurrentMacAddress[2], MacAddr+2, 4) == 4) ){
			break;
		}
		pLink = NULL;
	}

	KIP_UNLOCK_ADAPTER( pAdapter );
	return pLink;
}

//***********************************************************************************
// Name: NDIS_AllocateMiniport
//
// Routine Description: 
//	Allocates and initializes new adapter entry.
//
// Return Value: 
//		Pointer to adapter structure
//
// Parameters:
//	None.
//***********************************************************************************
PKIP_NDIS_MINIPORT NDIS_AllocateMiniport ( VOID )
{
	PKIP_NDIS_MINIPORT		pMiniport;
	NDIS_STATUS				nStatus;
	NDIS_PHYSICAL_ADDRESS	noMaxAddr = NDIS_PHYSICAL_ADDRESS_CONST(-1,-1);

	pMiniport = 
		( PKIP_NDIS_MINIPORT )
			ExAllocatePoolWithTag( 
				NonPagedPool, 
				sizeof ( KIP_NDIS_MINIPORT ), 
				MODULE_TAG 
				);

	if ( pMiniport == NULL ){
		DBGLOG(( LCriticalError, "ExAllocatePoolWithTag(%lu) failed\n", sizeof ( KIP_NDIS_MINIPORT ) ));
		return NULL;
	}

	NdisZeroMemory (pMiniport, sizeof ( KIP_NDIS_MINIPORT ));
	InitializeListHead(&pMiniport->Link);
	NdisAllocateSpinLock( &pMiniport->Lock);

	InitializeListHead(&pMiniport->AdapterList);

	// put it in global list
	NdisAcquireSpinLock ( &g_NdisGlobalLock );
	InsertHeadList( &g_MiniportList, &pMiniport->Link );
	NdisReleaseSpinLock ( &g_NdisGlobalLock );

	return pMiniport;
}

//***********************************************************************************
//  Name: NDIS_FreeMiniport
// 
//	Routine Description: 
//		This routine deallocates PKIP_NDIS_MINIPORT structure 
//
//	Return Value: None
//
//	Parameters: 
//		pMiniport - pointer to allocated KIP_NDIS_ADAPTER structure
//	
//  NOTE: None	
//***********************************************************************************

VOID NDIS_FreeMiniport ( PKIP_NDIS_MINIPORT pMiniport )
{
	// put it in global list
	NdisAcquireSpinLock ( &g_NdisGlobalLock );
	while ( !IsListEmpty( &pMiniport->AdapterList ) )
	{
		PLIST_ENTRY ListEntry = RemoveHeadList( &pMiniport->AdapterList );
		PKIP_NDIS_ADAPTER pAdapter = CONTAINING_RECORD( ListEntry, KIP_NDIS_ADAPTER, MiniportLink );

		// TODO: put asserts here
		//remove from global list
		RemoveEntryList( &pAdapter->Interface.Link );

		// Free adapter entry structure
		ExFreePoolWithTag( pAdapter, MODULE_TAG );
	}
	NdisReleaseSpinLock ( &g_NdisGlobalLock );

	// Free driver entry structure
	ExFreePoolWithTag( pMiniport, MODULE_TAG );
}

//***********************************************************************************
// Name: NDIS_FindMiniportByWrapperHandle
//
// Description: 
//		Routine finds adapter by ndis5 wrapper handle
//
// Return value: 
//		pointer to adapter if found, NULL otherwise
//
// Parameters: 
//	MiniportHandle	- adapter handle
//
// NOTE: None
// **********************************************************************************

PKIP_NDIS_MINIPORT
	NDIS_FindMiniportByWrapperHandle (
		IN NDIS_HANDLE NdisWrapperHandle
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_NDIS_MINIPORT pMiniport;
	
	NdisAcquireSpinLock( &g_NdisGlobalLock );

	// Walk the list of adapters
	for ( ListEntry = g_MiniportList.Flink; ListEntry != &g_MiniportList; ListEntry = ListEntry->Flink )
	{
		pMiniport = CONTAINING_RECORD ( ListEntry, KIP_NDIS_MINIPORT, Link );
		if ( pMiniport->MajorNdisVersion <= 5 && pMiniport->Ndis5.NdisWrapperHandle == NdisWrapperHandle ){
			// If adapter found return it
			break;
		}
		pMiniport = NULL;
	}

	NdisReleaseSpinLock( &g_NdisGlobalLock );
	return pMiniport;
}

//***********************************************************************************
// Name: NDIS_FindMiniportByDriverHandle
//
// Description: 
//		Routine finds adapter by ndis6 driver handle
//
// Return value: 
//		pointer to adapter if found, NULL otherwise
//
// Parameters: 
//	MiniportHandle	- adapter handle
//
// NOTE: None
// **********************************************************************************

PKIP_NDIS_MINIPORT
	NDIS_FindMiniportByDriverHandle (
		IN NDIS_HANDLE NdisDriverHandle
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_NDIS_MINIPORT pMiniport;
	
	NdisAcquireSpinLock( &g_NdisGlobalLock );

	// Walk the list of adapters
	for ( ListEntry = g_MiniportList.Flink; ListEntry != &g_MiniportList; ListEntry = ListEntry->Flink )
	{
		pMiniport = CONTAINING_RECORD ( ListEntry, KIP_NDIS_MINIPORT, Link );
		if ( pMiniport->MajorNdisVersion >= 6 && pMiniport->Ndis6.NdisMiniportDriverHandle == NdisDriverHandle){
			// If adapter found return it
			break;
		}
		pMiniport = NULL;
	}

	NdisReleaseSpinLock( &g_NdisGlobalLock );
	return pMiniport;
}

//***********************************************************************************
// Name: NDIS_FindMiniportByMiniportDriverContext
//
// Description: 
//		Routine finds adapter by ndis6 driver context
//
// Return value: 
//		pointer to adapter if found, NULL otherwise
//
// Parameters: 
//	MiniportHandle	- adapter handle
//
// NOTE: None
// **********************************************************************************

PKIP_NDIS_MINIPORT
	NDIS_FindMiniportByMiniportDriverContext (
		IN NDIS_HANDLE  MiniportAdapterContext
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_NDIS_MINIPORT pMiniport;
	
	NdisAcquireSpinLock( &g_NdisGlobalLock );

	// Walk the list of adapters
	for ( ListEntry = g_MiniportList.Flink; ListEntry != &g_MiniportList; ListEntry = ListEntry->Flink )
	{
		pMiniport = CONTAINING_RECORD ( ListEntry, KIP_NDIS_MINIPORT, Link );
		if ( pMiniport->Ndis6.NdisMiniportAdapterContext == MiniportAdapterContext ){
			// If adapter found return it
			break;
		}
		pMiniport = NULL;
	}

	NdisReleaseSpinLock( &g_NdisGlobalLock );
	return pMiniport;
}

//***********************************************************************************
// Name: NDIS_FindMiniportByDriverObject
//
// Description: 
//		Routine finds adapter by miniport driver object
//
// Return value: 
//		pointer to adapter if found, NULL otherwise
//
// Parameters: 
//	MiniportHandle	- adapter handle
//
// NOTE: None
// **********************************************************************************

PKIP_NDIS_MINIPORT
	NDIS_FindMiniportByDriverObject (
		IN PDRIVER_OBJECT DriverObject
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_NDIS_MINIPORT pMiniport;
	
	NdisAcquireSpinLock( &g_NdisGlobalLock );

	// Walk the list of adapters
	for ( ListEntry = g_MiniportList.Flink; ListEntry != &g_MiniportList; ListEntry = ListEntry->Flink )
	{
		pMiniport = CONTAINING_RECORD ( ListEntry, KIP_NDIS_MINIPORT, Link );
		if ( pMiniport->DriverObject == DriverObject ){
			// If adapter found return it
			break;
		}
		pMiniport = NULL;
	}

	NdisReleaseSpinLock( &g_NdisGlobalLock );
	return pMiniport;
}

//////////////////////////////////////////////////////////////////////////
// NDIS INTERFACE FUNCTIONS

//***********************************************************************************
// Name: NDIS_MQueryInformation
//
// Description: 
//		returns information for adapter
// Return value: 
//		NDIS_STATUS_SUCCESS 
//		MiniportQueryInformation returned the requested information at InformationBuffer 
//		and set the variable at BytesWritten to the amount of information it returned. 
//		NDIS_STATUS_PENDING 
//		The driver will complete the request asynchronously with a call to 
//		NdisMQueryInformationComplete when it has gathered the requested information. 
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	NDIS_MQueryInformation(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_OID  Oid,
		IN PVOID  InformationBuffer,
		IN ULONG  InformationBufferLength,
		OUT PULONG  BytesWritten,
		OUT PULONG  BytesNeeded
		)
{
	PKIP_NDIS_MINIPORT	pMiniport = pAdapter->Interface.Miniport;
	NDISPROT_REQUEST  OidRequest = { 0 };
	NDIS_STATUS Status;	
	UINT Count;
	BOOLEAN CoNdisRequest = FALSE;

	//
	// If the miniport is being reset, then wait for the reset to complete before going any further.
	// Make sure we do not wait indefinitely either
	//

	for (Count = 0; Count < 5000; Count ++)
	{
		if (!KIP_MINIPORT_TEST_FLAG(pAdapter, (fADAPTER_RESET_IN_PROGRESS | fADAPTER_RESET_REQUESTED)))
		{
			break;
		}
		NdisMSleep(1000); // 1 msec
	}

	if ( Count == 5000 )
	{
		return(NDIS_STATUS_RESET_IN_PROGRESS);
	}


	if ( pMiniport->MajorNdisVersion >= 6 ){

		OidRequest.OidRequest.Header.Type = NDIS_OBJECT_TYPE_OID_REQUEST;
		OidRequest.OidRequest.Header.Revision = NDIS_OID_REQUEST_REVISION_1;
		OidRequest.OidRequest.Header.Size = sizeof(NDIS_OID_REQUEST);
		OidRequest.OidRequest.RequestType = NdisRequestQueryInformation;
		OidRequest.OidRequest.PortNumber = 0;
		OidRequest.OidRequest.RequestId = NPROT_GET_NEXT_CANCEL_ID();

		OidRequest.OidRequest.DATA.QUERY_INFORMATION.Oid = Oid;
		OidRequest.OidRequest.DATA.QUERY_INFORMATION.InformationBuffer = InformationBuffer;
		OidRequest.OidRequest.DATA.QUERY_INFORMATION.InformationBufferLength = InformationBufferLength;
		NdisInitializeEvent( &OidRequest.ReqEvent );

		pAdapter->Interface.CurrentRequest = &OidRequest;

		Status =
			pMiniport->Ndis6.MiniportCharacteristics.OidRequestHandler(
				pAdapter->NdisMiniportAdapterContext,
				&OidRequest.OidRequest
				);
		if ( Status == NDIS_STATUS_PENDING ){
			NdisWaitEvent( &OidRequest.ReqEvent, 0 );
			Status = OidRequest.Status;
		}else{
			pAdapter->Interface.CurrentRequest = NULL;
		}
	}else{
		if (KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_IS_CO))
		{
			// TODO: handle connection oriented miniports
			if( pMiniport->Ndis5.MiniportCharacteristics.QueryInformationHandler == NULL )
			{
				OidRequest.NdisRequest.RequestType = NdisRequestQueryInformation;
				OidRequest.NdisRequest.DATA.QUERY_INFORMATION.Oid = Oid;
				OidRequest.NdisRequest.DATA.QUERY_INFORMATION.InformationBuffer = InformationBuffer;
				OidRequest.NdisRequest.DATA.QUERY_INFORMATION.InformationBufferLength = InformationBufferLength;
				NdisInitializeEvent( &OidRequest.ReqEvent );
				pAdapter->Interface.CurrentRequest = &OidRequest;
				CoNdisRequest = TRUE;

				Status = 
					pMiniport->Ndis5.MiniportCharacteristics.CoRequestHandler(
						pAdapter->NdisMiniportAdapterContext,
						NULL,
						&OidRequest.NdisRequest
						);
			}
		}

		if ( !CoNdisRequest ){
			// save the current query request
			pAdapter->Interface.QueryInformationRequest.Internal = TRUE;
			pAdapter->Interface.QueryInformationRequest.Oid = Oid;
			pAdapter->Interface.QueryInformationRequest.InformationBuffer = InformationBuffer;
			pAdapter->Interface.QueryInformationRequest.InformationBufferLength = InformationBufferLength;
			pAdapter->Interface.QueryInformationRequest.BytesWritten = BytesWritten;
			pAdapter->Interface.QueryInformationRequest.BytesNeeded = BytesNeeded;

			Status =
				pMiniport->Ndis5.MiniportCharacteristics.QueryInformationHandler(
					pAdapter->NdisMiniportAdapterContext,
					Oid,
					InformationBuffer,
					InformationBufferLength,
					BytesWritten,
					BytesNeeded
					);
		}

		if ( Status == NDIS_STATUS_PENDING ){
			NdisWaitEvent( &pAdapter->Interface.QueryInformationRequest.Event, 0 );
			Status = pAdapter->Interface.QueryInformationRequest.Status;
		}
	}
	return Status;
}

//***********************************************************************************
// Name: NDIS_MQueryOid
//
// Description: 
//		returns oid value for adapter
// Return value: 
//		NDIS_STATUS_SUCCESS 
//		MiniportQueryInformation returned the requested information at InformationBuffer 
//		and set the variable at BytesWritten to the amount of information it returned. 
//		NDIS_STATUS_PENDING 
//		The driver will complete the request asynchronously with a call to 
//		NdisMQueryInformationComplete when it has gathered the requested information. 
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	NDIS_MQueryOid(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_OID  Oid,
		IN PVOID  InformationBuffer,
		IN ULONG  InformationBufferLength
		)
{
	NDIS_STATUS Status;
	ULONG  BytesWritten;
	ULONG  BytesNeeded;

	return
		NDIS_MQueryInformation(
			pAdapter,
			Oid,
			InformationBuffer,
			InformationBufferLength,
			&BytesWritten,
			&BytesNeeded
			);	
}

//***********************************************************************************
// Name: NDIS_MSetInformation
//
// Description: 
//		queries information from adapter
// Return value: 
//		NDIS_STATUS_SUCCESS 
//		MiniportSetInformation used the data at InformationBuffer to set itself or 
//		its NIC to the state required by the given Oid, and it set the variable at 
//		BytesRead to the amount of supplied data it used. 
//		NDIS_STATUS_PENDING 
//		The driver will complete the request asynchronously with a call to 
//		NdisMSetInformationComplete when it has set itself or its NIC to the state requested. 
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	NDIS_MSetInformation(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_OID  Oid,
		IN PVOID  InformationBuffer,
		IN ULONG  InformationBufferLength,
		OUT PULONG  BytesRead,
		OUT PULONG  BytesNeeded
		)
{
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;

	// save the current set request
	pAdapter->Interface.SetInformationRequest.Internal = TRUE;
	pAdapter->Interface.SetInformationRequest.Oid = Oid;
	pAdapter->Interface.SetInformationRequest.InformationBuffer = InformationBuffer;
	pAdapter->Interface.SetInformationRequest.InformationBufferLength = InformationBufferLength;
	pAdapter->Interface.SetInformationRequest.BytesRead = BytesRead;
	pAdapter->Interface.SetInformationRequest.BytesNeeded = BytesNeeded;

	return pMiniport->Ndis5.MiniportCharacteristics.SetInformationHandler(
		pAdapter->NdisMiniportAdapterContext,
		Oid,
		InformationBuffer,
		InformationBufferLength,
		BytesRead,
		BytesNeeded
		);
}

//***********************************************************************************
// Name: NDIS_TransferData
//
// Description: 
//		forwards a request to copy data received on the underlying NIC into 
//		a protocol-supplied packet.
// Return value: 
//		NDIS_STATUS_PENDING 
//		The request is being handled asynchronously, and the caller's 
//		ProtocolTransferComplete function will be called when it is completed. 
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	NDIS_TransferData(
		IN PKIP_NDIS_ADAPTER pAdapter,
		OUT PNDIS_PACKET            Packet,
		OUT PUINT                   BytesTransferred,
		IN  NDIS_HANDLE             MiniportReceiveContext,
		IN  UINT                    ByteOffset,
		IN  UINT                    BytesToTransfer
		)
{
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
	return pMiniport->Ndis5.MiniportCharacteristics.TransferDataHandler(
		Packet,
		BytesTransferred,
		pAdapter->NdisMiniportAdapterContext,
		MiniportReceiveContext,
		ByteOffset,
		BytesToTransfer
		);
}

VOID
	NDIS_ReturnPacket(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_PACKET  Packets
		)
{
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
	pMiniport->Ndis5.MiniportCharacteristics.ReturnPacketHandler(
		pAdapter->NdisMiniportAdapterContext,
		Packets
		);
}

VOID
	NDIS_ReturnNetBufferList(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNET_BUFFER_LIST  NetBufferList,
		IN BOOLEAN bDispatch
		)
{
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
	pMiniport->Ndis6.MiniportCharacteristics.ReturnNetBufferListsHandler(
		pAdapter->NdisMiniportHandle,
		NetBufferList,
		bDispatch ? NDIS_RETURN_FLAGS_DISPATCH_LEVEL : 0
		);
}

//***********************************************************************************
// Name: NDIS_CopyPacketData
//
// Description: 
//
//		Routine copies data from packet into caller supplied buffer
//
// Return value: Operation status
//
// Parameters: 
//  Packet					- pointer onto packet
//  lpBuffer				- buffer there packet data should be stored
//  nNumberOfBytesToRead	- number of bytes to copy
//  nOffset - offset to start copying (Starting With MAC Header)
//  lpNumberOfBytesRead - caller supplied variable to store number of read bytes
//
// NOTE: None
// **********************************************************************************
BOOLEAN 
	NDIS_CopyPacketData(
		IN PNDIS_PACKET	Packet,
		IN OUT PUCHAR		lpBuffer,
		IN ULONG			nNumberOfBytesToRead,
		IN ULONG			nOffset,                
		IN PULONG			lpNumberOfBytesRead
		)
{
	PNDIS_BUFFER    CurrentBuffer;
	UINT            nBufferCount, TotalPacketLength;
	PUCHAR          VirtualAddress;
	UINT            CurrentLength, CurrentOffset;
	UINT            AmountToMove;

	*lpNumberOfBytesRead = 0;

	//
	// Query packet
	//
	NdisQueryPacket(
		(PNDIS_PACKET )Packet,
		(PUINT )NULL,           // Physical Buffer Count
		(PUINT )&nBufferCount,  // Buffer Count
		&CurrentBuffer,         // First Buffer
		&TotalPacketLength      // TotalPacketLength
		);

	//
	// Query the first buffer
	//
	NdisQueryBufferSafe(
		CurrentBuffer,
		&VirtualAddress,
		&CurrentLength,
		NormalPagePriority
		);

	// If out of memory, do nothing
	if ( !VirtualAddress )
		return FALSE;

	CurrentOffset = 0;

	while( nOffset || nNumberOfBytesToRead )
	{
		while( !CurrentLength )
		{
			NdisGetNextBuffer(
				CurrentBuffer,
				&CurrentBuffer
				);

			// If we've reached the end of the packet.  We return with what
			// we've done so far (which must be shorter than requested).
			if (!CurrentBuffer)
				return TRUE;

			NdisQueryBufferSafe(
				CurrentBuffer,
				&VirtualAddress,
				&CurrentLength,
				NormalPagePriority
				);
			if ( !VirtualAddress )
				return FALSE;

			CurrentOffset = 0;
		}

		if( nOffset )
		{
			// Compute how much data to move from this fragment
			if( CurrentLength > nOffset )
				CurrentOffset = nOffset;
			else
				CurrentOffset = CurrentLength;

			nOffset -= CurrentOffset;
			CurrentLength -= CurrentOffset;
		}

		if( nOffset ) {
			CurrentLength = 0;
			continue;
		}

		if( !CurrentLength ) {
			continue;
		}

		// Compute how much data to move from this fragment
		if (CurrentLength > nNumberOfBytesToRead)
			AmountToMove = nNumberOfBytesToRead;
		else
			AmountToMove = CurrentLength;

		//
		// Copy the data.
		//
		NdisMoveMemory(
			lpBuffer,
			&VirtualAddress[ CurrentOffset ],
			AmountToMove
			);

		//
		// Update destination pointer
		//
		lpBuffer += AmountToMove;

		//
		// Update counters
		//
		*lpNumberOfBytesRead +=AmountToMove;
		nNumberOfBytesToRead -=AmountToMove;
		CurrentLength = 0;
	}
	return TRUE;
}
//***********************************************************************************
// Name: NDIS_FreePacket
//
// Description: 
//		Frees ndis packet and all buffers
//
// Return value: None
//
// NOTE: None
// **********************************************************************************
VOID 
	NDIS_FreePacket( 
		IN PKIP_NDIS_PROTOCOL pProtocol, 
		IN PNDIS_PACKET Packet 
		)
{
   UINT           nDataSize, nBufferCount;
   PNDIS_BUFFER   pBuffer;

   if( !Packet ) {
      return;
   }

   // Query our packet
   NdisQueryPacket(
      Packet,
      NULL,
      &nBufferCount,
      &pBuffer,
      &nDataSize
      );

   // Free all of the packet's buffers
   while( nBufferCount-- > 0L )
   {
      NdisUnchainBufferAtFront ( Packet, &pBuffer );
      NdisFreeBuffer ( pBuffer );
   }

   // Recycle the packet
   NdisReinitializePacket ( Packet ); 

   KIP_LOCK_PACKET_POOL( pProtocol );
   // And free the packet
   NdisFreePacket ( Packet );
   KIP_UNLOCK_PACKET_POOL( pProtocol );
}
//***********************************************************************************
// Name: NDIS_CopyNetBufferToBuffer
//
// Description: 
//		Copies net buffer list to raw buffer
//
// Return value: TRUE if success
//
// NOTE: None
// **********************************************************************************
BOOLEAN
	NDIS_CopyNetBufferToBuffer(
		 IN PNET_BUFFER		pNetBuffer,
		 IN OUT PUCHAR		lpBuffer,
		 IN ULONG			nNumberOfBytesToRead,
		 IN ULONG			nOffset,                
		 OUT PULONG			lpNumberOfBytesRead
		 )
{
	PMDL		pCurrMDL;
	PUCHAR		pCurrData;
	ULONG		ulBufferLength;
	ULONG		CopyDataLength;
	ULONG		CurrBufferOffset = 0;
	ULONG		CurrentNetBufferSize = 0;

	if (pNetBuffer == NULL || lpBuffer == NULL || lpNumberOfBytesRead == NULL)
		return FALSE;	
	
	*lpNumberOfBytesRead = 0;

	if (nNumberOfBytesToRead == 0)//need zero bytes
		return TRUE;

	pCurrMDL = NET_BUFFER_CURRENT_MDL(pNetBuffer);	
	CurrentNetBufferSize = NET_BUFFER_DATA_LENGTH(pNetBuffer);
	
	if(pCurrMDL == NULL || CurrentNetBufferSize == 0){ 

		//
		// skip empty NET_BUFFER
		//
		ASSERT(FALSE);
		return FALSE;
	}

	if (MmGetMdlByteCount(pCurrMDL) <= NET_BUFFER_CURRENT_MDL_OFFSET(pNetBuffer)){

		//
		// offset is more or equal to data size
		// NET_BUFFER seems to be invalid
		//
		ASSERT(FALSE);
		return FALSE;
	}

	ulBufferLength = min(CurrentNetBufferSize,
		MmGetMdlByteCount(pCurrMDL) - NET_BUFFER_CURRENT_MDL_OFFSET(pNetBuffer));

	pCurrData = (PUCHAR)MmGetSystemAddressForMdlSafe(pCurrMDL, NormalPagePriority);
	if (pCurrData == NULL)
		return FALSE;

	pCurrData = pCurrData + NET_BUFFER_CURRENT_MDL_OFFSET(pNetBuffer);

	while(ulBufferLength && pCurrData){

		if(CurrentNetBufferSize <= nOffset){

			//
			// requested offset is more than packet size
			//
			return FALSE;
		}

		if (0 == nOffset){
			//
			// We have reached requested offset
			//
			break;
		}

		if (nOffset >= ulBufferLength)
		{
			nOffset -= ulBufferLength;
			CurrentNetBufferSize -= ulBufferLength;
			ulBufferLength = 0;
		}
		else // nOffset < ulBufferLength
		{
			ulBufferLength -= nOffset;
			CurrentNetBufferSize -= nOffset;
			pCurrData += nOffset;
			nOffset = 0;
			break;
		}

		if(0 == CurrentNetBufferSize){
			ASSERT(FALSE);
			return FALSE;
		}
		
		pCurrMDL = pCurrMDL->Next;
		if (pCurrMDL == NULL){

			ASSERT(FALSE);
			return FALSE;
		}

		ulBufferLength = min(CurrentNetBufferSize,MmGetMdlByteCount(pCurrMDL));
		pCurrData = (PUCHAR)MmGetSystemAddressForMdlSafe(pCurrMDL, NormalPagePriority);
	}

	ASSERT(!(0 == ulBufferLength || NULL == pCurrData));

	//
	// do copy
	//
	while (ulBufferLength && pCurrData){

		CopyDataLength = min(nNumberOfBytesToRead, ulBufferLength);
			
		RtlCopyMemory(lpBuffer + CurrBufferOffset, pCurrData, CopyDataLength);
		CurrBufferOffset += CopyDataLength;
		nNumberOfBytesToRead -= CopyDataLength;

		if (nNumberOfBytesToRead == 0){
			//
			// finish, don't need more data
			//
			*lpNumberOfBytesRead = CurrBufferOffset;
			return TRUE;
		}
		ulBufferLength -= CopyDataLength;
		CurrentNetBufferSize -= CopyDataLength;

		if (0 == CurrentNetBufferSize){
			//
			// We have been asked to copy more data than we have
			//
			*lpNumberOfBytesRead = CurrBufferOffset;
			return TRUE;
		}

		pCurrMDL = pCurrMDL->Next;
		if (pCurrMDL == NULL)
		{// end of MDL chain, but NET_BUFFER has more data
			return FALSE;
		}

		ulBufferLength = min(CurrentNetBufferSize,MmGetMdlByteCount(pCurrMDL));
		pCurrData = (PUCHAR)MmGetSystemAddressForMdlSafe(pCurrMDL, NormalPagePriority);
	}

	return FALSE;
}
//***********************************************************************************
// Name: NdisGetFirstBufferFromNetBuffer
//
// Description: 
//		returns header for the incoming net buffer list
//
// Return value: 
//		pointer to the buffer header
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	NDIS_GetFirstBufferFromNetBuffer(
		IN PNET_BUFFER pNetBuffer,
		OUT PVOID *FirstBufferVA,
		OUT PULONG FirstBufferLength
		)
{
	ULONG			ulTotalLength = 0;
	ULONG			ulBufferLength = 0;
	ULONG			ulCurrentNetBufferSize = 0;
	PUCHAR			pAddress = NULL;
	ULONG			hlen = 0;
	PUCHAR			pEthHeader;
	UINT			nBytesRequired = 0;
	ULONG			Flags;
	PMDL			pCurrMDL = NULL;

	if ( FirstBufferVA ){
		*FirstBufferVA = NULL;
	}

	if ( FirstBufferLength ){
		*FirstBufferLength = 0;
	}

	ulTotalLength = NET_BUFFER_DATA_LENGTH(pNetBuffer);

	if (ulTotalLength <  SIZEOF_ETH_HDR){
		return;
	}

	ulCurrentNetBufferSize = NET_BUFFER_DATA_LENGTH(pNetBuffer);
	pCurrMDL = NET_BUFFER_CURRENT_MDL(pNetBuffer);

	if (MmGetMdlByteCount(pCurrMDL) <= NET_BUFFER_CURRENT_MDL_OFFSET(pNetBuffer)){
		return;
	}		

	ulBufferLength = min(MmGetMdlByteCount(pCurrMDL) - NET_BUFFER_CURRENT_MDL_OFFSET(pNetBuffer), ulCurrentNetBufferSize);
	pEthHeader = ((PUCHAR)MmGetSystemAddressForMdlSafe(pCurrMDL, NormalPagePriority) +
		NET_BUFFER_CURRENT_MDL_OFFSET(pNetBuffer));

	if ( FirstBufferVA ){
		*FirstBufferVA = pEthHeader;
	}

	if ( FirstBufferLength ){
		*FirstBufferLength = ulBufferLength;
	}
}
//***********************************************************************************
// Name: NDIS_GetPoolFromPacket
//
// Description: 
//		wk2 doesn't export NdisGetPoolFromPacket. we remplement it
//
// Return value: TRUE if success
//
// NOTE: None
// **********************************************************************************
NDIS_HANDLE
	NDIS_GetPoolFromPacket(
		IN PNDIS_PACKET            Packet
		)
{
	return (NDIS_HANDLE)Packet->Private.Pool;
}
//***********************************************************************************
// Name: NDIS_GetMiniportName
//
// Description: 
//		return miniport name from version specific structure
//
// Return value: TRUE if success
//
// NOTE: None
// **********************************************************************************
UNICODE_STRING
	NDIS_GetMiniportName(
		IN NDIS_HANDLE Miniport
		)
{
	UNICODE_STRING MiniportName = {0,0,NULL};
	if ( OsGetMajorVersion() == 5 )
	{
		if ( OsGetMinorVersion() == 0 ){ // wk2
			PNDIS_MINIPORT_BLOCK_W2K MiniportBlock = (PNDIS_MINIPORT_BLOCK_W2K)Miniport;
			MiniportName = MiniportBlock->MiniportName;
		} else if ( OsGetMinorVersion() == 1 ){ //xp
			PNDIS_MINIPORT_BLOCK_WXP MiniportBlock = (PNDIS_MINIPORT_BLOCK_WXP)Miniport;
			MiniportName = MiniportBlock->MiniportName;
		}else if ( OsGetMinorVersion() > 1 ){ //w2k3
			PNDIS_MINIPORT_BLOCK_WNET MiniportBlock = (PNDIS_MINIPORT_BLOCK_WNET)Miniport;
			MiniportName = MiniportBlock->MiniportName;
		}

	} 
	else if ( OsGetMajorVersion() == 6 )
	{
		if ( OsGetMinorVersion() == 0 ){ // vista
			if ( OsGetServicePack() == 0 ){ 
				PNDIS_MINIPORT_BLOCK_WIN6 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6)Miniport;
				MiniportName = MiniportBlock->MiniportName;
			}else if ( OsGetServicePack() == 1 ){ 
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				MiniportName = MiniportBlock->MiniportName;
			}else if ( OsGetServicePack() == 2 ){
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				MiniportName = MiniportBlock->MiniportName;
			}
		}else if ( OsGetMinorVersion() == 1 ){ // win7
			PNDIS_MINIPORT_BLOCK_WIN7 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN7)Miniport;
			MiniportName = MiniportBlock->MiniportName;
		}else if ( OsGetMinorVersion() == 2 ){ // win8
			PNDIS_MINIPORT_BLOCK_WIN8 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN8)Miniport;
			MiniportName = MiniportBlock->MiniportName;
		}
	}
	return MiniportName;
}

//***********************************************************************************
// Name: NDIS_GetMiniportDeviceObject
//
// Description: 
//		return miniport device from version specific structure
//
// Return value: TRUE if success
//
// NOTE: None
// **********************************************************************************
PDEVICE_OBJECT
	NDIS_GetMiniportDeviceObject(
		IN NDIS_HANDLE Miniport
		)
{
	PDEVICE_OBJECT DeviceObject = NULL;
	if ( OsGetMajorVersion() == 5 )
	{
		if ( OsGetMinorVersion() == 0 ){ //w2k
			PNDIS_MINIPORT_BLOCK_W2K MiniportBlock = (PNDIS_MINIPORT_BLOCK_W2K)Miniport;
			DeviceObject = MiniportBlock->DeviceObject;
		} else if ( OsGetMinorVersion() == 1 ){ //xp
			PNDIS_MINIPORT_BLOCK_WXP MiniportBlock = (PNDIS_MINIPORT_BLOCK_WXP)Miniport;
			DeviceObject = MiniportBlock->DeviceObject;
		}else if ( OsGetMinorVersion() > 1 ){ //w2k3
			PNDIS_MINIPORT_BLOCK_WNET MiniportBlock = (PNDIS_MINIPORT_BLOCK_WNET)Miniport;
			DeviceObject = MiniportBlock->DeviceObject;
		}

	} 
	else if ( OsGetMajorVersion() == 6 )
	{
		if ( OsGetMinorVersion() == 0 ){ //vista

			if ( OsGetServicePack() == 0 ){
				PNDIS_MINIPORT_BLOCK_WIN6 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6)Miniport;
				DeviceObject = MiniportBlock->DeviceObject;
			} else if ( OsGetServicePack() == 1 ){
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				DeviceObject = MiniportBlock->DeviceObject;
			} else if ( OsGetServicePack() == 2 ){
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				DeviceObject = MiniportBlock->DeviceObject;
			}
		} else if ( OsGetMinorVersion() == 1 ){ // win7
			PNDIS_MINIPORT_BLOCK_WIN7 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN7)Miniport;
			DeviceObject = MiniportBlock->DeviceObject;
		}else if ( OsGetMinorVersion() == 2 ){ // win8
			PNDIS_MINIPORT_BLOCK_WIN8 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN8)Miniport;
			DeviceObject = MiniportBlock->DeviceObject;
		}
	}
	return DeviceObject;
}

//***********************************************************************************
// Name: NDIS_GetSGListLookasideList
//
// Description: 
//		return miniport SGListLookasideList from version specific structure
//
// Return value: TRUE if success
//
// NOTE: None
// **********************************************************************************
PNPAGED_LOOKASIDE_LIST
	NDIS_GetSGListLookasideList(
		IN NDIS_HANDLE Miniport
		)
{
	PNPAGED_LOOKASIDE_LIST SGListLookasideList = NULL;
	if ( OsGetMajorVersion() == 5 )
	{
		if ( OsGetMinorVersion() == 1 ){ // w2k
			PNDIS_MINIPORT_BLOCK_WXP MiniportBlock = (PNDIS_MINIPORT_BLOCK_WXP)Miniport;
			SGListLookasideList = MiniportBlock->SGListLookasideList;
		}else if ( OsGetMinorVersion() > 1 ){ // xp
			PNDIS_MINIPORT_BLOCK_WNET MiniportBlock = (PNDIS_MINIPORT_BLOCK_WNET)Miniport;
			if ( MiniportBlock->MiniportSGDmaBlock ){ //w2k3
				SGListLookasideList = MiniportBlock->MiniportSGDmaBlock->SGListLookasideList;
			}else {
				SGListLookasideList = MiniportBlock->SGListLookasideList;
			}
		}
	} 
	else if ( OsGetMajorVersion() == 6 )
	{
		if ( OsGetMinorVersion() == 0 ){ // vista
			if ( OsGetServicePack() == 0 ){
				PNDIS_MINIPORT_BLOCK_WIN6 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6)Miniport;
				if ( MiniportBlock->MiniportSGDmaBlock ){
					SGListLookasideList = MiniportBlock->MiniportSGDmaBlock->SGListLookasideList;
				}
			}else if ( OsGetServicePack() == 1 ){
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				if ( MiniportBlock->MiniportSGDmaBlock ){
					SGListLookasideList = MiniportBlock->MiniportSGDmaBlock->SGListLookasideList;
				}
			}else if ( OsGetServicePack() == 2 ){
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				if ( MiniportBlock->MiniportSGDmaBlock ){
					SGListLookasideList = MiniportBlock->MiniportSGDmaBlock->SGListLookasideList;
				}
			}
		} else if ( OsGetMinorVersion() == 1 ){ // win7
			PNDIS_MINIPORT_BLOCK_WIN7 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN7)Miniport;
			if ( MiniportBlock->MiniportSGDmaBlock ){
				SGListLookasideList = MiniportBlock->MiniportSGDmaBlock->SGListLookasideList;
			}
		}else if ( OsGetMinorVersion() == 2 ){ // win8
			PNDIS_MINIPORT_BLOCK_WIN8 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN8)Miniport;
			if ( MiniportBlock->MiniportSGDmaBlock ){
				SGListLookasideList = MiniportBlock->MiniportSGDmaBlock->SGListLookasideList;
			}
		}
	}
	return SGListLookasideList;
}

//***********************************************************************************
// Name: NDIS_GetScatterGatherListSize
//
// Description: 
//		return miniport ScatterGatherListSize from version specific structure
//
// Return value: TRUE if success
//
// NOTE: None
// **********************************************************************************
ULONG
	NDIS_GetScatterGatherListSize(
		IN NDIS_HANDLE Miniport
		)
{
	ULONG ScatterGatherListSize = 0;
	if ( OsGetMajorVersion() == 5 )
	{
		if ( OsGetMinorVersion() == 1 ){ // xp
			PNDIS_MINIPORT_BLOCK_WXP MiniportBlock = (PNDIS_MINIPORT_BLOCK_WXP)Miniport;
			ScatterGatherListSize = MiniportBlock->ScatterGatherListSize;
		}else if ( OsGetMinorVersion() == 2 ){ //w2k3
			PNDIS_MINIPORT_BLOCK_WNET MiniportBlock = (PNDIS_MINIPORT_BLOCK_WNET)Miniport;
			if ( MiniportBlock->MiniportSGDmaBlock ){
				ScatterGatherListSize = MiniportBlock->MiniportSGDmaBlock->ScatterGatherListSize;
			}else {
				ScatterGatherListSize = MiniportBlock->ScatterGatherListSize;
			}
		}

	} 
	else if ( OsGetMajorVersion() == 6 )
	{
		if ( OsGetMinorVersion() == 0 ){ // vista
			if ( OsGetServicePack() == 0 ){
				PNDIS_MINIPORT_BLOCK_WIN6 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6)Miniport;
				if ( MiniportBlock->MiniportSGDmaBlock ){
					ScatterGatherListSize = MiniportBlock->MiniportSGDmaBlock->ScatterGatherListSize;
				}
			} 
			else if ( OsGetServicePack() == 1 )
			{
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				if ( MiniportBlock->MiniportSGDmaBlock ){
					ScatterGatherListSize = MiniportBlock->MiniportSGDmaBlock->ScatterGatherListSize;
				}
			}
			else if ( OsGetServicePack() == 2 )
			{
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				if ( MiniportBlock->MiniportSGDmaBlock ){
					ScatterGatherListSize = MiniportBlock->MiniportSGDmaBlock->ScatterGatherListSize;
				}
			}
		} else if ( OsGetMinorVersion() == 1 ){ //win7
			PNDIS_MINIPORT_BLOCK_WIN7 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN7)Miniport;
			if ( MiniportBlock->MiniportSGDmaBlock ){
				ScatterGatherListSize = MiniportBlock->MiniportSGDmaBlock->ScatterGatherListSize;
			}
		}else if ( OsGetMinorVersion() == 2 ){ //win8
			PNDIS_MINIPORT_BLOCK_WIN8 MiniportBlock = (PNDIS_MINIPORT_BLOCK_WIN8)Miniport;
			if ( MiniportBlock->MiniportSGDmaBlock ){
				ScatterGatherListSize = MiniportBlock->MiniportSGDmaBlock->ScatterGatherListSize;
			}
		}
	}
	return ScatterGatherListSize;
}

//***********************************************************************************
// Name: NDIS_GetSystemAdapterObject
//
// Description: 
//		return miniport SystemAdapterObject from version specific structure
//
// Return value: TRUE if success
//
// NOTE: None
// **********************************************************************************
PDMA_ADAPTER
	NDIS_GetSystemAdapterObject(
		IN NDIS_HANDLE Miniport
		)
{
	PDMA_ADAPTER SystemAdapterObject = NULL;
	if ( OsGetMajorVersion() == 5 )
	{
		if ( OsGetMinorVersion() == 0 ) // win2k
		{
			PNDIS_MINIPORT_BLOCK_W2K pNdisMiniportW2K = (PNDIS_MINIPORT_BLOCK_W2K)Miniport;
			SystemAdapterObject = (PDMA_ADAPTER)pNdisMiniportW2K->SystemAdapterObject;
		} 
		else if ( OsGetMinorVersion() == 1 ) // wxp
		{
			PNDIS_MINIPORT_BLOCK_WXP pNdisMiniportWXP = (PNDIS_MINIPORT_BLOCK_WXP)Miniport;
			if ( pNdisMiniportWXP->SystemAdapterObject ) {
				SystemAdapterObject = (PDMA_ADAPTER)pNdisMiniportWXP->SystemAdapterObject;				
			}else if ( pNdisMiniportWXP->SavedSystemAdapterObject ){
				SystemAdapterObject = (PDMA_ADAPTER)pNdisMiniportWXP->SavedSystemAdapterObject;				
			}
		}
		else if ( OsGetMinorVersion() == 2 ) // w2k3
		{
			PNDIS_MINIPORT_BLOCK_WNET pNdisMiniportWNET = (PNDIS_MINIPORT_BLOCK_WNET)Miniport;
			if ( pNdisMiniportWNET->MiniportSGDmaBlock ){
				if ( pNdisMiniportWNET->MiniportSGDmaBlock->DmaAdapterObject ){
					SystemAdapterObject = pNdisMiniportWNET->MiniportSGDmaBlock->DmaAdapterObject;
				}else if ( pNdisMiniportWNET->MiniportSGDmaBlock->SavedDmaAdapterObject ){
					SystemAdapterObject = pNdisMiniportWNET->MiniportSGDmaBlock->SavedDmaAdapterObject;
				}
			}else if ( pNdisMiniportWNET->SystemAdapterObject ) {
				SystemAdapterObject = (PDMA_ADAPTER)pNdisMiniportWNET->SystemAdapterObject;				
			}else if ( pNdisMiniportWNET->SavedSystemAdapterObject ){
				SystemAdapterObject = (PDMA_ADAPTER)pNdisMiniportWNET->SavedSystemAdapterObject;				
			}
		}
	} else {
		
		if ( OsGetMinorVersion() == 0 ){ //vista
			if ( OsGetServicePack() == 0 ){
				PNDIS_MINIPORT_BLOCK_WIN6 pNdisMiniportWIN7 = (PNDIS_MINIPORT_BLOCK_WIN6)Miniport;
				if ( pNdisMiniportWIN7->MiniportSGDmaBlock ){
					if ( pNdisMiniportWIN7->MiniportSGDmaBlock->DmaAdapterObject ){
						SystemAdapterObject = pNdisMiniportWIN7->MiniportSGDmaBlock->DmaAdapterObject;
					}else if ( pNdisMiniportWIN7->MiniportSGDmaBlock->SavedDmaAdapterObject ){
						SystemAdapterObject = pNdisMiniportWIN7->MiniportSGDmaBlock->SavedDmaAdapterObject;
					}
				}
			} else if ( OsGetServicePack() == 1 ){
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 pNdisMiniportWIN7 = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				if ( pNdisMiniportWIN7->MiniportSGDmaBlock ){
					if ( pNdisMiniportWIN7->MiniportSGDmaBlock->DmaAdapterObject ){
						SystemAdapterObject = pNdisMiniportWIN7->MiniportSGDmaBlock->DmaAdapterObject;
					}else if ( pNdisMiniportWIN7->MiniportSGDmaBlock->SavedDmaAdapterObject ){
						SystemAdapterObject = pNdisMiniportWIN7->MiniportSGDmaBlock->SavedDmaAdapterObject;
					}
				}
			} else if ( OsGetServicePack() == 2 ){
				PNDIS_MINIPORT_BLOCK_WIN6_SP1 pNdisMiniportWIN7 = (PNDIS_MINIPORT_BLOCK_WIN6_SP1)Miniport;
				if ( pNdisMiniportWIN7->MiniportSGDmaBlock ){
					if ( pNdisMiniportWIN7->MiniportSGDmaBlock->DmaAdapterObject ){
						SystemAdapterObject = pNdisMiniportWIN7->MiniportSGDmaBlock->DmaAdapterObject;
					}else if ( pNdisMiniportWIN7->MiniportSGDmaBlock->SavedDmaAdapterObject ){
						SystemAdapterObject = pNdisMiniportWIN7->MiniportSGDmaBlock->SavedDmaAdapterObject;
					}
				}
			}
			
		} else if ( OsGetMinorVersion() == 1 ){ // win7
			PNDIS_MINIPORT_BLOCK_WIN7 pNdisMiniportWIN7 = (PNDIS_MINIPORT_BLOCK_WIN7)Miniport;
			if ( pNdisMiniportWIN7->MiniportSGDmaBlock ){
				if ( pNdisMiniportWIN7->MiniportSGDmaBlock->DmaAdapterObject ){
					SystemAdapterObject = pNdisMiniportWIN7->MiniportSGDmaBlock->DmaAdapterObject;
				}else if ( pNdisMiniportWIN7->MiniportSGDmaBlock->SavedDmaAdapterObject ){
					SystemAdapterObject = pNdisMiniportWIN7->MiniportSGDmaBlock->SavedDmaAdapterObject;
				}
			}
		}else if ( OsGetMinorVersion() == 2 ){ //win8
			PNDIS_MINIPORT_BLOCK_WIN8 pNdisMiniportWIN8 = (PNDIS_MINIPORT_BLOCK_WIN8)Miniport;
			if ( pNdisMiniportWIN8->MiniportSGDmaBlock ){
				if ( pNdisMiniportWIN8->MiniportSGDmaBlock->DmaAdapterObject ){
					SystemAdapterObject = pNdisMiniportWIN8->MiniportSGDmaBlock->DmaAdapterObject;
				}else if ( pNdisMiniportWIN8->MiniportSGDmaBlock->SavedDmaAdapterObject ){
					SystemAdapterObject = pNdisMiniportWIN8->MiniportSGDmaBlock->SavedDmaAdapterObject;
				}
			}
		}
	}
	return SystemAdapterObject;
}

