/*  @(#)revarp.c 1.1 91/11/13 SMI  */

/*
 * Copyright (c) 1990 by Sun Microsystems, Inc.
 */


#include <stdio.h>
#include <netdb.h>

#include <sys/types.h>

#include <sys/errno.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stropts.h>
#include <sys/resource.h>

#include <net/if.h>
#include <net/if_arp.h>
#include <net/nit_if.h>
#include <net/nit_pf.h>
#include <net/packetfilt.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip_var.h>
#include <netinet/udp.h>
#include <netinet/udp_var.h>


#define NIT_DEV		"/dev/nit"

#define BUFSIZE		10000

#define MAXIFS		16

static struct ether_addr my_etheraddr;
static struct ether_addr targ_etheraddr;

struct ether_addr etherbroadcast = 
		{{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }};

/* global flags - should come from command line*/
int debug = 0;
int safe = 0;

/* globals from ifconfig.c */
extern int setaddr;
extern char name[];
extern struct sockaddr_in sin;


static void	get_etheraddr ();	/* get ethernet addr from IF */

extern char	*malloc();
extern char	*inet_ntoa();

struct rarp_request {
	struct	ether_header rr_eheader;
	struct ether_arp rr_arpheader;
	u_short rr_len;  /* not part of the protocol; fill in at "read" time */
};

#ifdef REVARP_MAIN
main(argc, argv)
	int argc;
	char **argv;
{
	struct ifreq reqbuf [MAXIFS];
	struct ifreq *ifr;
	struct ifconf ifc;
	char *cmdname;
	char *targethost = (char *) 0;
	int s;
	int n;
	int c;
	extern char *optarg;
	extern int optind;

	cmdname = argv[0];

        while ((c = getopt(argc, argv, "adf:nv")) != -1) {

		switch ((char) c) {
		case 'a':
			allflag++;
			break;

		case 'd':
			debug++;
			break;

		case 'f':
			targethost = optarg;
			break;

		case 'n':
			safe++;
			break;
			
		case 'v':
			verbose++;
			break;
			
		case '?':
			usage(cmdname);
			exit(1);
		}
	}



	if (allflag) {
		if ((s = socket (AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror ("socket");
			exit (1);
		}
		ifc.ifc_buf = (caddr_t) &reqbuf [0];
		ifc.ifc_len = sizeof (reqbuf);
		if (ioctl (s, SIOCGIFCONF, (char *) &ifc) < 0) {
			perror ("ioctl SIOCGIFCONF");
			exit (1);
		}
		ifr = ifc.ifc_req;
		n = ifc.ifc_len / sizeof (struct ifreq);
		for (; n > 0 ; n--, ifr++) {
			if (ioctl (s, SIOCGIFFLAGS, (char *) ifr) < 0) {
				perror ("ioctl SIOCGIFFLAGS");
				exit (1);
			}
			if ((ifr->ifr_flags & IFF_LOOPBACK) ||
			    (ifr->ifr_flags & IFF_NOARP) ||
			    (ifr->ifr_flags & IFF_POINTOPOINT))
				continue;
			do_revarp(ifr->ifr_name, targethost);
		}
		exit(0);
	}

	if (optind >= argc) {
		/* no interface names were specified */
		usage(cmdname);
		exit(1);
	}

	for (; optind < argc; optind++) {
		do_revarp(argv[optind], targethost);
	}
}
#endif

alarmclock()
{
	fprintf(stderr, 
		"%s: RARP timed out.\n", name);
	resetifup();
	fflush(stderr);
	exit(1);
}


int origflags;

/* temporarily set interface flags to IFF_UP so that we can send the revarp */

setifup()
{
	int s;
	struct ifreq ifr;

	if ((s = socket (AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror ("socket");
		exit (1);
	}

	strncpy (ifr.ifr_name, name, sizeof(ifr.ifr_name));
	if (ioctl (s, SIOCGIFFLAGS, (char *) &ifr) < 0) {
		perror ("ioctl SIOCGIFFLAGS");
		exit (1);
	}
	origflags = ifr.ifr_flags;
	ifr.ifr_flags |= IFF_UP;

	if (ioctl (s, SIOCSIFFLAGS, (char *) &ifr) < 0) {
		perror ("ioctl SIOCSIFFLAGS");
		exit (1);
	}
}
	

resetifup()
{
	int s;
	struct ifreq ifr;
	
	if ((s = socket (AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror ("socket");
		exit (1);
	}
	
	strncpy (ifr.ifr_name, name, sizeof(ifr.ifr_name));
	if (ioctl (s, SIOCGIFFLAGS, (char *) &ifr) < 0) {
		perror ("ioctl SIOCGIFFLAGS");
		exit (1);
	}

	ifr.ifr_flags = origflags;

	if (ioctl (s, SIOCSIFFLAGS, (char *) &ifr) < 0) {
		perror ("ioctl SIOCSIFFLAGS");
		exit (1);
	}

}

setifrevarp(arg, param)
	char *arg;
	int param;
{
	register int i;
	char *buf, *cause;
	struct rarp_request req;
	struct rarp_request ans;
	char host[256];
	struct timeval now, then;
	struct in_addr from;
	struct in_addr answer;
	int received = 0;
	int bad;
	static int	if_fd;			/* NIT stream with filter */
	int s;
	struct ifreq ifr;

	if (name == (char *) 0) {
		fprintf(stderr, "setifrevarp: name not set\n");
		exit(1);
	}

	if (debug)
		printf("setifrevarp interface %s\n", name);
	
	
	if ((s = socket (AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror ("socket");
		exit (1);
	}
	
	strncpy (ifr.ifr_name, name, sizeof(ifr.ifr_name));
	if (ioctl (s, SIOCGIFFLAGS, (char *) &ifr) < 0) {
		perror ("ioctl SIOCGIFFLAGS");
		exit (1);
	}

	/* Don't try to revarp if we know it won't work. */
	if ((ifr.ifr_flags & IFF_LOOPBACK) ||
	    (ifr.ifr_flags & IFF_NOARP) ||
	    (ifr.ifr_flags & IFF_POINTOPOINT))
		return;

	/*
	 * Open interface and pull out goodies:
	 *   ieee802 (48 bit) address, IP address, IP subnet mask
	 */
	if_fd = rarp_open (name, htons(ETHERTYPE_REVARP));
	if (if_fd < 0) {
		exit(1);
	}

	get_etheraddr(if_fd, name, &my_etheraddr);

	if (debug) {
		printf("my_etheraddr = %s\n", ether_ntoa(&my_etheraddr));
	}

	/* create the request */
	bzero ((char *) &req, sizeof(req));
	
	bcopy ((char *) &etherbroadcast, (char *) &req.rr_eheader.ether_dhost,
	       sizeof(etherbroadcast));
	bcopy ((char *) &my_etheraddr, (char *) &req.rr_eheader.ether_shost,
	       sizeof(my_etheraddr));
	req.rr_eheader.ether_type = ETHERTYPE_REVARP;

	req.rr_arpheader.arp_hrd = ARPHRD_ETHER;
	req.rr_arpheader.arp_pro= ETHERTYPE_IP;
	req.rr_arpheader.arp_hln= 6;
	req.rr_arpheader.arp_pln= 4;
	req.rr_arpheader.arp_op= REVARP_REQUEST;

	bcopy ((char *) &my_etheraddr, (char *)&req.rr_arpheader.arp_sha,
	       sizeof(my_etheraddr));
	/* don't know arp_spa */
	bcopy ((char *) &my_etheraddr, (char *)&req.rr_arpheader.arp_tha,
	       sizeof(my_etheraddr));
	/* don't know arp_tpa */

	setifup();

	/* send the request */
	i = sizeof(req) - sizeof(req.rr_len);
	req.rr_len = i;
	if (rarp_write (if_fd, (char *) &req, i) != i) {
		perror("write if_fd");
		exit(1);
	}

	if (debug)
		printf("rarp sent.\n");

	signal(SIGALRM, alarmclock);
	alarm(10);

	/* read the answers */
	buf = malloc(BUFSIZE);
	for ( ; ; received++) {
		if ((i = read(if_fd, buf, BUFSIZE)) < 0) {
			perror("rarpd: read");
			exit(11);
		}
		bcopy (buf, (char *) &ans, sizeof (ans));

		ans.rr_len = i;
		/*
		 * Sanity checks ... set i to sizeof an rarp packet
		 */
		i = sizeof (ans) - sizeof (ans.rr_len);
		cause = 0;

		if (ans.rr_len < i)
			cause="rr_len";
		else if (ans.rr_eheader.ether_type != ntohs(ETHERTYPE_REVARP))
			cause="type";
		else if (ans.rr_arpheader.arp_hrd != htons(ARPHRD_ETHER))
			cause="hrd";
		else if (ans.rr_arpheader.arp_pro != htons(ETHERTYPE_IP))
			cause="pro";
		else if (ans.rr_arpheader.arp_hln != 6)
			cause="hln";
		else if (ans.rr_arpheader.arp_pln != 4)
			cause="pln";
		if (cause) {
			(void) fprintf(stderr,
				       "sanity check failed; cause: %s\n",
				       cause);
			continue;
		}
		switch (ntohs(ans.rr_arpheader.arp_op)) {
		case ARPOP_REQUEST:
			if (debug)
				printf("Got an arp request\n");
			break;

		case ARPOP_REPLY:
			if (debug)
				printf("Got an arp reply.\n");
			break;

		case REVARP_REQUEST:
			if (debug)
				printf ("Got an rarp request.\n");
			break;

		case REVARP_REPLY:
			bcopy (ans.rr_arpheader.arp_tpa, &answer, 
			       sizeof(answer));
			bcopy (ans.rr_arpheader.arp_spa, &from, 
			       sizeof(from));
			if (debug) {
				printf("Answer: %s", inet_ntoa(answer));
				printf(" [from %s]\n", inet_ntoa(from));
			}
			sin.sin_addr.s_addr = answer.s_addr;
			if (!safe) {
				setaddr++;
				resetifup();
				return;
			}
			break;

		default:
			(void) fprintf (stderr,
					"unknown opcode 0x%xd\n", 
					ans.rr_arpheader.arp_op);
			bad++;
			break;
		}
	}
}

/*
 * Obtain the Ethernet address of the interface named
 * by dev (and bound to fd), assigning it to *ap.
 */
static void
get_etheraddr(fd, dev, ap)
	int			fd;
	char			*dev;
	struct ether_addr	*ap;
{
	struct strioctl	si;
	struct ifreq	ifr;

	(void) strncpy(ifr.ifr_name, dev, sizeof ifr.ifr_name);

	si.ic_timout = INFTIM;
	si.ic_cmd = SIOCGIFADDR;
	si.ic_len = sizeof ifr;
	si.ic_dp = (char *)&ifr;

	if (ioctl(fd, I_STR, (char *)&si) < 0) {
		perror("ioctl (I_STR: SIOCGIFADDR)");
		exit(15);
	}

	*ap = *(struct ether_addr *) &ifr.ifr_addr.sa_data[0];
}

/*
 * Open the NIT device, and establish the following characteristics.
 * 1)	Bind it to device.
 * 2)	Push an instance of the packet-filtering module on it that passes
 *	through only packets whose Ethernet type field is type.
 * 3)	Set it into message-discard mode.
 *
 * Return the resulting descriptor.
 */
static int
rarp_open(device, type)
	char *device;
	u_short type;
{
	struct strioctl si;
	struct ifreq ifr;
	struct ether_header eh;		/* used only for offset values */
	struct packetfilt pf;
	register u_short *fwp;
	register int fd;

	if ((fd = open(NIT_DEV, O_RDWR)) < 0) {
		perror("nit open");
		return (-1);
	}

	if (ioctl(fd, I_SRDOPT, (char *)RMSGD) < 0) {
		perror("ioctl (I_SRDOPT)");
		return (-2);
	}

	/*
	 * Push the packet filtering module.
	 */
	if (ioctl(fd, I_PUSH, "pf") < 0) {
		perror("ioctl (I_PUSH \"pf\")");
		return (-3);
	}

	/*
	 * Set up filter.
	 */
	fwp = &pf.Pf_Filter[0];
	*fwp++ = ENF_PUSHWORD +	
		((u_int) &eh.ether_type - (u_int) &eh.ether_dhost) / 
			sizeof (u_short);
	*fwp++ = ENF_PUSHLIT | ENF_EQ;
	*fwp++ = type;			/* arg is in network byte order */
	pf.Pf_FilterLen = fwp - &pf.Pf_Filter[0];
	pf.Pf_Priority = 5;		/* unimportant, so long as > 2 */

	/*
	 * Package up the NIOCSETF ioctl and issue it.
	 */
	si.ic_cmd = NIOCSETF;
	si.ic_timout = INFTIM;
	si.ic_len = sizeof pf;
	si.ic_dp = (char *)&pf;

	if (ioctl(fd, I_STR, (char *)&si) < 0) {
		perror("ioctl (I_STR: NIOCSETF)");
		return (-4);
	}

	/*
	 * We defer the bind until after we've pushed the filter
	 * to prevent our being flooded with extraneous packets.
	 */
	(void) strncpy(ifr.ifr_name, device, sizeof ifr.ifr_name - 1);
	ifr.ifr_name[sizeof ifr.ifr_name - 1] = '\0';
	/*
	 * Package so the stream head can cope with it.
	 */
	si.ic_cmd = NIOCBIND;
	si.ic_len = sizeof ifr;
	si.ic_dp = (char *)&ifr;

	if (ioctl(fd, I_STR, (char *)&si) < 0) {
		perror("ioctl (I_STR: NIOCBIND)");
		return (-5);
	}

	return (fd);
}



usage(cmdname)
	char *cmdname;
{
	(void) fprintf(stderr,
		       "Usage: %s [-a] [-d] [-n] [-v] [<ifname> . . .]\n", 
		       cmdname);
}

static int
rarp_write(fd, buf, len)
	int fd, len;
	char *buf;
{
	struct sockaddr sa;
	struct strbuf cbuf, dbuf;
	/*
	 * Sleaze!  Offset used ambiguously, taking advantage of fact that
	 * sizeof sa.sa_data == sizeof (struct ether_header).
	 */
	register int offset = sizeof(sa.sa_data);

	sa.sa_family = AF_UNSPEC;
	bcopy(buf, sa.sa_data, offset);

	/*
	 * Set up NIT (write-side) protocol info.
	 */
	cbuf.maxlen = cbuf.len = sizeof sa;
	cbuf.buf = (char *)&sa;

	/*
	 * The interface output routines will paste the
	 * ether header back onto the front of the message.
	 */
	dbuf.maxlen = dbuf.len = len - offset;
	dbuf.buf = buf + offset;

	if (putmsg(fd, &cbuf, &dbuf, 0) < 0) {
		perror("rarp_write: putmsg");
		return (-1);
	}

	return (len);
}

