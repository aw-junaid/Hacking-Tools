/*+
 *  Modified ls command to mask certain files from being
 *  listed with ls command.  *NOT FULLY TESTED*
+*/

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
static	char sccsid[] = "@(#)ls.c 1.1 91/11/13 SMI"; /* from UCB 5.6 5/12/86 */
#endif not lint

/*
 * ls
 *
 * 4.2bsd version for symbolic links, variable length
 * directory entries, block size in the inode, etc.
 */
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdio.h>
#include <sgtty.h>
#include <errno.h>

#include <ctype.h>
#include <locale.h>

#include "config.h"

#define	kbytes(size)	(((size) + 1023) / 1024)

struct afile {
	char	ftype;		/* file type, e.g. 'd', 'c', 'f' */
	ino_t	fnum;		/* inode number of file */
	short	fflags;		/* mode&~S_IFMT, perhaps ISARG */
	short	fnl;		/* number of links */
	uid_t	fuid;		/* owner id */
	gid_t	fgid;		/* group id */
	long	fsize;		/* file size */
	long	fblks;		/* number of blocks used */
	time_t	fmtime;		/* time (modify or access or create) */
	char	*fname;		/* file name */
	char	*flinkto;	/* symbolic link value */
};

#define	ISARG	0x8000		/* extra ``mode'' */

struct subdirs {
	char	*sd_name;
	struct	subdirs *sd_next;
} *subdirs;

int	aflg, dflg, gflg, lflg, sflg, tflg, uflg, iflg, fflg, cflg, rflg = 1;
int	qflg, Aflg, Cflg, Fflg, Lflg, Rflg;

int	usetabs;

time_t	now, sixmonthsago, onehourfromnow;

char	*dotp = ".";

struct	winsize win;
int	twidth;

struct	afile *gstat();
int	fcmp();
char	*cat(), *savestr();
char	*fmtentry();
static char	*getname(), *getgroup();

char	*ctime();
char	*malloc(), *calloc(), *realloc();
char	*sprintf(), *strcpy(), *strcat();

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
int	showall=0;

/*+  End hack vars  +*/

main(argc, argv)
	int argc;
	char *argv[];
{
	int i;
	struct afile *fp0, *fplast;
	register struct afile *fp;
	struct sgttyb sgbuf;

	argc--, argv++;
	if (getuid() == 0)
		Aflg++;
	(void) time(&now);
	sixmonthsago = now - 6L*30L*24L*60L*60L;
	onehourfromnow = now + 60L*60L;
	now += 60;
	twidth = 80;
	if (isatty(1)) {
		qflg = Cflg = 1;
		(void) gtty(1, &sgbuf);
		if (ioctl(1, TIOCGWINSZ, &win) != -1)
			twidth = (win.ws_col == 0 ? 80 : win.ws_col);
		if ((sgbuf.sg_flags & XTABS) != XTABS)
			usetabs = 1;
	} else
		usetabs = 1;

	setlocale(LC_ALL, "");			/* get local environment */

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

	while (argc > 0 && **argv == '-') {
		(*argv)++;
		while (**argv) switch (*(*argv)++) {

		case 'C':
			Cflg = 1; break;
		case 'q':
			qflg = 1; break;
		case '1':
			Cflg = 0; break;
		case 'a':
			aflg++; break;
		case 'A':
			Aflg++; break;
		case 'c':
			cflg++; break;
		case 's':
			sflg++; break;
		case 'd':
			dflg++; break;
		case 'g':
			gflg++; break;
		case 'l':
			lflg++; break;
		case 'r':
			rflg = -1; break;
		case 't':
			tflg++; break;
		case 'u':
			uflg++; break;
		case 'i':
			iflg++; break;
		case 'f':
			fflg++; break;
		case 'L':
			Lflg++; break;
		case 'F':
			Fflg++; break;
		case 'R':
			Rflg++; break;
#if defined (SHOWFLAG)
                case '/':
                        showall++;
                        break;
#endif
 
		}
		argc--, argv++;
	}
	if (fflg) {
		aflg++; lflg = 0; sflg = 0; tflg = 0;
	}
	if (lflg)
		Cflg = 0;
	if (argc == 0) {
		argc++;
		argv = &dotp;
	}
	fp = (struct afile *)calloc(argc, sizeof (struct afile));
	if (fp == 0) {
		fprintf(stderr, "ls: out of memory\n");
		exit(1);
	}
	fp0 = fp;
	for (i = 0; i < argc; i++) {
		if (gstat(fp, *argv, 1, (int *)0)) {
			fp->fname = *argv;
			fp->fflags |= ISARG;
			fp++;
		}
		argv++;
	}
	fplast = fp;
	qsort(fp0, fplast - fp0, sizeof (struct afile), fcmp);
	if (dflg) {
		formatf(fp0, fplast);
		exit(0);
	}
	if (fflg)
		fp = fp0;
	else {
		for (fp = fp0; fp < fplast && fp->ftype != 'd'; fp++)
			continue;
		formatf(fp0, fp);
	}
	if (fp < fplast) {
		if (fp > fp0)
			printf("\n");
		for (;;) {
			formatd(fp->fname, argc > 1);
			while (subdirs) {
				struct subdirs *t;

				t = subdirs; subdirs = t->sd_next;
				printf("\n");
				formatd(t->sd_name, 1);
				cfree(t->sd_name);
				cfree((char *)t);
			}
			if (++fp == fplast)
				break;
			printf("\n");
		}
	}
	exit(0);
	/* NOTREACHED */
}

formatd(name, title)
	char *name;
	int title;
{
	register struct afile *fp;
	register struct subdirs *dp;
	struct afile *dfp0, *dfplast;
	int nkb;

	nkb = getdir(name, &dfp0, &dfplast);
	if (dfp0 == 0)
		return;
	if (fflg == 0)
		qsort(dfp0, dfplast - dfp0, sizeof (struct afile), fcmp);
	if (title)
		printf("%s:\n", name);
	if (lflg || sflg)
		printf("total %ld\n", nkb);
	formatf(dfp0, dfplast);
	if (Rflg)
		for (fp = dfplast - 1; fp >= dfp0; fp--) {
			if (fp->ftype != 'd' ||
			    !strcmp(fp->fname, ".") ||
			    !strcmp(fp->fname, ".."))
				continue;
			dp = (struct subdirs *)malloc(sizeof (struct subdirs));
			dp->sd_name = savestr(cat(name, fp->fname));
			dp->sd_next = subdirs; subdirs = dp;
		}
	for (fp = dfp0; fp < dfplast; fp++) {
		if ((fp->fflags&ISARG) == 0 && fp->fname)
			cfree(fp->fname);
		if (fp->flinkto)
			cfree(fp->flinkto);
	}
	cfree((char *)dfp0);
}

getdir(dir, pfp0, pfplast)
	char *dir;
	struct afile **pfp0, **pfplast;
{
	register struct afile *fp;
	DIR *dirp;
	register struct direct *dp;
	int nb, nent = 20;

	/*
	 * This code (opendir, readdir, and the "for" loop) is arranged in
	 * this strange manner to handle the case where UNIX lets root open
	 * any directory for reading, but NFS does not let root read the
	 * opened directory.
	 */
	*pfp0 = *pfplast = NULL;
	if ((dirp = opendir(dir)) == NULL) {
		printf("%s unreadable\n", dir);		/* not stderr! */
		return (0);
	}
	errno = 0;
	if (((dp = readdir(dirp)) == NULL) && (errno != 0)) {
		/* root reading across NFS can get to this error case */
		printf("%s unreadable\n", dir);		/* not stderr! */
		closedir(dirp);
		return (0);
	}
	fp = *pfp0 = (struct afile *)calloc(nent, sizeof (struct afile));
	*pfplast = *pfp0 + nent;
	nb = 0;
	for (; dp != NULL; dp = readdir(dirp)) {
		if (dp->d_ino == 0) 
			continue; 

/*+
 *  mask out the files in the list we read in earlier.  if
 *  showflag is >0 then we should not do any masking.  Note
 *  that each time a file is read from the structure it is
 *  compared.  If you have 1000 files in your ls blockout
 *  file, this might go a bit slow.
 *
 *  Note, I don't like continue but its an easy work around.
+*/	
		if (!showall)
	                for (h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next) 
				if (!(strcmp(dp->d_name, h_tmp->filename)))
					break;

		/*  More kludge - bleah  */
		if (!(strcmp(dp->d_name, h_tmp->filename)))
			continue;
				
	

		if (aflg == 0 && dp->d_name[0]=='.' &&
		    (Aflg == 0 || dp->d_name[1]==0 ||
			dp->d_name[1]=='.' && dp->d_name[2]==0))
				continue;
		if (gstat(fp, cat(dir, dp->d_name), Fflg+Rflg, &nb) == 0)
			continue;
		fp->fnum = dp->d_ino;
		fp->fname = savestr(dp->d_name);
		fp++;
		if (fp == *pfplast) {
			*pfp0 = (struct afile *)realloc((char *)*pfp0,
			    2 * nent * sizeof (struct afile));
			if (*pfp0 == 0) {
				fprintf(stderr, "ls: out of memory\n");
				exit(1);
			}
			fp = *pfp0 + nent;
			*pfplast = fp + nent;
			nent *= 2;
		}
	}
	closedir(dirp);
	*pfplast = fp;
	return (kbytes(dbtob(nb)));
}

int	stat(), lstat();

struct afile *
gstat(fp, file, statarg, pnb)
	register struct afile *fp;
	char *file;
	int statarg;
	int *pnb;	/* (int *)0 if file is ISARG */
{
	int (*statf)() = Lflg ? stat : lstat;
	char buf[BUFSIZ]; int cc;
	static struct afile azerofile;

	*fp = azerofile;
	fp->fflags = 0;
	fp->fnum = 0;
	fp->ftype = '-';
	if (statarg || sflg || lflg || tflg || cflg) {
		struct stat stb, stb1;

		if ((*statf)(file, &stb) < 0) {
			if (statf == lstat || lstat(file, &stb) < 0) {
				if (errno == ENOENT)
					fprintf(stderr, "%s not found\n", file);
				else {
					fprintf(stderr, "ls: ");
					perror(file);
				}
				return (0);
			}
		}
		fp->fblks = stb.st_blocks;
		fp->fsize = stb.st_size;
		switch (stb.st_mode & S_IFMT) {

		case S_IFDIR:
			fp->ftype = 'd'; break;
		case S_IFBLK:
			fp->ftype = 'b'; fp->fsize = stb.st_rdev; break;
		case S_IFCHR:
			fp->ftype = 'c'; fp->fsize = stb.st_rdev; break;
		case S_IFSOCK:
			fp->ftype = 's'; fp->fsize = 0; break;
		case S_IFIFO:
			fp->ftype = 'p'; fp->fsize = 0; break;
		case S_IFLNK:
			fp->ftype = 'l';
			if (lflg) {
				cc = readlink(file, buf, BUFSIZ);
				if (cc >= 0) {
					/*
					 * here we follow the symbolic
					 * link to generate the proper
					 * Fflg marker for the object,
					 * eg, /bin -> /pub/bin/
					 */
					buf[cc] = 0;
					if (Fflg && !stat(buf, &stb1))
						switch (stb1.st_mode & S_IFMT){
						case S_IFDIR:
							buf[cc++] = '/';
							break;
						case S_IFSOCK:
							buf[cc++] = '=';
							break;
						default:
						if ((stb1.st_mode & ~S_IFMT)
						    & 0111)
							buf[cc++] = '*';
							break;
						}
					buf[cc] = 0;
					fp->flinkto = savestr(buf);
				}
				break;
			}
			/*
			 *  this is a hack from UCB to avoid having
			 *  ls /bin behave differently from ls /bin/
			 *  when /bin is a symbolic link.  We hack the
			 *  hack to have that happen, but only for
			 *  explicit arguments, by inspecting pnb.
			 */
			if (pnb != (int *)0 || stat(file, &stb1) < 0)
				break;
			if ((stb1.st_mode & S_IFMT) == S_IFDIR) {
				stb = stb1;
				fp->ftype = 'd';
				fp->fsize = stb.st_size;
				fp->fblks = stb.st_blocks;
			}
			break;
		}
		fp->fnum = stb.st_ino;
		fp->fflags = stb.st_mode & ~S_IFMT;
		fp->fnl = stb.st_nlink;
		fp->fuid = stb.st_uid;
		fp->fgid = stb.st_gid;
		if (uflg)
			fp->fmtime = stb.st_atime;
		else if (cflg)
			fp->fmtime = stb.st_ctime;
		else
			fp->fmtime = stb.st_mtime;
		if (pnb)
			*pnb += stb.st_blocks;
	}
	return (fp);
}

formatf(fp0, fplast)
	struct afile *fp0, *fplast;
{
	register struct afile *fp;
	int width = 0, w, nentry = fplast - fp0;
	int i, j, columns, lines;
	char *cp;

	if (fp0 == fplast)
		return;
	if (lflg || Cflg == 0)
		columns = 1;
	else {
		for (fp = fp0; fp < fplast; fp++) {
			int len = strlen(fmtentry(fp));

			if (len > width)
				width = len;
		}
		if (usetabs)
			width = (width + 8) &~ 7;
		else
			width += 2;
		columns = twidth / width;
		if (columns == 0)
			columns = 1;
	}
	lines = (nentry + columns - 1) / columns;
	for (i = 0; i < lines; i++) {
		for (j = 0; j < columns; j++) {
			fp = fp0 + j * lines + i;
			cp = fmtentry(fp);
			printf("%s", cp);
			if (fp + lines >= fplast) {
				printf("\n");
				break;
			}
			w = strlen(cp);
			while (w < width)
				if (usetabs) {
					w = (w + 8) &~ 7;
					putchar('\t');
				} else {
					w++;
					putchar(' ');
				}
		}
	}
}

fcmp(f1, f2)
	register struct afile *f1, *f2;
{

	if (dflg == 0 && fflg == 0) {
		if ((f1->fflags&ISARG) && f1->ftype == 'd') {
			if ((f2->fflags&ISARG) == 0 || f2->ftype != 'd')
				return (1);
		} else {
			if ((f2->fflags&ISARG) && f2->ftype == 'd')
				return (-1);
		}
	}
	if (tflg || cflg) {
		if (f2->fmtime == f1->fmtime)
			return (0);
		if (f2->fmtime > f1->fmtime)
			return (rflg);
		return (-rflg);
	}
	return (rflg * strcmp(f1->fname, f2->fname));
}

char *
cat(dir, file)
	char *dir, *file;
{
	static char dfile[BUFSIZ];

	if (strlen(dir)+1+strlen(file)+1 > BUFSIZ) {
		fprintf(stderr, "ls: filename too long\n");
		exit(1);
	}
	if (!strcmp(dir, "") || !strcmp(dir, ".")) {
		(void) strcpy(dfile, file);
		return (dfile);
	}
	(void) strcpy(dfile, dir);
	if (dir[strlen(dir) - 1] != '/' && *file != '/')
		(void) strcat(dfile, "/");
	(void) strcat(dfile, file);
	return (dfile);
}

char *
savestr(str)
	char *str;
{
	char *cp = malloc(strlen(str) + 1);

	if (cp == NULL) {
		fprintf(stderr, "ls: out of memory\n");
		exit(1);
	}
	(void) strcpy(cp, str);
	return (cp);
}

char	*fmtinum(), *fmtsize(), *fmtlstuff(), *fmtmode();

char *
fmtentry(fp)
	register struct afile *fp;
{
	static char fmtres[BUFSIZ];
	register char *cp, *dp;

	(void) sprintf(fmtres, "%s%s%s",
	    iflg ? fmtinum(fp) : "",
	    sflg ? fmtsize(fp) : "",
	    lflg ? fmtlstuff(fp) : "");
	dp = &fmtres[strlen(fmtres)];
	for (cp = fp->fname; *cp; cp++)
		if (qflg && !isprint((unsigned char)*cp))
			*dp++ = '?';
		else
			*dp++ = *cp;
	/* avoid both "->" and trailing marks */
	if (Fflg && ! (lflg && fp->flinkto)) {
		if (fp->ftype == 'd')
			*dp++ = '/';
		else if (fp->ftype == 'l')
			*dp++ = '@';
		else if (fp->ftype == 's')
			*dp++ = '=';
		else if (fp->fflags & 0111)
			*dp++ = '*';
	}
	if (lflg && fp->flinkto) {
		(void) strcpy(dp, " -> "); dp += 4;
		for (cp = fp->flinkto; *cp; cp++)
			if (qflg && !isprint((unsigned char) *cp))
				*dp++ = '?';
			else
				*dp++ = *cp;
	}
	*dp++ = 0;
	return (fmtres);
}

char *
fmtinum(p)
	register struct afile *p;
{
	static char inumbuf[8];

	(void) sprintf(inumbuf, "%6d ", p->fnum);
	return (inumbuf);
}

char *
fmtsize(p)
	register struct afile *p;
{
	static char sizebuf[32];

	(void) sprintf(sizebuf, "%4ld ", kbytes(dbtob(p->fblks)));
	return (sizebuf);
}

char *
fmtlstuff(p)
	register struct afile *p;
{
	static char lstuffbuf[256];
	char gname[32], uname[32], fsize[32], ftime[32];
	register char *lp = lstuffbuf;

	/* type mode uname gname fsize ftime */
/* get uname */
	{ char *cp = getname(p->fuid);
	    (void) sprintf(uname, "%-9.9s", cp);
	}
/* get gname */
	if (gflg) {
	    char *cp = getgroup(p->fgid);
		(void) sprintf(gname, "%-9.9s", cp);
	}
/* get fsize */
	if (p->ftype == 'b' || p->ftype == 'c')
		(void) sprintf(fsize, "%3d,%4d",
		    major(p->fsize), minor(p->fsize));
	else if (p->ftype == 's')
		(void) sprintf(fsize, "%8ld", 0);
	else
		(void) sprintf(fsize, "%8ld", p->fsize);
/* get ftime */
	{ char *cp = ctime(&p->fmtime);
	    if ((p->fmtime < sixmonthsago) || (p->fmtime > onehourfromnow))
		(void) sprintf(ftime, " %-7.7s %-4.4s ", cp+4, cp+20);
	    else
		(void) sprintf(ftime, " %-12.12s ", cp+4);
	}
/* splat */
	*lp++ = p->ftype;
	lp = fmtmode(lp, p->fflags);
	(void) sprintf(lp, "%3d %s%s%s%s",
	    p->fnl, uname, gflg ? gname : "", fsize, ftime);
	return (lstuffbuf);
}

int	m1[] = { 1, S_IREAD>>0, 'r', '-' };
int	m2[] = { 1, S_IWRITE>>0, 'w', '-' };
int	m3[] = { 3, S_ISUID|(S_IEXEC>>0), 's', S_IEXEC>>0, 'x', S_ISUID, 'S',
	'-' };
int	m4[] = { 1, S_IREAD>>3, 'r', '-' };
int	m5[] = { 1, S_IWRITE>>3, 'w', '-' };
int	m6[] = { 3, S_ISGID|(S_IEXEC>>3), 's', S_IEXEC>>3, 'x', S_ISGID, 'S',
	'-' };
int	m7[] = { 1, S_IREAD>>6, 'r', '-' };
int	m8[] = { 1, S_IWRITE>>6, 'w', '-' };
int	m9[] = { 3, S_ISVTX|(S_IEXEC>>6), 't', S_IEXEC>>6, 'x', S_ISVTX, 'T',
	'-'};

int	*m[] = { m1, m2, m3, m4, m5, m6, m7, m8, m9};

char *
fmtmode(lp, flags)
	char *lp;
	int flags;
{
	int **mp;

	for (mp = &m[0]; mp < &m[sizeof (m)/sizeof (m[0])]; ) {
		register int *pairp = *mp++;
		register int n = *pairp++;

		while (n-- > 0) {
			if ((flags&*pairp) == *pairp) {
				pairp++;
				break;
			} else
				pairp += 2;
		}
		*lp++ = *pairp;
	}
	return (lp);
}

/* rest should be done with nameserver or database */

#include <pwd.h>
#include <grp.h>
#include <utmp.h>

struct	utmp utmp;

#define	NMAX	(sizeof (utmp.ut_name))
#define	SCPYN(a, b)	strncpy(a, b, NMAX)


struct cachenode {		/* this struct must be zeroed before using */
	struct cachenode *lesschild;	/* subtree whose entries < val */
	struct cachenode *grtrchild;	/* subtree whose entries > val */
	int val;			/* the uid or gid of this entry */
	int initted;			/* name has been filled in */
	char name[NMAX+1];		/* the string that val maps to */
} *names, *groups;

static struct cachenode *
findincache(head, val)
	struct cachenode **head;
	register int val;
{
	register struct cachenode **parent = head;
	register struct cachenode *c = *parent;

	while (c != NULL) {
		if (val == c->val) {
			/* found it */
			return (c);
		} else if (val < c->val) {
			parent = &c->lesschild;
			c = c->lesschild;
		} else {
			parent = &c->grtrchild;
			c = c->grtrchild;
		}
	}

	/* not in the cache, make a new entry for it */
	*parent = c = (struct cachenode *) calloc(1, sizeof (struct cachenode));
	c->val = val;
	return (c);
}

static char *
getname(uid)
	int uid;
{
	register struct cachenode *c;
	struct passwd *pw;

	c = findincache(&names, uid);
	if (c->initted == 0) {
		if ((pw = getpwuid(uid)) != NULL) {
			SCPYN(&c->name[0], pw->pw_name);
		} else {
			(void) sprintf(&c->name[0], "%-8d", uid);
		}
		c->initted = 1;
	}
	return (&c->name[0]);
}

static char *
getgroup(gid)
	int gid;
{
	register struct cachenode *c;
	struct group *gr;

	c = findincache(&groups, gid);
	if (c->initted == 0) {
		if ((gr = getgrgid(gid)) != NULL) {
			SCPYN(&c->name[0], gr->gr_name);
		} else {
			(void) sprintf(&c->name[0], "%-8d", gid);
		}
		c->initted = 1;
	}
	return (&c->name[0]);
}
