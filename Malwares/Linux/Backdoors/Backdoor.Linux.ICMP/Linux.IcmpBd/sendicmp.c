/* Copyright (C) 2002  Martin J. Muench <mjm@codito.de> */                                   

#include "config.h"


void sendpacket(char *text, char *host, int seqnr) {
	int             pktsize;
	char		*packet, *data, stext[CHARS + strlen(SID)];
	
	memset(&stext,0,sizeof(stext));
        strncat(stext,SID,strlen(SID));
        strncat(stext,text,strlen(text));
  
	pktsize = sizeof(struct icmp) + sizeof(stext);
        
        if(!(packet = (char *)malloc(pktsize)))
		close(rawsock), perror("Malloc:"), exit(-1); 
                
        icmp = ( struct icmp * )packet;
        data = (char *)(packet + sizeof(struct icmp) );
	memset(packet,0,pktsize);
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=inet_addr(host);
	icmp->icmp_type = icmpcode;
	icmp->icmp_seq = seqnr;
	icmp->icmp_cksum = 0;
	strncat(data,stext,sizeof(stext));
 
	icmp->icmp_cksum = in_cksum((u_short *)icmp,sizeof(struct icmp) + sizeof(stext));

	if (sendto(rawsock, packet, pktsize, 0,  (struct sockaddr *)&saddr, sizeof(struct sockaddr)) < 0 )
		close(rawsock), perror("SendError:"), exit(-1);

}

/* Copied from FreeBSD ping.c  */
int in_cksum (u_short *addr, int len) {
	register int nleft = len;
	register u_short *w = addr;
	register int sum = 0;
	u_short answer = 0;
	while (nleft > 1) 
		sum += *w++, nleft -= 2;

	if (nleft == 1)
		*(u_char *)(&answer) = *(u_char *)w; sum += answer;
		
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}
