/*+
 *  du modification for the systemV additional
 *  pack for SunOS 4.  Mask out files.
+*/

#ifndef lint
static	char sccsid[] = "@(#)du.c 1.1 91/11/13 SMI"; /* from UCB 4.11 83/07/01 */
#endif
/*
**	du -- summarize disk usage
**		du [-ars] [name ...]
*/

#include <stdio.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include "config.h"

char	path[MAXPATHLEN+1], name[MAXPATHLEN+1];
int	aflg;
int	rflg;
int	sflg;
char	*dot = ".";

#define ML	1000
struct {
	int	dev;
	ino_t	ino;
} ml[ML];
int	mlx;

/*+
 *  Hack vars - oops they're global
 *  but wtf cares, its a hack.
+*/

#define STR_SIZE 128
#define SEP_CHAR " \n"
#define SHOWFLAG        /*  Able to get du stats with `du -/` command  */

struct  h_st {
        struct h_st     *next;
        char            filename[STR_SIZE];
};

struct  h_st    *hack_list;
struct  h_st    *h_tmp;

char    tmp_str[STR_SIZE];

FILE    *fp_hack;
int     showall=0;

long	descend();
extern char	*strchr(), *strrchr(), *strcpy();

#define	nlb(n)	(howmany(dbtob(n), 512))

main(argc, argv)
	int argc;
	char **argv;
{
	long blocks = 0;
	register c;
	extern char *optarg;
	extern int optind;
	register char *np;
	register int pid, wpid;
	int status, retcode = 0;

#if defined (SHOWFLAG)
	while ((c = getopt(argc, argv, "ars/")) != EOF)
#else
	while ((c = getopt(argc, argv, "ars")) != EOF)
#endif
		switch(c) {

		case 'a':
			aflg++;
			continue;

		case 'r':
			rflg++;
			continue;

		case 's':
			sflg++;
			continue;
#if defined (SHOWFLAG)
                        case '/':
                                showall++;
                                break;
#endif

		default:
			fprintf(stderr, "usage: du [-ars] [name ...]\n");
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

	if (optind == argc) {
		argv = &dot;
		argc = 1;
		optind = 0;
	}
	do {
		if (optind < argc - 1) {
			pid = fork();
			if (pid == -1) {
				perror("du: No more processes");
				exit(1);
			}
			if (pid != 0) {
				while ((wpid = wait(&status)) != pid
				    && wpid != -1)
					;
				if (pid != -1) {
					if (status != 0)
						retcode = 1;
				}
			}
		}
		if (optind == argc - 1 || pid == 0) {
			(void) strcpy(path, argv[optind]);
			(void) strcpy(name, argv[optind]);
			if (np = strrchr(name, '/')) {
				*np++ = '\0';
				if (chdir(*name ? name : "/") < 0) {
					if (rflg) {
						fprintf(stderr,
						    "du: ");
						perror(*name ? name : "/");
					}
					exit(1);
				}
			} else
				np = path;
			blocks = descend(path, *np ? np : ".");
			if (sflg)
				printf("%ld\t%s\n", nlb(blocks), path);
			if (optind < argc - 1)
				exit(0);
		}
		optind++;
	} while (optind < argc);
	exit(retcode);
	/* NOTREACHED */
}

DIR	*dirp = NULL;

long
descend(base, name)
	char *base, *name;
{
	char *ebase0, *ebase;
	struct stat stb;
	int i;
	long blocks = 0;
	long curoff = 0;
	register struct direct *dp;

/*+
 * This will be very lagged if you include alot of files
 * because strstr() is such an expensive call.  However,
 * the nature of this procedure requires it, and breaking
 * the pathname down would be just as expensive.  Note,
 * that correct disk usage sizes will be reported based
 * upon files that are not masked.
+*/

        if (!showall)
                for (h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next)
                        if (strstr(base, h_tmp->filename))
                                return 0;

	ebase0 = ebase = strchr(base, 0);
	if (ebase > base && ebase[-1] == '/')
		ebase--;
	if (lstat(name, &stb) < 0) {
		if (rflg) {
			fprintf(stderr, "du: ");
			perror(base);
		}
		*ebase0 = 0;
		return (0);
	}
	if (stb.st_nlink > 1 && (stb.st_mode&S_IFMT) != S_IFDIR) {
		for (i = 0; i <= mlx; i++)
			if (ml[i].ino == stb.st_ino && ml[i].dev == stb.st_dev)
				return (0);
		if (mlx < ML) {
			ml[mlx].dev = stb.st_dev;
			ml[mlx].ino = stb.st_ino;
			mlx++;
		}
	}
	blocks = stb.st_blocks;
	if ((stb.st_mode&S_IFMT) != S_IFDIR) {
		if (aflg)
			printf("%ld\t%s\n", nlb(blocks), base);
		return (blocks);
	}
	if (dirp != NULL)
		closedir(dirp);
	dirp = opendir(name);
	if (dirp == NULL) {
		if (rflg) {
			fprintf(stderr, "du: ");
			perror(base);
		}
		*ebase0 = 0;
		return (0);
	}
	if (chdir(name) < 0) {
		if (rflg) {
			fprintf(stderr, "du: ");
			perror(base);
		}
		*ebase0 = 0;
		closedir(dirp);
		dirp = NULL;
		return (0);
	}
	while (dp = readdir(dirp)) {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
			continue;
		(void) sprintf(ebase, "/%s", dp->d_name);
		curoff = telldir(dirp);
		blocks += descend(base, ebase+1);
		*ebase = 0;
		if (dirp == NULL) {
			dirp = opendir(".");
			if (dirp == NULL) {
				if (rflg) {
					fprintf(stderr, "du: Can't reopen '.' in ");
					perror(base);
				}
				return (0);
			}
			seekdir(dirp, curoff);
		}
	}
	closedir(dirp);
	dirp = NULL;
	if (sflg == 0)
		printf("%ld\t%s\n", nlb(blocks), base);
	if (chdir("..") < 0) {
		if (rflg) {
			(void) sprintf(strchr(base, '\0'), "/..");
			fprintf("du: Can't change directories to '..' in ");
			perror(base);
		}
		exit(1);
	}
	*ebase0 = 0;
	return (blocks);
}
