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
static	char sccsid[] = "@(#)mbuf.c 1.1 91/11/13 SMI"; /* from UCB 5.6 6/29/88 */
#endif
 
#include <stdio.h>
#include <malloc.h>
#include <sys/param.h>
#include <sys/mbuf.h>
#include <sys/stream.h>
#include <sys/strstat.h>
#define	YES	1
typedef int bool;
 
struct	mbstat mbstat;
extern	int kread();
 
static struct mbtypes {
	int	mt_type;
	char	*mt_name;
} mbtypes[] = {
	{ MT_DATA,	"data" },
	{ MT_HEADER,	"packet headers" },
	{ MT_SOCKET,	"socket structures" },
	{ MT_PCB,	"protocol control blocks" },
	{ MT_RTABLE,	"routing table entries" },
	{ MT_HTABLE,	"IMP host table entries" },
	{ MT_ATABLE,	"address resolution tables" },
	{ MT_FTABLE,	"fragment reassembly queue headers" },
	{ MT_SONAME,	"socket names and addresses" },
	{ MT_ZOMBIE,	"zombie process information" },
	{ MT_SOOPTS,	"socket options" },
	{ MT_RIGHTS,	"access rights" },
	{ MT_IFADDR,	"interface addresses" }, 
	{ 0, 0 }
};
 
int nmbtypes = sizeof(mbstat.m_mtypes) / sizeof(short);
bool seen[256];			/* "have we seen this type yet?" */
 
/*
 * Print mbuf statistics.
 */
mbpr(mbaddr)
	off_t mbaddr;
{
	register int totmem, totfree, totmbufs;
	register int i;
	register struct mbtypes *mp;
 
	if (nmbtypes != 256) {
		fprintf(stderr, "unexpected change to mbstat; check source\n");
		return;
	}
	if (mbaddr == 0) {
		printf("mbstat: symbol not in namelist\n");
		return;
	}
	kread(mbaddr, &mbstat, sizeof (mbstat));
	printf("%d/%d mbufs in use:\n",
		mbstat.m_mbufs - mbstat.m_mtypes[MT_FREE], mbstat.m_mbufs);
	totmbufs = 0;
	/*
	 * Tally up totals for known mbuf types.
	 */
	for (mp = mbtypes; mp->mt_name; mp++)
		if (mbstat.m_mtypes[mp->mt_type]) {
			seen[mp->mt_type] = YES;
			printf("\t%d mbufs allocated to %s\n",
			    mbstat.m_mtypes[mp->mt_type], mp->mt_name);
			totmbufs += mbstat.m_mtypes[mp->mt_type];
		}
	seen[MT_FREE] = YES;
	/*
	 * Tally up totals for unknown mbuf types
	 * (ones that don't appear in the mbtypes table).
	 */
	for (i = 0; i < nmbtypes; i++)
		if (!seen[i] && mbstat.m_mtypes[i]) {
			printf("\t%d mbufs allocated to <mbuf type %d>\n",
			    mbstat.m_mtypes[i], i);
			totmbufs += mbstat.m_mtypes[i];
		}
	if (totmbufs != mbstat.m_mbufs - mbstat.m_mtypes[MT_FREE])
		printf("*** %d mbufs missing ***\n",
			(mbstat.m_mbufs - mbstat.m_mtypes[MT_FREE]) - totmbufs);
	printf("%d/%d cluster buffers in use\n",
		mbstat.m_clusters - mbstat.m_clfree, mbstat.m_clusters);
	/*
	 * XXX:	Should account for MCL_LOANED mbufs here, too, but that
	 *	requires examining all the mbuf headers.
	 */
	totmem = mbstat.m_mbufs * MSIZE + mbstat.m_clusters * MCLBYTES;
	totfree = mbstat.m_mtypes[MT_FREE]*MSIZE + mbstat.m_clfree * MCLBYTES;
	printf("%d Kbytes allocated to network (%d%% in use)\n",
		totmem / 1024, (totmem - totfree) * 100 / totmem);
	printf("%d requests for memory denied\n", mbstat.m_drops);
	printf("%u requests for memory delayed\n", mbstat.m_wait);
	printf("%u calls to protocol drain routines\n", mbstat.m_drain);
}
 
 
/*
 * Streams allocation statistics code.
 */
 
struct strstat	strst;
u_int		nstrbufsz;
alcdat		*strbufstat;
 
/*
 * Print statistics for streams allocation.
 */
strstpr(straddr, nstrbufszaddr, strbufsizesaddr, strbufstataddr)
	off_t	straddr;
	off_t	nstrbufszaddr;
	off_t	strbufsizesaddr;
	off_t	strbufstataddr;
{
	register int		i;
	register u_int		size;
	register struct strstat	*s = &strst;
	register u_int		*strbufsizes;
	register alcdat		*sbs;
	char			buf[50];
 
	/*
	 * Maximize the probability of getting an internally consistent
	 * snapshot by gathering all data before printing anything.
	 */
 
	if (straddr == 0 || nstrbufszaddr == 0 || strbufsizesaddr == 0 ||
	    strbufstataddr == 0)
		return;
 
	/*
	 * Grab the strst structure (which contains overall STREAMS
	 * statistics).
	 */
	if (kread(straddr, (char *)&strst, sizeof strst) != sizeof strst) {
		printf("prstrst: bad read\n");
		return;
	}
 
	/*
	 * Get number of buckets for actual buffer allocations.
	 */
	if (kread(nstrbufszaddr, (char *)&nstrbufsz, sizeof nstrbufsz) !=
	    sizeof nstrbufsz) {
		(void) fprintf(stderr, "strstpr: bad read of nstrbufsz\n");
		return;
	}
 
	/*
	 * Allocate space for the strbufsizes array and pull it in.
	 */
	size = sizeof *strbufsizes * nstrbufsz;
	strbufsizes = (u_int *) malloc(size);
	if (strbufsizes == NULL) {
		(void) fprintf(stderr, "strstpr: bad malloc of strbufsizes\n");
		return;
	}
	if (kread(strbufsizesaddr, (char *)strbufsizes, size) != size) {
		(void) fprintf(stderr, "strstpr: bad read of strbufsizes\n");
		return;
	}
 
	/*
	 * Allocate space for the strbufstat array and pull it in.  Note that
	 * it contains two extra entries at the beginning for external buffers
	 * and buffers embedded in dblks.  Also, since the kernel itself
	 * dynamically allocates space for this array, we have to go through
	 * an extra level of indirection here.
	 */
	if (kread(strbufstataddr, (char *)&strbufstat, sizeof strbufstat) !=
	    sizeof strbufstat) {
		(void) fprintf(stderr, "strstpr: bad read of strbufstat\n");
		return;
	}
	size = sizeof *sbs * (2 + nstrbufsz);
	sbs = (alcdat *) malloc(size);
	if (strbufstat == NULL) {
		(void) fprintf(stderr,
				"strstpr: bad malloc of strbufstat array\n");
		return;
	}
	if (kread((off_t)strbufstat, (char *)sbs, size) != size) {
		(void) fprintf(stderr,
				"strstpr: bad read of strbufstat array\n");
		return;
	}
 
	/*
	 * Display overall STREAMS data.
	 */
	printf("streams allocation:\n");
	printf("%*s%s\n", 41, "", "cumulative  allocation");
	printf("%*s%s\n", 22, "", "current   maximum       total    failures");
	pf_strstat("streams", &s->stream);
	pf_strstat("queues", &s->queue);
	pf_strstat("mblks", &s->mblock);
	pf_strstat("dblks", &s->dblock);
 
	/*
	 * Display buffer-related data.
	 */
	printf("streams buffers:\n");
	pf_strstat("external", &sbs[0]);
	pf_strstat("within-dblk", &sbs[1]);
	for (i = 0; i < nstrbufsz - 1; i++) {
		(void) sprintf(buf, "size <= %5d", strbufsizes[i]);
		pf_strstat(buf, &sbs[i + 2]);
	}
	(void) sprintf(buf, "size >  %5d", strbufsizes[nstrbufsz - 2]);
	pf_strstat(buf, &sbs[nstrbufsz + 1]);
}
 
/*
 * Print a line of streams allocation information, as recorded
 * in the (alcdat *) given as argument.
 */
pf_strstat(label, alp)
	char	*label;
	alcdat	*alp;
{
	printf("%s%*s%6d    %6d     %7d      %6d\n",
		label,
		23 - strlen(label), "",	/* Move to column 24 */
		alp->use,
		alp->max,
		alp->total,
		alp->fail);
}
 
#ifdef	notdef
/*
 * Streams display layout follows.
 */
streams allocation:
                                         cumulative  allocation
                      current   maximum       total    failures
streams                000000    000000     0000000      000000
queues                 000000    000000     0000000      000000
mblks                  000000    000000     0000000      000000
dblks                  000000    000000     0000000      000000
streams buffers:
external               000000    000000     0000000      000000
within-dblk            000000    000000     0000000      000000
size <=   ll           000000    000000     0000000      000000
size <=  mmm           000000    000000     0000000      000000
size <= nnnn           000000    000000     0000000      000000
size >  nnnn           000000    000000     0000000      000000
#endif	notdef

