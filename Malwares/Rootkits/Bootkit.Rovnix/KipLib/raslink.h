#ifndef _RAS_LINK_H_
#define _RAS_LINK_H_

BOOL 
	WAN_CheckForContext (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PCHAR pProtoContext
		);

NDIS_STATUS 
	WAN_AddRasLink (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WAN_LINE_UP pWanLineUp
		);

VOID 
	WAN_RemoveRasLink (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WAN_LINE_DOWN pWanLineDown
		);

VOID 
	WAN_RemoveAllLinks (
		IN PKIP_NDIS_ADAPTER pAdapter
		);

VOID 
	WAN_WwanActivated (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WWAN_CONTEXT_STATE ContextState
		);

VOID 
	WAN_WwanDeactivated (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_WWAN_CONTEXT_STATE ContextState
		);

#endif //_RAS_LINK_H_
