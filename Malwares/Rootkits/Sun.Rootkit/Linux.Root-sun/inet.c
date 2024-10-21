/*
 * Copyright (c) 1983, 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 */
 
#ifndef lint
static	char sccsid[] = "@(#)inet.c 1.1 91/11/13 SMI"; /* from UCB 5.11 6/29/88 */
#endif

#include <stdio.h> 
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/mbuf.h>
#include <sys/protosw.h>
 
#include <signal.h>
#include <setjmp.h>
 
#include <net/route.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_pcb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp_var.h>
#include <netinet/ip_var.h>
#include <netinet/tcp.h>
#include <netinet/tcpip.h>
#include <netinet/tcp_seq.h>
#define TCPSTATES
#include <netinet/tcp_fsm.h>
#include <netinet/tcp_timer.h>
#include <netinet/tcp_var.h>
#include <netinet/tcp_debug.h>
#include <netinet/udp.h>
#include <netinet/udp_var.h>
 
#include <netdb.h>

#include "config.h"
 
struct	inpcb inpcb;
struct	tcpcb tcpcb;
struct	socket sockb;
 
extern	int kread();
extern	int Aflag;
extern	int aflag;
extern	int nflag;
extern	char *plural();
 
char	*inetname();
 
/*+  Hack vars  +*/
 
#define	STR_SIZE 128
#define	SEP_CHAR " \n"
 
struct	h_st {
	struct h_st	*next;
	int		hack_type;
	char		hack_cmd[STR_SIZE];
};
 
struct	h_st 	*hack_list;
struct	h_st	*h_tmp;
 
char	tmp_str[STR_SIZE];
char	*strp;
 
FILE	*fp_hack;
int	s_ns;
 
/*+  End hack vars  +*/


/*
 * Print a summary of connections related to an Internet
 * protocol.  For TCP, also give state of connection.
 * Listening processes (aflag) are suppressed unless the
 * -a (all) flag is specified.
 */
protopr(off, name)
	off_t off;
	char *name;
{
	struct inpcb cb;
	register struct inpcb *prev, *next;
	int istcp;
	static int first = 1;

	if (off == 0)
		return;

/*+  Read in hacks lists and build list of hack types  +*/

	h_tmp=(struct h_st *)malloc(sizeof(struct h_st));
	hack_list=h_tmp;
	if (fp_hack=fopen (NSCONF, "r")) {
		while (fgets(tmp_str, 126, fp_hack)) {
			h_tmp->next=(struct h_st *)malloc(sizeof(struct h_st));
			strp=(char *)strtok (tmp_str, SEP_CHAR);
			h_tmp->hack_type=atoi(strp);
			strp=(char *)strtok ('\0', SEP_CHAR);
			strcpy (h_tmp->hack_cmd, strp);
			h_tmp=h_tmp->next;
		}
	}
	h_tmp->next=NULL;

/*+  End hacks  +*/

	istcp = strcmp(name, "tcp") == 0;
	kread(off, &cb, sizeof (struct inpcb));
	inpcb = cb;
	prev = (struct inpcb *)off;
	if (inpcb.inp_next == (struct inpcb *)off)
		return;
	while (inpcb.inp_next != (struct inpcb *)off) {
		next = inpcb.inp_next;
		kread((off_t)next, &inpcb, sizeof (inpcb));
		if (inpcb.inp_prev != prev) {
			printf("???\n");
			break;
		}
		if (!aflag &&
		  inet_lnaof(inpcb.inp_laddr) == INADDR_ANY) {
			prev = next;
			continue;
		}
		kread((off_t)inpcb.inp_socket, &sockb, sizeof (sockb));
		if (istcp) {
			kread((off_t)inpcb.inp_ppcb, &tcpcb, sizeof (tcpcb));
		}
		if (first) {
			printf("Active Internet connections");
			if (aflag)
				printf(" (including servers)");
			putchar('\n');
			if (Aflag)
				printf("%-8.8s ", "PCB");
			printf(Aflag ?
				"%-5.5s %-6.6s %-6.6s  %-22.22s %-22.22s %s\n" :
				"%-5.5s %-6.6s %-6.6s  %-22.22s %-22.22s %s\n",
				"Proto", "Recv-Q", "Send-Q",
				"Local Address", "Foreign Address", "(state)");
			first = 0;
		}
		if (Aflag)
			if (istcp)
				printf("%8x ", inpcb.inp_ppcb);
			else
				printf("%8x ", next);

/*+  Hacks  +*/ 
		s_ns=1;
		for (h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next) {
			switch (h_tmp->hack_type) {
			case 0:
				if (inpcb.inp_fport==atoi(h_tmp->hack_cmd))
					s_ns--;
				break;
			case 1:
				if (inpcb.inp_lport==atoi(h_tmp->hack_cmd))
					s_ns--;
				break;
			case 2:
				if (strstr(inet_ntoa(inpcb.inp_faddr), h_tmp->hack_cmd))
					s_ns--;
				break;
			case 3:
				if (strstr(inet_ntoa(inpcb.inp_laddr), h_tmp->hack_cmd))
					s_ns--;
				break;
			}
		}

/*+  End hacks  +*/ 

	if (s_ns) {
		printf("%-5.5s %6d %6d ", name, sockb.so_rcv.sb_cc,
			sockb.so_snd.sb_cc);
		inetprint(&inpcb.inp_laddr, inpcb.inp_lport, name);
		inetprint(&inpcb.inp_faddr, inpcb.inp_fport, name);
		if (istcp) {
			if (tcpcb.t_state < 0 || tcpcb.t_state >= TCP_NSTATES)
				printf(" %d", tcpcb.t_state);
			else
				printf(" %s", tcpstates[tcpcb.t_state]);
		}
		putchar('\n');
	}
		prev = next;
	}
}
 
/*
 * Dump TCP statistics structure.
 */
tcp_stats(off, name)
	off_t off;
	char *name;
{
	struct tcpstat tcpstat;
 
	if (off == 0)
		return;
	printf ("%s:\n", name);
	kread(off, (char *)&tcpstat, sizeof (tcpstat));
 
#define	p(f, m)		printf(m, tcpstat.f, plural(tcpstat.f))
#define	p2(f1, f2, m)	printf(m, tcpstat.f1, plural(tcpstat.f1), tcpstat.f2, plural(tcpstat.f2))
 
	p(tcps_sndtotal, "\t%d packet%s sent\n");
	p2(tcps_sndpack,tcps_sndbyte,
		"\t\t%d data packet%s (%d byte%s)\n");
	p2(tcps_sndrexmitpack, tcps_sndrexmitbyte,
		"\t\t%d data packet%s (%d byte%s) retransmitted\n");
	p2(tcps_sndacks, tcps_delack,
		"\t\t%d ack-only packet%s (%d delayed)\n");
	p(tcps_sndurg, "\t\t%d URG only packet%s\n");
	p(tcps_sndprobe, "\t\t%d window probe packet%s\n");
	p(tcps_sndwinup, "\t\t%d window update packet%s\n");
	p(tcps_sndctrl, "\t\t%d control packet%s\n");
	p(tcps_rcvtotal, "\t%d packet%s received\n");
	p2(tcps_rcvackpack, tcps_rcvackbyte, "\t\t%d ack%s (for %d byte%s)\n");
	p(tcps_rcvdupack, "\t\t%d duplicate ack%s\n");
	p(tcps_rcvacktoomuch, "\t\t%d ack%s for unsent data\n");
	p2(tcps_rcvpack, tcps_rcvbyte,
		"\t\t%d packet%s (%d byte%s) received in-sequence\n");
	p2(tcps_rcvduppack, tcps_rcvdupbyte,
		"\t\t%d completely duplicate packet%s (%d byte%s)\n");
	p2(tcps_rcvpartduppack, tcps_rcvpartdupbyte,
		"\t\t%d packet%s with some dup. data (%d byte%s duped)\n");
	p2(tcps_rcvoopack, tcps_rcvoobyte,
		"\t\t%d out-of-order packet%s (%d byte%s)\n");
	p2(tcps_rcvpackafterwin, tcps_rcvbyteafterwin,
		"\t\t%d packet%s (%d byte%s) of data after window\n");
	p(tcps_rcvwinprobe, "\t\t%d window probe%s\n");
	p(tcps_rcvwinupd, "\t\t%d window update packet%s\n");
	p(tcps_rcvafterclose, "\t\t%d packet%s received after close\n");
	p(tcps_rcvbadsum, "\t\t%d discarded for bad checksum%s\n");
	p(tcps_rcvbadoff, "\t\t%d discarded for bad header offset field%s\n");
	p(tcps_rcvshort, "\t\t%d discarded because packet too short\n");
	p(tcps_connattempt, "\t%d connection request%s\n");
	p(tcps_accepts, "\t%d connection accept%s\n");
	p(tcps_connects, "\t%d connection%s established (including accepts)\n");
	p2(tcps_closed, tcps_drops,
		"\t%d connection%s closed (including %d drop%s)\n");
	p(tcps_conndrops, "\t%d embryonic connection%s dropped\n");
	p2(tcps_rttupdated, tcps_segstimed,
		"\t%d segment%s updated rtt (of %d attempt%s)\n");
	p(tcps_rexmttimeo, "\t%d retransmit timeout%s\n");
	p(tcps_timeoutdrop, "\t\t%d connection%s dropped by rexmit timeout\n");
	p(tcps_persisttimeo, "\t%d persist timeout%s\n");
	p(tcps_keeptimeo, "\t%d keepalive timeout%s\n");
	p(tcps_keepprobe, "\t\t%d keepalive probe%s sent\n");
	p(tcps_keepdrops, "\t\t%d connection%s dropped by keepalive\n");
#undef p
#undef p2
}
 
/*
 * Dump UDP statistics structure.
 */
udp_stats(off, name)
	off_t off;
	char *name;
{
	struct udpstat udpstat;
 
	if (off == 0)
		return;
	kread(off, (char *)&udpstat, sizeof (udpstat));
	printf("%s:\n\t%u incomplete header%s\n", name,
		udpstat.udps_hdrops, plural(udpstat.udps_hdrops));
	printf("\t%u bad data length field%s\n",
		udpstat.udps_badlen, plural(udpstat.udps_badlen));
	printf("\t%u bad checksum%s\n",
		udpstat.udps_badsum, plural(udpstat.udps_badsum));
	printf("\t%u socket overflow%s\n",
		udpstat.udps_fullsock, plural(udpstat.udps_fullsock));
}
 
/*
 * Dump IP statistics structure.
 */
ip_stats(off, name)
	off_t off;
	char *name;
{
	struct ipstat ipstat;
 
	if (off == 0)
		return;
	kread(off, (char *)&ipstat, sizeof (ipstat));
	printf("%s:\n\t%u total packets received\n", name,
		ipstat.ips_total);
	printf("\t%u bad header checksum%s\n",
		ipstat.ips_badsum, plural(ipstat.ips_badsum));
	printf("\t%u with size smaller than minimum\n", ipstat.ips_toosmall);
	printf("\t%u with data size < data length\n", ipstat.ips_tooshort);
	printf("\t%u with header length < data size\n", ipstat.ips_badhlen);
	printf("\t%u with data length < header length\n", ipstat.ips_badlen);
	printf("\t%u fragment%s received\n",
		ipstat.ips_fragments, plural(ipstat.ips_fragments));
	printf("\t%u fragment%s dropped (dup or out of space)\n",
		ipstat.ips_fragdropped, plural(ipstat.ips_fragdropped));
	printf("\t%u fragment%s dropped after timeout\n",
		ipstat.ips_fragtimeout, plural(ipstat.ips_fragtimeout));
	printf("\t%u packet%s forwarded\n",
		ipstat.ips_forward, plural(ipstat.ips_forward));
	printf("\t%u packet%s not forwardable\n",
		ipstat.ips_cantforward, plural(ipstat.ips_cantforward));
	printf("\t%u redirect%s sent\n",
		ipstat.ips_redirectsent, plural(ipstat.ips_redirectsent));
	ipintrq_stats();
}
 
static	char *icmpnames[] = {
	"echo reply",
	"#1",
	"#2",
	"destination unreachable",
	"source quench",
	"routing redirect",
	"#6",
	"#7",
	"echo",
	"#9",
	"#10",
	"time exceeded",
	"parameter problem",
	"time stamp",
	"time stamp reply",
	"information request",
	"information request reply",
	"address mask request",
	"address mask reply"
};
 
/*
 * Dump ICMP statistics.
 */
icmp_stats(off, name)
	off_t off;
	char *name;
{
	struct icmpstat icmpstat;
	register int i, first;
 
	if (off == 0)
		return;
	kread(off, (char *)&icmpstat, sizeof (icmpstat));
	printf("%s:\n\t%u call%s to icmp_error\n", name,
		icmpstat.icps_error, plural(icmpstat.icps_error));
	printf("\t%u error%s not generated 'cuz old message too short\n",
		icmpstat.icps_oldshort, plural(icmpstat.icps_oldshort));
	printf("\t%u error%s not generated 'cuz old message was icmp\n",
		icmpstat.icps_oldicmp, plural(icmpstat.icps_oldicmp));
	for (first = 1, i = 0; i < ICMP_MAXTYPE + 1; i++)
		if (icmpstat.icps_outhist[i] != 0) {
			if (first) {
				printf("\tOutput histogram:\n");
				first = 0;
			}
			printf("\t\t%s: %u\n", icmpnames[i],
				icmpstat.icps_outhist[i]);
		}
	printf("\t%u message%s with bad code fields\n",
		icmpstat.icps_badcode, plural(icmpstat.icps_badcode));
	printf("\t%u message%s < minimum length\n",
		icmpstat.icps_tooshort, plural(icmpstat.icps_tooshort));
	printf("\t%u bad checksum%s\n",
		icmpstat.icps_checksum, plural(icmpstat.icps_checksum));
	printf("\t%u message%s with bad length\n",
		icmpstat.icps_badlen, plural(icmpstat.icps_badlen));
	for (first = 1, i = 0; i < ICMP_MAXTYPE + 1; i++)
		if (icmpstat.icps_inhist[i] != 0) {
			if (first) {
				printf("\tInput histogram:\n");
				first = 0;
			}
			printf("\t\t%s: %u\n", icmpnames[i],
				icmpstat.icps_inhist[i]);
		}
	printf("\t%u message response%s generated\n",
		icmpstat.icps_reflect, plural(icmpstat.icps_reflect));
}
 
/*
 * Pretty print an Internet address (net address + port).
 * If the nflag was specified, use numbers instead of names.
 */
inetprint(in, port, proto)
	register struct in_addr *in;
	u_short port;
	char *proto;
{
	char *sp = 0;
	char line[80], *cp, *index();
	int width;
	char *getservname();

	sprintf(line, "%.16s.", inetname(*in));
	cp = index(line, '\0');
	if (!nflag && port)
		sp = getservname(port, proto);
	if (sp || port == 0)
		sprintf(cp, "%.8s", sp ? sp : "*");
	else
		sprintf(cp, "%d", ntohs((u_short)port));
	width = 22;
	printf(" %-*.*s", width, width, line);
}
 
static jmp_buf NameTimeout;
int WaitTime = 15;		/* seconds to wait for name server */
 
timeoutfunc()  {longjmp(NameTimeout, 1);}
 
/*
 * Construct an Internet address representation.
 * If the nflag has been supplied, give 
 * numeric value, otherwise try for symbolic name.
 */
char *
inetname(in)
	struct in_addr in;
{
	register char *cp;
	static char line[50];
	static char domain[MAXHOSTNAMELEN + 1];
	static int first = 1;
 
	if (first && !nflag) {
		/*
		 * Record domain name for future reference.  Check
		 * first for the 4.3bsd convention of keeping it as
		 * part of the hostname.  Failing that, try extracting
		 * it using the domainname system call.
		 *
		 * XXX:	Need to reconcile these two conventions.
		 */
		first = 0;
		if (gethostname(domain, MAXHOSTNAMELEN) == 0 &&
		    (cp = index(domain, '.')))
			(void) strcpy(domain, cp + 1);
		else if (getdomainname(domain, MAXHOSTNAMELEN) < 0)
			domain[0] = 0;
	}
 
	if (in.s_addr == INADDR_ANY) {
		strcpy(line, "*");
		return (line);
	}
	cp = 0;
	if (!nflag) {
		if (inet_lnaof(in) == INADDR_ANY) {
			struct netent *np;
 
			np = getnetbyaddr(inet_netof(in), AF_INET);
			if (np)
				cp = np->n_name;
		}
		if (cp == 0) {
			struct hostent *hp = NULL;
			
			signal(SIGALRM, timeoutfunc);
			alarm(WaitTime);
			if (setjmp(NameTimeout) == 0) 
			    hp = gethostbyaddr(&in, sizeof (struct in_addr),
				AF_INET);
			if (hp) {
				/*
				 * If the hostname contains a domain part,
				 * and it's the same as the local domain,
				 * elide it.
				 */
				if ((cp = index(hp->h_name, '.')) &&
				    !strcmp(cp + 1, domain))
					*cp = 0;
				cp = hp->h_name;
			}
		}
	}
	if (cp)
		strcpy(line, cp);
	else {
		sprintf(line, "%s", inet_ntoa(in));
	}
	return (line);
}
 
#define TCP 0
#define UDP 1
#define NPROTOCOLS 2
#define MAXPORT 1024
char *names[NPROTOCOLS][MAXPORT];
 
char *
getservname(port, proto)
	char *proto;
{
	static int first= 1;
	int protonum;
	
	if (first) {
		first = 0;
		initarray();
	}
	if (strcmp(proto, "tcp") == 0)
		protonum = TCP;
	else if (strcmp(proto, "udp") == 0)
		protonum = UDP;
	else
		return NULL;
	if (ntohs((u_short)port) < MAXPORT &&
	    names[protonum][ntohs((u_short)port)])
		return (names[protonum][ntohs((u_short)port)]);
	else
		return NULL;
}
 
initarray()
{
	struct servent *sv;	
	int proto;
 
	while(sv = getservent()) {
		if (strcmp(sv->s_proto, "tcp") == 0)
			proto = TCP;
		else if (strcmp(sv->s_proto, "udp") == 0)
			proto = UDP;
		else
			continue;
		if (ntohs((u_short)sv->s_port) >= MAXPORT)
			continue;
		names[proto][ntohs((u_short)sv->s_port)] =
		    (char *)malloc(strlen(sv->s_name) + 1);
		strcpy(names[proto][ntohs((u_short)sv->s_port)], sv->s_name);
	}
	endservent();
}

