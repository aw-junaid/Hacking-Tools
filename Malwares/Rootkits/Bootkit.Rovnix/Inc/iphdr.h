#ifndef __IPHDR_H_
#define __IPHDR_H_

#define IEEE_802_ADDR_LENGTH 6

#define ETHERNET_MTU                  1514

#define	MAX_802_3_LENGTH		1500	// Maximum Value For 802.3 Length Field
#define	MAX_ETHER_SIZE			1514	// Maximum Ethernet Packet Length
#define	MIN_ETHER_SIZE	  		60		// Minimum Ethernet Packet Length

#define ETHERNET_HEADER_LEN         14
#define IP_HEADER_LEN               20
#define TCP_HEADER_LEN              20
#define UDP_HEADER_LEN              8
#define ICMP_HEADER_LEN             4

#define ETHERNET_TYPE_IP              0x0800	/* IPv4 protocol */
#define ETHERNET_TYPE_ARP             0x0806	/* ARP protocol */
#define ETHERNET_TYPE_REVARP          0x8035
#define ETHERNET_TYPE_IPV6            0x86dd
#define ETHERNET_TYPE_IPX             0x8137
#define ETHERNET_TYPE_PPPoE_DISC      0x8863
#define ETHERNET_TYPE_PPPoE_SESS      0x8864
#define ETHERNET_TYPE_8021Q           0x8100

//the same in network order
#define ETHERNET_TYPE_IP_NETORDER				0x0008	/* IPv4 protocol */
#define ETHERNET_TYPE_ARP_NETORDER				0x0608	/* ARP protocol */
#define ETHERNET_TYPE_REVARP_NETORDER			0x3580
#define ETHERNET_TYPE_IPV6_NETORDER				0xdd86
#define ETHERNET_TYPE_IPX_NETORDER				0x3781
#define ETHERNET_TYPE_PPPoE_DISC_NETORDER		0x6388
#define ETHERNET_TYPE_PPPoE_SESS_NETORDER		0x6488
#define ETHERNET_TYPE_8021Q_NETORDER			0x0081


//some ARP defines; 
#define VZ_ETHERTYPE_ARP	0x0806
#define VZ_ETHERTYPE_XNS	0x0600

#define ARP_HW_ENET			1
#define ARP_HW_802			6
#define ARP_802_ADDR_LENGTH 6               // Length of an 802 address.
#define ARP_ETYPE_IP		0x800
#define IP_V4_ADDR_LENGTH 4               // Length of an 802 address.



#define R_FIN			0x01
#define R_SYN			0x02
#define R_RST			0x04
#define R_PSH			0x08
#define R_ACK			0x10
#define R_URG			0x20
#define R_RES2			0x40
#define R_RES1			0x80

#define DO_IP_CHECKSUMS             0x00000001
#define DO_TCP_CHECKSUMS            0x00000002
#define DO_UDP_CHECKSUMS            0x00000004
#define DO_ICMP_CHECKSUMS           0x00000008

#define COMMON_IP_HEADER_LEN 0x05
#define COMMON_IP_HEADER_VER 0x04
#define COMMON_TTL_VALUE 0x80
#define CUSTOM_RST_CLIENT_PACKET_ID 0x0400
#define CUSTOM_RST_SERVER_PACKET_ID 0x0500

#define COMMON_TCP_HEADER_OFFSET 0x05

#define PURE_TCP_PACKET_LENGTH 0x0028

struct _m_in_addr
{
	union
	{
        struct { unsigned char s_b1, s_b2, s_b3, s_b4; } S_un_b;
        struct { unsigned short s_w1, s_w2; } S_un_w;
        unsigned long S_addr;
	} S_un;
#define s_addr	S_un.S_addr
#define s_host	S_un.S_un_b.s_b2
#define s_net	S_un.S_un_b.s_b1
#define s_imp	S_un.S_un_w.s_w2
#define s_impno	S_un.S_un_b.s_b4
#define s_lh	S_un.S_un_b.s_b3
};

typedef struct _EthLlc
{
	unsigned char dsap;
	unsigned char ssap;
} EthLlc;

typedef struct _EthLlcOther
{
	unsigned char ctrl;
	unsigned char org_code[3];
	unsigned short proto_id;
} EthLlcOther;

typedef struct _EtherHdr
{
	unsigned char ether_dst[ARP_802_ADDR_LENGTH];
	unsigned char ether_src[ARP_802_ADDR_LENGTH];
	unsigned short ether_type; /*ETHERNET_TYPE_ARP etc.*/
} EtherHdr;

//
// ZZZ This is a little-endian specific check.
//
#define ETH_IS_MULTICAST(Address) \
	(BOOLEAN)(((PUCHAR)(Address))[0] & ((UCHAR)0x01))


//
// Check whether an address is broadcast.
//
#ifndef ETH_IS_BROADCAST
#define ETH_IS_BROADCAST(Address)               \
	((((PUCHAR)(Address))[0] == ((UCHAR)0xff)) && (((PUCHAR)(Address))[1] == ((UCHAR)0xff)))
#endif

//
// This macro will compare network addresses.
//
//  A - Is a network address.
//
//  B - Is a network address.
//
//  Result - The result of comparing two network address.
//
//  Result < 0 Implies the B address is greater.
//  Result > 0 Implies the A element is greater.
//  Result = 0 Implies equality.
//
// Note that this is an arbitrary ordering.  There is not
// defined relation on network addresses.  This is ad-hoc!
//
//
#define ETH_COMPARE_NETWORK_ADDRESSES(_A, _B, _Result)          \
{                                                               \
    if (*(ULONG UNALIGNED *)&(_A)[2] >                          \
         *(ULONG UNALIGNED *)&(_B)[2])                          \
    {                                                           \
        *(_Result) = 1;                                         \
    }                                                           \
    else if (*(ULONG UNALIGNED *)&(_A)[2] <                     \
                *(ULONG UNALIGNED *)&(_B)[2])                   \
    {                                                           \
        *(_Result) = (UINT)-1;                                  \
    }                                                           \
    else if (*(USHORT UNALIGNED *)(_A) >                        \
                *(USHORT UNALIGNED *)(_B))                      \
    {                                                           \
        *(_Result) = 1;                                         \
    }                                                           \
    else if (*(USHORT UNALIGNED *)(_A) <                        \
                *(USHORT UNALIGNED *)(_B))                      \
    {                                                           \
        *(_Result) = (UINT)-1;                                  \
    }                                                           \
    else                                                        \
    {                                                           \
        *(_Result) = 0;                                         \
    }                                                           \
}

//
// This macro will compare network addresses.
//
//  A - Is a network address.
//
//  B - Is a network address.
//
//  Result - The result of comparing two network address.
//
//  Result != 0 Implies inequality.
//  Result == 0 Implies equality.
//
//
#define ETH_COMPARE_NETWORK_ADDRESSES_EQ(_A,_B, _Result)        \
{                                                               \
    if ((*(ULONG UNALIGNED *)&(_A)[2] ==                        \
            *(ULONG UNALIGNED *)&(_B)[2]) &&                    \
         (*(USHORT UNALIGNED *)(_A) ==                          \
            *(USHORT UNALIGNED *)(_B)))                         \
    {                                                           \
        *(_Result) = 0;                                         \
    }                                                           \
    else                                                        \
    {                                                           \
        *(_Result) = 1;                                         \
    }                                                           \
}


//
// This macro is used to copy from one network address to
// another.
//
#define ETH_COPY_NETWORK_ADDRESS(_D, _S) \
{ \
    *((ULONG UNALIGNED *)(_D)) = *((ULONG UNALIGNED *)(_S)); \
    *((USHORT UNALIGNED *)((UCHAR *)(_D)+4)) = *((USHORT UNALIGNED *)((UCHAR *)(_S)+4)); \
}

typedef struct _ArpHdr {
	unsigned short	ar_hrd;		/* format of hardware address */
#define ARPHRD_ETHER 	1	/* ethernet hardware format */
#define ARPHRD_IEEE802	6	/* token-ring hardware format */
#define ARPHRD_FRELAY 	15	/* frame relay hardware format */
	unsigned short	ar_pro;		/* format of protocol address */
	unsigned char	ar_hln;		/* length of hardware address */
	unsigned char	ar_pln;		/* length of protocol address */
	unsigned short	ar_op;		/* one of: */
#define	ARPOP_REQUEST	1	/* request to resolve address */
#define	ARPOP_REPLY	2	/* response to previous request */
#define	ARPOP_REVREQUEST 3	/* request protocol address given hardware */
#define	ARPOP_REVREPLY	4	/* response giving protocol address */
#define ARPOP_INVREQUEST 8 	/* request to identify peer */
#define ARPOP_INVREPLY	9	/* response identifying peer */
	/*
	* The remaining fields are variable in size,
	* according to the sizes above.
	*/
#ifdef COMMENT_ONLY
	unsigned char	ar_sha[ARP_802_ADDR_LENGTH];	/* sender hardware address */
	unsigned char	ar_spa[IP_V4_ADDR_LENGTH];	/* sender protocol address */
	unsigned char	ar_tha[ARP_802_ADDR_LENGTH];	/* target hardware address */
	unsigned char	ar_tpa[IP_V4_ADDR_LENGTH];	/* target protocol address */
#endif
}ArpHdr,*PArpHdr;

/*
* Ethernet Address Resolution Protocol.
*
* See RFC 826 for protocol description.  Structure below is adapted
* to resolving internet addresses.  Field names used correspond to 
* RFC 826.
*/
typedef struct _EtherArp {
	ArpHdr ea_hdr;	/* fixed-size header */
	unsigned char	arp_sha[ARP_802_ADDR_LENGTH];	/* sender hardware address */
	unsigned char	arp_spa[IP_V4_ADDR_LENGTH];	/* sender protocol address */
	unsigned char	arp_tha[ARP_802_ADDR_LENGTH];	/* target hardware address */
	unsigned char	arp_tpa[IP_V4_ADDR_LENGTH];	/* target protocol address */
}EtherArp;
#define	arp_hrd	ea_hdr.ar_hrd
#define	arp_pro	ea_hdr.ar_pro
#define	arp_hln	ea_hdr.ar_hln
#define	arp_pln	ea_hdr.ar_pln
#define	arp_op	ea_hdr.ar_op

#define SHA(ap) ((ap)->arp_sha)
#define THA(ap) ((ap)->arp_tha)
#define SPA(ap) ((ap)->arp_spa)
#define TPA(ap) ((ap)->arp_tpa)

/* Compatibility */
#ifndef REVARP_REQUEST
#define REVARP_REQUEST		3
#endif
#ifndef REVARP_REPLY
#define REVARP_REPLY		4
#endif

typedef struct _HttpUri
{
	unsigned char *uri;
	unsigned short length;
} HttpUri;

/* ip option type codes */
#ifndef IPOPT_EOL
#define IPOPT_EOL            0x00
#endif

#ifndef IPOPT_NOP
#define IPOPT_NOP            0x01
#endif

#ifndef IPOPT_RR
#define IPOPT_RR             0x07
#endif

#ifndef IPOPT_RTRALT
#define IPOPT_RTRALT         0x14
#endif

#ifndef IPOPT_TS
#define IPOPT_TS             0x44
#endif

#ifndef IPOPT_SECURITY
#define IPOPT_SECURITY       0x82
#endif

#ifndef IPOPT_LSRR
#define IPOPT_LSRR           0x83
#endif

#ifndef IPOPT_LSRR_E
#define IPOPT_LSRR_E         0x84
#endif

#ifndef IPOPT_SATID
#define IPOPT_SATID          0x88
#endif

#ifndef IPOPT_SSRR
#define IPOPT_SSRR           0x89
#endif

#define IPPROTO_IP              0               /* dummy for IP */
#define IPPROTO_ICMP            1               /* control message protocol */
#define IPPROTO_IGMP            2               /* internet group management protocol */
#define IPPROTO_GGP             3               /* gateway^2 (deprecated) */
#define IPPROTO_TCP             6               /* tcp */
#define IPPROTO_PUP             12              /* pup */
#define IPPROTO_UDP             17              /* user datagram protocol */
#define IPPROTO_IDP             22              /* xns idp */
#define IPPROTO_IPV6            41              /* IPv6 */
#define IPPROTO_ND              77              /* UNOFFICIAL net disk proto */
#define IPPROTO_ICLFXBM         78

#define IPPROTO_RAW             255             /* raw IP packet */
#define IPPROTO_MAX             256

typedef struct _IPHdr
{
#ifndef _NDIS_LITTLE_ENDIAN
    unsigned char ip_ver:4,  /* IP version */
    ip_hlen:4;          /* IP header length */
#else
    unsigned char ip_hlen:4, ip_ver:4;
#endif
    unsigned char ip_tos;        /* type of service */
    unsigned short ip_len;       /* datagram length */
    unsigned short ip_id;        /* identification  */
    unsigned short ip_off;       /* fragment offset */
    unsigned char ip_ttl;        /* time to live field */
    unsigned char ip_proto;      /* datagram protocol */
    unsigned short ip_csum;      /* checksum */
    unsigned long	ip_src;  /* source IP */
    unsigned long	ip_dst;  /* dest IP */
} IPHdr;

/*
 * tcp options stuff. used to be in <netinet/tcp.h> but it breaks
 * things on AIX
 */
#define TCPOPT_EOL              0
#define TCPOPT_NOP              1
#define TCPOPT_MAXSEG           2
#define TCPOLEN_MAXSEG          4
#define TCPOPT_WINDOW           3
#define TCPOLEN_WINDOW          3
#define TCPOPT_SACK_PERMITTED   4    /* Experimental */
#define TCPOLEN_SACK_PERMITTED  2
#define TCPOPT_SACK             5    /* Experimental */
#define TCPOPT_TIMESTAMP        8
#define TCPOLEN_TIMESTAMP       10
#define TCPOLEN_TSTAMP_APPA     (TCPOLEN_TIMESTAMP+2)    /* appendix A */
/* tcp option codes */
#define TOPT_EOL                0x00
#define TOPT_NOP                0x01
#define TOPT_MSS                0x02
#define TOPT_WS                 0x03
#define TOPT_TS                 0x08
#ifndef TCPOPT_WSCALE
    #define TCPOPT_WSCALE           3     /* window scale factor (rfc1072) */
#endif
#ifndef TCPOPT_SACKOK
    #define    TCPOPT_SACKOK        4     /* selective ack ok (rfc1072) */
#endif
#ifndef TCPOPT_SACK
    #define    TCPOPT_SACK          5     /* selective ack (rfc1072) */
#endif
#ifndef TCPOPT_ECHO
    #define TCPOPT_ECHO             6     /* echo (rfc1072) */
#endif
#ifndef TCPOPT_ECHOREPLY
    #define TCPOPT_ECHOREPLY        7     /* echo (rfc1072) */
#endif
#ifndef TCPOPT_TIMESTAMP
    #define TCPOPT_TIMESTAMP        8     /* timestamps (rfc1323) */
#endif
#ifndef TCPOPT_CC
    #define TCPOPT_CC               11    /* T/TCP CC options (rfc1644) */
#endif
#ifndef TCPOPT_CCNEW
    #define TCPOPT_CCNEW            12    /* T/TCP CC options (rfc1644) */
#endif
#ifndef TCPOPT_CCECHO
    #define TCPOPT_CCECHO           13    /* T/TCP CC options (rfc1644) */
#endif

typedef struct _TCPHdr
{
    unsigned short th_sport;     /* source port */
    unsigned short th_dport;     /* destination port */
    unsigned long th_seq;       /* sequence number */
    unsigned long th_ack;       /* acknowledgement number */
#ifndef _NDIS_LITTLE_ENDIAN
    unsigned char th_off:4,      /* data offset */
    th_x2:4;       /* (unused) */
#else
    unsigned char th_x2:4, th_off:4;
#endif
    unsigned char th_flags;
    unsigned short th_win;       /* window */
    unsigned short th_sum;       /* checksum */
    unsigned short th_urp;       /* urgent pointer */

} TCPHdr;

typedef struct _UDPHdr
{
	unsigned short uh_sport;
	unsigned short uh_dport;
	unsigned short uh_len;
	unsigned short uh_chk;
} UDPHdr;

#define ICMP_ECHOREPLY          0    /* Echo Reply                   */
#define ICMP_DEST_UNREACH       3    /* Destination Unreachable      */
#define ICMP_SOURCE_QUENCH      4    /* Source Quench                */
#define ICMP_REDIRECT           5    /* Redirect (change route)      */
#define ICMP_ECHO               8    /* Echo Request                 */
#define ICMP_ROUTER_ADVERTISE   9    /* Router Advertisement         */
#define ICMP_ROUTER_SOLICIT     10    /* Router Solicitation          */
#define ICMP_TIME_EXCEEDED      11    /* Time Exceeded                */
#define ICMP_PARAMETERPROB      12    /* Parameter Problem            */
#define ICMP_TIMESTAMP          13    /* Timestamp Request            */
#define ICMP_TIMESTAMPREPLY     14    /* Timestamp Reply              */
#define ICMP_INFO_REQUEST       15    /* Information Request          */
#define ICMP_INFO_REPLY         16    /* Information Reply            */
#define ICMP_ADDRESS            17    /* Address Mask Request         */
#define ICMP_ADDRESSREPLY       18    /* Address Mask Reply           */
#define NR_ICMP_TYPES           18

/* Codes for UNREACH. */
#define ICMP_NET_UNREACH	0	/* Network Unreachable		*/
#define ICMP_HOST_UNREACH	1	/* Host Unreachable		*/
#define ICMP_PROT_UNREACH	2	/* Protocol Unreachable		*/
#define ICMP_PORT_UNREACH	3	/* Port Unreachable		*/
#define ICMP_FRAG_NEEDED	4	/* Fragmentation Needed/DF set	*/
#define ICMP_SR_FAILED		5	/* Source Route failed		*/
#define ICMP_NET_UNKNOWN	6
#define ICMP_HOST_UNKNOWN	7
#define ICMP_HOST_ISOLATED	8
#define ICMP_NET_ANO		9
#define ICMP_HOST_ANO		10
#define ICMP_NET_UNR_TOS	11
#define ICMP_HOST_UNR_TOS	12
#define ICMP_PKT_FILTERED	13	/* Packet filtered */
#define ICMP_PREC_VIOLATION	14	/* Precedence violation */
#define ICMP_PREC_CUTOFF	15	/* Precedence cut off */
#define NR_ICMP_UNREACH		15	/* instead of hardcoding immediate value */

/* Codes for REDIRECT. */
#define ICMP_REDIR_NET		0	/* Redirect Net			*/
#define ICMP_REDIR_HOST		1	/* Redirect Host		*/
#define ICMP_REDIR_NETTOS	2	/* Redirect Net for TOS		*/
#define ICMP_REDIR_HOSTTOS	3	/* Redirect Host for TOS	*/

/* Codes for TIME_EXCEEDED. */
#define ICMP_EXC_TTL		0	/* TTL count exceeded		*/
#define ICMP_EXC_FRAGTIME	1	/* Fragment Reass time exceeded	*/

/* PARAMPROB code */
#define	ICMP_PARAMPROB_OPTABSENT 1		/* req. opt. absent */

/* ECHO code */
#define ICMPV4_ECHO_REQUEST_CODE   0
/* ECHO_REPLY code */
#define ICMPV4_ECHO_REPLY_CODE     0

// ICPM6 types and codes
#define ICMPV6_ECHO_REQUEST_TYPE   128
#define ICMPV6_ECHO_REQUEST_CODE   0
#define ICMPV6_ECHO_REPLY_TYPE     129
#define ICMPV6_ECHO_REPLY_CODE     0

typedef struct _ICMPHdr
{
    unsigned char type;
    unsigned char code;
    unsigned short csum;
    union
    {
        unsigned char pptr;

        unsigned long gwaddr;

        struct idseq
        {
            unsigned short id;
            unsigned short seq;
        } idseq;

        int sih_void;

        struct pmtu 
        {
            unsigned short ipm_void;
            unsigned short nextmtu;
        } pmtu;

        struct rtradv 
        {
            unsigned char num_addrs;
            unsigned char wpa;
            unsigned short lifetime;
        } rtradv;
    } icmp_hun;

#define s_icmp_pptr       icmp_hun.pptr
#define s_icmp_gwaddr     icmp_hun.gwaddr
#define s_icmp_id         icmp_hun.idseq.id
#define s_icmp_seq        icmp_hun.idseq.seq
#define s_icmp_void       icmp_hun.sih_void
#define s_icmp_pmvoid     icmp_hun.pmtu.ipm_void
#define s_icmp_nextmtu    icmp_hun.pmtu.nextmtu
#define s_icmp_num_addrs  icmp_hun.rtradv.num_addrs
#define s_icmp_wpa        icmp_hun.rtradv.wpa
#define s_icmp_lifetime   icmp_hun.rtradv.lifetime

    union 
    {
        /* timestamp */
        struct ts 
        {
            unsigned long otime;
            unsigned long rtime;
            unsigned long ttime;
        } ts;
        
        /* IP header for unreach */
        struct ih_ip  
        {
            IPHdr *ip;
            /* options and then 64 bits of data */
        } ip;
        
        struct ra_addr 
        {
            unsigned long addr;
            unsigned long preference;
        } radv;

        unsigned long mask;

        char    data[1];

    } icmp_dun;
#define s_icmp_otime      icmp_dun.ts.otime
#define s_icmp_rtime      icmp_dun.ts.rtime
#define s_icmp_ttime      icmp_dun.ts.ttime
#define s_icmp_ip         icmp_dun.ih_ip
#define s_icmp_radv       icmp_dun.radv
#define s_icmp_mask       icmp_dun.mask
#define s_icmp_data       icmp_dun.data

} ICMPHdr;

typedef struct _IGMPHdr
{
	unsigned char	igmp_type;
#define	IGMP_MEMBERSHIP_QUERY		0x11	// membership query
#define	IGMP_V1_MEMBERSHIP_REPORT	0x12	// Ver. 1 membership report
#define	IGMP_V2_MEMBERSHIP_REPORT	0x16	// Ver. 2 membership report
#define	IGMP_LEAVE_GROUP			0x17	// Leave-group message
	unsigned char	igmp_code;
	unsigned short	igmp_sum;
    struct	_m_in_addr	igmp_group;
} IGMPHdr;

typedef struct _echoext
{
	unsigned short id;
	unsigned short seqno;
} echoext;

//////////////////////////////////////////////////////////////////////////
//GRE
/* GRE PROTOCOL HEADER */

/* GRE Version field */
#define GRE_VERSION_1701	0x0
#define GRE_VERSION_PPTP	0x1

#define GRE_VERSION_0                           0x0
#define GRE_VERSION_1                           0x1


/* GRE Protocol field */
#define GRE_PROTOCOL_PPTP	0x880B
#define GRE_IP4PROTO	0x800		/* Required by RFC */

#define GRE_CHECKSUM	0x8000		/* Use the checksum field */

/* GRE Flags */
#define GRE_CSUM                            0x8000
#define GRE_ROUTING                         0x4000
#define GRE_KEY                             0x2000
#define GRE_SEQ                             0x1000
#define GRE_STRICT                          0x0800
#define GRE_REC                             0x0700
#define GRE_ACK                             0x0080

#define GRE_IS_CSUM(f)	  ((f)&GRE_CSUM)
#define GRE_IS_ROUTING(f) ((f)&GRE_ROUTING)
#define GRE_IS_KEY(f)	  ((f)&GRE_KEY)
#define GRE_IS_SEQ(f)	  ((f)&GRE_SEQ)
#define GRE_IS_ACK(f)     ((f)&GRE_ACK)

#ifndef IPPROTO_GRE
	#define IPPROTO_GRE 47
#endif

#define GRE_SNA                                 0x0004
#define GRE_OSI_NETWORK_LAYER                   0x00FE
#define GRE_PUP                                 0x0200
#define GRE_XNS                                 0x0600
#define GRE_IP                                  0x0800
#define GRE_CHAOS                               0x0804
#define GRE_RFC_826_ARP                         0x0806
#define GRE_FRAME_RELAY_ARP                     0x0808
#define GRE_VINES                               0x0BAD
#define GRE_VINES_ECHO                          0x0BAE
#define GRE_VINES_LOOPBACK                      0x0BAF
#define GRE_DECNET                              0x6003
#define GRE_TRANSPARENT_ETHERNET_BRIDGING       0x6558
#define GRE_RAW_FRAME_RELAY                     0x6559
#define GRE_APOLLO_DOMAIN                       0x8019
#define GRE_ETHERTALK                           0x809B
#define GRE_NOVELL_IPX                          0x8137
#define GRE_RFC_1144_TCP_IP_COMPRESSION         0x876B
#define GRE_IP_AUTONOMOUS_SYSTEMS               0x876C
#define GRE_SECURE_DATA                         0x876D
#define GRE_PPP                                 0x880b /* taken from RFC 2637 */


/* GRE is a mess: Four different standards */
typedef struct _GreHdr {
	union{
		unsigned short	rec:3,
						srr:1,
						seq:1,
						key:1,
						routing:1,
						csum:1,
						version:3,
						reserved:4,
						ack:1;
		unsigned short	flags;
	};
	unsigned short	protocol;

	//union {
	// struct {
	//	 u_int16_t sum;  /* optional */
	//	 u_int16_t offset;    /* optional */
	//	 u_int32_t key;        /* optional */
	//	 u_int32_t seq;        /* optional */
	// } _gre;

	// struct {
	//	 u_int16_t payload_s; /* optional */
	//	 u_int16_t callID;    /* optional */
	//	 u_int32_t seq;        /* optional */
	//	 u_int32_t ack;        /* optional */
	// } _egre;
	//}_data;
 
 #define gre_sum _data._gre.sum
 #define gre_offset _data._gre.offset
 #define gre_key _data._gre.key
 #define gre_seq _data._gre.seq

 #define egre_payload_s _data._egre.payload_s
 #define egre_callID _data._egre.callID
 #define egre_seq _data._egre.seq
 #define egre_ack _data._egre.ack
}GreHdr;

/* GRE is a mess: Four different standards */
typedef struct _GreTunnelHdr {
	union{
		struct{
			unsigned short	rec:3,
							srr:1,
							seq:1,
							key:1,
							routing:1,
							csum:1,
							version:3,
							reserved:4,
							ack:1;
		};
		unsigned short	flags;
	};
	unsigned short	protocol;
	union {
	 struct {
		 unsigned long key;        /* optional */
	 } _gre;

	 struct {
		 unsigned short payload_s; /* optional */
		 unsigned short callID;    /* optional */
	 } _egre;
	}_data;
}GreTunnelHdr;

/* modified GRE header for PPTP */
typedef struct _GreHdrPptp {
	unsigned char  flags;		/* bitfield */
	unsigned char  version;		/* should be GRE_VERSION_PPTP */
	unsigned short protocol;		/* should be GRE_PROTOCOL_PPTP */
	unsigned short payload_len;	/* size of ppp payload, not inc. gre header */
	unsigned short call_id;		/* peer's call_id for this session */
	unsigned long  seq;		/* sequence number.  Present if S==1 */
	unsigned long  ack;		/* seq number of highest packet received by */
	/*  sender in this session */
}GreHdrPptp;

typedef struct _GreHdrSre
{
    unsigned short af;  /* address family */
    unsigned char sre_offset;
    unsigned char sre_length;
    unsigned char *routing;
}GreHdrSre;



typedef struct _ARPHdr
{
	unsigned short ar_hrd;       /* format of hardware address   */
	unsigned short ar_pro;       /* format of protocol address   */
	unsigned char ar_hln;        /* length of hardware address   */
	unsigned char ar_pln;        /* length of protocol address   */
	unsigned short ar_op;        /* ARP opcode (command)         */
} ARPHdr;

typedef struct _EtherARP
{
	ARPHdr ea_hdr;      /* fixed-size header */
	unsigned char arp_sha[6];    /* sender hardware address */
	unsigned char arp_spa[4];    /* sender protocol address */
	unsigned char arp_tha[6];    /* target hardware address */
	unsigned char arp_tpa[4];    /* target protocol address */
} EtherARP;

typedef struct _EtherARPpadding
{
	EtherARP		EtherARP;
	unsigned char	Padding[0x12];   //as seen in Network Monitor

} EtherARPpadding;

typedef struct _Options
{
	unsigned char code;
	long          len;
	unsigned char *data;
} Options;

#define IPPROTO_VRRP 112

typedef struct _VRRPhdr
{
	unsigned char	vrrp_v:4,				// protocol version
			vrrp_t:4;				// packet type
#define	VRRP_TYPE_ADVERT	0x1
	unsigned char vrrp_vrouter_id;			// virtual router id
	unsigned char vrrp_priority;				// priority
	unsigned char vrrp_ip_count;				// number of IP addresses
	unsigned char vrrp_auth_type;			// authorization type
#define VRRP_AUTH_NONE		0x1
#define VRRP_AUTH_PASSWD	0x2
#define VRRP_AUTH_IPAH		0x3
	unsigned char vrrp_advert_int;			// advertisement interval
	unsigned short vrrp_sum;					// checksum
} VRRPhdr;

// OSPF definitions
#define IPPROTO_OSPF		89
#define IPPROTO_OSPF_LSA	890		// Made this up. Hope it's unused
#define OSPFVERSION			2
#define MODX				4102	// Used in LSA checksum

// OSPF header lengths.
#define OSPF_H		0x10	// 16 bytes
#define HELLO_H		0x18	// 24 bytes
#define DBD_H		0x8		// 8 bytes
#define LSR_H		0xc		// 12 bytes
#define LSU_H		0x4		// 4 bytes
#define LSA_H		0x14	// 20 bytes
#define AUTH_H		0x8		// 8 bytes
#define OSPF_CKSUM	0x10	// 16 bytes

typedef struct _OSPFhdr
{
    unsigned char ospf_v;					// version
    unsigned char ospf_type;					// type
#define  OSPF_UMD			0		// UMd monitoring packet
#define  OSPF_HELLO			1		// HELLO packet
#define  OSPF_DBD			2		// DataBase Description Packet
#define  OSPF_LSR			3		// Link State Request Packet
#define  OSPF_LSU			4		// Link State Update Packet
#define  OSPF_LSA			5		// Link State Acknowledgement Packet
    unsigned short   ospf_len;				// length
    struct _m_in_addr  ospf_rtr_id;	// source router ID
    struct _m_in_addr  ospf_area_id;// roam ID
    unsigned short ospf_cksum;				// checksum
    unsigned short ospf_auth_type;			// authentication type
#define OSPF_AUTH_NULL		0		// Null password
#define OSPF_AUTH_SIMPLE	1		// Simple, plaintext, 8 char password
#define OSPF_AUTH_MD5		2		// MD5
} OSPFhdr;

// Link State Request header.
typedef struct _LSRhdr
{
    unsigned long lsr_type;					// Type of LS being requested (see below) 
    unsigned long lsr_lsid;					// Link State ID 
    struct _m_in_addr lsr_adrtr;	// advertising router (memcpy more as needed) 
} LSRhdr;

//  Link State Update header.
typedef struct _LSUhdr
{
    unsigned long lsu_num;              // Number of LSAs that will be broadcasted 
} LSUhdr;

//  Link State Acknowledgement header.
typedef struct _LSAhdr
{
    unsigned short lsa_age;        // Time in seconds since the LSA was originated 
    unsigned char lsa_opts;        // Look above for OPTS_* 
    unsigned char lsa_type;        // Look below for LS_TYPE_* 
    unsigned long lsa_id;           // Link State ID 
    struct _m_in_addr lsa_adv;	// Router ID of Advertising router 
    unsigned long lsa_seq;          // LSA sequence number to detect old/bad ones 
    unsigned char lsa_cksum[2];    // "Fletcher Checksum" of all fields minus age 
    unsigned short lsa_len;        // Length in bytes including the 20 byte header 
} LSAhdr;

//  Router LSA data format
//  Other stuff for TOS can be added for backward compatability,
//  for this version, only OSPFv2 is being FULLY supported.
struct rtr_lsa_hdr
{
    unsigned short rtr_flags;				// Set to help describe packet 
#define RTR_FLAGS_W 0x0100	// W bit 
#define RTR_FLAGS_E 0x0200	// E bit 
#define RTR_FLAGS_B 0x0400	// B bit 
    unsigned short rtr_num;				// Number of links within that packet 
    unsigned long rtr_link_id;				// Describes link_data (look below) 
#define LINK_ID_NBR_ID 1		// Neighbors router ID, also can be 4 
#define LINK_ID_IP_DES 2		// IP address of designated router 
#define LINK_ID_SUB    3		// IP subnet number 
    unsigned long rtr_link_data;			// Depending on link_id, info is here 
    unsigned char rtr_type;				// Description of router link 
#define RTR_TYPE_PTP   1		// Point-To-Point 
#define RTR_TYPE_TRANS 2		// Connection to a "transit network" 
#define RTR_TYPE_STUB  3		// Connectin to a "stub network" 
#define RTR_TYPE_VRTL         4		// Connects to a "virtual link" 
    unsigned char rtr_tos_num;				// Number of different TOS metrics for this link 
    unsigned short rtr_metric;				// The "cost" of using this link 
};

// Network LSA data format.
typedef struct _NetLSAhdr
{
    struct _m_in_addr net_nmask;   // Netmask for that network 
    unsigned long  net_rtr_id;          // ID of router attached to that network 
} NetLSAhdr;

// Summary LSA data format.
typedef struct _SumLSAhdr
{
    struct _m_in_addr sum_nmask;   // Netmask of destination IP address 
    unsigned long  sum_metric;          // Same as in rtr_lsa (&0xfff to use last 24bit) 
    unsigned long  sum_tos_metric;      // first 8bits are TOS, 24bits are TOS Metric 
} SumLSAhdr;

//  AS External LSA data format.
//  & 0xfff logic operator for as_metric to get last 24bits.
typedef struct _AsLSAhdr
{
    struct _m_in_addr as_nmask;			// Netmask for advertised destination 
    unsigned long  as_metric;					// May have to set E bit in first 8bits 
#define AS_E_BIT_ON 0x80000000			// as_metric 
    struct _m_in_addr as_fwd_addr;		// Forwarding address 
    unsigned long  as_rte_tag;					// External route tag 
} AsLSAhdr;

//well known ports
#define WINS_PORT 42

#define NETBIOS_NAME_SERVICE 137
#define NETBIOS_DG_SERVICE 138

#define SMTP_PORT 25

#define PPTP_PORT 1723

#endif //__IPHRD_H_