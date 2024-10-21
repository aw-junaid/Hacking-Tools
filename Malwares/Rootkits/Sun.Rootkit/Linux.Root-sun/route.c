/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
 
#ifndef lint
static	char sccsid[] = "@(#)route.c 1.1 91/11/13 SMI"; /* from UCB 5.6 86/04/23 */
#endif
 
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/mbuf.h>
 
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
 
#ifdef	ENABLE_XNS
#include <netns/ns.h>
#endif	ENABLE_XNS
 
#include <netdb.h>
 
extern	int kread();
extern	int nflag;
extern	char *routename(), *netname(), *inet_ntoa();
#ifdef	ENABLE_XNS
extern	char *ns_print();
#endif	ENABLE_XNS
 
/*
 * Definitions for showing gateway flags.
 */
struct bits {
	short	b_mask;
	char	b_val;
} bits[] = {
	{ RTF_UP,	'U' },
	{ RTF_GATEWAY,	'G' },
	{ RTF_HOST,	'H' },
	{ RTF_DYNAMIC,	'D' },
	{ 0 }
};
 
/*
 * Print routing tables.
 */
routepr(hostaddr, netaddr, hashsizeaddr)
	off_t hostaddr, netaddr, hashsizeaddr;
{
	struct mbuf mb;
	register struct rtentry *rt;
	register struct mbuf *m;
	register struct bits *p;
	char name[16], *flags;
	struct mbuf **routehash;
	struct ifnet ifnet;
	int hashsize;
	int i, doinghost = 1;
 
	if (hostaddr == 0) {
		printf("rthost: symbol not in namelist\n");
		return;
	}
	if (netaddr == 0) {
		printf("rtnet: symbol not in namelist\n");
		return;
	}
	if (hashsizeaddr == 0) {
		printf("rthashsize: symbol not in namelist\n");
		return;
	}
	kread(hashsizeaddr, &hashsize, sizeof (hashsize));
	routehash = (struct mbuf **)malloc( hashsize*sizeof (struct mbuf *) );
	kread(hostaddr, routehash, hashsize*sizeof (struct mbuf *));
	printf("Routing tables\n");
	printf("%-20.20s %-20.20s %-8.8s %-6.6s %-10.10s %s\n",
		"Destination", "Gateway",
		"Flags", "Refcnt", "Use", "Interface");
again:
	for (i = 0; i < hashsize; i++) {
		if (routehash[i] == 0)
			continue;
		m = routehash[i];
		while (m) {
			struct sockaddr_in *sin;
 
			kread(m, &mb, sizeof (mb));
			rt = mtod(&mb, struct rtentry *);
			if ((unsigned)rt < (unsigned)&mb ||
			    (unsigned)rt >= (unsigned)(&mb + 1)) {
				printf("???\n");
				return;
			}
 
			switch(rt->rt_dst.sa_family) {
			case AF_INET:
				sin = (struct sockaddr_in *)&rt->rt_dst;
				printf("%-20.20s ",
				    (sin->sin_addr.s_addr == 0) ? "default" :
				    (rt->rt_flags & RTF_HOST) ?
				    routename(sin->sin_addr) :
					netname(sin->sin_addr.s_addr, 0));
				sin = (struct sockaddr_in *)&rt->rt_gateway;
				printf("%-20.20s ", routename(sin->sin_addr));
				break;
#ifdef	ENABLE_XNS
			case AF_NS:
				printf("%-20s ",
				    ns_print((struct sockaddr_ns *)&rt->rt_dst));
				printf("%-20s ",
				    ns_print((struct sockaddr_ns *)&rt->rt_gateway));
				break;
#endif	ENABLE_XNS
			default: {
				u_short *s = (u_short *)rt->rt_dst.sa_data;
 
				printf("(%d)%x %x %x %x %x %x %x ",
				    rt->rt_dst.sa_family,
				    s[0], s[1], s[2], s[3], s[4], s[5], s[6]);
				s = (u_short *)rt->rt_gateway.sa_data;
				printf("(%d)%x %x %x %x %x %x %x ",
				    rt->rt_gateway.sa_family,
				    s[0], s[1], s[2], s[3], s[4], s[5], s[6]);
			    }
			}
			for (flags = name, p = bits; p->b_mask; p++)
				if (p->b_mask & rt->rt_flags)
					*flags++ = p->b_val;
			*flags = '\0';
			printf("%-8.8s %-6d %-10d ", name,
				rt->rt_refcnt, rt->rt_use);
			if (rt->rt_ifp == 0) {
				putchar('\n');
				m = mb.m_next;
				continue;
			}
			kread(rt->rt_ifp, &ifnet, sizeof (ifnet));
			kread((int)ifnet.if_name, name, 16);
			printf("%s%d\n", name, ifnet.if_unit);
			m = mb.m_next;
		}
	}
	if (doinghost) {
		kread(netaddr, routehash, hashsize*sizeof (struct mbuf *));
		doinghost = 0;
		goto again;
	}
	free(routehash);
}
 
char *
routename(in)
	struct in_addr in;	/* in network order */
{
	register char *cp;
	static char line[50];
	struct hostent *hp;
	static char domain[MAXHOSTNAMELEN + 1];
	static int first = 1;
	char *index();
 
	if (first) {
		/*
		 * Record domain name for future reference.  Check
		 * first for the 4.3bsd convention of keeping it as
		 * part of the hostname.  Failing that, try extracting
		 * it using the domainname system call.
		 */
		first = 0;
		if (gethostname(domain, MAXHOSTNAMELEN) == 0 &&
		    (cp = index(domain, '.')))
			(void) strcpy(domain, cp + 1);
		else if (getdomainname(domain, MAXHOSTNAMELEN) < 0)
			domain[0] = 0;
	}
	cp = 0;
	if (!nflag) {
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
	if (cp)
		strcpy(line, cp);
	else
		strcpy(line, inet_ntoa(in));
	return (line);
}
 
/*
 * Return the name of the network whose address is given.
 * The address is assumed to be that of a net or subnet, not a host.
 */
char *
netname(iaddr, mask)
	u_long iaddr;
	u_long mask;
{
	char *cp = 0;
	static char line[50];
	struct in_addr in;
 
	in.s_addr = ntohl(iaddr);
	if (!nflag && in.s_addr) {
		struct netent *np = 0;
		struct hostent *hp;
		u_long net;
 
		if (mask == 0) {
			register u_long i = in.s_addr;
			int subnetshift;
 
			if (IN_CLASSA(i)) {
				mask = IN_CLASSA_NET;
				subnetshift = 8;
			} else if (IN_CLASSB(i)) {
				mask = IN_CLASSB_NET;
				subnetshift = 8;
			} else {
				mask = IN_CLASSC_NET;
				subnetshift = 4;
			}
			/*
			 * If there are more bits than the standard mask
			 * would suggest, subnets must be in use.
			 * Guess at the subnet mask, assuming reasonable
			 * width subnet fields.
			 */
			while (in.s_addr &~ mask)
				mask = (long)mask >> subnetshift;
		}
		net = in.s_addr & mask;
		/*
		 * Right-justify the network number.
		 *
		 * This is a throw-back to the old conventions used in the
		 * kernel.  We now store it left-justified in the kernel,
		 * but still right-justified in the NIS maps for backward
		 * compatibility.
		 */
		while ((mask & 1) == 0)
			mask >>= 1, net >>= 1;
		np = getnetbyaddr(net, AF_INET);
		if (np && np->n_net == net)
			cp = np->n_name;
		else {
			  /*
			   * gethostbyaddr takes network order; above
			   * wanted host order.
			   */
			in.s_addr = iaddr;
			hp = gethostbyaddr(&in,sizeof(struct in_addr),AF_INET);
			if (hp)
			  cp = hp->h_name;
		}
	}
	if (cp)
		strcpy(line, cp);
	else {
		in.s_addr = iaddr;
		strcpy(line, inet_ntoa(in));
	}
	return (line);
}
 
/*
 * Print routing statistics
 */
rt_stats(off)
	off_t off;
{
	struct rtstat rtstat;
 
	if (off == 0) {
		printf("rtstat: symbol not in namelist\n");
		return;
	}
	kread(off, (char *)&rtstat, sizeof (rtstat));
	printf("routing:\n");
	printf("\t%d bad routing redirect%s\n",
		rtstat.rts_badredirect, plural(rtstat.rts_badredirect));
	printf("\t%d dynamically created route%s\n",
		rtstat.rts_dynamic, plural(rtstat.rts_dynamic));
	printf("\t%d new gateway%s due to redirects\n",
		rtstat.rts_newgateway, plural(rtstat.rts_newgateway));
	printf("\t%d destination%s found unreachable\n",
		rtstat.rts_unreach, plural(rtstat.rts_unreach));
	printf("\t%d use%s of a wildcard route\n",
		rtstat.rts_wildcard, plural(rtstat.rts_wildcard));
}
 
#ifdef	ENABLE_XNS
 
short ns_nullh[] = {0,0,0};
short ns_bh[] = {-1,-1,-1};
 
char *
ns_print(sns)
struct sockaddr_ns *sns;
{
	struct ns_addr work;
	union { union ns_net net_e; u_long long_e; } net;
	u_short port;
	static char mybuf[50], cport[10], chost[25];
	char *host = "";
	register char *p; register u_char *q; u_char *q_lim;
 
	work = sns->sns_addr;
	port = ntohs(work.x_port);
	work.x_port = 0;
	net.net_e  = work.x_net;
	if (ns_nullhost(work) && net.long_e == 0) {
		if (port ) {
			sprintf(mybuf, "*.%xH", port);
			upHex(mybuf);
		} else
			sprintf(mybuf, "*.*");
		return (mybuf);
	}
 
	if (bcmp(ns_bh, work.x_host.c_host, 6) == 0) { 
		host = "any";
	} else if (bcmp(ns_nullh, work.x_host.c_host, 6) == 0) {
		host = "*";
	} else {
		q = work.x_host.c_host;
		sprintf(chost, "%02x%02x%02x%02x%02x%02xH",
			q[0], q[1], q[2], q[3], q[4], q[5]);
		for (p = chost; *p == '0' && p < chost + 12; p++);
		host = p;
	}
	if (port)
		sprintf(cport, ".%xH", htons(port));
	else
		*cport = 0;
 
	sprintf(mybuf,"%xH.%s%s", ntohl(net.long_e), host, cport);
	upHex(mybuf);
	return(mybuf);
}
 
char *
ns_phost(sns)
struct sockaddr_ns *sns;
{
	struct sockaddr_ns work;
	static union ns_net ns_zeronet;
	char *p;
	
	work = *sns;
	work.sns_addr.x_port = 0;
	work.sns_addr.x_net = ns_zeronet;
 
	p = ns_print(&work);
	if (strncmp("0H.", p, 3) == 0)
		p += 3;
	return(p);
}
upHex(p0)
char *p0;
{
	register char *p = p0;
	for (; *p; p++) switch (*p) {
 
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		*p += ('A' - 'a');
	}
}
#endif	ENABLE_XNS

