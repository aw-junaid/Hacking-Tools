//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: ndishook.c
// $Revision: 82 $
// $Date: 2012-01-18 22:08:12 +0400 (Ð¡Ñ€, 18 ÑÐ½Ð² 2012) $
// description: 
//  KIP library NDIS function hooks.

#include "project.h"

#define		szNdis		"NDIS"
#define MODULE_TAG 'IDNK'

// ---- Types and constants ---------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// ndis 6

typedef
NDIS_STATUS
	(NTAPI *FUNC_NdisMRegisterMiniportDriver)(
		IN PDRIVER_OBJECT  DriverObject,
		IN PUNICODE_STRING  RegistryPath,
		IN NDIS_HANDLE  MiniportDriverContext,
		IN PNDIS_MINIPORT_DRIVER_CHARACTERISTICS  MiniportDriverCharacteristics,
		OUT PNDIS_HANDLE  NdisMiniportDriverHandle
		);

typedef
VOID 
	(NTAPI *FUNC_NdisMDeregisterMiniportDriver)(
		IN NDIS_HANDLE  NdisMiniportDriverHandle
		); 

typedef
NDIS_STATUS
	(NTAPI *FUNC_NdisMSetMiniportAttributes) (
		IN NDIS_HANDLE  NdisMiniportAdapterHandle,
		IN PNDIS_MINIPORT_ADAPTER_ATTRIBUTES  MiniportAttributes
		); 

typedef
VOID 
(NTAPI *FUNC_NdisMIndicateReceiveNetBufferLists)(
	IN NDIS_HANDLE  MiniportAdapterHandle,
	IN PNET_BUFFER_LIST  NetBufferLists,
	IN NDIS_PORT_NUMBER  PortNumber,
	IN ULONG  NumberOfNetBufferLists,
	IN ULONG  ReceiveFlags
	); 

typedef
VOID 
(NTAPI *FUNC_NdisMSendNetBufferListsComplete)(
	IN NDIS_HANDLE  MiniportAdapterHandle,
	IN PNET_BUFFER_LIST  NetBufferLists,
	IN ULONG  SendCompleteFlags
	);

typedef
VOID 
(NTAPI *FUNC_NdisMRestartComplete)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		);

typedef
VOID 
(NTAPI *FUNC_NdisMResetComplete)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status,
		IN BOOLEAN  AddressingReset
		);

typedef
VOID 
(NTAPI *FUNC_NdisMIndicateStatusEx)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_STATUS_INDICATION  StatusIndication
		);

typedef
VOID 
(NTAPI *FUNC_NdisMOidRequestComplete)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_OID_REQUEST  OidRequest,
		IN NDIS_STATUS  Status
		);

//////////////////////////////////////////////////////////////////////////
// ndis 5

typedef
NDIS_STATUS
	(NTAPI *FUNC_NdisMRegisterMiniport)(
		IN NDIS_HANDLE  NdisWrapperHandle,
		IN PNDIS_MINIPORT_CHARACTERISTICS  MiniportCharacteristics,
		IN UINT  CharacteristicsLength
		);

typedef
VOID 
	(NTAPI *FUNC_NdisTerminateWrapper)(
		IN NDIS_HANDLE  NdisWrapperHandle,
		IN PVOID  SystemSpecific
		);

typedef
VOID 
	(NTAPI* FUNC_NdisMSetAttributes)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_HANDLE  MiniportAdapterContext,
		IN BOOLEAN  BusMaster,
		IN NDIS_INTERFACE_TYPE  AdapterType
		);

typedef
VOID 
	(NTAPI* FUNC_NdisMSetAttributesEx)(
		IN NDIS_HANDLE MiniportAdapterHandle,
		IN NDIS_HANDLE MiniportAdapterContext,
		IN UINT  CheckForHangTimeInSeconds  OPTIONAL,
		IN ULONG  AttributeFlags,
		IN NDIS_INTERFACE_TYPE AdapterType
		);

typedef
VOID 
	(NTAPI *FUNC_NdisMSendComplete) (
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_PACKET  Packet,
		IN NDIS_STATUS  Status
		);

typedef
VOID 
	(NTAPI *FUNC_NdisMTransferDataComplete)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_PACKET  Packet,
		IN NDIS_STATUS  Status,
		IN UINT  BytesTransferred
		);

typedef
VOID 
	(NTAPI *FUNC_NdisMQueryInformationComplete)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		);

typedef
VOID 
	(NTAPI *FUNC_NdisMSetInformationComplete)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		);

typedef
VOID 
(NTAPI *FUNC_NdisMIndicateStatus)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  GeneralStatus,
		IN PVOID  StatusBuffer,
		IN UINT  StatusBufferSize
		);

typedef
NDIS_STATUS
(NTAPI *FUNC_NdisMInitializeScatterGatherDma)(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN BOOLEAN  Dma64BitAddresses,
		IN ULONG  MaximumPhysicalMapping
		);

// ---- Predefinitions ----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// ndis 6

NDIS_STATUS
	my_NdisMRegisterMiniportDriver(
		IN PDRIVER_OBJECT  DriverObject,
		IN PUNICODE_STRING  RegistryPath,
		IN NDIS_HANDLE  MiniportDriverContext,
		IN PNDIS_MINIPORT_DRIVER_CHARACTERISTICS  MiniportDriverCharacteristics,
		OUT PNDIS_HANDLE  NdisMiniportDriverHandle
		);

VOID 
	my_NdisMDeregisterMiniportDriver(
		IN NDIS_HANDLE  NdisMiniportDriverHandle
		); 

NDIS_STATUS
	my_NdisMSetMiniportAttributes(
		IN NDIS_HANDLE  NdisMiniportAdapterHandle,
		IN PNDIS_MINIPORT_ADAPTER_ATTRIBUTES  MiniportAttributes
		); 

VOID 
	my_NdisMIndicateReceiveNetBufferLists(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNET_BUFFER_LIST  NetBufferLists,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN ULONG  NumberOfNetBufferLists,
		IN ULONG  ReceiveFlags
		); 

VOID
	my_NdisMSendNetBufferListsComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNET_BUFFER_LIST  NetBufferLists,
		IN ULONG  SendCompleteFlags
		);

VOID
	my_NdisMRestartComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		);

VOID
	my_NdisMResetComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status,
		IN BOOLEAN  AddressingReset
		);

VOID
	my_NdisMIndicateStatusEx(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_STATUS_INDICATION  StatusIndication
		) ;

VOID
	my_NdisMOidRequestComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_OID_REQUEST  OidRequest,
		IN NDIS_STATUS  Status
		);

//////////////////////////////////////////////////////////////////////////
// ndis 5

NDIS_STATUS
	my_NdisMRegisterMiniport(
		IN NDIS_HANDLE  NdisWrapperHandle,
		IN PNDIS_MINIPORT_CHARACTERISTICS  MiniportCharacteristics,
		IN UINT  CharacteristicsLength
		);

VOID 
	my_NdisTerminateWrapper(
		IN NDIS_HANDLE  NdisWrapperHandle,
		IN PVOID  SystemSpecific
		);

VOID 
	my_NdisMSetAttributes(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_HANDLE  MiniportAdapterContext,
		IN BOOLEAN  BusMaster,
		IN NDIS_INTERFACE_TYPE  AdapterType
		);

VOID 
	my_NdisMSetAttributesEx(
		IN NDIS_HANDLE MiniportAdapterHandle,
		IN NDIS_HANDLE MiniportAdapterContext,
		IN UINT  CheckForHangTimeInSeconds  OPTIONAL,
		IN ULONG  AttributeFlags,
		IN NDIS_INTERFACE_TYPE AdapterType
		);

VOID
	my_NdisMSendComplete (
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_PACKET  Packet,
		IN NDIS_STATUS  Status
		);
VOID
	my_NdisMTransferDataComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_PACKET  Packet,
		IN NDIS_STATUS  Status,
		IN UINT  BytesTransferred
		);

VOID
	my_NdisMQueryInformationComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		);

VOID
	my_NdisMSetInformationComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		);

VOID
	my_NdisMIndicateStatus(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  GeneralStatus,
		IN PVOID  StatusBuffer,
		IN UINT  StatusBufferSize
		);

NDIS_STATUS
	my_NdisMInitializeScatterGatherDma(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN BOOLEAN  Dma64BitAddresses,
		IN ULONG  MaximumPhysicalMapping
		);

// ---- NDIS hooks -------------------------------------------------------------------------------------------------

#define DECLARE_NDIS_HOOK(FuncName) \
	HOOK_FUNCTION hook_ndis_##FuncName = {szNdis, #FuncName, &my_##FuncName, NULL, NULL}

#define DEFINE_NDIS_HOOK(FuncName,Flag) \
	DEFINE_HOOK(&hook_ndis_##FuncName, HOOK_TYPE_EXPORT | Flag)

#define NDIS_ORIGINAL(FuncName) \
	((FUNC_##FuncName)hook_ndis_##FuncName.Original)

#define NDIS_MAKE_CALL_STUB(Adapter,Miniport,FuncName,nArgs) \
	Miniport->##FuncName = MakeCallStub(&Adapter->STUB_##FuncName,my_##FuncName,Adapter,nArgs)

// ndis6
DECLARE_NDIS_HOOK(NdisMRegisterMiniportDriver);
DECLARE_NDIS_HOOK(NdisMDeregisterMiniportDriver);
DECLARE_NDIS_HOOK(NdisMSetMiniportAttributes);
DECLARE_NDIS_HOOK(NdisMIndicateReceiveNetBufferLists);
DECLARE_NDIS_HOOK(NdisMSendNetBufferListsComplete);
DECLARE_NDIS_HOOK(NdisMRestartComplete);
DECLARE_NDIS_HOOK(NdisMResetComplete);
DECLARE_NDIS_HOOK(NdisMIndicateStatusEx);
DECLARE_NDIS_HOOK(NdisMOidRequestComplete);

//ndis5
DECLARE_NDIS_HOOK(NdisMRegisterMiniport);
DECLARE_NDIS_HOOK(NdisTerminateWrapper);
DECLARE_NDIS_HOOK(NdisMSetAttributes);
DECLARE_NDIS_HOOK(NdisMSetAttributesEx);
DECLARE_NDIS_HOOK(NdisMSendComplete);
DECLARE_NDIS_HOOK(NdisMTransferDataComplete);
DECLARE_NDIS_HOOK(NdisMQueryInformationComplete);
DECLARE_NDIS_HOOK(NdisMSetInformationComplete);
DECLARE_NDIS_HOOK(NdisMIndicateStatus);
DECLARE_NDIS_HOOK(NdisMInitializeScatterGatherDma);

static HOOK NdisExportHooks[] = {
	// ndis6
	DEFINE_NDIS_HOOK(NdisMRegisterMiniportDriver, HOOK_CAN_FAIL),
	DEFINE_NDIS_HOOK(NdisMDeregisterMiniportDriver, HOOK_CAN_FAIL),
	DEFINE_NDIS_HOOK(NdisMSetMiniportAttributes, HOOK_CAN_FAIL),
	DEFINE_NDIS_HOOK(NdisMIndicateReceiveNetBufferLists, HOOK_CAN_FAIL),
	DEFINE_NDIS_HOOK(NdisMSendNetBufferListsComplete, HOOK_CAN_FAIL),
	DEFINE_NDIS_HOOK(NdisMRestartComplete, HOOK_CAN_FAIL),
	DEFINE_NDIS_HOOK(NdisMResetComplete, HOOK_CAN_FAIL),
	DEFINE_NDIS_HOOK(NdisMIndicateStatusEx, HOOK_CAN_FAIL),
	DEFINE_NDIS_HOOK(NdisMOidRequestComplete, HOOK_CAN_FAIL),

	//ndis5
	DEFINE_NDIS_HOOK(NdisMRegisterMiniport,0),
	DEFINE_NDIS_HOOK(NdisTerminateWrapper,0),
	DEFINE_NDIS_HOOK(NdisMSetAttributes,0),
	DEFINE_NDIS_HOOK(NdisMSetAttributesEx,0),
	DEFINE_NDIS_HOOK(NdisMSendComplete,0),
	DEFINE_NDIS_HOOK(NdisMTransferDataComplete,0),
	DEFINE_NDIS_HOOK(NdisMQueryInformationComplete,0),
	DEFINE_NDIS_HOOK(NdisMSetInformationComplete,0),
	DEFINE_NDIS_HOOK(NdisMIndicateStatus,0),
	DEFINE_NDIS_HOOK(NdisMInitializeScatterGatherDma,0),
};


// ---- Internal functions -------------------------------------------------------------------------------------------------


// ---- Public functions ---------------------------------------------------------------------------------------------------

NTSTATUS	NdisSetHooks(VOID)
{
	NTSTATUS ntStatus;
	PVOID	NdisBase;

	if (NdisBase = GetModuleBase(szNdis))
	{
		// Check if BK present
		if (BkNtKernelBase)
		{		
			// Setting IMAGE_FILE_RELOCS_STRIPPED flag to make sure NDIS will not be relocated by MiReloadBootLoadedDrivers.
			((PIMAGE_NT_HEADERS)((PCHAR)NdisBase + ((PIMAGE_DOS_HEADER)NdisBase)->e_lfanew))->FileHeader.Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;
		}

		NdisAllocateNetBufferListPtr     = (PNDIS_ALLOCATE_NET_BUFFER_LIST)BkGetFunctionAddress(NdisBase, "NdisAllocateNetBufferList");
		NdisAllocateNetBufferListPoolPtr = (PNDIS_ALLOCATE_NET_BUFFER_LIST_POOL)BkGetFunctionAddress(NdisBase, "NdisAllocateNetBufferListPool");
		NdisFreeNetBufferListPtr         = (PNDIS_FREE_NET_BUFFER_LIST)BkGetFunctionAddress(NdisBase, "NdisFreeNetBufferList");
		NdisFreeNetBufferListPoolPtr     = (PNDIS_FREE_NET_BUFFER_LIST_POOL)BkGetFunctionAddress(NdisBase, "NdisFreeNetBufferListPool");
		NdisMIndicateReceiveNetBufferListsPtr = (PNDIS_M_INDICATE_RECEIVE_NET_BUFFER_LISTS)BkGetFunctionAddress(NdisBase, "NdisMIndicateReceiveNetBufferLists");
	}

	ntStatus = SetMultipleHooks((PHOOK)&NdisExportHooks, (sizeof(NdisExportHooks) / sizeof(HOOK)), NULL);
	if ( ntStatus != STATUS_SUCCESS ){
		DBGLOG(( LCriticalError, "%08X=SetMultipleHooks failed\n", ntStatus ));
	}

	return(ntStatus);
}
// ---- Hook functions -----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// 
// NDIS 6.0 interface
//

//***********************************************************************************
// Name: my_MiniportInitializeEx
//
// Description: 
//		MiniportInitializeEx hook.
//		NDIS calls a miniport driver's MiniportInitializeEx function to initialize 
//		a miniport adapter for network I/O operations.
//		we need to intercept it to understand that miniport is ready.
//
// Return value: 
//		operation status
//
// Parameters: 
//	MiniportAdapterHandle - An NDIS-supplied handle that identifies the miniport 
//							adapter that the miniport driver should initialize. 
//
//	MiniportDriverContext - A handle to a driver-allocated context area where the driver 
//							maintains state and configuration information. The miniport 
//							driver passed this context area to the 
//							NdisMRegisterMiniportDriver function. 
//
//	MiniportInitParameters - A pointer to an NDIS_MINIPORT_INIT_PARAMETERS structure 
//							that defines the initialization parameters for 
//							the miniport adapter.
// NOTE: None
// **********************************************************************************

NDIS_STATUS
	my_MiniportInitializeEx(
		IN PKIP_NDIS_MINIPORT pMiniport,
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_HANDLE  MiniportDriverContext,
		IN PNDIS_MINIPORT_INIT_PARAMETERS  MiniportInitParameters
		)
{
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;
	//PKIP_NDIS_MINIPORT pMiniport;
	PKIP_NDIS_ADAPTER pAdapter;
		
	//pMiniport = NDIS_FindMiniportByMiniportDriverContext( MiniportDriverContext );
	//lookup adapter by handle
	//ASSERT(pMiniport);

	// allocate new miniport
	pAdapter = NDIS_AllocateAdapter( pMiniport );
	if ( pAdapter == NULL ){
		DBGLOG((LCriticalError,"NDIS_AllocateAdapter failed\n"));
		return NDIS_STATUS_FAILURE;
	}

	KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_IN_INITIALIZE);
	pAdapter->NdisMiniportHandle = MiniportAdapterHandle;
	nStatus = 
		pMiniport->Ndis6.MiniportCharacteristics.InitializeHandlerEx( 
			MiniportAdapterHandle,
			MiniportDriverContext,
			MiniportInitParameters
			);

	KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_IN_INITIALIZE);

	if ( nStatus == NDIS_STATUS_SUCCESS ){

		// initialize NDIS5 miniport interface
		nStatus = 
			MpInitialize( 
				pAdapter, 
				pAdapter->Interface.MediaType // media type was initialized at NdisMSetMiniportAttributes
				);

		// we do not support the underling medium
		if ( nStatus == NDIS_STATUS_NOT_SUPPORTED ){
			KIP_MINIPORT_SET_INACTIVE(pAdapter);
			nStatus = NDIS_STATUS_SUCCESS;
		}else if ( nStatus == NDIS_STATUS_SUCCESS ){

			//
			// bind our protocol to miniport
			//
			nStatus = 
				ProtocolBindAdapter( 
					(PKIP_NDIS_INTERFACE)pAdapter,
					0,0,0,FALSE
					);
			if ( nStatus != NDIS_STATUS_SUCCESS ){
				DBGLOG(( LError, "%08X = ProtocolBindAdapter failed\n", nStatus ));
			}
		}

		// adapter is initialized
		pAdapter->Flags |= KIP_ADAPTER_INITIALIZED;
	}else{
		DBGLOG(( LInfo, "%08X=InitializeHandlerEx failed\n", nStatus ));
		NDIS_FreeAdapter( pAdapter );
	}
	return nStatus;
}
//***********************************************************************************
// Name: my_MiniportHaltEx
//
// Description: 
//		MiniportHaltEx hook.
//		NDIS calls a miniport driver's MiniportHaltEx function to deallocate resources 
//		when a miniport adapter is removed, and to stop the hardware.
//
// Return value: 
//		None
//
// Parameters
//	MiniportAdapterContext 
//		A handle to a context area that the miniport driver allocated in its 
//		MiniportInitializeEx function. The miniport driver uses this context area to 
//		maintain state information for a miniport adapter. 
//	HaltAction 
//		The reason for halting the miniport adapter. It can be one of the following values:
//		NdisHaltDeviceDisabled 
//			NDIS is halting the miniport adapter in response to a Plug and Play (PnP) 
//			remove message. 
//		NdisHaltDeviceInstanceDeInitialized 
//			NDIS is halting the miniport adapter in response to an intermediate driver 
//			calling the NdisIMDeInitializeDeviceInstance function. 
//		NdisHaltDevicePoweredDown 
//			NDIS is halting the miniport adapter because the system is going to 
//			a sleeping state. 
//		NdisHaltDeviceSurpriseRemoved 
//			The miniport adapter has been surprise removed and the hardware is not present. 
//		NdisHaltDeviceFailed 
//			The miniport adapter is being removed because of a hardware failure. Either 
//			the miniport driver called the NdisMRemoveMiniport function or a bus driver 
//			did not power up the NIC on resume. 
//		NdisHaltDeviceInitializationFailed 
//			NDIS could not initialize the miniport adapter for an unknown reason after 
//			the MiniportInitializeEx function completed successfully. 
//		NdisHaltDeviceStopped 
//			NDIS is halting the miniport adapter in response to a PnP stop device message. 
//
// NOTE: None
// **********************************************************************************
VOID
	my_MiniportHaltEx(
		IN  NDIS_HANDLE             MiniportAdapterContext,
		IN  NDIS_HALT_ACTION        HaltAction
		)
{
	PKIP_NDIS_MINIPORT pMiniport;
	PKIP_NDIS_ADAPTER pAdapter;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );
	ASSERT( pAdapter );
	if ( pAdapter )
	{
		DBGLOG(( LDebug, "Halting miniport %p\n", pAdapter ));

		pMiniport = pAdapter->Interface.Miniport;
		KIP_MINIPORT_SET_FLAG(pAdapter,fADAPTER_HALTING);

		pMiniport->Ndis6.MiniportCharacteristics.HaltHandlerEx(
			MiniportAdapterContext,
			HaltAction
			);

		// halt adapter
		MpHalt( pAdapter );

		DBGLOG(( LDebug, "Miniport is halted %p\n", pAdapter ));
		pAdapter = NULL;// adapter is not valid any more
	}
}
//***********************************************************************************
// Name: my_MiniportPause
//
// Description: 
//		MiniportPause hook.
//		NDIS calls a miniport driver's MiniportPause function to stop the flow of 
//		network data through a specified miniport adapter.
//
// Return value: 
//		Operation status
//
// Parameters
//	MiniportAdapterContext 
//		A handle to a context area that the miniport driver allocated in its MiniportInitializeEx 
//		function. The miniport driver uses this context area to maintain state 
//		information for an miniport adapter. 
//	MiniportPauseParameters 
//		A pointer to an NDIS_MINIPORT_PAUSE_PARAMETERS structure that defines 
//		the pause parameters for the miniport adapter.
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	my_MiniportPause(
		IN NDIS_HANDLE  MiniportAdapterContext,
		IN PNDIS_MINIPORT_PAUSE_PARAMETERS  MiniportPauseParameters
		)
{
	PKIP_NDIS_MINIPORT pMiniport;
	PKIP_NDIS_ADAPTER pAdapter;
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );
	ASSERT( pAdapter );
	if ( pAdapter )
	{
		DBGLOG(( LDebug, "Pausing miniport %p\n", pAdapter ));

		pMiniport = pAdapter->Interface.Miniport;
		pMiniport->Flags |= KIP_ADAPTER_PAUSED;
		nStatus = 
			pMiniport->Ndis6.MiniportCharacteristics.PauseHandler(
				MiniportAdapterContext,
				MiniportPauseParameters
				);	

		// TODO: notify protocol
		DBGLOG(( LDebug, "Miniport is paused %p\n", pAdapter ));
	}
	return nStatus;
}

//***********************************************************************************
// Name: my_MiniportRestart
//
// Description: 
//		RestartHandler hook.
//		The MiniportRestart function initiates a restart request for a miniport 
//		adapter that is paused.
//
// Return value: 
//		Operation status
//
// Parameters
//	MiniportAdapterContext 
//		A handle to a context area that the miniport driver allocated in its MiniportInitializeEx 
//		function. The miniport driver uses this context area to maintain state 
//		information for an miniport adapter. 
//	MiniportPauseParameters 
//		A pointer to an NDIS_MINIPORT_RESTART_PARAMETERS structure that defines 
//		the restart parameters for the miniport adapter. 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	my_MiniportRestart(
		IN NDIS_HANDLE  MiniportAdapterContext,
		IN PNDIS_MINIPORT_RESTART_PARAMETERS  MiniportRestartParameters
		)
{
	PKIP_NDIS_MINIPORT pMiniport;
	PKIP_NDIS_ADAPTER pAdapter;
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );
	ASSERT( pAdapter );
	if ( pAdapter )
	{
		DBGLOG(( LDebug, "Restarting miniport %p\n", pAdapter ));

		pMiniport = pAdapter->Interface.Miniport;
		nStatus = 
			pMiniport->Ndis6.MiniportCharacteristics.RestartHandler(
				MiniportAdapterContext,
				MiniportRestartParameters
				);
		if ( nStatus == NDIS_STATUS_SUCCESS ){
			pAdapter->Flags &= ~KIP_ADAPTER_PAUSED;
			// TODO: notify protocol
		}
		DBGLOG(( LDebug, "Miniport is restarted %p\n", pAdapter ));
	}
	return nStatus;
}

//***********************************************************************************
// Name: my_MiniportResetEx
//
// Description: 
//		MiniportResetEx hook.
//		NDIS calls a miniport driver's MiniportResetEx function to initiate a reset of a NIC.
//
// Return value: 
//		Operation status
//
// Parameters
//	MiniportAdapterContext 
//		A handle to a context area that the miniport driver allocated in its MiniportInitializeEx 
//		function. The miniport driver uses this context area to maintain state 
//		information for an miniport adapter. 
//	AddressingReset 
//		A pointer to a Boolean variable. The driver sets this variable to TRUE 
//		if NDIS should call the MiniportOidRequest function to restore addressing 
//		information to the current values.
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	my_MiniportResetEx(
		IN NDIS_HANDLE  MiniportAdapterContext,
		IN PBOOLEAN  AddressingReset
		)
{
	PKIP_NDIS_MINIPORT pMiniport;
	PKIP_NDIS_ADAPTER  pAdapter;
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );
	ASSERT( pAdapter );
	if ( pAdapter )
	{
		DBGLOG(( LDebug, "Resetting miniport %p\n", pAdapter ));
		pMiniport = pAdapter->Interface.Miniport;

		// miniport is resetting
		KIP_MINIPORT_SET_FLAG(pAdapter,fADAPTER_RESET_IN_PROGRESS);

		nStatus = 
			pMiniport->Ndis6.MiniportCharacteristics.ResetHandlerEx(
				MiniportAdapterContext,
				AddressingReset
				);
		if ( nStatus == NDIS_STATUS_SUCCESS ){
			// reset is complete
			KIP_MINIPORT_CLEAR_FLAG(pAdapter,fADAPTER_RESET_IN_PROGRESS);
		}
		DBGLOG(( LDebug, "Miniport is reset %p\n", pAdapter ));
	}
	return nStatus;
}

//***********************************************************************************
// Name: my_NdisMRegisterMiniportDriver
//
// Description: 
//		NdisMRegisterMiniportDriver hook.
//		A miniport driver calls the NdisMRegisterMiniportDriver function to register 
//		MiniportXxx entry points with NDIS as the first step in initialization.
//		we intercept it to obtain miniport's handlers and miniport handle
//
// Return value: 
//		operation status
//
// Parameters: 
//
//	DriverObject - A pointer to an opaque driver object that the miniport driver received 
//					in its DriverEntry routine at the Argument1 parameter 
//					(see DriverEntry of NDIS Miniport Drivers). 
//
//	RegistryPath - A pointer to an opaque registry path that the miniport driver received 
//					in its DriverEntry routine at the Argument2 parameter. 
//
//	MiniportDriverContext - A handle to a driver-allocated context area where the driver 
//							maintains state and configuration information. 
//
//	MiniportDriverCharacteristics - A pointer to an NDIS_MINIPORT_DRIVER_CHARACTERISTICS 
//									structure that the caller initialized. 
//
//	NdisMiniportDriverHandle - A pointer to a caller-supplied handle variable. 
//								NDIS writes a handle to this variable that uniquely 
//								identifies this driver. The driver must save this handle 
//								for use in subsequent NdisXxx function calls.
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	my_NdisMRegisterMiniportDriver(
		IN PDRIVER_OBJECT  DriverObject,
		IN PUNICODE_STRING  RegistryPath,
		IN NDIS_HANDLE  MiniportDriverContext,
		IN PNDIS_MINIPORT_DRIVER_CHARACTERISTICS  MiniportDriverCharacteristics,
		OUT PNDIS_HANDLE  NdisMiniportDriverHandle
		)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	PKIP_NDIS_MINIPORT pMiniport;

	ENTER_HOOK();

	if ( (MiniportDriverCharacteristics->Flags & NDIS_INTERMEDIATE_DRIVER) == NDIS_INTERMEDIATE_DRIVER )
	{
		Status = 
			NDIS_ORIGINAL(NdisMRegisterMiniportDriver)(
				DriverObject,
				RegistryPath,
				MiniportDriverContext,
				MiniportDriverCharacteristics,
				NdisMiniportDriverHandle
				);
		LEAVE_HOOK();
		return Status;
	}

	//allocate new adapter structure
	pMiniport = NDIS_AllocateMiniport();

	if ( pMiniport == NULL ){
		DBGLOG(( LCriticalError, "NDIS_AllocateMiniport faied\n" ));
		LEAVE_HOOK();
		return NDIS_STATUS_FAILURE;
	}

	pMiniport->MajorNdisVersion = MiniportDriverCharacteristics->MajorNdisVersion;
	pMiniport->MinorNdisVersion = MiniportDriverCharacteristics->MinorNdisVersion;
	
	#undef SET_MINIPORT_FIELD
	#define SET_MINIPORT_FIELD(_MINIPORT,_FIELD) \
		(_MINIPORT)->Ndis6.MiniportCharacteristics._FIELD = MiniportDriverCharacteristics->_FIELD

	SET_MINIPORT_FIELD( pMiniport, Header );
	SET_MINIPORT_FIELD( pMiniport, MajorNdisVersion );
	SET_MINIPORT_FIELD( pMiniport, MinorNdisVersion );
	SET_MINIPORT_FIELD( pMiniport, MajorDriverVersion );
	SET_MINIPORT_FIELD( pMiniport, MinorDriverVersion );
	SET_MINIPORT_FIELD( pMiniport, Flags );
	SET_MINIPORT_FIELD( pMiniport, SetOptionsHandler );
	SET_MINIPORT_FIELD( pMiniport, InitializeHandlerEx );
	SET_MINIPORT_FIELD( pMiniport, HaltHandlerEx );
	SET_MINIPORT_FIELD( pMiniport, UnloadHandler );
	SET_MINIPORT_FIELD( pMiniport, PauseHandler );
	SET_MINIPORT_FIELD( pMiniport, RestartHandler );
	SET_MINIPORT_FIELD( pMiniport, OidRequestHandler );
	SET_MINIPORT_FIELD( pMiniport, SendNetBufferListsHandler );
	SET_MINIPORT_FIELD( pMiniport, ReturnNetBufferListsHandler );
	SET_MINIPORT_FIELD( pMiniport, CancelSendHandler );
	SET_MINIPORT_FIELD( pMiniport, CheckForHangHandlerEx );
	SET_MINIPORT_FIELD( pMiniport, ResetHandlerEx );
	SET_MINIPORT_FIELD( pMiniport, DevicePnPEventNotifyHandler );
	SET_MINIPORT_FIELD( pMiniport, ShutdownHandlerEx );
	SET_MINIPORT_FIELD( pMiniport, CancelOidRequestHandler );

	pMiniport->Ndis6.NdisMiniportAdapterContext = MiniportDriverContext;

	//hook miniport handlers
	if ( MiniportDriverCharacteristics->InitializeHandlerEx ){
		//MiniportDriverCharacteristics->InitializeHandlerEx = my_MiniportInitializeEx;

		MiniportDriverCharacteristics->InitializeHandlerEx = 
			MakeCallStub(
				&pMiniport->Ndis6.STUB_MiniportInitializeEx,
				&my_MiniportInitializeEx,
				pMiniport,
				3
				);
	}
	// hook state functions
	if ( MiniportDriverCharacteristics->HaltHandlerEx ){
		MiniportDriverCharacteristics->HaltHandlerEx  = my_MiniportHaltEx;	
	}
	if ( MiniportDriverCharacteristics->PauseHandler ){
		MiniportDriverCharacteristics->PauseHandler   = my_MiniportPause;
	}
	if ( MiniportDriverCharacteristics->RestartHandler ){
		MiniportDriverCharacteristics->RestartHandler = my_MiniportRestart;
	}
	if ( MiniportDriverCharacteristics->ResetHandlerEx ){
		MiniportDriverCharacteristics->ResetHandlerEx = my_MiniportResetEx;
	}
	
	Status = 
		NDIS_ORIGINAL(NdisMRegisterMiniportDriver)(
			DriverObject,
			RegistryPath,
			MiniportDriverContext,
			MiniportDriverCharacteristics,
			NdisMiniportDriverHandle
			);

	if ( Status == NDIS_STATUS_SUCCESS )
	{
		pMiniport->Ndis6.NdisMiniportDriverHandle = *NdisMiniportDriverHandle;
	}
	else
	{
		DBGLOG(( LInfo, "%08X=NdisMRegisterMiniportDriver failed\n", Status ));
		NDIS_FreeMiniport( pMiniport );
	}

	LEAVE_HOOK();

	return Status;
}

//***********************************************************************************
// Name: my_NdisMDeregisterMiniportDriver
//
// Description: 
//		NdisMDeregisterMiniportDriver hook.
//		A miniport driver calls the NdisMDeregisterMiniportDriver function to release 
//		resources that it allocated with a previous call to the NdisMRegisterMiniportDriver 
//		function.
//		we releases our structures here
//
// Return value: 
//		None
//
// Parameters: 
//
//	NdisMiniportDriverHandle - The handle that the miniport driver obtained in 
//								a previous call to NdisMRegisterMiniportDriver. 
// NOTE: None
// **********************************************************************************

VOID
	my_NdisMDeregisterMiniportDriver(
		IN NDIS_HANDLE  NdisMiniportDriverHandle
		) 
{
	PKIP_NDIS_MINIPORT pMiniport;

	ENTER_HOOK();

	//lookup adapter by handle
	pMiniport = NDIS_FindMiniportByDriverHandle( NdisMiniportDriverHandle );
	if ( pMiniport ) {

		DBGLOG(( LDebug, "Removing miniport\n" ));

		//remove adapter in global list
		NDIS_FreeMiniport( pMiniport );
	}

	NDIS_ORIGINAL(NdisMDeregisterMiniportDriver)(NdisMiniportDriverHandle);

	LEAVE_HOOK();
}

//***********************************************************************************
// Name: my_NdisMSetMiniportAttributes
//
// Description: 
//		NdisMSetMiniportAttributes hook.
//		A miniport driver must call the NdisMSetMiniportAttributes function from its 
//		MiniportInitializeEx function to identify a context area for miniport adapter 
//		to NDIS, and to provide NDIS with information about the miniport adapter.
//		we intercetp adapter information here
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	my_NdisMSetMiniportAttributes (
		IN NDIS_HANDLE  NdisMiniportAdapterHandle,
		IN PNDIS_MINIPORT_ADAPTER_ATTRIBUTES  MiniportAttributes
		)
{
	NDIS_STATUS nStatus;
	PKIP_NDIS_ADAPTER pAdapter;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( NdisMiniportAdapterHandle );
	if ( pAdapter )
	{
		MpSetMiniportAttributes(
			pAdapter,
			MiniportAttributes
			);
	}

	nStatus = 
		NDIS_ORIGINAL(NdisMSetMiniportAttributes)(
			NdisMiniportAdapterHandle,
			MiniportAttributes
			);

	LEAVE_HOOK();

	return nStatus;
}
//***********************************************************************************
// Name: my_NdisMIndicateReceiveNetBufferLists
//
// Description: 
//		NdisMIndicateReceiveNetBufferLists hook.
//		Miniport drivers call the NdisMIndicateReceiveNetBufferLists function 
//		to indicate the receipt of data from the network.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMIndicateReceiveNetBufferLists(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNET_BUFFER_LIST  NetBufferLists,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN ULONG  NumberOfNetBufferLists,
		IN ULONG  ReceiveFlags
		)
{
	PKIP_NDIS_ADAPTER pAdapter;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		MpIndicateReceiveNetBufferLists(
			pAdapter,
			&NetBufferLists,
			PortNumber,
			&NumberOfNetBufferLists,
			ReceiveFlags
			);
	}

	if ( NetBufferLists && NumberOfNetBufferLists ){
		// call original
		NDIS_ORIGINAL(NdisMIndicateReceiveNetBufferLists)(
			MiniportAdapterHandle,
			NetBufferLists,
			PortNumber,
			NumberOfNetBufferLists,
			ReceiveFlags
			);
	}

	LEAVE_HOOK();
}

//***********************************************************************************
// Name: my_NdisMSendNetBufferListsComplete
//
// Description: 
//		NdisMSendNetBufferListsComplete hook.
//		Miniport drivers call the NdisMSendNetBufferListsComplete function to return 
//		a linked list of NET_BUFFER_LIST structures to an overlying driver and 
//		to return the final status of a send request.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMSendNetBufferListsComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNET_BUFFER_LIST  NetBufferLists,
		IN ULONG  SendCompleteFlags
		)
{
	PKIP_NDIS_ADAPTER pAdapter;
	BOOLEAN fbResult = FALSE;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		// our miniport completes all buffers that we own
		// and returns the lists that should be completed by ndis
		MpSendNetBufferListsComplete(
			pAdapter,
			&NetBufferLists,
			SendCompleteFlags
			);
	}

	// complete all the buffers that we don't own
	if ( NetBufferLists ) {
		// call original
		NDIS_ORIGINAL(NdisMSendNetBufferListsComplete)(
			MiniportAdapterHandle,
			NetBufferLists,
			SendCompleteFlags
			);
	}

	LEAVE_HOOK();
}
//***********************************************************************************
// Name: my_NdisMRestartComplete
//
// Description: 
//		NdisMRestartComplete hook.
//		A miniport driver must call the NdisMRestartComplete function to complete 
//		a restart operation if the driver returned NDIS_STATUS_PENDING from 
//		its MiniportRestart function.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMRestartComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		)
{
	PKIP_NDIS_ADAPTER pAdapter;
	ENTER_HOOK();


	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter )
	{
		DBGLOG(( LDebug, "Restarting miniport %p\n", pAdapter ));

		if ( Status == NDIS_STATUS_SUCCESS ){
			pAdapter->Flags &= ~KIP_ADAPTER_PAUSED;
			// TODO: notify protocol
		}
	}

	// call original
	NDIS_ORIGINAL(NdisMRestartComplete)(
		MiniportAdapterHandle,
		Status
		);

	LEAVE_HOOK();
}

//***********************************************************************************
// Name: my_NdisMResetComplete
//
// Description: 
//		NdisMResetComplete hook.
//		The NdisMResetComplete function returns the final status of a reset request 
//		for which the miniport driver previously returned NDIS_STATUS_PENDING.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMResetComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status,
		IN BOOLEAN  AddressingReset
		)
{
	PKIP_NDIS_ADAPTER pAdapter;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter )
	{
		DBGLOG(( LDebug, "Reset complete %p\n", pAdapter ));

		if ( Status == NDIS_STATUS_SUCCESS ){
			KIP_MINIPORT_CLEAR_FLAG(pAdapter,fADAPTER_RESET_IN_PROGRESS);
		}
	}
	// call original
	NDIS_ORIGINAL(NdisMResetComplete)(
		MiniportAdapterHandle,
		Status,
		AddressingReset
		);

	LEAVE_HOOK();
}

//***********************************************************************************
// Name: my_NdisMIndicateStatusEx
//
// Description: 
//		NdisMIndicateStatusEx hook.
//		Miniport drivers call the NdisMIndicateStatusEx function to report a change 
//		in the status of a miniport adapter.
//
// Return value: 
//		None
//
// Parameters: 
//	MiniportAdapterHandle 
//		The miniport adapter handle that NDIS passed at the MiniportAdapterHandle 
//		parameter of the MiniportInitializeEx function. 
//	StatusIndication 
//		A pointer to an NDIS_STATUS_INDICATION structure that contains the status 
//		information. 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMIndicateStatusEx(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_STATUS_INDICATION  StatusIndication
		) 
{
	PKIP_NDIS_ADAPTER pAdapter;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		MpIndicateStatusHandler(
			pAdapter,
			StatusIndication->StatusCode,
			StatusIndication->StatusBuffer,
			StatusIndication->StatusBufferSize,
			TRUE
			);
	}

	// call original
	NDIS_ORIGINAL(NdisMIndicateStatusEx)(
		MiniportAdapterHandle,
		StatusIndication
		);

	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		MpIndicateStatusHandler(
			pAdapter,
			StatusIndication->StatusCode,
			StatusIndication->StatusBuffer,
			StatusIndication->StatusBufferSize,
			FALSE //after
			);
	}

	LEAVE_HOOK();
}

//***********************************************************************************
// Name: my_NdisMOidRequestComplete
//
// Description: 
//		NdisMOidRequestComplete hook.
//		Miniport drivers call the NdisMOidRequestComplete function to return the final 
//		status of an OID request for which the driver's MiniportOidRequest function 
//		returned NDIS_STATUS_PENDING.

//
// Return value: 
//		None
//
// Parameters: 
//	MiniportAdapterHandle 
//		The miniport adapter handle that NDIS passed at the MiniportAdapterHandle 
//		parameter of the MiniportInitializeEx function. 
//	OidRequest 
//		A pointer to a buffer that is formatted as an NDIS_OID_REQUEST structure. 
//		The miniport driver obtained this pointer as an input parameter to its 
//		MiniportOidRequest function. 
//	Status 
//		The final status of the request operation, either NDIS_STATUS_SUCCESS, 
//		NDIS_STATUS_REQUEST_ABORTED, or any driver-determined NDIS_STATUS_XXX except 
//		NDIS_STATUS_PENDING.
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMOidRequestComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_OID_REQUEST  OidRequest,
		IN NDIS_STATUS  Status
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;
	BOOLEAN fbCallOriginal = TRUE;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		// call miniport
		MpOidRequestComplete(
			pAdapter,
			OidRequest,
			Status,
			&fbCallOriginal
			);
	}

	if ( fbCallOriginal ){
		NDIS_ORIGINAL(NdisMOidRequestComplete)( 
			MiniportAdapterHandle, 
			OidRequest,
			Status
			);
	}

	LEAVE_HOOK();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 
//		NDIS 5.0 interface
//

//***********************************************************************************
// Name: my_PacketIndicateHandler
//
// Description: 
//		MiniportBlock.PacketIndicateHandler hook.
//		This routine is called by the Miniport to indicate packets.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_PacketIndicateHandler(
		IN  NDIS_HANDLE             Miniport,
		IN  PPNDIS_PACKET           PacketArray,
		IN  UINT                    NumberOfPackets
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)Miniport;
	PKIP_NDIS_ADAPTER		pAdapter;
	PPNDIS_PACKET           SavedPacketArray = PacketArray;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( Miniport );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) && KIP_IS_ADAPTER_INITIALIZED(pAdapter) ){
			MpIndicatePacketsHandler(
				pAdapter,
				&PacketArray,
				&NumberOfPackets
				);
		}
		if ( NumberOfPackets ){
			// call original
			pAdapter->PacketIndicateHandler(
				Miniport,
				PacketArray,
				NumberOfPackets
				);
		}
		if ( PacketArray != SavedPacketArray ){
			ExFreePool( PacketArray );
		}
	}
}
//***********************************************************************************
// Name: my_MiniportReturnPacket
//
// Description: 
//		MiniportBlock.ReturnPacketHandler hook.
//		here we releases our packet copy 
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_MiniportReturnPacket(
		IN NDIS_HANDLE  MiniportAdapterContext,
		IN PNDIS_PACKET  Packet
		) 
{
	PKIP_NDIS_ADAPTER		pAdapter;
	PKIP_NDIS_MINIPORT		pMiniport;
	BOOLEAN fbCallOrig = TRUE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		pMiniport = pAdapter->Interface.Miniport;
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) && KIP_IS_ADAPTER_INITIALIZED(pAdapter) ){
			MpReturnPacketHandler(
				pAdapter,
				&Packet,
				&fbCallOrig
				);
		}
		if ( fbCallOrig ){

			pMiniport->Ndis5.MiniportCharacteristics.ReturnPacketHandler(
				MiniportAdapterContext,
				Packet
				);
		}
	}
}
//***********************************************************************************
// Name: my_SendCompleteHandler
//
// Description: 
//		MiniportBlock.SendCompleteHandler hook.
//		Called from NdisMSendComplete (which is macro)
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_SendCompleteHandler(
		IN  NDIS_HANDLE             MiniportAdapterHandle,
		IN  PVOID                   Packet,
		IN  NDIS_STATUS             Status
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;
	BOOLEAN fbResult;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) ){
			fbResult = 
				MpSendPacketCompleteHandler(
					pAdapter,
					(PNDIS_PACKET)Packet,
					Status
					);

			// TRUE, if we own the packet
		}else{
			fbResult = FALSE;
		}
		
		//we do not own this packet
		// pass it down
		if ( fbResult == FALSE ){
			// call original
			pAdapter->SendCompleteHandler(
				MiniportAdapterHandle,
				Packet,
				Status
				);
		}
	}
}
//***********************************************************************************
// Name: my_ResetCompleteHandler
//
// Description: 
//		MiniportBlock.ResetCompleteHandler hook.
//		returns the final status of a reset request for which the miniport driver 
//		previously returned NDIS_STATUS_PENDING.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_ResetCompleteHandler(
		IN  NDIS_HANDLE             MiniportAdapterHandle,
		IN  NDIS_STATUS             Status,
		IN  BOOLEAN                 AddressingReset
	)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		// TODO: 

		DBGLOG(( LDebug, "Reset complete %p\n", pAdapter ));

		// call original
		pAdapter->ResetCompleteHandler(
			MiniportAdapterHandle,
			Status,
			AddressingReset
			);
	}
}
//***********************************************************************************
// Name: my_EthRxIndicateHandler
//
// Description: 
//		MiniportBlock.EthRxIndicateHandler hook.
//		This routine is called by the MAC to indicate a packet to
//		all bindings.  The packet will be filtered so that only the
//		appropriate bindings will receive the packet.  This is the
//		code path for ndis 3.0 miniport drivers.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_EthRxIndicateHandler(
		IN PETH_FILTER             Filter,
		IN NDIS_HANDLE             MacReceiveContext,
		IN PCHAR                   Address,
		IN PVOID                   HeaderBuffer,
		IN UINT                    HeaderBufferSize,
		IN PVOID                   LookaheadBuffer,
		IN UINT                    LookaheadBufferSize,
		IN UINT                    PacketSize
		)
{
	PKIP_NDIS_ADAPTER       pAdapter;
	BOOLEAN fbCallOriginal = FALSE;

	pAdapter = NDIS_FindAdapterByHandle( (NDIS_HANDLE)Filter->Miniport );
	
	ASSERT(pAdapter);
	if ( pAdapter )
	{
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) ){
			MpIndicateEthReceiveHandler(
				pAdapter,
				MacReceiveContext,
				HeaderBuffer,
				HeaderBufferSize,
				LookaheadBuffer,
				LookaheadBufferSize,
				PacketSize,
				&fbCallOriginal
				);
		}
		if ( fbCallOriginal == TRUE ){
			// call original
			pAdapter->EthRxIndicateHandler(
				Filter,
				MacReceiveContext,
				Address,
				HeaderBuffer,
				HeaderBufferSize,
				LookaheadBuffer,
				LookaheadBufferSize,
				PacketSize
				);
		}
	}
}

//***********************************************************************************
// Name: my_EthRxCompleteHandler
//
// Description: 
//		MiniportBlock.EthRxCompleteHandler hook.
//		This function notifies NDIS that an Ethernet receive packet, identified in 
//		a preceding call to the NdisMEthIndicateReceive function, has been fully 
//		transferred by the NIC so that NDIS can notify the appropriate bound protocols.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_EthRxCompleteHandler(
		IN PETH_FILTER             Filter
		)
{
	PKIP_NDIS_ADAPTER       pAdapter;
	pAdapter = NDIS_FindAdapterByHandle( (NDIS_HANDLE)Filter->Miniport );

	ASSERT(pAdapter);
	if ( pAdapter )
	{
		// TODO: 
		// call original
		pAdapter->EthRxCompleteHandler(
			Filter
			);
	}
}
//***********************************************************************************
// Name: my_WanSendCompleteHandler
//
// Description: 
//		MiniportBlock.WanSendCompleteHandler hook.
//		returns the packet and final status of a completed send 
//		request for which the miniport driver previously returned NDIS_STATUS_PENDING.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_WanSendCompleteHandler(
		IN  NDIS_HANDLE             MiniportAdapterHandle,
		IN  PVOID                   Packet,
		IN  NDIS_STATUS             Status
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		// TODO: 

		// call original
		pAdapter->WanSendCompleteHandler(
			MiniportAdapterHandle,
			Packet,
			Status
			);
	}
}
//***********************************************************************************
// Name: my_WanRcvHandler
//
// Description: 
//		MiniportBlock.WanRcvHandler hook.
//		notifies NDISWAN that a packet has been received for an established link to 
//		be forwarded to the appropriate bound protocol driver.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_WanRcvHandler(
		OUT PNDIS_STATUS            Status,
		IN  NDIS_HANDLE              MiniportAdapterHandle,
		IN  NDIS_HANDLE              NdisLinkContext,
		IN  PUCHAR                   Packet,
		IN  ULONG                    PacketSize
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		PKIP_RAS_LINK pLink = NDIS_FindRasLinkByHandle( pAdapter, NdisLinkContext );
		if ( pLink ){

		}

		// call original
		pAdapter->WanRcvHandler(
			Status,
			MiniportAdapterHandle,
			NdisLinkContext,
			Packet,
			PacketSize
			);
	}
}
//***********************************************************************************
// Name: my_WanRcvCompleteHandler
//
// Description: 
//		MiniportBlock.WanRcvCompleteHandler hook.
//		notifies NDISWAN that one or more receives have been completed for an established 
//		link so that NDISWAN can notify the appropriate bound protocol(s).
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_WanRcvCompleteHandler(
		IN NDIS_HANDLE              MiniportAdapterHandle,
		IN NDIS_HANDLE              NdisLinkContext
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		// TODO: 

		// call original
		pAdapter->WanRcvCompleteHandler(
			MiniportAdapterHandle,
			NdisLinkContext
			);
	}
}
//***********************************************************************************
// Name: my_StatusHandler
//
// Description: 
//		MiniportBlock.StatusHandler hook.
//		indicates the status.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_StatusHandler(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  GeneralStatus,
		IN PVOID  StatusBuffer,
		IN UINT  StatusBufferSize
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;


	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) ) {
			MpIndicateStatusHandler(
				pAdapter,
				GeneralStatus,
				StatusBuffer,
				StatusBufferSize,
				TRUE
				);
		}

		// call original
		pAdapter->StatusHandler(
			MiniportAdapterHandle,
			GeneralStatus,
			StatusBuffer,
			StatusBufferSize
			);

		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) ) {
			MpIndicateStatusHandler(
				pAdapter,
				GeneralStatus,
				StatusBuffer,
				StatusBufferSize,
				FALSE //after
				);
		}
	}
}
//***********************************************************************************
// Name: my_StatusCompleteHandler
//
// Description: 
//		MiniportBlock.StatusCompleteHandler hook.
//		indicates the status is complete.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_StatusCompleteHandler(
		IN  NDIS_HANDLE	MiniportAdapterHandle
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		// TODO: handle the status request completion
		// call original
		pAdapter->StatusCompleteHandler(
			MiniportAdapterHandle
			);
	}
}
//***********************************************************************************
// Name: my_MiniportSetInformation
//
// Description: 
//		MiniportBlock.SetInformationHandler hook.
//		allows bound protocol drivers (or NDIS) to request changes in the state 
//		information that the miniport driver maintains for particular OIDs, such 
//		as changes in multicast addresses.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	my_MiniportSetInformation(
		IN NDIS_HANDLE             MiniportAdapterContext,
		IN NDIS_OID                Oid,
		IN PVOID                   InformationBuffer,
		IN ULONG                   InformationBufferLength,
		OUT PULONG                 BytesRead,
		IN PULONG                  BytesNeeded
		)
{
	PKIP_NDIS_ADAPTER pAdapter;
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;
	BOOLEAN fbCallOriginal = TRUE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );
	ASSERT( pAdapter );
	if ( pAdapter )
	{
		PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter ))
		{	
			// save the current set request
			pAdapter->Interface.SetInformationRequest.Internal = FALSE;
			pAdapter->Interface.SetInformationRequest.Oid = Oid;
			pAdapter->Interface.SetInformationRequest.InformationBuffer = InformationBuffer;
			pAdapter->Interface.SetInformationRequest.InformationBufferLength = InformationBufferLength;
			pAdapter->Interface.SetInformationRequest.BytesRead = BytesRead;
			pAdapter->Interface.SetInformationRequest.BytesNeeded = BytesNeeded;

			// call miniport
			nStatus = 
				MpSetInformationHandler(
					pAdapter,
					&pAdapter->Interface.SetInformationRequest,
					&fbCallOriginal
					);

			if ( fbCallOriginal )
			{
				// call original			
				nStatus =
					pMiniport->Ndis5.MiniportCharacteristics.SetInformationHandler(
						MiniportAdapterContext,
						Oid,
						InformationBuffer,
						InformationBufferLength,
						BytesRead,
						BytesNeeded
						);
				if ( nStatus != NDIS_STATUS_PENDING ){
					MpSetInformationCompleteHandler(
						pAdapter,
						&pAdapter->Interface.SetInformationRequest,
						nStatus,
						&fbCallOriginal
						);
				}
			}
		}else {
			// call original			
			nStatus =
				pMiniport->Ndis5.MiniportCharacteristics.SetInformationHandler(
					MiniportAdapterContext,
					Oid,
					InformationBuffer,
					InformationBufferLength,
					BytesRead,
					BytesNeeded
					);
		}
	}
	return nStatus;
}
//***********************************************************************************
// Name: my_SetCompleteHandler
//
// Description: 
//		MiniportBlock.SetCompleteHandler hook.
//		indicates the completion of a set information operation.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_SetCompleteHandler(
		IN	NDIS_HANDLE				MiniportAdapterHandle,
		IN	NDIS_STATUS				Status
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;
	BOOLEAN fbCallOriginal = TRUE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) )
		{
			// handle it
			MpSetInformationCompleteHandler(
				pAdapter,
				&pAdapter->Interface.SetInformationRequest,
				Status,
				&fbCallOriginal
				);
		}

		if ( fbCallOriginal ){
			// call original
			pAdapter->SetCompleteHandler(
				MiniportAdapterHandle,
				Status
				);
		}
	}
}
//***********************************************************************************
// Name: my_MiniportQueryInformation
//
// Description: 
//		MiniportBlock.QueryInformationHandler hook.
//		returns information about the capabilities and status of the driver and/or its NIC.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	my_MiniportQueryInformation(
		IN NDIS_HANDLE             MiniportAdapterContext,
		IN NDIS_OID                Oid,
		IN PVOID                   InformationBuffer,
		IN ULONG                   InformationBufferLength,
		IN PULONG                  BytesWritten,
		IN PULONG                  BytesNeeded
		)
{
	PKIP_NDIS_ADAPTER pAdapter;
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;
	BOOLEAN fbCallOriginal = TRUE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );
	ASSERT( pAdapter );
	if ( pAdapter )
	{
		PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;

		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter ))
		{
			// save the current query request
			pAdapter->Interface.QueryInformationRequest.Internal = FALSE;
			pAdapter->Interface.QueryInformationRequest.Oid = Oid;
			pAdapter->Interface.QueryInformationRequest.InformationBuffer = InformationBuffer;
			pAdapter->Interface.QueryInformationRequest.InformationBufferLength = InformationBufferLength;
			pAdapter->Interface.QueryInformationRequest.BytesWritten = BytesWritten;
			pAdapter->Interface.QueryInformationRequest.BytesNeeded = BytesNeeded;

			// call miniport
			nStatus = 
				MpQueryInformationHandler(
					pAdapter,
					&pAdapter->Interface.QueryInformationRequest,
					&fbCallOriginal
					);

			if ( fbCallOriginal )
			{
				// call original
				nStatus =
					pMiniport->Ndis5.MiniportCharacteristics.QueryInformationHandler(
						MiniportAdapterContext,
						pAdapter->Interface.QueryInformationRequest.Oid,
						pAdapter->Interface.QueryInformationRequest.InformationBuffer,
						pAdapter->Interface.QueryInformationRequest.InformationBufferLength,
						pAdapter->Interface.QueryInformationRequest.BytesWritten,
						pAdapter->Interface.QueryInformationRequest.BytesNeeded
						);

				if ( nStatus != NDIS_STATUS_PENDING ){
					MpQueryInformationCompleteHandler(
						pAdapter,
						&pAdapter->Interface.QueryInformationRequest,
						nStatus,
						&fbCallOriginal
						);
				}
			}
		}else{
			// call original
			nStatus =
				pMiniport->Ndis5.MiniportCharacteristics.QueryInformationHandler(
					MiniportAdapterContext,
					Oid,
					InformationBuffer,
					InformationBufferLength,
					BytesWritten,
					BytesNeeded
					);
		}
	}
	return nStatus;
}
//***********************************************************************************
// Name: my_QueryCompleteHandler
//
// Description: 
//		MiniportBlock.QueryCompleteHandler hook.
//		indicates the completion of a query information operation.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_QueryCompleteHandler(
		IN	NDIS_HANDLE				MiniportAdapterHandle,
		IN	NDIS_STATUS				Status
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;
	BOOLEAN fbCallOriginal = TRUE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) ){
			// call miniport
			MpQueryInformationCompleteHandler(
				pAdapter,
				&pAdapter->Interface.QueryInformationRequest,
				Status,
				&fbCallOriginal
				);
		}

		if ( fbCallOriginal ){
			// call original
			pAdapter->QueryCompleteHandler(
				MiniportAdapterHandle,
				Status
				);
		}
	}
}

//***********************************************************************************
// Name: my_TDCompleteHandler
//
// Description: 
//		MiniportBlock.TDCompleteHandler hook.
//		indicates the completion of a transfer data request.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_TDCompleteHandler(
		IN	NDIS_HANDLE				MiniportAdapterHandle,
		IN	PNDIS_PACKET			Packet,
		IN	NDIS_STATUS				Status,
		IN	UINT					BytesTransferred
		)
{
	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;
	BOOLEAN fbCallOrig = TRUE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		if ( KIP_MINIPORT_IS_ACTIVE(pAdapter) )
		{
			MpTransferDataComplete(
				pAdapter,
				Packet,
				Status,
				BytesTransferred,
				&fbCallOrig
				);
		}

		if ( fbCallOrig ){
			// call original
			pAdapter->TDCompleteHandler(
				MiniportAdapterHandle,
				Packet,
				Status,
				BytesTransferred
				);
		}
	}
}
//***********************************************************************************
// Name: my_ndisPnPDispatch
//
// Description: 
//		DRIVER_OBJECT.MajorFunction[IRP_MJ_PNP]
//		miniport PNP major handler
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

NTSTATUS
	my_ndisPnPDispatch(
		IN	PDEVICE_OBJECT			DeviceObject,
		IN	PIRP					Irp
		)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	PKIP_NDIS_MINIPORT  pMiniport;
	PIO_STACK_LOCATION IoStackLocation = IoGetCurrentIrpStackLocation( Irp );
	UCHAR MajorFunction = IoStackLocation->MajorFunction;
	UCHAR MinorFunction = IoStackLocation->MinorFunction;

	pMiniport = NDIS_FindMiniportByDriverObject( DeviceObject->DriverObject );

	ASSERT(pMiniport);
	if ( pMiniport ){
		ntStatus = pMiniport->PnPDispatch ( DeviceObject, Irp );
		if ( NT_SUCCESS ( ntStatus ) && MinorFunction == IRP_MN_START_DEVICE ) {
			PKIP_NDIS_ADAPTER  pAdapter;
			pAdapter = NDIS_FindAdapterByDeviceObject( pMiniport, DeviceObject );
			if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter) ){
				PNDIS_MINIPORT_BLOCK pNdisMiniport = (PNDIS_MINIPORT_BLOCK)pAdapter->NdisMiniportHandle;
				if ( pNdisMiniport->PacketIndicateHandler != my_PacketIndicateHandler ){
					if ( pNdisMiniport->PacketIndicateHandler ){
						pAdapter->PacketIndicateHandler = pNdisMiniport->PacketIndicateHandler;
						pNdisMiniport->PacketIndicateHandler = my_PacketIndicateHandler;
					}
				}
			}
		}
	}

	return ntStatus;
}
//***********************************************************************************
// Name: NdisReinstallMiniportHook
//
// Description: 
//		Reinstalls some NDIS_MINIPORT_BLOCK_HOOKS that ndis can modify dynamically.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	NdisReinstallMiniportHook(
		IN PKIP_NDIS_ADAPTER pAdapter
		)
{
	PNDIS_MINIPORT_BLOCK pNdisMiniport = (PNDIS_MINIPORT_BLOCK)pAdapter->NdisMiniportHandle;
	if ( pNdisMiniport->PacketIndicateHandler != my_PacketIndicateHandler ){
		if ( pNdisMiniport->PacketIndicateHandler ){
			pAdapter->PacketIndicateHandler = pNdisMiniport->PacketIndicateHandler;
			pNdisMiniport->PacketIndicateHandler = my_PacketIndicateHandler;
		}
	}

	if ( pNdisMiniport->SendCompleteHandler ){
		if ( (PVOID)pNdisMiniport->SendCompleteHandler != (PVOID)NDIS_ORIGINAL(NdisMSendComplete) &&
			(PVOID)pNdisMiniport->SendCompleteHandler != my_SendCompleteHandler )
		{
			pAdapter->SendCompleteHandler = pNdisMiniport->SendCompleteHandler;
			pNdisMiniport->SendCompleteHandler = (NDIS_M_SEND_COMPLETE_HANDLER)my_SendCompleteHandler;
		}
	}
}

//***********************************************************************************
// Name: my_MiniportInitialize
//
// Description: 
//		MiniportInitialize hook.
//		The MiniportInitialize function is a required function that sets up a NIC 
//		(or virtual NIC) for network I/O operations, claims all hardware resources 
//		necessary to the NIC in the registry, and allocates resources the driver needs 
//		to carry out network I/O operations.
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS 
	my_MiniportInitialize(
		IN PKIP_NDIS_MINIPORT pMiniport,
		OUT PNDIS_STATUS  OpenErrorStatus,
		OUT PUINT  SelectedMediumIndex,
		IN PNDIS_MEDIUM  MediumArray,
		IN UINT  MediumArraySize,
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_HANDLE  WrapperConfigurationContext
		)
{
	PNDIS_MINIPORT_BLOCK pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	NDIS_STATUS nStatus;
	NDIS_STATUS nErrorCode;
	PKIP_NDIS_ADAPTER pAdapter;

	//lookup adapter by handle
	ASSERT(pMiniport);

	//allocate new adapter
	pAdapter = NDIS_AllocateAdapter( pMiniport );
	if ( pAdapter == NULL ){
		return NDIS_STATUS_FAILURE;
	}

	// save adapter handle
	pAdapter->NdisMiniportHandle = MiniportAdapterHandle;

		// save miniport original handlers
#undef SET_ADAPTER_FIELD
#define SET_ADAPTER_FIELD(_ADAPTER,_FIELD) \
	(_ADAPTER)->_FIELD = pNdisMiniport->_FIELD

	SET_ADAPTER_FIELD(pAdapter,PacketIndicateHandler);
	SET_ADAPTER_FIELD(pAdapter,SendCompleteHandler);
	SET_ADAPTER_FIELD(pAdapter,SendResourcesHandler);
	SET_ADAPTER_FIELD(pAdapter,ResetCompleteHandler);

	SET_ADAPTER_FIELD(pAdapter,SendPacketsHandler);
	SET_ADAPTER_FIELD(pAdapter,DeferredSendHandler);

	SET_ADAPTER_FIELD(pAdapter,EthRxIndicateHandler);
	SET_ADAPTER_FIELD(pAdapter,TrRxIndicateHandler);

	SET_ADAPTER_FIELD(pAdapter,EthRxCompleteHandler);
	SET_ADAPTER_FIELD(pAdapter,TrRxCompleteHandler);

	SET_ADAPTER_FIELD(pAdapter,StatusHandler);
	SET_ADAPTER_FIELD(pAdapter,StatusCompleteHandler);
	SET_ADAPTER_FIELD(pAdapter,TDCompleteHandler);
	SET_ADAPTER_FIELD(pAdapter,QueryCompleteHandler);
	SET_ADAPTER_FIELD(pAdapter,SetCompleteHandler);

	SET_ADAPTER_FIELD(pAdapter,WanSendCompleteHandler);
	SET_ADAPTER_FIELD(pAdapter,WanRcvHandler);
	SET_ADAPTER_FIELD(pAdapter,WanRcvCompleteHandler);

	// hook miniport handlers
	if ( pNdisMiniport->SendCompleteHandler ){
		if ( (PVOID)pNdisMiniport->SendCompleteHandler != (PVOID)NDIS_ORIGINAL(NdisMSendComplete) ){
			pNdisMiniport->SendCompleteHandler     = (NDIS_M_SEND_COMPLETE_HANDLER)my_SendCompleteHandler;
		}
	}
	if ( pNdisMiniport->ResetCompleteHandler ){
		if ( (PVOID)pNdisMiniport->ResetCompleteHandler != (PVOID)NDIS_ORIGINAL(NdisMResetComplete) ){
			pNdisMiniport->ResetCompleteHandler    = (NDIS_M_RESET_COMPLETE_HANDLER)my_ResetCompleteHandler;
		}
	}
	if ( pNdisMiniport->EthRxIndicateHandler ){
		//NDIS_MAKE_CALL_STUB(pAdapter,pNdisMiniport,EthRxIndicateHandler,8);
		pNdisMiniport->EthRxIndicateHandler = (ETH_RCV_INDICATE_HANDLER)my_EthRxIndicateHandler;
	}
	if ( pNdisMiniport->EthRxCompleteHandler ){
		//NDIS_MAKE_CALL_STUB(pAdapter,pNdisMiniport,EthRxCompleteHandler,1);
//		pNdisMiniport->EthRxCompleteHandler = (ETH_RCV_COMPLETE_HANDLER)my_EthRxCompleteHandler;
	}

	if ( pNdisMiniport->StatusHandler ){
		pNdisMiniport->StatusHandler = (NDIS_M_STATUS_HANDLER)my_StatusHandler;
	}
	if ( pNdisMiniport->StatusCompleteHandler ){
//		pNdisMiniport->StatusCompleteHandler = (NDIS_M_STS_COMPLETE_HANDLER)my_StatusCompleteHandler;
	}

	if ( pNdisMiniport->QueryCompleteHandler ){
		pNdisMiniport->QueryCompleteHandler = (NDIS_M_REQ_COMPLETE_HANDLER)my_QueryCompleteHandler;
	}
	if ( pNdisMiniport->SetCompleteHandler ){
		pNdisMiniport->SetCompleteHandler = (NDIS_M_REQ_COMPLETE_HANDLER)my_SetCompleteHandler;
	}

	if ( pNdisMiniport->TDCompleteHandler ){
		pNdisMiniport->TDCompleteHandler = (NDIS_M_TD_COMPLETE_HANDLER)my_TDCompleteHandler;
	}

	if ( pNdisMiniport->PacketIndicateHandler ){
		pNdisMiniport->PacketIndicateHandler   = (FILTER_PACKET_INDICATION_HANDLER)my_PacketIndicateHandler;
	}

	if ( pMiniport->MajorNdisVersion >= 4 ){
		if ( pNdisMiniport->WanSendCompleteHandler ){
//			pNdisMiniport->WanSendCompleteHandler = (NDIS_WM_SEND_COMPLETE_HANDLER)my_WanSendCompleteHandler;
		}
		if ( pNdisMiniport->WanRcvHandler ){
//			pNdisMiniport->WanRcvHandler = (WAN_RCV_HANDLER)my_WanRcvHandler;
		}
		if ( pNdisMiniport->WanRcvCompleteHandler ){
//			pNdisMiniport->WanRcvCompleteHandler = (WAN_RCV_COMPLETE_HANDLER)my_WanRcvCompleteHandler;
		}

		//
		//  Set some flags describing the miniport.
		//

		//
		//	Does this miniport indicate packets?
		//
		if ( pMiniport->Ndis5.MiniportCharacteristics.ReturnPacketHandler )
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_INDICATES_PACKETS);
		}
		if (pMiniport->MajorNdisVersion == 5)
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_IS_NDIS_5_0);

			if ( pMiniport->Ndis5.MiniportCharacteristics.CoSendPacketsHandler != NULL )
			{
				//
				//	This is a connection-oriented miniport.
				//
				KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_IS_CO);
			}
		}
	}

	KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_IN_INITIALIZE);

	// call original
	nStatus = 
		pMiniport->Ndis5.MiniportCharacteristics.InitializeHandler(
			OpenErrorStatus,
			SelectedMediumIndex,
			MediumArray,
			MediumArraySize,
			MiniportAdapterHandle, 
			WrapperConfigurationContext
			);

	KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_IN_INITIALIZE);

	if ( nStatus == NDIS_STATUS_SUCCESS )
	{
		// some handler can be modified by call above
		NdisReinstallMiniportHook(pAdapter);

		// initialize NDIS5 miniport interface
		nStatus = 
			MpInitialize( 
				pAdapter, 
				MediumArray[*SelectedMediumIndex]
				);

		// we do not support the underling medium
		if ( nStatus == NDIS_STATUS_NOT_SUPPORTED ){
			KIP_MINIPORT_SET_INACTIVE(pAdapter);
			nStatus = NDIS_STATUS_SUCCESS;
		}else if ( nStatus == NDIS_STATUS_SUCCESS ){

			if ( KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_IS_CO) == FALSE )
			{
				//
				// bind our protocol to miniport
				//
				nStatus = 
					ProtocolBindAdapter( 
						(PKIP_NDIS_INTERFACE)pAdapter,
						0,0,0,FALSE
						);
				if ( nStatus != NDIS_STATUS_SUCCESS ){
					DBGLOG(( LError, "%08X = ProtocolBindAdapter failed\n", nStatus ));
					KIP_MINIPORT_SET_INACTIVE(pAdapter);
					nStatus = NDIS_STATUS_SUCCESS;
				}
			}
		}

		// adapter is initialized
		pAdapter->Flags |= KIP_ADAPTER_INITIALIZED;
	}
	else
	{
		DBGLOG(( LInfo, "%08X=InitializeHandler failed\n", nStatus ));
		NDIS_FreeAdapter( pAdapter );
	}
	return nStatus;
}
//***********************************************************************************
// Name: my_MiniportHalt
//
// Description: 
//		MiniportCharacteristics.MiniportHalt hook.
//		The MiniportHalt function is a required function that deallocates resources 
//		when the NIC is removed and halts the NIC.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID 
	my_MiniportHalt(
		IN NDIS_HANDLE  MiniportAdapterContext
		)
{
	PKIP_NDIS_ADAPTER		pAdapter;
	PKIP_NDIS_MINIPORT		pMiniport;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		DBGLOG(( LDebug, "Halting miniport %p\n", pAdapter ));

		pMiniport = pAdapter->Interface.Miniport;
		KIP_MINIPORT_SET_FLAG(pAdapter,fADAPTER_HALTING);
		// call original
		pMiniport->Ndis5.MiniportCharacteristics.HaltHandler(
			MiniportAdapterContext
			);

		// halt adapter
		MpHalt( pAdapter );

		DBGLOG(( LDebug, "Miniport halted %p\n", pAdapter ));
		pAdapter = NULL;// adapter is not valid any more
	}
}
//***********************************************************************************
// Name: my_MiniportReset
//
// Description: 
//		MiniportCharacteristics.MiniportReset hook.
//		MiniportReset is a required function that issues a hardware reset to 
//		the NIC and/or resets the driver's software state. 
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS 
	my_MiniportReset(
		OUT PBOOLEAN  AddressingReset,
		IN NDIS_HANDLE  MiniportAdapterContext
		)
{
	PKIP_NDIS_ADAPTER		pAdapter;
	PKIP_NDIS_MINIPORT		pMiniport;
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByAdapterContext( MiniportAdapterContext );

	ASSERT( pAdapter );
	if ( pAdapter )
	{
		DBGLOG(( LDebug, "Resetting miniport %p\n", pAdapter ));

		pMiniport = pAdapter->Interface.Miniport;
		// miniport is resetting
		KIP_MINIPORT_SET_FLAG(pAdapter,fADAPTER_RESET_IN_PROGRESS);

		nStatus = 
			pMiniport->Ndis5.MiniportCharacteristics.ResetHandler(
				AddressingReset,
				MiniportAdapterContext
				);
		if ( nStatus == NDIS_STATUS_SUCCESS ){
			// reset is complete
			KIP_MINIPORT_CLEAR_FLAG(pAdapter,fADAPTER_RESET_IN_PROGRESS);
		}
		DBGLOG(( LDebug, "Reset done %p\n", pAdapter ));
	}
	return nStatus;
}

//***********************************************************************************
// Name: my_NdisMRegisterMiniport
//
// Description: 
//		NdisMRegisterMiniport hook.
//		registers a miniport's MiniportXxx entry points with the NDIS library as 
//		the first step in NIC driver initialization.
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS 
	my_NdisMRegisterMiniport(
		IN NDIS_HANDLE  NdisWrapperHandle,
		IN PNDIS_MINIPORT_CHARACTERISTICS  MiniportCharacteristics,
		IN UINT  CharacteristicsLength
		)
{
	NDIS_STATUS nStatus;
	PKIP_NDIS_MINIPORT pMiniport;
	PNDIS_WRAPPER_HANDLE Wrapper = (PNDIS_WRAPPER_HANDLE)NdisWrapperHandle;

	ENTER_HOOK();

	DBGLOG(( LDebug, "Registering miniport NdisMajor=%u\n", MiniportCharacteristics->MajorNdisVersion ));

	//allocate new miniport structure
	pMiniport = NDIS_AllocateMiniport();

	if ( pMiniport == NULL ){
		DBGLOG(( LCriticalError, "Failed to allocate miniport\n" ));
		LEAVE_HOOK();
		return NDIS_STATUS_FAILURE;
	}

	pMiniport->MajorNdisVersion = MiniportCharacteristics->MajorNdisVersion;
	pMiniport->MinorNdisVersion = MiniportCharacteristics->MinorNdisVersion;
	pMiniport->DriverObject = Wrapper->DriverObject;

	pMiniport->Ndis5.NdisWrapperHandle = NdisWrapperHandle;

	#undef SET_MINIPORT_FIELD
	#define SET_MINIPORT_FIELD(_ADAPTER,_FIELD) \
			(_ADAPTER)->Ndis5.MiniportCharacteristics._FIELD = MiniportCharacteristics->_FIELD

	// save original handlers
	if ( MiniportCharacteristics->MajorNdisVersion >= 3 )
	{
		SET_MINIPORT_FIELD( pMiniport, CheckForHangHandler );
		SET_MINIPORT_FIELD( pMiniport, DisableInterruptHandler );
		SET_MINIPORT_FIELD( pMiniport, EnableInterruptHandler );
		SET_MINIPORT_FIELD( pMiniport, HaltHandler );
		SET_MINIPORT_FIELD( pMiniport, HandleInterruptHandler );
		SET_MINIPORT_FIELD( pMiniport, InitializeHandler );
		SET_MINIPORT_FIELD( pMiniport, ISRHandler );
		SET_MINIPORT_FIELD( pMiniport, QueryInformationHandler );
		SET_MINIPORT_FIELD( pMiniport, ReconfigureHandler );
		SET_MINIPORT_FIELD( pMiniport, ResetHandler );
		SET_MINIPORT_FIELD( pMiniport, SendHandler );
		SET_MINIPORT_FIELD( pMiniport, SetInformationHandler );
		SET_MINIPORT_FIELD( pMiniport, TransferDataHandler );
	}

	if ( MiniportCharacteristics->MajorNdisVersion >= 4 )
	{
		SET_MINIPORT_FIELD( pMiniport, ReturnPacketHandler );
		SET_MINIPORT_FIELD( pMiniport, SendPacketsHandler );
		SET_MINIPORT_FIELD( pMiniport, AllocateCompleteHandler );
	}

	if ( MiniportCharacteristics->MajorNdisVersion >= 5 )
	{
		SET_MINIPORT_FIELD( pMiniport, CoCreateVcHandler );
		SET_MINIPORT_FIELD( pMiniport, CoDeleteVcHandler );
		SET_MINIPORT_FIELD( pMiniport, CoActivateVcHandler );
		SET_MINIPORT_FIELD( pMiniport, CoDeactivateVcHandler );
		SET_MINIPORT_FIELD( pMiniport, CoSendPacketsHandler );
		SET_MINIPORT_FIELD( pMiniport, CoRequestHandler );

		if ( MiniportCharacteristics->MinorNdisVersion >= 1 )
		{
			SET_MINIPORT_FIELD( pMiniport, CancelSendPacketsHandler );
			SET_MINIPORT_FIELD( pMiniport, PnPEventNotifyHandler );
			SET_MINIPORT_FIELD( pMiniport, AdapterShutdownHandler );
		}
	}

	// save miniport handle
	pMiniport->Ndis5.NdisWrapperHandle = NdisWrapperHandle;

	// hook miniport handlers
	if ( MiniportCharacteristics->InitializeHandler )
	{
		MiniportCharacteristics->InitializeHandler = 
			MakeCallStub(
				&pMiniport->Ndis5.STUB_MiniportInitialize,
				&my_MiniportInitialize,
				pMiniport,
				6
				);
	}
	if ( MiniportCharacteristics->HaltHandler ){
		MiniportCharacteristics->HaltHandler = my_MiniportHalt;
	}
	if ( MiniportCharacteristics->ResetHandler ){
		MiniportCharacteristics->ResetHandler = my_MiniportReset;
	}
	if ( MiniportCharacteristics->ReturnPacketHandler ){
		MiniportCharacteristics->ReturnPacketHandler = my_MiniportReturnPacket;
	}
	if ( MiniportCharacteristics->SetInformationHandler ){
		MiniportCharacteristics->SetInformationHandler = my_MiniportSetInformation;
	}
	if ( MiniportCharacteristics->QueryInformationHandler ){
		MiniportCharacteristics->QueryInformationHandler = my_MiniportQueryInformation;
	}
	//call original
	nStatus = 
		NDIS_ORIGINAL(NdisMRegisterMiniport)(
			NdisWrapperHandle,
			MiniportCharacteristics,
			CharacteristicsLength
			);

	// complete initialization
	if ( nStatus != NDIS_STATUS_SUCCESS ){

		DBGLOG(( LInfo, "%08X=NdisMRegisterMiniport failed\n", nStatus ));
		NDIS_FreeMiniport( pMiniport );
	}else{
		// hook add device function
		pMiniport->PnPDispatch = pMiniport->DriverObject->MajorFunction[IRP_MJ_PNP];
		pMiniport->DriverObject->MajorFunction[IRP_MJ_PNP] = my_ndisPnPDispatch;
	}

	LEAVE_HOOK();

	DBGLOG(( LDebug, "New miniport registered \n" ));

	return nStatus;
}
//***********************************************************************************
// Name: my_NdisTerminateWrapper
//
// Description: 
//		NdisTerminateWrapper hook.
//		NdisTerminateWrapper releases system resources allocated when the NIC 
//		driver called NdisMInitializeWrapper.
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisTerminateWrapper(
		IN NDIS_HANDLE  NdisWrapperHandle,
		IN PVOID  SystemSpecific
		)
{
	PKIP_NDIS_MINIPORT pMiniport;

	DBGLOG(( LDebug, "Terminating miniport %p\n", NdisWrapperHandle ));

	ENTER_HOOK();

	//lookup adapter by handle
	pMiniport = NDIS_FindMiniportByWrapperHandle( NdisWrapperHandle );
	if ( pMiniport )
	{
		NDIS_FreeMiniport( pMiniport );
	}

	NDIS_ORIGINAL(NdisTerminateWrapper)( NdisWrapperHandle, SystemSpecific );

	LEAVE_HOOK();
}
//***********************************************************************************
// Name: my_NdisMSetAttributes
//
// Description: 
//		NdisMSetAttributes hook.
//		NdisMSetAttributes informs the NDIS library about significant features of 
//		the caller’s NIC during initialization. 
//
// Return value: 
//		None
//
// Parameters: 
//		MiniportAdapterHandle 
//			Specifies the handle input to MiniportInitialize. 
//		MiniportAdapterContext 
//			Specifies a handle for a resident context area allocated by MiniportInitialize. 
//		BusMaster 
//			Specifies TRUE if the caller’s NIC is a bus-master DMA device. 
//		AdapterType 
//
// NOTE: None
// **********************************************************************************
VOID 
	my_NdisMSetAttributes(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_HANDLE  MiniportAdapterContext,
		IN BOOLEAN  BusMaster,
		IN NDIS_INTERFACE_TYPE  AdapterType
		)
{
	PKIP_NDIS_ADAPTER pAdapter;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter )
	{
		pAdapter->NdisMiniportAdapterContext = MiniportAdapterContext;

		if ( BusMaster )
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_BUS_MASTER);
		}
	}

	NDIS_ORIGINAL(NdisMSetAttributes)( 
		MiniportAdapterHandle, 
		MiniportAdapterContext,
		BusMaster,
		AdapterType
		);

	LEAVE_HOOK();
}

//***********************************************************************************
// Name: my_NdisMSetAttributesEx
//
// Description: 
//		NdisMSetAttributesEx hook.
//		NdisMSetAttributesEx informs the NDIS library about significant features of 
//		the caller’s NIC during initialization. 
//
// Return value: 
//		None
//
// Parameters: 
//		MiniportAdapterHandle 
//			Specifies the handle input to MiniportInitialize. 
//		MiniportAdapterContext 
//			Specifies a handle for a resident context area allocated by MiniportInitialize. 
//		CheckForHangTimeInSeconds 
//		AttributeFlags  
//		AdapterType 
//
// NOTE: None
// **********************************************************************************
VOID 
	my_NdisMSetAttributesEx(
		IN NDIS_HANDLE MiniportAdapterHandle,
		IN NDIS_HANDLE MiniportAdapterContext,
		IN UINT  CheckForHangTimeInSeconds  OPTIONAL,
		IN ULONG  AttributeFlags,
		IN NDIS_INTERFACE_TYPE AdapterType
		)
{
	PKIP_NDIS_ADAPTER pAdapter;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter )
	{
		pAdapter->NdisMiniportAdapterContext = MiniportAdapterContext;

		if (AttributeFlags & NDIS_ATTRIBUTE_BUS_MASTER)
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_BUS_MASTER);
		}
		if (AttributeFlags & NDIS_ATTRIBUTE_IGNORE_PACKET_TIMEOUT)
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_IGNORE_PACKET_QUEUE);
		}
		if (AttributeFlags & NDIS_ATTRIBUTE_IGNORE_REQUEST_TIMEOUT)
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_IGNORE_REQUEST_QUEUE);
		}
		if (AttributeFlags & NDIS_ATTRIBUTE_IGNORE_TOKEN_RING_ERRORS)
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_IGNORE_TOKEN_RING_ERRORS);
		}
		if (AttributeFlags & NDIS_ATTRIBUTE_INTERMEDIATE_DRIVER)
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_INTERMEDIATE_DRIVER);
		}

		if (((AttributeFlags & NDIS_ATTRIBUTE_DESERIALIZE) ||
			KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_IS_CO)))
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_DESERIALIZE);
		}

	}

	NDIS_ORIGINAL(NdisMSetAttributesEx)( 
		MiniportAdapterHandle, 
		MiniportAdapterContext,
		CheckForHangTimeInSeconds,
		AttributeFlags,
		AdapterType
		);

	LEAVE_HOOK();
}

//***********************************************************************************
// Name: my_NdisMSendComplete 
//
// Description: 
//		NdisMSendComplete  hook.
//		NdisMSendComplete returns the packet and final status of a completed send 
//		request for which the driver previously returned NDIS_STATUS_PENDING.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMSendComplete (
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_PACKET  Packet,
		IN NDIS_STATUS  Status
		)
{
	PKIP_NDIS_ADAPTER pAdapter;
	BOOLEAN fbResult = FALSE;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		// process packet by our protocol
		fbResult =
			MpSendPacketCompleteHandler(
				pAdapter,
				Packet,
				Status
				);
		// true, if we own this packet
	}
	if ( fbResult == FALSE ){
		NDIS_ORIGINAL(NdisMSendComplete)( MiniportAdapterHandle, Packet, Status );
	}

	LEAVE_HOOK();
}
//***********************************************************************************
// Name: my_NdisMTransferDataComplete 
//
// Description: 
//		NdisMTransferDataComplete  hook.
//		NdisMTransferDataComplete returns the packet and completion status for 
//		a transfer-data request for which the miniport driver previously returned 
//		NDIS_STATUS_PENDING.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMTransferDataComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN PNDIS_PACKET  Packet,
		IN NDIS_STATUS  Status,
		IN UINT  BytesTransferred
		)
{
	PKIP_NDIS_ADAPTER pAdapter;
	BOOLEAN fbCallOrig = TRUE;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		MpTransferDataComplete(
			pAdapter,
			Packet,
			Status,
			BytesTransferred,
			&fbCallOrig
			);
	}

	if ( fbCallOrig ){
		NDIS_ORIGINAL(NdisMTransferDataComplete)( 
			MiniportAdapterHandle, 
			Packet, 
			Status,
			BytesTransferred
			);
	}

	LEAVE_HOOK();
}
//***********************************************************************************
// Name: my_NdisMIndicateStatus 
//
// Description: 
//		NdisMIndicateStatus hook.
//		indicates changes in the status of a NIC to higher-level NDIS drivers.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMIndicateStatus(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  GeneralStatus,
		IN PVOID  StatusBuffer,
		IN UINT  StatusBufferSize
		)
{
	PKIP_NDIS_ADAPTER pAdapter;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );

	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		MpIndicateStatusHandler(
			pAdapter,
			GeneralStatus,
			StatusBuffer,
			StatusBufferSize,
			TRUE
			);
	}

	NDIS_ORIGINAL(NdisMIndicateStatus)( 
		MiniportAdapterHandle,
		GeneralStatus,
		StatusBuffer,
		StatusBufferSize
		);

	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		MpIndicateStatusHandler(
			pAdapter,
			GeneralStatus,
			StatusBuffer,
			StatusBufferSize,
			FALSE //after
			);
	}

	LEAVE_HOOK();
}
//***********************************************************************************
// Name: my_NdisMQueryInformationComplete
//
// Description: 
//		NdisMQueryInformationComplete hook.
//		NdisMQueryInformationComplete indicates that a prior call to MiniportQueryInformation, 
//		which returned NDIS_STATUS_PENDING, has completed.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMQueryInformationComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		)
{

	PNDIS_MINIPORT_BLOCK	pNdisMiniport = (PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle;
	PKIP_NDIS_ADAPTER		pAdapter;
	BOOLEAN fbCallOriginal = TRUE;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		// call miniport
		MpQueryInformationCompleteHandler(
			pAdapter,
			&pAdapter->Interface.QueryInformationRequest,
			Status,
			&fbCallOriginal
			);
	}

	if ( fbCallOriginal ){
		NDIS_ORIGINAL(NdisMQueryInformationComplete)( 
			MiniportAdapterHandle, 
			Status
			);
	}

	LEAVE_HOOK();
}
//***********************************************************************************
// Name: my_NdisMSetInformationComplete
//
// Description: 
//		NdisMSetInformationComplete hook.
//		NdisMSetInformationComplete indicates that a prior call to MiniportSetInformation, 
//		which returned NDIS_STATUS_PENDING, has completed.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	my_NdisMSetInformationComplete(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN NDIS_STATUS  Status
		)
{
	PKIP_NDIS_ADAPTER pAdapter;
	BOOLEAN fbCallOriginal = TRUE;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	if ( pAdapter && KIP_MINIPORT_IS_ACTIVE(pAdapter))
	{
		// call miniport
		MpSetInformationCompleteHandler(
			pAdapter,
			&pAdapter->Interface.SetInformationRequest,
			Status,
			&fbCallOriginal
			);
	}

	if ( fbCallOriginal ){
		NDIS_ORIGINAL(NdisMSetInformationComplete)( 
			MiniportAdapterHandle, 
			Status
			);
	}

	LEAVE_HOOK();
}
//***********************************************************************************
// Name: my_NdisMInitializeScatterGatherDma
//
// Description: 
//		NdisMInitializeScatterGatherDma hook.
//		Ndis calls this function to indicate that it uses SGDMA for IO
//		Such adapters uses special path for packets sending.
//
// Return value: 
//		Operation status.
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	my_NdisMInitializeScatterGatherDma(
		IN NDIS_HANDLE  MiniportAdapterHandle,
		IN BOOLEAN  Dma64BitAddresses,
		IN ULONG  MaximumPhysicalMapping
		)
{
	NDIS_STATUS nStatus;
	PKIP_NDIS_ADAPTER pAdapter;

	ENTER_HOOK();

	//lookup adapter by handle
	pAdapter = NDIS_FindAdapterByHandle( MiniportAdapterHandle );
	
	nStatus =
		NDIS_ORIGINAL(NdisMInitializeScatterGatherDma)( 
			MiniportAdapterHandle, 
			Dma64BitAddresses,
			MaximumPhysicalMapping
			);

	if ( pAdapter )
	{
		if ( nStatus == NDIS_STATUS_SUCCESS ){
			KIP_MINIPORT_SET_FLAG(pAdapter,fADAPTER_SG_LIST);
		}
	}

	LEAVE_HOOK();
	return nStatus;
}