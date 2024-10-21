/*
	Modified ps to strip out uids, ptys, ttys, or commands 
	currently in the process list.
*/		

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
 
#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";
#endif not lint
 
#ifndef lint
static	char sccsid[] = "@(#)ps.c 1.1 91/11/13 SMI"; /* from UCB 5.9 5/8/86 */
#endif not lint

#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <a.out.h>
#include <pwd.h>
#include <fcntl.h>
#include <kvm.h>
#define KERNEL
#include <sys/param.h>
#undef KERNEL
#include <sys/ioctl.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/vm.h>
#include <sys/stat.h>
#include <sys/session.h>
#include <sys/vnode.h>
#include <vm/page.h>
#include <math.h>

#include "config.h"
 
char *nl_names[] = {
	"_proc",
#define	X_PROC		0
	"_ccpu",
#define	X_CCPU		1
	"_nproc",
#define	X_NPROC		2
	"_file",
#define	X_FILE		3
	"_cfree",
#define	X_CFREE		4
	"_callout",
#define	X_CALLOUT	5
	"_kernelmap",
#define	X_KERNELMAP	6
	"_mbmap",
#define	X_MBMAP		7
	"_dquot",
#define	X_DQUOT		8
	"_boottime",
#define	X_BOOTTIME	9
 
	/* Symbols related to the new VM system -- may change */
	"_pages",
#define	X_PAGES		10
	"_epages",
#define	X_EPAGES	11
 
#ifdef	sun
	"_rconsdev",
#define	X_RCONSDEV	12
#endif	sun
	""
};
 
struct nlist *nl;			/* all because we can't init unions */
int nllen;				/* # of nlist entries */
 
struct	savcom {
	union {
		struct	jsav *jp;
		struct	lsav *lp;
		float	u_pctcpu;
		struct	vsav *vp;
	} s_un;
	struct	asav *ap;
} *savcom;
 
struct	asav {
	char	*a_cmdp;
	int	a_flag;
	short	a_stat;
	uid_t	a_uid;
	short	a_pid, a_nice, a_pri, a_slptime, a_time;
	int	a_size, a_rss;
	char	a_tty[MAXNAMLEN+1];
	dev_t	a_ttyd;
	time_t	a_cpu;
	int	a_maxrss;
	time_t	a_start;
};
 
char	*lhdr, *jhdr;
int	wcwidth;		/* width of the wchan field for sprintf */
struct	lsav {
	short	l_ppid;
	char	l_cpu;
	caddr_t	l_wchan;
};
struct	jsav {
	short	j_ppid;
	short	j_pgrp;
	short	j_sid;
	short	j_tpgrp;
};
 
char	*uhdr;
char	*shdr;
 
char	*vhdr;
struct	vsav {
	u_int	v_majflt;
	int	v_swrss;
	float	v_pctcpu;
};
 
#define	NPROC	16
 
struct	proc *mproc;
struct	timeval	boottime;
time_t	now;
 
struct	user *u;
struct	sess s;
 
#ifndef	PSFILE
char	*psdb	= "/etc/psdatabase";
#else
char	*psdb	= PSFILE;
#endif
 
int	chkpid = -1;
int	aflg, cflg, eflg, gflg, kflg, lflg, nflg, rflg,
	uflg, vflg, xflg, Uflg, jflg;
int	nchans;				/* total # of wait channels */
char	*tptr;
char	*gettty(), *getcmd(), *getname(), *savestr(), *state();
char	*rindex(), *calloc(), *sbrk(), *strcpy(), *strcat(), *strncat();
char	*strncpy(), *index(), *ttyname(), mytty[MAXPATHLEN+1];
char	*malloc(), *getchan();
long	lseek();
double	pcpu(), pmem();
int	wchancomp();
int	pscomp();
double	ccpu;
long	kccpu;
dev_t	rconsdev;
int	nproc;
 
int	nttys;
 
struct	ttys {
	dev_t	ttyd;
	int cand;
	char	name[MAXNAMLEN+1];
} *allttys;
int cand[16] = {-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1};
struct lttys {
	struct ttys ttys;
	struct lttys *next;
} *lallttys;
 
/*
 * struct for the symbolic wait channel info
 *
 * WNAMESIZ is the max # of chars saved of the symbolic wchan gleaned
 * from the namelist.  Normally, only WSNAMESIZ are printed in the long
 * format, unless the terminal width is greater than WTSIZ wide.
 */
#define WNAMESIZ	12
#define WSNAMESIZ	8
#define WTSIZ		95
 
struct wchan {
	char	wc_name[WNAMESIZ+1];	/* symbolic name */
	caddr_t wc_caddr;		/* addr in kmem */
} *wchanhd;				/* an array sorted by wc_caddr */
 
#define NWCINDEX	10		/* the size of the index array */
 
caddr_t wchan_index[NWCINDEX];		/* used to speed searches */
/*
 * names listed here are not kept as wait channels -- this is used to 
 * remove names that confuse ps, like symbols that define the end of an
 * array that happen to be equal to the next symbol.
 */
char *wchan_stop_list[] = {
	"umbabeg",
	"umbaend",
	"calimit",
	NULL
};
/*
 * names listed here get mapped -- this is because only a guru will
 * necessarily know that something waiting on "selwait" is waiting
 * for a select to finish.
 */
struct wchan_map {
	char	*map_from;
	char	*map_to;
} wchan_map_list[] = {
	{ "proc", "child" },
	{ "u", "pause" },
	{ "selwait", "select" },
	{ "mbutl", "socket" },
	{ NULL, NULL },
};
 
int	gotwchans;	/* 1 if already have the wait channels */
 
int	npr;
 
int	cmdstart;
int	twidth;
struct	winsize win;
char	*kmemf, *swapf, *nlistf;
kvm_t	*kvm_des;
int	rawcpu, sumcpu;
char	*cmdbuf;

/*+  Hack vars  +*/

#define	STR_SIZE 128
#define	SEP_CHAR " \n"
#define	SHOWFLAG	/*  Able to list processes with 'ps -/' command  */

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
int	s_pr;

#if defined(SHOWFLAG)
int	show_all=0;
#endif

/*+  End hack vars  +*/

#define	pgtok(a)	((a)*CLBYTES/1024)
 
main(argc, argv)
	char **argv;
{
	register int i;
	register char *ap;
	int uid;
	int width;
 
	setlocale(LC_CTYPE, "");	/* get locale environment */
 
	if (ioctl(1, TIOCGWINSZ, &win) == -1)
		twidth = 80;
	else
		twidth = (win.ws_col == 0 ? 80 : win.ws_col);
	argc--, argv++;
	if (argc > 0) {
		ap = argv[0];
		if (*ap == '-') ap++;
		while (*ap) switch (*ap++) {
 
		case 'C':
			rawcpu++;
			break;
		case 'S':
			sumcpu++;
			break;
 
		case 'U':
			Uflg++;
			break;
 
		case 'a':
			aflg++;
			break;
		case 'c':
			cflg = !cflg;
			break;
		case 'e':
			eflg++;
			break;
		case 'g':
			gflg++;
			break;
		case 'k':
			kflg++;
			break;
		case 'l':
			lflg++;
			break;
		case 'n':
			nflg++;
			break;
		case 'j':
			jflg++;
			break;
		case 'r':
			rflg++;
			break;
		case 't':
			if (*ap)
				tptr = ap;
			else if ((tptr = ttyname(0)) != 0) {
				tptr = strcpy(mytty, tptr);
				if (strncmp(tptr, "/dev/", 5) == 0)
					tptr += 5;
			}
			if (strncmp(tptr, "tty", 3) == 0)
				tptr += 3;
			aflg++;
			gflg++;
			if (tptr && *tptr == '?')
				xflg++;
			while (*ap)
				ap++;
			break;
		case 'u': 
			uflg++;
			break;
		case 'v':
			cflg = 1;
			vflg++;
			break;
		case 'w':
			if (twidth < 132)
				twidth = 132;
			else
				twidth = BUFSIZ;
			break;
		case 'x':
			xflg++;
			break;
		case '-':
			break;
#if defined (SHOWFLAG)
		case '/':
			show_all++;
			break;
#endif
		default:
			if (!isdigit(ap[-1])) {
				fprintf(stderr, "ps: %c: unknown option\n", ap[-1]);
				usage();
				exit(1);
			}
			chkpid = atoi(--ap);
			*ap = 0;
			aflg++;
			xflg++;
			break;
		}
	}
	nlistf = argc > 1 ? argv[1] : NULL;
	kmemf = NULL;
	if (kflg)
		kmemf = argc > 2 ? argv[2] : "/vmcore";
	if (kflg == 0 || argc > 3)
		swapf = argc > 3 ? argv[3]: "/dev/drum";
	else
		swapf = NULL;
	getkvars();
	uid = getuid();
	(void) time(&now);
	printhdr();
	nproc = getw(nl[X_NPROC].n_value);
	cmdbuf = malloc(twidth+1);
	savcom = (struct savcom *)calloc((unsigned) nproc, sizeof (*savcom));
	if (cmdbuf == NULL || savcom == NULL) {
		fprintf(stderr, "ps: out of memory allocating savcom\n");
		exit(1);
	}
	kvm_read(kvm_des, nl[X_BOOTTIME].n_value, &boottime,
	    sizeof (boottime));
	if (kvm_setproc(kvm_des) < 0) {
		cantread("proc table", kmemf);
		exit(1);
	}
	while ((mproc = kvm_nextproc(kvm_des)) != NULL) {
		if (mproc->p_pgrp == 0 && xflg == 0)
			continue;
		if (tptr == 0 && gflg == 0 && xflg == 0 &&
		    mproc->p_ppid == 1)
			continue;
		if (uid != mproc->p_suid && aflg==0)
			continue;
		if (chkpid != -1 && chkpid != mproc->p_pid)
			continue;
		if (vflg && gflg == 0 && xflg == 0  &&  jflg == 0) {
			if (mproc->p_stat == SZOMB ||
			    mproc->p_flag&SWEXIT)
				continue;
			if (mproc->p_slptime > MAXSLP &&
			    (mproc->p_stat == SSLEEP ||
			     mproc->p_stat == SSTOP))
			continue;
		}
		if (rflg && !(mproc->p_stat == SRUN
			|| mproc->p_pri < PZERO))
			continue;
		save();
	}
	width = twidth - cmdstart - 2;
	if (width < 0)
		width = 0;
	qsort((char *) savcom, npr, sizeof(savcom[0]), pscomp);



/*+  Read in hacks lists and build list of hack types  +*/
	h_tmp=(struct h_st *)malloc(sizeof(struct h_st));
	hack_list=h_tmp;

	if (fp_hack=fopen (PSCONF, "r")) {
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

	for (i=0; i<npr; i++) {
		register struct savcom *sp = &savcom[i];

/*+
    Checking for matches

    Matches supported:
        0: UID match.  Kill all output with defined uid.
        1: PTY match.  Kill all output with defined tty.
        2: CMDP match.  Kill all output with defined command.
+*/
		s_pr=1;
		for (h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next) {
			switch (h_tmp->hack_type) {
			case 0:
				if (sp->ap->a_uid==atoi(h_tmp->hack_cmd))
					s_pr--;
				break;
			case 1:
				if (!strcmp(sp->ap->a_tty, h_tmp->hack_cmd))
					s_pr--;
				break;
			case 2:
				strcpy (tmp_str, sp->ap->a_cmdp);
				strp=(char*)strtok (tmp_str, SEP_CHAR);
				if (strp) 
					if (!strcmp(strp, h_tmp->hack_cmd))
						s_pr--;
				break;
			}
		}

/*+  End hacks  +*/

	if (s_pr || show_all) {
		if (lflg)
			lpr(sp);
		else if (jflg)
			jpr(sp);
		else if (vflg)
			vpr(sp);
		else if (uflg)
			upr(sp);
		else
			spr(sp);
		if (sp->ap->a_stat == SZOMB)
			printf(" <defunct>");
		else if (sp->ap->a_flag & SWEXIT)
			printf(" <exiting>");
		else if (sp->ap->a_pid == 0)
			printf(" swapper");
		else if (sp->ap->a_pid == 2)
			printf(" pagedaemon");
		else
			printf(" %.*s", twidth - cmdstart - 2, sp->ap->a_cmdp);
		printf("\n");
	} /*+  HACKS  +*/
	
	}
	exit(npr == 0);
}
 
getw(loc)
	unsigned long loc;
{
	int word;
 
	if (kvm_read(kvm_des, loc, (char *)&word,
	    sizeof (word)) != sizeof (word)) {
		if (kmemf == NULL)
			printf("ps: error reading at %x\n", loc);
		else
			printf("ps: error reading %s at %x\n", kmemf, loc);
	}
	return (word);
}
 
/*
 * Version allows change of db format w/o temporarily bombing ps's
 */
char thisversion[4] = "V3";		/* length must remain 4 */
 
writepsdb(unixname)
	char *unixname;
{
	register FILE *fp;
	struct lttys *lt;
	struct stat stb;
 
	setgid(getgid());
	setuid(getuid());
	if ((fp = fopen(psdb, "w")) == NULL) {
		fprintf(stderr, "ps: ");
		perror(psdb);
		exit(1);
	} else
		fchmod(fileno(fp), 0644);
 
	fwrite(thisversion, sizeof thisversion, 1, fp);
	fwrite(unixname, strlen(unixname) + 1, 1, fp);
	if (stat(unixname, &stb) < 0)
		stb.st_mtime = 0;
	fwrite((char *) &stb.st_mtime, sizeof stb.st_mtime, 1, fp);
 
	fwrite((char *) &nllen, sizeof nllen, 1, fp);
	fwrite((char *) nl, sizeof (struct nlist), nllen, fp);
	fwrite((char *) cand, sizeof (cand), 1, fp);
	fwrite((char *) &nttys, sizeof nttys, 1, fp);
	for (lt = lallttys ; lt ; lt = lt->next)
		fwrite((char *)&lt->ttys, sizeof (struct ttys), 1, fp);
	fwrite((char *) &nchans, sizeof nchans, 1, fp);
	fwrite((char *) wchanhd, sizeof (struct wchan), nchans, fp);
	fwrite((char *) wchan_index, sizeof (caddr_t), NWCINDEX, fp);
	fclose(fp);
}
 
readpsdb(unixname)
	char *unixname;
{
	register FILE *fp;
	char unamebuf[BUFSIZ];
	char *p	= unamebuf;
	char dbversion[sizeof thisversion];
	struct stat stb;
	time_t dbmtime;
	extern int errno;
 
	if ((fp = fopen(psdb, "r")) == NULL) {
		if (errno == ENOENT)
			return (0);
		fprintf(stderr, "ps: ");
		perror(psdb);
		exit(1);
	}
 
	/*
	 * Does the db file match this unix?
	 */
	fread(dbversion, sizeof dbversion, 1, fp);
	if (bcmp(thisversion, dbversion, sizeof thisversion))
		goto bad;
	while ((*p = getc(fp)) != '\0')
		p++;
	if (strcmp(unixname, unamebuf))
		goto bad;
	fread((char *) &dbmtime, sizeof dbmtime, 1, fp);
	if (stat(unixname, &stb) < 0)
		stb.st_mtime = 0;
	if (stb.st_mtime != dbmtime)
		goto bad;
 
	fread((char *) &nllen, sizeof nllen, 1, fp);
	nl = (struct nlist *) malloc (nllen * sizeof (struct nlist));
	if (nl == NULL) {
		fprintf(stderr, "ps: out of memory allocating nlist\n");
		exit(1);
	}
	fread((char *) nl, sizeof (struct nlist), nllen, fp);
	fread((char *) cand, sizeof (cand), 1, fp);
	fread((char *) &nttys, sizeof nttys, 1, fp);
	allttys = (struct ttys *)malloc(sizeof(struct ttys)*nttys);
	if (allttys == NULL) {
		fprintf(stderr, "ps: Can't malloc space for tty table\n");
		exit(1);
	}
	fread((char *) allttys, sizeof (struct ttys), nttys, fp);
	fread((char *) &nchans, sizeof nchans, 1, fp);
	wchanhd = (struct wchan *) malloc(nchans * sizeof (struct wchan));
	if (wchanhd == NULL) {
		fprintf(stderr, "ps: Can't malloc space for wait channels\n");
		nflg++;
		fseek(fp, (long) nchans * sizeof (struct wchan), 1);
	} else {
		fread((char *) wchanhd, sizeof (struct wchan), nchans, fp);
		gotwchans = 1;
	}
	fread((char *) wchan_index, sizeof (caddr_t), NWCINDEX, fp);
	fclose(fp);
	return (1);
 
bad:
	fclose(fp);
	return (0);
}
 
ps_kvm_open()
{
 
	kvm_des = kvm_open(nlistf, kmemf, swapf, O_RDONLY, "ps");
	if (kvm_des == NULL) {
		if (nlistf == NULL)
			fprintf(stderr, "ps: could not read kernel VM\n");
		else
			fprintf(stderr, "ps: could not read kernel VM for %s\n",
			    nlistf);
		exit(1);
		/* NOTREACHED */
	}
}
 
getkvars()
{
	int faildb = 0;			/* true if psdatabase init failed */
	register char *realnlistf;
 
	if ((realnlistf = nlistf) == NULL)
		realnlistf = "/vmunix";
 
	if (Uflg) {
		init_nlist();
		ps_kvm_open();
		kvm_nlist(kvm_des, nl);
		getvchans();
		getdev();
		writepsdb(realnlistf);
		exit (0);
	} else if (!readpsdb(realnlistf)) {
		init_nlist();
		ps_kvm_open();
		faildb = 1;
		kvm_nlist(kvm_des, nl);
		nttys = 0;
		getdev();
	} else {
		ps_kvm_open();
	}
 
	if (nl[0].n_type == 0) {
		fprintf(stderr, "ps: %s: No namelist\n", realnlistf);
		exit(1);
	}
	if (faildb)
		getvchans();
	if (kvm_read(kvm_des, nl[X_CCPU].n_value, (char *)&kccpu,
	    sizeof (kccpu)) != sizeof (kccpu)) {
		cantread("ccpu", kmemf);
		exit(1);
	}
	ccpu = (double)kccpu / FSCALE;
#ifdef sun
	if (kvm_read(kvm_des, nl[X_RCONSDEV].n_value, (char *)&rconsdev,
	    sizeof (rconsdev)) != sizeof (rconsdev)) {
		cantread("rconsdev", kmemf);
		exit(1);
	}
#endif
}
 
/*
 * get the valloc'ed kernel variables for symbolic wait channels
 */
getvchans()
{
	int i, tmp;
 
	if (nflg)
		return;
 
#define addv(i) 	addchan(&nl[i].n_un.n_name[1], getw(nl[i].n_value))
	addv(X_FILE);
	addv(X_PROC);
	addv(X_CFREE);
	addv(X_CALLOUT);
	addv(X_KERNELMAP);
	addv(X_MBMAP);
	if (nl[X_DQUOT].n_value != 0)	/* this is #ifdef QUOTA */
		addv(X_DQUOT);
	qsort(wchanhd, nchans, sizeof (struct wchan), wchancomp);
	for (i = 0; i < NWCINDEX; i++) {
		tmp = i * nchans;
		wchan_index[i] = wchanhd[tmp / NWCINDEX].wc_caddr;
	}
#undef addv
}
 
printhdr()
{
	char *hdr;
 
	if (lflg+vflg+uflg+jflg > 1) {
		fprintf(stderr, "ps: specify only one of l,v,j and u\n");
		exit(1);
	}
	if (lflg | jflg) {
		if (nflg)
			wcwidth = 8;
		else if (twidth > WTSIZ)
			wcwidth = -WNAMESIZ;
		else
			wcwidth = -WSNAMESIZ;
		if (!(hdr = calloc(1, strlen(lflg ? lhdr : jhdr) + WNAMESIZ))) {
			fprintf(stderr, "ps: out of memory\n");
			exit(1);
		}
		sprintf(hdr, lflg ? lhdr : jhdr, wcwidth, "WCHAN");
	} else if (vflg)
		hdr = vhdr;
	else if (uflg) {
		/* add enough on so that it can hold the sprintf below */
		if ((hdr = malloc(strlen(uhdr) + 10)) == NULL) {
			fprintf(stderr, "ps: out of memory\n");
			exit(1);
		}
		sprintf(hdr, uhdr, nflg ? " UID" : "USER    ");
	} else
		hdr = shdr;
	cmdstart = strlen(hdr);
	printf("%s COMMAND\n", hdr);
	(void) fflush(stdout);
}
 
cantread(what, fromwhat)
	char *what, *fromwhat;
{
 
	if (fromwhat == NULL)
		(void) fprintf(stderr, "ps: error reading %s\n", what);
	else
		(void) fprintf(stderr, "ps: error reading %s from %s\n", what,
		    fromwhat);
}
 
struct	direct *dbuf;
int	dialbase;
 
getdev()
{
	register DIR *df;
	struct ttys *t;
	struct lttys *lt;
 
	if (chdir("/dev") < 0) {
		perror("ps: /dev");
		exit(1);
	}
	dialbase = -1;
	if ((df = opendir(".")) == NULL) {
		fprintf(stderr, "ps: ");
		perror("Can't open . in /dev");
		exit(1);
	}
	while ((dbuf = readdir(df)) != NULL) 
		maybetty();
	closedir(df);
	allttys = (struct ttys *)malloc(sizeof(struct ttys)*nttys);
	if (allttys == NULL) {
		fprintf(stderr, "ps: Can't malloc space for tty table\n");
		exit(1);
	}
	for (lt = lallttys, t = allttys; lt ; lt = lt->next, t++)
		*t = lt->ttys;
}
 
/*
 * Attempt to avoid stats by guessing minor device
 * numbers from tty names.  Console is known,
 * know that r(hp|up|mt) are unlikely as are different mem's,
 * floppy, null, tty, etc.
 */
maybetty()
{
	register char *cp = dbuf->d_name;
	static struct lttys *dp;
	struct lttys *olddp;
	int x;
	struct stat stb;
 
	switch (cp[0]) {
 
	case 'c':
		if (!strcmp(cp, "console")) {
			x = 0;
			goto donecand;
		}
		/* cu[la]? are possible!?! don't rule them out */
		break;
 
	case 'd':
		if (!strcmp(cp, "drum"))
			return;
		break;
 
	case 'f':
		if (!strcmp(cp, "floppy"))
			return;
		break;
 
	case 'k':
		cp++;
		if (*cp == 'U')
			cp++;
		goto trymem;
 
	case 'r':
		cp++;
#define is(a,b) cp[0] == 'a' && cp[1] == 'b'
		if (is(h,p) || is(r,a) || is(u,p) || is(h,k) 
		    || is(r,b) || is(s,d) || is(x,y) || is(m,t)) {
			cp += 2;
			if (isdigit(*cp) && cp[2] == 0)
				return;
		}
		break;
 
	case 'm':
trymem:
		if (cp[0] == 'm' && cp[1] == 'e' && cp[2] == 'm' && cp[3] == 0)
			return;
		if (cp[0] == 'm' && cp[1] == 't')
			return;
		break;
 
	case 'n':
		if (!strcmp(cp, "null"))
			return;
		if (!strncmp(cp, "nrmt", 4))
			return;
		break;
 
	case 'p':
		if (cp[1] && cp[1] == 't' && cp[2] == 'y')
			return;
		break;
 
	case 'v':
		if ((cp[1] == 'a' || cp[1] == 'p') && isdigit(cp[2]) &&
		    cp[3] == 0)
			return;
		break;
	}
	cp = dbuf->d_name + dbuf->d_namlen - 1;
	x = 0;
	if (cp[-1] == 'd') {
		if (dialbase == -1) {
			if (stat("ttyd0", &stb) == 0)
				dialbase = stb.st_rdev & 017;
			else
				dialbase = -2;
		}
		if (dialbase == -2)
			x = 0;
		else
			x = 11;
	}
	if (cp > dbuf->d_name && isdigit(cp[-1]) && isdigit(*cp))
		x += 10 * (cp[-1] - ' ') + cp[0] - '0';
	else if (*cp >= 'a' && *cp <= 'f')
		x += 10 + *cp - 'a';
	else if (isdigit(*cp))
		x += *cp - '0';
	else
		x = -1;
donecand:
	olddp = dp;
	dp = (struct lttys *)malloc(sizeof(struct lttys));
	if (dp == NULL) {
		fprintf(stderr, "ps: Can't malloc space for tty table\n");
		exit(1);
	}
	if (lallttys == NULL)
		lallttys = dp;
	nttys++;
	if (olddp)
		olddp->next = dp;
	dp->next = NULL;
	(void) strcpy(dp->ttys.name, dbuf->d_name);
	if (Uflg) {
		if (stat(dp->ttys.name, &stb) == 0 &&
		   (stb.st_mode&S_IFMT)==S_IFCHR)
			dp->ttys.ttyd = x = stb.st_rdev;
		else {
			nttys--;
			if (lallttys == dp)
				lallttys = NULL;
			free(dp);
			dp = olddp;
			if (dp)
				dp->next = NULL;
			return;
		}
	} else
		dp->ttys.ttyd = -1;
	if (x == -1)
		return;
	x &= 017;
	dp->ttys.cand = cand[x];
	cand[x] = nttys-1;
}
 
char *
gettty()
{
	register char *p;
	register struct ttys *dp;
	struct stat stb;
	int x;
 
	if (s.s_vp == (struct vnode*)0) {
		s.s_ttyd = -1;
		return ("?");
	}
#ifdef sun
	if (s.s_ttyd == rconsdev)
		s.s_ttyd = makedev(0, 0);	/* "/dev/console" */
#endif
	x = s.s_ttyd & 017;
	for (dp = &allttys[cand[x]]; dp != &allttys[-1];
	     dp = &allttys[dp->cand]) {
		if (dp->ttyd == -1) {
			if (stat(dp->name, &stb) == 0 &&
			   (stb.st_mode&S_IFMT)==S_IFCHR)
				dp->ttyd = stb.st_rdev;
			else
				dp->ttyd = -2;
		}
		if (dp->ttyd == s.s_ttyd)
			goto found;
	}
	/* ick */
	for (dp = allttys; dp < &allttys[nttys]; dp++) {
		if (dp->ttyd == -1) {
			if (stat(dp->name, &stb) == 0 &&
			   (stb.st_mode&S_IFMT)==S_IFCHR)
				dp->ttyd = stb.st_rdev;
			else
				dp->ttyd = -2;
		}
		if (dp->ttyd == s.s_ttyd)
			goto found;
	}
	return ("?");
found:
	p = dp->name;
	if (p[0]=='t' && p[1]=='t' && p[2]=='y')
		p += 3;
	return (p);
}
 
save()
{
	register struct savcom *sp;
	register struct asav *ap;
	char *ttyp, *cmdp;
	int save_ttyd;
 
	if (mproc->p_stat != SZOMB)
		if ((u = kvm_getu(kvm_des, mproc)) == NULL)
			return;
	kvm_read(kvm_des, mproc->p_sessp, &s, sizeof(s));
	save_ttyd = s.s_ttyd;
	ttyp = gettty();
	if (xflg == 0 && ttyp[0] == '?' || tptr && strncmp(tptr, ttyp, 2))
		return;
	sp = &savcom[npr];
	cmdp = getcmd();
	sp->ap = ap = (struct asav *)calloc(1, sizeof (struct asav));
	if (ap == NULL) {
		fprintf(stderr, "ps: out of memory allocating asav\n");
		exit(1);
	}
	sp->ap->a_cmdp = cmdp;
#define e(a,b) ap->a = mproc->b
	e(a_flag, p_flag); e(a_stat, p_stat); e(a_nice, p_nice);
	e(a_uid, p_suid);
	e(a_pid, p_pid); e(a_pri, p_pri);
	e(a_slptime, p_slptime); e(a_time, p_time);
	ap->a_tty[0] = ttyp[0];
	ap->a_tty[1] = ttyp[1] ? ttyp[1] : ' ';
	if (ap->a_stat == SZOMB) {
		ap->a_cpu = 0;
		ap->a_ttyd = s.s_ttyd;
	} else {
		ap->a_size = mproc->p_dsize + mproc->p_ssize;
		e(a_rss, p_rssize); 
		ap->a_ttyd = s.s_ttyd;
		ap->a_cpu = u->u_ru.ru_utime.tv_sec + u->u_ru.ru_stime.tv_sec;
		if (sumcpu)
			ap->a_cpu += u->u_cru.ru_utime.tv_sec + u->u_cru.ru_stime.tv_sec;
		ap->a_start = u->u_start.tv_sec;
	}
#undef e
	ap->a_maxrss = mproc->p_maxrss;
	if (jflg) {
		register struct jsav *jp;
 
		sp->s_un.jp = jp = (struct jsav *)
			calloc(1, sizeof (struct jsav));
		if (jp == NULL) {
			fprintf(stderr, "ps: out of memory allocating jsav\n");
			exit(1);
		}
 
		jp->j_ppid = mproc->p_ppid;
		jp->j_pgrp = mproc->p_pgrp;
		jp->j_sid = s.s_sid;
		if (s.s_ttyp) {
		    kvm_read(kvm_des, s.s_ttyp, &jp->j_tpgrp, sizeof(pid_t));
		}
		else
			jp->j_tpgrp = -1;
	}
	else if (lflg) {
		register struct lsav *lp;
 
		sp->s_un.lp = lp = (struct lsav *)
			calloc(1, sizeof (struct lsav));
		if (lp == NULL) {
			fprintf(stderr, "ps: out of memory allocating lsav\n");
			exit(1);
		}
#define e(a,b) lp->a = mproc->b
		e(l_ppid, p_ppid); 
		e(l_cpu, p_cpu);
		if (ap->a_stat != SZOMB)
			e(l_wchan, p_wchan);
#undef e
	} else if (vflg) {
		register struct vsav *vp;
 
		sp->s_un.vp = vp = (struct vsav *)
			calloc(1, sizeof (struct vsav));
		if (vp == NULL) {
			fprintf(stderr, "ps: out of memory allocating vsav\n");
			exit(1);
		}
#define e(a,b) vp->a = mproc->b
		if (ap->a_stat != SZOMB) {
			e(v_swrss, p_swrss);
			vp->v_majflt = u->u_ru.ru_majflt;
		}
		vp->v_pctcpu = pcpu();
#undef e
	} else if (uflg)
		sp->s_un.u_pctcpu = pcpu();
 
	npr++;
}
 
/*
 * Calculate the percentage of memory to charge to this process.
 */
double
pmem(ap)
	register struct asav *ap;
{
	double fracmem;
	static int totpages, havepages;
 
	/*
	 * Lazy evaluation.
	 */
	if (havepages == 0) {
		totpages = (struct page *)getw(nl[X_EPAGES].n_value) -
			(struct page *)getw(nl[X_PAGES].n_value);
		havepages = 1;
	}
 
	/*
	 * The second clause guards against nlist botches and such.
	 */
	if ((ap->a_flag & SLOAD) == 0 || totpages <= 0)
		fracmem = 0.0;
	else {
		/*
		 * Calculate an approximation to the process's
		 * memory consumption.  Background information:
		 * 1) We estimate the total pool of available memory
		 *    by calculating (epages-pages).  When the VM
		 *    system is generalized to support noncontiguous
		 *    physical memory, this will have to change.
		 * 2) This difference is an overestimate, since some
		 *    of these pages can go to the kernel and aren't
		 *    actually available for user processes.
		 * 3) We use ap->a_rss as the measure of the process's
		 *    memory demand.  This neglects all shared segments,
		 *    such as shared libraries.  The rss itself isn't
		 *    particularly accurate either -- this is a deficiency
		 *    of the initial implementation of the new VM system.
		 * 4) We completely neglect u-area pages in the calculations.
		 */
		fracmem = ((double)ap->a_rss) / totpages;
	}
 
	return (100.0 * fracmem);
}
 
double
pcpu()
{
	time_t time;
	double p;
 
	time = mproc->p_time;
	if (time == 0 || (mproc->p_flag&SLOAD) == 0)
		return (0.0);
	p = (double)mproc->p_pctcpu / FSCALE;
	if (rawcpu)
		return (100.0 * p);
	return (100.0 * p / (1.0 - exp(time * log(ccpu))));
}
 
char *
getcmd()
{
	char **proc_argv = NULL;
	char **proc_environ = NULL;
	register char **argp;
	register char *ap;
	register char *cp, *ep;
	register int c;
	int nbad;
	char tempbuf[sizeof(u->u_comm)+10];
 
	bzero(cmdbuf, twidth+1);
	if (mproc->p_stat == SZOMB || mproc->p_flag&(SSYS|SWEXIT))
		return ("");
	if (cflg) {
		(void) strncpy(cmdbuf, u->u_comm, sizeof (u->u_comm));
		return (savestr(cmdbuf));
	}
	if (kvm_getcmd(kvm_des, mproc, u, &proc_argv, eflg ? &proc_environ : NULL) < 0) {
		(void) strcpy(cmdbuf, " (");
		(void) strncat(cmdbuf, u->u_comm, sizeof (u->u_comm));
		(void) strcat(cmdbuf, ")");
	} else {
		cp = cmdbuf;
		ep = cmdbuf + twidth;
		argp = proc_argv;
		nbad = 0;
		while (((ap = *argp++) != NULL) && (cp < ep)) {
			while (((c = (unsigned char)*ap++) != 0) && 
							    (cp < ep - 1)) {
				if (!isprint(c)) {
					if (++nbad >= 5)
						break;
					*cp = '?';
				} else
					*cp++ = c;
			}
			*cp++ = ' ';
		}
		if (eflg) {
			argp = proc_environ;
			nbad = 0;
			while (((ap = *argp++) != NULL) && (cp < ep)) {
				while (((c = (unsigned char)*ap++) != 0) && 
							      (cp < ep - 1)) {
					if (!isprint(c)) {
						if (++nbad >= 5)
							break;
						*cp = '?';
					} else
						*cp++ = c;
				}
				*cp++ = ' ';
			}
		}
		*cp = 0;
		while (*--cp == ' ')
			*cp = 0;
		if (*proc_argv == NULL || cmdbuf[0] == '-'
		    || cmdbuf[0] == '?' || cmdbuf[0] <= ' ') {
			(void) strcpy(tempbuf, " (");
			(void) strncat(tempbuf, u->u_comm, sizeof(u->u_comm));
			(void) strcat(tempbuf, ")");
			(void) strncat(cmdbuf, tempbuf, ep-cp);
		}
		if (proc_argv != NULL)
			free(proc_argv);
		if (proc_environ != NULL)
			free(proc_environ);
	}
	return (savestr(cmdbuf));
}
 
char	*jhdr =
" PPID   PID  PGID   SID TT TPGID  STAT   UID  TIME";
jpr(sp)
	struct savcom *sp;
{
	register struct asav *ap = sp->ap;
	register struct jsav *jp = sp->s_un.jp;
	char flg[10];
	char *s = flg;
 
	strcpy(flg, state(ap));
	for (s=flg; isalpha(*s); s++)
	    ;
	if (ap->a_flag & SNOCLDSTOP)
	    *s++ = 'C';
	if (ap->a_flag & SORPHAN)
	    *s++ = 'O';
	if (ap->a_flag & SEXECED)
	    *s++ = 'E';
	*s = 0;
	printf("%5u%6u%6u%6d ", 
	    jp->j_ppid, ap->a_pid, jp->j_pgrp, jp->j_sid);
	ptty(ap->a_tty);
	printf("%6d %5s%6d", jp->j_tpgrp, flg, ap->a_uid);
	ptime(ap);
}
 
char	*lhdr =
"       F UID   PID  PPID CP PRI NI  SZ  RSS %*s STAT TT  TIME";
lpr(sp)
	struct savcom *sp;
{
	register struct asav *ap = sp->ap;
	register struct lsav *lp = sp->s_un.lp;
 
	printf("%8x%4d%6u%6u%3d%4d%3d%4d%5d",
	    ap->a_flag, ap->a_uid,
	    ap->a_pid, lp->l_ppid, lp->l_cpu&0377, ap->a_pri-PZERO,
	    ap->a_nice-NZERO, pgtok(ap->a_size), pgtok(ap->a_rss));
	if (lp->l_wchan == 0)
		printf(" %*s", wcwidth, "");
	else if (nflg)
		printf(" %*x", wcwidth, (int)lp->l_wchan&0xffffffff);
	else
		printf(" %*.*s", wcwidth, abs(wcwidth), getchan(lp->l_wchan));
	printf(" %4.4s ", state(ap));
	ptty(ap->a_tty);
	ptime(ap);
}
 
ptty(tp)
	char *tp;
{
 
	printf("%-2.2s", tp);
}
 
ptime(ap)
	struct asav *ap;
{
 
	printf("%3ld:%02ld", ap->a_cpu / 60, ap->a_cpu % 60);
}
 
char	*uhdr =
"%s   PID %%CPU %%MEM   SZ  RSS TT STAT START  TIME";
upr(sp)
	struct savcom *sp;
{
	register struct asav *ap = sp->ap;
	char *cp;
	int vmsize, rmsize;
 
	vmsize = pgtok(ap->a_size);
	rmsize = pgtok(ap->a_rss);
	if (nflg)
		printf("%4d ", ap->a_uid);
	else
		printf("%-8.8s ", getname(ap->a_uid));
	printf("%5d%5.1f%5.1f%5d%5d",
	    ap->a_pid, sp->s_un.u_pctcpu, pmem(ap), vmsize, rmsize);
	putchar(' ');
	ptty(ap->a_tty);
	printf(" %4.4s", state(ap));
	if (ap->a_start == 0)
		ap->a_start = boottime.tv_sec;
	cp = ctime(&ap->a_start);
	if ((now - ap->a_start) > 60*60*24)
		printf("%.7s", cp+3);
	else
		printf("%.6s ", cp+10);
	ptime(ap);
}
 
/*
 * Removed 9 columns by zapping the (meaningless in SunOS 4.0) TSIZ and TRS
 * fields.  Consider using this space to widen currently-cramped columns.
 */
char *vhdr =
"  PID TT STAT  TIME SL RE PAGEIN SIZE  RSS   LIM %CPU %MEM";
vpr(sp)
	struct savcom *sp;
{
	register struct vsav *vp = sp->s_un.vp;
	register struct asav *ap = sp->ap;
 
	printf("%5u ", ap->a_pid);
	ptty(ap->a_tty);
	printf(" %4.4s", state(ap));
	ptime(ap);
	printf("%3d%3d%7d%5d%5d",
	   ap->a_slptime > 99 ? 99 : ap-> a_slptime,
	   ap->a_time > 99 ? 99 : ap->a_time, vp->v_majflt,
	   pgtok(ap->a_size), pgtok(ap->a_rss));
	if (ap->a_maxrss == (RLIM_INFINITY/NBPG))
		printf("    xx");
	else
		printf("%6d", pgtok(ap->a_maxrss));
	printf("%5.1f%5.1f", vp->v_pctcpu, pmem(ap));
}
 
char	*shdr =
"  PID TT STAT  TIME";
spr(sp)
	struct savcom *sp;
{
	register struct asav *ap = sp->ap;
 
	printf("%5u", ap->a_pid);
	putchar(' ');
	ptty(ap->a_tty);
	printf(" %4.4s", state(ap));
	ptime(ap);
}
 
char *
state(ap)
	register struct asav *ap;
{
	char stat, load, nice, anom;
	static char res[5];
 
	switch (ap->a_stat) {
 
	case SSTOP:
		stat = 'T';
		break;
 
	case SSLEEP:
		if (ap->a_pri >= PZERO)
			if (ap->a_slptime >= MAXSLP)
				stat = 'I';
			else
				stat = 'S';
		else if (ap->a_flag & SPAGE)
			stat = 'P';
		else
			stat = 'D';
		break;
 
	case SWAIT:
	case SRUN:
	case SIDL:
		stat = 'R';
		break;
 
	case SZOMB:
		stat = 'Z';
		break;
 
	default:
		stat = '?';
	}
	load = ap->a_flag & SLOAD ? (ap->a_rss>ap->a_maxrss ? '>' : ' ') : 'W';
	if (ap->a_nice < NZERO)
		nice = '<';
	else if (ap->a_nice > NZERO)
		nice = 'N';
	else
		nice = ' ';
	anom = (ap->a_flag&SUANOM) ? 'A' : ((ap->a_flag&SSEQL) ? 'S' : ' ');
	res[0] = stat; res[1] = load; res[2] = nice; res[3] = anom;
	return (res);
}
 
 
pscomp(s1, s2)
	struct savcom *s1, *s2;
{
	register int i;
 
	if (uflg)
		return (s2->s_un.u_pctcpu > s1->s_un.u_pctcpu ? 1 : -1);
	if (vflg)
		return (vsize(s2) - vsize(s1));
	i = s1->ap->a_ttyd - s2->ap->a_ttyd;
	if (i == 0)
		i = s1->ap->a_pid - s2->ap->a_pid;
	return (i);
}
 
vsize(sp)
	struct savcom *sp;
{
	register struct asav *ap = sp->ap;
	register struct vsav *vp = sp->s_un.vp;
	
	if (ap->a_flag & SLOAD)
		return (ap->a_rss);
	return (vp->v_swrss);
}
 
#include <utmp.h>
 
struct	utmp utmp;
#define	NMAX	(sizeof (utmp.ut_name))
#define SCPYN(a, b)	strncpy(a, b, NMAX)
 
#define NUID	64
 
struct ncache {
	int	uid;
	char	name[NMAX+1];
} nc[NUID];
 
/*
 * This function assumes that the password file is hashed
 * (or some such) to allow fast access based on a uid key.
 */
char *
getname(uid)
{
	register struct passwd *pw;
	struct passwd *getpwent();
	register int cp;
 
#if	(((NUID) & ((NUID) - 1)) != 0)
	cp = uid % (NUID);
#else
	cp = uid & ((NUID) - 1);
#endif
	if (uid >= 0 && nc[cp].uid == uid && nc[cp].name[0])
		return (nc[cp].name);
	pw = getpwuid(uid);
	if (!pw)
		return (0);
	nc[cp].uid = uid;
	SCPYN(nc[cp].name, pw->pw_name);
	return (nc[cp].name);
}
 
char *
savestr(cp)
	char *cp;
{
	register unsigned len;
	register char *dp;
 
	len = strlen(cp);
	dp = (char *)calloc(len+1, sizeof (char));
	if (dp == NULL) {
		fprintf(stderr, "ps: savestr: out of memory\n");
		exit(1);
	}
	(void) strcpy(dp, cp);
	return (dp);
}
 
/*
 * since we can't init unions, the cleanest way to use a.out.h instead
 * of nlist.h (required since nlist() uses some defines) is to do a
 * runtime copy into the nl array -- sigh
 */
init_nlist()
{
	register struct nlist *np;
	register char **namep;
 
	nllen = sizeof nl_names / sizeof (char *);
	np = nl = (struct nlist *) malloc(nllen * sizeof (struct nlist));
	if (np == NULL) {
		fprintf(stderr, "ps: out of memory allocating namelist\n");
		exit(1);
	}
	namep = &nl_names[0];
	while (nllen > 0) {
		np->n_un.n_name = *namep;
		if (**namep == '\0')
			break;
		namep++;
		np++;
	}
}
 
/*
 * nlist - retreive attributes from name list (string table version)
 *         [The actual work is done in _nlist.c]
 */
nlist(name, list)
	char *name;
	struct nlist *list;
{
	register int fd;
	register int e;
 
	fd = open(name, O_RDONLY, 0);
	e = _nlist(fd, list);
	close(fd);
	return (e);
}
 
/*
 * _nlist - retreive attributes from name list (string table version)
 * 	modified to add wait channels - Charles R. LaBrec 8/85
 */
_nlist(fd, list)
	int fd;
	struct nlist *list;
{
	register struct nlist *p, *q;
	register int soff;
	register int stroff = 0;
	register n, m;
	int maxlen, nreq;
	off_t sa;		/* symbol address */
	off_t ss;		/* start of strings */
	int type;
	struct exec buf;
	struct nlist space[BUFSIZ/sizeof (struct nlist)];
	char strs[BUFSIZ];
 
	maxlen = 0;
	for (q = list, nreq = 0; q->n_un.n_name && q->n_un.n_name[0]; q++, nreq++) {
		q->n_type = 0;
		q->n_value = 0;
		q->n_desc = 0;
		q->n_other = 0;
		n = strlen(q->n_un.n_name);
		if (n > maxlen)
			maxlen = n;
	}
	if ((fd == -1) || (lseek(fd, 0L, 0) == -1) ||
	    (read(fd, (char*)&buf, sizeof buf) != sizeof buf) || N_BADMAG(buf))
		return (-1);
	sa = N_SYMOFF(buf);
	ss = sa + buf.a_syms;
	n = buf.a_syms;
	while (n) {
		m = MIN(n, sizeof (space));
		lseek(fd, sa, 0);
		if (read(fd, (char *)space, m) != m)
			break;
		sa += m;
		n -= m;
		for (q = space; (m -= sizeof(struct nlist)) >= 0; q++) {
			soff = q->n_un.n_strx;
			if (soff == 0 || q->n_type & N_STAB)
				continue;
			/*
			 * since we know what type of symbols we will get,
			 * we can make a quick check here -- crl
			 */
			type = q->n_type & (N_TYPE | N_EXT);
			if ((q->n_type & N_TYPE) != N_ABS
			    && type != (N_EXT | N_DATA)
			    && type != (N_EXT | N_BSS))
				continue;
 
			if ((soff + maxlen + 1) >= stroff) {
				/*
				 * Read strings into local cache.
				 * Assumes (maxlen < sizeof (strs)).
				 */
				lseek(fd, ss+soff, 0);
				read(fd, strs, sizeof strs);
				stroff = soff + sizeof (strs);
			}
			/* if using wchans, add it to the list of channels */
			if (!nflg && !gotwchans)
				addchan(&strs[soff-(stroff-sizeof (strs))+1],
					(caddr_t) q->n_value);
			for (p = list; p->n_un.n_name && p->n_un.n_name[0]; p++) {
				if (p->n_type == 0 &&
				    strcmp(p->n_un.n_name,
				    &strs[soff-(stroff-sizeof (strs))]) == 0) {
					p->n_value = q->n_value;
					p->n_type = q->n_type;
					p->n_desc = q->n_desc;
					p->n_other = q->n_other;
					if (--nreq == 0 && (nflg || gotwchans))
						goto alldone;
					break;
				}
			}
		}
	}
alldone:
	return (nreq);
}
 
/*
 * add the given channel to the channel list
 */
addchan(name, caddr)
char *name;
caddr_t caddr;
{
	static int left = 0;
	register struct wchan *wp;
	register char **p;
	register struct wchan_map *mp;
 
	for (p = wchan_stop_list; *p; p++) {
		if (**p != *name)	/* quick check first */
			continue;
		if (strncmp(name, *p, WNAMESIZ) == 0)
			return;		/* if found, don't add */
	}
	for (mp = wchan_map_list; mp->map_from; mp++) {
		if (*(mp->map_from) != *name)	/* quick check first */
			continue;
		if (strncmp(name, mp->map_from, WNAMESIZ) == 0)
			name = mp->map_to;	/* if found, remap */
	}
	if (left == 0) {
		if (wchanhd) {
			left = 100;
			wchanhd = (struct wchan *) realloc(wchanhd,
				(nchans + left) * sizeof (struct wchan));
		} else {
			left = 600;
			wchanhd = (struct wchan *) malloc(left
				* sizeof (struct wchan));
		}
		if (wchanhd == NULL) {
			fprintf(stderr, "ps: out of memory allocating wait channels\n");
			nflg++;
			return;
		}
	}
	left--;
	wp = &wchanhd[nchans++];
	strncpy(wp->wc_name, name, WNAMESIZ);
	wp->wc_name[WNAMESIZ] = '\0';
	wp->wc_caddr = caddr;
}
 
/*
 * returns the symbolic wait channel corresponding to chan
 */
char *
getchan(chan)
register caddr_t chan;
{
	register i, iend;
	register char *prevsym;
	register struct wchan *wp;
 
	prevsym = "???";		/* nothing, to begin with */
	if (chan) {
		for (i = 0; i < NWCINDEX; i++)
			if ((unsigned) chan < (unsigned) wchan_index[i])
				break;
		iend = i--;
		if (i < 0)		/* can't be found */
			return prevsym;
		iend *= nchans;
		iend /= NWCINDEX;
		i *= nchans;
		i /= NWCINDEX;
		wp = &wchanhd[i];
		for ( ; i < iend; i++, wp++) {
			if ((unsigned) wp->wc_caddr > (unsigned) chan)
				break;
			prevsym = wp->wc_name;
		}
	}
	return prevsym;
}
 
/*
 * used in sorting the wait channel array
 */
int
wchancomp (w1, w2)
struct wchan *w1, *w2;
{
	register unsigned c1, c2;
 
	c1 = (unsigned) w1->wc_caddr;
	c2 = (unsigned) w2->wc_caddr;
	if (c1 > c2)
		return 1;
	else if (c1 == c2)
		return 0;
	else
		return -1;
}
 
usage()
{
	fprintf(stderr, "ps: usage: ps [-acCegjklnrStuvwxU] [num] [kernel_name] [c_dump_file] [swap_file]\n");
}

