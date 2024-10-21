/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: raslink.c
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//	WAN link interface

#include "project.h"
#include "etharp.h"

#define ETHARP_FLAG_STATIC_ENTRY 4

BOOL 
	WAN_CheckForContext (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PCHAR pProtoContext
		)
{
	PLIST_ENTRY ListEntry;
	PKIP_RAS_LINK pRasLink;
	BOOLEAN fbResult = FALSE;

	DBGLOG(( LTrace, "Context: %.2X%.2X%.2X%.2X\n", 
				pProtoContext[0],
				pProtoContext[1],
				pProtoContext[2],
				pProtoContext[3]
				));

	KIP_LOCK_ADAPTER(pAdapter);
	for ( ListEntry = pAdapter->RasLinkList.Flink; 
		ListEntry != &pAdapter->RasLinkList;
		ListEntry = ListEntry->Flink)
	{
		pRasLink = CONTAINING_RECORD(ListEntry,KIP_RAS_LINK,Interface.Link);
		if ( (RtlCompareMemory(&pRasLink->Interface.CurrentMacAddress[2], pProtoContext, 4) == 4) ){
			fbResult = TRUE;
			break;
		}
	}

	KIP_UNLOCK_ADAPTER(pAdapter);

	return fbResult;
}

NDIS_STATUS 
	WAN_AddRasLink (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WAN_LINE_UP pWanLineUp
		)
{
	NDIS_STATUS nStatus = NDIS_STATUS_SUCCESS;
	PKIP_RAS_LINK RasLink = NULL;
	
	ULONG OsVersion = OsGetMajorVersion();

	DBGLOG(( LTrace, "Local MAC: %.2X%.2X%.2X%.2X%.2X%.2X Remote MAC: %.2X%.2X%.2X%.2X%.2X%.2X\n", 
				pWanLineUp->LocalAddress[0],
				pWanLineUp->LocalAddress[1],
				pWanLineUp->LocalAddress[2],
				pWanLineUp->LocalAddress[3],
				pWanLineUp->LocalAddress[4],
				pWanLineUp->LocalAddress[5],
				pWanLineUp->RemoteAddress[0],
				pWanLineUp->RemoteAddress[1],
				pWanLineUp->RemoteAddress[2],
				pWanLineUp->RemoteAddress[3],
				pWanLineUp->RemoteAddress[4],
				pWanLineUp->RemoteAddress[5]
				));

	if ( pWanLineUp->ProtocolType != ETHTYPE_IP ){
		return NDIS_STATUS_SUCCESS;
	}

	if ( OsVersion < 5 ){
		if ( pWanLineUp->ProtocolBufferLength < sizeof(WAN_IP_INFO_NT)){
			return NDIS_STATUS_SUCCESS;
		}
	}else if ( OsVersion < 6 ){
		if ( pWanLineUp->ProtocolBufferLength < sizeof(WAN_IP_INFO_W2K)){
			return NDIS_STATUS_SUCCESS;
		}
	}else if ( OsVersion < 7 ){
		if ( pWanLineUp->ProtocolBufferLength < sizeof(WAN_IP_INFO_VISTA)){
			return NDIS_STATUS_SUCCESS;
		}
	}

	// To avoid RAS entries duplications
	if (WAN_CheckForContext(pAdapter, &pWanLineUp->LocalAddress[2]) == FALSE )
	{
		RasLink = NDIS_AllocateRasLink( pAdapter, pWanLineUp );
		if ( RasLink == NULL ) {
			nStatus = NDIS_STATUS_RESOURCES;
		}else{
			
			if ( OsVersion < 5 ){
				PWAN_IP_INFO_NT WanIpInfo = (PWAN_IP_INFO_NT)pWanLineUp->ProtocolBuffer;
				ip4_addr_set_u32(&RasLink->IpAddr,WanIpInfo->IpAddress);
				ip4_addr_set_u32(&RasLink->IpMask,IPADDR_BROADCAST);
			}else if ( OsVersion < 6 ){
				PWAN_IP_INFO_W2K WanIpInfo = (PWAN_IP_INFO_W2K)pWanLineUp->ProtocolBuffer;
				ip4_addr_set_u32(&RasLink->IpAddr,WanIpInfo->LocalIpAddress);
				ip4_addr_set_u32(&RasLink->IpMask,WanIpInfo->IpSubnetMask);
				ip4_addr_set_u32(&RasLink->RemoteIpAddr,WanIpInfo->RemoteIpAddress);
			}else if ( OsVersion < 7 ){
				PWAN_IP_INFO_VISTA WanIpInfo = (PWAN_IP_INFO_VISTA)pWanLineUp->ProtocolBuffer;
				ip4_addr_set_u32(&RasLink->IpAddr,WanIpInfo->LocalIpAddress);
				ip4_addr_set_u32(&RasLink->IpMask,WanIpInfo->IpSubnetMask);
				ip4_addr_set_u32(&RasLink->RemoteIpAddr,WanIpInfo->RemoteIpAddress);
			}

			ProtocolBindAdapter(
				(PKIP_NDIS_INTERFACE)RasLink,
				RasLink->IpAddr.addr,
				RasLink->IpMask.addr,
				RasLink->IpAddr.addr,
				TRUE
				);

			// update arp table
			update_arp_entry( 
				&RasLink->Interface.Protocol.IIF, 
				&RasLink->RemoteIpAddr, 
				(struct eth_addr*)RasLink->RemoteAddress, 
				ETHARP_FLAG_STATIC_ENTRY 
				);

			update_arp_entry( 
				&RasLink->Interface.Protocol.IIF, 
				&RasLink->IpAddr, 
				(struct eth_addr*)RasLink->RemoteAddress,
				ETHARP_FLAG_STATIC_ENTRY 
				);
		}
	}
	return nStatus;
}

VOID 
	WAN_RemoveRasLink (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WAN_LINE_DOWN pWanLineDown
		)
{
	PKIP_RAS_LINK RasLink = NULL;
	PLIST_ENTRY ListEntry;
	
	DBGLOG(( LTrace, "Local MAC: %.2X%.2X%.2X%.2X%.2X%.2X Remote MAC: %.2X%.2X%.2X%.2X%.2X%.2X\n", 
				pWanLineDown->LocalAddress[0],
				pWanLineDown->LocalAddress[1],
				pWanLineDown->LocalAddress[2],
				pWanLineDown->LocalAddress[3],
				pWanLineDown->LocalAddress[4],
				pWanLineDown->LocalAddress[5],
				pWanLineDown->RemoteAddress[0],
				pWanLineDown->RemoteAddress[1],
				pWanLineDown->RemoteAddress[2],
				pWanLineDown->RemoteAddress[3],
				pWanLineDown->RemoteAddress[4],
				pWanLineDown->RemoteAddress[5]
				));

	KIP_LOCK_ADAPTER(pAdapter);

	for( ListEntry = pAdapter->RasLinkList.Flink;
		ListEntry != &pAdapter->RasLinkList;
		ListEntry = ListEntry->Flink )
	{
		RasLink = CONTAINING_RECORD(ListEntry,KIP_RAS_LINK,Interface.Link);
		if ( (RtlCompareMemory(RasLink->RemoteAddress, pWanLineDown->RemoteAddress, ETHARP_HWADDR_LEN) == ETHARP_HWADDR_LEN ) 
			&& (RtlCompareMemory(RasLink->Interface.CurrentMacAddress, pWanLineDown->LocalAddress, ETHARP_HWADDR_LEN) == ETHARP_HWADDR_LEN ) )
		{
			RemoveEntryList (&RasLink->Interface.Link);
			break;
		}
		RasLink = NULL;
	}
	KIP_UNLOCK_ADAPTER(pAdapter);

	if ( RasLink ){

		// update arp table
		etharp_remove_static_entry( &RasLink->IpAddr );
		etharp_remove_static_entry( &RasLink->RemoteIpAddr );

		ProtocolUnBindAdapter(
			RasLink->Interface.Miniport,
			&RasLink->Interface,
			&RasLink->Interface.Protocol
			);
		NdisFreeMemory ( RasLink, sizeof (KIP_RAS_LINK), 0 );		
	}
}

VOID 
	WAN_RemoveAllLinks (
		IN PKIP_NDIS_ADAPTER pAdapter
		)
{
	PKIP_RAS_LINK RasLink = NULL;
	PLIST_ENTRY ListEntry;

	KIP_LOCK_ADAPTER(pAdapter);

	while (!IsListEmpty(&pAdapter->RasLinkList))
	{
		ListEntry = RemoveHeadList(&pAdapter->RasLinkList);
		RasLink = CONTAINING_RECORD(ListEntry,KIP_RAS_LINK,Interface.Link);
		NdisFreeMemory ( RasLink, sizeof (KIP_RAS_LINK), 0 );		
	}

	KIP_UNLOCK_ADAPTER(pAdapter);
}

VOID 
	WAN_WwanActivated (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WWAN_CONTEXT_STATE ContextState
		)
{
	//if(WWAN_STATUS_SUCCESS == pContextState->uStatus) {
	//	NdisMoveMemory (
	//		&pAdapter -> m_LocalAddress[2],
	//		&pContextState->ContextState.ConnectionId,
	//		sizeof(ULONG)
	//		);
	//}
}

VOID 
	WAN_WwanDeactivated (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WWAN_CONTEXT_STATE ContextState
		)
{
}