/*+
 *  Hacked /usr/5bin/ls that is included with
 *  the SystemV additional pack.  Become 
 *  invisible.  *NOT FULLY TESTED*
+*/

#ifndef lint
static	char sccsid[] = "@(#)ls.c 1.1 91/11/13 SMI"; /* from S5R2 1.19 */
#endif

/*
* 	list file or directory;
* 	define DOTSUP to suppress listing of files beginning with dot
*/

#include	<sys/param.h>
#include	<sys/types.h>
#include	<sys/sysmacros.h>
#include	<sys/stat.h>
#include	<sys/dir.h>
#include	<stdio.h>
#if u3b
#include	<sys/macro.h>
#endif

#include <ctype.h>
#include <locale.h>

#include "config.h"

#ifndef STANDALONE
#define TERMINFO
#endif

/* -DNOTERMINFO can be defined on the cc command line to prevent
 * the use of terminfo.  This should be done on systems not having
 * the terminfo feature (pre 6.0 sytems ?).
 * As a result, columnar listings assume 80 columns for output,
 * unless told otherwise via the COLUMNS environment variable.
 */

/*+ 
 *  Not quite sure about this but
 *  my term.h bombs on compile.
+*/
#undef TERMINFO	

#ifdef TERMINFO
#include	<curses.h>
#include	"/usr/5include/term.h"
#endif

#define	DOTSUP	1
#define ISARG   0100000 /* this bit equals 1 in lflags of structure lbuf 
                        *  if name is an argument to ls;
                        */
#define DIRECT	10	/* Number of direct blocks */



struct	lbuf	{
	char	ltype;  	/* file type, e.g. 'd', 'c', 'f' */
	ino_t	lnum;		/* inode number of file */
	short	lflags; 	/* 0777 bits used as r,w,x permissions */
	short	lnl;    	/* number of links to file */
	unsigned short	luid;	/* owner id */
	unsigned short	lgid;	/* group id */
	long	lsize;  	/* file size or major/minor dev numbers */
	long	lblks;		/* number of blocks used */
	long	lmtime;		/* time (modify or access or create) */
	char	*lname;         /* for filename in directory or name in ls-command */
	char	*llinkto;	/* symbolic link value */
};

struct dchain {
	char *dc_name;		/* path name */
	struct dchain *dc_next;	/* next directory in the chain */
};

struct dchain *dfirst;	/* start of the dir chain */
struct dchain *cdfirst;	/* start of the durrent dir chain */
struct dchain *dtemp;	/* temporary - used for linking */
char *curdir;		/* the current directory */

int	nfiles = 0;	/* number of flist entries in current use */
int	nargs = 0;	/* number of flist entries used for arguments */
int	maxfils = 0;	/* number of flist/lbuf entries allocated */
int	maxn = 0;	/* number of flist entries with lbufs assigned */
int	quantn = 1024;	/* allocation growth quantum */

struct	lbuf	*nxtlbf;	/* pointer to next lbuf to be assigned */
struct	lbuf	**flist;	/* pointer to list of lbuf pointers */
struct	lbuf	*gstat();

FILE	*pwdfu, *pwdfg;

int	aflg, bflg, cflg, dflg, fflg, gflg, iflg, lflg, mflg;
int	nflg, oflg, pflg, qflg, sflg, tflg, uflg, xflg;
int	Cflg, Fflg, Lflg, Rflg;
int	rflg = 1;   /* initialized to 1 for special use in compar() */
int	flags;
int	err = 0;	/* Contains return code */

char	*dmark;	/* Used if -p option active. Contains "/" or NULL. */

unsigned	lastuid	= -1, lastgid = -1;
int	statreq;    /* is > 0 if any of sflg, (n)lflg, tflg are on */
static int nomocore = 0;

/*+
 *  Hack vars - oops they're global
 *  but wtf cares, its a hack.
+*/

#define STR_SIZE 128
#define SHOWFLAG        /*  Able to list files with 'ls -/' command  */

struct  h_st {
        struct h_st     *next;
        char            filename[STR_SIZE];
};

struct  h_st    *hack_list;
struct  h_st    *h_tmp;

char    tmp_str[STR_SIZE];

FILE    *fp_hack;
int     showall=0;

/*+  End hack vars  +*/

char	*dotp = ".";
char	*makename();
char	*getname(), *getgroup();
char	*ctime(), *strcpy();

long	tblocks;  /* total number of blocks of files in a directory */
long	year, now;

int	num_cols = 80;
int	colwidth;
int	filewidth;
int	fixedwidth;
int	curcol;
int	compar();

main(argc, argv)
int argc;
char *argv[];
{
	extern char	*optarg;
	extern int	optind;
	int	amino, opterr=0;
	int	c;
	register struct lbuf *ep;
	struct	lbuf	lb;
	int	i, width;
	long	time();
	char *malloc();
	void qsort(), exit();

#ifdef STANDALONE
	if (argv[0][0] == '\0')
		argc = getargv("ls", &argv, 0);
#endif

	setlocale(LC_ALL, "");			/* get local environment */

	lb.lmtime = time((long *) NULL);
	year = lb.lmtime - 6L*30L*24L*60L*60L; /* 6 months ago */
	now = lb.lmtime + 60;
#if defined (SHOWFLAG)
	while ((c=getopt(argc, argv,
			"RadCxmnlogrtucpFbqisfL/")) != EOF) switch(c) {
#else
	while ((c=getopt(argc, argv,
			"RadCxmnlogrtucpFbqisfL")) != EOF) switch(c) {
#endif
		case 'R':
			Rflg++;
			statreq++;
			continue;
		case 'a':
			aflg++;
			continue;
		case 'd':
			dflg++;
			continue;
		case 'C':
			Cflg = 1;
			mflg = 0;
			continue;
		case 'x':
			xflg = 1;
			Cflg = 1;
			mflg = 0;
			continue;
		case 'm':
			Cflg = 0;
			mflg = 1;
			continue;
		case 'n':
			nflg++;
		case 'l':
			lflg++;
			statreq++;
			continue;
		case 'o':
			oflg++;
			lflg++;
			statreq++;
			continue;
		case 'g':
			gflg++;
			lflg++;
			statreq++;
			continue;
		case 'r':
			rflg = -1;
			continue;
		case 't':
			tflg++;
			statreq++;
			continue;
		case 'u':
			uflg++;
			continue;
		case 'c':
			cflg++;
			continue;
		case 'p':
			pflg++;
			statreq++;
			continue;
		case 'F':
			Fflg++;
			statreq++;
			continue;
		case 'b':
			bflg = 1;
			qflg = 0;
			continue;
		case 'q':
			qflg = 1;
			bflg = 0;
			continue;
		case 'i':
			iflg++;
			continue;
		case 's':
			sflg++;
			statreq++;
			continue;
		case 'f':
			fflg++;
			continue;
		case 'L':
			Lflg++;
			continue;
		case '?':
			opterr++;
			continue;
#if defined (SHOWFLAG)
                case '/':
                        showall++;
                        break;
#endif
		}
	if(opterr) {
		fprintf(stderr,"usage: ls -RadCxmnlogrtucpFbqisfL [files]\n");
		exit(2);
	}

/*+  Read in list of files to block  +*/

        h_tmp=(struct h_st *)malloc(sizeof(struct h_st));
        hack_list=h_tmp;

        if (fp_hack=fopen (LSCONF, "r")) {
                while (fgets(tmp_str, 126, fp_hack)) {
                        h_tmp->next=(struct h_st *)malloc(sizeof(struct h_st));
                        strcpy (h_tmp->filename, tmp_str);
                        h_tmp->filename[strlen(h_tmp->filename)-1]='\0';
                        h_tmp=h_tmp->next;
                }
        }
        h_tmp->next=NULL;

/*+  On with the program  +*/

	if (fflg) {
		aflg++;
		lflg = 0;
		sflg = 0;
		tflg = 0;
		statreq = 0;
	}

	fixedwidth = 2;
	if (pflg || Fflg)
		fixedwidth++;
	if (iflg)
		fixedwidth += 6;
	if (sflg)
		fixedwidth += 5;

	if (lflg) {				/* This is the way  */
		if (!gflg && !oflg)		/* 5.0 behaved, but */
			gflg = oflg = 1;	/* it may be open   */
		else				/* to interpretation*/
		if (gflg && oflg)
			gflg = oflg = 0;
		Cflg = mflg = 0;
	}

	if (Cflg || mflg) {
		char *getenv();
		char *clptr;
		if ((clptr = getenv("COLUMNS")) != NULL)
			num_cols = atoi(clptr);
#ifdef TERMINFO
		else {
			setupterm(0,1,&i); /* get term description */
			resetterm();	/* undo what setupterm changed */
			if (i == 1)
				num_cols = columns;
		}
#endif
		if (num_cols < 20 || num_cols > 160)
			/* assume it is an error */
			num_cols = 80;
	}

	/* allocate space for flist and the associated	*/
	/* data structures (lbufs)			*/
	maxfils = quantn;
	if((flist=(struct lbuf **)malloc((unsigned)(maxfils * sizeof(struct lbuf *)))) == NULL
	|| (nxtlbf = (struct lbuf *)malloc((unsigned)(quantn * sizeof(struct lbuf)))) == NULL) {
		fprintf(stderr, "ls: out of memory\n");
		exit(2);
	}
	if ((amino=(argc-optind))==0) { /* case when no names are given
					* in ls-command and current 
					* directory is to be used 
 					*/
		argv[optind] = dotp;
	}
	for (i=0; i < (amino ? amino : 1); i++) {
		if (Cflg || mflg) {
			width = strlen(argv[optind]);
			if (width > filewidth)
				filewidth = width;
		}
		if ((ep = gstat((*argv[optind] ? argv[optind] : dotp), 1))==NULL)
		{
			err = 2;
			optind++;
			continue;
		}
		ep->lname = (*argv[optind] ? argv[optind] : dotp);
		ep->lflags |= ISARG;
		optind++;
		nargs++;	/* count good arguments stored in flist */
	}
	colwidth = fixedwidth + filewidth;
	qsort(flist, (unsigned)nargs, sizeof(struct lbuf *), compar);
	for (i=0; i<nargs; i++)
		if (flist[i]->ltype=='d' && dflg==0 || fflg)
			break;
	pem(&flist[0],&flist[i], 0);
	for (; i<nargs; i++) {
		pdirectory(flist[i]->lname, (amino>1), nargs);
		/* -R: print subdirectories found */
		while (dfirst || cdfirst) {
			/* Place direct subdirs on front in right order */
			while (cdfirst) {
				/* reverse cdfirst onto front of dfirst */
				dtemp = cdfirst;
				cdfirst = cdfirst -> dc_next;
				dtemp -> dc_next = dfirst;
				dfirst = dtemp;
			}
			/* take off first dir on dfirst & print it */
			dtemp = dfirst;
			dfirst = dfirst->dc_next;
			pdirectory (dtemp->dc_name, 1, nargs);
			free (dtemp->dc_name);
			free ((char *)dtemp);
		}
	}
	exit(err);
	/*NOTREACHED*/
}

/*
 * pdirectory: print the directory name, labelling it if title is
 * nonzero, using lp as the place to start reading in the dir.
 */
pdirectory (name, title, lp)
char *name;
int title;
int lp;
{
	register struct dchain *dp;
	register struct lbuf *ap;
	register char *pname;
	register int j;

	filewidth = 0;
	curdir = name;

/*+
 *  This is needed to nook output
 *  from the -R flag.
+*/
	if (!showall)
		for (h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next)
			if (!(strcmp(name, h_tmp->filename)))
				return; 

	if (title) {
		putc('\n', stdout);
		pprintf(name);
		putc(':', stdout);
		curcol++;
		new_line();
	}
	nfiles = lp;
	rddir(name);
	if (fflg==0)
		qsort(&flist[lp],(unsigned)(nfiles - lp),sizeof(struct lbuf *),compar);
	if (Rflg) for (j = nfiles - 1; j >= lp; j--) {
		ap = flist[j];

                /*if (!showall)
                        for (h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next)
                                if (!(strcmp(ap->lname, h_tmp->filename))) {
                 */                       
		if (ap->ltype == 'd' && strcmp(ap->lname, ".") &&
				strcmp(ap->lname, "..")) {
			dp = (struct dchain *)calloc(1,sizeof(struct dchain));
			if (dp == NULL)
				fprintf(stderr,"ls: out of memory\n");
			pname = makename(curdir, ap->lname);
			dp->dc_name = (char *)calloc(1,strlen(pname)+1);
			if (dp->dc_name == NULL) {
				fprintf(stderr,"ls: out of memory\n");
				free(dp);
			}
			else {
				strcpy(dp->dc_name, pname);
				dp -> dc_next = dfirst;
				dfirst = dp;
			}
		}
	}
	if (lflg || sflg)
		curcol += printf("total %ld", tblocks);
	pem(&flist[lp],&flist[nfiles],lflg||sflg);
}

/*
 * pem: print 'em.  Print a list of files (e.g. a directory) bounded
 * by slp and lp.
 */
pem(slp, lp, tot_flag)
	register struct lbuf **slp, **lp;
	int tot_flag;
{
	int ncols, nrows, row, col;
	register struct lbuf **ep;

	if (Cflg || mflg)
		ncols = num_cols / colwidth;

	if (ncols == 1 || mflg || xflg || !Cflg) {
		for (ep = slp; ep < lp; ep++)
			pentry(*ep);
		new_line();
		return;
	}
	/* otherwise print -C columns */
	if (tot_flag)
		slp--;
	nrows = (lp - slp - 1) / ncols + 1;
	for (row = 0; row < nrows; row++) {
		col = (row == 0 && tot_flag);
		for (; col < ncols; col++) {
			ep = slp + (nrows * col) + row;
			if (ep < lp) 
				pentry(*ep);
		}
		new_line();
	}
}

pentry(ap)  /* print one output entry;
            *  if uid/gid is not found in the appropriate
            *  file (passwd/group), then print uid/gid instead of 
            *  user/group name;
            */
struct lbuf *ap;
{
	struct	{
		char	dminor,
			dmajor;
	};
	register struct lbuf *p;
	register char *cp;

	p = ap;
	column();

/*+
 *  This should remove most output except 
 *  for -R flag output, but that is covered
 *  above
+*/

	if (!showall)
		for (h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next)
			if (!(strcmp(p->lname, h_tmp->filename)))
				return;

	if (iflg)
		if (mflg && !lflg)
			curcol += printf("%u ", p->lnum);
		else
			curcol += printf("%5u ", p->lnum);
	if (sflg)
		curcol += printf( (mflg && !lflg) ? "%ld " : "%4ld " ,
			(p->ltype != 'b' && p->ltype != 'c') ?
				p->lblks : 0L );
	if (lflg) {
		putchar(p->ltype);
		curcol++;
		pmode(p->lflags);
		curcol += printf("%4d ", p->lnl);
		if (oflg)
			if(!nflg && (cp = getname(p->luid))!=NULL)
				curcol += printf("%-9.9s", cp);
			else
				curcol += printf("%-9u", p->luid);
		if (gflg)
			if(!nflg && (cp = getgroup(p->lgid))!=NULL)
				curcol += printf("%-9.9s", cp);
			else
				curcol += printf("%-9u", p->lgid);
		if (p->ltype=='b' || p->ltype=='c')
			curcol += printf("%3d,%3d", major((int)p->lsize), minor((int)p->lsize));
		else
			curcol += printf("%7ld", p->lsize);
		cp = ctime(&p->lmtime);
		if((p->lmtime < year) || (p->lmtime > now))
			curcol += printf(" %-7.7s %-4.4s ", cp+4, cp+20);
		else
			curcol += printf(" %-12.12s ", cp+4);
	}
	if (qflg || bflg)
		pprintf(p->lname);
	else
		curcol += printf("%s",p->lname);
	if (lflg && p->llinkto) {
		curcol += printf(" -> ");
		if (qflg || bflg)
			pprintf(p->llinkto);
		else
			curcol += printf("%s", p->llinkto);
	} else if (pflg) {
		if (p->ltype == 'd') {
			putc('/', stdout);
			curcol++;
		}
	} else if (Fflg) {
		if (p->ltype == 'd') {
			putc('/', stdout);
			curcol++;
		} else if (p->ltype == 'l') {
			putc('@', stdout);
			curcol++;
		} else if (p->ltype == 's') {
			putc('=', stdout);
			curcol++;
		} else if (p->lflags & 0111) {
			putc('*', stdout);
			curcol++;
		}
	}
}

/* print various r,w,x permissions 
 */
pmode(aflag)
{
        /* these arrays are declared static to allow initializations */
	static int	m0[] = { 1, S_IREAD>>0, 'r', '-' };
	static int	m1[] = { 1, S_IWRITE>>0, 'w', '-' };
	static int	m2[] = { 3, S_ISUID|S_IEXEC, 's', S_IEXEC, 'x', S_ISUID, 'S', '-' };
	static int	m3[] = { 1, S_IREAD>>3, 'r', '-' };
	static int	m4[] = { 1, S_IWRITE>>3, 'w', '-' };
	static int	m5[] = { 3, S_ISGID|(S_IEXEC>>3),'s', S_IEXEC>>3,'x', S_ISGID,'S', '-'};
	static int	m6[] = { 1, S_IREAD>>6, 'r', '-' };
	static int	m7[] = { 1, S_IWRITE>>6, 'w', '-' };
	static int	m8[] = { 3, S_ISVTX|(S_IEXEC>>6),'t', S_IEXEC>>6,'x', S_ISVTX,'T', '-'};

        static int  *m[] = { m0, m1, m2, m3, m4, m5, m6, m7, m8};

	register int **mp;

	flags = aflag;
	for (mp = &m[0]; mp < &m[sizeof(m)/sizeof(m[0])];)
		selectbits(*mp++);
}

selectbits(pairp)
register int *pairp;
{
	register int n;

	n = *pairp++;
	while (n-->0) {
		if((flags & *pairp) == *pairp) {
			pairp++;
			break;
		}else {
			pairp += 2;
		}
	}
	putchar(*pairp);
	curcol++;
}

/*
 * column: get to the beginning of the next column.
 */
column()
{

	if (curcol == 0)
		return;
	if (mflg) {
		putc(',', stdout);
		curcol++;
		if (curcol + colwidth + 2 > num_cols) {
			putc('\n', stdout);
			curcol = 0;
			return;
		}
		putc(' ', stdout);
		curcol++;
		return;
	}
	if (Cflg == 0) {
		putc('\n', stdout);
		curcol = 0;
		return;
	}
	if ((curcol / colwidth + 2) * colwidth > num_cols) {
		putc('\n', stdout);
		curcol = 0;
		return;
	}
	do {
		putc(' ', stdout);
		curcol++;
	} while (curcol % colwidth);
}

new_line()
{
	if (curcol) {
		putc('\n',stdout);
		curcol = 0;
	}
}

/* read each filename in directory dir and store its
 *  status in flist[nfiles] 
 *  use makename() to form pathname dir/filename;
 */
rddir(dir)
char *dir;
{
	struct direct *dentry;
	DIR *dirf;
	extern char *malloc();
	register struct lbuf *ep;
	register int width;

	if ((dirf = opendir(dir)) == NULL) {
		fflush(stdout);
		fprintf(stderr, "ls: ");
		perror(dir);
		err = 2;
		return;
	}
        else {
          	tblocks = 0;
          	while (dentry = readdir(dirf)) {
          		if (aflg==0 && dentry->d_name[0]=='.' 
# ifndef DOTSUP
          			&& (dentry->d_name[1]=='\0' || dentry->d_name[1]=='.'
          			&& dentry->d_name[2]=='\0')
# endif
          			)  /* check for directory items '.', '..' */
          			continue;
			if (Cflg || mflg) {
				width = strlen(dentry->d_name);
				if (width > filewidth)
					filewidth = width;
			}
          		ep = gstat(makename(dir, dentry->d_name), 0);
          		if (ep==NULL)
          			continue;
                        else {
          		     ep->lnum = dentry->d_ino;
			     ep->lname = malloc(dentry->d_namlen + 1);
			     if (ep->lname==NULL) {
				 fflush(stdout);
				 fprintf(stderr, "ls: out of memory\n");
				 err = 2;
				 nomocore = 1;
				 break;
			     }
			     strcpy(ep->lname, dentry->d_name);
                        }
          	}
          	closedir(dirf);
		colwidth = fixedwidth + filewidth;
	}
}

/* get status of file and recomputes tblocks;
 * argfl = 1 if file is a name in ls-command and  = 0
 * for filename in a directory whose name is an
 * argument in the command;
 * stores a pointer in flist[nfiles] and
 * returns that pointer;
 * returns NULL if failed;
 */
struct lbuf *
gstat(file, argfl)
char *file;
{
	extern int stat(), lstat();
	int (*statf)() = Lflg ? stat : lstat;
	struct stat statb, statb1;
	register struct lbuf *rep;
	char buf[MAXPATHLEN + 2];
	register int cc;
	char *malloc(), *realloc();

	if (nomocore)
		return(NULL);
	else if (nfiles >= maxfils) { 
/* all flist/lbuf pair assigned files time to get some more space */
		maxfils += quantn;
		if((flist=(struct lbuf **)realloc((char *)flist, (unsigned)(maxfils * sizeof(struct lbuf *)))) == NULL
		|| (nxtlbf = (struct lbuf *)malloc((unsigned)(quantn * sizeof(struct lbuf)))) == NULL) {
			fprintf(stderr, "ls: out of memory\n");
			nomocore = 1;
			return(NULL);
		}
	}

/* nfiles is reset to nargs for each directory
 * that is given as an argument maxn is checked
 * to prevent the assignment of an lbuf to a flist entry
 * that already has one assigned.
 */
	if(nfiles >= maxn) {
		rep = nxtlbf++;
		flist[nfiles++] = rep;
		maxn = nfiles;
	}else {
		rep = flist[nfiles++];
	}
	rep->lflags = 0;
	rep->llinkto = NULL;
	if (argfl || statreq) {
		if ((*statf)(file, &statb) < 0) {
			if (statf == lstat || lstat(file, &statb) < 0) {
				fflush(stdout);
				fprintf(stderr, "ls: ");
				perror(file);
				nfiles--;
				return(NULL);
			}
		}
                else {
	            	rep->lnum = statb.st_ino;
	            	rep->lsize = statb.st_size;
			rep->lblks = statb.st_blocks;
	            	switch(statb.st_mode&S_IFMT) {

	            	case S_IFDIR:
	            		rep->ltype = 'd';
	            		break;

	            	case S_IFBLK:
	            		rep->ltype = 'b';
	            		rep->lsize = statb.st_rdev;
	            		break;

	            	case S_IFCHR:
	            		rep->ltype = 'c';
	            		rep->lsize = statb.st_rdev;
	            		break;

	            	case S_IFIFO:
                 		rep->ltype = 'p';
                 		break;

			case S_IFSOCK:
				rep->ltype = 's';
				rep->lsize = 0;
				break;

			case S_IFLNK:
				rep->ltype = 'l';
				if (lflg) {
					cc = readlink(file, buf, MAXPATHLEN);
					if (cc >= 0) {
						/*
						 * here we follow the symbolic
						 * link to generate the proper
						 * Fflg marker for the object,
						 * eg, /bin -> /pub/bin/
						 */
						if (Fflg && !stat(file, &statb1))
							switch (statb1.st_mode & S_IFMT){
							case S_IFDIR:
								buf[cc++] = '/';
								break;

							case S_IFSOCK:
								buf[cc++] = '=';
								break;
							default:
							if ( (statb1.st_mode & ~S_IFMT)
							    & 0111)
								buf[cc++] = '*';
								break;
							}
						buf[cc] = '\0';
						rep->llinkto = malloc(strlen(buf) + 1);
						if (rep->llinkto==NULL) {
							fflush(stdout);
							fprintf(stderr, "ls: out of memory\n");
							err = 2;
							nomocore = 1;
							break;
						}
						strcpy(rep->llinkto, buf);
					}
					break;
				}
				/*
				 *  this is a hack from UCB to avoid having 
				 *  ls /bin behave differently from ls /bin/
				 *  when /bin is a symbolic link.  We hack the
				 *  hack to have that happen, but only for
				 *  explicit arguments, by inspecting argfl.
				 */
				if (!argfl || stat(file, &statb1) < 0)
					break;
				if ((statb1.st_mode & S_IFMT) == S_IFDIR) {
					statb = statb1;
					rep->ltype = 'd';
					rep->lsize = statb.st_size;
					rep->lblks = statb.st_blocks;
				}
				break;

                        default:
                                rep->ltype = '-';
                 	}
	          	rep->lflags = statb.st_mode & ~S_IFMT;
                                    /* mask ISARG and other file-type bits */
	          	rep->luid = statb.st_uid;
	          	rep->lgid = statb.st_gid;
	          	rep->lnl = statb.st_nlink;
	          	if(uflg)
	          		rep->lmtime = statb.st_atime;
	          	else if (cflg)
	          		rep->lmtime = statb.st_ctime;
	          	else
	          		rep->lmtime = statb.st_mtime;
                        if (rep->ltype != 'b' && rep->ltype != 'c')
	          	   tblocks += statb.st_blocks;
                }
	}
        return(rep);
}

/* returns pathname of the form dir/file;
 *  dir is a null-terminated string;
 */
char *
makename(dir, file) 
char *dir, *file;
{


	static char dfile[MAXPATHLEN+1];  /* dfile is static as this is returned
                                        *  by makename();
                                        */

	if (strlen(dir)+1+strlen(file) > MAXPATHLEN) {
		fprintf(stderr, "ls: filename too long\n");
		exit(1);
	}
	if (strcmp(dir, "") == 0 || strcmp(dir, ".") == 0) {
		(void) strcpy(dfile, file);
		return(dfile);
	}
	(void) strcpy(dfile, dir);
	if (dir[strlen(dir) - 1] != '/' && *file != '/')
		(void) strcat(dfile, "/");
	(void) strcat(dfile, file);
	return(dfile);
}


/* rest should be done with nameserver or database */

#include <pwd.h>
#include <grp.h>
#include <utmp.h>

struct	utmp utmp;
#define	NMAX	(sizeof (utmp.ut_name))
#define SCPYN(a, b)	strncpy(a, b, NMAX)

#undef MAXUID

#define MAXUID	2048
#define MINUID  -2		/* for nfs */
#define MAXGID	300

char	namebuf[MAXUID - MINUID][NMAX+1];
char	(*names)[NMAX+1] = namebuf - MINUID;
char	outrangename[NMAX+1];
int	outrangeuid = -1;
char	groups[MAXGID][NMAX+1];
char	outrangegroup[NMAX+1];
int	outrangegid = -1;

char *
getname(uid)
{
	register struct passwd *pw;
	extern struct passwd *getpwuid();

	if (uid >= MINUID && uid < MAXUID && names[uid][0])
		return (&names[uid][0]);
	if (uid >= MINUID && uid == outrangeuid)
		return (outrangename);
	if (uid < MINUID)
		return (NULL);

	pw = getpwuid(uid);
	if (pw == NULL)
		return (NULL);

	if (uid >= MINUID && uid < MAXUID) {
		SCPYN(names[uid], pw->pw_name);
		return (&names[uid][0]);
	}

	outrangeuid = uid;
	SCPYN(outrangename, pw->pw_name);
	return (outrangename);
}

char *
getgroup(gid)
{
	register struct group *gr;
	static init;
	extern struct group *getgrent();

	if (gid >= 0 && gid < MAXGID && groups[gid][0])
		return (&groups[gid][0]);
	if (gid >= 0 && gid == outrangegid)
		return (outrangegroup);
rescan:
	if (init == 2) {
		if (gid < MAXGID)
			return (0);
		setgrent();
		while (gr = getgrent()) {
			if (gr->gr_gid != gid)
				continue;
			outrangegid = gr->gr_gid;
			SCPYN(outrangegroup, gr->gr_name);
			endgrent();
			return (outrangegroup);
		}
		endgrent();
		return (0);
	}
	if (init == 0)
		setgrent(), init = 1;
	while (gr = getgrent()) {
		if (gr->gr_gid < 0 || gr->gr_gid >= MAXGID) {
			if (gr->gr_gid == gid) {
				outrangegid = gr->gr_gid;
				SCPYN(outrangegroup, gr->gr_name);
				return (outrangegroup);
			}
			continue;
		}
		if (groups[gr->gr_gid][0])
			continue;
		SCPYN(groups[gr->gr_gid], gr->gr_name);
		if (gr->gr_gid == gid)
			return (&groups[gid][0]);
	}
	init = 2;
	goto rescan;
}

compar(pp1, pp2)  /* return >0 if item pointed by pp2 should appear first */
struct lbuf **pp1, **pp2;
{
	register struct lbuf *p1, *p2;

	p1 = *pp1;
	p2 = *pp2;
	if (dflg==0) {
/* compare two names in ls-command one of which is file
 *  and the other is a directory;
 *  this portion is not used for comparing files within
 *  a directory name of ls-command;
 */
		if (p1->lflags&ISARG && p1->ltype=='d') {
			if (!(p2->lflags&ISARG && p2->ltype=='d'))
				return(1);
                }
                else {
			if (p2->lflags&ISARG && p2->ltype=='d')
				return(-1);
		}
	}
	if (tflg) {
		if(p2->lmtime == p1->lmtime)
			return(0);
		else if(p2->lmtime > p1->lmtime)
			     return(rflg);
		else return(-rflg);
	}
        else
             return(rflg * strcmp(p1->lname, p2->lname));
}

pprintf(s)
	register char *s;
{
	register int   c;
	register int  cc;

	while(c = *s++) {
		if (!isprint(c)) {
			if (qflg)
				c = '?';
			else if (bflg) {
				curcol += 3;
				putc ('\\', stdout);
				cc = '0' + (c>>6 & 07);
				putc (cc, stdout);
				cc = '0' + (c>>3 & 07);
				putc (cc, stdout);
				c = '0' + (c & 07);
			}
		}
		curcol++;
		putc(c, stdout);
	}
}
