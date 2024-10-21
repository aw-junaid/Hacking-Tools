#include "project.h"
#include "lwip\ip_frag.h"
#include "lwip\udp.h"
#include "lwip\tcp_impl.h"

KSPIN_LOCK DumpLock = 0;

void
__tcp_debug_print_flags(u8_t flags)
{
	DbgPrint("Flags(%.2X) ",flags);
	if (flags & TCP_FIN) {
		DbgPrint("FIN ");
	}
	if (flags & TCP_SYN) {
		DbgPrint("SYN ");
	}
	if (flags & TCP_RST) {
		DbgPrint("RST ");
	}
	if (flags & TCP_PSH) {
		DbgPrint("PSH ");
	}
	if (flags & TCP_ACK) {
		DbgPrint("ACK ");
	}
	if (flags & TCP_URG) {
		DbgPrint("URG ");
	}
	if (flags & TCP_ECE) {
		DbgPrint("ECE ");
	}
	if (flags & TCP_CWR) {
		DbgPrint("CWR ");
	}
	DbgPrint("\n");
}

void DumpBin( PUCHAR Buffer, ULONG Length )
{
	unsigned int k, m;

	Length = min(Length,40);

	for ( k = 0; k < Length; k+=20)
	{
		for ( m = k; m < k+20 && m < Length; ++m ){
			DbgPrint("%.2X ", Buffer[m]);
		}

		for(;m < k + 20;++m){
			DbgPrint ("   ");
		}

		for ( m = k; m < k+20 && m < Length; ++m ){
			if ( isprint( Buffer[m] ))
				DbgPrint ("%c", Buffer[m]);
			else
				DbgPrint (".");
		}
		DbgPrint ("\n");
	}		

}

void _DumpNdisBuffer( struct eth_hdr *pEthHeader, PVOID Buffer, ULONG Size )
{
	DbgPrint("=================================\n");
	DbgPrint("Packet dump size=%lu:\n",Size);
	DbgPrint("Ethernet header:\n");
	DbgPrint("+-------------------------------+\n");
	if ( pEthHeader->type == PP_HTONS(ETHTYPE_ARP))
	{
		DbgPrint("ETHERNET: ARP\n");
	}
	else if(pEthHeader->type == PP_HTONS(ETHTYPE_IP))
	{
		DbgPrint("ETHERNET: IP\n");
	}
	else if(pEthHeader->type == PP_HTONS(ETHTYPE_PPPOE) )
	{
		DbgPrint("ETHERNET: PPPOE\n");
	}else{
		DbgPrint("ETHERNET: %04X\n",pEthHeader->type);
	}

	DbgPrint("s_addr=%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\nd_addr=%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", 
		pEthHeader->src.addr[0],pEthHeader->src.addr[1],pEthHeader->src.addr[2],
		pEthHeader->src.addr[3],pEthHeader->src.addr[4],pEthHeader->src.addr[5],
		pEthHeader->dest.addr[0],pEthHeader->dest.addr[1],pEthHeader->dest.addr[2],
		pEthHeader->dest.addr[3],pEthHeader->dest.addr[4],pEthHeader->dest.addr[5]
	);

	if ( pEthHeader->type == PP_HTONS(ETHTYPE_ARP))
	{
		struct etharp_hdr *pEthrApr = (struct etharp_hdr*)Buffer;

		DbgPrint("ARP header:\n");
		DbgPrint("+-------------------------------+\n");

		if ( Size < SIZEOF_ETHARP_HDR ){
			DbgPrint("Size < SIZEOF_ETHARP_PACKET\n");
			DbgPrint("=================================\n");
			return;
		}
		if ( pEthrApr->hwtype == PP_HTONS(1) ){
			DbgPrint("  ARP.hwtype = HWTYPE_ETHERNET\n");
			if ( pEthrApr->proto == PP_HTONS(ETHTYPE_IP) ){
				DbgPrint("  ARP.proto = IP\n");
			}else{
				DbgPrint("  ARP.proto = %04X\n",pEthrApr->proto);
			}

			if ( pEthrApr->opcode == PP_HTONS(ARP_REPLY) ){
				DbgPrint("  ARP.opcode = ARP_REPLY\n");
			}else if ( pEthrApr->opcode == PP_HTONS(ARP_REQUEST) ){
				DbgPrint("  ARP.opcode = ARP_REQUEST\n");
			} else if ( pEthrApr->opcode == PP_HTONS(3) ){
				DbgPrint("  ARP.opcode = ARP_RARP_REQUEST\n");
			}else if ( pEthrApr->opcode == PP_HTONS(4) ){
				DbgPrint("  ARP.opcode = ARP_RARP_REPLY\n");
			}else {
				DbgPrint("  ARP.opcode = %04X\n",pEthrApr->opcode);
			}
			

			DbgPrint("+-------------------------------+\n");
			DbgPrint("shwaddr=%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\ndhwaddr=%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", 
				pEthrApr->shwaddr.addr[0],pEthrApr->shwaddr.addr[1],pEthrApr->shwaddr.addr[2],
				pEthrApr->shwaddr.addr[3],pEthrApr->shwaddr.addr[4],pEthrApr->shwaddr.addr[5],
				pEthrApr->dhwaddr.addr[0],pEthrApr->dhwaddr.addr[1],pEthrApr->dhwaddr.addr[2],
				pEthrApr->dhwaddr.addr[3],pEthrApr->dhwaddr.addr[4],pEthrApr->dhwaddr.addr[5]
			);

			DbgPrint("+-------------------------------+\n");
			DbgPrint("|  %3"U16_F"  |  %3"U16_F"  |  %3"U16_F"  |  %3"U16_F"  | (src)\n",
				ip4_addr1_16(&pEthrApr->sipaddr),
				ip4_addr2_16(&pEthrApr->sipaddr),
				ip4_addr3_16(&pEthrApr->sipaddr),
				ip4_addr4_16(&pEthrApr->sipaddr));
			DbgPrint("+-------------------------------+\n");
			DbgPrint("|  %3"U16_F"  |  %3"U16_F"  |  %3"U16_F"  |  %3"U16_F"  | (dest)\n",
				ip4_addr1_16(&pEthrApr->dipaddr),
				ip4_addr2_16(&pEthrApr->dipaddr),
				ip4_addr3_16(&pEthrApr->dipaddr),
				ip4_addr4_16(&pEthrApr->dipaddr));
			DbgPrint("+-------------------------------+\n");
		}else{
			DbgPrint("  ARP.hwtype = %04X\n",pEthHeader->type);
		}
	} 
	else if ( pEthHeader->type == PP_HTONS(ETHTYPE_IP))
	{
		struct ip_hdr *iphdr = (struct ip_hdr*)Buffer;

		USHORT			wFragOffset;
		UCHAR			byRF,byDF,byMF;
		ULONG			ip_len,hlen = 0;
		ip_addr_t src_ip;
		ip_addr_t dst_ip;

		DbgPrint("IP header:\n");

		if ( Size < IP_HEADER_LEN ){
			DbgPrint("Size < SIZEOF_ETH_HDR + IP_HEADER_LEN\n");
			DbgPrint("=================================\n");
			return;
		}

		ip_addr_copy(dst_ip, iphdr->dest);
		ip_addr_copy(src_ip, iphdr->src);

		// set the IP datagram length */
		ip_len = ntohs( IPH_LEN(iphdr) );

		// set the IP header length 
		hlen = IPH_HL(iphdr) << 2;

		/* check for fragmented packets */
		wFragOffset = ntohs(IPH_OFFSET(iphdr));

		/* 
		* get the values of the reserved, more 
		* fragments and don't fragment flags 
		*/
		byMF = (unsigned char)((wFragOffset & 0x2000) >> 13);

		/* mask off the high bits in the fragment offset field */
		wFragOffset &= 0x1FFF;

		if ( wFragOffset || byMF ){
			DbgPrint("ip fragmented, id=%04X\n",IPH_ID(iphdr));
			DbgPrint("=================================\n");
			return;
		}

		DbgPrint("+-------------------------------+\n");
		DbgPrint("ip_hlen=%d\n",hlen);
		DbgPrint("ip_len=%d\n",ip_len);
		DbgPrint("+-------------------------------+\n");
		switch (IPH_PROTO(iphdr))
		{
		case IP_PROTO_UDP:
			DbgPrint("ip_proto=UDP\n");
			break;
		case IP_PROTO_TCP:
			DbgPrint("ip_proto=TCP\n");
			break;
		case IP_PROTO_ICMP:
			DbgPrint("ip_proto=ICMP\n");
			break;
		default:
			DbgPrint("ip_proto=%02X\n",IPH_PROTO(iphdr));
			break;
		}

		DbgPrint("+-------------------------------+\n");
		DbgPrint("|  %3"U16_F"  |  %3"U16_F"  |  %3"U16_F"  |  %3"U16_F"  | (src)\n",
			ip4_addr1_16(&iphdr->src),
			ip4_addr2_16(&iphdr->src),
			ip4_addr3_16(&iphdr->src),
			ip4_addr4_16(&iphdr->src));
		DbgPrint("+-------------------------------+\n");
		DbgPrint("|  %3"U16_F"  |  %3"U16_F"  |  %3"U16_F"  |  %3"U16_F"  | (dest)\n",
			ip4_addr1_16(&iphdr->dest),
			ip4_addr2_16(&iphdr->dest),
			ip4_addr3_16(&iphdr->dest),
			ip4_addr4_16(&iphdr->dest));

		if ( Size < SIZEOF_ETH_HDR + hlen ){
			DbgPrint("Size < SIZEOF_ETH_HDR + hlen\n");
			DbgPrint("=================================\n");
			return;
		}

		if ( IPH_PROTO(iphdr) == IP_PROTO_TCP )
		{
			struct tcp_hdr *tcphdr = (struct tcp_hdr*)((PCHAR)Buffer+hlen);

			if ( Size < SIZEOF_ETH_HDR + hlen +sizeof(struct tcp_hdr) ){
				DbgPrint("Size < SIZEOF_ETH_HDR + hlen + sizeof(struct tcp_hdr)\n");
				DbgPrint("=================================\n");
				return;
			}

			DbgPrint("TCP header:\n");
			DbgPrint("+-------------------------------+\n");
			DbgPrint("|    %5"U16_F"      |    %5"U16_F"      | (src port, dest port)\n",
				ntohs(tcphdr->src), ntohs(tcphdr->dest));
			DbgPrint("+-------------------------------+\n");
			DbgPrint("|           %010"U32_F"          | (seq no)\n",
				ntohl(tcphdr->seqno));
			DbgPrint("+-------------------------------+\n");
			DbgPrint("|           %010"U32_F"          | (ack no)\n",
				ntohl(tcphdr->ackno));
			DbgPrint("+-------------------------------+\n");
			__tcp_debug_print_flags(TCPH_FLAGS(tcphdr));
			DbgPrint("+-------------------------------+\n");
			DbgPrint("|    0x%04"X16_F"     |     %5"U16_F"     | (chksum, urgp)\n",
				ntohs(tcphdr->chksum), ntohs(tcphdr->urgp));
			DbgPrint("+-------------------------------+\n");

			DumpBin( (PUCHAR)tcphdr+sizeof(struct tcp_hdr), Size -(SIZEOF_ETH_HDR + hlen +sizeof(struct tcp_hdr)) );
		} 
		else if ( IPH_PROTO(iphdr) == IP_PROTO_UDP )
		{
			struct udp_hdr *udphdr = (struct udp_hdr*)((PCHAR)Buffer+hlen);
			if ( Size < SIZEOF_ETH_HDR + hlen +sizeof(struct udp_hdr) ){
				DbgPrint("Size < SIZEOF_ETH_HDR + hlen + sizeof(struct udp_hdr)\n");
				DbgPrint("=================================\n");
				return;
			}

			DbgPrint("UDP header:\n");
			DbgPrint("+-------------------------------+\n");
			DbgPrint("|     %5"U16_F"     |     %5"U16_F"     | (src port, dest port)\n",
				ntohs(udphdr->src), ntohs(udphdr->dest));
			DbgPrint("+-------------------------------+\n");
			DbgPrint("|     %5"U16_F"     |     0x%04"X16_F"    | (len, chksum)\n",
				ntohs(udphdr->len), ntohs(udphdr->chksum));
			DbgPrint("+-------------------------------+\n");
		}
		else if ( IPH_PROTO(iphdr) == IP_PROTO_ICMP )
		{
			struct icmp_echo_hdr *icmphdr = (struct icmp_echo_hdr*)((PCHAR)Buffer+hlen);

			if ( Size < SIZEOF_ETH_HDR + hlen + 2 ){
				DbgPrint("Size < SIZEOF_ETH_HDR + hlen + 2\n");
				DbgPrint("=================================\n");
				return;
			}

			DbgPrint("ICMP header:\n");
			DbgPrint("+-------------------------------+\n");
			switch(ICMPH_TYPE(icmphdr))
			{
			case ICMP_ER://   0    /* echo reply */
				DbgPrint("type=ICMP_ER\n");
				break;
			case ICMP_DUR://  3    /* destination unreachable */
				DbgPrint("type=ICMP_DUR\n");
				break;
			case ICMP_SQ://   4    /* source quench */
				DbgPrint("type=ICMP_SQ\n");
				break;
			case ICMP_RD://   5    /* redirect */
				DbgPrint("type=ICMP_RD\n");
				break;
			case ICMP_ECHO:// 8    /* echo */
				DbgPrint("type=ICMP_ECHO\n");
				break;
			case ICMP_TE://  11    /* time exceeded */
				DbgPrint("type=ICMP_TE\n");
				break;
			case ICMP_PP://  12    /* parameter problem */
				DbgPrint("type=ICMP_PP\n");
				break;
			case ICMP_TS://  13    /* timestamp */
				DbgPrint("type=ICMP_TS\n");
				break;
			case ICMP_TSR:// 14    /* timestamp reply */
				DbgPrint("type=ICMP_TSR\n");
				break;
			case ICMP_IRQ:// 15    /* information request */
				DbgPrint("type=ICMP_IRQ\n");
				break;
			case ICMP_IR://  16    /* information reply */
				DbgPrint("type=ICMP_IR\n");
				break;
			default:
				DbgPrint("type=%.2X\n",ICMPH_TYPE(icmphdr));
				break;
			}
			DbgPrint("+-------------------------------+\n");
		}
	}
	DbgPrint("=================================\n");
}

void DumpNdisBuffer( PCHAR Direction, struct eth_hdr *pEthHeader, PVOID Buffer, ULONG Size )
{
	KIRQL OldIrql;

	KeAcquireSpinLock(&DumpLock,&OldIrql);
	DbgPrint("%s /////////////////////////////////////////////////////\n",Direction);
	DbgPrint("Buffer %p\n",pEthHeader);
	_DumpNdisBuffer( pEthHeader, Buffer, Size );
	DbgPrint("END DUMP /////////////////////////////////////////////////////\n",Direction);
	KeReleaseSpinLock(&DumpLock,OldIrql);
}

void DumpNdisPacket( PCHAR Direction, PNDIS_PACKET Packet )
{
	UINT nBufferCount, TotalPacketLength, uReadyLen,FirstBufferLength;
	BOOLEAN fbResult = FALSE;
	PNDIS_BUFFER  FirstBuffer;
	PVOID FirstBufferVA;

	NdisGetFirstBufferFromPacket(
		Packet,
		&FirstBuffer,
		&FirstBufferVA,
		&FirstBufferLength,
		&TotalPacketLength
		);

	if ( FirstBufferLength < SIZEOF_ETH_HDR ){
		return;
	}

	//TEST
	if ( FirstBufferLength < SIZEOF_ETH_HDR + IP_HEADER_LEN + TCP_HEADER_LEN )
		return;

	if ( FirstBufferVA ){
		KIRQL OldIrql;
		struct eth_hdr *pEthHeader = (struct eth_hdr *)FirstBufferVA;

		KeAcquireSpinLock(&DumpLock,&OldIrql);
		DbgPrint("%s /////////////////////////////////////////////////////\n",Direction);
		DbgPrint("Packet %p\n",Packet);
		_DumpNdisBuffer( pEthHeader, (PCHAR)FirstBufferVA + SIZEOF_ETH_HDR, FirstBufferLength - SIZEOF_ETH_HDR );
		DbgPrint("END DUMP /////////////////////////////////////////////////////\n",Direction);
		KeReleaseSpinLock(&DumpLock,OldIrql);
	}
}