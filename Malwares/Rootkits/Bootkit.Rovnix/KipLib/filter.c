//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: filter.c
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//  Here we process the incoming packets and
//  make decision if it should be passed to ndis,
//  indicated to lwip or both

#include "project.h"
#include "filter.h"
#include "lwip\ip_frag.h"
#include "lwip\udp.h"
#include "lwip\tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip\dhcp.h"

#define HWTYPE_ETHERNET 1

#ifndef DNS_SERVER_PORT
#define DNS_SERVER_PORT           53
#endif

//***********************************************************************************
// Name: FltVerifyIpHeader
//
// Description: 
//		IP header sanity checks 
//
// Return value: 
//		filtering result
//
// Parameters: 
//
// NOTE: None
// **********************************************************************************

BOOLEAN
	FltVerifyIpHeader(
		IN struct ip_hdr *IpHdr,
		IN ULONG	Length,
		IN PULONG	phlen
		)
{
	ULONG			ip_len,hlen;
	USHORT			wFragOffset;
	UCHAR			byRF,byDF,byMF;

	*phlen = 0;
	//check ip packet
	if ( IPH_V(IpHdr) != 4 ){
		return FALSE;
	}

	// set the IP datagram length */
	ip_len = ntohs( IPH_LEN(IpHdr) );

	// set the IP header length 
	hlen = IPH_HL(IpHdr) << 2;
	*phlen = hlen;

	/* header length sanity check */
	if(hlen < IP_HLEN ) {
		return FALSE;
	}

	if (ip_len != Length){
		if (ip_len > Length || ip_len == 0) 
			ip_len = Length;
	}
	if(ip_len < hlen) {
		return FALSE;
	}

	return TRUE;
}

//***********************************************************************************
// Name: FltFastProcessIncomingEthrenet
//
// Description: 
//		Fast Ethernet header processing path. Simple validation. 
//
// Return value: 
//		filtering result
//
// Parameters: 
//	pAdapter - 
//	Buffer - pointer to Ethernet header
//
// NOTE: None
// **********************************************************************************

FILTER_RESULT
	FltFastProcessIncomingEthrenet(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID Buffer,
		IN ULONG BufferSize,
		OUT PBOOLEAN IsBroadCast
		)
{
	LONG Result;
	struct eth_hdr *pEthHeader = (struct eth_hdr *)Buffer;

	// check  size
	if ( BufferSize < SIZEOF_ETH_HDR ){
		DBGLOG(( LTrace, "BufferSize (%lu) < SIZEOF_ETH_HDR\n", BufferSize ));
		return FILTER_PASS;
	}

	// check type
	if ( pEthHeader->type != PP_HTONS(ETHTYPE_ARP) &&
		pEthHeader->type != PP_HTONS(ETHTYPE_IP) &&
		pEthHeader->type != PP_HTONS(ETHTYPE_PPPOE) )
	{
		DBGLOG(( LTrace, "unknown ether type\n", pEthHeader->type ));
		return FILTER_PASS; // pass to original
	}

	// indicate broadcasts to orig and lwip
	if ( ETH_IS_BROADCAST(pEthHeader->dest.addr) ){
		*IsBroadCast = TRUE;
		if ( pEthHeader->type == PP_HTONS(ETHTYPE_ARP) ){
			return FILTER_MORE_PROCESSING;
		}
		if ( pEthHeader->type == PP_HTONS(ETHTYPE_IP) ){
			return FILTER_MORE_PROCESSING;
		}
		return FILTER_INDICATE_COPY; 
	}

	// drop all foreign packets
	ETH_COMPARE_NETWORK_ADDRESSES_EQ(pProtocol->IIF.hwaddr,pEthHeader->dest.addr,&Result); 
	if ( Result != 0 ){
		DBGLOG(( LTrace, "not to us addr=%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", 
			pEthHeader->dest.addr[0],pEthHeader->dest.addr[1],pEthHeader->dest.addr[2],
		pEthHeader->dest.addr[3],pEthHeader->dest.addr[4],pEthHeader->dest.addr[5]));
		//return FILTER_PASS; // don't care, pass to original
	}

	return FILTER_MORE_PROCESSING;
}

//***********************************************************************************
// Name: FltFastProcessIncomingARP
//
// Description: 
//		Fast ARP header processing path. Simple validation. 
//
// Return value: 
//		filtering result
//
// Parameters: 
//	pAdapter - 
//	Buffer - pointer to arp header
//
// NOTE: None
// **********************************************************************************

FILTER_RESULT
	FltFastProcessIncomingARP(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID Buffer,
		IN ULONG BufferSize
		)
{
	struct etharp_hdr *pEthrApr = (struct etharp_hdr*)Buffer;
	// check  size
	if ( BufferSize < SIZEOF_ETHARP_HDR ){
		DBGLOG(( LTrace, "BufferSize (%lu) < SIZEOF_ETHARP_HDR\n", BufferSize ));
		return FILTER_PASS;
	}

	// verify
	if ( pEthrApr->hwtype != PP_HTONS(HWTYPE_ETHERNET) ){
		DBGLOG(( LTrace, "unknown hwtype\n", pEthrApr->hwtype ));
		return FILTER_PASS;
	}

	if ((pEthrApr->hwtype != PP_HTONS(HWTYPE_ETHERNET)) ||
		(pEthrApr->hwlen != ETHARP_HWADDR_LEN) ||
		(pEthrApr->protolen != sizeof(ip_addr_t)) ||
		(pEthrApr->proto != PP_HTONS(ETHTYPE_IP)))  
	{
		DBGLOG(( LTrace, "invalid arp packet\n" ));
		return FILTER_PASS;
	}

	// we process arp reply only
	if ( pEthrApr->opcode != PP_HTONS(ARP_REPLY) ){
		return FILTER_PASS; // don't care
	}

	return FILTER_INDICATE_COPY;
}
//***********************************************************************************
// Name: FltFastProcessIncomingIP
//
// Description: 
//		Fast IP header processing path. Simple validation. 
//
// Return value: 
//		filtering result
//
// Parameters: 
//	pAdapter - 
//	Buffer - pointer to IP header
//
// NOTE: None
// **********************************************************************************
FILTER_RESULT
	FltFastProcessIncomingIP(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID Buffer,
		IN ULONG BufferSize,
		IN BOOLEAN EthernetBroadcast
		)
{
	struct ip_hdr *IpHdr = (struct ip_hdr *)Buffer;
	ULONG hlen;

	// check  size
	if ( BufferSize < IP_HLEN ){
		// need to get all data
		return FILTER_MORE_PROCESSING;
	}

	if ( FltVerifyIpHeader( IpHdr, BufferSize, &hlen ) == FALSE ){
		return FILTER_PASS; // don't care, pass to original
	}

	// we support icmp, udp and tcp
	if ( IPH_PROTO(IpHdr) != IP_PROTO_TCP && 
		IPH_PROTO(IpHdr) != IP_PROTO_UDP && 
		IPH_PROTO(IpHdr) != IP_PROTO_ICMP ) 
	{
		return FILTER_PASS; // don't care, pass to original
	}

	// broadcast
	if ( ip_addr_isbroadcast ( &IpHdr->dest, &pProtocol->IIF ) ){
		if ( EthernetBroadcast ){
			return FILTER_MORE_PROCESSING;
		}
		return FILTER_INDICATE_COPY;
	}

	// check dst ip
	if ( !ip_addr_isany(&pProtocol->IIF.ip_addr) && 
		!ip_addr_cmp(&IpHdr->dest,&pProtocol->IIF.ip_addr ) )
	{
		if ( IPH_PROTO(IpHdr) != IP_PROTO_UDP )
			return FILTER_PASS; // don't care, pass to original
	}

	// check the source ip
	//if ( KipIsKnownHost( IpHdr->src.addr ) ){
	//	return FILTER_INDICATE;
	//}
	return FILTER_MORE_PROCESSING; // continue processing
}

FILTER_RESULT
	FltProcessIncomingIP(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN struct eth_hdr *pEthHeader,
		IN struct ip_hdr *IpHdr,
		IN ULONG Length
		)
{
	FILTER_RESULT Result = FILTER_PASS;
	USHORT			wFragOffset;
	UCHAR			byRF,byDF,byMF;
	ULONG			ip_len,hlen = 0;
	ip_addr_t src_ip;
	ip_addr_t dst_ip;
	BOOLEAN fbEthernetBroadcast = ETH_IS_BROADCAST(pEthHeader->dest.addr);

	ip_addr_copy(dst_ip, IpHdr->dest);
	ip_addr_copy(src_ip, IpHdr->src);

	// set the IP datagram length */
	ip_len = ntohs( IPH_LEN(IpHdr) );

	// set the IP header length 
	hlen = IPH_HL(IpHdr) << 2;

	/* check for fragmented packets */
	wFragOffset = ntohs(IPH_OFFSET(IpHdr));

	/* 
	* get the values of the reserved, more 
	* fragments and don't fragment flags 
	*/
	byMF = (unsigned char)((wFragOffset & 0x2000) >> 13);

	/* mask off the high bits in the fragment offset field */
	wFragOffset &= 0x1FFF;

	if ( wFragOffset || byMF ){
		// TODO: check by ID
		//__debugbreak();
		if ( ip_reass_lookup_dg(IpHdr) ){
			return FILTER_INDICATE;
		}
		return /*FILTER_MORE_PROCESSING*/FILTER_PASS;
	}

	switch (IPH_PROTO(IpHdr))
	{
	case IP_PROTO_UDP:
		if ( Length >= hlen + sizeof(struct udp_hdr) )
		{
			struct udp_hdr *udphdr = (struct udp_hdr*)((PCHAR)IpHdr + hlen);
			u16_t src, dest;
			
			/* convert src and dest ports to host byte order */
			src = ntohs(udphdr->src);
			dest = ntohs(udphdr->dest);

			if ( dest == DHCP_CLIENT_PORT && src == DHCP_SERVER_PORT ) {
				Result = FILTER_INDICATE_COPY;
				break;
			}
			if ( fbEthernetBroadcast ){
				Result = FILTER_PASS;
				break;
			}

			// check src/dst ip
			if ( ip_addr_cmp(&dst_ip,&pProtocol->IIF.ip_addr ) == FALSE ){
				Result = FILTER_PASS;
				break;
			}

			if ( src == DNS_SERVER_PORT ) {
				//Result = FILTER_INDICATE_COPY;
				//break;
			}
			// lookup UDP endpoints
			if ( udp_lookup_pcb( &src_ip, &dst_ip, src, dest ) ){
				Result = FILTER_INDICATE;
				break;
			}
		}
		Result = FILTER_PASS;
		break;

	case IP_PROTO_TCP:
		// lookup TCP endpoints
		if ( Length >= hlen + sizeof(struct tcp_hdr) )
		{
			struct tcp_hdr *tcphdr = (struct tcp_hdr*)((PCHAR)IpHdr + hlen);
			u16_t src, dest;
			u8_t flags = TCPH_FLAGS(tcphdr);

			if ( fbEthernetBroadcast ){
				Result = FILTER_PASS;
				break;
			}

			// check src/dst ip
			if ( ip_addr_cmp(&dst_ip,&pProtocol->IIF.ip_addr ) == FALSE ){
				Result = FILTER_PASS;
				break;
			}

			/* convert src and dest ports to host byte order */
			src = ntohs(tcphdr->src);
			dest = ntohs(tcphdr->dest);

			if ( flags == TCP_SYN ){
				if ( tcp_lookup_listen_pcb( &dst_ip, dest )){
					Result = FILTER_INDICATE;
					break;
				}
			}else if ( tcp_lookup_pcb( &src_ip, &dst_ip, src, dest )){
				Result = FILTER_INDICATE;
				break;
			}			
		}
		Result = FILTER_PASS;
		break;

	case IP_PROTO_ICMP:
		Result = FILTER_PASS;
		//if ( Length >= hlen + 2 )
		//{
		//	struct icmp_echo_hdr *icmphdr = (struct icmp_echo_hdr*)((PCHAR)IpHdr+SIZEOF_ETH_HDR + hlen);
		//	if ( ICMPH_TYPE(icmphdr) == ICMP_ER/* || ICMPH_TYPE(icmphdr) == ICMP_ECHO*/ ){
		//		Result = FILTER_INDICATE_COPY;
		//	}
		//}		
		//break;

	default:
		Result = FILTER_PASS;
		break;
	}
	return Result;
}

FILTER_RESULT
	FltProcessIncomingBuffer(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PVOID Buffer,
		IN ULONG Length
		)
{
	FILTER_RESULT FilterResult;
	struct eth_hdr *pEthHeader;
	BOOLEAN fbEtherBroadcast = FALSE;

	//
	// Fast filter packet
	//

	// filter incoming ethernet
	FilterResult = 
		FltFastProcessIncomingEthrenet( 
			pProtocol, 
			Buffer, 
			Length,
			&fbEtherBroadcast
			);

	if ( FilterResult == FILTER_MORE_PROCESSING ){
		pEthHeader = (struct eth_hdr *)Buffer;
		switch ( pEthHeader->type )
		{
		case PP_HTONS(ETHTYPE_IP):
			// filter incoming ip
			FilterResult = 
				FltFastProcessIncomingIP( 
					pProtocol, 
					(PCHAR)Buffer + SIZEOF_ETH_HDR, 
					Length - SIZEOF_ETH_HDR,
					fbEtherBroadcast
					);
			break;
		case PP_HTONS(ETHTYPE_ARP):
			// filter incoming ip
			FilterResult = 
				FltFastProcessIncomingARP( 
					pProtocol, 
					(PCHAR)Buffer + SIZEOF_ETH_HDR, 
					Length - SIZEOF_ETH_HDR
					);
			break;
		}
	}
	return FilterResult;
}
//***********************************************************************************
// Name: FltFastProcessIncomingFrame
//
// Description: 
//		Fast filter incoming frame 
//
// Return value: 
//		filtering result
//
// Parameters: 
//	pAdapter - 
//	Buffer - pointer to IP header
//
// NOTE: None
// **********************************************************************************
FILTER_RESULT
	FltProcessIncomingPacket(
		IN PKIP_NDIS_PROTOCOL pProtocol,
		IN PKIP_PACKET_DESCRIPTOR PacketDesc
		)
{
	FILTER_RESULT Result = FILTER_PASS;
	struct eth_hdr *pEthHeader = (struct eth_hdr *)PacketDesc->DataBuffer;
	ASSERT(pEthHeader);

	if (pEthHeader->type == PP_HTONS(ETHTYPE_ARP))
	{

	}
	else if ( pEthHeader->type == PP_HTONS(ETHTYPE_IP) )
	{
		Result =
			FltProcessIncomingIP(
				pProtocol,
				pEthHeader,
				(struct ip_hdr *)((PCHAR)pEthHeader+SIZEOF_ETH_HDR),
				PacketDesc->Length-SIZEOF_ETH_HDR
				);
	}

	return Result;
}