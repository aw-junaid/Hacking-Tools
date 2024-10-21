#ifndef __FILTER_H_
#define __FILTER_H_

typedef enum{

	FILTER_PASS,		// pass packet to ndis
	FILTER_INDICATE,		// pass packet to lwip
	FILTER_INDICATE_COPY,		// pass packet to original and indicate packet to lwip
	FILTER_MORE_PROCESSING,
	FILTER_DROP //drop it
}FILTER_RESULT;

FILTER_RESULT
	FltFastProcessIncomingEthrenet(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID Buffer,
		IN ULONG BufferSize,
		OUT PBOOLEAN IsBroadCast
		);

FILTER_RESULT
	FltFastProcessIncomingARP(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID Buffer,
		IN ULONG BufferSize
		);

FILTER_RESULT
	FltFastProcessIncomingIP(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID Buffer,
		IN ULONG BufferSize,
		IN BOOLEAN EthernetBroadcast
		);

FILTER_RESULT
	FltProcessIncomingIP(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN struct eth_hdr *pEthHeader,
		IN struct ip_hdr *IpHdr,
		IN ULONG Length
		);

FILTER_RESULT
	FltProcessIncomingBuffer(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID Buffer,
		IN ULONG Length
		);

FILTER_RESULT
	FltProcessIncomingPacket(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PKIP_PACKET_DESCRIPTOR PacketDesc
		);

#endif //__FILTER_H_