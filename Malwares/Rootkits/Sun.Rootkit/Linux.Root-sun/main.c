/*+
 *  sun4 netstat patch.  Hides network connections
 *  based upon ip or port.
+*/


/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
 
#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1983 Regents of the University of California.\n\
 All rights reserved.\n";
#endif not lint
 
#ifndef lint
static	char sccsid[] = "@(#)main.c 1.1 91/11/13 SMI"; /* from UCB 5.7 5/22/86 */
#endif
 
#include <sys/param.h>
#include <sys/vmmac.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <kvm.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <nlist.h>
#include <stdio.h>
 
struct nlist nl[] = {
#define	N_MBSTAT	0
	{ "_mbstat" },
#define	N_IPSTAT	1
	{ "_ipstat" },
#define	N_TCB		2
	{ "_tcb" },
#define	N_TCPSTAT	3
	{ "_tcpstat" },
#define	N_UDB		4
	{ "_udb" },
#define	N_UDPSTAT	5
	{ "_udpstat" },
#define	N_IFNET		6
	{ "_ifnet" },
#define	N_HOSTS		7
	{ "_hosts" },
#define	N_RTHOST	8
	{ "_rthost" },
#define	N_RTNET		9
	{ "_rtnet" },
#define	N_ICMPSTAT	10
	{ "_icmpstat" },
#define	N_RTSTAT	11
	{ "_rtstat" },
#define	N_NFILE		12
	{ "_nfile" },
#define	N_FILE		13
	{ "_file" },
#define	N_UNIXSW	14
	{ "_unixsw" },
#define N_RTHASHSIZE	15
	{ "_rthashsize" },
#define N_IDP		16
	{ "_nspcb"},
#define N_IDPSTAT	17
	{ "_idpstat"},
#define N_SPPSTAT	18
	{ "_spp_istat"},
#define N_NSERR		19
	{ "_ns_errstat"},
#define	N_STRST		20
	{ "_strst" },
#define	N_NSTRBUFSZ	21
	{ "_nstrbufsz" },
#define	N_STRBUFSIZES	22
	{ "_strbufsizes" },
#define	N_STRBUFSTAT	23
	{ "_strbufstat" },
	"",
};
 
/* internet protocols */
extern	int protopr();
extern	int tcp_stats(), udp_stats(), ip_stats(), icmp_stats();
#ifdef	ENABLE_XNS
extern	int nsprotopr();
extern	int spp_stats(), idp_stats(), nserr_stats();
#endif	ENABLE_XNS
 
struct protox {
	u_char	pr_index;		/* index into nlist of cb head */
	u_char	pr_sindex;		/* index into nlist of stat block */
	u_char	pr_wanted;		/* 1 if wanted, 0 otherwise */
	int	(*pr_cblocks)();	/* control blocks printing routine */
	int	(*pr_stats)();		/* statistics printing routine */
	char	*pr_name;		/* well-known name */
} protox[] = {
	{ N_TCB,	N_TCPSTAT,	1,	protopr,
	  tcp_stats,	"tcp" },
	{ N_UDB,	N_UDPSTAT,	1,	protopr,
	  udp_stats,	"udp" },
	{ -1,		N_IPSTAT,	1,	0,
	  ip_stats,	"ip" },
	{ -1,		N_ICMPSTAT,	1,	0,
	  icmp_stats,	"icmp" },
	{ -1,		-1,		0,	0,
	  0,		0 }
};
 
#ifdef	ENABLE_XNS
struct protox nsprotox[] = {
	{ N_IDP,	N_IDPSTAT,	1,	nsprotopr,
	  idp_stats,	"idp" },
	{ N_IDP,	N_SPPSTAT,	1,	nsprotopr,
	  spp_stats,	"spp" },
	{ -1,		N_NSERR,	1,	0,
	  nserr_stats,	"ns_err" },
	{ -1,		-1,		0,	0,
	  0,		0 }
};
#endif	ENABLE_XNS
 
char	*system = NULL;
char	*kmemf = NULL;
kvm_t	*kd;
int	Aflag;
int	aflag;
int	iflag;
int	mflag;
int	nflag;
int	rflag;
int	sflag;
int	tflag;
int	interval;
char	*interface;
int	unit;
char	usage[] = "[ -Aaimnrst ] [-f address_family] [-I interface] [ interval ] [ system ] [ core ]";
 
int	af = AF_UNSPEC;
 
main(argc, argv)
	int argc;
	char *argv[];
{
	char *cp, *name;
	register struct protoent *p;
	register struct protox *tp;
 
	name = argv[0];
	argc--, argv++;
  	while (argc > 0 && **argv == '-') {
		for (cp = &argv[0][1]; *cp; cp++)
		switch(*cp) {
 
		case 'A':
			Aflag++;
			break;
 
		case 'a':
			aflag++;
			break;
 
		case 'i':
			iflag++;
			break;
 
		case 'm':
			mflag++;
			break;
 
		case 'n':
			nflag++;
			break;
 
		case 'r':
			rflag++;
			break;
 
		case 's':
			sflag++;
			break;
 
		case 't':
			tflag++;
			break;
 
		case 'u':
			af = AF_UNIX;
			break;
 
		case 'f':
			argv++;
			argc--;
			if (argc < 1) {
			   fprintf(stderr, "address family not specified\n");
			   exit(10);
			}
			if (strcmp(*argv, "inet") == 0)
				af = AF_INET;
#ifdef	ENABLE_XNS
			else if (strcmp(*argv, "ns") == 0)
				af = AF_NS;
#endif	ENABLE_XNS
			else if (strcmp(*argv, "unix") == 0)
				af = AF_UNIX;
			else {
				fprintf(stderr, "%s: unknown address family\n",
					*argv);
				exit(10);
			}
			break;
			
		case 'I':
			iflag++;
			if (*(interface = cp + 1) == 0) {
				if ((interface = argv[1]) == 0)
					break;
				argv++;
				argc--;
			}
			for (cp = interface; isalpha(*cp); cp++)
				;
			unit = atoi(cp);
			*cp-- = 0;
			break;
 
		default:
use:
			printf("usage: %s %s\n", name, usage);
			exit(1);
		}
		argv++, argc--;
	}
	if (argc > 0 && isdigit(argv[0][0])) {
		interval = atoi(argv[0]);
		if (interval <= 0)
			goto use;
		argv++, argc--;
		iflag++;
	}
	if (argc > 0) {
		system = *argv;
		argv++, argc--;
	}
	if (argc > 0)
		kmemf = *argv;
 
	if ((kd = kvm_open(system, kmemf, NULL, O_RDONLY, argv[0])) == NULL) {
		if (system == NULL)
			fprintf(stderr, "Unable to read kernel VM\n");
		else
			fprintf(stderr, "Unable to read kernel VM for %s\n",
			    system);
		exit(1);
	}
	if (kvm_nlist(kd, nl) < 0) {
		fprintf(stderr, "netstat: bad namelist\n");
		exit(1);
	}
	if (mflag) {
		mbpr(nl[N_MBSTAT].n_value);
		printf("\n");
		strstpr(nl[N_STRST].n_value, nl[N_NSTRBUFSZ].n_value,
		    nl[N_STRBUFSIZES].n_value, nl[N_STRBUFSTAT].n_value);
		exit(0);
	}
	/*
	 * Keep file descriptors open to avoid overhead
	 * of open/close on each call to get* routines.
	 */
	sethostent(1);
	setnetent(1);
	if (iflag) {
		intpr(interval, nl[N_IFNET].n_value);
		exit(0);
	}
	if (rflag) {
		if (sflag)
			rt_stats(nl[N_RTSTAT].n_value);
		else
			routepr(nl[N_RTHOST].n_value, nl[N_RTNET].n_value,
				nl[N_RTHASHSIZE].n_value);
		exit(0);
	}

    if (af == AF_INET || af == AF_UNSPEC) {
	setprotoent(1);
	setservent(1);
	while (p = getprotoent()) {
 
		for (tp = protox; tp->pr_name; tp++)
			if (strcmp(tp->pr_name, p->p_name) == 0)
				break;
		if (tp->pr_name == 0 || tp->pr_wanted == 0)
			continue;
		if (sflag) {
			if (tp->pr_stats)
				(*tp->pr_stats)(nl[tp->pr_sindex].n_value,
					p->p_name);
		} else
			if (tp->pr_cblocks)
				(*tp->pr_cblocks)(nl[tp->pr_index].n_value,
					p->p_name);
	}
	endprotoent();
    }

#ifdef	ENABLE_XNS
    if (af == AF_NS || af == AF_UNSPEC) {
	for (tp = nsprotox; tp->pr_name; tp++) {
		if (sflag) {
			if (tp->pr_stats)
				(*tp->pr_stats)(nl[tp->pr_sindex].n_value,
					tp->pr_name);
		} else
			if (tp->pr_cblocks)
				(*tp->pr_cblocks)(nl[tp->pr_index].n_value,
					tp->pr_name);
	}
    }
#endif	ENABLE_XNS
    if ((af == AF_UNIX || af == AF_UNSPEC) && !sflag)
	    unixpr(nl[N_NFILE].n_value, nl[N_FILE].n_value,
		nl[N_UNIXSW].n_value);
    exit(0);
	/* NOTREACHED */
}
 
kread(addr, buf, nbytes)
	unsigned long addr;
	char *buf;
	unsigned nbytes;
{
	return kvm_read(kd, addr, buf, nbytes);
}
 
char *
plural(n)
	int n;
{
 
	return (n != 1 ? "s" : "");
}

