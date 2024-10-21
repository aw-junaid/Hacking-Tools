/*+
 	Modified to remove PROMISC message
	when -a flag is passed
+*/

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 * @(#)ifconfig.c 1.1 91/11/13 SMI from UCB 4.18 5/22/86 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/file.h>

#include <netinet/in.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#ifdef NS
#define	NSIP
#include <netns/ns.h>
#include <netns/ns_if.h>
#endif

#ifdef APPLETALK
#include <netat/atalk.h>
#endif APPLETALK

#define	NIT
#ifdef	NIT
#include <sys/time.h>
#include <net/nit_if.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>

char *inet_ntoa();

struct	ifreq ifr;
struct	sockaddr_in sin = { AF_INET };
struct	sockaddr_in broadaddr;
struct	sockaddr_in netmask = { AF_INET };
struct	sockaddr_in ipdst = { AF_INET };
char	name[30];
int	setaddr;
int	setmask;
int	setbroadaddr;
int	setipdst;
int	s;

void	setifflags(), setifaddr(), setifdstaddr(), setifnetmask();
void	setifmetric(), setifbroadaddr(), setifipdst(), setifether();

#define	NEXTARG		0xffffff
#define	AF_ANY		(-1)

struct	cmd {
	char	*c_name;
	int	c_parameter;		/* NEXTARG means next argv */
	void	(*c_func)();
	int	c_af;			/* address family restrictions */
} cmds[] = {
	{ "up",		IFF_UP,		setifflags,	AF_ANY } ,
	{ "down",	-IFF_UP,	setifflags,	AF_ANY },
	{ "trailers",	-IFF_NOTRAILERS,setifflags,	AF_ANY },
	{ "-trailers",	IFF_NOTRAILERS,	setifflags,	AF_ANY },
	{ "arp",	-IFF_NOARP,	setifflags,	AF_ANY },
	{ "-arp",	IFF_NOARP,	setifflags,	AF_ANY },
	{ "private",	IFF_PRIVATE,	setifflags,	AF_ANY },
	{ "-private",	-IFF_PRIVATE,	setifflags,	AF_ANY },
	{ "netmask",	NEXTARG,	setifnetmask,	AF_INET },
	{ "metric",	NEXTARG,	setifmetric,	AF_ANY },
	{ "broadcast",	NEXTARG,	setifbroadaddr,	AF_ANY },
	{ "ipdst",	NEXTARG,	setifipdst,	AF_NS },
	{ "ether",	NEXTARG,	setifether,	AF_ANY },
	{ 0,		0,		setifaddr,	AF_ANY },
	{ 0,		0,		setifdstaddr,	AF_ANY },
};

/*
 * XNS support liberally adapted from
 * code written at the University of Maryland
 * principally by James O'Toole and Chris Torek.
 */

int	in_status(), in_getaddr();
int	xns_status(), xns_getaddr();
int	at_status(), at_getaddr();
int	ether_status(), ether_getaddr();

/* Known address families */
struct afswtch {
	char *af_name;
	short af_af;
	int (*af_status)();
	int (*af_getaddr)();
} afs[] = {
	{ "inet",	AF_INET,	in_status,	in_getaddr },
	{ "ns",		AF_NS,		xns_status,	xns_getaddr },
	{ "appletalk",	AF_APPLETALK,	at_status,	at_getaddr },
	{ "ether",	AF_INET,	ether_status,	ether_getaddr },
	{ 0,		0,		0,		0 }
};

struct afswtch *afp;	/*the address family being set or asked about*/

main(argc, argv)
	int argc;
	char *argv[];
{
	int af = AF_INET;
	void foreachinterface(), ifconfig();

	if (argc < 2) {
		fprintf(stderr, "usage: ifconfig interface\n%s%s%s%s%s\n",
		    "\t[ <af> ] [ <address> [ <dest_addr> ] ] [ up ] [ down ]\n",
		    "\t[ netmask <mask> ] [ broadcast <broad_addr> ]\n",
		    "\t[ metric <n> ]\n",
		    "\t[ trailers | -trailers ] [private | -private]\n",
		    "\t[ arp | -arp ] \n");
		exit(1);
	}
	argc--, argv++;
	strncpy(name, *argv, sizeof(name));
	argc--, argv++;
	if (argc > 0) {
		struct afswtch *myafp;
		
		for (myafp = afp = afs; myafp->af_name; myafp++)
			if (strcmp(myafp->af_name, *argv) == 0) {
				afp = myafp; argc--; argv++;
				break;
			}
		af = ifr.ifr_addr.sa_family = afp->af_af;
	}
	s = socket(af, SOCK_DGRAM, 0);
	if (s < 0) {
		perror("ifconfig: socket");
		exit(1);
	}
	if (strcmp(name, "-a"))
		ifconfig(argc, argv, af, (struct ifnet *) NULL);
	else
		foreachinterface(ifconfig, argc, argv, af);

	exit(0);
	/* NOTREACHED */
}

/*
 *	For each interface, call (*func)(argc, argv, af, ifrp).
 */
void
foreachinterface(func, argc, argv, af)
	void (*func)();
	int argc;
	char **argv;
	int af;
{
	int n;
	char buf[BUFSIZ];
	struct ifconf ifc;
	register struct ifreq *ifrp;

	ifc.ifc_len = sizeof (buf);
	ifc.ifc_buf = buf;
	if (ioctl(s, SIOCGIFCONF, (char *)&ifc) < 0) {
		perror("ioctl (get interface configuration)");
		close(s);
		return;
	}

	ifrp = ifc.ifc_req;
	for (n = ifc.ifc_len / sizeof (struct ifreq); n > 0; n--, ifrp++) {
		/*
		 *	We must close and recreate the socket each time
		 *	since we don't know what type of socket it is now
		 *	(each status function may change it).
		 */
		(void) close(s);
		s = socket(af, SOCK_DGRAM, 0);
		if (s == -1) {
			perror("ifconfig: socket");
			exit(1);
		}

		/*
		 *	Reset global state variables to known values.
		 */
		setaddr = setbroadaddr = setipdst = setmask = 0;
		(void) strncpy(name, ifrp->ifr_name, sizeof(name));

		(*func)(argc, argv, af, ifrp);
	}
}

void
ifconfig(argc, argv, af, ifrp)
	int argc;
	char **argv;
	int af;
	register struct ifreq *ifrp;
{
	if (argc == 0) {
		status();
		return;
	}
	while (argc > 0) {
		register struct cmd *p;

		for (p = cmds; p->c_name; p++)
			if (strcmp(*argv, p->c_name) == 0)
				break;
		if (p->c_name == 0 && setaddr)
			p++;	/* got src, do dst */
		if (p->c_func) {
			if (p->c_parameter == NEXTARG) {
				argc--, argv++;
				if (argc == 0) {
					(void) fprintf(stderr, 
					    "ifconfig: no argument for %s\n",
					    p->c_name);
					exit(1);
				}
				/*
				 *	Call the function if:
				 *
				 *		there's no address family
				 *		restriction
				 *	OR
				 *		we don't know the address yet
				 *		(because we were called from
				 *		main)
				 *	OR
				 *		there is a restriction and
				 *		the address families match
				 */
				if ((p->c_af == AF_ANY)			||
				    (ifrp == (struct ifreq *) NULL)	||
				    (ifrp->ifr_addr.sa_family == p->c_af)
				   )
					(*p->c_func)(*argv);
			} else
				if ((p->c_af == AF_ANY)			||
				    (ifrp == (struct ifreq *) NULL)	||
				    (ifrp->ifr_addr.sa_family == p->c_af)
				   )
					(*p->c_func)(*argv, p->c_parameter);
		}
		argc--, argv++;
	}
	if ((setmask || setaddr) && (af == AF_INET) && 
	     netmask.sin_addr.s_addr != INADDR_ANY) {
		/*
		 * If setting the address and not the mask,
		 * clear any existing mask and the kernel will then
		 * assign the default.  If setting both,
		 * set the mask first, so the address will be
		 * interpreted correctly.
		 */
		strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
		ifr.ifr_addr = *(struct sockaddr *)&netmask;
		if (ioctl(s, SIOCSIFNETMASK, (caddr_t)&ifr) < 0)
			Perror("ioctl (SIOCSIFNETMASK)");
	}
#ifdef NS
	if (setipdst && af == AF_NS) {
		struct nsip_req rq;
		int size = sizeof(rq);

		rq.rq_ns = *(struct sockaddr *) &sin;
		rq.rq_ip = *(struct sockaddr *) &ipdst;

		if (setsockopt(s, 0, SO_NSIP_ROUTE, &rq, size) < 0)
			Perror("Encapsulation Routing");
		setaddr = 0;
	}
#endif
	if (setaddr) {
		ifr.ifr_addr = *(struct sockaddr *) &sin;
		strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
		if (ioctl(s, SIOCSIFADDR, (caddr_t)&ifr) < 0)
			Perror("ioctl (SIOCSIFADDR)");
	}
	if (setbroadaddr) {
		ifr.ifr_addr = *(struct sockaddr *)&broadaddr;
		strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
		if (ioctl(s, SIOCSIFBRDADDR, (caddr_t)&ifr) < 0)
			Perror("ioctl (SIOCSIFBRDADDR)");
	}
}

/*ARGSUSED*/
void
setifaddr(addr, param)
	char *addr;
	short param;
{
	/*
	 * Delay the ioctl to set the interface addr until flags are all set.
	 * The address interpretation may depend on the flags,
	 * and the flags may change when the address is set.
	 */
	setaddr++;
	(*afp->af_getaddr)(addr, &sin);
}

void
setifnetmask(addr)
	char *addr;
{
	if (strcmp(addr, "+") == 0) {
		setmask = in_getmask( &netmask);
		return;
	}
	in_getaddr(addr, &netmask);
	setmask++;
}

void
setifbroadaddr(addr)
	char *addr;
{
	/*
	 *	This doesn't set the broadcast address at all.  Rather, it
	 *	gets, then sets the interface's address, relying on the fact
	 *	that resetting the address will reset the broadcast address.
	 */
	if (strcmp(addr, "+") == 0) {
		if (!setaddr) {
			/*
			 * If we do not already have an address to set,
			 * then we just read the interface to see if it
			 * is already set.
			 */
			strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
			if (ioctl(s, SIOCGIFADDR, (caddr_t)&ifr) < 0) {
				if (errno != EADDRNOTAVAIL)
					perror("SIOCGIFADDR failed - cannot reset broadcast address");
				return;
			}
			sin = * ( (struct sockaddr_in *)&ifr.ifr_addr);
			setaddr++;

		}
		/*
		 *	Turn off setbraodcast to allow for the (rare)
		 *	case of a user saying
		 *	ifconfig ... broadcast <number> ... broadcast +
		 */
		setbroadaddr = 0;
		return;
	}
	(*afp->af_getaddr)(addr, &broadaddr);
	setbroadaddr++;
}

void
setifipdst(addr)
	char *addr;
{
	in_getaddr(addr, &ipdst);
	setipdst++;
}

/*ARGSUSED*/
void
setifdstaddr(addr, param)
	char *addr;
	int param;
{

	(*afp->af_getaddr)(addr, &ifr.ifr_addr);
	strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
	if (ioctl(s, SIOCSIFDSTADDR, (caddr_t)&ifr) < 0)
		Perror("ioctl (SIOCSIFDSTADDR)");
}

void
setifflags(vname, value)
	char *vname;
	int value;
{

	strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
	if (ioctl(s, SIOCGIFFLAGS, (caddr_t)&ifr) < 0)
		Perror("ioctl (SIOCGIFFLAGS)");
	if (value < 0) {
		value = -value;
		ifr.ifr_flags &= ~value;
	} else
		ifr.ifr_flags |= value;
	strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
	if (ioctl(s, SIOCSIFFLAGS, (caddr_t)&ifr) < 0)
		Perror(vname);
}

void
setifmetric(val)
	char *val;
{
	strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
	ifr.ifr_metric = atoi(val);
	if (ioctl(s, SIOCSIFMETRIC, (caddr_t)&ifr) < 0)
		perror("ioctl (set metric)");
}

void
setifether(addr)
	char *addr;
{
#ifdef	NIT
	int t;
	struct ether_addr *ea, *ether_aton();

	t = open("/dev/nit", O_RDONLY);
	if (t == -1) {
		perror("/dev/nit");
		exit(1);
	}
	strncpy(ifr.ifr_name, name, sizeof ifr.ifr_name);
	if (ioctl(t, NIOCBIND, (caddr_t) &ifr) < 0) {
		perror("ioctl:  NIOCBIND");
		exit(1);
	}
	ea = ether_aton(addr);
	(void) bcopy((caddr_t) ifr.ifr_addr.sa_data, ea, sizeof(struct ether_addr));
	if (ioctl(t, SIOCSIFADDR, (caddr_t)&ifr) == -1) {
		perror("ioctl (SIOCSIFADDR)");
	}
	(void) close(t);
#endif	/* NIT */
}

/*+  Here are old bit flags

#define	IFFBITS \
"\020\1UP\2BROADCAST\3DEBUG\4LOOPBACK\5POINTOPOINT\6NOTRAILERS\7RUNNING\10NOARP\11PROMISC\12MULTI\020PRIVATE"

+*/

/*+
  Just remove the bit flag and no more PROMISC message.

  if they run strings they won't find PROMISC message, but
  if they are running strings on binaries, chances are they
  know your there already and are going to find you :-)
+*/

#define	IFFBITS \
"\020\1UP\2BROADCAST\3DEBUG\4LOOPBACK\5POINTOPOINT\6NOTRAILERS\7RUNNING\10NOARP\12MULTI\020PRIVATE"

/*
 * Print the status of the interface.  If an address family was
 * specified, show it and it only; otherwise, show them all.
 */
status()
{
	register struct afswtch *p = afp;
	short af = ifr.ifr_addr.sa_family;
	register int flags;

	strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name));
	if (ioctl(s, SIOCGIFFLAGS, (caddr_t)&ifr) < 0) {
		Perror("ioctl (SIOCGIFFLAGS)");
		exit(1);
	}
	flags = ifr.ifr_flags;
	printf("%s: ", name);
	printb("flags", flags, IFFBITS);
	strncpy(ifr.ifr_name, name, sizeof ifr.ifr_name);
	if (ioctl(s, SIOCGIFMETRIC, (caddr_t)&ifr) < 0)
		perror("ioctl (SIOCGIFMETRIC)");
	else {
		if (ifr.ifr_metric)
			printf(" metric %d", ifr.ifr_metric);
	}
	putchar('\n');
	if ((p = afp) != NULL) {
		(*p->af_status)(1, flags);
	} else for (p = afs; p->af_name; p++) {
		ifr.ifr_addr.sa_family = p->af_af;
		(*p->af_status)(0, flags);
	}
}

in_status(force, flags)
	int force;
	int flags;
{
	struct sockaddr_in *sin;
	char *inet_ntoa();

	strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
	if (ioctl(s, SIOCGIFADDR, (caddr_t)&ifr) < 0) {
		if (errno == EADDRNOTAVAIL || errno == EAFNOSUPPORT) {
			if (!force)
				return;
			bzero((char *)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
		} else
			Perror("ioctl (SIOCGIFADDR)");
	}
	sin = (struct sockaddr_in *)&ifr.ifr_addr;
	printf("\tinet %s ", inet_ntoa(sin->sin_addr));
	strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
	if (ioctl(s, SIOCGIFNETMASK, (caddr_t)&ifr) < 0) {
		if (errno != EADDRNOTAVAIL)
			perror("ioctl (SIOCGIFNETMASK)");
		bzero((char *)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
	} else
		netmask.sin_addr =
		    ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
	if (flags & IFF_POINTOPOINT) {
		strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
		if (ioctl(s, SIOCGIFDSTADDR, (caddr_t)&ifr) < 0) {
			if (errno == EADDRNOTAVAIL)
			    bzero((char *)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
			else
			    perror("ioctl (SIOCGIFDSTADDR)");
		}
		sin = (struct sockaddr_in *)&ifr.ifr_dstaddr;
		printf("--> %s ", inet_ntoa(sin->sin_addr));
	}
	printf("netmask %x ", ntohl(netmask.sin_addr.s_addr));
	if (flags & IFF_BROADCAST) {
		strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
		if (ioctl(s, SIOCGIFBRDADDR, (caddr_t)&ifr) < 0) {
			if (errno == EADDRNOTAVAIL)
			    bzero((char *)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
			else
			    perror("ioctl (SIOCGIFADDR)");
		}
		sin = (struct sockaddr_in *)&ifr.ifr_addr;
		if (sin->sin_addr.s_addr != 0)
			printf("broadcast %s", inet_ntoa(sin->sin_addr));
	}
	putchar('\n');
}


xns_status(force, flags)
	int force;
	int flags;
{
#ifdef NS
	struct sockaddr_ns *sns;

	close(s);
	s = socket(AF_NS, SOCK_DGRAM, 0);
	if (s < 0) {
		if (errno == EAFNOSUPPORT || errno == EPROTONOSUPPORT)
			return;
		perror("ifconfig: ns socket");
		exit(1);
	}
	strncpy(ifr.ifr_name, name, sizeof ifr.ifr_name);
	if (ioctl(s, SIOCGIFADDR, (caddr_t)&ifr) < 0) {
		if (errno == EADDRNOTAVAIL || errno == EAFNOSUPPORT) {
			if (!force)
				return;
			bzero((char *)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
		} else
			perror("ioctl (SIOCGIFADDR)");
	}
	sns = (struct sockaddr_ns *)&ifr.ifr_addr;
	printf("\tns %s ", ns_ntoa(sns->sns_addr));
	if (flags & IFF_POINTOPOINT) { /* by W. Nesheim@Cornell */
		strncpy(ifr.ifr_name, name, sizeof ifr.ifr_name);
		if (ioctl(s, SIOCGIFDSTADDR, (caddr_t)&ifr) < 0) {
			if (errno == EADDRNOTAVAIL)
			    bzero((char *)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
			else
			    Perror("ioctl (SIOCGIFDSTADDR)");
		}
		sns = (struct sockaddr_ns *)&ifr.ifr_dstaddr;
		printf("--> %s ", ns_ntoa(sns->sns_addr));
	}
	putchar('\n');
#endif 
}

#ifdef APPLETALK
char *
at_ntoa(sad)
	struct a_addr sad;
{
	static char buf[256];
	(void) sprintf( buf, "%d.%d", sad.at_Net, sad.at_Node);
	return(buf);
}
#endif AAPLETALK

at_status(force, flags)
	int force;
	int flags;
{
#ifdef APPLETALK
	struct sockaddr_at *sat;

	close(s);
	s = socket(AF_APPLETALK, SOCK_DGRAM, 0);
	if (s < 0) {
		if (errno == EAFNOSUPPORT || errno == EPROTONOSUPPORT)
			return;
		perror("ifconfig: appletalk socket");
		exit(1);
	}
	strncpy(ifr.ifr_name, name, sizeof ifr.ifr_name);
	if (ioctl(s, SIOCGIFADDR, (caddr_t)&ifr) < 0) {
		if (errno == EADDRNOTAVAIL || errno == EAFNOSUPPORT) {
			if (!force)
				return;
			bzero((char *)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
		} else
			perror("ioctl (SIOCGIFADDR)");
	}
	sat = (struct sockaddr_at *)&ifr.ifr_addr;
	printf("\tappletalk %s ", at_ntoa(sat->at_addr));
	if (flags & IFF_POINTOPOINT) {
		strncpy(ifr.ifr_name, name, sizeof ifr.ifr_name);
		if (ioctl(s, SIOCGIFDSTADDR, (caddr_t)&ifr) < 0) {
			if (errno == EADDRNOTAVAIL)
			    bzero((char *)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
			else
			    Perror("ioctl (SIOCGIFDSTADDR)");
		}
		sat = (struct sockaddr_at *)&ifr.ifr_dstaddr;
		printf("--> %s ", at_ntoa(sat->at_addr));
	}
	putchar('\n');
#endif APPLETALK
}

/*ARGSUSED*/
ether_status(force, flags)
	int force;
	int flags;
{
#ifdef	NIT
	register struct sockaddr *saddr;
	char *ether_ntoa();

	if (getuid())
		return;
	(void) close(s);
	s = open("/dev/nit", O_RDONLY);
	if (s == -1)
		if (force) {
			perror("/dev/nit");
			exit(1);
		}
		else
			return;
	strncpy(ifr.ifr_name, name, sizeof ifr.ifr_name);
	if (ioctl(s, NIOCBIND, (caddr_t) &ifr) < 0) {
		perror("ioctl:  NIOCBIND");
		exit(1);
	}
	if (ioctl(s, SIOCGIFADDR, (caddr_t)&ifr) < 0) {
		if (errno == EINVAL)
			return;
		else
			perror("ioctl (SIOCGIFADDR)");
	}
	saddr = (struct sockaddr *)&ifr.ifr_addr;
	printf("\tether %s ", ether_ntoa((struct ether_addr *) saddr->sa_data));
	putchar('\n');
#endif	/* NIT */
}

Perror(cmd)
	char *cmd;
{
	extern int errno;

	fprintf(stderr, "ifconfig: ");
	switch (errno) {

	case ENXIO:
		fprintf(stderr, "%s: no such interface\n", cmd);
		break;

	case EPERM:
		fprintf(stderr, "%s: permission denied\n", cmd);
		break;

	default:
		perror(cmd);
	}
	exit(1);
}

struct	in_addr inet_makeaddr();

in_getaddr(s, saddr)
	char *s;
	struct sockaddr *saddr;
{
	register struct sockaddr_in *sin = (struct sockaddr_in *)saddr;
	struct hostent *hp;
	struct netent *np;
	int val;

	bzero((char *)saddr, sizeof *saddr);
	sin->sin_family = AF_INET;

	/*
	 *	Try to catch attempts to set the broadcast address to all 1's.
	 */
	if (strcmp(s,"255.255.255.255") == 0 || 
	    ((u_int) strtol(s, (char **) NULL, 0) == (u_int) 0xffffffff)) {
		sin->sin_addr.s_addr = 0xffffffff;
		return;
	}

	val = inet_addr(s);
	if (val != -1) {
		sin->sin_addr.s_addr = val;
		return;
	}
	hp = gethostbyname(s);
	if (hp) {
		sin->sin_family = hp->h_addrtype;
		bcopy(hp->h_addr, (char *)&sin->sin_addr, hp->h_length);
		return;
	}
	np = getnetbyname(s);
	if (np) {
		sin->sin_family = np->n_addrtype;
		sin->sin_addr = inet_makeaddr(np->n_net, INADDR_ANY);
		return;
	}
	fprintf(stderr, "ifconfig: %s: bad address\n", s);
	exit(1);
}

/*
 * Print a value a la the %b format of the kernel's printf
 */
printb(s, v, bits)
	char *s;
	register char *bits;
	register unsigned short v;
{
	register int i, any = 0;
	register char c;

	if (bits && *bits == 8)
		printf("%s=%o", s, v);
	else
		printf("%s=%x", s, v);
	bits++;
	if (bits) {
		putchar('<');
		while (i = *bits++) {
			if (v & (1 << (i-1))) {
				if (any)
					putchar(',');
				any = 1;
				for (; (c = *bits) > 32; bits++)
					putchar(c);
			} else
				for (; *bits > 32; bits++)
					;
		}
		putchar('>');
	}
}

xns_getaddr(addr, saddr)
char *addr;
struct sockaddr *saddr;
{
#ifdef NS
	struct sockaddr_ns *sns = (struct sockaddr_ns *)saddr;
	struct ns_addr ns_addr();

	bzero((char *)saddr, sizeof *saddr);
	sns->sns_family = AF_NS;
	sns->sns_addr = ns_addr(addr);
#endif
}

at_getaddr(addr, saddr)
char *addr;
struct sockaddr *saddr;
{
# ifdef APPLETALK
	struct sockaddr_at *sat = (struct sockaddr_at *)saddr;

	bzero((char *)saddr, sizeof *saddr);
	sat->at_family = AF_APPLETALK;
	sat->at_addr.at_Net = atoi(addr);
# endif APPLETALK
}

extern struct ether_addr *ether_aton();

ether_getaddr(addr, saddr)
char *addr;
struct sockaddr *saddr;
{
	struct ether_addr *eaddr;

	bzero((char *)saddr, sizeof *saddr);
	saddr->sa_family = AF_UNSPEC;
	/*
	 * call the library routine to do the conversion.
	 */
	eaddr = ether_aton(addr);
	if (eaddr == NULL) {
		fprintf(stderr, "ifconfig: %s: bad address\n", addr);
		exit(1);
	}
	bcopy(eaddr, saddr->sa_data, sizeof(struct ether_addr));
}

# include <rpcsvc/ypclnt.h>

/*
 * get the appropriate network mask entry given an address
 */
char *
getmaskbyaddr(netname)
	char *netname;
{
	static char *ypDomain = NULL;
	char *out, *strtok();
	int keylen, outsize, stat;
	FILE *f;
	static char line[1024];

	if (ypDomain == NULL)
		yp_get_default_domain(&ypDomain);
	if (ypDomain != NULL) {
		keylen = strlen(netname);
		stat = yp_match(ypDomain, "netmasks.byaddr", 
			netname, keylen, &out, &outsize);
		if (stat == 0)
			return(out);
		if (stat == YPERR_KEY)
			return(0);
	}
	  /*
	   * NIS did not work - read the local file
	   */
	f = fopen("/etc/netmasks", "r");
	if (f == NULL)
		return(0);
	while (fgets(line, sizeof(line)-1, f)) {
		out = strtok(line, " \t\n");
		if (strcmp(line,netname) == 0) {
			out = strtok(NULL, " \t\n");
			return(out);
		}
	}
	fclose(f);
	return(0);
}


/*
 * Look in the NIS for the network mask.
 * returns true if we found one to set.
 */
in_getmask(saddr)
	struct sockaddr_in *saddr;
{
	char *out;
	u_char *bytes;
	int val;
	char key[128];
	struct in_addr net;
	u_long i;

	if (!setaddr) {
	  /*
	   * If we do not already have an address to set, then we just
	   * read the interface to see if it is already set.
	   */
		strncpy(ifr.ifr_name, name, sizeof (ifr.ifr_name));
		if (ioctl(s, SIOCGIFADDR, (caddr_t)&ifr) < 0) {
			if (errno != EADDRNOTAVAIL)
				printf("Need net number for mask\n");
			return(0);
		}
		sin = * ( (struct sockaddr_in *)&ifr.ifr_addr);
	}
	i = ntohl(sin.sin_addr.s_addr);
	bytes = (u_char *)(&net);
	if (IN_CLASSA(i)) {
		net.s_addr = htonl(i & IN_CLASSA_NET);
		sprintf(key, "%d", bytes[0]);
	}
	else if (IN_CLASSB(i)) {
		net.s_addr = htonl(i & IN_CLASSB_NET);
		sprintf(key, "%d.%d", bytes[0], bytes[1]);
	}
	else {
		net.s_addr = htonl(i & IN_CLASSC_NET);
		sprintf(key, "%d.%d.%d", bytes[0], bytes[1], bytes[2]);
	}
	out = getmaskbyaddr(key);
	if (out == NULL) {
		  /*
		   * If at first not found, try unshifted too
		   */
		strcpy( key, inet_ntoa(net));
		out = getmaskbyaddr(key);
		if (out == NULL) {
			saddr->sin_addr.s_addr = INADDR_ANY;
			return(0);
		}
	}
	val = (int)inet_addr(out);
	if (val == -1) {
		printf("Invalid netmask for %s: %s\n", key, out);
		return(0);
	}
	saddr->sin_family = AF_INET;
	saddr->sin_addr.s_addr = val;
	printf("Setting netmask of %s to %s\n", name,
				inet_ntoa(saddr->sin_addr) );
	return(1);
}
