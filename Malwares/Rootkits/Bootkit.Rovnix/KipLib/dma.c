/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: dma.c
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//	SGDMA interface
#include "project.h"

#define MODULE_TAG 'AMDK'

//***********************************************************************************
// Name: MpProcessSGList 
//
// Description: 
//		starts a direct memory access (DMA) scatter/gather operation.
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpProcessSGList(
		IN	PDEVICE_OBJECT			pDO,
		IN	PIRP					pIrp,
		IN	PSCATTER_GATHER_LIST	pSGL,
		IN	PVOID					Context
		)
{
	NDIS_STATUS nStatus;
	PNDIS_PACKET Packet = (PNDIS_PACKET)Context;
	PKIP_PACKET_CONTEXT PacketContext = NDIS_PER_PACKET_CONTEXT(Packet);
	PKIP_NDIS_PROTOCOL pProtocol = PacketContext->pProtocol;
	PKIP_NDIS_ADAPTER pAdapter = (PKIP_NDIS_ADAPTER)pProtocol->pInterface;
	PKIP_NDIS_MINIPORT pMiniport = pAdapter->Interface.Miniport;
	//PNDIS_CO_VC_PTR_BLOCK	VcPtr;

	NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, ScatterGatherListPacketInfo) = pSGL;

	//
	// Handle Send/SendPacket differently
	//
	//MINIPORT_SET_PACKET_FLAG(Packet, fPACKET_PENDING);
	if (KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_IS_CO))
	{
		//VcPtr = GET_VCPTR_FROM_PACKET(Packet);
		//(*VcPtr->WCoSendPacketsHandler)(VcPtr->MiniportContext,
		//								&Packet,
		//								1);

		//TODO:
		ASSERT(FALSE);
	
	}
	else if ( pMiniport->Ndis5.MiniportCharacteristics.SendPacketsHandler )
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
}
//***********************************************************************************
// Name: MpAllocSGList
//
// Description: 
//		sends packet via DMA
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
NDIS_STATUS
	MpAllocSGList(
		IN  PKIP_NDIS_ADAPTER		pAdapter,
		IN  PNDIS_PACKET            Packet
		)
{
	NDIS_STATUS		nStatus = NDIS_STATUS_FAILURE; 
	PNDIS_BUFFER	Buffer;
	PVOID			pBufferVa;
	ULONG			PacketLength;
	PDMA_ADAPTER	SystemAdapterObject;
	PDEVICE_OBJECT	DeviceObject;
	PNPAGED_LOOKASIDE_LIST SGListLookasideList = NULL;
	ULONG ScatterGatherListSize = 0;
	PVOID pSGList = NULL;
	KIRQL OldIrql;
	PVOID SendBuffer = NULL;
	PNDIS_BUFFER	SendNdisBuffer;
	ULONG nBytesRead = 0;
	BOOLEAN fbResult;
	BOOLEAN fbDMAV2;

	do 
	{
		SystemAdapterObject = NDIS_GetSystemAdapterObject( pAdapter->NdisMiniportHandle );
		if ( SystemAdapterObject == NULL ){
			DBGLOG(( LError, "SystemAdapterObject is NULL\n" ));
			nStatus = NDIS_STATUS_RESOURCES;
			break;
		}

		fbDMAV2 = (SystemAdapterObject->DmaOperations->Size >= sizeof(DMA_OPERATIONS_V2) );

		DeviceObject = NDIS_GetMiniportDeviceObject( pAdapter->NdisMiniportHandle );
		if ( DeviceObject == NULL ){
			DBGLOG(( LError, "DeviceObject is NULL\n" ));
			nStatus = NDIS_STATUS_RESOURCES;
			break;
		}

		if ( fbDMAV2 ){
			SGListLookasideList = NDIS_GetSGListLookasideList( pAdapter->NdisMiniportHandle );
			if ( SGListLookasideList == NULL ){
				DBGLOG(( LError, "SGListLookasideList is NULL\n" ));
				nStatus = NDIS_STATUS_RESOURCES;
				break;
			}

			ScatterGatherListSize = NDIS_GetScatterGatherListSize( pAdapter->NdisMiniportHandle );
			if ( ScatterGatherListSize == 0 ){
				DBGLOG(( LError, "SGListLookasideList is NULL\n" ));
				nStatus = NDIS_STATUS_RESOURCES;
				break;
			}
		}

		NdisQueryPacket(Packet, NULL, NULL, &Buffer, &PacketLength);
		if ( Buffer == NULL ){
			DBGLOG(( LError, "Buffer is NULL\n" ));
			nStatus = NDIS_STATUS_FAILURE;
			break;
		}

		// try to use build sg list interface
		if ( fbDMAV2 ){
			// allocate SG list
			pSGList = 
				ExAllocateFromNPagedLookasideList(
					SGListLookasideList
					);
			if ( pSGList != NULL ){

				Packet->Private.Flags |= NDIS_FLAGS_USES_SG_BUFFER_LIST; // sg list
				NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, NdisReserved) = pSGList;
				// raise irql to dispatch
				KeRaiseIrql(DISPATCH_LEVEL,&OldIrql);
				nStatus = 
					SystemAdapterObject->DmaOperations->BuildScatterGatherList(
						SystemAdapterObject,
						DeviceObject,
						Buffer,
						MmGetMdlVirtualAddress(Buffer),
						PacketLength,
						MpProcessSGList,
						Packet,
						TRUE,
						pSGList,
						ScatterGatherListSize
						);
				KeLowerIrql( OldIrql );

				if ( !NT_SUCCESS ( nStatus )){
					DBGLOG(( LDebug, "%08X=BuildMdlFromScatterGatherList failed\n",nStatus ));
					Packet->Private.Flags &= ~NDIS_FLAGS_RESERVED2; // sg list
					NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, ScatterGatherListPacketInfo) = NULL;
					ExFreeToNPagedLookasideList( SGListLookasideList, pSGList );
				}
			}else{
				DBGLOG(( LDebug, "ExAllocateFromNPagedLookasideList failed\n" ));
				nStatus = NDIS_STATUS_RESOURCES;
			}
		}

		// if ( !fbDMAV2 ) then nStatus = NDIS_STATUS_FAILURE; 
		if ( !NT_SUCCESS ( nStatus ))
		{
			// raise irql to dispatch
			KeRaiseIrql(DISPATCH_LEVEL,&OldIrql);

			// call DMA
			nStatus = 
				SystemAdapterObject->DmaOperations->GetScatterGatherList(
					SystemAdapterObject,
					DeviceObject,
					Buffer,
					MmGetMdlVirtualAddress(Buffer),
					PacketLength,
					MpProcessSGList,
					Packet,
					TRUE
					);
			KeLowerIrql( OldIrql );

			if ( !NT_SUCCESS ( nStatus )){
				DBGLOG(( LError, "%08X=GetScatterGatherList failed\n",nStatus ));
			}
		}

		if ( fbDMAV2 && !NT_SUCCESS ( nStatus ) ) {
			
			SendBuffer = 
				ExAllocatePoolWithTag(
					NonPagedPool,
					PacketLength,
					MODULE_TAG
					);
			if ( SendBuffer == NULL ){
				DBGLOG(( LError, "ExAllocatePoolWithTag failed\n" ));
				nStatus = NDIS_STATUS_RESOURCES;
				break;
			}
			NdisAllocateBuffer(
				&nStatus,
				&SendNdisBuffer,
				NULL,
				SendBuffer,
				PacketLength
				);

			if ( nStatus != NDIS_STATUS_SUCCESS ){
				DBGLOG(( LError, "%08X=NdisAllocateBuffer failed\n",nStatus ));
				break;
			}
			//copy data to intermediate buffer
			fbResult =
				NDIS_CopyPacketData(
					Packet,
					SendBuffer,
					PacketLength,0,
					&nBytesRead
					);
			if ( !fbResult || nBytesRead != PacketLength ){
				DBGLOG(( LError, "NDIS_CopyPacketData failed\n" ));
				nStatus = NDIS_STATUS_RESOURCES;
				break;
			}

			Packet->Private.Flags |= NDIS_FLAGS_DOUBLE_BUFFERED; // sg list
			NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, NdisReserved) = SendBuffer;

			// raise irql to dispatch
			KeRaiseIrql(DISPATCH_LEVEL,&OldIrql);
			// call DMA
			nStatus = 
				SystemAdapterObject->DmaOperations->GetScatterGatherList(
					SystemAdapterObject,
					DeviceObject,
					SendNdisBuffer,
					MmGetMdlVirtualAddress(SendNdisBuffer),
					PacketLength,
					MpProcessSGList,
					Packet,
					TRUE
					);
			KeLowerIrql( OldIrql );			
		}

		if ( !NT_SUCCESS ( nStatus ) ){

			if ( SendNdisBuffer ){
				NdisFreeBuffer( SendNdisBuffer );
			}
			if ( SendBuffer ){
				ExFreePoolWithTag( SendBuffer, MODULE_TAG );
			}

			NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, NdisReserved) = NULL;
			if (KIP_MINIPORT_TEST_FLAG(pAdapter, fADAPTER_IS_CO))
			{
				ASSERT(FALSE); // TODO:
				//NdisMCoSendComplete(NDIS_STATUS_FAILURE, GET_VCPTR_FROM_PACKET(Packet), Packet);
			}
			else
			{
				//MpSendPacketCompleteHandler( pAdapter, Packet, NDIS_STATUS_FAILURE );
				// we complete it in caller
			}
		}

	} while ( FALSE );

	return nStatus;
}
//***********************************************************************************
// Name: MpFreeSGList 
//
// Description: 
//		frees sgdma list
//
// Return value: 
//		None
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************
VOID
	MpFreeSGList(
		IN  PKIP_NDIS_ADAPTER		pAdapter,
		IN	PNDIS_PACKET			Packet
		)
{
	PSCATTER_GATHER_LIST pSGL;
	PDMA_ADAPTER	SystemAdapterObject = NDIS_GetSystemAdapterObject( pAdapter->NdisMiniportHandle );

	pSGL = NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, ScatterGatherListPacketInfo);
	if ( SystemAdapterObject ){
		SystemAdapterObject->DmaOperations->PutScatterGatherList( SystemAdapterObject, pSGL, TRUE );
	}

	if ( (Packet->Private.Flags & NDIS_FLAGS_USES_SG_BUFFER_LIST ) ==  NDIS_FLAGS_USES_SG_BUFFER_LIST ){
		PNPAGED_LOOKASIDE_LIST SGListLookasideList = NDIS_GetSGListLookasideList( pAdapter->NdisMiniportHandle );
		Packet->Private.Flags &= ~NDIS_FLAGS_USES_SG_BUFFER_LIST;
		pSGL = NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, NdisReserved);
		if ( pSGL && SGListLookasideList ){
			ExFreeToNPagedLookasideList( SGListLookasideList, pSGL );
		}
		NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, NdisReserved) = NULL;
	} 
	else if ( ( Packet->Private.Flags & NDIS_FLAGS_DOUBLE_BUFFERED ) == NDIS_FLAGS_DOUBLE_BUFFERED ){
		PNDIS_BUFFER NdisBuffer;
		PVOID NdisBufferVA;
		Packet->Private.Flags &= ~NDIS_FLAGS_DOUBLE_BUFFERED;
		NdisBuffer = NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, NdisReserved);
		if ( NdisBuffer ){
			NdisBufferVA = MmGetMdlVirtualAddress(NdisBuffer);
			NdisFreeBuffer( NdisBuffer );
			if ( NdisBufferVA ){
				ExFreePoolWithTag(NdisBufferVA,MODULE_TAG);
			}
		}
	}
}