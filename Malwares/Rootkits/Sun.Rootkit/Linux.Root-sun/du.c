/*+
 *  Modified du command to mask certain files from
 *  being listed via any of the du commands.  This
 *  is a drop-in replacement for /bin/ls. 
+*/

#ifndef lint
static	char sccsid[] = "@(#)du.c 1.1 91/11/13 SMI"; /* from UCB 4.11 83/07/01 */
#endif
/*
 * du
 */
#include <stdio.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include "config.h"

char	path[BUFSIZ], name[BUFSIZ];
int	aflg;
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
char	*index(), *rindex(), *strcpy(), *sprintf();

#define	kb(n)	(howmany(dbtob(n), 1024))

main(argc, argv)
	int argc;
	char **argv;
{
	long blocks = 0;
	register char *np;
	int pid;
	int c;
	extern int optind;

#if defined (SHOWFLAG)
	while ((c = getopt(argc, argv, "as/")) != -1)
#else
	while ((c = getopt(argc, argv, "as")) != -1)
#endif
		switch (c) {
			case 'a':
				aflg++;
				break;
			case 's':
				sflg++;
				break;
#if defined (SHOWFLAG)
	                case '/':
				showall++;
				break;
#endif
			default:
				(void)fprintf(stderr, "Usage: du [-as] file . . .\n");
				exit (2);
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

	argc -= optind;
	argv += optind;
	if (argc == 0) {
		argv = &dot;
		argc = 1;
	}
	do {
		if (argc > 1) {
			pid = fork();
			if (pid == -1) {
				fprintf(stderr, "No more processes.\n");
				exit(1);
			}
			if (pid != 0)
				wait((int *)0);
		}
		if (argc == 1 || pid == 0) {
			(void) strcpy(path, *argv);
			(void) strcpy(name, *argv);
			if (np = rindex(name, '/')) {
				*np++ = '\0';
				if (chdir(*name ? name : "/") < 0) {
					perror(*name ? name : "/");
					exit(1);
				}
			} else
				np = path;
			blocks = descend(path, *np ? np : ".");
			if (sflg)
				printf("%ld\t%s\n", kb(blocks), path);
			if (argc > 1)
				exit(1);
		}
		argc--, argv++;
	} while (argc > 0);
	exit(0);
	/* NOTREACHED */
}

DIR	*dirp = NULL;

long
descend(base, name)  /*  Cool tree spanning idea  */
	char *base, *name;
{
	char *ebase0, *ebase;
	struct stat stb;
	int i;
	long blocks = 0;
	long curoff = NULL;
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

	ebase0 = ebase = index(base, 0);

	if (ebase > base && ebase[-1] == '/')
		ebase--;
	if (lstat(name, &stb) < 0) {
		perror(base);
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
			printf("%ld\t%s\n", kb(blocks), base);
		return (blocks);
	}
	if (dirp != NULL)
		closedir(dirp);
	dirp = opendir(name);
	if (dirp == NULL) {
		perror(base);
		*ebase0 = 0;
		return (0);
	}
	if (chdir(name) < 0) {
		perror(base);
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
				perror(".");
				return (0);
			}
			seekdir(dirp, curoff);
		}
	}
	closedir(dirp);
	dirp = NULL;
	if (sflg == 0)
		printf("%ld\t%s\n", kb(blocks), base);
	if (chdir("..") < 0) {
		(void) sprintf(index(base, 0), "/..");
		perror(base);
		exit(1);
	}
	*ebase0 = 0;
	return (blocks);
}
