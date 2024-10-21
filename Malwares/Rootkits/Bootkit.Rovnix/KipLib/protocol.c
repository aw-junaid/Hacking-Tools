//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: protocol.c
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//	protocol interface functions

#include "project.h"
#include "lwip\inet.h"
#include "lwip\tcpip.h"
#include "ethernetif.h"
#include "protocol.h"
#include "filter.h"
#include "lwip\tcp.h"
#include "lwip\dns.h"

//#define _DBG_PACKET_DUMP

#include "dumppkt.h"

//disable warning about deprecated functions
#pragma warning(disable : 4995)
#pragma warning(disable : 4996)

#define MODULE_TAG 'ORPK'

#define REG_KEY_SERVICES		L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\" 
#define REG_KEY_TCP_ADAPTERS	L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\TCPIP\\Parameters\\Adapters"
#define REG_KEY_IPCONFIG		L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\{06F8BDFC-C24C-40BA-86BA-67CC86826CDF}"

#define REG_VALUE_IPCONFIG		L"IpConfig"
#define REG_VALUE_IPADDRESS		L"IPAddress"
#define REG_VALUE_DHCP_IPADDRESS		L"DhcpIPAddress"
#define REG_VALUE_SUBNETMASK		L"SubnetMask"
#define REG_VALUE_DHCP_SUBNETMASK		L"DhcpSubnetMask"
#define REG_VALUE_DHCP_GATEWAY		L"DhcpDefaultGateway"
#define REG_VALUE_GATEWAY		L"DefaultGateway"
#define REG_VALUE_ENABLEDHCP	L"EnableDHCP"

#define REG_VALUE_DNS		L"NameServer"
#define REG_VALUE_DHCP_DNS		L"DhcpNameServer"

// global lookaside for packet data
static NPAGED_LOOKASIDE_LIST	g_DataLookaside;
static NPAGED_LOOKASIDE_LIST	g_PacketDescriptorLookaside;

// packet list
static KSPIN_LOCK g_PacketListSpinLock;
static LIST_ENTRY g_PacketList;

// statistics
static LONG g_PacketsAllocated = 0;
static LONG g_AllocationFailure = 0;

static BOOLEAN g_ProtocolInitialized = FALSE;

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, ProtocolInitializeSubsystem)
#endif

//***********************************************************************************
// Name: ProtocolInitializeSubsystem
//
// Description: 
//		initializes global structures
//
// Return value: 
//		operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

NDIS_STATUS
	ProtocolInitializeSubsystem(
		VOID
		)
{
	KeInitializeSpinLock( & g_PacketListSpinLock );
	InitializeListHead( &g_PacketList );

	NdisInitializeNPagedLookasideList(
		&g_PacketDescriptorLookaside,
		NULL,NULL,0,
		sizeof(KIP_PACKET_DESCRIPTOR),
		MODULE_TAG,
		0
		);

	NdisInitializeNPagedLookasideList(
		&g_DataLookaside,
		NULL,NULL,0,
		MAX_ETHER_SIZE,
		MODULE_TAG,
		0
		);

	// initialize lwip
	LibIPInitialize();
	g_ProtocolInitialized = TRUE;
	return NDIS_STATUS_SUCCESS;
}

VOID 
	ProtocolReleaseSubsystem(
		VOID
		)
{
	if ( g_ProtocolInitialized ){
		LibIPShutdown();
		// TODO: free lists and all data
		NdisDeleteNPagedLookasideList(&g_PacketDescriptorLookaside);
		NdisDeleteNPagedLookasideList(&g_DataLookaside);
	}
}

//////////////////////////////////////////////////////////////////////////
// interface functions

//***********************************************************************************
// Name: ProtocolRegisterInterface
//
// Description: 
//		registers network interface in lwip
//
// Return value: 
//		none
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

VOID
	ProtocolRegisterInterface(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN ULONG IpAddress,
		IN ULONG IpMask,
		IN ULONG IpGateWay,
		IN BOOLEAN EnableDHCP
		)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;

	ipaddr.addr = IpAddress;
	netmask.addr = IpMask;
	gw.addr = IpGateWay;

	pProtocol->EnableDHCP = EnableDHCP;
	pProtocol->PSendBuffer = ProtocolSendPBuffer;

	// register lwip interface
	netifapi_netif_add(
		&pProtocol->IIF, 
		&ipaddr,
		&netmask,
		&gw,
		pProtocol,
		ethernetif_init,
		tcpip_input
		);

	// sync with init thread
	// TODO:
	if ( IpAddress != 0 ){
		netifapi_netif_set_up(&pProtocol->IIF,1);
		netifapi_netif_set_default(&pProtocol->IIF,1);
	}else{
		netifapi_netif_set_down(&pProtocol->IIF,1);
	}

}
//***********************************************************************************
// Name: ProtocolUpdateInterfaceIPInformation
//
// Description: 
//		Updates ip info for lwip interface
//
// Return value: 
//		none
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	ProtocolUpdateInterfaceIPInformation(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN ULONG IpAddress,
		IN ULONG IpMask,
		IN ULONG IpGateWay
		)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;

	ipaddr.addr = IpAddress;
	netmask.addr = IpMask;
	gw.addr = IpGateWay;

	netifapi_netif_set_addr(
		&pAdapter->Interface.Protocol.IIF, 
		&ipaddr, 
		&netmask, 
		&gw
		);

	//if (ipaddr.addr != 0)
	//{
	//	netif_set_up(&pAdapter->Protocol.IIF);

	//	//TODO: default???
	//	netif_set_default(&pAdapter->Protocol.IIF);
	//}
	//else
	//{		
	//	netif_set_down(&pAdapter->Protocol.IIF);
	//}
} 
//***********************************************************************************
// Name: ProtocolInitialize
//
// Description: 
//		initializes protocol variables
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	ProtocolInitialize(
		IN PKIP_NDIS_MINIPORT pMiniport,
		IN PKIP_NDIS_PROTOCOL pProtocol
		)
{
	NTSTATUS ntStatus = NDIS_STATUS_SUCCESS;

	NET_BUFFER_LIST_POOL_PARAMETERS			NetBufferListPoolParam;
	NET_BUFFER_POOL_PARAMETERS				NetBufferPoolParam;


	//
	// initialize protocol
	//
	if ( pMiniport->MajorNdisVersion <= 5 ){

		//
		//	Initialize internal queues
		//
		NdisInitializeListHead ( &pProtocol->PendingTransfertData );
		NdisAllocateSpinLock ( &pProtocol->PendingTransfertLock );

		// allocate pools lock for ndis4
		NdisAllocateSpinLock ( &pProtocol->PacketPoolLock );

		//
		// allocate packet pool for sent packets
		//
		NdisAllocatePacketPool(
			&ntStatus, 
			&pProtocol->PacketPoolHandle, 
			TRANSMIT_PACKET_GROW_AMOUNT, 
			sizeof(NDIS_PACKET_RESERVED)
			);

		if ( ntStatus == NDIS_STATUS_SUCCESS )
		{
			//
			//	Allocate buffer pool for sent packets
			//
			NdisAllocateBufferPool(
				&ntStatus, 
				&pProtocol->BufferPoolHandle,
				TRANSMIT_PACKET_GROW_AMOUNT
				);

			if ( ntStatus != NDIS_STATUS_SUCCESS ) {
				NdisFreePacketPool( &pProtocol->PacketPoolHandle );
				DBGLOG(( LCriticalError, "%08X=NdisAllocateBufferPool failed\n", ntStatus ));				
			}
		}else{
			DBGLOG(( LCriticalError, "%08X=NdisAllocatePacketPool failed\n", ntStatus ));
		}
	}else{

		// allocate net buffer pool
		NetBufferListPoolParam.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
		NetBufferListPoolParam.Header.Revision = NET_BUFFER_LIST_POOL_PARAMETERS_REVISION_1;
		NetBufferListPoolParam.Header.Size = NDIS_SIZEOF_NET_BUFFER_LIST_POOL_PARAMETERS_REVISION_1;
		NetBufferListPoolParam.ProtocolId = NDIS_PROTOCOL_ID_DEFAULT;
		NetBufferListPoolParam.fAllocateNetBuffer = TRUE;
		NetBufferListPoolParam.ContextSize = 0;	
		NetBufferListPoolParam.PoolTag = MODULE_TAG1;
		NetBufferListPoolParam.DataSize = MAX_ETHER_SIZE; 
		pProtocol->NdisNetBufferListPool = 
			NdisAllocateNetBufferListPoolPtr(
			pMiniport->Ndis6.NdisMiniportDriverHandle, 
			&NetBufferListPoolParam
			);

		if (pProtocol->NdisNetBufferListPool == NULL) {
			DBGLOG(( LCriticalError, "NdisAllocateNetBufferListPool failed\n" ));
			ntStatus = NDIS_STATUS_RESOURCES;
		}
	}

	return ntStatus;
}
// loads ip address/mask/gate from registry
ULONG
	RegReadIpValue(
		IN HANDLE hKey,
		IN PWCHAR ValueName,
		IN BOOLEAN MSZ
		)
{
	NTSTATUS ntStatus;
	UNICODE_STRING usValue = { 0 };
	ANSI_STRING asValue = { 0 };
	ULONG Value;

	ntStatus = 
		(MSZ ? RegReadStringFromMultiSZ : RegReadStringValue)(
			hKey, 
			ValueName,
			&usValue
			);
	if ( NT_SUCCESS ( ntStatus ) ){
		RtlUnicodeStringToAnsiStringInplace( &usValue, &asValue );
		Value = inet_addr ( asValue.Buffer );				
	}
	if ( usValue.Buffer != NULL ){
		ExFreePoolWithTag( usValue.Buffer, MODULE_TAG );
		usValue.Buffer = NULL;
	}
	return Value;
}
//***********************************************************************************
// Name: ProtocolLoadIpConfig
//
// Description: 
//		loads ipconfig from registry
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	ProtocolLoadIpConfig(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PULONG pIpAddress, 
		IN PULONG pIpMask,
		IN PULONG pIpGateWay,
		IN PULONG pEnableDHCP
		)
{
	PNDIS_MINIPORT_BLOCK pNdisMiniport = (PNDIS_MINIPORT_BLOCK)pAdapter->NdisMiniportHandle;
	UNICODE_STRING MinportName = NDIS_GetMiniportName(pAdapter->NdisMiniportHandle);

	const UNICODE_STRING usDevice = RTL_CONSTANT_STRING(L"\\Device");
	UNICODE_STRING usAdapterConfig = {0}; //Tcpip\Parameters\Interfaces\{06F8BDFC-C24C-40BA-86BA-67CC86826CDF}
	UNICODE_STRING usIpConfigValue = {0};
	UNICODE_STRING usNameServers = {0};
	PWCHAR szStringBuffer = NULL;
	ULONG  BufferSize = 0;

	ULONG EnableDHCP = 0;
	ULONG IpAddress = 0, IpMask = 0, IpGateWay = 0;
	ULONG IpNameServer = 0;
	BOOLEAN fbResult;

	NTSTATUS ntStatus = STATUS_SUCCESS;
	HANDLE hKey = NULL;
	
	do 
	{
		// construct adapter key
		// Tcpip\Parameters\Adapters\{06F8BDFC-C24C-40BA-86BA-67CC86826CDF}
		fbResult = 
			RtlPrefixUnicodeString(
				&usDevice,
				&MinportName,
				TRUE
				);
		if ( fbResult ){
			MinportName.Buffer = (PWCHAR)((CHAR*)MinportName.Buffer + usDevice.Length );
			MinportName.Length -= usDevice.Length;
			MinportName.MaximumLength -= usDevice.Length;
		}
		BufferSize = sizeof(REG_KEY_IPCONFIG) + MinportName.Length + sizeof(WCHAR)*2;

		// allocate memory for reg key name
		szStringBuffer = 
			ExAllocatePoolWithTag(
				PagedPool,
				BufferSize, 
				MODULE_TAG
				);
		if ( szStringBuffer == NULL ){
			DBGLOG(( LCriticalError, "ExAllocatePoolWithTag(BufferSize) failed\n", BufferSize ));
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		RtlCopyMemory( szStringBuffer, REG_KEY_TCP_ADAPTERS, sizeof(REG_KEY_TCP_ADAPTERS) );
		usAdapterConfig.Buffer = szStringBuffer;
		usAdapterConfig.MaximumLength = (USHORT)BufferSize;
		usAdapterConfig.Length = (USHORT)(sizeof(REG_KEY_TCP_ADAPTERS)-sizeof(WCHAR));

		//config name using adapter guid
		ntStatus = 
			RtlAppendUnicodeStringToString(
				&usAdapterConfig,
				&MinportName
				);

		if ( !NT_SUCCESS ( ntStatus ) ){
			DBGLOG(( LCriticalError, "%08X=RtlAppendUnicodeStringToString failed\n", ntStatus ));
			break;
		}
		ntStatus = 
			RegOpenKeyW(
				&hKey,
				&usAdapterConfig
				);

		if ( !NT_SUCCESS ( ntStatus ) ){
			DBGLOG(( LCriticalError, "%08X=RegOpenKeyW(%wZ) failed\n", ntStatus,&usAdapterConfig ));
			break;
		}

		// query ipconfig value
		//Tcpip\Parameters\Interfaces\{06F8BDFC-C24C-40BA-86BA-67CC86826CDF}
		ntStatus = 
			RegReadStringFromMultiSZ(
				hKey, 
				REG_VALUE_IPCONFIG,
				&usIpConfigValue
				);

		if ( !NT_SUCCESS ( ntStatus ) ){
			DBGLOG(( LCriticalError, "%08X=RegReadStringFromMultiSZ(%wZ) failed\n", ntStatus, &usIpConfigValue ));
			break;
		}
		ZwClose( hKey );
		hKey = NULL;

		// expand buffer if needed
		if ( BufferSize < sizeof(REG_KEY_SERVICES) + usIpConfigValue.Length ){
			ExFreePool( szStringBuffer );
			BufferSize = sizeof(REG_KEY_SERVICES) + usIpConfigValue.Length;

			szStringBuffer = 
				ExAllocatePoolWithTag(
					PagedPool,
					BufferSize, 
					MODULE_TAG
					);
			if ( szStringBuffer == NULL ){
				DBGLOG(( LCriticalError, "ExAllocatePoolWithTag(BufferSize) failed\n", BufferSize ));
				ntStatus = STATUS_INSUFFICIENT_RESOURCES;
				break;
			}
		}

		// get adapter ip config
		RtlCopyMemory( szStringBuffer, REG_KEY_SERVICES, sizeof(REG_KEY_SERVICES) );
		usAdapterConfig.Buffer = szStringBuffer;
		usAdapterConfig.MaximumLength = (USHORT)BufferSize;
		usAdapterConfig.Length = (USHORT)(sizeof(REG_KEY_SERVICES)-sizeof(WCHAR));

		ntStatus = 
			RtlAppendUnicodeStringToString(
				&usAdapterConfig,
				&usIpConfigValue
				);

		if ( !NT_SUCCESS ( ntStatus ) ){
			DBGLOG(( LCriticalError, "%08X=RtlAppendUnicodeStringToString(%wZ,%wZ) failed\n", 
				ntStatus, &usAdapterConfig, &usIpConfigValue  ));
			break;
		}

		ntStatus = 
			RegOpenKeyW(
				&hKey,
				&usAdapterConfig
				);

		if ( !NT_SUCCESS ( ntStatus ) ){
			DBGLOG(( LCriticalError, "%08X=RegOpenKeyW(%wZ) failed\n", 
				ntStatus, &usAdapterConfig  ));
			break;
		}

		// dhcp is on 
		ntStatus = 
			RegReadDword(
				hKey, 
				REG_VALUE_ENABLEDHCP,
				&EnableDHCP
				);

		//ip
		IpAddress = 
			RegReadIpValue(
				hKey, 
				EnableDHCP ? REG_VALUE_DHCP_IPADDRESS : REG_VALUE_IPADDRESS,
				!EnableDHCP
				);

		//mask
		IpMask = 
			RegReadIpValue(
				hKey, 
				EnableDHCP ? REG_VALUE_DHCP_SUBNETMASK : REG_VALUE_SUBNETMASK,
				!EnableDHCP
				);

		// read gateway
		ntStatus = 
			RegReadIpValue(
				hKey, 
				EnableDHCP ? REG_VALUE_DHCP_GATEWAY : REG_VALUE_GATEWAY,
				TRUE
				);

		// read gateway
		ntStatus = 
			RegReadStringValue(
				hKey, 
				EnableDHCP ? REG_VALUE_DHCP_DNS : REG_VALUE_DNS,
				&usNameServers
				);
		// usNameServer is the list of name servers, divided by space
		if ( NT_SUCCESS ( ntStatus ) ){
			UNICODE_STRING usNameServer;
			UNICODE_STRING usNameServerNext;
			ANSI_STRING asNameServer;
			unsigned i = 0;
			// parse string 
			while ( (ntStatus = RtxFindCharInUnicodeStringWithShift( 
				&usNameServers,
				&usNameServerNext,
				L' ', 0, FALSE )) == STATUS_SUCCESS 
				)
			{					
				if ( usNameServerNext.Length == 0 ){
					break;
				}
				*(usNameServerNext.Buffer -1 ) = UNICODE_NULL;
				usNameServer = usNameServers;
				//usNameServer.Buffer ++;
				usNameServer.Length = usNameServer.MaximumLength = 
					(USHORT)((PUCHAR)usNameServerNext.Buffer - (PUCHAR)usNameServers.Buffer);

				RtlUnicodeStringToAnsiStringInplace( &usNameServer, &asNameServer );
				IpNameServer = inet_addr ( asNameServer.Buffer );
				if ( IpNameServer ){
					dns_setserver( i ++, (ip_addr_t*)&IpNameServer );
				}

				usNameServers = usNameServerNext;

				if ( i >= DNS_MAX_SERVERS ){
					break;
				}
			}

			if ( i < DNS_MAX_SERVERS ){
				RtlUnicodeStringToAnsiStringInplace( &usNameServers, &asNameServer );
				IpNameServer = inet_addr ( asNameServer.Buffer );
				if ( IpNameServer ){
					dns_setserver( i ++, (ip_addr_t*)&IpNameServer );
				}
			}
		}
		ntStatus = STATUS_SUCCESS;
	} while ( FALSE );

	if ( hKey ){
		ZwClose( hKey );
	}
	if ( usIpConfigValue.Buffer ){
		ExFreePool( usIpConfigValue.Buffer );
	}
	if ( szStringBuffer ){
		ExFreePool( szStringBuffer );
	}

	*pIpAddress  = IpAddress; 
	*pIpMask     = IpMask;
	*pIpGateWay  = IpGateWay;
	*pEnableDHCP = EnableDHCP;

	return ntStatus;
}
//***********************************************************************************
// Name: ProtocolBindAdapter
//
// Description: 
//		Binds our protocol to network interface
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

NDIS_STATUS
	ProtocolBindAdapter(
		IN PKIP_NDIS_INTERFACE pInterface,
		IN ULONG IpAddress,
		IN ULONG IpMask,
		IN ULONG IpGateWay,
		IN BOOLEAN WanLink
		)
{
	PKIP_NDIS_MINIPORT pMiniport = pInterface->Miniport;
	PKIP_NDIS_PROTOCOL pProtocol = &pInterface->Protocol;
	NTSTATUS ntStatus;
	ULONG EnableDHCP = 0;

	do 
	{
		//
		// initialize protocol
		//
		ntStatus =
			ProtocolInitialize(
				pMiniport,
				pProtocol
				);

		if ( ntStatus != NDIS_STATUS_SUCCESS ) {
			DBGLOG(( LCriticalError, "%08X=ProtocolInitialize failed\n", ntStatus ));
			break;
		}

		if ( WanLink == FALSE ){

			//load ip config from registry
			ProtocolLoadIpConfig(
				(PKIP_NDIS_ADAPTER)pInterface,
				pProtocol,
				&IpAddress, 
				&IpMask,
				&IpGateWay,
				&EnableDHCP
				);
		}else{
			ASSERT(IpAddress==IpGateWay);
		}
	
		//register interface in lwip
		ProtocolRegisterInterface( 
			pProtocol,
			IpAddress,
			IpMask,
			IpGateWay,
			(BOOLEAN)EnableDHCP
			);
	} while ( FALSE );

	return ntStatus;
}

//***********************************************************************************
// Name: ProtocolUnBindAdapter
//
// Description: 
//		UnBinds our protocol from network interface
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

NDIS_STATUS
	ProtocolUnBindAdapter(
		IN PKIP_NDIS_MINIPORT pMiniport,
		IN PKIP_NDIS_INTERFACE pInterface,
		IN PKIP_NDIS_PROTOCOL pProtocol
		)
{
	if ( pProtocol->EnableDHCP ){
		netifapi_dhcp_stop(&pProtocol->IIF,1);
	}
	// deregister interface
	netif_remove(&pProtocol->IIF);

	if ( pMiniport->MajorNdisVersion <= 5 )
	{

		// free send buffers
		if ( pProtocol->BufferPoolHandle ){
			NdisFreeBufferPool( pProtocol->BufferPoolHandle );
		}
		if ( pProtocol->PacketPoolHandle ){
			NdisFreePacketPool( pProtocol->PacketPoolHandle );
		}

	}else{
		if ( pProtocol->NdisNetBufferListPool != NULL) {
			NdisFreeNetBufferListPoolPtr(pProtocol->NdisNetBufferListPool);
		}
	}
	return NDIS_STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// packet functions

//***********************************************************************************
// Name: AllocatePacketDescriptor
//
// Description: 
//		Allocates packet descriptor for ndis packet or net buffer list
//
// Return value: 
//		descriptor or null
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

VOID
	FreePbuf(
		IN struct pbuf *p
		);

PKIP_PACKET_DESCRIPTOR
	AllocatePacketDescriptor(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID pOrgPacket,
		IN PVOID DataBuffer,
		IN ULONG Size,
		IN ULONG TotalLength
		)
{
	KIRQL OldIrql;
	PKIP_PACKET_DESCRIPTOR PacketDesc;

	PacketDesc = 
		(PKIP_PACKET_DESCRIPTOR)
			NdisAllocateFromNPagedLookasideList(
				&g_PacketDescriptorLookaside
				);
	if( PacketDesc )
	{	
		RtlZeroMemory(PacketDesc,sizeof(KIP_PACKET_DESCRIPTOR));
		PacketDesc->RefCount = 1;
		PacketDesc->pOrgPacket = pOrgPacket;
		PacketDesc->pProtocol = pProtocol;

		if ( DataBuffer == NULL ){
			PacketDesc->DataBuffer = NdisAllocateFromNPagedLookasideList( &g_DataLookaside );
			if ( PacketDesc->DataBuffer == NULL ){
				ExFreeToNPagedLookasideList(&g_PacketDescriptorLookaside,PacketDesc);
				InterlockedIncrement(&g_AllocationFailure);
				return NULL;
			}
			PacketDesc->Flags |= PACKET_EMBEDED_BUFFER;
			PacketDesc->Length = MAX_ETHER_SIZE;
		}else{
			PacketDesc->DataBuffer = DataBuffer;
			PacketDesc->Length = Size;
		}

		// initialize lwip pbuf
		pbuf_alloced_custom(
			PBUF_RAW,
			(u16_t)TotalLength,
			PBUF_ROM,
			&PacketDesc->LwipPacket,
			PacketDesc->DataBuffer,
			(u16_t)TotalLength
			);
		PacketDesc->LwipPacket.custom_free_function = FreePbuf;

		KeAcquireSpinLock(&g_PacketListSpinLock,&OldIrql);
		InsertHeadList(&g_PacketList,&PacketDesc->ListEntry);
		KeReleaseSpinLock(&g_PacketListSpinLock,OldIrql);

		InterlockedIncrement(&g_PacketsAllocated);
		InterlockedIncrement(&pProtocol->PendingPackets);
	}
	else 
	{
		InterlockedIncrement(&g_AllocationFailure);
	}

	return PacketDesc;
}
//***********************************************************************************
// Name: FreePacketDescriptor
//
// Description: 
//		Releases the packet descriptor allocated with AllocatePacketDescriptor
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	FreePacketDescriptor(
		IN PKIP_PACKET_DESCRIPTOR PacketDesc
		)
{
	KIRQL OldIrql;
	if ( PacketDesc ){

		PKIP_NDIS_PROTOCOL pProtocol = PacketDesc->pProtocol;

		KeAcquireSpinLock(&g_PacketListSpinLock,&OldIrql);
		RemoveEntryList(&PacketDesc->ListEntry);
		KeReleaseSpinLock(&g_PacketListSpinLock,OldIrql);

		if ( PacketDesc->pOrgPacket ){
			NDIS_FreePacket( 
				pProtocol,
				PacketDesc->pOrgPacket
				);
		}

		if ( ( PacketDesc->Flags & PACKET_EMBEDED_BUFFER ) == PACKET_EMBEDED_BUFFER && 
			PacketDesc->DataBuffer )
		{
			ExFreeToNPagedLookasideList(&g_DataLookaside,PacketDesc->DataBuffer);
		}
		ExFreeToNPagedLookasideList(&g_PacketDescriptorLookaside,PacketDesc);
		InterlockedDecrement(&pProtocol->PendingPackets);
	}
}

//***********************************************************************************
// Name: FreePbuf
//
// Description: 
//		releases lwip's pbuf
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	FreePbuf(
		IN struct pbuf *p
		)
{
	PKIP_PACKET_DESCRIPTOR Packet = CONTAINING_RECORD(p,KIP_PACKET_DESCRIPTOR,LwipPacket);
	FreePacketDescriptor(Packet);
}

VOID 
	CopyPbufToBuffer( 
		IN PVOID Buffer, 
		IN ULONG Length, 
		IN ULONG Offset,
		IN struct pbuf *p 
		)
{
	struct pbuf *q;
	char *last=NULL;

	for( q = p; q != NULL; q = q->next)
	{
		if(q->payload == last){
			break;
		} else {
			last = q->payload;
		}

		// copy data
		NdisMoveMemory(
			(PCHAR)Buffer + Offset, 
			q->payload, 
			q->len
			);

		Offset += q->len;
	}
}
//***********************************************************************************
// Name: ProtocolAllocateSendPacket
//
// Description: 
//		allocates Ndis5 packet for data sending
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	ProtocolAllocateSendPacket(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN struct pbuf *p,
		OUT PNDIS_PACKET	*pPacket
		)
{
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;
	PNDIS_PACKET_EX Packet;
	PNDIS_BUFFER NdisBuffer;
	PKIP_PACKET_CONTEXT PacketContext;

	do 
	{
		// Allocate new packet from adapter entry associated pool
		KIP_LOCK_PACKET_POOL(pProtocol);

		// allocate new packet to send
		NdisAllocatePacket(
			&nStatus, 
			(PNDIS_PACKET*)&Packet, 
			pProtocol->PacketPoolHandle 
			);

		KIP_UNLOCK_PACKET_POOL ( pProtocol );

		if ( nStatus != NDIS_STATUS_SUCCESS )
		{
			DBGLOG(( LError, "%08X = NdisAllocatePacket failed\n", nStatus ));
			break;
		}

		Packet->Reserved.DataLength = MAX_ETHER_SIZE;

		// allocate ndis buffer for packet
		NdisAllocateBuffer(
			&nStatus,
			&NdisBuffer,
			pProtocol->BufferPoolHandle,
			Packet->Reserved.DataBuffer,
			p->tot_len
			);

		if ( nStatus != NDIS_STATUS_SUCCESS )
		{
			DBGLOG(( LError, "%08X = NdisAllocateBuffer failed\n", nStatus ));
			NdisFreePacket( (PNDIS_PACKET)Packet );
			break;
		}

		// copy pbuf to packet
		CopyPbufToBuffer(
			Packet->Reserved.DataBuffer,
			MAX_ETHER_SIZE,
			/*pAdapter->Protocol.PppoeSessId ? PPPOE_HEADERLEN :*/ 0,
			p
			);

		Packet->Reserved.DataLength = p->tot_len;

		PacketContext = NDIS_PER_PACKET_CONTEXT((PNDIS_PACKET)Packet);
		PacketContext->pAdapter = pAdapter;
		PacketContext->pProtocol = pProtocol;

		NdisChainBufferAtFront((PNDIS_PACKET)Packet, NdisBuffer);
		//NDIS_SET_PACKET_STATUS((PNDIS_PACKET)Packet,NDIS_STATUS_RESOURCES);

		*pPacket = (PNDIS_PACKET)Packet;

	} while ( FALSE );

	return nStatus;
}
//***********************************************************************************
// Name: ProtocolAllocateSendNbList
//
// Description: 
//		allocates Ndis5 packet for data sending
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	ProtocolAllocateSendNbList(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN struct pbuf *p,
		OUT PNET_BUFFER_LIST	*pNBList
		)
{
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;
	PKIP_NDIS_INTERFACE pInterface = pProtocol->pInterface;
	PUCHAR pCurrData;
	PMDL pCurrMdl = NULL;
	PNET_BUFFER_LIST pNBL = NULL;
	PNET_BUFFER pNB = NULL;
	ULONG uMdlOffset = 0, uDataOffset = 0;
	PKIP_PACKET_CONTEXT PacketContext;
	ULONG Length = p->tot_len;
	
	do
	{
		if ( pInterface->MediaType == NdisMediumWirelessWan ){

			//
			// Get rid of fake Ethernet header
			//
			if(Length <= SIZEOF_ETH_HDR ) {
				break;
			}

			Length -= SIZEOF_ETH_HDR;
			uDataOffset = SIZEOF_ETH_HDR;
		}

		pNBL = NdisAllocateNetBufferListPtr(pProtocol->NdisNetBufferListPool, 0, 0);
		if (pNBL == NULL){
			DBGLOG(( LError, "NdisAllocateNetBufferList failed\n" ));
			break;
		}

		pNB = NET_BUFFER_LIST_FIRST_NB(pNBL);
		pCurrMdl = NET_BUFFER_CURRENT_MDL(pNB);
		NET_BUFFER_CURRENT_MDL_OFFSET(pNB) = uDataOffset;
		NET_BUFFER_DATA_OFFSET(pNB) = uDataOffset;
		NET_BUFFER_DATA_LENGTH(pNB) = Length;

		pNBL->SourceHandle = NULL;

		// copy data if any
		pCurrData = (unsigned char*)MmGetSystemAddressForMdlSafe(pCurrMdl, NormalPagePriority);
		
		if(NULL == pCurrData){
			DBGLOG(( LError, "MmGetSystemAddressForMdlSafe failed\n" ));
			break;
		}

		// copy pbuf to packet
		CopyPbufToBuffer(
			pCurrData,
			MAX_ETHER_SIZE,
			/*pAdapter->Protocol.PppoeSessId ? PPPOE_HEADERLEN :*/ 0,
			p
			);

		if ( pInterface->MediaType == NdisMediumWirelessWan ){
			struct eth_hdr *pEthHeader = (struct eth_hdr*)pCurrData;
			NET_BUFFER_LIST_INFO(pNBL, NetBufferListFrameType) = (PVOID)(ULONG_PTR)pEthHeader->type;

			switch( pEthHeader->type ){
			case PP_HTONS(ETHTYPE_IP):
				NdisSetNblFlag(pNBL, /*NDIS_NBL_FLAGS_IS_IPV4*/0x00000200);
				break;
			//case ETH_P_IPV6_NET:
			//	NdisSetNblFlag(pNBL, NDIS_NBL_FLAGS_IS_IPV6);
			//	break;
			}
		}

		PacketContext = NDIS_PER_NBLIST_CONTEXT(pNBL);
		PacketContext->pAdapter = pAdapter;
		PacketContext->pProtocol = pProtocol;
		
		nStatus = NDIS_STATUS_SUCCESS;
	} while ( FALSE );

	if ( nStatus != NDIS_STATUS_SUCCESS ){
		if (pNBL != NULL){
			NdisFreeNetBufferListPtr(pNBL);
		}
	}else{
		*pNBList = pNBL;
	}

	return nStatus;
}
//***********************************************************************************
// Name: ProtocolSendPacketComplete
//
// Description: 
//		This function completes the processing of a protocol-initiated send, 
//		which returned NDIS_STATUS_PENDING.
//
// Return value: 
//		TRUE, if we own this packet
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

BOOLEAN
	ProtocolSendPacketComplete(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET     Packet,
		IN NDIS_STATUS      Status
		)
{
	DBGLOG(( LTrace, "complete packet %p with status = %08X\n", Packet, NDIS_GET_PACKET_STATUS((PNDIS_PACKET)Packet) ));

	// release packet
	NDIS_FreePacket( pProtocol, Packet );

	return TRUE;
}

//***********************************************************************************
// Name: ProtocolSendNetBufferListsComplete
//
// Description: 
//		This function completes the processing of a protocol-initiated send
//
// Return value: 
//		TRUE, if we own this packet
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

BOOLEAN
	ProtocolSendNetBufferListComplete(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNET_BUFFER_LIST NetBufferList
		)
{
	ASSERT ( NetBufferList->NdisPoolHandle == pProtocol->NdisNetBufferListPool );

	// reclaim the list
	NdisFreeNetBufferListPtr ( NetBufferList );
	InterlockedDecrement ( &pProtocol->OutstandingSends );

	return TRUE;
}

//***********************************************************************************
// Name: ProtocolSendPBuffer
//
// Description: 
//		lwip interface function that performs real packet sending
//
// Return value: 
//		Operation status
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
BOOLEAN
	ProtocolSendPBuffer(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN struct pbuf *p
		)
{
	PKIP_NDIS_ADAPTER pAdapter;
	PKIP_NDIS_INTERFACE pInterface = pProtocol->pInterface;
	PKIP_NDIS_MINIPORT pMiniport = pInterface->Miniport;
	NDIS_STATUS nStatus;
	BOOLEAN fbResult = FALSE;

	if ( pInterface->Type == IF_TYPE_NIC ){
		pAdapter = (PKIP_NDIS_ADAPTER)pInterface;
	}else{
		pAdapter = ((PKIP_RAS_LINK)pInterface)->pAdapter;
	}

	if ( pMiniport->MajorNdisVersion <= 5 )
	{
		PNDIS_PACKET_EX Packet;
		// Allocate new packet from adapter entry associated pool
		nStatus =
			ProtocolAllocateSendPacket(
				pAdapter,
				pProtocol,
				p,
				(PNDIS_PACKET*)&Packet
				);
		if ( nStatus != STATUS_SUCCESS ){
			DBGLOG(( LError, "%08X=ProtocolAllocateSendPacket failed\n", nStatus ));
			return FALSE;
		}

		DBGLOG(( LTrace, "send packet %p len=%d\n", Packet, p->tot_len ));
#ifdef _DBG_PACKET_DUMP
		DumpNdisPacket("<===", (PNDIS_PACKET)Packet );
#endif
		// send it
		nStatus = 
			MpSendPacketHandler( 
				pAdapter, 
				(PNDIS_PACKET)Packet 
				);
		if ( nStatus == NDIS_STATUS_SUCCESS ){
			fbResult = TRUE;
		}

	}else{
		PNET_BUFFER_LIST pNBL;
		// Allocate new packet from adapter entry associated pool
		nStatus =
			ProtocolAllocateSendNbList(
				pAdapter,
				pProtocol,
				p,
				&pNBL
				);
		if ( nStatus != STATUS_SUCCESS ){
			DBGLOG(( LError, "%08X=ProtocolAllocateSendNbList failed\n", nStatus ));
			return FALSE;
		}

		DBGLOG(( LTrace, "send nbl %p len=%d\n", pNBL, p->tot_len ));
#ifdef _DBG_PACKET_DUMP
		DumpNdisBuffer("<===", (struct eth_hdr*)p->payload, (PCHAR)p->payload + SIZEOF_ETH_HDR,p->len );
#endif
		// send it
		MpSendSendNetBufferLists( 
			pAdapter, 
			pNBL
			);
		fbResult = TRUE;
	}
	return fbResult;
}
//***********************************************************************************
// Name: ProtocolIndicate
//
// Description: 
//		indicates packet to lwip
//
// Return value: 
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
FORCEINLINE
VOID
	ProtocolIndicate(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PKIP_PACKET_DESCRIPTOR Packet
		)
{
	tcpip_input((struct pbuf*)&Packet->LwipPacket,&pProtocol->IIF);
}
//***********************************************************************************
// Name: ProtocolReceiveInternal
//
// Description: 
//		handles incoming data packet to ndis buffer
//
// Return value: 
//		NDIS_STATUS_SUCCESS, if protocol has processed the packet
//		NDIS_STATUS_NOT_ACCEPTED, if we don't own the packet
//		NDIS_STATUS_PENDING, protocol will release the packet later
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	ProtocolReceiveInternal(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET_EX MyPacket
		)
{
	PKIP_PACKET_DESCRIPTOR PacketDescr = NULL;
	NDIS_STATUS nStatus;
	BOOLEAN bCopy;
	FILTER_RESULT FilterResult = MyPacket->Reserved.FilterResult;

	do {
		bCopy = ( FilterResult == FILTER_INDICATE_COPY ) ||
			( FilterResult == FILTER_MORE_PROCESSING );

		if ( bCopy ){
			// allocate packet descriptor and copy data
			PacketDescr = 
				AllocatePacketDescriptor( 
					pProtocol,
					MyPacket, 
					NULL,0,
					MyPacket->Reserved.DataLength
					);
			if ( PacketDescr == NULL ){
				DBGLOG(( LError, "AllocatePacketDescriptor failed\n" ));
				nStatus = NDIS_STATUS_RESOURCES;
				break;
			}
			// save private data copy
			NdisMoveMemory ( 
				PacketDescr->DataBuffer,
				MyPacket->Reserved.DataBuffer,
				MyPacket->Reserved.DataLength
				);
			PacketDescr->Length = MyPacket->Reserved.DataLength;
		}else{
			// allocate packet descriptor
			PacketDescr = 
				AllocatePacketDescriptor( 
					pProtocol,
					MyPacket, 
					MyPacket->Reserved.DataBuffer,
					MyPacket->Reserved.DataLength,
					MyPacket->Reserved.DataLength
					);
		}

		if ( FilterResult == FILTER_MORE_PROCESSING ){
			// filter
			FilterResult =
				FltProcessIncomingPacket(
					pProtocol,
					PacketDescr
					);
		}

		switch ( FilterResult )
		{
		case FILTER_INDICATE:
			nStatus = NDIS_STATUS_SUCCESS;
			ProtocolIndicate(pProtocol,PacketDescr); //indicate to lwip
			break;
		case FILTER_INDICATE_COPY:
			PacketDescr->pOrgPacket = NULL; // see FreePacketDescriptor
			ProtocolIndicate(pProtocol,PacketDescr); //indicate to lwip
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			break;
		case FILTER_PASS:
		case FILTER_DROP:
			//DBGLOG(( LTrace, "PASS packet %p\n", Packet ));
		default:
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			PacketDescr->pOrgPacket = NULL; // see FreePacketDescriptor
			FreePacketDescriptor( PacketDescr );
			break;
		}

	} while ( FALSE );

	return nStatus;
}
//***********************************************************************************
// Name: ProtocolReceivePacket
//
// Description: 
//		handles incoming NDIS5 packet. protocol checks if we own the packet and sends it to
//		lwip for processing
//
// Return value: 
//		NDIS_STATUS_SUCCESS, if protocol has processed the packet
//		NDIS_STATUS_NOT_ACCEPTED, if we don't own the packet
//		NDIS_STATUS_PENDING, protocol will release the packet later
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	ProtocolReceivePacket(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET Packet
		)
{
	FILTER_RESULT FilterResult;
	PKIP_PACKET_DESCRIPTOR PacketDescr = NULL;
	NDIS_STATUS nStatus = NDIS_STATUS_NOT_ACCEPTED;
	UINT nBufferCount, TotalPacketLength, uReadyLen,FirstBufferLength;
	BOOLEAN fbResult = FALSE;
	PNDIS_BUFFER  FirstBuffer;
	PVOID FirstBufferVA;
	struct eth_hdr *pEthHeader;
	BOOLEAN fbEtherBroadcast = FALSE;

	do 
	{
		NdisGetFirstBufferFromPacket(
			Packet,
			&FirstBuffer,
			&FirstBufferVA,
			&FirstBufferLength,
			&TotalPacketLength
			);

#ifdef _DBG_PACKET_DUMP
		DBGLOG(( LTrace, "Incoming packet %d bytes\n", TotalPacketLength ));
		DumpNdisPacket("===>", Packet);
#endif

		// copy packet data to buffer
		if ( TotalPacketLength == 0 ){
			DBGLOG(( LError, "TotalPacketLength == 0\n" ));
			nStatus = NDIS_STATUS_FAILURE;
			break;
		}

		//
		// Fast filter packet
		//
		FilterResult = 
			FltProcessIncomingBuffer(
				pProtocol,
				FirstBufferVA,
				FirstBufferLength
				);

		// call original
		if ( FilterResult == FILTER_PASS || FilterResult == FILTER_DROP )
		{
			DBGLOG(( LTrace, "PASS packet %p\n", Packet ));
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			break;
		}

		// allocate packet descriptor and copy data
		PacketDescr = 
			AllocatePacketDescriptor( 
				pProtocol,
				Packet, 
				NULL,0 ,
				TotalPacketLength
				);

		if ( PacketDescr == NULL ){
			DBGLOG(( LError, "AllocatePacketDescriptor failed\n" ));
			nStatus = NDIS_STATUS_RESOURCES;
			break;
		}
	
		fbResult =
			NDIS_CopyPacketData( 
				Packet,
				PacketDescr->DataBuffer,
				TotalPacketLength, 0,
				&uReadyLen
				);
		if ( !fbResult ){
			DBGLOG(( LError, "NDIS_CopyPacketData failed\n" ));
			nStatus = NDIS_STATUS_FAILURE;
			break;
		}
		PacketDescr->pOrgPacket = NULL; // we complete miniport's packet immediately
		PacketDescr->Length = TotalPacketLength;
		
		//
		// now we are ready to handle the incoming data
		//

		if ( FilterResult == FILTER_MORE_PROCESSING ){
			// filter
			FilterResult =
				FltProcessIncomingPacket(
					pProtocol,
					PacketDescr
					);
		}

		switch ( FilterResult )
		{
		case FILTER_INDICATE:
			nStatus = NDIS_STATUS_SUCCESS;
			ProtocolIndicate(pProtocol,PacketDescr); //indicate to lwip
			DBGLOG(( LTrace, "INDICATE packet %p\n", Packet ));
			fbResult = TRUE;
			break;
		case FILTER_INDICATE_COPY:
			nStatus = NDIS_STATUS_NOT_ACCEPTED; // this status forces packet to be indicated to ndis
			ProtocolIndicate(pProtocol,PacketDescr); //indicate to lwip
			DBGLOG(( LTrace, "INDICATE_COPY packet %p\n", Packet ));
			fbResult = TRUE;
			break;
		case FILTER_PASS:
		case FILTER_DROP:
			DBGLOG(( LTrace, "PASS packet %p\n", Packet ));
		default:
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			fbResult = FALSE;
			break;
		}

	} while ( FALSE );

	if ( fbResult == FALSE ){
		if ( PacketDescr ){
			FreePacketDescriptor( PacketDescr );
		}
	}
	return nStatus;
}
//***********************************************************************************
// Name: ProtocolReceive
//
// Description: 
//		old-style data indication handler
//
// Return value: 
//		Operation status.
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
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
		)
{
	PKIP_NDIS_ADAPTER pAdapter = (PKIP_NDIS_ADAPTER)pProtocol->pInterface;
	FILTER_RESULT FilterResult;
	NDIS_STATUS nStatus = NDIS_STATUS_SUCCESS;
	PNDIS_PACKET_EX MyPacket;
	PNDIS_BUFFER	NdisBuffer,ResidualBuffer;
	struct eth_hdr *pEthHeader = (struct eth_hdr *)HeaderBuffer;
	BOOLEAN fbEtherBroadcast = FALSE;

	do 
	{
#ifdef _DBG_PACKET_DUMP
		DBGLOG(( LTrace, "Incoming packet %d bytes\n", PacketSize ));
		if ( HeaderBufferSize >= SIZEOF_ETH_HDR ){
			DumpNdisBuffer("===>", HeaderBuffer,LookaheadBuffer,LookaheadBufferSize);
		}
#endif

		//
		// fast filter input
		//

		// filter incoming ethernet
		FilterResult = 
			FltFastProcessIncomingEthrenet( 
				pProtocol, 
				HeaderBuffer, 
				HeaderBufferSize,
				&fbEtherBroadcast
				);

		if ( FilterResult == FILTER_MORE_PROCESSING ){
			switch ( pEthHeader->type )
			{
			case PP_HTONS(ETHTYPE_ARP):
				// filter incoming ip
				FilterResult = 
					FltFastProcessIncomingARP( 
						pProtocol, 
						LookaheadBuffer, 
						LookaheadBufferSize
						);
				break;
			case PP_HTONS(ETHTYPE_IP):
				// filter incoming ip
				FilterResult = 
					FltFastProcessIncomingIP( 
						pProtocol, 
						LookaheadBuffer, 
						LookaheadBufferSize,
						fbEtherBroadcast
						);
				break;
			}
		}

		// call original
		if ( FilterResult == FILTER_PASS || FilterResult == FILTER_DROP )
		{
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			break;
		}

		if ( ResidualSize == 0 ){
			PKIP_PACKET_DESCRIPTOR PacketDescr;

			if ( FilterResult == FILTER_MORE_PROCESSING )
			{
				if ( pEthHeader->type == PP_HTONS(ETHTYPE_IP) ){
					FilterResult =
						FltProcessIncomingIP(
							pProtocol,
							(struct eth_hdr*)HeaderBuffer,
							(struct ip_hdr*)LookaheadBuffer,
							PacketSize
							);
				}else{
					ASSERT(FALSE);
				}
			}

			if ( FilterResult != FILTER_INDICATE && FilterResult != FILTER_INDICATE_COPY ){
				nStatus = NDIS_STATUS_NOT_ACCEPTED;
				break;
			}

			// allocate descriptor and indicate packet to lwip
			PacketDescr = 
				AllocatePacketDescriptor( 
					pProtocol,
					NULL, 
					NULL,0 ,
					PacketSize
					);

			if ( PacketDescr == NULL ){
				DBGLOG(( LError, "AllocatePacketDescriptor failed\n" ));
				nStatus = NDIS_STATUS_RESOURCES;
				break;
			}

			// copy packet data
			NdisCopyLookaheadData(
				PacketDescr->DataBuffer,
				HeaderBuffer, 
				HeaderBufferSize,
				pAdapter->MacOptions
				);
			// save packet data
			NdisCopyLookaheadData(
				(PCHAR)PacketDescr->DataBuffer+HeaderBufferSize,
				LookaheadBuffer, 
				LookaheadBufferSize,
				pAdapter->MacOptions
				);
			PacketDescr->Length = PacketSize;

			switch ( FilterResult )
			{
			case FILTER_INDICATE:
				nStatus = NDIS_STATUS_SUCCESS;
				ProtocolIndicate(pProtocol,PacketDescr); //indicate to lwip
				break;
			case FILTER_INDICATE_COPY:
				nStatus = NDIS_STATUS_NOT_ACCEPTED; // this status forces packet to be indicated to ndis
				ProtocolIndicate(pProtocol,PacketDescr); //indicate to lwip
				break;
			}
			return nStatus;
		}

		// now we have result FILTER_INDICATE, FILTER_INDICATE_COPY or FILETR_MORE_PROCESSING
		// go ahead and copy data

		// now we build packet and receive the data
		KIP_LOCK_PACKET_POOL ( pProtocol );

		NdisAllocatePacket ( 
			&nStatus, 
			(PNDIS_PACKET*)(&MyPacket),
			pProtocol->PacketPoolHandle
			);

		KIP_UNLOCK_PACKET_POOL( pProtocol );

		if ( nStatus != NDIS_STATUS_SUCCESS ){
			DBGLOG(( LError, "%08X=NdisAllocatePacket failed\n", nStatus ));
			break;
		}

		NdisReinitializePacket ((PNDIS_PACKET)MyPacket);

		// allocate new buffer for data
		NdisAllocateBuffer ( 
			&nStatus,
			&NdisBuffer,
			pProtocol->BufferPoolHandle,
			(PVOID) MyPacket->Reserved.DataBuffer,
			HeaderBufferSize + PacketSize - ResidualSize
			);

		if ( nStatus != NDIS_STATUS_SUCCESS ){
			DBGLOG(( LError, "%08X=NdisAllocateBuffer failed\n", nStatus ));
			NDIS_FreePacket( pProtocol, (PNDIS_PACKET)MyPacket );
			break;
		}

		//
		// Chain Our Buffer To Our Packet
		//
		NdisChainBufferAtFront((PNDIS_PACKET )MyPacket, NdisBuffer );

		//
		// Header buffer to our packet data
		//
		NdisCopyLookaheadData (
			MyPacket->Reserved.DataBuffer,
			HeaderBuffer,
			HeaderBufferSize,
			pAdapter->MacOptions
			);

		//Lookahead buffer to our packet data
		NdisCopyLookaheadData (
			&MyPacket->Reserved.DataBuffer[ HeaderBufferSize],
			LookaheadBuffer,
			LookaheadBufferSize,
			pAdapter->MacOptions
			);

		MyPacket->Reserved.DataLength   = HeaderBufferSize + LookaheadBufferSize;
		MyPacket->Reserved.FilterResult = FilterResult; // save filter result here
		MyPacket->Reserved.BytesTransferred = 0;
		MyPacket->Reserved.Status = NDIS_STATUS_SUCCESS;

		// Insert into adapter associated list
		NdisAcquireSpinLock( &pProtocol->PendingTransfertLock );
		InsertHeadList (
			&pProtocol->PendingTransfertData,
			&MyPacket->Reserved.qLink
			);
		NdisReleaseSpinLock( &pProtocol->PendingTransfertLock );

		if ( ResidualSize )
		{
			// Not whole packet was indicated here
			// Allocate and chain temporary buffer to our packet
			NdisAllocateBuffer (
				&nStatus,
				&ResidualBuffer,
				pProtocol->BufferPoolHandle,
				(PVOID) &MyPacket->Reserved.DataBuffer[HeaderBufferSize+LookaheadBufferSize],
				ResidualSize
				);

			if ( nStatus != NDIS_STATUS_SUCCESS )
			{
				DBGLOG(( LError, "%08X=NdisAllocateBuffer failed\n", nStatus ));

				// unlink packet
				NdisAcquireSpinLock( &pProtocol->PendingTransfertLock );
				RemoveEntryList ( &MyPacket->Reserved.qLink	);
				NdisReleaseSpinLock( &pProtocol->PendingTransfertLock );

				NDIS_FreePacket( pProtocol, (PNDIS_PACKET)MyPacket );
				break;
			}

			NdisChainBufferAtFront((PNDIS_PACKET)MyPacket, ResidualBuffer);

			//Set OOB header size
			NDIS_SET_PACKET_HEADER_SIZE(( PNDIS_PACKET )MyPacket, HeaderBufferSize);
			//
			// Call NDIS to transfer data remain
			//
			MyPacket->Reserved.Status =
				NDIS_TransferData(
					pAdapter,
					(PNDIS_PACKET)MyPacket,
					&MyPacket->Reserved.BytesTransferred,
					MacReceiveContext,
					LookaheadBufferSize,
					ResidualSize
					);
		}

		//
		//	Transfer 
		//
		if ( MyPacket->Reserved.Status != NDIS_STATUS_PENDING )
		{
			BOOLEAN fbCallOrig;
			BOOLEAN fbResult =
				MpTransferDataComplete( 
					pAdapter,
					( PNDIS_PACKET ) MyPacket,
					MyPacket->Reserved.Status,
					MyPacket->Reserved.BytesTransferred,
					&fbCallOrig
					);
			if ( fbResult == FALSE ){
				// we failed - recycle the packet

				// unlink packet
				NdisAcquireSpinLock( &pProtocol->PendingTransfertLock );
				RemoveEntryList ( &MyPacket->Reserved.qLink	);
				NdisReleaseSpinLock( &pProtocol->PendingTransfertLock );

				NDIS_FreePacket( pProtocol, (PNDIS_PACKET)MyPacket );
				nStatus = NDIS_STATUS_FAILURE;
				break;
			}

			nStatus = NDIS_STATUS_SUCCESS;
		}
		else
		{
			nStatus = NDIS_STATUS_PENDING;
		}

	} while ( FALSE );

	return nStatus;
}
//***********************************************************************************
// Name: ProtocolReturnPacketHandler
//
// Description: 
//		handles packet return to miniport. here we check if it's our packet and
//		reclaim it
//
// Return value: 
//		NDIS_STATUS_SUCCESS, if protocol has processed the packet
//		NDIS_STATUS_NOT_ACCEPTED, if we don't own the packet, pass it to original
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
PNDIS_PACKET
	ProtocolReturnPacketHandler(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET  Packet
		) 
{
	//
	// Get packet pool
	//

	if ( NDIS_GetPoolFromPacket( Packet ) == pProtocol->PacketPoolHandle ){

		//
		// Reclaim packet
		//

		NDIS_FreePacket(pProtocol,Packet);
		Packet = NULL;
	}
	return Packet;
}
//***********************************************************************************
// Name: ProtocolWanReceive
//
// Description: 
//		processes
//
// Return value: 
//		Operation status.
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	ProtocolWanReceive(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNDIS_PACKET Packet
		)
{
	FILTER_RESULT FilterResult;
	NDIS_STATUS nStatus = NDIS_STATUS_SUCCESS;
	PKIP_RAS_LINK pRasLink;
	PKIP_PACKET_DESCRIPTOR PacketDescr;
	BOOLEAN fbEtherBroadcast = FALSE;

	PNDIS_BUFFER  FirstBuffer;
	PVOID FirstBufferVA;
	UINT TotalPacketLength = 0,FirstBufferLength = 0;
	struct eth_hdr *pEthHeader;

	do 
	{
		NdisGetFirstBufferFromPacket(
			Packet,
			&FirstBuffer,
			&FirstBufferVA,
			&FirstBufferLength,
			&TotalPacketLength
			);

		if ( TotalPacketLength == 0 ){
			DBGLOG(( LError, "TotalPacketLength == 0\n" ));
			nStatus = NDIS_STATUS_FAILURE;
			break;
		}

		if ( FirstBufferLength < SIZEOF_ETH_HDR ){
			DBGLOG(( LError, "FirstBufferLength < SIZEOF_ETH_HDR\n" ));
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			break;
		}

		pEthHeader = (struct eth_hdr *)FirstBufferVA;

		// get wan link ptr
		pRasLink = NDIS_FindRasLinkByMac( pAdapter, pEthHeader->dest.addr );
		if ( pRasLink == NULL ){
			DBGLOG(( LError, "can't find link for %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
				pEthHeader->dest.addr[0],
				pEthHeader->dest.addr[1],
				pEthHeader->dest.addr[2],
				pEthHeader->dest.addr[3],
				pEthHeader->dest.addr[4],
				pEthHeader->dest.addr[5],
				pEthHeader->dest.addr[0],
				pEthHeader->dest.addr[1],
				pEthHeader->dest.addr[2],
				pEthHeader->dest.addr[3],
				pEthHeader->dest.addr[4],
				pEthHeader->dest.addr[5]
			));
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			break;
		}

		pProtocol = &pRasLink->Interface.Protocol;

		nStatus =
			ProtocolReceivePacket(
				pProtocol,
				Packet
				);

	}while (FALSE);

	return nStatus;
}
//***********************************************************************************
// Name: ProtocolIndicateNetBufferList
//
// Description: 
//		indicates net buffer to protocol
//
// Return value: 
//		NDIS_STATUS_SUCCESS, if protocol has processed the packet
//		NDIS_STATUS_NOT_ACCEPTED, if we don't own the packet, pass it to original
//
// Parameters: 
//
// NOTE: We can't break buffers chain. we need process each buffer separately:
//       pass it to lwip or indicate to ndis
// **********************************************************************************
NDIS_STATUS
	ProtocolIndicateNetBuffer(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNET_BUFFER NetBuffer
		) 
{
	PKIP_RAS_LINK pRasLink;
	NDIS_STATUS nStatus = NDIS_STATUS_NOT_ACCEPTED;
	FILTER_RESULT FilterResult;
	PUCHAR	FirstBufferVA = NULL;
	ULONG	FirstBufferLength;
	PKIP_PACKET_DESCRIPTOR PacketDescr = NULL;
	ULONG TotalPacketLength = 0;
	BOOLEAN fbResult;

	do {

		NDIS_GetFirstBufferFromNetBuffer(
			NetBuffer,
			&FirstBufferVA,
			&FirstBufferLength
			);

		if( FirstBufferVA == NULL || FirstBufferLength == 0 ){
			ASSERT(FALSE);
			DBGLOG(( LTrace, "Can't get first buffer VA\n" ));
			break;
		}

#ifdef _DBG_PACKET_DUMP
		DumpNdisBuffer("===>", (struct eth_hdr*)FirstBufferVA, (PCHAR)FirstBufferVA + SIZEOF_ETH_HDR, FirstBufferLength );
#endif
		if ( KIP_MINIPORT_TEST_FLAG(pAdapter,fADAPTER_IS_CO) == TRUE ){
			struct eth_hdr *pEthHeader = (struct eth_hdr *)FirstBufferVA;
			if ( FirstBufferLength < SIZEOF_ETH_HDR ){
				break;
			}
			// get wan link ptr
			pRasLink = NDIS_FindRasLinkByMac( pAdapter, pEthHeader->dest.addr );
			if ( pRasLink == NULL ){
				ASSERT(FALSE);
				break;
			}
			pProtocol = &pRasLink->Interface.Protocol;
		}

		//
		// Fast filter packet
		//
		FilterResult = 
			FltProcessIncomingBuffer(
				pProtocol,
				FirstBufferVA,
				FirstBufferLength
				);

		// call original
		if ( FilterResult == FILTER_PASS || FilterResult == FILTER_DROP ) {
			// data not accepted
			DBGLOG(( LTrace, "PASS net buffer %p\n", NetBuffer ));
			break;
		}

		// allocate packet descriptor and copy data
		PacketDescr = 
			AllocatePacketDescriptor( 
				pProtocol,
				NULL, // no packet 
				NULL,0,  // allocate buffer for us
				MAX_ETHER_SIZE // maximum supported
				);

		if ( PacketDescr == NULL ){
			DBGLOG(( LError, "AllocatePacketDescriptor failed\n" ));
			nStatus = NDIS_STATUS_RESOURCES;
			break;
		}

		fbResult =
			NDIS_CopyNetBufferToBuffer( 
				NetBuffer,
				PacketDescr->DataBuffer,
				MAX_ETHER_SIZE, // maximum supported
				0,
				&TotalPacketLength
				);
		if ( !fbResult ){
			DBGLOG(( LError, "NDIS_CopyNetBufferToBuffer failed\n" ));
			nStatus = NDIS_STATUS_FAILURE;
			break;
		}
		PacketDescr->Length = TotalPacketLength; // save the original packet length
		PacketDescr->LwipPacket.pbuf.len = PacketDescr->LwipPacket.pbuf.tot_len = (u16_t)TotalPacketLength;

		//
		// now we are ready to handle the incoming data
		//

		if ( FilterResult == FILTER_MORE_PROCESSING ){
			// filter
			FilterResult =
				FltProcessIncomingPacket(
					pProtocol,
					PacketDescr
					);
		}

		switch ( FilterResult )
		{
		case FILTER_INDICATE:
			nStatus = NDIS_STATUS_SUCCESS;
			ProtocolIndicate(pProtocol,PacketDescr); //indicate to lwip
			DBGLOG(( LTrace, "INDICATE net buffer %p\n", NetBuffer ));
			fbResult = TRUE;
			break;
		case FILTER_INDICATE_COPY:
			nStatus = NDIS_STATUS_NOT_ACCEPTED; // this status forces packet to be indicated to ndis
			ProtocolIndicate(pProtocol,PacketDescr); //indicate to lwip
			DBGLOG(( LTrace, "INDICATE_COPY packet net buffer %p\n", NetBuffer ));
			fbResult = TRUE;
			break;
		case FILTER_PASS:
		case FILTER_DROP:
			DBGLOG(( LTrace, "PASS packet net buffer %p\n", NetBuffer ));
		default:
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			fbResult = FALSE;
			break;
		}

	} while ( FALSE );

	if ( fbResult == FALSE ){
		if ( PacketDescr ){
			FreePacketDescriptor( PacketDescr );
		}
	}
	return nStatus;
}
//***********************************************************************************
// Name: ProtocolIndicateNetBufferList
//
// Description: 
//		handles incoming net buffer list
//
// Return value: 
//		NDIS_STATUS_SUCCESS, if protocol has processed the packet
//		NDIS_STATUS_NOT_ACCEPTED, if we don't own the packet, pass it to original
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	ProtocolIndicateNetBufferList(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PNET_BUFFER_LIST NetBufferList,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN ULONG  ReceiveFlags
		) 
{
	NDIS_STATUS nStatus = NDIS_STATUS_NOT_ACCEPTED;
	PNET_BUFFER pNB = NULL;
	BOOLEAN fbProcessed = FALSE;
//	PKIP_NDIS_ADAPTER pAdapter = (PKIP_NDIS_ADAPTER)pProtocol->pInterface;

#define KIP_NB_PASS_FLAG (PVOID)1
#define KIP_NB_PROCESSED_FLAG (PVOID)2

	// sanity checks
	pNB = NET_BUFFER_LIST_FIRST_NB(NetBufferList);
	if( pNB == NULL ){
		return nStatus;
	}

	for(pNB = NET_BUFFER_LIST_FIRST_NB(NetBufferList);
		pNB;
		pNB = NET_BUFFER_NEXT_NB(pNB))
	{
		nStatus = 
			ProtocolIndicateNetBuffer(
				pAdapter,
				pProtocol,
				pNB
				);

		if ( nStatus == NDIS_STATUS_NOT_ACCEPTED ){
			 // mark it to pass
			pNB->ProtocolReserved[0] = KIP_NB_PASS_FLAG;
		}else{
			fbProcessed = TRUE;
			pNB->ProtocolReserved[0] = KIP_NB_PROCESSED_FLAG;
		}
	}

	if ( fbProcessed == FALSE ){
		// cleanup flags
		for(pNB = NET_BUFFER_LIST_FIRST_NB(NetBufferList);
			pNB;
			pNB = NET_BUFFER_NEXT_NB(pNB))
		{
			pNB->ProtocolReserved[0] = NULL;
		}
		// pass nbl
		nStatus = NDIS_STATUS_NOT_ACCEPTED;

	}else{
		// now each net buffer is marked in pNB->ProtocolReserved with 'processed' flag
		// if there is processed buffer, we need to indicate other buffers separately
		// since we can't break buffers chain

		for(pNB = NET_BUFFER_LIST_FIRST_NB(NetBufferList);
			pNB;
			pNB = NET_BUFFER_NEXT_NB(pNB))
		{
			if ( pNB->ProtocolReserved[0] == KIP_NB_PASS_FLAG ){
				pNB->ProtocolReserved[0] = NULL;
				// indicate nb to ndis
				MpIndicateDataBufferToNdis(
					pAdapter,
					pNB,
					PortNumber,
					ReceiveFlags
					);
			}else { // if ( pNB->ProtocolReserved[0] == KIP_NB_PROCESSED_FLAG ){
				// do nothing
				pNB->ProtocolReserved[0] = NULL;
			}			 
		}

		// we've processed nbl manually
		nStatus = NDIS_STATUS_SUCCESS;
	}

	return nStatus;
}