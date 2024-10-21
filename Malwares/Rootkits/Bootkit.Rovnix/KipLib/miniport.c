//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: miniport.c
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//	miniport interface functions that handle incoming events

#include "project.h"
#include "ndishook.h"
#include "filter.h"
#include "dma.h"
#include "raslink.h"
#include "lwip\netifapi.h"

#define MODULE_TAG 'MPIK'

#define	INTERNAL_INDICATION_SIZE			-2
#define	INTERNAL_INDICATION_BUFFER			(PVOID)-1

BOOLEAN ndisMediaTypeCl[NdisMediumMax] =
{
	TRUE,
	TRUE,
	TRUE,
	FALSE,
	TRUE,
	TRUE,
	TRUE,
	TRUE,
	FALSE,
	TRUE,
	TRUE,
	TRUE,
	FALSE,
	FALSE
};

//***********************************************************************************
// Name: MpInitialize_NDIS5
//
// Description: 
//		Initializes NDIS5 miniport
//
// Return value: 
//		operation status.
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

NDIS_STATUS
	MpInitialize(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_MEDIUM MediaType
		)
{
	NDIS_STATUS nStatus;
	ULONG GenericUlong = 0;
	UCHAR CurrentLongAddress[6];
	NDIS_HANDLE ConfigurationHandle;

	do{

		// adapter media
		pAdapter->Interface.MediaType = MediaType;

		// check if we support this media
		if ( MediaType == NdisMedium802_3/* || 
			NdisMediumWirelessWan*/)
		{
			KIP_MINIPORT_SET_ACTIVE(pAdapter);
		}
		else{
			KIP_MINIPORT_SET_INACTIVE(pAdapter);
			nStatus = NDIS_STATUS_NOT_SUPPORTED;
			break;
		}

		//
		// get vendor description
		//
		nStatus = 
			NDIS_MQueryOid(
				pAdapter,
				OID_GEN_VENDOR_DESCRIPTION,
				pAdapter->szVendorDescription,
				sizeof(pAdapter->szVendorDescription)
				);
		if ( nStatus != NDIS_STATUS_SUCCESS )
		{
			DBGLOG(( LInfo, "%08X = NDIS_MQueryOid(OID_GEN_VENDOR_DESCRIPTION) failed\n", nStatus ));
		}		

		if ( pAdapter->MaximumLookahead == 0 ){
			//
			// Set Maximumlookahead to 0 as default. For lan media query the real
			// stuff.
			//
			if ((NdisMediumWan != MediaType) && ndisMediaTypeCl[MediaType])
			{
				//
				// Query maximum lookahead
				//
				nStatus = 
					NDIS_MQueryOid(
						pAdapter,
						OID_GEN_MAXIMUM_LOOKAHEAD,
						&GenericUlong,
						sizeof(GenericUlong)
						);
				if ( nStatus != NDIS_STATUS_SUCCESS )
				{
					DBGLOG(( LError, "%08X = NDIS_MQueryOid(OID_GEN_MAXIMUM_LOOKAHEAD) failed\n", nStatus ));
					//break;
				}
			}

			//
			// Now adjust based on media type
			//
			switch( MediaType )
			{
			case NdisMedium802_3:
				pAdapter->MaximumLookahead = ((NDIS_M_MAX_LOOKAHEAD - 14) < GenericUlong) ?
					NDIS_M_MAX_LOOKAHEAD - 14 : GenericUlong;
				break;
			case NdisMediumWan:
				pAdapter->MaximumLookahead = NDIS_M_MAX_LOOKAHEAD - 14;
				break;

			case NdisMediumIrda:
			case NdisMediumWirelessWan:
			case NdisMediumLocalTalk:
				pAdapter->MaximumLookahead = GenericUlong;
				break;
			default:
				pAdapter->MaximumLookahead = NDIS_M_MAX_LOOKAHEAD;
			}
		}
		
		if ( pAdapter->CurrentLookahead == 0 )
		{
			//
			// Get CurrentLookahead
			//
			pAdapter->CurrentLookahead = pAdapter->MaximumLookahead;
			if ((NdisMediumWan != MediaType) && ndisMediaTypeCl[MediaType])
			{
				//
				// Query maximum lookahead
				//
				nStatus = 
					NDIS_MQueryOid(
						pAdapter,
						OID_GEN_CURRENT_LOOKAHEAD,
						&GenericUlong,
						sizeof(GenericUlong)
						);
				if ( nStatus != NDIS_STATUS_SUCCESS )
				{
					DBGLOG(( LError, "%08X = NDIS_MQueryOid(OID_GEN_MAXIMUM_LOOKAHEAD) failed\n", nStatus ));
					//break;
				}else{
					pAdapter->CurrentLookahead = GenericUlong;
				}

				if ( pAdapter->CurrentLookahead >= pAdapter->MaximumLookahead ){
					pAdapter->CurrentLookahead = pAdapter->MaximumLookahead;
				}
			}
		}

		//
		// Set MacOptions to 0 as default. For lan media query the real
		// stuff.  We also need to call this for lan drivers.
		//
		if ( ndisMediaTypeCl[MediaType] || (NdisMediumWan == MediaType))
		{
			if ( pAdapter->MacOptions == 0 )
			{
				//
				// Query mac options
				//
				nStatus = 
					NDIS_MQueryOid(
						pAdapter,
						OID_GEN_MAC_OPTIONS,
						&GenericUlong,
						sizeof(GenericUlong)
						);

				if ( nStatus != NDIS_STATUS_SUCCESS )
				{
					DBGLOG(( LError, "%08X = NDIS_MQueryOid(OID_GEN_MAC_OPTIONS) failed\n", nStatus ));
					//break;
				}
				pAdapter->MacOptions = (UINT)GenericUlong;
			}
			if ((pAdapter->MacOptions & (NDIS_MAC_OPTION_RESERVED | NDIS_MAC_OPTION_NDISWAN )) == (NDIS_MAC_OPTION_RESERVED | NDIS_MAC_OPTION_NDISWAN ))
			{
				pAdapter->MaximumLookahead = NDIS_M_MAX_LOOKAHEAD - 14;
			}

			//if we got the options, initialize our optimizing booleans
			if ( (pAdapter->MacOptions & NDIS_MAC_OPTION_TRANSFERS_NOT_PEND) == NDIS_MAC_OPTION_TRANSFERS_NOT_PEND )
			{
				pAdapter->Interface.bNdisTransferDataNeverCalled = TRUE;
			}

			if ( (pAdapter->MacOptions & NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA) == NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA )
			{
				pAdapter->Interface.bLookaheadDataMayBeUsed = TRUE;
			}

			if (pAdapter->Interface.bNdisTransferDataNeverCalled)
			{
				//TEST ONLY FOR RTL8029
				//Check if the lower miniport has it's own TransferData handler
				//if so, set bNdisTransferDataNeverCalled to FALSE 
				CHAR ns8029[] = "8029";
				CHAR nsRtl[] = "RTL";
				CHAR nsRealtek[] = "Realtek";

				//check if it is RTL 8029 network adapter; this f..cking driver sets 
				//NDIS_MAC_OPTION_TRANSFERS_NOT_PEND option, but still uses NdisTransferData
				if ((RtxFindSubstring(pAdapter->szVendorDescription,ns8029,TRUE))  &&
					((RtxFindSubstring(pAdapter->szVendorDescription,nsRealtek,TRUE))||(RtxFindSubstring(pAdapter->szVendorDescription,nsRtl,TRUE))))
				{
					pAdapter->Interface.bNdisTransferDataNeverCalled = FALSE;
				}
			}

			switch( MediaType )
			{
			case NdisMedium802_3:
				if ( pAdapter->Interface.CurrentMacAddress[0] == 0 && 
					 pAdapter->Interface.CurrentMacAddress[1] == 0 &&
					 pAdapter->Interface.CurrentMacAddress[2] == 0 && 
					 pAdapter->Interface.CurrentMacAddress[3] == 0 &&
					 pAdapter->Interface.CurrentMacAddress[4] == 0 && 
					 pAdapter->Interface.CurrentMacAddress[5] == 0 )
				{
					nStatus = 
						NDIS_MQueryOid(
							pAdapter,
							KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_IS_CO) ? OID_WAN_CURRENT_ADDRESS : OID_802_3_CURRENT_ADDRESS,
							pAdapter->Interface.CurrentMacAddress,
							sizeof(pAdapter->Interface.CurrentMacAddress)
							);

					if ( nStatus != NDIS_STATUS_SUCCESS )
					{
						DBGLOG(( LError, "%08X = NDIS_MQueryOid(OID_802_3_CURRENT_ADDRESS) failed\n", nStatus ));
						//break;
					}
				}
				break;
			case NdisMediumWan:
				if ( pAdapter->Interface.CurrentMacAddress[0] == 0 && 
					pAdapter->Interface.CurrentMacAddress[1] == 0 &&
					pAdapter->Interface.CurrentMacAddress[2] == 0 && 
					pAdapter->Interface.CurrentMacAddress[3] == 0 &&
					pAdapter->Interface.CurrentMacAddress[4] == 0 && 
					pAdapter->Interface.CurrentMacAddress[5] == 0 )
				{
					nStatus = 
						NDIS_MQueryOid(
							pAdapter,
							OID_WAN_CURRENT_ADDRESS,
							pAdapter->Interface.CurrentMacAddress,
							sizeof(pAdapter->Interface.CurrentMacAddress)
							);
					if ( nStatus != NDIS_STATUS_SUCCESS )
					{
						DBGLOG(( LError, "%08X = NDIS_MQueryOid(OID_WAN_CURRENT_ADDRESS) failed\n", nStatus ));
						//break;
					}
				}
			}
			// get MTU
			nStatus = 
				NDIS_MQueryOid(
					pAdapter,
					OID_GEN_MAXIMUM_FRAME_SIZE,
					&GenericUlong,
					sizeof(GenericUlong)
					);

			if ( nStatus == NDIS_STATUS_SUCCESS ){
				if ( GenericUlong > MAX_802_3_LENGTH ){
					pAdapter->Interface.MTU = MAX_802_3_LENGTH;
				}else{
					pAdapter->Interface.MTU = (USHORT)GenericUlong;
				}
			}else{
				DBGLOG(( LError, "%08X = NDIS_MQueryOid(OID_GEN_MAXIMUM_FRAME_SIZE) failed\n", nStatus ));
				pAdapter->Interface.MTU = MAX_802_3_LENGTH;
			}

			// maximum ethernet packet size
			nStatus = 
				NDIS_MQueryOid(
					pAdapter,
					OID_GEN_MAXIMUM_TOTAL_SIZE,
					&pAdapter->MaxFrameSize,
					sizeof(pAdapter->MaxFrameSize)
					);

			if ( nStatus == NDIS_STATUS_SUCCESS ){
				if ( pAdapter->MaxFrameSize > MAX_ETHER_SIZE ){
					pAdapter->MaxFrameSize = MAX_ETHER_SIZE;
				}
			}else{
				DBGLOG(( LError, "%08X = NDIS_MQueryOid(OID_GEN_MAXIMUM_TOTAL_SIZE) failed\n", nStatus ));
				pAdapter->MaxFrameSize = MAX_ETHER_SIZE;
			}

			// maximum send packets
			//nStatus = 
				NDIS_MQueryOid(
					pAdapter,
					OID_GEN_MAXIMUM_SEND_PACKETS,
					&pAdapter->MaxSendPackets,
					sizeof(pAdapter->MaxSendPackets)
					);

			if ( pAdapter->Interface.Speed == 0 )
			{
				//get link speed
				nStatus = 
					NDIS_MQueryOid(
						pAdapter,
						OID_GEN_LINK_SPEED,
						&pAdapter->Interface.Speed,
						sizeof(pAdapter->Interface.Speed)
						);

				if ( nStatus == NDIS_STATUS_SUCCESS ){
					pAdapter->Interface.Speed *= 100L;
				}
			}
			if ( pAdapter->PacketFilter == 0 )
			{
				//get packet filter
				//nStatus = 
					NDIS_MQueryOid(
						pAdapter,
						OID_GEN_CURRENT_PACKET_FILTER,
						&pAdapter->PacketFilter,
						sizeof(pAdapter->PacketFilter)
						);	
			}

			nStatus = NDIS_STATUS_SUCCESS;
		}

		if ( nStatus == NDIS_STATUS_SUCCESS ){
			DBGLOG(( LDebug, "New adapter:  \n" \
				"MediaType=%u\n" \
				"HWAddress=%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n" \
				"HeaderSize=%u\n" \
				"MTU=%u\n" \
				"MinFrameSize=%u\n" \
				"MaxFrameSize=%u\n" \
				"MaxSendPackets=%u\n" \
				"MacOptions=%08X\n" \
				"Speed=%u\n" \
				"PacketFilter=%08X\n" \
				"CurrentLookahead=%08X\n",
				pAdapter->Interface.MediaType,
				pAdapter->Interface.CurrentMacAddress[0], 
				pAdapter->Interface.CurrentMacAddress[1],
				pAdapter->Interface.CurrentMacAddress[2], 
				pAdapter->Interface.CurrentMacAddress[3],
				pAdapter->Interface.CurrentMacAddress[4],
				pAdapter->Interface.CurrentMacAddress[5],
				(unsigned int)pAdapter->HeaderSize,
				(unsigned int)pAdapter->Interface.MTU,
				pAdapter->MinFrameSize,
				pAdapter->MaxFrameSize,
				pAdapter->MaxSendPackets,
				pAdapter->MacOptions,
				pAdapter->Interface.Speed,
				pAdapter->PacketFilter,
				pAdapter->CurrentLookahead
				));
		}

	}while ( FALSE );

	return nStatus;
}

//***********************************************************************************
// Name: MpHalt
//
// Description: 
//		Halts adapter and releases  resources
//
// Return value: 
//		None.
//
// Parameters: 
//
// NOTE: Adapter is not valid after this function call!!!
// **********************************************************************************
VOID
	MpHalt(
		IN PKIP_NDIS_ADAPTER pAdapter
		)
{
	PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;
	DBGLOG(( LDebug, "Unbinding adapter\n", pAdapter ));
	netifapi_netif_set_down(&pProtocol->IIF,1);

	// complete pending operations
	while ( pProtocol->OutstandingSends ){
		// wait a little
		NdisMSleep(20000);
	}

	// complete pending operations
	while ( pProtocol->PendingPackets ){
		// wait a little
		NdisMSleep(20000);
	}

	// unbind protocol first
	ProtocolUnBindAdapter( 
		pAdapter->Interface.Miniport,
		&pAdapter->Interface,
		pProtocol
		);

	// free any resources
	NDIS_FreeAdapter( pAdapter );
}
//***********************************************************************************
// Name: MpSendPacketHandler
//
// Description: 
//		Sends 1 ndis5 packet handler to NIC
//
// Return value: 
//		None.
//
// Parameters: 
//
// NOTE: none
// **********************************************************************************
NDIS_STATUS
	MpSendPacketHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_PACKET Packet
		)
{
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
	PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;
	NDIS_STATUS nStatus;

	// miniport halted or paused
	if ( KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_RESET_IN_PROGRESS )){
		MpSendPacketCompleteHandler( pAdapter, Packet, NDIS_STATUS_RESET_IN_PROGRESS );
		return NDIS_STATUS_RESET_IN_PROGRESS;
	}

	InterlockedIncrement( &pProtocol->OutstandingSends );

	if ( KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_SG_LIST) ){
		nStatus = MpAllocSGList( pAdapter, Packet );
		// if not pending then complete it
		if ( nStatus != NDIS_STATUS_SUCCESS )
		{
			MpSendPacketCompleteHandler( pAdapter, Packet, NDIS_STATUS_FAILURE );
		}
		return nStatus;
	}

	//
	// HANDLE loopback
	//
	if (((NdisGetPacketFlags(Packet) & NDIS_FLAGS_DONT_LOOPBACK) == 0) ||
		(pAdapter->MacOptions & NDIS_MAC_OPTION_NO_LOOPBACK))
	{
		// TODO: loopbacks support
		//ndisMLoopbackPacketX(Miniport, Packet);
	}
	
	if ( pMiniport->Ndis5.MiniportCharacteristics.SendPacketsHandler )
	{
		pMiniport->Ndis5.MiniportCharacteristics.SendPacketsHandler(
			pAdapter->NdisMiniportAdapterContext,
			&Packet, 1
			);
	} 
	else if ( pMiniport->Ndis5.MiniportCharacteristics.SendHandler ){
		nStatus = 
			pMiniport->Ndis5.MiniportCharacteristics.SendHandler(
				pAdapter->NdisMiniportAdapterContext,
				Packet, 0
				);

		// if not pending then complete it
		if (nStatus != NDIS_STATUS_PENDING)
		{
			MpSendPacketCompleteHandler( pAdapter, Packet, nStatus );
		}
	}
	return NDIS_STATUS_PENDING;
}

//***********************************************************************************
// Name: MpSendPacketCompleteHandler
//
// Description: 
//		Handles adapter packet sending completion
// Return value: 
//		TRUE, if we own this packet
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
BOOLEAN
	MpSendPacketCompleteHandler(
		IN  PKIP_NDIS_ADAPTER		pAdapter,
		IN  PNDIS_PACKET            Packet,
		IN  NDIS_STATUS             Status
		)
{
	PSCATTER_GATHER_LIST pSGL = NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, ScatterGatherListPacketInfo);
	PKIP_PACKET_CONTEXT PacketContext = NDIS_PER_PACKET_CONTEXT(Packet);
	PKIP_NDIS_PROTOCOL pProtocol;
	BOOLEAN fbResult;

	if ( PacketContext->pAdapter != pAdapter ){
		return FALSE;
	}
	// pass it to protocol
	pProtocol = PacketContext->pProtocol;
	fbResult = 
		ProtocolSendPacketComplete( 
			pProtocol, 
			Packet, 
			Status 
			);
	if ( fbResult ){

		if (KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_SG_LIST) && pSGL != NULL)
		{
			MpFreeSGList(pAdapter, Packet);
		}

		InterlockedDecrement ( &pProtocol->OutstandingSends );
	}
	return fbResult;
}

NDIS_STATUS
	MpIndicatePacket(
		IN  PKIP_NDIS_ADAPTER pAdapter,
		IN  PNDIS_PACKET Packet
		)
{
	NDIS_STATUS nStatus;
	if ( KIP_MINIPORT_TEST_FLAG(pAdapter,fADAPTER_IS_CO) == FALSE ){
		PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;
		nStatus = 
			ProtocolReceivePacket( 
				pProtocol, 
				Packet
				);
	}else{
		PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;
		nStatus = 
			ProtocolWanReceive(
				pAdapter,
				pProtocol, 
				Packet
				);
	}
	return nStatus;
}
//***********************************************************************************
// Name: MpIndicatePacketsHandler
//
// Description: 
//		Handles incoming packets. It passes packets to protocol, that processes the packet
//		If we own the packet, ndis layer frees it and fixes packet array and number
// Return value: 
//		Number of handled packets
//
// Parameters: 
//	PacketArray - array of NDIS5 packets, on output we drop all packets that we own
//  NumberOfPackets - the number of indicated packets, on output 
//      NumberOfPackets = NumberOfPackets - number_of_processed_packets
//
// NOTE: None
// **********************************************************************************
INT
	MpIndicatePacketsHandler(
		IN  PKIP_NDIS_ADAPTER		pAdapter,
		IN  PPNDIS_PACKET           *ppPacketArray,
		IN  PUINT                   pNumberOfPackets
		)
{
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
	PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;
	NDIS_STATUS nStatus;
	PPNDIS_PACKET PacketArray = *ppPacketArray;
	PNDIS_PACKET Packet;
	unsigned int nCount = *pNumberOfPackets;
	unsigned int i = 0, j = 0;

	PPNDIS_PACKET NewPacketArray;
	INT nProcessed = 0;

	NewPacketArray = 
		ExAllocatePoolWithTag(
			NonPagedPool,
			sizeof(PNDIS_PACKET)*nCount,
			MODULE_TAG
			);

	if ( NewPacketArray ){
		for ( i = 0; i < nCount; ++i )
		{
			Packet = PacketArray[i];
			nStatus = 
				MpIndicatePacket( 
					pAdapter, 
					Packet
					);

			if ( nStatus == NDIS_STATUS_NOT_ACCEPTED ){

				//
				// it's not ours, pass it thru
				//
				NewPacketArray[j++] = Packet;

			} else { //	if ( nStatus == NDIS_STATUS_SUCCESS or not ){

				//
				// protocol has processed the packet
				//
				if ( KIP_MINIPORT_TEST_FLAG(pAdapter,fADAPTER_DESERIALIZE ) ){
					if ( NDIS_GET_PACKET_STATUS(Packet) != NDIS_STATUS_RESOURCES ){
						NDIS_ReturnPacket(pAdapter,Packet);
					}
				}else{
					// return packet to miniport
					NDIS_SET_PACKET_STATUS(Packet,NDIS_STATUS_SUCCESS);
				}
				nProcessed ++;
			} 
		}
		// pass packets down 
		if ( j < nCount ){
			*ppPacketArray = NewPacketArray;
			*pNumberOfPackets = j;
		}else{
			ExFreePool( NewPacketArray );
		}
	}
	return nProcessed;
}

//***********************************************************************************
// Name: MpIndicateEthReceiveHandler
//
// Description: 
//		handles incoming data
//
// Return value: 
//		None.
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpIndicateEthReceiveHandler(
		IN PKIP_NDIS_ADAPTER       pAdapter,
		IN NDIS_HANDLE             MacReceiveContext,
		IN PVOID                   HeaderBuffer,
		IN UINT                    HeaderBufferSize,
		IN PVOID                   LookaheadBuffer,
		IN UINT                    LookaheadBufferSize,
		IN UINT                    PacketSize,
		IN PBOOLEAN                CallOrig
		)
{
	PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;
	NDIS_STATUS nStatus;
	ULONG nDataSize = 0;
	ULONG nResidualSize = 0;

	*CallOrig = TRUE;
	do 
	{
		if ( HeaderBufferSize != SIZEOF_ETH_HDR )
		{
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			break;
		}

		nDataSize = HeaderBufferSize + PacketSize;

		/*
		*	Sanity checks
		*/
		if ( nDataSize > MAX_ETHER_SIZE ){
			nStatus = NDIS_STATUS_NOT_ACCEPTED;
			break;
		}

		//
		// Sanity Check On LookaheadBufferSize
		// -----------------------------------
		// At the September, 1998 Networking And Communications Conference at
		// Microsoft a comment was made that some MAC drivers incorrectly included
		// HeaderBufferSize in the LookaheadBufferSize. If this is true, then
		// using LookaheadBufferSize as the length to move could result in
		// writing beyond the end of the W32N_READ_UNIT buffer.
		//
		// The following forces LookaheadBufferSize to be no longer then
		// PacketSize.
		//
		if( LookaheadBufferSize > PacketSize )
		{
			LookaheadBufferSize = PacketSize;
		}

		//
		// Handle NDIS_MAC_OPTION_TRANSFERS_NOT_PEND
		// -----------------------------------------
		// A lower miniport that sets this option never may not have a
		// MiniportTransferData function. For these miniports LookaheadBufferSize
		// should be equal to PacketSize. However, some adapter drivers have
		// been observed that violate this.
		//
		// Here we force LookaheadBufferSize = PacketSize if the ..._NOT_PEND
		// option is set. This insures that subsequent code will not call
		// NdisTransferData and cause a fault.
		//
		if( pAdapter->Interface.bNdisTransferDataNeverCalled )
		{
			//MICHAEL: the comments above seems to be wrong!!!  
			//TODO... more research needed
			// see htp://www.pcausa.com/support/KB03080201.htm
			//ASSERT(PacketSize==LookaheadBufferSize);

			LookaheadBufferSize = PacketSize;
		}

		// cal residual buffer size
		if ( LookaheadBufferSize < PacketSize )
		{
			// Some old adapters can't indicate the whole packet
			// so we need calling TransferDataHandler for them
			nResidualSize = PacketSize - LookaheadBufferSize;
		}

		nStatus =
			ProtocolReceive(
				pProtocol,
				MacReceiveContext,
				HeaderBuffer,
				HeaderBufferSize,
				LookaheadBuffer,
				LookaheadBufferSize,
				nResidualSize,
				PacketSize
				);
	} while ( FALSE );

	if ( nStatus == NDIS_STATUS_NOT_ACCEPTED ){
		*CallOrig = TRUE;
	}else{
		*CallOrig = FALSE;
	}

	return;
}

//***********************************************************************************
// Name: MpTransferDataComplete
//
// Description: 
//		handles data transfer completion. we pass packet to protocol
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
BOOLEAN 
	MpTransferDataComplete(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_PACKET  Packet,
		IN NDIS_STATUS  Status,
		IN UINT  BytesTransferred,
		OUT PBOOLEAN CallOrig
		)
{
	NDIS_STATUS nStatus;
	PNDIS_PACKET_EX	MyPacket;
	PLIST_ENTRY  ListEntry;
	PNDIS_BUFFER pResidualBuffer;
	FILTER_RESULT FilterResult = FILTER_PASS;
	PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;

	*CallOrig = TRUE;

	NdisAcquireSpinLock ( &pProtocol->PendingTransfertLock );

	if (IsListEmpty ( &pProtocol->PendingTransfertData ) ){

		*CallOrig = TRUE;
		NdisReleaseSpinLock ( &pProtocol->PendingTransfertLock );
		return FALSE;
	}

	// We did calls to TransferDataHandler or directly to this handler
	// Find associated pending transfers data call
	for ( ListEntry = pAdapter->Interface.Protocol.PendingTransfertData.Flink; 
		ListEntry != &pAdapter->Interface.Protocol.PendingTransfertData;
		ListEntry = ListEntry->Flink )
	{
		MyPacket = CONTAINING_RECORD ( ListEntry, NDIS_PACKET_EX, Reserved.qLink );

		if ( (PNDIS_PACKET) MyPacket == Packet)
		{
			if ( Status != NDIS_STATUS_SUCCESS )
			{
				FilterResult = FILTER_DROP;
				break;
			}

			FilterResult = MyPacket->Reserved.FilterResult;
			// unlink the packet from list
			RemoveEntryList ( &MyPacket->Reserved.qLink );
			NdisReleaseSpinLock ( &pProtocol->PendingTransfertLock );
		}
		MyPacket = NULL;
	}

	// packet was not found
	// just pass it to original
	if ( MyPacket == NULL ){
		NdisReleaseSpinLock ( &pProtocol->PendingTransfertLock );
		*CallOrig = TRUE;
		return FALSE;
	}

	if ( FilterResult == FILTER_DROP ){
		// free packet and exit
		*CallOrig = FALSE;
		NDIS_FreePacket( pProtocol, Packet );
		return TRUE; // processed
	}

	// fixup data length
	MyPacket->Reserved.DataLength += BytesTransferred;

	// Reorder buffers in packet (call to transfer data
	// copies remain of data to the first buffer, but the beginning
	// of the packet is in the second)
	NdisUnchainBufferAtFront((PNDIS_PACKET)MyPacket, &pResidualBuffer);
	NdisChainBufferAtBack ((PNDIS_PACKET)MyPacket, pResidualBuffer);

	//
	// Force protocols above to make a copy if they want to hang
	// on to data in this packet. This is because we are in our
	// Receive handler (not ReceivePacket) and we can't return a
	// ref count from here.
	//
	NDIS_SET_PACKET_STATUS((PNDIS_PACKET)MyPacket, NDIS_STATUS_RESOURCES); 

	// process it by protocol
	nStatus = 
		ProtocolReceiveInternal( 
			pProtocol, 
			MyPacket 
			);

	if ( nStatus == NDIS_STATUS_NOT_ACCEPTED ){
		//indicate packet to ndis
		NDIS_IndicatePacket(pAdapter,MyPacket);
		// reclaim the packet on return
		NDIS_FreePacket( pProtocol, (PNDIS_PACKET)MyPacket );
	}

	return TRUE; // processed
}

//***********************************************************************************
// Name: MpReturnPacketHandler
//
// Description: 
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
	MpReturnPacketHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_PACKET *pPacket,
		IN PBOOLEAN CallOrig
		) 
{
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
	PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;
	PNDIS_PACKET MyPacket = *pPacket;
	PNDIS_PACKET OrigPacket;
	//
	// process packet by our protocol
	//
	OrigPacket = 
		ProtocolReturnPacketHandler( 
			pProtocol, 
			MyPacket 
			);

	// complete the original packet if needed
	if ( OrigPacket ){
		*pPacket = OrigPacket;
		*CallOrig = TRUE;
	}else{
		*CallOrig = FALSE;
	}
}
//***********************************************************************************
// Name: MpIndicateStatusHandler 
//
// Description: 
//		processes underlaying miniport status indication
//
// Return value: 
//		TRUE, if this is swap handler indication
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpIndicateStatusHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_STATUS  GeneralStatus,
		IN PVOID  StatusBuffer,
		IN UINT  StatusBufferSize,
		IN BOOLEAN BeforeMiniport
		)
{
	PNDIS_MINIPORT_BLOCK pNdisMiniport = (PNDIS_MINIPORT_BLOCK)pAdapter->NdisMiniportHandle;
	KIRQL OldIrql;
	BOOLEAN fbSwap = FALSE;
	BOOLEAN fbInternal = FALSE;

	if ( BeforeMiniport ){
		switch ( GeneralStatus ) {

		case NDIS_STATUS_WAN_LINE_UP:
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_IS_CO);
			break;

		case NDIS_STATUS_WAN_LINE_DOWN:
			DBGLOG(( LTrace, "NDIS_STATUS_WAN_LINE_DOWN\n", pAdapter ));
			WAN_RemoveRasLink( pAdapter, (PNDIS_WAN_LINE_DOWN)StatusBuffer );
			break;

		case NDIS_STATUS_WWAN_CONTEXT_STATE:
			{
				PNDIS_WWAN_CONTEXT_STATE pContextState = (PNDIS_WWAN_CONTEXT_STATE)StatusBuffer;
				switch(pContextState->ContextState.ActivationState)
				{
					case WwanActivationStateActivated:
						break;
					case WwanActivationStateDeactivated:
						WAN_WwanDeactivated(pAdapter,pContextState);
						break;
				} // switch(pContextState->ContextState.ActivationState){
				break;
			}
		case NDIS_STATUS_MEDIA_DISCONNECT:
			DBGLOG(( LTrace, "NDIS_STATUS_MEDIA_DISCONNECT\n", pAdapter ));

			KIP_LOCK_ADAPTER(pAdapter);
			InterlockedIncrement ( &pAdapter->Interface.MediaConnectCount );
			KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);

			if ( !fbInternal )
			{
				//
				// miniport can do media sense and indicate status
				//
				KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_REQUIRES_MEDIA_POLLING);
				KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_SUPPORTS_MEDIA_SENSE);
			}
			KIP_UNLOCK_ADAPTER(pAdapter);

			// update lwip status
			netifapi_netif_set_down(&pAdapter->Interface.Protocol.IIF,0);
			break;
		case NDIS_STATUS_LINK_STATE:
			{
				PNDIS_LINK_STATE LinkState = (PNDIS_LINK_STATE)StatusBuffer;
				if ( StatusBufferSize >= sizeof(NDIS_LINK_STATE) ){
					DBGLOG(( LTrace, "NDIS_STATUS_LINK_STATE\n", pAdapter ));

					if ( LinkState->MediaConnectState == MediaConnectStateConnected )
					{
						// after orig
					}
					else 
					{
						KIP_LOCK_ADAPTER(pAdapter);
						InterlockedIncrement ( &pAdapter->Interface.MediaConnectCount );
						KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);
						KIP_UNLOCK_ADAPTER(pAdapter);

						netifapi_netif_set_down(&pAdapter->Interface.Protocol.IIF,0);
					}				
				}
			}
		}
	}
	else
	{
		//
		// Internal indications are media-sense indications. These are detected by
		// a StatusBufferSize of -1 and StatusBuffer of NULL.
		//
		if ((GeneralStatus == NDIS_STATUS_MEDIA_CONNECT) || (GeneralStatus == NDIS_STATUS_MEDIA_DISCONNECT))
		{
			fbInternal = ((StatusBufferSize == INTERNAL_INDICATION_SIZE) && (StatusBuffer == INTERNAL_INDICATION_BUFFER));
		}

		switch ( GeneralStatus )
		{
		case NDIS_STATUS_ONLINE:
			DBGLOG(( LTrace, "NDIS_STATUS_ONLINE\n", pAdapter ));
			break;
		case NDIS_STATUS_RESET_START:
			DBGLOG(( LTrace, "NDIS_STATUS_RESET_START\n", pAdapter ));
			break;
		case NDIS_STATUS_RESET_END:
			DBGLOG(( LTrace, "NDIS_STATUS_RESET_END\n", pAdapter ));
			break;
		case NDIS_STATUS_RING_STATUS:
			DBGLOG(( LTrace, "NDIS_STATUS_RING_STATUS\n", pAdapter ));
			break;
		case NDIS_STATUS_CLOSED:
			DBGLOG(( LTrace, "NDIS_STATUS_CLOSED\n", pAdapter ));
			break;
		case NDIS_STATUS_MEDIA_CONNECT:
			DBGLOG(( LTrace, "NDIS_STATUS_MEDIA_CONNECT\n", pAdapter ));

			KIP_LOCK_ADAPTER(pAdapter);
			InterlockedIncrement ( &pAdapter->Interface.MediaConnectCount );
			//if (!KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED))
			//{
				fbSwap = TRUE;
			//}
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);
			if (!fbInternal)
			{
				//
				// miniport can do media sense and can indicate that status to Ndis. Do not poll
				//
				KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_REQUIRES_MEDIA_POLLING);
				KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_SUPPORTS_MEDIA_SENSE);
			}
			KIP_UNLOCK_ADAPTER(pAdapter);

			// update lwip status
			netifapi_netif_set_up(&pAdapter->Interface.Protocol.IIF,0);
			break;
		case NDIS_STATUS_MEDIA_DISCONNECT:
			//DBGLOG(( LTrace, "NDIS_STATUS_MEDIA_DISCONNECT\n", pAdapter ));
			fbSwap = TRUE;
			break;
		case NDIS_STATUS_LINK_STATE:
			{
				PNDIS_LINK_STATE LinkState = (PNDIS_LINK_STATE)StatusBuffer;
				if ( StatusBufferSize >= sizeof(NDIS_LINK_STATE) ){
					DBGLOG(( LTrace, "NDIS_STATUS_LINK_STATE\n", pAdapter ));
					
					if ( LinkState->MediaConnectState == MediaConnectStateConnected )
					{
						KIP_LOCK_ADAPTER(pAdapter);
						pAdapter->Interface.Speed = (ULONG)LinkState->XmitLinkSpeed;
						InterlockedIncrement ( &pAdapter->Interface.MediaConnectCount );
						KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);
						KIP_UNLOCK_ADAPTER(pAdapter);

						netifapi_netif_set_up(&pAdapter->Interface.Protocol.IIF,0);
					}
					else 
					{
						// before orig
					}				
				}
			}
			break;
		case NDIS_STATUS_HARDWARE_LINE_UP:
			DBGLOG(( LTrace, "NDIS_STATUS_HARDWARE_LINE_UP\n", pAdapter ));
			break;
		case NDIS_STATUS_HARDWARE_LINE_DOWN:
			DBGLOG(( LTrace, "NDIS_STATUS_HARDWARE_LINE_DOWN\n", pAdapter ));
			break;
		case NDIS_STATUS_INTERFACE_UP:
			DBGLOG(( LTrace, "NDIS_STATUS_INTERFACE_UP\n", pAdapter ));
			break;
		case NDIS_STATUS_INTERFACE_DOWN:
			DBGLOG(( LTrace, "NDIS_STATUS_INTERFACE_DOWN\n", pAdapter ));
			break;
		case NDIS_STATUS_WAN_LINE_UP:
			{
				DBGLOG(( LTrace, "NDIS_STATUS_WAN_LINE_UP\n", pAdapter ));
				fbSwap = TRUE;
				WAN_AddRasLink( pAdapter, (PNDIS_WAN_LINE_UP)StatusBuffer );
			}
			break;
		case NDIS_STATUS_WAN_LINE_DOWN:
			{
				//DBGLOG(( LTrace, "NDIS_STATUS_WAN_LINE_DOWN\n", pAdapter ));
				fbSwap = TRUE;			
			}
			break;
		case NDIS_STATUS_WAN_FRAGMENT:
			DBGLOG(( LTrace, "NDIS_STATUS_WAN_FRAGMENT\n", pAdapter ));
			break;
		case NDIS_STATUS_MEDIA_BUSY:
			DBGLOG(( LTrace, "NDIS_STATUS_MEDIA_BUSY\n", pAdapter ));
			break;
		case NDIS_STATUS_MEDIA_SPECIFIC_INDICATION:
			DBGLOG(( LTrace, "NDIS_STATUS_MEDIA_SPECIFIC_INDICATION\n", pAdapter ));
			break;
		case NDIS_STATUS_LINK_SPEED_CHANGE:
			DBGLOG(( LTrace, "NDIS_STATUS_LINK_SPEED_CHANGE\n", pAdapter ));
			break;
		case NDIS_STATUS_WAN_GET_STATS:
			DBGLOG(( LTrace, "NDIS_STATUS_WAN_GET_STATS\n", pAdapter ));
			break;
		case NDIS_STATUS_WAN_CO_FRAGMENT:
			DBGLOG(( LTrace, "NDIS_STATUS_WAN_CO_FRAGMENT\n", pAdapter ));
			break;
		case NDIS_STATUS_WAN_CO_LINKPARAMS:
			DBGLOG(( LTrace, "NDIS_STATUS_WAN_CO_LINKPARAMS\n", pAdapter ));
			break;
		case NDIS_STATUS_WAN_CO_MTULINKPARAMS:
			DBGLOG(( LTrace, "NDIS_STATUS_WAN_CO_MTULINKPARAMS\n", pAdapter ));
			break;
		case NDIS_STATUS_WWAN_CONTEXT_STATE:
			DBGLOG(( LTrace, "NDIS_STATUS_WWAN_CONTEXT_STATE\n", pAdapter ));
			break;
		default:
			DBGLOG(( LTrace, "Status=%08X\n", GeneralStatus ));
			break;
		}
	}

	if ( fbSwap && pAdapter->Interface.Miniport->MajorNdisVersion < 6 ){
		NdisReinstallMiniportHook( pAdapter );
	}
}
//***********************************************************************************
// Name: MpQueryInformationHandler 
//
// Description: 
//		processes request to set miniport parameters
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	MpSetInformationHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_SET_INFORMATION Request,
		OUT PBOOLEAN         CallOrig
		)
{
	*CallOrig = TRUE;
	return NDIS_STATUS_SUCCESS;
}
//***********************************************************************************
// Name: MpSetInformationComplete 
//
// Description: 
//		Handles the completed set information request
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpSetInformationComplete(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_OID Oid,
		IN PVOID InformationBuffer,
		IN ULONG InformationBufferLength,
		IN NDIS_STATUS Status
		)
{
	NDIS_DEVICE_POWER_STATE NewDeviceState;
	ULONG GenericUlong = 0;

	// sanity checks
	if ( Status == NDIS_STATUS_SUCCESS )
	{
		switch ( Oid )
		{
		case OID_PNP_SET_POWER:
			NewDeviceState = (*(PNDIS_DEVICE_POWER_STATE)InformationBuffer);

			//
			// Check for invalid device state
			//
			if ((pAdapter->MPDeviceState > NdisDeviceStateD0) && (NewDeviceState != NdisDeviceStateD0))
			{
				break;
			}    

			//
			// Is the miniport transitioning from an On (D0) state to an Low Power State (>D0)
			// If so, then set the StandingBy Flag - (Block all incoming requests)
			//
			if (pAdapter->MPDeviceState == NdisDeviceStateD0 && NewDeviceState > NdisDeviceStateD0)
			{
				pAdapter->StandingBy = TRUE;
			}

			//
			// If the miniport is transitioning from a low power state to ON (D0), then clear the StandingBy flag
			// All incoming requests will be pended until the physical miniport turns ON.
			//
			if (pAdapter->MPDeviceState > NdisDeviceStateD0 &&  NewDeviceState == NdisDeviceStateD0)
			{
				pAdapter->StandingBy = FALSE;
			}
			break;
		case OID_GEN_CURRENT_LOOKAHEAD:
			GenericUlong = (*(PULONG)InformationBuffer);
			pAdapter->CurrentLookahead = GenericUlong;
			break;
		default:
			break;
		}
	}
}
//***********************************************************************************
// Name: MpSetInformationCompleteHandler 
//
// Description: 
//		indicates that a prior call to MiniportSetInformation, which returned 
//		NDIS_STATUS_PENDING, has completed.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpSetInformationCompleteHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_SET_INFORMATION Request,
		IN NDIS_STATUS       Status,
		OUT PBOOLEAN         CallOrig
		)
{
	NDIS_DEVICE_POWER_STATE NewDeviceState;
	ULONG GenericUlong = 0;

	// always call original
	*CallOrig = TRUE;

	MpSetInformationComplete(
		pAdapter,
		Request->Oid,
		Request->InformationBuffer,
		Request->InformationBufferLength,
		Status
		);

	if ( pAdapter->Interface.SetInformationRequest.Internal ){
		// set request completion event
		pAdapter->Interface.SetInformationRequest.Status = Status;
		NdisSetEvent( &pAdapter->Interface.SetInformationRequest.Event );
	}
}
//***********************************************************************************
// Name: MpQueryInformationHandler 
//
// Description: 
//		processes miniport information querying. We call it after original function
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	MpQueryInformationHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_QUERY_INFORMATION Request,
		OUT PBOOLEAN         CallOrig
		)
{
	NDIS_STATUS nStatus = NDIS_STATUS_SUCCESS;
	switch ( Request->Oid )
	{
	case OID_GEN_SUPPORTED_GUIDS:
		//
		//  Do not forward this, otherwise we will end up with multiple
		//  instances of private GUIDs that the underlying miniport
		//  supports.
		//
		*CallOrig = FALSE;
		nStatus = NDIS_STATUS_NOT_SUPPORTED;
		break;
	case OID_TCP_TASK_OFFLOAD:
		//
		// Fail this -if- this driver performs data transformations
		// that can interfere with a lower driver's ability to offload
		// TCP tasks.
		//
		*CallOrig = FALSE;
		nStatus = NDIS_STATUS_NOT_SUPPORTED;
		break;
	}
	return NDIS_STATUS_SUCCESS;
}
//***********************************************************************************
// Name: MpQueryInformationComplete 
//
// Description: 
//		Filters completed ndis request (ndis5 and 6)
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpQueryInformationComplete(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN NDIS_OID Oid,
		IN PVOID InformationBuffer,
		IN ULONG InformationBufferLength,
		IN NDIS_STATUS Status
		)
{
	ULONG   ConnectionStatus;
	PUSHORT pMTU;
	BOOLEAN fbSwap = FALSE;

	// sanity checks
	if ( Status == NDIS_STATUS_SUCCESS ){

		switch ( Oid )
		{
		case OID_GEN_MEDIA_CONNECT_STATUS:
		//case OID_GEN_MEDIA_CONNECT_STATUS_EX:
			ConnectionStatus = *((PULONG)InformationBuffer);
			switch( ConnectionStatus )
			{
			case NdisMediaStateConnected:
				if ( !KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED) ){
					fbSwap = TRUE;
				}
				KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);
				break;
			case NdisMediaStateDisconnected:
				if ( KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED) ){
					fbSwap = TRUE;
				}
				KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);
				break;
			}

			if ( fbSwap )
			{
				PNDIS_MINIPORT_BLOCK pNdisMiniport = (PNDIS_MINIPORT_BLOCK)pAdapter->NdisMiniportHandle;
				if ( pAdapter->Interface.Miniport->MajorNdisVersion < 6 ){
					NdisMIndicateStatus( 
						pAdapter->NdisMiniportHandle,
						ConnectionStatus == NdisMediaStateConnected ? NDIS_STATUS_MEDIA_CONNECT : NDIS_STATUS_MEDIA_DISCONNECT,
						NULL,0
						);
					//
					// If we got a connect/disconnect, swap open handlers
					//
					NdisReinstallMiniportHook( pAdapter );
				}
			}// if ( fbSwap )
			break;
		// We manually set maximum MTU as MAX_802_3_LENGTH bytes to avoid situation with 
		// 9000 byte packets like on some Gigabit Ethernet adapters possible.
		// An example such protocols, like IPSEC require adding information into packet
		// so lower MTU if you will need this
		case OID_GEN_MAXIMUM_FRAME_SIZE:
			{
				pMTU = (PUSHORT)InformationBuffer;
				if ( *pMTU > MAX_802_3_LENGTH )
					*pMTU = MAX_802_3_LENGTH;

				pAdapter->Interface.MTU = *pMTU; // Save MTU info in adapter relative storage
			}
			break;
		// Process OID_GEN_MAXIMUM_TOTAL_SIZE also, this is the same value as returned 
		// by OID_GEN_MAXIMUM_FRAME_SIZE plus packet header length
		case OID_GEN_MAXIMUM_TOTAL_SIZE:
			{
				pMTU = (PUSHORT)InformationBuffer;
				if ( *pMTU > MAX_ETHER_SIZE )
					*pMTU = MAX_ETHER_SIZE;

				pAdapter->MaxFrameSize = *pMTU;
			}
			break;
		// Save request result for OID_802_3_CURRENT_ADDRESS
		case OID_802_3_CURRENT_ADDRESS:
		case OID_WAN_CURRENT_ADDRESS:
			{
				NdisMoveMemory ( 
					pAdapter->Interface.CurrentMacAddress,
					InformationBuffer,
					ETHARP_HWADDR_LEN
					);
			}
			break;
		}
	}
}
//***********************************************************************************
// Name: MpQueryInformationCompleteHandler 
//
// Description: 
//		indicates that a prior call to MiniportQueryInformation, which returned 
//		NDIS_STATUS_PENDING, has completed.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpQueryInformationCompleteHandler(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_QUERY_INFORMATION Request,
		IN NDIS_STATUS       Status,
		OUT PBOOLEAN         CallOrig
		)
{
	ULONG   ConnectionStatus;
	PUSHORT pMTU;
	BOOLEAN fbSwap = FALSE;

	// always call original
	*CallOrig = TRUE;

	MpQueryInformationComplete(
		pAdapter,
		Request->Oid,
		Request->InformationBuffer,
		Request->InformationBufferLength,
		Status
		);

	// set request completion event
	if ( pAdapter->Interface.QueryInformationRequest.Internal ){
		pAdapter->Interface.QueryInformationRequest.Status = Status;
		NdisSetEvent( &pAdapter->Interface.QueryInformationRequest.Event );
	}
}

//***********************************************************************************
// Name: MpOidRequestComplete 
//
// Description: 
//		indicates that a prior call to MiniportOidRequest, which returned 
//		NDIS_STATUS_PENDING, has completed.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: Ndis6
// **********************************************************************************
VOID
	MpOidRequestComplete(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_OID_REQUEST  OidRequest,
		IN NDIS_STATUS       Status,
		OUT PBOOLEAN         CallOrig
		)
{
	if ( pAdapter->Interface.CurrentRequest && 
		(&pAdapter->Interface.CurrentRequest->OidRequest ==  OidRequest))
	{
		PNDIS_EVENT Event = &pAdapter->Interface.CurrentRequest->ReqEvent;
		pAdapter->Interface.CurrentRequest->Status = Status;
		pAdapter->Interface.CurrentRequest = NULL;
		NdisSetEvent(Event);
		*CallOrig = FALSE;
		return;
	}

	// always call original
	*CallOrig = TRUE;

	if ( OidRequest->RequestType ==  NdisRequestQueryInformation ){
		MpQueryInformationComplete(
			pAdapter,
			OidRequest->DATA.QUERY_INFORMATION.Oid,
			OidRequest->DATA.QUERY_INFORMATION.InformationBuffer,
			OidRequest->DATA.QUERY_INFORMATION.InformationBufferLength,
			Status
			);
	}else if ( OidRequest->RequestType ==  NdisRequestSetInformation ){
		MpSetInformationComplete(
			pAdapter,
			OidRequest->DATA.QUERY_INFORMATION.Oid,
			OidRequest->DATA.QUERY_INFORMATION.InformationBuffer,
			OidRequest->DATA.QUERY_INFORMATION.InformationBufferLength,
			Status
			);
	}
}
//***********************************************************************************
// Name: MpSetMiniportAttributes 
//
// Description: 
//		filterers miniport attributes
//
// Return value: 
//		operation status
//
// Parameters: 
//
// NOTE: Ndis6
// **********************************************************************************
NDIS_STATUS
	MpSetMiniportAttributes (
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNDIS_MINIPORT_ADAPTER_ATTRIBUTES  MiniportAttributes
		)
{
	switch( MiniportAttributes->GeneralAttributes.Header.Type )
	{
	case NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES:
		pAdapter->NdisMiniportAdapterContext = MiniportAttributes->RegistrationAttributes.MiniportAdapterContext;
		if ( MiniportAttributes->RegistrationAttributes.AttributeFlags & NDIS_MINIPORT_ATTRIBUTES_BUS_MASTER )
		{
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_BUS_MASTER);
		}
		break;
	case NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES:
		pAdapter->Interface.MediaType = MiniportAttributes->GeneralAttributes.MediaType;
		pAdapter->Interface.PhysicalMediumType = MiniportAttributes->GeneralAttributes.PhysicalMediumType;
		pAdapter->Interface.MTU = (USHORT)MiniportAttributes->GeneralAttributes.MtuSize;
		//MiniportAttributes->GeneralAttributes.MaxXmitLinkSpeed;
		pAdapter->Interface.Speed = (ULONG)MiniportAttributes->GeneralAttributes.MaxXmitLinkSpeed; //The transmit link speed of the adapter in bits per second
		//MiniportAttributes->GeneralAttributes.MaxRcvLinkSpeed;
		//MiniportAttributes->GeneralAttributes.RcvLinkSpeed;
		switch( MiniportAttributes->GeneralAttributes.MediaConnectState )
		{
		case MediaConnectStateConnected:
			KIP_MINIPORT_SET_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);
			break;
		case MediaConnectStateDisconnected:
			KIP_MINIPORT_CLEAR_FLAG(pAdapter, fADAPTER_MEDIA_CONNECTED);
			break;
		}
		//NDIS_MEDIA_DUPLEX_STATE         MediaDuplexState;
		pAdapter->CurrentLookahead = pAdapter->MaximumLookahead = 
			MiniportAttributes->GeneralAttributes.LookaheadSize;
		pAdapter->MacOptions = MiniportAttributes->GeneralAttributes.MacOptions;
		pAdapter->PacketFilter = MiniportAttributes->GeneralAttributes.SupportedPacketFilters;
		//ULONG                           MaxMulticastListSize;
		pAdapter->Interface.HWAddressLength = MiniportAttributes->GeneralAttributes.MacAddressLength;
		NdisMoveMemory(
			pAdapter->Interface.CurrentMacAddress,
			MiniportAttributes->GeneralAttributes.CurrentMacAddress,
			sizeof(pAdapter->Interface.CurrentMacAddress)
			);
		//NET_IF_ACCESS_TYPE              AccessType; // NET_IF_ACCESS_BROADCAST for a typical ethernet adapter
		//NET_IF_DIRECTION_TYPE           DirectionType; // NET_IF_DIRECTION_SENDRECEIVE for a typical ethernet adapter
		//NET_IF_CONNECTION_TYPE          ConnectionType; // IF_CONNECTION_DEDICATED for a typical ethernet adapter
		//NET_IFTYPE                      IfType; // IF_TYPE_ETHERNET_CSMACD for a typical ethernet adapter (regardless of speed)
		//BOOLEAN                         IfConnectorPresent; // RFC 2665 TRUE if physical adapter
		//ULONG                           SupportedStatistics; // use NDIS_STATISTICS_XXXX_SUPPORTED    
		//ULONG                           SupportedPauseFunctions; // IEEE 802.3 37.2.1
		//ULONG                           DataBackFillSize;
		//ULONG                           ContextBackFillSize;
		//PNDIS_OID                       SupportedOidList;
		//ULONG                           SupportedOidListLength;
		//ULONG                           AutoNegotiationFlags;
		break;
	case NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES:
		break;
	case NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES:
		break;
	}
	return NDIS_STATUS_SUCCESS;
}
//***********************************************************************************
// Name: MpSendSendNetBufferLists
//
// Description: 
//		Sends netbufer list to NIC
//
// Return value: 
//		None.
//
// Parameters: 
//
// NOTE: none
// **********************************************************************************
VOID
	MpSendSendNetBufferLists(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNET_BUFFER_LIST  NetBufferLists
		)
{
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
	NDIS_STATUS nStatus;

	// miniport halted or paused
	if ( KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_RESET_IN_PROGRESS )){
		MpSendNetBufferListsComplete(pAdapter,&NetBufferLists,0);
		return;
	}

	InterlockedIncrement( &pAdapter->Interface.Protocol.OutstandingSends );

	if ( KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_SG_LIST) ){
		//nStatus = MpAllocSGList( pAdapter, Packet );
		//// if not pending then complete it
		//if ( nStatus != NDIS_STATUS_SUCCESS )
		//{
		//	MpSendPacketCompleteHandler( pAdapter, Packet, NDIS_STATUS_FAILURE );
		//}
		// TODO: SG DMA
		//__debugbreak();
//		return nStatus;
		MpSendNetBufferListsComplete(pAdapter,&NetBufferLists,0);
		return;
	}

	//
	// HANDLE loopback
	//
	//if (((NdisGetPacketFlags(Packet) & NDIS_FLAGS_DONT_LOOPBACK) == 0) ||
	//	(pAdapter->MacOptions & NDIS_MAC_OPTION_NO_LOOPBACK))
	//{
	//	// TODO: loopbacks support
	//	//ndisMLoopbackPacketX(Miniport, Packet);
	//}

	pMiniport->Ndis6.MiniportCharacteristics.SendNetBufferListsHandler(
		pAdapter->NdisMiniportAdapterContext,
		NetBufferLists,
		0, // default  port
		0 // on send flags
		);
}
//***********************************************************************************
// Name: MpSendNetBufferListsComplete
//
// Description: 
//		Completes the adapter send operation
// Return value: 
//		TRUE, if we own this packet
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
BOOLEAN
	MpSendNetBufferListsComplete(
		IN  PKIP_NDIS_ADAPTER pAdapter,
		IN  PNET_BUFFER_LIST  *NetBufferLists,
		IN  ULONG  SendCompleteFlags
		)
{
	PNET_BUFFER_LIST pNBL = *NetBufferLists;
	PNET_BUFFER_LIST pCurrNBL = pNBL;
	PNET_BUFFER_LIST pPrevNBL = NULL;
	PNET_BUFFER_LIST pNextNBL = NULL;

	PKIP_PACKET_CONTEXT PacketContext;

	while ( pCurrNBL ){
		PKIP_PACKET_CONTEXT PacketContext = NDIS_PER_NBLIST_CONTEXT(pCurrNBL);

		if ( PacketContext->pAdapter == pAdapter ){
			PKIP_NDIS_PROTOCOL pProtocol = PacketContext->pProtocol;
			ASSERT( pCurrNBL->NdisPoolHandle == pProtocol->NdisNetBufferListPool );

			if ( pCurrNBL->NdisPoolHandle == pProtocol->NdisNetBufferListPool ){

				if ( pPrevNBL == NULL ){
					pNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
				}else{
					//unchain nbl
					NET_BUFFER_LIST_NEXT_NBL(pPrevNBL) = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
				}
				pNextNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);

				// reclaim the list
				ProtocolSendNetBufferListComplete(pProtocol,pCurrNBL);

				pCurrNBL = pNextNBL;
			}else{
				pPrevNBL = pCurrNBL;
				pCurrNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
			}
		}else{
			pPrevNBL = pCurrNBL;
			pCurrNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
		}
	}
	*NetBufferLists = pNBL;
	return TRUE;
}
//***********************************************************************************
// Name: MpIndicateReceiveNetBufferLists
//
// Description: 
//		Processes incoming NB lists
// Return value: 
//		TRUE, if we own this packet
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpIndicateReceiveNetBufferLists(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNET_BUFFER_LIST  *NetBufferLists,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN PULONG  NumberOfNetBufferLists,
		IN ULONG  ReceiveFlags
		)
{
	PNET_BUFFER_LIST pNBL = *NetBufferLists;
	ULONG  i, nNBL = *NumberOfNetBufferLists;
	NDIS_STATUS nStatus;
	BOOLEAN bDispatchLevel, bResources;

	PNET_BUFFER_LIST pCurrNBL = pNBL;
	PNET_BUFFER_LIST pPrevNBL = NULL;
	PNET_BUFFER_LIST pNextNBL = NULL;
	PKIP_NDIS_PROTOCOL pProtocol = &pAdapter->Interface.Protocol;

	bResources = NDIS_TEST_RECEIVE_FLAG(ReceiveFlags, NDIS_RECEIVE_FLAGS_RESOURCES);
	bDispatchLevel = NDIS_TEST_RECEIVE_AT_DISPATCH_LEVEL(ReceiveFlags);

	if(bResources){
		//
		// We must not break NET BUFFER LIST in such cases
		//
		for(pCurrNBL = pNBL,i = 0; 
			pCurrNBL != NULL && i < nNBL; 
			pCurrNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL), ++i )
		{
			nStatus = 
				ProtocolIndicateNetBufferList(
					pAdapter,
					pProtocol,
					pCurrNBL,
					PortNumber,
					ReceiveFlags
					);

			if ( nStatus == NDIS_STATUS_NOT_ACCEPTED )
			{
				// save Next 
				pNextNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
				NET_BUFFER_LIST_NEXT_NBL(pCurrNBL) = NULL;

				// indicate nbl to ndis
				NdisMIndicateReceiveNetBufferListsPtr(
					pAdapter->NdisMiniportHandle, 
					pCurrNBL, 
					PortNumber, 1, 
					ReceiveFlags
					);					

				// restore Next 
				NET_BUFFER_LIST_NEXT_NBL(pCurrNBL) = pNextNBL;
			}else{
				NET_BUFFER_LIST_STATUS(pCurrNBL) = nStatus;
			}
		}
		*NetBufferLists = NULL;
		*NumberOfNetBufferLists = 0;
		return;
	} // if(bResources)

	pCurrNBL = pNBL;
	
	while ( pCurrNBL && nNBL ){

		nStatus = 
			ProtocolIndicateNetBufferList(
				pAdapter,
				pProtocol,
				pCurrNBL,
				PortNumber,
				ReceiveFlags
				);
		if ( nStatus != NDIS_STATUS_NOT_ACCEPTED ){
			if ( pPrevNBL == NULL ){
				pNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
			}else{
				//unchain nbl
				NET_BUFFER_LIST_NEXT_NBL(pPrevNBL) = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
			}

			// return buffer list to miniport
			nNBL --;
			pNextNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
			NET_BUFFER_LIST_NEXT_NBL(pCurrNBL) = NULL;
			NDIS_ReturnNetBufferList(pAdapter,pCurrNBL,bDispatchLevel);
			pCurrNBL = pNextNBL;
		}else{
			pPrevNBL = pCurrNBL;
			pCurrNBL = NET_BUFFER_LIST_NEXT_NBL(pCurrNBL);
		}
	}
	*NetBufferLists = pNBL;
	*NumberOfNetBufferLists = nNBL;
}

//***********************************************************************************
// Name: MpIndicateBufferToNdis
//
// Description: 
//		indicates data buffer to ndis
// Return value: 
//		operation status.
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	MpIndicateDataBufferToNdis(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PNET_BUFFER NetBuffer,
		IN NDIS_PORT_NUMBER  PortNumber,
		IN ULONG  ReceiveFlags
		)
{
	NDIS_STATUS nStatus = NDIS_STATUS_FAILURE;
	PNET_BUFFER_LIST pNBL = NULL;
	PNET_BUFFER pNB = NULL;
	PMDL pCurrMdl = NULL;
	PVOID pCurrData;
	ULONG TotalPacketLength = 0;
	BOOLEAN fbResult;

	do {
		pNBL = NdisAllocateNetBufferListPtr(pAdapter->Interface.Protocol.NdisNetBufferListPool, 0, 0);
		if (pNBL == NULL){
			DBGLOG(( LError, "NdisAllocateNetBufferList failed\n" ));
			nStatus = NDIS_STATUS_RESOURCES;			
			break;
		}

		pNB = NET_BUFFER_LIST_FIRST_NB(pNBL);
		pCurrMdl = NET_BUFFER_CURRENT_MDL(pNB);
		NET_BUFFER_CURRENT_MDL_OFFSET(pNB) = 0;
		NET_BUFFER_DATA_OFFSET(pNB) = 0;
		NET_BUFFER_DATA_LENGTH(pNB) = 0;

		pNBL->SourceHandle = pAdapter->NdisMiniportHandle;

		// copy data if any
		pCurrData = (unsigned char*)MmGetSystemAddressForMdlSafe(pCurrMdl, NormalPagePriority);
		if(NULL == pCurrData){
			DBGLOG(( LError, "MmGetSystemAddressForMdlSafe failed\n" ));
			nStatus = NDIS_STATUS_RESOURCES;			
			break;
		}

		fbResult =
			NDIS_CopyNetBufferToBuffer( 
				NetBuffer, //src
				pCurrData, //dst
				NET_BUFFER_DATA_LENGTH(NetBuffer), // maximum supported
				NET_BUFFER_DATA_OFFSET(NetBuffer),
				&TotalPacketLength
				);
		if( fbResult == FALSE ){
			DBGLOG(( LError, "MmGetSystemAddressForMdlSafe failed\n" ));
			nStatus = NDIS_STATUS_RESOURCES;			
			break;
		}
		ASSERT(TotalPacketLength == NET_BUFFER_DATA_LENGTH(NetBuffer));

		NET_BUFFER_CURRENT_MDL_OFFSET(pNB) = 0;
		NET_BUFFER_DATA_OFFSET(pNB) = 0;
		NET_BUFFER_DATA_LENGTH(pNB) = TotalPacketLength;

		NdisMIndicateReceiveNetBufferListsPtr( 
			pAdapter->NdisMiniportHandle,
			pNBL,
			PortNumber,
			1,
			ReceiveFlags | NDIS_RECEIVE_FLAGS_RESOURCES 
			);
		
	}while ( FALSE );

	if ( pNBL ){
		// reclaim the list
		NdisFreeNetBufferListPtr ( pNBL );
	}

	return nStatus;
}
NDIS_STATUS
	MpWanRcv(
		IN PKIP_NDIS_ADAPTER pAdapter,
		IN PKIP_RAS_LINK pRasLink,
		IN PUCHAR Packet,
		IN ULONG PacketSize
		)
{
	NDIS_STATUS nStatus = NDIS_STATUS_SUCCESS;
	return nStatus;
}