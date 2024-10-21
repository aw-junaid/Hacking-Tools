/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
 
#ifndef lint
static	char sccsid[] = "@(#)if.c 1.1 91/11/13 SMI"; /* from UCB 5.3 4/23/86 */
#endif
 
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
 
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/in_var.h>
#ifdef	ENABLE_XNS
#include <netns/ns.h>
#endif	ENABLE_XNS
 
#include <stdio.h>
 
extern	int aflag;
extern	int tflag;
extern	int nflag;
extern	char *interface;
extern	int unit;
extern	int kread();
extern	char *routename(), *netname();
 
/*
 * Print a description of the network interfaces.
 */
intpr(interval, ifnetaddr)
	int interval;
	off_t ifnetaddr;
{
	struct ifnet ifnet;
	union {
		struct ifaddr ifa;
		struct in_ifaddr in;
	} ifaddr;
	off_t ifaddraddr;
	char name[16];
 
	if (ifnetaddr == 0) {
		printf("ifnet: symbol not defined\n");
		return;
	}
	if (interval) {
		sidewaysintpr(interval, ifnetaddr);
		return;
	}
	kread(ifnetaddr, &ifnetaddr, sizeof ifnetaddr);
	printf("%-5.5s %-5.5s%-13.13s %-14.14s %-6.6s %-5.5s %-6.6s %-5.5s",
		"Name", "Mtu", "Net/Dest", "Address", "Ipkts", "Ierrs",
		"Opkts", "Oerrs");
	printf(" %-6.6s", "Collis");
	if (tflag)
		printf(" %-6.6s", "Timer");
	else
		printf(" %-6.6s", "Queue");
	putchar('\n');
	ifaddraddr = 0;
	while (ifnetaddr || ifaddraddr) {
		struct sockaddr_in *sin;
		register char *cp;
		int n, pad;
		char *index();
		struct in_addr in, inet_makeaddr();
		char buf[80];
 
		if (ifaddraddr == 0) {
			/*
			 * No address list for the current interface:
			 * find the first address.
			 */
			if (kread(ifnetaddr, &ifnet, sizeof ifnet) < 0)
				break;
			if (kread((off_t)ifnet.if_name, name, 16) < 0)
				break;
			name[15] = '\0';
			ifnetaddr = (off_t) ifnet.if_next;
			/*
			 * If a particular interface has been singled
			 * out, skip over all others.
			 */
			if (interface) {
				if (strcmp(name, interface) != 0 ||
				    unit != ifnet.if_unit)
					continue;
			}
			/*
			 * Extend device name with unit number.
			 */
			cp = index(name, '\0');
			*cp++ = ifnet.if_unit + '0';
 
			if ((ifnet.if_flags&IFF_UP) == 0) {
				/*
				 * The interface is down: don't report on it
				 * unless it's been singled out or we're
				 * reporting everything.
				 */
				if (!interface && !aflag)
					continue;
				*cp++ = '*';
			}
			*cp = '\0';
 
			ifaddraddr = (off_t)ifnet.if_addrlist;
		}
		if (ifaddraddr == 0) {
			/*
			 * There's no address associated with the current
			 * interface.
			 */
			if (!aflag) 
				continue;
			printf("%-5s %-5d", name, ifnet.if_mtu);
			printf("%-13.13s ", "none");
			printf("%-14.14s ", "none");
		} else {
			printf("%-5s %-5d", name, ifnet.if_mtu);
			kread(ifaddraddr, &ifaddr, sizeof ifaddr);
			ifaddraddr = (off_t)ifaddr.ifa.ifa_next;
			switch (ifaddr.ifa.ifa_addr.sa_family) {
			case AF_UNSPEC:
				printf("%-13.13s ", "none");
				printf("%-14.14s ", "none");
				break;
			case AF_INET:
				if (ifnet.if_flags & IFF_POINTOPOINT) {
				    sin = (struct sockaddr_in *)
				    		&ifaddr.in.ia_dstaddr;
				    printf("%-13s ", 
					routename(sin->sin_addr));
				} else {
				    printf("%-13s ",
					netname(htonl(ifaddr.in.ia_subnet),
						ifaddr.in.ia_subnetmask));
				}
				sin = (struct sockaddr_in *)&ifaddr.in.ia_addr;
				printf("%-14s ", routename(sin->sin_addr));
				break;
#ifdef ENABLE_XNS
			case AF_NS: {
				struct sockaddr_ns *sns =
				    (struct sockaddr_ns *)&ifaddr.in.ia_addr;
				long net;
				char host[8];
 
				*(union ns_net *) &net = sns->sns_addr.x_net;
				sprintf(host, "%lxH", ntohl(net));
				upHex(host);
				printf("ns:%-8s ", host);
 
				printf("%-12s ",ns_phost(sns));
				break;
			    }
#endif	ENABLE_XNS
			default:
				pad = 29;	/* chars in this field */
				/*
				 * We use this roundabout sprintf into a 
				 * buffer technique here so that we
				 * can count the number of characters we 
				 * print.  System V printf doesn't return
				 * the number of bytes written.
				 */
				sprintf(buf, "af%2d: ", 
					ifaddr.ifa.ifa_addr.sa_family);
				pad -= strlen(buf);
				printf("%s", buf);
 
				/*
				 * Shave off trailing zero bytes in the
				 * address for printing, but always print
				 * at least one byte.
				 */
				for (cp = (char *)&ifaddr.ifa.ifa_addr +
				    sizeof(struct sockaddr) - 1;
				    cp > ifaddr.ifa.ifa_addr.sa_data; --cp)
					if (*cp != 0)
						break;
				n = cp - (char *)ifaddr.ifa.ifa_addr.sa_data 
					+ 1;
				cp = (char *)ifaddr.ifa.ifa_addr.sa_data;
				if (n <= 6)
					while (--n) {
						sprintf(buf, "%02d.",
							      *cp++ & 0xff);
						pad -= strlen(buf);
						printf("%s", buf);
					}
				else
					while (--n) {
						sprintf(buf, "%02d", 
							      *cp++ & 0xff);
						pad -= strlen(buf);
						printf("%s", buf);
					}
				sprintf(buf, "%02d ", *cp & 0xff);
				pad -= strlen(buf);
				printf("%s", buf);
				if (pad > 0)
					while (pad--)
						printf(" ");
				break;
			}
		}
		printf("%-7d %-4d %-7d %-4d %-6d",
		    ifnet.if_ipackets, ifnet.if_ierrors,
		    ifnet.if_opackets, ifnet.if_oerrors,
		    ifnet.if_collisions);
		if (tflag)
			printf(" %-6d", ifnet.if_timer);
		else
			printf(" %-6d", ifnet.if_snd.ifq_len);
		putchar('\n');
	}
}
 
#define	MAXIF	20
struct	iftot {
	char	ift_name[16];		/* interface name */
	int	ift_ip;			/* input packets */
	int	ift_ie;			/* input errors */
	int	ift_op;			/* output packets */
	int	ift_oe;			/* output errors */
	int	ift_co;			/* collisions */
} iftot[MAXIF];
 
/*
 * Print a running summary of interface statistics.
 * Repeat display every interval seconds, showing
 * statistics collected over that interval.  First
 * line printed at top of screen is always cumulative.
 */
sidewaysintpr(interval, off)
	int interval;
	off_t off;
{
	struct ifnet ifnet;
	off_t firstifnet;
	static char sobuf[BUFSIZ];
	register struct iftot *ip, *total;
	register int line;
	struct iftot *lastif, *sum, *interesting;
	int maxtraffic, traffic;
 
	setbuf(stdout, sobuf);
	kread(off, &firstifnet, sizeof (off_t));
	lastif = iftot;
	sum = iftot + MAXIF - 1;
	total = sum - 1;
	maxtraffic = 0, interesting = iftot;
	for (off = firstifnet, ip = iftot; off;) {
		char *cp;
 
		kread(off, &ifnet, sizeof ifnet);
		traffic = ifnet.if_ipackets + ifnet.if_opackets;
		if (traffic > maxtraffic)
			maxtraffic = traffic, interesting = ip;
		ip->ift_name[0] = '(';
		kread((int)ifnet.if_name, ip->ift_name + 1, 15);
		if (interface && strcmp(ip->ift_name + 1, interface) == 0 &&
		    unit == ifnet.if_unit) {
			interesting = ip;
			maxtraffic = 0x7FFFFFFF;
		}
		ip->ift_name[15] = '\0';
		cp = index(ip->ift_name, '\0');
		sprintf(cp, "%d)", ifnet.if_unit);
		ip++;
		if (ip >= iftot + MAXIF - 2)
			break;
		off = (off_t) ifnet.if_next;
	}
	lastif = ip;
banner:
	printf("    input   %-6.6s    output        ", interesting->ift_name);
	if (lastif - iftot > 0)
		printf("   input  (Total)    output       ");
	for (ip = iftot; ip < iftot + MAXIF; ip++) {
		ip->ift_ip = 0;
		ip->ift_ie = 0;
		ip->ift_op = 0;
		ip->ift_oe = 0;
		ip->ift_co = 0;
	}
	putchar('\n');
	printf("%-7.7s %-5.5s %-7.7s %-5.5s %-6.6s ",
		"packets", "errs", "packets", "errs", "colls");
	if (lastif - iftot > 0)
		printf("%-7.7s %-5.5s %-7.7s %-5.5s %-6.6s ",
			"packets", "errs", "packets", "errs", "colls");
	putchar('\n');
	fflush(stdout);
	line = 0;
loop:
	sum->ift_ip = 0;
	sum->ift_ie = 0;
	sum->ift_op = 0;
	sum->ift_oe = 0;
	sum->ift_co = 0;
	for (off = firstifnet, ip = iftot; off && ip < lastif; ip++) {
		kread(off, &ifnet, sizeof ifnet);
		if (ip == interesting)
			printf("%-7d %-5d %-7d %-5d %-6d ",
				ifnet.if_ipackets - ip->ift_ip,
				ifnet.if_ierrors - ip->ift_ie,
				ifnet.if_opackets - ip->ift_op,
				ifnet.if_oerrors - ip->ift_oe,
				ifnet.if_collisions - ip->ift_co);
		ip->ift_ip = ifnet.if_ipackets;
		ip->ift_ie = ifnet.if_ierrors;
		ip->ift_op = ifnet.if_opackets;
		ip->ift_oe = ifnet.if_oerrors;
		ip->ift_co = ifnet.if_collisions;
		sum->ift_ip += ip->ift_ip;
		sum->ift_ie += ip->ift_ie;
		sum->ift_op += ip->ift_op;
		sum->ift_oe += ip->ift_oe;
		sum->ift_co += ip->ift_co;
		off = (off_t) ifnet.if_next;
	}
	if (lastif - iftot > 0)
		printf("%-7d %-5d %-7d %-5d %-6d\n",
			sum->ift_ip - total->ift_ip,
			sum->ift_ie - total->ift_ie,
			sum->ift_op - total->ift_op,
			sum->ift_oe - total->ift_oe,
			sum->ift_co - total->ift_co);
	*total = *sum;
	fflush(stdout);
	line++;
	if (interval)
		sleep(interval);
	if (line == 21)
		goto banner;
	goto loop;
	/*NOTREACHED*/
}

